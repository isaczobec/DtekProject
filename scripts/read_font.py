
class Glyph:
    def __init__(
            self,
            xOffset,
            yOffset,
            bitmapRows,
            bitmapPointer,
            asciiCode,
            advanceWidth,
            ):
        self.xOffset = xOffset
        self.yOffset = yOffset
        self.bitmapRows = bitmapRows
        self.bitmapPointer = bitmapPointer
        self.asciiCode = asciiCode
        self.advanceWidth = advanceWidth


glyphs: list[Glyph] = []
bitmaps = []

lines = []
with open("../assets/fonts/04B_03__8pt.bdf", encoding="latin-1") as file:
    lines = file.readlines()

while not len(lines) == 0:
    l = lines.pop(0)
    if "STARTCHAR" in l:

        xOffset = 0
        yOffset = 0
        bitmapRows = 0
        bitmapPointer = len(bitmaps)
        asciiCode = 0
        advanceWidth = 0

        while (True):
            l = lines.pop(0)
            if "BBX" in l:
                xOffset = int(l.split(" ")[3])
                yOffset = int(l.split(" ")[4])
            if "ENCODING" in l:
                asciiCode = int(l.split(" ")[1])

            if "DWIDTH" in l:
                advanceWidth = int(l.split(" ")[1])

            if "BITMAP" in l:
                l = lines.pop(0)
                while not "ENDCHAR" in l:
                    bitmaps.append(l.strip())
                    bitmapRows += 1
                    l = lines.pop(0)
                    
                break

        g = Glyph(xOffset, yOffset, bitmapRows, bitmapPointer, asciiCode, advanceWidth)
        glyphs.append(g)

# print arrays

print(f"uint8 bitmaps[{len(bitmaps)}] = {{")
for b in bitmaps:
    print(f"0x{b},")
print("};")

for g in glyphs:
    print(f"glyphs[{g.asciiCode}] = (BBFGlyph){{")
    print(f"    .advanceWidth = {g.advanceWidth},")
    print(f"    .xOffset = {g.xOffset},")
    print(f"    .yOffset = {g.yOffset},")
    print(f"    .bitmapPtr = &bitmaps[{g.bitmapPointer}],")
    print(f"    .bitmapRows = {g.bitmapRows}")
    print("};")

# print(glyphs)
# print(bitmaps)