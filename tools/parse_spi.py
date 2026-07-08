#!/usr/bin/env python3
"""
Parser completo dei CSV ETW Windows per Surface Laptop 4 AMD (MSHW0231).
Estrae: transazioni SPI byte-by-byte, timing, GPIO IRQ, e sequenze di init.
"""
import csv, re, os, sys
from struct import unpack_from

def parse_hex(s):
    """Parse hex string like '0x0B000000FF00000000' into bytes."""
    s = s.strip()
    if s.startswith('0x') or s.startswith('0X'):
        s = s[2:]
    # Pad to even length
    if len(s) % 2:
        s = '0' + s
    return bytes.fromhex(s)

def parse_boot_trace(path):
    """Parse a CSV boot trace and extract structured SPI transactions."""
    transactions = []  # list of dicts
    current = None
    buffers = []  # list of (direction, bytes)
    
    with open(path, 'r', encoding='latin-1') as f:
        reader = csv.reader(f)
        next(reader)  # skip header
        
        for row in reader:
            if len(row) < 2:
                continue
            
            name = row[0].strip()
            etype = row[1].strip()
            
            if etype == 'IoSpbPayloadStart':
                # New transaction
                if current and buffers:
                    current['buffers'] = buffers
                    transactions.append(current)
                    buffers = []
                
                clock = int(row[16]) if len(row) > 16 and row[16].strip() else 0
                total_size = int(row[19]) if len(row) > 19 and row[19].strip() else 0
                transfer_count = int(row[20]) if len(row) > 20 and row[20].strip() else 0
                
                current = {
                    'clock_100ns': clock,
                    'total_size': total_size,
                    'transfer_count': transfer_count,
                    'buffers': [],
                }
            
            elif etype == 'IoSpbPayloadTdStart' and current:
                direction = row[20].strip() if len(row) > 20 else ''
                td_size = int(row[21]) if len(row) > 21 and row[21].strip() else 0
            
            elif etype == 'IoSpbPayloadTdBuffer' and current:
                if len(row) > 21 and row[21].strip():
                    try:
                        data = parse_hex(row[21])
                        # Determine direction from preceding TdStart
                        direction = 'TX'  # default
                        if len(buffers) >= 1:
                            direction = 'RX'
                        buffers.append((direction, data))
                    except:
                        pass
            
            elif etype == 'IoSpbPayloadStop':
                if current and buffers:
                    current['buffers'] = buffers
                    transactions.append(current)
                    buffers = []
                    current = None
            
            elif etype == 'InterruptInvokeDeviceIsrStart':
                clock = int(row[16]) if len(row) > 16 and row[16].strip() else 0
                pin = row[20].strip() if len(row) > 20 else ''
                flags = row[21].strip() if len(row) > 21 else ''
                transactions.append({
                    'type': 'IRQ_START',
                    'clock_100ns': clock,
                    'pin': pin,
                    'flags': flags,
                })
            
            elif etype == 'InterruptInvokeDeviceIsrComplete':
                clock = int(row[16]) if len(row) > 16 and row[16].strip() else 0
                transactions.append({
                    'type': 'IRQ_COMPLETE',
                    'clock_100ns': clock,
                })
    
    # Don't forget last transaction
    if current and buffers:
        current['buffers'] = buffers
        transactions.append(current)
    
    return transactions

HID_TYPES = {0: 'ACK', 1: 'DATA', 3: 'RESET_RSP', 4: 'CMD_RESP', 
             5: 'GET_FEAT_RESP', 7: 'DEVICE_DESC', 8: 'RPT_DESC'}

def decode_hid_header(data):
    """Decode HID-over-SPI header from bytes."""
    if len(data) < 4:
        return None
    htype = (data[0] >> 4) & 0xF
    hver = data[0] & 0xF
    body_len = (((data[1] >> 4) & 0xF) | (data[2] << 4)) * 4
    sync = data[3]
    return {
        'type': htype,
        'type_name': HID_TYPES.get(htype, f'UNK({htype})'),
        'version': hver,
        'body_len': body_len,
        'sync': sync,
    }

def find_sync_index(data):
    """Find the 5-byte sync pattern (0xFF*5) + HID header."""
    # Look for 0x5A preceded by appropriate data
    for i in range(min(20, len(data) - 3)):
        if data[i + 3] == 0x5A:
            # Check if this looks like a valid HID header
            hver = data[i] & 0xF
            if hver == 2:
                return i
    return -1

def analyze_trace(transactions, label):
    """Print a human-readable analysis of the SPI trace."""
    print(f"\n{'='*90}")
    print(f"  {label}")
    print(f"{'='*90}")
    
    prev_clock = 0
    spi_count = 0
    summary = {'read': 0, 'write': 0, 'other': 0, 'responses': []}
    
    for txn in transactions:
        if 'type' in txn and txn['type'].startswith('IRQ'):
            clock_us = txn['clock_100ns'] / 10.0
            gap = clock_us - prev_clock if prev_clock > 0 else 0
            prev_clock = clock_us
            print(f"\n  {'>>>' if 'START' in txn['type'] else '<<<'} {txn['type']}  +{gap:.0f}us  ({clock_us:.0f}us)")
            if 'pin' in txn:
                print(f"       Pin={txn['pin']}  Flags={txn['flags']}")
            continue
        
        if 'buffers' not in txn:
            continue
        
        bufs = txn['buffers']
        clock_us = txn['clock_100ns'] / 10.0
        gap = clock_us - prev_clock if prev_clock > 0 else 0
        prev_clock = clock_us
        
        spi_count += 1
        
        # Separate TX and RX
        tx_data = b''
        rx_data = b''
        for direction, data in bufs:
            if direction == 'TX' or (len(bufs) == 1 and not rx_data):
                tx_data = data
            else:
                rx_data = data
        
        # Determine opcode from first TX byte
        if len(tx_data) == 0:
            # RX-only transaction
            print(f"\n  TXN#{spi_count:3d} | +{gap:8.0f}us | RX-ONLY ({len(rx_data)}B)")
            continue
        
        opcode = tx_data[0]
        
        if opcode == 0x0B:
            summary['read'] += 1
            # Read approval
            addr = (tx_data[1] << 16) | (tx_data[2] << 8) | tx_data[3] if len(tx_data) >= 4 else 0
            a7 = tx_data[6] if len(tx_data) > 6 else None
            a8 = tx_data[7] if len(tx_data) > 7 else None
            
            # Analyze RX for HID response
            resp_info = ""
            if len(rx_data) > 0:
                sync_idx = find_sync_index(rx_data)
                if sync_idx >= 0:
                    hdr = decode_hid_header(rx_data[sync_idx:])
                    if hdr:
                        resp_info = f" -> {hdr['type_name']} body={hdr['body_len']}B sync={sync_idx}"
                        summary['responses'].append({
                            'txn': spi_count,
                            'clock_us': clock_us,
                            'type': hdr['type'],
                            'type_name': hdr['type_name'],
                            'body_len': hdr['body_len'],
                            'header': rx_data[sync_idx:sync_idx+4].hex(),
                        })
                elif len(rx_data) >= 4:
                    # Check for ACK pattern (03 00 00 00) without 0x5A
                    if rx_data[:4] == b'\x03\x00\x00\x00':
                        resp_info = " -> ACK (raw 03 00 00 00, no 0x5A sync)"
            
            marker = ""
            if a7 is not None and a7 != 0x00:
                marker += f" a7=0x{a7:02X}"
            if a8 is not None and a8 != 0x00:
                marker += f" a8=0x{a8:02X}"
            
            print(f"  TXN#{spi_count:3d} | +{gap:8.0f}us | READ 0x0B reg=0x{addr:06X}{marker} | TX={len(tx_data)}B RX={len(rx_data)}B{resp_info}")
            
            if len(rx_data) > 10:
                # Show first bytes of RX (after sync)
                rx_display = ' '.join(f'{b:02X}' for b in rx_data[:min(40, len(rx_data))])
                print(f"         RX: {rx_display}{'...' if len(rx_data)>40 else ''}")
        
        elif opcode == 0x02:
            summary['write'] += 1
            addr = (tx_data[1] << 16) | (tx_data[2] << 8) | tx_data[3] if len(tx_data) >= 4 else 0
            vlen = tx_data[4] if len(tx_data) >= 5 else 0
            
            # Check RX for inline response
            resp_info = ""
            if len(rx_data) > 0 and rx_data[0] != 0x00:
                sync_idx = find_sync_index(rx_data)
                if sync_idx >= 0:
                    hdr = decode_hid_header(rx_data[sync_idx:])
                    if hdr:
                        resp_info = f" <- {hdr['type_name']}"
            
            tx_hex = ' '.join(f'{b:02X}' for b in tx_data[:min(20, len(tx_data))])
            print(f"  TXN#{spi_count:3d} | +{gap:8.0f}us | WRITE 0x02 reg=0x{addr:06X} vlen=0x{vlen:02X} | TX={len(tx_data)}B{resp_info}")
            print(f"         TX: {tx_hex}{'...' if len(tx_data)>20 else ''}")
        
        elif opcode == 0x00:
            summary['other'] += 1
            tx_hex = ' '.join(f'{b:02X}' for b in tx_data[:min(20, len(tx_data))])
            print(f"  TXN#{spi_count:3d} | +{gap:8.0f}us | ACTIVATION 0x00 | TX={len(tx_data)}B")
            print(f"         TX: {tx_hex}{'...' if len(tx_data)>20 else ''}")
        
        elif opcode == 0x70:
            summary['other'] += 1
            tx_hex = ' '.join(f'{b:02X}' for b in tx_data[:min(4, len(tx_data))])
            print(f"  TXN#{spi_count:3d} | +{gap:8.0f}us | FW STATUS 0x70 | TX={len(tx_data)}B RX={len(rx_data)}B")
            if len(rx_data) > 0:
                rx_hex = ' '.join(f'{b:02X}' for b in rx_data[:min(20, len(rx_data))])
                print(f"         RX: {rx_hex}{'...' if len(rx_data)>20 else ''}")
        
        elif opcode == 0xB0:
            summary['other'] += 1
            if spi_count <= 15 or (spi_count > 110 and spi_count < 125):
                print(f"  TXN#{spi_count:3d} | +{gap:8.0f}us | FW BLOCK 0xB0 | TX={len(tx_data)}B")
        
        elif opcode == 0xB1:
            summary['other'] += 1
            tx_hex = ' '.join(f'{b:02X}' for b in tx_data[:min(10, len(tx_data))])
            print(f"  TXN#{spi_count:3d} | +{gap:8.0f}us | POST-FW 0xB1 | TX={len(tx_data)}B")
            print(f"         TX: {tx_hex}")
        
        elif opcode == 0x22 or opcode == 0x28:
            summary['other'] += 1
            print(f"  TXN#{spi_count:3d} | +{gap:8.0f}us | OPCODE 0x{opcode:02X} | TX={len(tx_data)}B")
        
        else:
            summary['other'] += 1
            tx_hex = ' '.join(f'{b:02X}' for b in tx_data[:min(10, len(tx_data))])
            print(f"  TXN#{spi_count:3d} | +{gap:8.0f}us | UNKNOWN 0x{opcode:02X} | TX={len(tx_data)}B RX={len(rx_data)}B")
            print(f"         TX: {tx_hex}")
            if len(rx_data) > 0:
                rx_hex = ' '.join(f'{b:02X}' for b in rx_data[:min(30, len(rx_data))])
                print(f"         RX: {rx_hex}")
    
    print(f"\n  SUMMARY: {summary['read']} reads, {summary['write']} writes, {summary['other']} other")
    return summary

def dump_response_sequence(summaries):
    """Dump the full response sequence for the boot_auto trace."""
    for label, summary in summaries.items():
        if 'responses' not in summary:
            continue
        print(f"\n  --- {label}: Response sequence ---")
        for r in summary['responses']:
            gap = r['clock_us']
            print(f"    TXN#{r['txn']:3d}  {r['type_name']:16s}  body={r['body_len']:5d}B  hdr={r['header']}")

def analyze_touch(path):
    """Analyze the touch CSV — extract runtime HID reports."""
    print(f"\n{'='*90}")
    print(f"  TOUCH RUNTIME (surface_touch.csv)")
    print(f"{'='*90}")
    
    txns = parse_boot_trace(path)
    data_reports = []
    prev_clock = 0
    count = 0
    
    for txn in txns:
        if 'type' in txn and txn['type'].startswith('IRQ'):
            continue
        if 'buffers' not in txn:
            continue
        
        bufs = txn['buffers']
        clock_us = txn['clock_100ns'] / 10.0
        gap = clock_us - prev_clock if prev_clock > 0 else 0
        prev_clock = clock_us
        
        tx_data = b''
        rx_data = b''
        for direction, data in bufs:
            if direction == 'TX' or (len(bufs) == 1 and not rx_data):
                tx_data = data
            else:
                rx_data = data
        
        if len(tx_data) > 0 and tx_data[0] == 0x0B and len(rx_data) > 0:
            sync_idx = find_sync_index(rx_data)
            if sync_idx >= 0:
                hdr = decode_hid_header(rx_data[sync_idx:])
                if hdr and hdr['type'] == 1:  # DATA
                    body = rx_data[sync_idx + 4:sync_idx + 4 + min(hdr['body_len'], len(rx_data) - sync_idx - 4)]
                    data_reports.append({
                        'clock_us': clock_us,
                        'gap_us': gap,
                        'body_len': hdr['body_len'],
                        'body': body,
                    })
                    count += 1
                    if count <= 5:
                        print(f"\n  DATA#{count:3d} | +{gap:8.0f}us | body={hdr['body_len']}B")
                        print(f"         Header: {rx_data[sync_idx:sync_idx+4].hex(' ')}")
                        print(f"         Body: {body[:min(60, len(body))].hex(' ')}{'...' if len(body)>60 else ''}")
    
    print(f"\n  Total DATA reports: {count}")
    
    # Analyze gaps between data reports
    gaps = [r['gap_us'] for r in data_reports if r['gap_us'] > 0]
    if gaps:
        gaps.sort()
        print(f"  Gap min={gaps[0]:.0f}us  median={gaps[len(gaps)//2]:.0f}us  max={gaps[-1]:.0f}us")
    
    return data_reports

def analyze_init(path):
    """Deep analysis of the cold boot init trace."""
    txns = parse_boot_trace(path)
    s = analyze_trace(txns, "COLD BOOT (surface_init.csv)")
    
    # Extract the vendor init sequence
    print(f"\n  --- VENDOR INIT SEQUENCE (cold boot) ---")
    prev_clock = 0
    for txn in txns:
        if 'type' in txn and txn['type'].startswith('IRQ'):
            continue
        if 'buffers' not in txn:
            continue
        bufs = txn['buffers']
        tx_data = b''
        rx_data = b''
        for direction, data in bufs:
            if direction == 'TX' or (len(bufs) == 1 and not rx_data):
                tx_data = data
            else:
                rx_data = data
        
        if len(tx_data) >= 14 and tx_data[0] == 0x02 and tx_data[3] == 0x04:
            # Vendor init: write to register 0x04
            clock_us = txn['clock_100ns'] / 10.0
            print(f"\n  VENDOR WRITE @0x04 ({len(tx_data)}B): {tx_data.hex(' ')}")
        
        if len(rx_data) > 0 and len(tx_data) == 0:
            # RX-only: could be checksum/response
            clock_us = txn['clock_100ns'] / 10.0
            if len(rx_data) < 200:
                print(f"  RX-ONLY ({len(rx_data)}B): {rx_data[:64].hex(' ')}")
    
    return s

if __name__ == '__main__':
    base = '/home/simone/spi-hid/traces'
    summaries = {}
    
    # Warm boot
    bt = parse_boot_trace(f'{base}/surface_boot_auto.csv')
    s1 = analyze_trace(bt, "WARM BOOT (surface_boot_auto.csv)")
    summaries['WARM_BOOT'] = s1
    
    # Cold boot
    ct = parse_boot_trace(f'{base}/surface_init.csv')
    s2 = analyze_trace(ct, "COLD BOOT (surface_init.csv)")
    summaries['COLD_BOOT'] = s2
    
    # Touch runtime
    analyze_touch(f'{base}/surface_touch.csv')
    
    # Response sequence
    dump_response_sequence(summaries)
