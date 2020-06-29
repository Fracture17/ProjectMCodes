#ifndef CSSCUSTOMCONTROLS_GFPADSYSTEM_H
#define CSSCUSTOMCONTROLS_GFPADSYSTEM_H

#include "gfPadStatus.h"

struct gfPadSystem {
	char _spacer[0x40];
	//0x40
	//actually much more for 4 GC and 16? Wii slots, I think
	gfPadGamecube pads[4];

};

//global, statically allocated gfPadSystem object
#define PAD_SYSTEM ((gfPadSystem* const) 0x805bacc0)

#endif