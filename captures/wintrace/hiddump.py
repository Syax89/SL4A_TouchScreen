import sys

TAGS = {
 (0,8):"Input",(0,9):"Output",(0,10):"Collection",(0,11):"Feature",(0,12):"EndCollection",
 (1,0):"UsagePage",(1,1):"LogicalMin",(1,2):"LogicalMax",(1,3):"PhysicalMin",(1,4):"PhysicalMax",
 (1,5):"UnitExp",(1,6):"Unit",(1,7):"ReportSize",(1,8):"ReportID",(1,9):"ReportCount",(1,10):"Push",(1,11):"Pop",
 (2,0):"Usage",(2,1):"UsageMin",(2,2):"UsageMax",(2,3):"DesignatorIndex",(2,4):"DesignatorMin",(2,5):"DesignatorMax",
 (2,7):"StringIndex",(2,8):"StringMin",(2,9):"StringMax",(2,10):"Delimiter",
}
UPAGES = {0x01:"GenericDesktop",0x06:"GenericDevCtrl",0x07:"Keyboard",0x08:"LED",0x09:"Button",0x0C:"Consumer",
          0x0D:"Digitizer",0x0F:"PID",0x14:"AlphaDisplay",0xFF0B:"Vendor(FF0B)"}
GD_USAGES={0x01:"Pointer",0x02:"Mouse",0x04:"Joystick",0x06:"Keyboard",0x30:"X",0x31:"Y",0x38:"Wheel"}
DIG_USAGES={0x01:"Digitizer",0x02:"Pen",0x03:"LightPen",0x04:"TouchScreen",0x05:"TouchPad",0x20:"Stylus",
            0x21:"Puck",0x22:"Finger",0x30:"TipPressure",0x32:"InRange",0x33:"Touch",0x38:"Barrel",
            0x39:"Erase",0x3C:"Invert",0x42:"TipSwitch",0x44:"BarrelSwitch",0x45:"Eraser",0x47:"SecondaryTipSwitch",
            0x48:"DeviceMode",0x51:"ContactID",0x53:"ContactCount",0x54:"ContactCountMax",0x55:"ScanTime",
            0x56:"SurfaceSwitch",0x57:"ButtonSwitch",0x5A:"TransducerIndex",0x5B:"TransducerIndexSelector",
            0x70:"ConfidenceConditionalCollection",0x61:"DeviceIndex",0x62:"ContactCount",0x63:"DeviceMode",
            0x3F:"DataValid",0x91:"ButtonType",0x93:"TouchState"}

def usage_name(page, usage):
    if page == 0x09:
        return f"Button{usage}"
    table = {0x01:GD_USAGES,0x0D:DIG_USAGES}.get(page)
    if table and usage in table:
        return table[usage]
    return hex(usage)

def parse(data):
    i = 0
    indent = 0
    upage = 0
    out = []
    while i < len(data):
        b = data[i]
        if b == 0xFE:  # long item, not expected
            size = data[i+1]
            i += size + 3
            continue
        bSize = b & 0x03
        bType = (b >> 2) & 0x03
        bTag = (b >> 4) & 0x0F
        size = {0:0,1:1,2:2,3:4}[bSize]
        i += 1
        val = int.from_bytes(data[i:i+size], "little") if size else 0
        i += size
        name = TAGS.get((bType,bTag), f"Type{bType}Tag{bTag}")
        pad = "  "*indent
        if name == "UsagePage":
            upage = val
            out.append(f"{pad}UsagePage ({UPAGES.get(val, hex(val))})")
        elif name == "Usage":
            out.append(f"{pad}Usage ({usage_name(upage,val)})")
        elif name == "Collection":
            ctypes = {0:"Physical",1:"Application",2:"Logical",3:"Report",4:"NamedArray",5:"UsageSwitch",6:"UsageModifier"}
            out.append(f"{pad}Collection ({ctypes.get(val,val)})")
            indent += 1
        elif name == "EndCollection":
            indent -= 1
            out.append(f"{pad[:-2] if len(pad)>=2 else pad}EndCollection")
        elif name in ("Input","Output","Feature"):
            flags = val
            desc = []
            desc.append("Const" if flags&1 else "Data")
            desc.append("Var" if flags&2 else "Arr")
            desc.append("Rel" if flags&4 else "Abs")
            out.append(f"{pad}{name} ({','.join(desc)}) [0x{flags:02x}]")
        else:
            out.append(f"{pad}{name} = {val} (0x{val:x})")
    return out

data = open(sys.argv[1],'rb').read()
for line in parse(data):
    print(line)
