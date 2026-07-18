# HeatCore Report Table

`CapImg::Protocol::HID::BuildDevice` builds the report table used before input
frames reach the TouchPen processor. The decompilation is at
`docs/decomp/heatcore_20260716/functions/18001bac0_FUN_18001bac0.c`.

## Direct Behavior

- The input descriptor exposes fewer than 17 report entries (`+0xb6` stores the
  count): lines 110-133.
- Each entry has report ID at descriptor-entry offset `+2` and a report-data
  count at `+4`; HeatCore stores them as six-byte table records beginning at
  object offset `+0x56`: lines 112-132.
- HeatCore independently marks entries that carry Digitizer Usage `0x62` and
  validates Usage `0x56` value-capabilities against the same report IDs: lines
  134-189.
- `UpdateFrameDataTransferForReport` later uses the first byte of an input
  report to select this table and calls HID parser APIs according to the stored
  flags: `functions/180008930_FUN_180008930.c:55-169`.

## Consequences

This is direct evidence that HeatCore dispatches by descriptor report ID; it is
not a state machine assigning semantic roles to ID8 or ID0c. Both reports can
be registered if their descriptor records satisfy the required Usage layout.

The next static task is to recover the generated table for MSHW0231 from the
captured descriptor: list each table entry's report ID, fixed unit size, Usage
0x62 flag and Usage 0x56 capability. This can establish parser eligibility for
ID8 and ID0c, but cannot establish that either enables the other.
