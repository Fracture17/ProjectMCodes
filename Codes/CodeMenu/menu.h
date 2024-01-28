#ifndef PROJECTMCODES_MENU_H
#define PROJECTMCODES_MENU_H

#include "Containers/vector.h"
#include <Graphics/TextPrinter.h>
#include "./hitboxHeatmap.h"

#define strcat ((int (*)(char* destination, const char* source)) 0x803fa384)
#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)

#define RENDER_X_SPACING 80
#define RENDER_SCALE_X 0.5;
#define RENDER_SCALE_Y 0.5;
#define TOP_PADDING 69
#define LEFT_PADDING 20

// this is a completely arbitrary number I HIGHLY doubt anyone will need
// ever so it'll act as a default value
#define NUMERIC_DEFAULT -983746279

struct Menu;
struct Page;
struct SubpageOption;

struct OptionType {
  virtual void select() = 0;
  virtual void deselect() = 0;
  virtual void modify(float amount) = 0;
  virtual void render(TextPrinter* printer, char* buffer) = 0;
  virtual void down() = 0;
  virtual void up() = 0;
  virtual void setParentPage(Page* p) = 0;
  virtual ~OptionType() {};
  const char* name;
  Page* parent;
  SubpageOption* subParent = nullptr;
  bool canModify = true;
  bool isCurrent = false;
};

struct StandardOption : public OptionType {
  void up() { modify(1); }
  void down() { modify(-1); }
  void setParentPage(Page* p) { this->parent = p; }
  virtual void select() {}
  virtual void deselect() {}
  virtual ~StandardOption() {}
};

struct Page {
  Page(Menu* myMenu);
  void addOption(OptionType* option);
  void setTitle(char* newTitle);
  void hide();
  void up();
  void down();
  void modify(float amount);
  void render(TextPrinter* printer, char* buffer);
  virtual void show();
  virtual void select();
  virtual void deselect();
  virtual const char* getTitle();
  virtual ~Page() {}

  vector<OptionType*> options;
  char currentOption = 0;
  bool isSelected = false;
  Menu* menu;
protected:
  char* title;
};

struct BasicPage : public Page {
  BasicPage(Menu* myMenu, const char* title);
  void select();
  void deselect();
  const char* getTitle();
  virtual void show() = 0;
protected:
  const char* title;
};

struct Menu {
  void nextPage(Page* p);
  Page* getCurrentPage();
  void select();
  void deselect();
  void up();
  void down();
  void modify(float amount);
  void render(TextPrinter* printer, char* buffer);
  void unpause();
  void toggle();
  
  bool visible = false;
  bool paused = false;
  bool selected = false;
  vector<Page*> pages;
  vector<Page*> path;
  float opacity = 0xDD;
};

struct PageLink : public StandardOption {
  Page* target;
  PageLink(const char* name, Page* target) {
    this->target = target;
    this->name = name;
  }
  void modify(float);
  void select();
  void deselect();
  void render(TextPrinter* printer, char* buffer);
};

class IntOption : public StandardOption {
public:
  IntOption(const char* name, int& value) : value(value) {
    this->name = name;
    this->value = value;
  }
  IntOption(const char* name, int& value, int min, int max) : value(value) {
    this->name = name;
    this->value = value;
    this->min = min;
    this->max = max;
  }
  IntOption(const char* name, int& value, bool canModify) : value(value) {
    this->name = name;
    this->value = value;
    this->canModify = canModify;
  }
  IntOption(const char* name, int& value, int min, int max, bool canModify) : value(value) {
    this->name = name;
    this->value = value;
    this->min = min;
    this->max = max;
    this->canModify = canModify;
  }
  void modify(float amount);
  void select();
  void deselect();
  void render(TextPrinter* printer, char* buffer);

protected:
  int& value;
  int max = NUMERIC_DEFAULT;
  int min = NUMERIC_DEFAULT;
};

class FloatOption : public StandardOption {
public:
  FloatOption(const char* name, float& value) : value(value) {
    this->name = name;
    this->value = value;
  }
  FloatOption(const char* name, float& value, float min, float max) : value(value) {
    this->name = name;
    this->value = value;
    this->min = min;
    this->max = max;
  }
  FloatOption(const char* name, float& value, float changeMultiplier) : value(value) {
    this->name = name;
    this->value = value;
    this->changeMultiplier = changeMultiplier;
  }
  FloatOption(const char* name, float& value, float min, float max, float changeMultiplier) : value(value) {
    this->name = name;
    this->value = value;
    this->min = min;
    this->max = max;
    this->changeMultiplier = changeMultiplier;
  }
  FloatOption(const char* name, float& value, bool canModify) : value(value) {
    this->name = name;
    this->value = value;
    this->canModify = canModify;
  }
  FloatOption(const char* name, float& value, float min, float max, bool canModify) : value(value) {
    this->name = name;
    this->value = value;
    this->min = min;
    this->max = max;
    this->canModify = canModify;
  }
  void modify(float amount);
  void select();
  void deselect();
  void render(TextPrinter* printer, char* buffer);

  ~FloatOption() {}

protected:
  float& value;
  float max = NUMERIC_DEFAULT;
  float min = NUMERIC_DEFAULT;
  float changeMultiplier = 1;
};

class BoolOption : public StandardOption { 
public:
  BoolOption(const char* name, bool& value) : value(value) {
    this->name = name;
  }
  BoolOption(const char* name, bool& value, bool canModify) : value(value) {
    this->name = name;
    this->canModify = canModify;
  }

  void modify(float amount);
  void select();
  void deselect();
  void render(TextPrinter* printer, char* buffer);

protected:
  bool& value;
};

class BitOption : public IntOption {
public:
  BitOption(const char* name, int& value, bool canModify, u8 shiftAmount) 
  : IntOption(name, value, 0, 1, canModify) {
    this->bitNum = shiftAmount;
  }

  void modify(float amount);
  void render(TextPrinter* printer, char* buffer);

protected:
  u8 bitNum = 0;
};

enum HexSize {
  CHAR = 1,
  SHORT = 2,
  INT = 3
};

class HexObserver : public StandardOption {
public:
  HexObserver(const char* name, u32& value, HexSize size) : value(value) {
    this->name = name;
    this->canModify = false;
    this->size = size;
  }
  void modify(float amount);
  void select();
  void deselect();
  void render(TextPrinter* printer, char* buffer);

protected:
  u32& value;
  HexSize size;
};

class ControlOption : public StandardOption { 
public:
  ControlOption(const char* name, bool& value) : value(value) {
    this->name = name;
  }

  void modify(float amount);
  void select();
  void deselect();
  void render(TextPrinter* printer, char* buffer);

protected:
  bool& value;
};

class StringOption : public StandardOption { 
public:
  StringOption(const char* name, const char* value) : value(value) {
    this->name = name;
    this->canModify = false;
  }

  void modify(float amount);
  void select();
  void deselect();
  void render(TextPrinter* printer, char* buffer);

protected:
  const char* value;
};

class TempLogOption : public StandardOption { 
public:
  TempLogOption() {
    this->name = "log to console";
  }

  void modify(float amount);
  void select();
  void deselect();
  void render(TextPrinter* printer, char* buffer);

protected:
  const char* name;
};

class NamedIndexOption : public StandardOption {
public: 
  NamedIndexOption(const char* name, const char** nameArray, int& index, int arrayLength) : index(index), nameArray(nameArray) {
    this->name = name;

    this->canModify = false;
    this->arrayLength = arrayLength;
  }

  void modify(float amount);
  void select();
  void deselect();
  void render(TextPrinter* printer, char* buffer);

protected:
  const char** nameArray;
  int& index;
  int arrayLength;
};

class SubpageOption : public OptionType {
public:
  SubpageOption(const char* name) {
    this->name = name;
  }
  SubpageOption(const char* name, int height, int depth) {
    this->name = name;
    this->height = height;
    this->depth = depth;
  }
  SubpageOption(const char* name, bool collapsible) {
    this->name = name;
    this->collapsible = collapsible;
  }
  SubpageOption(const char* name, int height, int depth, bool collapsible) {
    this->name = name;
    this->height = height;
    this->depth = depth;
    this->collapsible = collapsible;
  }

  void modify(float amount);
  void select();
  void deselect();
  void render(TextPrinter* printer, char* buffer);
  void down();
  void up();
  void setParentPage(Page* p);
  
  void addOption(OptionType* option);
  void clearOptions();
  void removeOptions();
  int getOptionCount();

  virtual ~SubpageOption() {
    options.clear();
  }

  vector<OptionType *> options = vector<OptionType *>();
  int& currentOption = this->_index;
  int _index = 0;
  int scrollIdx = 0;
  int height = 10;
  int depth = 1;
  bool isSelected = false;
  bool hasSelection = false;
  bool collapsible = false;
};

class BarOption : public StandardOption {
public:
  BarOption(const char* name, float& value, float& max, GXColor color, float width) : value(value), max(max) {
    this->name = name;
    this->color = color;
    this->width = width;
    this->canModify = false;
  }

  BarOption(const char* name, float& value, float& max, float& min, GXColor color, float width) : value(value), max(max), min(min) {
    this->name = name;
    this->color = color;
    this->width = width;
    this->canModify = false;
  }

  void modify(float amount) {}
  void select() {}
  void deselect() {}
  void render(TextPrinter* printer, char* buffer);

protected:
  float& max;
  float& min = this->defaultMin;
  float defaultMin = 0;
  float& value;
  float width;
  GXColor color;
};

#endif // PROJECTMCODES_MENU_H