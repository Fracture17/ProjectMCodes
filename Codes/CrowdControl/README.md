# Crowd Control

This WIP project is designed to interact with the [Crowd Control](https://crowdcontrol.live/) Twitch extension which allows 
viewers to affect the game in exchange for channel points or bits.

[Demo](https://streamable.com/6mkjce)

## Setup for Developing

Follow the root of this repo for instructions on how to build.

Please use the included Dolphin ([Source](https://github.com/ilazoja/dolphin/tree/crowd-control) in case needs to be modified or built for
other platforms) as it is used to communicate with the game as well as interact with Crowd Control.

Download the [Crowd Control SDK](https://forum.warp.world/t/how-to-setup-and-use-the-crowd-control-sdk/5121) and 
load the PPlusCrowdControl.cs to test effects.

## Make Your Own Effects

Adding your own effects is easy! Just need to setup a new effect in PPlusCrowdControl.cs and then handle the effect
in CrowdControl.cpp.

## Acknowledgements

Credits for each effect are listed above its corresponding functions.

[Kapedani](https://twitter.com/kapadanii)  
[fudgepop01](https://github.com/fudgepop01)  
[Eon](https://twitter.com/Eon_TAS/)  
DukeItOut  
[Fracture](https://github.com/Fracture17/)  
[DesiacX](https://github.com/DesiacX)  
[PyotrLuzhin](https://www.youtube.com/c/%D0%9F%D0%B5%D1%82%D1%80%D0%9B%D1%83%D0%B6%D0%B8%D0%BD64)  
wiiztec

As well as a special thanks to:

[FaultyPine](https://github.com/FaultyPine/) for sending/receiving data to/from Dolphin using Exi  
[Sammi Husky](https://github.com/Sammi-Husky) for managing and hosting the Ghidra server

