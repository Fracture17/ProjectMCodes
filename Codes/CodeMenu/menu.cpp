

#include "menu.h"
#include "Graphics/Drawable.h"

#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)

//////////////////////////////////
// Page
//////////////////////////////////
Page::Page(Menu* myMenu) {
  menu = myMenu;
}

void Page::addOption(OptionType* option) {
  option->setParentPage(this);
  options.push(option);
}

void Page::deselect() {
  if (!isSelected) {
    menu->path.reallocate(menu->path.size() - 1);
    hide();
  } else {
    isSelected = false;
    options[currentOption]->deselect();
  }
}

void Page::select() {
  // isSelected = true;
  options[currentOption]->select();
}

void Page::setTitle(char* newTitle) {
  sprintf(this->title, newTitle);
}

void Page::up() {
  if (isSelected) options[currentOption]->up();
  else if (currentOption > 0) {
    char start = currentOption;
    for (char i = --currentOption; i > 0 && !options[i]->canModify; i--) {
      currentOption --;
    }
    if (!options[currentOption]->canModify) currentOption = start;
  }
}
void Page::down() {
  if (isSelected) options[currentOption]->down();
  else if (currentOption < options.size() - 1) {
    char start = currentOption;
    char size = options.size();
    for (char i = ++currentOption; i < (size - 1) && !options[i]->canModify; i++) {
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
    if (!options[i]->canModify) printer->setTextColor(0xFFAAAADD);
    else if (i == currentOption && isSelected) printer->setTextColor(0xFFFF00FF);
    else if (i == currentOption) printer->setTextColor(0xFFFFFFFF);
    else printer->setTextColor(0xFFFFFF88);
    printer->padToWidth(RENDER_X_SPACING / 5);
    options[i]->isCurrent = (i == currentOption);
    options[i]->render(printer, buffer);
  }
}

void Page::hide() {
  for (int i = options.size() - 1; i >= 0; i--) {
    delete options[i];
  }
  options.reallocate(0);
  options.reallocate(1);
}

void Page::show() {}
const char* Page::getTitle() { return this->title; }

//////////////////////////////////
// BasicPage
//////////////////////////////////
BasicPage::BasicPage(Menu* myMenu, const char* t) : Page(myMenu) {this->title = t;};
void BasicPage::select() {Page::select();}
void BasicPage::deselect() {Page::deselect();}
const char* BasicPage::getTitle() { return this->title; }

//////////////////////////////////
// Menu
//////////////////////////////////
void Menu::nextPage(Page* p) { 
  p->menu = this;
  p->show();
  path.push(p);
}

Page* Menu::getCurrentPage() { return path[path.size() - 1]; }

void Menu::select() { selected = true; getCurrentPage()->select(); }
void Menu::deselect() { getCurrentPage()->deselect(); selected = false; }
void Menu::up() { getCurrentPage()->up(); }
void Menu::down() { getCurrentPage()->down(); }

void Menu::modify(float amount) { getCurrentPage()->modify(amount); }

void Menu::render(TextPrinter* printer, char* buffer) {  
  printer->renderPre = true;
  printer->startBoundingBox();
  char amt = path.size();
  printer->print(" > ");
  for (char i = 0; i < amt; i++) {
    printer->print(path[i]->getTitle());
    printer->print(" / ");
  }
  printer->printLine("");
  getCurrentPage()->render(printer, buffer);
  printer->saveBoundingBox(0, (0x000000 << 8) | (int) opacity, 2);
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
  sprintf(buffer, (isCurrent) ? "(A) %s >" : "%s >", name);
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
  if (min == -1 && value == -1) sprintf(buffer, (isCurrent) ? "%s: < off >" : "%s: off", name);
  else sprintf(buffer, (isCurrent) ? "%s: < %03d >" : "%s: %03d", name, value);
  printer->printLine(buffer);
}

//////////////////////////////////
// FloatOption
//////////////////////////////////
void FloatOption::modify(float amount) {
  value += amount * changeMultiplier;
  if (max != NUMERIC_DEFAULT && value > max) value = min;
  else if (min != NUMERIC_DEFAULT && value < min) value = max;
}
void FloatOption::select() {}
void FloatOption::deselect() {}
void FloatOption::render(TextPrinter* printer, char* buffer) {
  if (min == -1.1 && value < 0) sprintf(buffer, (isCurrent) ? "%s: < off >" : "%s: off", name);
  else sprintf(buffer, (isCurrent) ? "%s: < %.3f >" : "%s: %.3f", name, value);
  printer->printLine(buffer);
}

//////////////////////////////////
// BoolOption
//////////////////////////////////
void BoolOption::modify(float amount) {
  if (amount > 0) value = true;
  else value = false;
}
void BoolOption::select() {}
void BoolOption::deselect() {}
void BoolOption::render(TextPrinter* printer, char* buffer) {
  sprintf(buffer, "%s: %s", name, value ? "on" : "off");
  printer->printLine(buffer);
}

//////////////////////////////////
// BitOption
//////////////////////////////////
void BitOption::modify(float amount) {
  // clears and sets the bit appropriately
  value &= ~(1 << bitNum);
  // "amount > 0" evaluates to 1 or 0
  value ^= ((amount > 0) << bitNum);
}
void BitOption::render(TextPrinter* printer, char* buffer) {
  sprintf(buffer, "%s: %s", name, (value & (1 << bitNum)) ? "on" : "off");
  printer->printLine(buffer);
}



//////////////////////////////////
// HexObserver
//////////////////////////////////
void HexObserver::modify(float amount) {}
void HexObserver::select() {}
void HexObserver::deselect() {}
void HexObserver::render(TextPrinter* printer, char* buffer) {
  switch (size) {
    case HexSize::CHAR:
      sprintf(buffer, "%s: 0x%02x", name, value);
      break;
    case HexSize::SHORT:
      sprintf(buffer, "%s: 0x%04x", name, value);
      break;
    case HexSize::INT:
      sprintf(buffer, "%s: 0x%08x", name, value);
      break;
  }
  printer->printLine(buffer);
}

//////////////////////////////////
// ControlOption
//////////////////////////////////
void ControlOption::modify(float amount) {}
void ControlOption::select() {
  value = true;
  this->parent->isSelected = true;
  this->parent->menu->paused = false;
}
void ControlOption::deselect() {
  value = false;
  this->parent->menu->paused = true;
}
void ControlOption::render(TextPrinter* printer, char* buffer) {
  sprintf(buffer, "%s", name);
  printer->printLine(buffer);
}

//////////////////////////////////
// StringOption
//////////////////////////////////
void StringOption::modify(float amount) {}
void StringOption::select() {}
void StringOption::deselect() {}
void StringOption::render(TextPrinter* printer, char* buffer) {
  sprintf(buffer, "%s: %s", name, value);
  printer->printLine(buffer);
}

//////////////////////////////////
// TempLogOption
//////////////////////////////////
extern FudgeAIHitbox fudgeAI;
void TempLogOption::modify(float amount) {};
void TempLogOption::select() { OSReport("xOffset = %.3f\nyOffset = %.3f\nxRange = %.3f\nyRange = %.3f\n", fudgeAI.trueXMin, fudgeAI.trueYMin, fudgeAI.width, fudgeAI.height); };
void TempLogOption::deselect() {};
void TempLogOption::render(TextPrinter* printer, char* buffer) {
  sprintf(buffer, (isCurrent) ? "(A) %s !" : "%s !", name);
  printer->printLine(buffer);
};

//////////////////////////////////
// NamedIndexOption
//////////////////////////////////
void NamedIndexOption::modify(float amount) {}
void NamedIndexOption::select() {}
void NamedIndexOption::deselect() {}
void NamedIndexOption::render(TextPrinter* printer, char* buffer) {
  sprintf(buffer, "%s: %s", name, (0 <= index && index < arrayLength) ? nameArray[index] : " ");
  printer->printLine(buffer);
}

//////////////////////////////////
// SubpageOption
//////////////////////////////////
void SubpageOption::deselect() {
  if (hasSelection) {
    hasSelection = false;
    options[currentOption]->deselect();
    if (this->subParent != nullptr) {
      this->subParent->hasSelection = true;
    }
    this->parent->isSelected = true;
  } else {
    isSelected = false;
    if (this->subParent != nullptr) this->subParent->hasSelection = false;
    else this->parent->isSelected = false;
  }
}

void SubpageOption::select() {
  if (isSelected) {
    options[currentOption]->select();
    hasSelection = true;
  }
  isSelected = true;
}
void SubpageOption::modify(float amount) {
  options[currentOption]->modify(amount);
}
void SubpageOption::up() {
  if (hasSelection) options[currentOption]->up();
  else if (currentOption > 0) {
    char start = currentOption;
    for (char i = --currentOption; i > 0 && !options[i]->canModify; i--) {
      currentOption --;
    }
    if (!options[currentOption]->canModify) currentOption = start;
    if (currentOption < (scrollIdx + (height / 2))) scrollIdx--;
  }
}
void SubpageOption::down() {
  if (hasSelection) options[currentOption]->down();
  else if (currentOption < options.size() - 1) {
    char start = currentOption;
    char size = options.size();
    for (char i = ++currentOption; i < (size - 1) && !options[i]->canModify; i++) {
      currentOption ++;
    }
    if (!options[currentOption]->canModify) currentOption = start;
    if (currentOption > (scrollIdx + (height / 2))) scrollIdx++;
  }
}

void SubpageOption::render(TextPrinter* printer, char* buffer) {
  int len = options.size();

  if (scrollIdx > (len - height)) scrollIdx = len - height;
  if (scrollIdx < 0) scrollIdx = 0;
  if (currentOption >= len) currentOption = len - 1;
  sprintf(buffer, (collapsible) ? ((isSelected) ? "v %s" : "> %s") : "%s:", name);
  printer->printLine(buffer);
  if (collapsible && isSelected || !(collapsible)) {
    for (int i = scrollIdx; i < (scrollIdx + height); i++) {
      if (i >= len) {
        printer->printLine("");
      } else {
        if (!options[i]->canModify) printer->setTextColor(0xFFAAAADD);
        else if (i == currentOption && hasSelection) printer->setTextColor(0xFFFF00FF);
        else if (i == currentOption) printer->setTextColor(0x88FF88FF);
        else printer->setTextColor(0xFFFFFF88);
        printer->padToWidth((RENDER_X_SPACING / 5 * (depth + 1)));
        options[i]->render(printer, buffer);
      }
    }
  }
}

void SubpageOption::addOption(OptionType* option) {
  option->parent = this->parent;
  option->subParent = this;
  options.push(option);
}

void SubpageOption::setParentPage(Page* p) {
  this->parent = p;
  int len = options.size(); 
  for (int i = 0; i < len; i++) options[i]->setParentPage(p);
}

void SubpageOption::clearOptions() {
  options.clear();
}

void SubpageOption::removeOptions() {
  options.reallocate(0);
  options.reallocate(1);
}

int SubpageOption::getOptionCount() {
  return options.size();
}

//////////////////////////////////
// BarOption
//////////////////////////////////
void BarOption::render(TextPrinter* printer, char* buffer) {
  sprintf(buffer, "%s: ", name);
  printer->print(buffer);

  float xPos = printer->message.xPos;
  float yPos = printer->message.yPos + 2;
  
  renderables.items.frame.push(new Rect(
    GXColor(0x000000CC),
    yPos - 1,
    yPos + printer->message.fontScaleY * 18 + 1,
    xPos - 1,
    xPos + width + 1,
    true
  ));

  renderables.items.frame.push(new Rect(
    color,
    yPos,
    yPos + printer->message.fontScaleY * 18,
    xPos,
    xPos + width * (((value - min) <= 0) ? 0 : (value - min) / max),
    true
  ));

  printer->printLine("");
}