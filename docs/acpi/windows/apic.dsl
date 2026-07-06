/*
 * Intel ACPI Component Architecture
 * AML/ASL+ Disassembler version 20251212 (64-bit version)
 * Copyright (c) 2000 - 2025 Intel Corporation
 * 
 * Disassembly of apic.dat
 *
 * ACPI Data Table [APIC]
 *
 * Format: [HexOffset DecimalOffset ByteLength]  FieldName : FieldValue (in hex)
 */

[000h 0000 004h]                   Signature : "APIC"    [Multiple APIC Description Table (MADT)]
[004h 0004 004h]                Table Length : 000000BE
[008h 0008 001h]                    Revision : 04
[009h 0009 001h]                    Checksum : D5
[00Ah 0010 006h]                      Oem ID : "MSFT  "
[010h 0016 008h]                Oem Table ID : "MSFT    "
[018h 0024 004h]                Oem Revision : 00000000
[01Ch 0028 004h]             Asl Compiler ID : "AMD "
[020h 0032 004h]       Asl Compiler Revision : 00000000

[024h 0036 004h]          Local Apic Address : FEE00000
[028h 0040 004h]       Flags (decoded below) : 00000001
                         PC-AT Compatibility : 1

[02Ch 0044 001h]               Subtable Type : 00 [Processor Local APIC]
[02Dh 0045 001h]                      Length : 08
[02Eh 0046 001h]                Processor ID : 00
[02Fh 0047 001h]               Local Apic ID : 00
[030h 0048 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
                      Runtime Online Capable : 0

[034h 0052 001h]               Subtable Type : 00 [Processor Local APIC]
[035h 0053 001h]                      Length : 08
[036h 0054 001h]                Processor ID : 02
[037h 0055 001h]               Local Apic ID : 02
[038h 0056 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
                      Runtime Online Capable : 0

[03Ch 0060 001h]               Subtable Type : 00 [Processor Local APIC]
[03Dh 0061 001h]                      Length : 08
[03Eh 0062 001h]                Processor ID : 04
[03Fh 0063 001h]               Local Apic ID : 04
[040h 0064 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
                      Runtime Online Capable : 0

[044h 0068 001h]               Subtable Type : 00 [Processor Local APIC]
[045h 0069 001h]                      Length : 08
[046h 0070 001h]                Processor ID : 06
[047h 0071 001h]               Local Apic ID : 08
[048h 0072 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
                      Runtime Online Capable : 0

[04Ch 0076 001h]               Subtable Type : 00 [Processor Local APIC]
[04Dh 0077 001h]                      Length : 08
[04Eh 0078 001h]                Processor ID : 08
[04Fh 0079 001h]               Local Apic ID : 0A
[050h 0080 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
                      Runtime Online Capable : 0

[054h 0084 001h]               Subtable Type : 00 [Processor Local APIC]
[055h 0085 001h]                      Length : 08
[056h 0086 001h]                Processor ID : 0A
[057h 0087 001h]               Local Apic ID : 0C
[058h 0088 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
                      Runtime Online Capable : 0

[05Ch 0092 001h]               Subtable Type : 00 [Processor Local APIC]
[05Dh 0093 001h]                      Length : 08
[05Eh 0094 001h]                Processor ID : 01
[05Fh 0095 001h]               Local Apic ID : 01
[060h 0096 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
                      Runtime Online Capable : 0

[064h 0100 001h]               Subtable Type : 00 [Processor Local APIC]
[065h 0101 001h]                      Length : 08
[066h 0102 001h]                Processor ID : 03
[067h 0103 001h]               Local Apic ID : 03
[068h 0104 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
                      Runtime Online Capable : 0

[06Ch 0108 001h]               Subtable Type : 00 [Processor Local APIC]
[06Dh 0109 001h]                      Length : 08
[06Eh 0110 001h]                Processor ID : 05
[06Fh 0111 001h]               Local Apic ID : 05
[070h 0112 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
                      Runtime Online Capable : 0

[074h 0116 001h]               Subtable Type : 00 [Processor Local APIC]
[075h 0117 001h]                      Length : 08
[076h 0118 001h]                Processor ID : 07
[077h 0119 001h]               Local Apic ID : 09
[078h 0120 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
                      Runtime Online Capable : 0

[07Ch 0124 001h]               Subtable Type : 00 [Processor Local APIC]
[07Dh 0125 001h]                      Length : 08
[07Eh 0126 001h]                Processor ID : 09
[07Fh 0127 001h]               Local Apic ID : 0B
[080h 0128 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
                      Runtime Online Capable : 0

[084h 0132 001h]               Subtable Type : 00 [Processor Local APIC]
[085h 0133 001h]                      Length : 08
[086h 0134 001h]                Processor ID : 0B
[087h 0135 001h]               Local Apic ID : 0D
[088h 0136 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
                      Runtime Online Capable : 0

[08Ch 0140 001h]               Subtable Type : 01 [I/O APIC]
[08Dh 0141 001h]                      Length : 0C
[08Eh 0142 001h]                 I/O Apic ID : 21
[08Fh 0143 001h]                    Reserved : 00
[090h 0144 004h]                     Address : FEC00000
[094h 0148 004h]                   Interrupt : 00000000

[098h 0152 001h]               Subtable Type : 01 [I/O APIC]
[099h 0153 001h]                      Length : 0C
[09Ah 0154 001h]                 I/O Apic ID : 22
[09Bh 0155 001h]                    Reserved : 00
[09Ch 0156 004h]                     Address : FEC01000
[0A0h 0160 004h]                   Interrupt : 00000018

[0A4h 0164 001h]               Subtable Type : 02 [Interrupt Source Override]
[0A5h 0165 001h]                      Length : 0A
[0A6h 0166 001h]                         Bus : 00
[0A7h 0167 001h]                      Source : 00
[0A8h 0168 004h]                   Interrupt : 00000002
[0ACh 0172 002h]       Flags (decoded below) : 0000
                                    Polarity : 0
                                Trigger Mode : 0

[0AEh 0174 001h]               Subtable Type : 02 [Interrupt Source Override]
[0AFh 0175 001h]                      Length : 0A
[0B0h 0176 001h]                         Bus : 00
[0B1h 0177 001h]                      Source : 09
[0B2h 0178 004h]                   Interrupt : 00000009
[0B6h 0182 002h]       Flags (decoded below) : 000F
                                    Polarity : 3
                                Trigger Mode : 3

[0B8h 0184 001h]               Subtable Type : 04 [Local APIC NMI]
[0B9h 0185 001h]                      Length : 06
[0BAh 0186 001h]                Processor ID : FF
[0BBh 0187 002h]       Flags (decoded below) : 0005
                                    Polarity : 1
                                Trigger Mode : 1
[0BDh 0189 001h]        Interrupt Input LINT : 01

Raw Table Data: Length 190 (0xBE)

    0000: 41 50 49 43 BE 00 00 00 04 D5 4D 53 46 54 20 20  // APIC......MSFT  
    0010: 4D 53 46 54 20 20 20 20 00 00 00 00 41 4D 44 20  // MSFT    ....AMD 
    0020: 00 00 00 00 00 00 E0 FE 01 00 00 00 00 08 00 00  // ................
    0030: 01 00 00 00 00 08 02 02 01 00 00 00 00 08 04 04  // ................
    0040: 01 00 00 00 00 08 06 08 01 00 00 00 00 08 08 0A  // ................
    0050: 01 00 00 00 00 08 0A 0C 01 00 00 00 00 08 01 01  // ................
    0060: 01 00 00 00 00 08 03 03 01 00 00 00 00 08 05 05  // ................
    0070: 01 00 00 00 00 08 07 09 01 00 00 00 00 08 09 0B  // ................
    0080: 01 00 00 00 00 08 0B 0D 01 00 00 00 01 0C 21 00  // ..............!.
    0090: 00 00 C0 FE 00 00 00 00 01 0C 22 00 00 10 C0 FE  // ..........".....
    00A0: 18 00 00 00 02 0A 00 00 02 00 00 00 00 00 02 0A  // ................
    00B0: 00 09 09 00 00 00 0F 00 04 06 FF 05 00 01        // ..............
