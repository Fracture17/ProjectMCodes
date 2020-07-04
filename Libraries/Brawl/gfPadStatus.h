#pragma once

#include "Wii/PADButtons.h"

enum controllerType {
    GAMECUBE,
    CLASSIC,
    WIIMOTE,
    WIICHUCK
};

//size is 0x40
//technically is gfPadStatus, and is used for all controller types
//Just using gamecube for now
struct gfPadGamecube {
	char _spacer[6];
	//0x6
	PADButtons buttons;
	char _spacer2[0x30 - 8];
	//0x30
	char stickX;
	char stickY;
	char cStickX;
	char cStickY;
	char LTrigger;
	char RTrigger;

	char spacer[0x38 - 0x36];
	//0x38
	//0xFF if not connected, else 0
	bool isNotConnected;
	//0x3C
	controllerType type = GAMECUBE;
}__attribute__((packed, aligned(4)));
