#ifndef PROJECTMCODES_MENU_H
#define PROJECTMCODES_MENU_H

#include "Containers/vector.h"
#include <Graphics/TextPrinter.h>

#define strcat ((int (*)(char* destination, const char* source)) 0x803fa384)
#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)

#define RENDER_X_SPACING 80
#define RENDER_SCALE_X 0.5;
#define RENDER_SCALE_Y 0.5;
#define TOP_PADDING 69
#define LEFT_PADDING 20

#define NUMERIC_DEFAULT -983746279

struct Menu;
struct Page;

struct OptionType {
  virtual void select() = 0;
  virtual void deselect() = 0;
  virtual void modify(float amount) = 0;
  virtual void render(TextPrinter* printer, char* buffer) = 0;
  char name[20];
  Page* parent;
  bool canModify = true;
};

struct Page {
  void addOption(OptionType* option);
  void setTitle(char* newTitle);
  void select();
  void deselect();
  void up();
  void down();
  void modify(float amount);
  void render(TextPrinter* printer, char* buffer);

  char title[20];
  vector<OptionType*> options;
  char currentOption = 0;
  bool isSelected = false;
  Menu* menu;
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
  
  bool visible = true;
  bool paused = false;
  bool selected = false;
  vector<Page*> pages;
  vector<Page*> path;
};

struct PageLink : public OptionType {
  Page* target;
  PageLink(char* name, Page* target) {
    this->target = target;
    sprintf(this->name, name);
  }
  void modify(float);
  void select();
  void deselect();
  void render(TextPrinter* printer, char* buffer);
};

class IntOption : public OptionType {
public:
  IntOption(char* name, int& value) : value(value) {
    sprintf(this->name, name);
    this->value = value;
  }
  IntOption(char* name, int& value, int min, int max) : value(value) {
    sprintf(this->name, name);
    this->value = value;
    this->min = min;
    this->max = max;
  }
  IntOption(char* name, int& value, bool canModify) : value(value) {
    sprintf(this->name, name);
    this->value = value;
    this->canModify = canModify;
  }
  IntOption(char* name, int& value, int min, int max, bool canModify) : value(value) {
    sprintf(this->name, name);
    this->value = value;
    this->min = min;
    this->max = max;
    this->canModify = canModify;
  }
  void modify(float amount);
  void select();
  void deselect();
  void render(TextPrinter* printer, char* buffer);

private:
  int& value;
  int max = NUMERIC_DEFAULT;
  int min = NUMERIC_DEFAULT;
};

class FloatOption : public OptionType {
public:
  FloatOption(char* name, float& value) : value(value) {
    sprintf(this->name, name);
    this->value = value;
  }
  FloatOption(char* name, float& value, float min, float max) : value(value) {
    sprintf(this->name, name);
    this->value = value;
    this->min = min;
    this->max = max;
  }
  FloatOption(char* name, float& value, bool canModify) : value(value) {
    sprintf(this->name, name);
    this->value = value;
    this->canModify = canModify;
  }
  FloatOption(char* name, float& value, float min, float max, bool canModify) : value(value) {
    sprintf(this->name, name);
    this->value = value;
    this->min = min;
    this->max = max;
    this->canModify = canModify;
  }
  void modify(float amount);
  void select();
  void deselect();
  void render(TextPrinter* printer, char* buffer);

private:
  float& value;
  float max = NUMERIC_DEFAULT;
  float min = NUMERIC_DEFAULT;
};

class BoolOption : public OptionType { 
public:
  BoolOption(char* name, bool& value) : value(value) {
    sprintf(this->name, name);
  }
  BoolOption(char* name, bool& value, bool canModify) : value(value) {
    sprintf(this->name, name);
    this->canModify = canModify;
  }

  void modify(float amount);
  void select();
  void deselect();
  void render(TextPrinter* printer, char* buffer);

private:
  bool& value;
};

#endif // PROJECTMCODES_MENU_H