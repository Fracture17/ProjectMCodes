

#include "menu.h"

#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)

//////////////////////////////////
// Page
//////////////////////////////////
void Page::addOption(OptionType* option) {
  option->parent = this;
  options.push(option);
}

void Page::deselect() {
  if (!isSelected) menu->path.reallocate(menu->path.size() - 1);
  options[currentOption]->deselect();
  isSelected = false;
}

void Page::select() {
  isSelected = true;
  options[currentOption]->select();
}

void Page::setTitle(char* newTitle) {
  sprintf(this->title, newTitle);
}

void Page::up() {
  if (currentOption > 0) {
    char start = currentOption;
    for (char i = --currentOption; i > 0 && !options[i]->canModify; i--) {
      currentOption --;
    }
    if (!options[currentOption]->canModify) currentOption = start;
  }
}
void Page::down() {
  if (currentOption < options.size() - 1) {
    char start = currentOption;
    char size = options.size();
    for (char i = ++currentOption; i < size && !options[i]->canModify; i++) {
      currentOption ++;
    }
    if (!options[currentOption]->canModify) currentOption = start;
  }
}

void Page::modify(float amount) {
  options[currentOption]->modify(amount);
}

void Page::render(TextPrinter* printer, char* buffer) {
  char len = options.size();
  for (char i = 0; i < len; i++) {
    if (!options[i]->canModify) printer->setTextColor(0xFFAAAA88);
    else if (i == currentOption && isSelected) printer->setTextColor(0xFFFF00FF);
    else if (i == currentOption) printer->setTextColor(0xFFFFFFFF);
    else printer->setTextColor(0xFFFFFF88);
    printer->padToWidth(RENDER_X_SPACING / 5);
    options[i]->render(printer, buffer);
  }
}

//////////////////////////////////
// Menu
//////////////////////////////////
void Menu::nextPage(Page* p) { 
  p->menu = this;
  path.push(p);
}

Page* Menu::getCurrentPage() { return path[path.size() - 1]; }

void Menu::select() { selected = true; getCurrentPage()->select(); }
void Menu::deselect() { getCurrentPage()->deselect(); selected = false; }
void Menu::up() { getCurrentPage()->up(); }
void Menu::down() { getCurrentPage()->down(); }

void Menu::modify(float amount) { getCurrentPage()->modify(amount); }

void Menu::render(TextPrinter* printer, char* buffer) {
  printer->startBoundingBox();
  char amt = path.size();
  printer->print(" > ");
  for (char i = 0; i < amt; i++) {
    printer->print(" / ");
    printer->print(path[i]->title);
  }
  printer->printLine("");
  getCurrentPage()->render(printer, buffer);
  printer->saveBoundingBox(0, 0x00000088, 2);
}
void Menu::unpause() { paused = false; }
void Menu::toggle() {
  if (visible) {
    paused = false;
    visible = false;
  } else {
    paused = true;
    visible = true;
  }
}

//////////////////////////////////
// PageLink
//////////////////////////////////
void PageLink::modify(float _) {}
void PageLink::select() {
  parent->menu->nextPage(target);
  parent->isSelected = false;
  parent->menu->selected = false;
}
void PageLink::deselect() {}
void PageLink::render(TextPrinter* printer, char* buffer) {
  sprintf(buffer, "%s >", name);
  printer->printLine(buffer);
}

//////////////////////////////////
// IntOption
//////////////////////////////////
void IntOption::modify(float amount) {
  value += (int) amount;
  if (max != NUMERIC_DEFAULT && value > max) value = min;
  else if (min != NUMERIC_DEFAULT && value < min) value = max;
}
void IntOption::select() {}
void IntOption::deselect() {}
void IntOption::render(TextPrinter* printer, char* buffer) {
  sprintf(buffer, "%s: %03d", name, value);
  printer->printLine(buffer);
}

//////////////////////////////////
// FloatOption
//////////////////////////////////
void FloatOption::modify(float amount) {
  value += amount;
  if (max != NUMERIC_DEFAULT && value > max) value = min;
  else if (min != NUMERIC_DEFAULT && value < min) value = max;
}
void FloatOption::select() {}
void FloatOption::deselect() {}
void FloatOption::render(TextPrinter* printer, char* buffer) {
  sprintf(buffer, "%s: %.3f", name, value);
  printer->printLine(buffer);
}

//////////////////////////////////
// BoolOption
//////////////////////////////////
void BoolOption::modify(float amount) {
  OSReport("ADDRESS OF THIS: %08x\n", &value);
  if (amount > 0) value = true;
  else value = false;
}
void BoolOption::select() {}
void BoolOption::deselect() {}
void BoolOption::render(TextPrinter* printer, char* buffer) {
  sprintf(buffer, "%s: %s", name, value ? "on" : "off");
  printer->printLine(buffer);
}