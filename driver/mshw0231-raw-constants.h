/* SPDX-License-Identifier: GPL-2.0 */
#ifndef MSHW0231_RAW_CONSTANTS_H
#define MSHW0231_RAW_CONSTANTS_H

/* Signal detection thresholds */
#define HEATMAP_TOUCH_MIN_RISE      200
#define HEATMAP_TOUCH_MIN_ABSOLUTE  400
#define HEATMAP_BASELINE_FRAMES      30

/* Peak detection */
#define HEATMAP_MAX_PEAKS            16
#define HEATMAP_PEAK_RADIUS           5

/* CCL flood-fill */
#define HEATMAP_MIN_BLOB_PIXELS       2
#define HEATMAP_VELOCITY_REJECT_RADIUS  6

/* Blob splitting */
#define HEATMAP_SPLIT_MIN_PEAKS       2
#define HEATMAP_SPLIT_MIN_DIST         4
#define HEATMAP_SPLIT_RADIUS           2

/* Edge penalty (percent × 100) */
#define HEATMAP_EDGE_PENALTY_TOP      97
#define HEATMAP_EDGE_PENALTY_BOTTOM   23

/* Hungarian cost matrix */
#define HUNGARIAN_COST_IN_RANGE       10
#define HUNGARIAN_COST_OUT_RANGE     100
#define HUNGARIAN_COST_EMPTY        1000
#define HUNGARIAN_COST_SCALE         100
#define HUNGARIAN_JUMP_REJECT_MARGIN  200
/*
 * Track-continuity bias: subtracted from the cost of a currently
 * *claimed* (state==2) slot candidate so the assignment solver
 * prefers keeping an actively-tracked finger on its existing slot
 * over a marginally cheaper swap with another claimed slot (e.g.
 * during a pinch or rotate where two fingers cross paths).
 * FIRST-PASS VALUE, NOT EMPIRICALLY VALIDATED ON HARDWARE: chosen as
 * half of HUNGARIAN_COST_IN_RANGE as a conservative starting point;
 * needs tuning against real multi-finger gesture captures.
 */
#define HUNGARIAN_CONTINUITY_BONUS     5

/* Slot state machine */
#define HEATMAP_HOLD_RECOVERY_WEIGHT   4000
#define HEATMAP_DEADBAND_THRESHOLD      20
#define HEATMAP_STATIONARY_FRAMES         2

/* Missed frame timeout (ms) */
#define HEATMAP_MISSED_FRAME_TIMEOUT_MS  60

/* EMA smoothing default (position tracking; module-param tunable via ema_alpha) */
#define HEATMAP_EMA_ALPHA_DEFAULT         2

/* Blob weight EMA alpha: fixed at the Windows-verified value (a = 1/8, i.e.
 * weight = (old*7 + new)/8). This is a protocol-matching constant, not an
 * experimental knob — it must stay decoupled from HEATMAP_EMA_ALPHA_DEFAULT,
 * which module param ema_alpha tunes for position-smoothing experiments. */
#define HEATMAP_WEIGHT_EMA_ALPHA          7

/* Association radius multipliers per finger count (×10) */
#define ASSOC_RADIUS_1_FINGER           22
#define ASSOC_RADIUS_3_FINGERS          28
#define ASSOC_RADIUS_4_FINGERS          34
#define ASSOC_RADIUS_5_FINGERS          40

#endif
