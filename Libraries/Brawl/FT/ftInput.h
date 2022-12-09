#ifndef PROJECTMCODES_FTINPUT_H
#define PROJECTMCODES_FTINPUT_H

struct FtEntry;
struct AiInput;

struct FtInput {
  char _spacer[0x4];

  // 0x4
  FtEntry* ftEntryPtr;

  char _spacer2[0xC - 0x4 - 4];
  // 0xC
  AiInput* aiInputMain;
  // 0x10
  AiInput* aiInputSub;
};

#endif //PROJECTMCODES_FTINPUT_H