/* SPDX-License-Identifier: GPL-2.0 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM spi_hid

#if !defined(_SPI_HID_TRACE_H) || defined(TRACE_HEADER_MULTI_READ)
#define _SPI_HID_TRACE_H

#include <linux/types.h>
#include <linux/tracepoint.h>
#include "spi-hid-core.h"

/*
 * spi_hid_transfer and spi_hid_irq were removed here: neither has a
 * DEFINE_EVENT instance or a producer left (spi_hid_irq's only instance,
 * spi_hid_dev_irq, went in the producer-less-event sweep; spi_hid_transfer
 * never had one in this tree), so neither can ever fire. The invariant is
 * pinned in tests/driver_source_sanity_test.py.
 */
DECLARE_EVENT_CLASS(spi_hid,
	TP_PROTO(struct spi_hid *shid),

	TP_ARGS(shid),

	TP_STRUCT__entry(
		__field(int, bus_num)
		__field(int, chip_select)
		__field(int, power_state)
		__field(bool, ready)

		__field(int, vendor_id)
		__field(int, product_id)
		__field(int, max_input_length)
		__field(int, max_output_length)
		__field(u16, hid_version)
		__field(u16, report_descriptor_length)
		__field(u16, version_id)
	),

	TP_fast_assign(
		__entry->bus_num = shid->spi->controller->bus_num;
		__entry->chip_select = spi_get_chipselect(shid->spi, 0);
		__entry->power_state = shid->power_state;
		__entry->ready = shid->ready;

		__entry->vendor_id = shid->desc.vendor_id;
		__entry->product_id = shid->desc.product_id;
		__entry->max_input_length = shid->desc.max_input_length;
		__entry->max_output_length = shid->desc.max_output_length;
		__entry->hid_version = shid->desc.hid_version;
		__entry->report_descriptor_length = shid->desc.report_descriptor_length;
		__entry->version_id = shid->desc.version_id;
	),

	TP_printk("spi%d.%d: (%04x:%04x v%d) HID v%d.%d state p:%d len i:%d o:%d r:%d flags %c",
		__entry->bus_num, __entry->chip_select, __entry->vendor_id,
		__entry->product_id, __entry->version_id,
		__entry->hid_version >> 8, __entry->hid_version & 0xff,
		__entry->power_state,
		__entry->max_input_length, __entry->max_output_length,
		__entry->report_descriptor_length,
		__entry->ready ? 'R' : 'r')
);


DEFINE_EVENT(spi_hid, spi_hid_create_device_work,
	TP_PROTO(struct spi_hid *shid),
	TP_ARGS(shid)
);


/*
 * NOTE: spi_hid_seq_state is intentionally a standalone TRACE_EVENT
 * rather than using the common DECLARE_EVENT_CLASS(spi_hid) template.
 * It is a lightweight event that only carries bus/chip/state/reason/ready
 * fields, avoiding the overhead of the full descriptor fields (vendor,
 * product, lengths, version) that the common class would include.
 */

TRACE_EVENT(spi_hid_seq_state,
	TP_PROTO(struct spi_hid *shid, int old_state, int new_state, int reason),
	TP_ARGS(shid, old_state, new_state, reason),

	TP_STRUCT__entry(
		__field(int, bus_num)
		__field(int, chip_select)
		__field(int, old_state)
		__field(int, new_state)
		__field(int, reason)
		__field(bool, ready)
	),

	TP_fast_assign(
		__entry->bus_num = shid->spi->controller->bus_num;
		__entry->chip_select = spi_get_chipselect(shid->spi, 0);
		__entry->old_state = old_state;
		__entry->new_state = new_state;
		__entry->reason = reason;
		__entry->ready = shid->ready;
	),

	TP_printk("spi%d.%d: state %d->%d reason=%d ready=%u",
		__entry->bus_num, __entry->chip_select, __entry->old_state,
		__entry->new_state, __entry->reason, __entry->ready)
);

TRACE_EVENT(spi_hid_lifecycle,
	TP_PROTO(struct spi_hid *shid, int action, int ret),
	TP_ARGS(shid, action, ret),

	TP_STRUCT__entry(
		__field(int, bus_num)
		__field(int, chip_select)
		__field(int, action)
		__field(int, ret)
		__field(bool, removing)
	),

	TP_fast_assign(
		__entry->bus_num = shid->spi->controller->bus_num;
		__entry->chip_select = spi_get_chipselect(shid->spi, 0);
		__entry->action = action;
		__entry->ret = ret;
		__entry->removing = shid->removing;
	),

	TP_printk("spi%d.%d: action=%d ret=%d removing=%u",
		__entry->bus_num, __entry->chip_select, __entry->action,
		__entry->ret, __entry->removing)
);

#endif /* _SPI_HID_TRACE_H */

#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .
#define TRACE_INCLUDE_FILE spi-hid_trace
#include <trace/define_trace.h>
