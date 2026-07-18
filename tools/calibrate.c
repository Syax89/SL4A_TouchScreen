/* SPDX-License-Identifier: GPL-2.0 */
/* Retired raw-mode calibration prototype. */
#include <stdio.h>

int main(void)
{
	fputs("Raw calibration is disabled: the raw frame layout is unvalidated.\n",
	      stderr);
	fputs("Follow docs/RAW_MODE_VALIDATION.md before enabling coordinates.\n",
	      stderr);
	return 2;
}
