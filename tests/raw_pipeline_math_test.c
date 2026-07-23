#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../driver/mshw0231-raw-constants.h"

typedef int32_t s32;
typedef uint32_t u32;
typedef uint16_t u16;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint8_t bool;
#define true 1
#define false 0

static int passed = 0;
static int failed = 0;

#define CHECK(cond, msg, ...) do { \
	if (!(cond)) { \
		fprintf(stderr, "FAIL: " msg "\n", ##__VA_ARGS__); \
		failed++; \
	} else { \
		passed++; \
	} \
} while (0)

/* ── Hungarian cost matrix build logic ───────────────────────────────── */

static u16 hungarian_cost_in_range(u32 dx, u32 dy)
{
	double dist = sqrt((double)(dx * dx + dy * dy));
	return (u16)(HUNGARIAN_COST_IN_RANGE * dist);
}

static u16 hungarian_cost_out_of_range(void) { return HUNGARIAN_COST_OUT_RANGE; }
static u16 hungarian_cost_empty_slot(void) { return HUNGARIAN_COST_EMPTY; }

static void test_hungarian_cost_computation(void)
{
	u16 c;

	c = hungarian_cost_in_range(0, 0);
	CHECK(c == 0, "in-range cost(0,0) = %u, expected 0", c);

	c = hungarian_cost_in_range(100, 0);
	CHECK(c == 1000, "in-range cost(100,0) = %u, expected 1000", c);
	CHECK(HUNGARIAN_COST_IN_RANGE == 10, "base cost multiplier is 10");

	c = hungarian_cost_out_of_range();
	CHECK(c == 100, "out-of-range cost = %u", c);

	c = hungarian_cost_empty_slot();
	CHECK(c == 1000, "empty slot cost = %u", c);

	CHECK(HUNGARIAN_COST_EMPTY > HUNGARIAN_COST_OUT_RANGE,
	      "empty slot cost (%u) > out-of-range (%u)", c, HUNGARIAN_COST_OUT_RANGE);
}

/* ── Association radius per finger count ─────────────────────────────── */

static u32 bmd_for_fingers(u32 bmd_base, int active_slots)
{
	if (active_slots == 1)      return bmd_base * ASSOC_RADIUS_1_FINGER / 10;
	else if (active_slots == 3) return bmd_base * ASSOC_RADIUS_3_FINGERS / 10;
	else if (active_slots == 4) return bmd_base * ASSOC_RADIUS_4_FINGERS / 10;
	else if (active_slots >= 5) return bmd_base * ASSOC_RADIUS_5_FINGERS / 10;
	return bmd_base;
}

static void test_association_radius_scaling(void)
{
	u32 base = 300; // blob_max_distance * 100 = 3 * 100

	CHECK(bmd_for_fingers(base, 0) == 300, "0 fingers: bmd=%u", bmd_for_fingers(base, 0));
	CHECK(bmd_for_fingers(base, 1) == 660, "1 finger: bmd=%u (×2.2)", bmd_for_fingers(base, 1));
	CHECK(bmd_for_fingers(base, 2) == 300, "2 fingers: bmd=%u (no scaling)", bmd_for_fingers(base, 2));
	CHECK(bmd_for_fingers(base, 3) == 840, "3 fingers: bmd=%u (×2.8)", bmd_for_fingers(base, 3));
	CHECK(bmd_for_fingers(base, 4) == 1020, "4 fingers: bmd=%u (×3.4)", bmd_for_fingers(base, 4));
	CHECK(bmd_for_fingers(base, 5) == 1200, "5+ fingers: bmd=%u (×4.0)", bmd_for_fingers(base, 5));
}

/* ── Ghost merge distance ────────────────────────────────────────────── */

static u32 ghost_dist_sq(int ghost_dist)
{
	return (u32)ghost_dist * (u32)ghost_dist * 10000u;
}

static int ghost_should_merge(u32 gx1, u32 gy1, u32 gx2, u32 gy2, u32 gdsq)
{
	u32 dx, dy;
	if (gx1 >= gx2) dx = gx1 - gx2; else dx = gx2 - gx1;
	if (gy1 >= gy2) dy = gy1 - gy2; else dy = gy2 - gy1;
	return dx * dx + dy * dy <= gdsq;
}

static void test_ghost_merge(void)
{
	u32 gdsq = ghost_dist_sq(6);
	CHECK(gdsq == 360000u, "ghost_dist_sq(6)=%u", gdsq);

	CHECK(ghost_should_merge(100, 100, 100, 100, gdsq),
	      "identical positions merge");
	CHECK(ghost_should_merge(100, 100, 105, 100, gdsq),
	      "within ghost_dist merge");

	gdsq = ghost_dist_sq(255);
	CHECK(gdsq == (u32)255 * 255 * 10000, "ghost_dist=255 sq no overflow");
}

/* ── EMA smoothing ───────────────────────────────────────────────────── */

static u32 ema_smooth(u32 old_val, u32 new_val, int alpha)
{
	return (old_val * alpha + new_val) / (alpha + 1);
}

static void test_ema_smoothing(void)
{
	CHECK(ema_smooth(0, 100, 0) == 100, "EMA alpha=0: instant follow");
	CHECK(ema_smooth(0, 100, 1) == 50, "EMA alpha=1: average");
	CHECK(ema_smooth(0, 100, 7) == 12, "EMA alpha=7: (%u*7+100)/8", ema_smooth(0, 100, 7));
	CHECK(ema_smooth(1000, 1000, 7) == 1000, "EMA stable input stays");
	CHECK(HEATMAP_EMA_ALPHA_DEFAULT == 7, "default EMA alpha is 7");
}

/* ── Deadband ────────────────────────────────────────────────────────── */

static u32 deadband_filter(u32 old, u32 new, u32 threshold)
{
	s32 diff = (s32)new - (s32)old;
	if (diff < 0) diff = -diff;
	return (u32)diff <= threshold ? old : new;
}

static void test_deadband(void)
{
	CHECK(deadband_filter(1000, 1050, HEATMAP_DEADBAND_THRESHOLD) == 1000,
	      "deadband: 50 <= 80, stays at old");
	CHECK(deadband_filter(1000, 1100, HEATMAP_DEADBAND_THRESHOLD) == 1100,
	      "deadband: 100 > 80, updates to new");
	CHECK(deadband_filter(1000, 920, HEATMAP_DEADBAND_THRESHOLD) == 1000,
	      "deadband: -80 <= 80, stays (negative within)");
	CHECK(deadband_filter(1000, 919, HEATMAP_DEADBAND_THRESHOLD) == 919,
	      "deadband: -81 > 80, updates (negative beyond)");
	CHECK(HEATMAP_DEADBAND_THRESHOLD == 80, "deadband threshold is 80");
}

/* ── Slot state machine ──────────────────────────────────────────────── */

enum { SLOT_EMPTY = 0, SLOT_NEW = 1, SLOT_CLAIMED = 2, SLOT_LIFT = 3, SLOT_HOLD = 4 };

struct slot_state {
	u8 state;
	u32 duration;
	u32 missed;
	int assigned_blob_idx;
	u32 blob_weight;
};

static void slot_fsm_tick(struct slot_state *s, int has_blob, u32 blob_weight,
			   int hold_frames, int blob_debounce, int blob_lift_frames)
{
	if (has_blob) {
		s->missed = 0;
		switch (s->state) {
		case SLOT_EMPTY:
		case SLOT_LIFT:
			s->state = SLOT_NEW;
			s->duration = 0;
			__attribute__((fallthrough));
		case SLOT_NEW:
			if (++s->duration >= (u32)blob_debounce)
				s->state = SLOT_CLAIMED;
			break;
		case SLOT_HOLD:
			if (blob_weight >= HEATMAP_HOLD_RECOVERY_WEIGHT) {
				s->state = SLOT_CLAIMED;
				s->duration = 0;
			}
			break;
		case SLOT_CLAIMED:
			s->duration++;
			break;
		}
	} else {
		s->missed++;
		switch (s->state) {
		case SLOT_NEW:
		case SLOT_EMPTY:
			s->state = SLOT_EMPTY;
			s->duration = 0;
			break;
		case SLOT_CLAIMED:
			if (hold_frames > 0 && s->missed <= (u32)hold_frames && s->blob_weight >= HEATMAP_HOLD_RECOVERY_WEIGHT)
				s->state = SLOT_HOLD;
			else
				s->state = SLOT_LIFT;
			s->duration = 0;
			break;
		case SLOT_HOLD:
			if (s->missed > (u32)hold_frames)
				s->state = SLOT_LIFT;
			break;
		case SLOT_LIFT:
			if (++s->duration >= (u32)blob_lift_frames)
				s->state = SLOT_EMPTY;
			break;
		}
	}
	if (has_blob) s->blob_weight = blob_weight;
}

static void test_slot_fsm(void)
{
	struct slot_state s;
	memset(&s, 0, sizeof(s));

	/* new → claimed (debounce 3 frames) */
	s.state = SLOT_EMPTY;
	slot_fsm_tick(&s, 1, 5000, 0, 3, 3);
	CHECK(s.state == SLOT_NEW, "tick1: empty→new");
	slot_fsm_tick(&s, 1, 5000, 0, 3, 3);
	CHECK(s.state == SLOT_NEW, "tick2: new (debouncing)");
	slot_fsm_tick(&s, 1, 5000, 0, 3, 3);
	CHECK(s.state == SLOT_CLAIMED, "tick3: new→claimed (debounced)");

	/* claimed → lift (blob goes away, no hold) */
	slot_fsm_tick(&s, 0, 0, 0, 3, 3);
	CHECK(s.state == SLOT_LIFT, "tick4: claimed→lift");

	/* lift → empty after lift_frames */
	slot_fsm_tick(&s, 0, 0, 0, 3, 3);
	CHECK(s.state == SLOT_LIFT, "tick5: lift (frame 1/3)");
	slot_fsm_tick(&s, 0, 0, 0, 3, 3);
	CHECK(s.state == SLOT_LIFT, "tick6: lift (frame 2/3)");
	slot_fsm_tick(&s, 0, 0, 0, 3, 3);
	CHECK(s.state == SLOT_EMPTY, "tick7: lift→empty");

	/* hold recovery: claimed→hold→claimed if weight ≥ 4000 */
	s.state = SLOT_CLAIMED; s.blob_weight = 5000; s.missed = 0; s.duration = 0;
	slot_fsm_tick(&s, 0, 0, 3, 3, 3);
	CHECK(s.state == SLOT_HOLD, "hold: claimed→hold (weight≥4000+hold_frames>0)");
	slot_fsm_tick(&s, 1, 5000, 3, 3, 3);
	CHECK(s.state == SLOT_CLAIMED, "hold recovery: hold→claimed (w≥4000)");

	/* stationary frame counter */
	CHECK(HEATMAP_STATIONARY_FRAMES == 6, "stationary lock at 6 frames");
	CHECK(HEATMAP_HOLD_RECOVERY_WEIGHT == 4000, "hold recovery weight=4000");
}

/* ── Grid→screen coordinate transform ────────────────────────────────── */

static u32 grid_to_screen(u32 gx, s32 scale, s32 offset, int invert, u32 screen_max)
{
	s32 screen = (s32)(((s64)gx * scale + 50000) / 100000) + offset;
	if (invert) screen = (s32)screen_max - screen;
	if (screen < 0) screen = 0;
	if (screen > (s32)screen_max) screen = (s32)screen_max;
	return (u32)screen;
}

static void test_grid_to_screen_transform(void)
{
	s32 scale_x = 32767 * 1000 / (72 - 1);
	u32 max_x = 32767;
	u32 val;

	CHECK(grid_to_screen(0, scale_x, 0, 0, max_x) == 0,
	      "grid 0 → screen 0");
	val = grid_to_screen(71, scale_x, 0, 0, max_x);
	CHECK(val > 0 && val <= 32767,
	      "grid 71 → screen %u (in range)", val);
	CHECK(grid_to_screen(35, scale_x, 0, 0, max_x) > 0,
	      "grid middle → screen positive");
	val = grid_to_screen(0, scale_x, 0, 1, max_x);
	CHECK(val <= 32767,
	      "grid 0 with invert → within range");
	CHECK(grid_to_screen(0, scale_x, 100, 0, max_x) == 100,
	      "offset +100 applied");
	CHECK(grid_to_screen(0, scale_x, -50000, 0, max_x) == 0,
	      "negative screen clamped to 0");
}

/* ── Edge penalty ────────────────────────────────────────────────────── */

static u32 apply_edge_penalty(u32 weight, int max_row, int blob_max_row)
{
	if (blob_max_row >= max_row - 1)
		return weight * HEATMAP_EDGE_PENALTY_BOTTOM / 100;
	if (blob_max_row <= 0)
		return weight * HEATMAP_EDGE_PENALTY_TOP / 100;
	return weight;
}

static void test_edge_penalty(void)
{
	CHECK(apply_edge_penalty(10000, 48, 47) == 2300,
	      "bottom edge penalty: 10000*23/100 = 2300");
	CHECK(apply_edge_penalty(10000, 48, 0) == 9700,
	      "top edge penalty: 10000*97/100 = 9700");
	CHECK(apply_edge_penalty(10000, 48, 24) == 10000,
	      "center no penalty");
	CHECK(HEATMAP_EDGE_PENALTY_BOTTOM == 23, "bottom penalty is 23%%");
	CHECK(HEATMAP_EDGE_PENALTY_TOP == 97, "top penalty is 97%%");
}

/* ── Signal thresholds ───────────────────────────────────────────────── */

static void test_signal_thresholds(void)
{
	CHECK(HEATMAP_TOUCH_MIN_RISE == 200, "min signal rise = 200");
	CHECK(HEATMAP_TOUCH_MIN_ABSOLUTE == 400, "min absolute c590 = 400");
	CHECK(HEATMAP_BASELINE_FRAMES == 30, "baseline frames = 30");
	CHECK(HEATMAP_MAX_PEAKS == 16, "max peaks = 16");
	CHECK(HEATMAP_MIN_BLOB_PIXELS == 2, "min blob pixels = 2");
	CHECK(HEATMAP_VELOCITY_REJECT_RADIUS == 6, "velocity reject radius = 6");
	CHECK(HEATMAP_SPLIT_MIN_DIST == 4, "split min distance = 4");
	CHECK(HEATMAP_MISSED_FRAME_TIMEOUT_MS == 60, "missed frame timeout = 60ms");
}

/* ── Jump rejection ──────────────────────────────────────────────────── */

static int should_reject_jump(u32 old_gx, u32 old_gy, u32 new_gx, u32 new_gy, u32 max_dist)
{
	u32 dx = old_gx > new_gx ? old_gx - new_gx : new_gx - old_gx;
	u32 dy = old_gy > new_gy ? old_gy - new_gy : new_gy - old_gy;
	u32 margin = HUNGARIAN_JUMP_REJECT_MARGIN;
	return dx > max_dist + margin || dy > max_dist + margin;
}

static void test_jump_rejection(void)
{
	CHECK(!should_reject_jump(1000, 1000, 1100, 1100, 300),
	      "within range: no reject");
	CHECK(should_reject_jump(1000, 1000, 2000, 1000, 300),
	      "dx=1000 > 300+200=500: rejected");
	CHECK(HUNGARIAN_JUMP_REJECT_MARGIN == 200, "jump reject margin = 200");
}

int main(void)
{
	printf("raw_pipeline_math_test: running...\n");

	test_hungarian_cost_computation();
	test_association_radius_scaling();
	test_ghost_merge();
	test_ema_smoothing();
	test_deadband();
	test_slot_fsm();
	test_grid_to_screen_transform();
	test_edge_penalty();
	test_signal_thresholds();
	test_jump_rejection();

	printf("raw_pipeline_math_test: %d assertions, %d failures\n", passed, failed);
	return failed != 0;
}
