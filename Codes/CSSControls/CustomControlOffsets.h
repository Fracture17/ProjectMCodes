//
// Created by johno on 6/7/2020.
//

#ifndef CSSCUSTOMCONTROLS_CUSTOMCONTROLOFFSETS_H
#define CSSCUSTOMCONTROLS_CUSTOMCONTROLOFFSETS_H


#include "Memory.h"
#include "Containers/vector.h"
#include "Assembly.h"

struct SettingInfo {
    SettingInfo(const char* name, unsigned char value);

    char name[10];
    unsigned char value;
};


struct ButtonInfo {
    ButtonInfo(const char* name, unsigned char offset, vector<SettingInfo*> settings);

    char name[10];
    unsigned char offset;
    vector<SettingInfo*> settings;
};

extern vector<SettingInfo*> GCSettingsInfo;
extern vector<ButtonInfo*> GCButtonInfo;

static const int test __attribute__((used)) = 7;


#define CSS_CONTROL_SETTINGS_EXTERN_DECLARATION \
extern SettingInfo GC_SETTING_ATTACK; \
extern SettingInfo GC_SETTING_SPECIAL; \
extern SettingInfo GC_SETTING_JUMP; \
extern SettingInfo GC_SETTING_SHIELD; \
extern SettingInfo GC_SETTING_GRAB; \
extern SettingInfo GC_SETTING_SMASH; \
extern SettingInfo GC_SETTING_UP_TAUNT; \
extern SettingInfo GC_SETTING_SIDE_TAUNT; \
extern SettingInfo GC_SETTING_DOWN_TAUNT; \
extern SettingInfo GC_SETTINGS_TAP_JUMP_ON; \
extern SettingInfo GC_SETTINGS_TAP_JUMP_OFF; \
extern vector<SettingInfo*> GCSettingsInfo; \
\
extern ButtonInfo GC_BUTTON_INFO_L; \
extern ButtonInfo GC_BUTTON_INFO_R; \
extern ButtonInfo GC_BUTTON_INFO_Z; \
extern ButtonInfo GC_BUTTON_INFO_DPAD_UP; \
extern ButtonInfo GC_BUTTON_INFO_DPAD_SIDE; \
extern ButtonInfo GC_BUTTON_INFO_DPAD_DOWN; \
extern ButtonInfo GC_BUTTON_INFO_B; \
extern ButtonInfo GC_BUTTON_INFO_A; \
extern ButtonInfo GC_BUTTON_INFO_CSTICK; \
extern ButtonInfo GC_BUTTON_INFO_Y; \
extern ButtonInfo GC_BUTTON_INFO_X; \
extern ButtonInfo GC_BUTTON_INFO_TAP_JUMP; \
extern vector<ButtonInfo*> GCButtonInfo;


#endif //CSSCUSTOMCONTROLS_CUSTOMCONTROLOFFSETS_H
