//
// Created by dareb on 7/29/2020.
//

#ifndef PROJECTMCODES_MATH_H
#define PROJECTMCODES_MATH_H

#define ONE_RAD_IN_DEG (*(float*) 0x805a1eb4)
#define PI (*(float*) 0x805a37f0)
#define MIN_FLOAT (*(float*) 0x805a298c)
#define FLOAT_MIN_VALUE (*(float*) 0x805a2974)
#define ZERO (*(float*) 0x805a3780)
#define NEGATIVE_ONE (*(float*) 0x805a3784)
#define math_cos ((double (*)(double param1)) 0x804004d8)
#define math_rad (*(float *) 0x805a4ad8)
#define math_sin ((double (*)(double param1)) 0x804009e0)
#define math_atan2 ((double (*)(double x, double y)) 0x803fd0b0)
#define math_abs ((int (*)(int param1)) 0x803f5060)
#define math_fabs ((double (*)(double param1)) 0x802a0bcc)
#define _floor ((float (*)(float value)) 0x804005ac)

#define _atan_s_atan ((double (*)(double input)) 0x8040012c)

#endif //PROJECTMCODES_MATH_H
