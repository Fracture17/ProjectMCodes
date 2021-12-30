import { writeFile } from 'fs/promises';
import * as CS from 'construct-js';

const buildColor = (colorString: string) => {
  const Color = CS.Struct("Color")

  const csSplit = colorString.match(/[0-9A-F]{2}/ig);
  Color
    .field("red", CS.U8(parseInt(csSplit[0], 16)))
    .field("green", CS.U8(parseInt(csSplit[1], 16)))
    .field("blue", CS.U8(parseInt(csSplit[2], 16)))
    .field("alpha", CS.U8(parseInt(csSplit[3], 16)))

  return Color;
}

(async () => {

  /**
   * ////////////
   * // in C++:
   * ////////////
   * 
   * struct Color {
   *    char red    = 0xFF; // 255
   *    char green  = 0x88; // 136
   *    char blue   = 0x00; // 0
   *    char alpha  = 0xDD; // 221
   * };
   * 
   * struct Rect {
   *    int xPos = 420;
   *    int yPos = -69;
   *    unsigned int width = 42;
   *    unsigned int height = 70;
   * 
   *    Color color;
   * };
   */

  const B = CS.Endian.Big;
  const Rect = CS.Struct("Rect")
    .field("xPos", CS.I32(234, B))
    .field("yPos", CS.I32(-43, B))
    .field("width", CS.U32(90, B))
    .field("height", CS.U32(10, B))
    .field("color", buildColor("42044869"));
  
  await writeFile(`${__dirname}/../out/Rect.bin`, Rect.toUint8Array());
})();