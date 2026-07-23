/* SPDX-License-Identifier: GPL-2.0 */
#ifndef MSHW0231_RAW_H
#define MSHW0231_RAW_H

#include <linux/types.h>

struct spi_hid;

/*
 * Raw consumers receive payloads already read and gated by the transport.
 * They never access SPI, schedule work, or change the sequencer state.
 */
int mshw0231_raw_consume_v0(struct spi_hid *shid, const u8 *body,
                             u32 body_length);
void mshw0231_raw_consume_samples(struct spi_hid *shid, const u8 *samples,
                                   u32 sample_count, u8 content_id);
void mshw0231_raw_init(struct spi_hid *shid);
void mshw0231_raw_reset(struct spi_hid *shid);
int mshw0231_raw_input_register(struct spi_hid *shid);

#endif
