#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int32_t s32;
typedef int16_t s16;
typedef int64_t s64;
typedef uint8_t u8;

#define C590_BASE     10000
#define C590_STEP_NUM 22204
#define C590_STEP_DEN 1000
#define C590_OFFSET   6000

static s16 c590_lut[256];
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

static void compute_c590_lut(void)
{
	int i;
	for (i = 0; i < 256; i++) {
		s32 v = C590_BASE - ((s32)i * C590_STEP_NUM / C590_STEP_DEN + C590_OFFSET);
		c590_lut[i] = (s16)(v > 0 ? v : 0);
	}
}

static s32 atan2_approx(s32 y, s32 x)
{
	s32 ax = x < 0 ? -x : x;
	s32 ay = y < 0 ? -y : y;
	s32 angle;

	if (ax + ay == 0)
		return 0;

	angle = (s32)(((s64)ay * 9000) / (ax + ay));

	if (x < 0)
		angle = 18000 - angle;
	if (y < 0)
		angle = -angle;

	return angle;
}

static void test_c590_lut_values(void)
{
	compute_c590_lut();

	CHECK(c590_lut[0] == 4000, "c590[0] = %d, expected 4000", (int)c590_lut[0]);
	CHECK(c590_lut[255] == 0, "c590[255] = %d, expected 0", (int)c590_lut[255]);

	CHECK(c590_lut[0] > c590_lut[1], "c590 not monotonically decreasing at [0..1]");
	CHECK(c590_lut[127] > c590_lut[128], "c590 not monotonically decreasing at [127..128]");

	CHECK(c590_lut[0] >= c590_lut[255], "c590[0] < c590[255]");
	CHECK(c590_lut[64] > 1000, "c590[64]=%d too low (noise ceiling)", (int)c590_lut[64]);
	CHECK(c590_lut[128] > 0, "c590[128]=%d should be positive", (int)c590_lut[128]);

	CHECK(c590_lut[127] == (s16)(C590_BASE - (127 * C590_STEP_NUM / C590_STEP_DEN + C590_OFFSET)),
	      "c590[127] computed incorrectly");
	CHECK(c590_lut[0] == (s16)(C590_BASE - C590_OFFSET),
	      "c590[0] should be C590_BASE - C590_OFFSET = %d", C590_BASE - C590_OFFSET);

	CHECK(c590_lut[200] == 0, "c590[200] should reach zero floor (negatives clamped)");
}

static void test_atan2_quadrants(void)
{
	CHECK(atan2_approx(0, 100) == 0, "atan2(0, +x) = %d, expected 0", atan2_approx(0, 100));
	CHECK(atan2_approx(100, 100) == 4500,
	      "atan2(y=x) = %d, expected 4500 (45deg)", atan2_approx(100, 100));
	CHECK(atan2_approx(100, 0) == 9000,
	      "atan2(+y, 0) = %d, expected 9000 (90deg)", atan2_approx(100, 0));

	CHECK(atan2_approx(100, -100) == 13500,
	      "atan2(+y, -x) = %d, expected 13500 (135deg)", atan2_approx(100, -100));

	CHECK(atan2_approx(-100, 100) == -4500,
	      "atan2(-y, +x) = %d, expected -4500 (-45deg)", atan2_approx(-100, 100));
	CHECK(atan2_approx(-100, -100) == -13500,
	      "atan2(-y, -x) = %d, expected -13500 (-135deg)", atan2_approx(-100, -100));
	CHECK(atan2_approx(-100, 0) == -9000,
	      "atan2(-y, 0) = %d, expected -9000 (-90deg)", atan2_approx(-100, 0));
}

static void test_atan2_monotonic(void)
{
	s32 angles[8];
	int i;

	for (i = 0; i < 8; i++)
		angles[i] = atan2_approx(100, i * 10 + 10);

	for (i = 0; i < 7; i++)
		CHECK(angles[i] >= angles[i + 1],
		      "atan2 not monotonic decreasing with increasing x: angles[%d]=%d, angles[%d]=%d",
		      i, angles[i], i + 1, angles[i + 1]);
}

static void test_atan2_zero_input(void)
{
	CHECK(atan2_approx(0, 0) == 0, "atan2(0,0)=%d, expected 0", atan2_approx(0, 0));
	CHECK(atan2_approx(0, 1) == 0, "atan2(0,1)=%d, expected 0", atan2_approx(0, 1));
	CHECK(atan2_approx(0, -1) == 18000,
	      "atan2(0,-1)=%d, expected 18000 (180deg for -x axis)", atan2_approx(0, -1));
}

static void test_atan2_clamp(void)
{
	CHECK(atan2_approx(1000000, 0) == 9000, "atan2(+inf,0)=%d expected 9000", atan2_approx(1000000, 0));
	CHECK(atan2_approx(-1000000, 0) == -9000, "atan2(-inf,0)=%d expected -9000", atan2_approx(-1000000, 0));
	CHECK(atan2_approx(1000000, 1) >= 8999 && atan2_approx(1000000, 1) <= 9001,
	      "atan2 scale asymptote = %d (expected ~9000)", atan2_approx(1000000, 1));
}

static void test_atan2_small_values(void)
{
	CHECK(atan2_approx(1, 100) == 89, "atan2 small angle = %d", atan2_approx(1, 100));
	CHECK(atan2_approx(-1, 100) == -89, "atan2 small negative = %d", atan2_approx(-1, 100));
	CHECK(atan2_approx(1, 1) == 4500,
	      "atan2(1,1)=%d expected 4500", atan2_approx(1, 1));
}

int main(void)
{
	printf("c590_atan2_host_test: running...\n");

	test_c590_lut_values();
	test_atan2_quadrants();
	test_atan2_monotonic();
	test_atan2_zero_input();
	test_atan2_clamp();
	test_atan2_small_values();

	printf("c590_atan2_host_test: %d assertions, %d failures\n", passed + failed, failed);
	return failed != 0;
}
