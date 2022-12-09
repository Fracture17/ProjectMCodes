#ifndef PROJECTMCODES_FIGHTER_H
#define PROJECTMCODES_FIGHTER_H

#include "Brawl/SO/StageObject.h"
#include "Brawl/SO/soModuleAccessor.h"

struct Fighter_vtable18 {
	char _spacer[0x150];
	void** cancelModuleFn;
};

// enum SUBACTION_NAME {
//     // Idle/Standing animation.
//   // (All?)
//   Wait1 = 0x000,
//   // Second Idle/Standing.
//   // (All?)
//   Wait2 = 0x001,
//   // Third Idle/Standing.
//   // (All?)
//   Wait3 = 0x002,
//   // Fourth Idle/Standing.
//   // (All?)
//   Wait4 = 0x003,
//   // Fifth Idle/Standing.
//   // (All?)
//   Wait5 = 0x004,
//   // Idle with Item in hand.
//   // (All?)
//   WaitItem = 0x005,
//   // Hand while picking up an item.
//   // (All?)
//   ItemHandPickUp = 0x006,
//   // Hand while holding a small item.
//   // (All?)
//   ItemHandHave = 0x007,
//   // Hand while holding a battering weapon.
//   // (All?)
//   ItemHandGrip = 0x008,
//   // Hand while holding a Trophy Stand.
//   // (All?)
//   ItemHandSmash = 0x009,
//   // Slow walk.
//   // (All?)
//   WalkSlow = 0x00A,
//   // Medium-speed walk.
//   // (All?)
//   WalkMiddle = 0x00B,
//   // Fast walk.
//   // (All?)
//   WalkFast = 0x00C,
//   // Stop walking.
//   // (All?)
//   WalkBrake = 0x00D,
//   // Initial dash.
//   // (All?)
//   Dash = 0x00E,
//   // Running.
//   // (All?)
//   Run = 0x00F,
//   // Stop running.
//   // (All?)
//   RunBrake = 0x010,
//   // Face opposite direction.
//   // (All?)
//   Turn = 0x011,
//   // Turn while running.
//   // (All?)
//   TurnRun = 0x012,
//   // Stop running while turning.
//   // (All?)
//   TurnRunBrake = 0x013,
//   // Initial jump, before leaving ground.
//   // (All?)
//   JumpSquat = 0x014,
//   // Forward jump.
//   // (All?)
//   JumpF = 0x015,
//   // Forward shorthop.
//   // (All?)
//   JumpF = 0x016,
//   // Backward jump.
//   // (All?)
//   JumpB = 0x017,
//   // Backward shorthop.
//   // (All?)
//   JumpB = 0x018,
//   // Forward double jump.
//   // (All?)
//   JumpAerialF = 0x019,
//   // Backward double jump.
//   // (All?)
//   JumpAerialB = 0x01A,
//   // Second jump (multi-jumpers).
//   // (Charizard, Dedede, Jigglypuff, Kirby, Meta Knight, Pit)
//   JumpAerialF = 0x01B,
//   // Third jump.
//   // (Charizard, Dedede, Jigglypuff, Kirby, Meta Knight, Pit)
//   JumpAerialF2 = 0x01C,
//   // Fourth jump.
//   // (Dedede, Jigglypuff, Kirby, Meta Knight, Pit)
//   JumpAerialF3 = 0x01D,
//   // Fifth jump.
//   // (Dedede, Jigglypuff, Kirby, Meta Knight)
//   JumpAerialF4 = 0x01E,
//   // Sixth jump.
//   // (Dedede, Jigglypuff, Kirby, Meta Knight)
//   JumpAerialF5 = 0x01F,
//   // Normal falling.
//   // (All?)
//   Fall = 0x020,
//   // Normal falling while leaning forwards.
//   // (All?)
//   FallF = 0x021,
//   // Normal falling while leaning backwards.
//   // (All?)
//   FallB = 0x022,
//   // Falling after double jump.
//   // (All?)
//   FallAerial = 0x023,
//   // Falling forwards after double jump.
//   // (All?)
//   FallAerialF = 0x024,
//   // Falling backwards after double jump.
//   // (All?)
//   FallAerialB = 0x025,
//   // Falling after using special move.
//   // (All?)
//   FallSpecial = 0x026,
//   // Falling forwards after using special move.
//   // (All?)
//   FallSpecialF = 0x027,
//   // Falling backwards after using special move.
//   // (All?)
//   FallSpecialB = 0x028,
//   // Tumble animation.
//   // (All?)
//   DamageFall = 0x029,
//   // Crouching animation.
//   // (All?)
//   Squat = 0x02A,
//   // Crouching idle animation.
//   // (All?)
//   SquatWait = 0x02B,
//   // Second crouching idle animation.
//   // (All?)
//   SquatWait2 = 0x02C,
//   // Crouching idle animation with item in hand.
//   // (All?)
//   SquatWaitItem = 0x02D,
//   // Crawling forward.
//   // (All?)
//   SquatF = 0x02E,
//   // Crawling backward.
//   // (All?)
//   SquatB = 0x02F,
//   // Standing up from crouch.
//   // (All?)
//   SquatRv = 0x030,
//   // Landing from fall (slower landing speed).
//   // (All?)
//   LandingLight = 0x031,
//   // Landing from fall (higher landing speed).
//   // (All?)
//   LandingHeavy = 0x032,
//   // Landing from FallSpecial.
//   // (All?)
//   LandingFallSpecial = 0x033,
//   // Footstooling someone.
//   // (All?)
//   StepJump = 0x034,
//   // Getting footstooled on the ground.
//   // (All?)
//   StepPose = 0x035,
//   // Recovering from grounded footstool.
//   // (All?)
//   StepBack = 0x036,
//   // Getting footstooled in the air.
//   // (All?)
//   StepAirPose = 0x037,
//   // Falling after airborne footstool.
//   // (All?)
//   StepFall = 0x038,
//   // Entering glide animation.
//   // (All?)
//   GlideStart = 0x039,
//   // Angling glide animation up and down.
//   // (All?)
//   GlideDirection = 0x03A,
//   // Separate animation for the wings.
//   // (All?)
//   GlideWing = 0x03B,
//   // Glide attack (g-air, gl-air).
//   // (All?)
//   GlideAttack = 0x03C,
//   // Exiting glide animation (having stalled).
//   // (All?)
//   GlideEnd = 0x03D,
//   // Landing while still in glide animation.
//   // (All?)
//   GlideLanding = 0x03E,
//   // Putting up shield.
//   // (All?)
//   GuardOn = 0x03F,
//   // Shield looping animation.
//   // (All?)
//   Guard = 0x040,
//   // Putting down shield.
//   // (All?)
//   GuardOff = 0x041,
//   // Shield getting hit.
//   // (All?)
//   GuardDamage = 0x042,
//   // Spot/Ground dodge.
//   // (All?)
//   EscapeN = 0x043,
//   // Forward roll Dodge.
//   // (All?)
//   EscapeF = 0x044,
//   // Backward roll Dodge.
//   // (All?)
//   EscapeB = 0x045,
//   // Air dodge.
//   // (All?)
//   EscapeAir = 0x046,
//   // Pushed backwards after having an attack clang.
//   // (All?)
//   Rebound = 0x047,
//   // Jab 1.
//   // (All?)
//   Attack11 = 0x048,
//   // Jab 2.
//   // (All?)
//   Attack12 = 0x049,
//   // Jab 3.
//   // (All?)
//   Attack13 = 0x04A,
//   // Entering rapid jab loop.
//   // (All?)
//   Attack100Start = 0x04B,
//   // Rapid jab loop.
//   // (All?)
//   Attack100 = 0x04C,
//   // Exiting rapid jab loop.
//   // (All?)
//   AttackEnd = 0x04D,
//   // Dash attack.
//   // (All?)
//   AttackDash = 0x04E,
//   // Forward tilt (f-tilt) angled upward.
//   // (All?)
//   AttackS3Hi = 0x04F,
//   // Forward tilt (f-tilt).
//   // (All?)
//   AttackS3S = 0x050,
//   // Forward tilt 2 (f-tilt.2).
//   // (Meta Knight, Snake)
//   AttackS3S2 = 0x051,
//   // Forward tilt 3 (f-tilt.3).
//   // (Meta Knight)
//   AttackS3S3 = 0x052,
//   // Forward tilt (f-tilt) angled downward.
//   // (All?)
//   AttackS3Lw = 0x053,
//   // Up tilt (u-tilt).
//   // (All?)
//   AttackHi3 = 0x054,
//   // Down tilt (d-tilt).
//   // (All?)
//   AttackLw3 = 0x055,
//   // Forward smash windup.
//   // (All?)
//   AttackS4Start = 0x056,
//   // Forward smash (f-smash) angled upward.
//   // (All?)
//   AttackS4Hi = 0x057,
//   // Forward smash (f-smash).
//   // (All?)
//   AttackS4S = 0x058,
//   // Forward smash 2 (f-smash.2).
//   // (Link, Toon Link)
//   AttackS4S2 = 0x059,
//   // Forward Smash (f-smash) angled downward.
//   // (All?)
//   AttackS4Lw = 0x05A,
//   // Forward smash charging.
//   // (All?)
//   AttackS4Hold = 0x05B,
//   // Up smash windup.
//   // (All?)
//   AttackHi4Start = 0x05C,
//   // Up smash (u-smash).
//   // (All?)
//   AttackHi4 = 0x05D,
//   // Up smash charging.
//   // (All?)
//   AttackHi4Hold = 0x05E,
//   // Down smash windup.
//   // (All?)
//   AttackLw4Start = 0x05F,
//   // Down smash (d-smash).
//   // (All?)
//   AttackLw4 = 0x060,
//   // Down smash charging.
//   // (All?)
//   AttackLw4Hold = 0x061,
//   // Neutral aerial (n-air).
//   // (All?)
//   AttackAirN = 0x062,
//   // Forward aerial (f-air).
//   // (All?)
//   AttackAirF = 0x063,
//   // Backward aerial (b-air).
//   // (All?)
//   AttackAirB = 0x064,
//   // Upward aerial (u-air).
//   // (All?)
//   AttackAirHi = 0x065,
//   // Downward aerial (d-air).
//   // (All?)
//   AttackAirLw = 0x066,
//   // Neutral Aerial (n-air) landing animation.
//   // (All?)
//   LandingAirN = 0x067,
//   // Forward aerial (f-air) landing animation.
//   // (All?)
//   LandingAirF = 0x068,
//   // Backward aerial (b-air) landing animation.
//   // (All?)
//   LandingAirB = 0x069,
//   // Upward aerial (u-air) landing animation.
//   // (All?)
//   LandingAirHi = 0x06A,
//   // Downward aerial (d-air) landing animation.
//   // (All?)
//   LandingAirLw = 0x06B,
//   // Standing grab.
//   // (All?)
//   Catch = 0x06C,
//   // Running grab.
//   // (All?)
//   CatchDash = 0x06D,
//   // Pivot grab.
//   // (All?)
//   CatchTurn = 0x06E,
//   // Holding opponent.
//   // (All?)
//   CatchWait = 0x06F,
//   // Pummel attack.
//   // (All?)
//   CatchAttack = 0x070,
//   // Releasing opponent (grab break).
//   // (All?)
//   CatchCut = 0x071,
//   // Backward throw (b-throw).
//   // (All?)
//   ThrowB = 0x072,
//   // Forward throw (f-throw).
//   // (All?)
//   ThrowF = 0x073,
//   // Upward throw (u-throw).
//   // (All?)
//   ThrowHi = 0x074,
//   // Downward throw (d-throw).
//   // (All?)
//   ThrowLw = 0x075,
//   // This is executed by the character in the process of being back thrown, not the character doing the throwing. For example, this is where the swing hitbox is for Mario's back throw, which is attached to the character being thrown.
//   // (All?)
//   ThrownB = 0x076,
//   // Same as ThrownB for forward throw.
//   // (All?)
//   ThrownF = 0x077,
//   // Same as ThrownB for upward throw.
//   // (All?)
//   ThrownHi = 0x078,
//   // Same as ThrownB for downward throw.
//   // (All?)
//   ThrownLw = 0x079,
//   // Same as ThrownB, but with the arms/legs pointing different directions. Used to compensate for some characters' non-standard rest poses. All "Dx" animations use this concept.
//   // (All?)
//   ThrownDxB = 0x07A,
//   // All?
//   ThrownDxF = 0x07B, 
//   // All?
//   ThrownDxHi = 0x07C,  
//   // All?
//   ThrownDxLw = 0x07D,  
//   // Getting grabbed by a taller opponent.
//   // (All?)
//   CapturePulledHi = 0x07E,
//   // Being held by a taller opponent.
//   // (All?)
//   CaptureWaitHi = 0x07F,
//   // Being pummeled by a taller opponent.
//   // (All?)
//   CaptureDamageHi = 0x080,
//   // Getting grabbed by a shorter opponent.
//   // (All?)
//   CapturePulledLw = 0x081,
//   // Being held by a shorter opponent.
//   // (All?)
//   CaptureWaitLw = 0x082,
//   // Being pummeled by a shorter opponent.
//   // (All?)
//   CaptureDamageLw = 0x083,
//   // Getting grabbed by Snake.
//   // (All?)
//   CapturePulledSnake = 0x084,
//   // Being held by Snake.
//   // (All?)
//   CaptureWaitSnake = 0x085,
//   // Being pummeled by Snake.
//   // (All?)
//   CaptureDamageSnake = 0x086,
//   // All?
//   CapturePulledSnake = 0x087,
//   // All?
//   CaptureWaitSnake = 0x088,
//   // All?
//   CaptureDamageSnake = 0x089,
//   // All?
//   CapturePulledDxSnake = 0x08A,
//   // All?
//   CaptureWaitDxSnake = 0x08B,
//   // All?
//   CaptureDamageDxSnake = 0x08C,
//   // All?
//   CapturePulledDxSnake = 0x08D,
//   // All?
//   CaptureWaitDxSnake = 0x08E,
//   // All?
//   CaptureDamageDxSnake = 0x08F,
//   // All?
//   CapturePulledBigSnake = 0x090,
//   // All?
//   CaptureWaitBigSnake = 0x091,
//   // All?
//   CaptureDamageBigSnake = 0x092,
//   // All?
//   CapturePulledBigSnake = 0x093,
//   // All?
//   CaptureWaitBigSnake = 0x094,
//   // All?
//   CaptureDamageBigSnake = 0x095,
//   // Escaping from an opponent's grab onto the ground (ground release).
//   // (All?)
//   CaptureCut = 0x096,
//   // Escaping from an opponent's grab into the air (air release).
//   // (All?)
//   CaptureJump = 0x097,
//   // Being hit weakly in the head.
//   // (All?)
//   DamageHi1 = 0x098,
//   // Being hit moderately in the head.
//   // (All?)
//   DamageHi2 = 0x099,
//   // Being hit strongly in the head.
//   // (All?)
//   DamageHi3 = 0x09A,
//   // Being hit weakly in the midsection.
//   // (All?)
//   DamageN1 = 0x09B,
//   // Being hit moderately in the midsection.
//   // (All?)
//   DamageN2 = 0x09C,
//   // Being hit strongly in the midsection.
//   // (All?)
//   DamageN3 = 0x09D,
//   // Being hit weakly in the legs.
//   // (All?)
//   DamageLw1 = 0x09E,
//   // Being hit moderately in the legs.
//   // (All?)
//   DamageLw2 = 0x09F,
//   // Being hit strongly in the legs.
//   // (All?)
//   DamageLw3 = 0x0A0,
//   // Being hit weakly in the air.
//   // (All?)
//   DamageAir1 = 0x0A1,
//   // Being hit moderately in the air.
//   // (All?)
//   DamageAir2 = 0x0A2,
//   // Being hit strongly in the air.
//   // (All?)
//   DamageAir3 = 0x0A3,
//   // Being hit in the head hard enough to be sent flying.
//   // (All?)
//   DamageFlyHi = 0x0A4,
//   // Being hit in the midsection hard enough to be sent flying.
//   // (All?)
//   DamageFlyN = 0x0A5,
//   // Being hit in the legs hard enough to be sent flying.
//   // (All?)
//   DamageFlyLw = 0x0A6,
//   // Getting hit upward and rolling into a tumble.
//   // (All?)
//   DamageFlyTop = 0x0A7,
//   // Getting hit hard enough to start reeling ("barrel roll").
//   // (All?)
//   DamageFlyRoll = 0x0A8,
//   // Hit by a Paralyze attack (i.e. ZSS's Paralyzer).
//   // (All?)
//   DamageElec = 0x0A9,
//   // Hitting the floor while tumbling, facing up.
//   // (All?)
//   DownBoundU = 0x0AA,
//   // Lying on ground, facing up.
//   // (All?)
//   DownWaitU = 0x0AB,
//   // Being hit while lying on ground, facing up.
//   // (All?)
//   DownDamageU = 0x0AC,
//   // Being hit strongly while lying on ground, facing up.
//   // (All?)
//   DownDamageU3 = 0x0AD,
//   // Eating something while in lying on ground, facing up.
//   // (All?)
//   DownEatU = 0x0AE,
//   // Standing up after lying down, facing up.
//   // (All?)
//   DownStandU = 0x0AF,
//   // Floor Attack, facing up.
//   // (All?)
//   DownAttackU = 0x0B0,
//   // Rolling forward while lying down, facing up.
//   // (All?)
//   DownForwardU = 0x0B1,
//   // Rolling backward while lying down, facing up.
//   // (All?)
//   DownBackU = 0x0B2,
//   // Hitting the floor while tumbling, facing down.
//   // (All?)
//   DownBoundD = 0x0B3,
//   // Lying on ground, facing down.
//   // (All?)
//   DownWaitD = 0x0B4,
//   // Being hit while lying on ground, facing down.
//   // (All?)
//   DownDamageD = 0x0B5,
//   // Being hit strongly while lying on ground, facing down.
//   // (All?)
//   DownDamageD3 = 0x0B6,
//   // Eating something while lying on ground, facing down.
//   // (All?)
//   DownEatD = 0x0B7,
//   // Standing up after lying down, facing down.
//   // (All?)
//   DownStandD = 0x0B8,
//   // Floor Attack, facing down.
//   // (All?)
//   DownAttackD = 0x0B9,
//   // Rolling forward while lying down, facing down.
//   // (All?)
//   DownForwardD = 0x0BA,
//   // Rolling backward while lying down, facing down.
//   // (All?)
//   DownBackD = 0x0BB,
//   // Lying down (facing up) while standing. Used in Stamina Mode, for example.
//   // (All?)
//   DownSpotU = 0x0BC,
//   // Lying down (facing down) while standing. Used in Stamina Mode, for example.
//   // (All?)
//   DownSpotD = 0x0BD,
//   // Teching the floor in place.
//   // (All?)
//   Passive = 0x0BE,
//   // Teching the floor, rolling forward.
//   // (All?)
//   PassiveStandF = 0x0BF,
//   // Teching the floor, rolling backward.
//   // (All?)
//   PassiveStandB = 0x0C0,
//   // Teching a wall.
//   // (All?)
//   PassiveWall = 0x0C1,
//   // Teching a wall into a jump.
//   // (All?)
//   PassiveWallJump = 0x0C2,
//   // Teching a ceiling.
//   // (All?)
//   PassiveCeil = 0x0C3,
//   // Wallcling position.
//   // (Diddy, Lucario, Sheik, Squirtle)
//   PassiveWall = 0x0C4,
//   // Stunned from shield break.
//   // (All?)
//   FuraFura = 0x0C5,
//   // Standing up from knockdown after shield break, facing down.
//   // (All?)
//   FuraFuraStartD = 0x0C6,
//   // Standing up from knockdown after shield break, facing up.
//   // (All?)
//   FuraFuraStartU = 0x0C7,
//   // Recovering from shield stun.
//   // (All?)
//   FuraFuraEnd = 0x0C8,
//   // Falling asleep.
//   // (All?)
//   FuraSleepStart = 0x0C9,
//   // Zzzzzzzzz...
//   // (All?)
//   FuraSleepLoop = 0x0CA,
//   // Waking up from sleep.
//   // (All?)
//   FuraSleepEnd = 0x0CB,
//   // Being eaten by Kirby, Dedede, or Wario.
//   // (All?)
//   Swallowed = 0x0CC,
//   // Dropping through a platform.
//   // (All?)
//   Pass = 0x0CD,
//   // Beginning to teeter over a ledge (Ottotto).
//   // (All?)
//   Ottotto = 0x0CE,
//   // Teetering over a ledge.
//   // (All?)
//   OttottoWait = 0x0CF,
//   // Bouncing off a wall or ceiling while tumbling or reeling.
//   // (All?)
//   WallDamage = 0x0D0,
//   // Hitting a ceiling while jumping.
//   // (All?)
//   StopCeil = 0x0D1,
//   // Running into a wall.
//   // (All?)
//   StopWall = 0x0D2,
//   // 	All?
//   StopCeil = 0x0D3,
//   // Slipping backwards off a platform (such as being hit off while shielding).
//   // (All?)
//   MissFoot = 0x0D4,
//   // Grabbing the ledge.
//   // (All?)
//   CliffCatch = 0x0D5,
//   // Waiting on the ledge.
//   // (All?)
//   CliffWait = 0x0D6,
//   // Ledge attack, under 100% damage.
//   // (All?)
//   CliffAttackQuick = 0x0D7,
//   // Standing from ledge, under 100% damage.
//   // (All?)
//   CliffClimbQuick = 0x0D8,
//   // Rolling from ledge, under 100% damage.
//   // (All?)
//   CliffEscapeQuick = 0x0D9,
//   // Preparing to jump from ledge, under 100% damage.
//   // (All?)
//   CliffJumpQuick1 = 0x0DA,
//   // Jumping off the ledge, under 100% damage.
//   // (All?)
//   CliffJumpQuick2 = 0x0DB,
//   // Ledge attack, over 100% damage.
//   // (All?)
//   CliffAttackSlow = 0x0DC,
//   // Standing from ledge, over 100% damage.
//   // (All?)
//   CliffClimbSlow = 0x0DD,
//   // Rolling from ledge, over 100% damage.
//   // (All?)
//   CliffEscapeSlow = 0x0DE,
//   // Preparing to jump from ledge, over 100% damage.
//   // (All?)
//   CliffJumpSlow1 = 0x0DF,
//   // Jumping off the ledge, over 100% damage.
//   // (All?)
//   CliffJumpSlow2 = 0x0E0,
//   // Tripping/slipping while already sitting on the floor.
//   // (All?)
//   SlipDown = 0x0E1,
//   // Tripping/slipping while standing.
//   // (All?)
//   Slip = 0x0E2,
//   // Tripping/slipping while turning around (while running).
//   // (All?)
//   SlipTurn = 0x0E3,
//   // Tripping/slipping while starting a dash.
//   // (All?)
//   SlipDash = 0x0E4,
//   // Tripping/slipping idle animation.
//   // (All?)
//   SlipWait = 0x0E5,
//   // Standing up from a trip.
//   // (All?)
//   SlipStand = 0x0E6,
//   // Attacking from a trip.
//   // (All?)
//   SlipAttack = 0x0E7,
//   // Rolling forward from a trip.
//   // (All?)
//   SlipEscapeF = 0x0E8,
//   // Rolling backward from a trip.
//   // (All?)
//   SlipEscapeB = 0x0E9,
//   // Use of Zair/tether recovery (z-air).
//   // (All?)
//   AirCatch = 0x0EA,
//   // Hanging from an edge after using a tether recovery (?).
//   // (All?)
//   AirCatchPose = 0x0EB,
//   // Tether recovery connects with an edge (?).
//   // (All?)
//   AirCatchHit = 0x0EC,
//   // All?
//   AirCatch = 0x0ED, 
//   // Swimming up to the surface.
//   // (All?)
//   SwimRise = 0x0EE,
//   // Reaching the surface of water.
//   // (All?)
//   SwimUp = 0x0EF,
//   // Reaching the surface of water while at immediate panic level (?).
//   // (All?)
//   SwimUpDamage = 0x0F0,
//   // Idle animation in the water.
//   // (All?)
//   Swim = 0x0F1,
//   // Swimming forward.
//   // (All?)
//   SwimF = 0x0F2,
//   // Stop swimming forward.
//   // (All?)
//   SwimEnd = 0x0F3,
//   // Turning around in the water.
//   // (All?)
//   SwimTurn = 0x0F4,
//   // Drowning animation in the water.
//   // (All?)
//   SwimDrown = 0x0F5,
//   // Falling down back into the water to death.
//   // (All?)
//   SwimDrownOut = 0x0F6,
//   // Picking up a small item.
//   // (All?)
//   LightGet = 0x0F7,
//   // Picking up a small item while walking.
//   // (All?)
//   LightWalkGet = 0x0F8,
//   // Picking up a food item.
//   // (All?)
//   LightEat = 0x0F9,
//   // Picking up a food item while walking.
//   // (All?)
//   LightWalkEat = 0x0FA,
//   // Picking up a heavy item.
//   // (All?)
//   HeavyGet = 0x0FB,
//   // Taking a step with a heavy item.
//   // (All?)
//   HeavyWalk1 = 0x0FC,
//   // Taking another step with a heavy item.
//   // (All?)
//   HeavyWalk2 = 0x0FD,
//   // Dropping a small item.
//   // (All?)
//   LightThrowDrop = 0x0FE,
//   // Throwing a small item forward.
//   // (All?)
//   LightThrowF = 0x0FF,
//   // Throwing a small item backward.
//   // (All?)
//   LightThrowB = 0x100,
//   // Throwing a small item upward.
//   // (All?)
//   LightThrowHi = 0x101,
//   // Throwing a small item downward.
//   // (All?)
//   LightThrowLw = 0x102,
//   // Smash-throwing a small item forward (?).
//   // (All?)
//   LightThrowF = 0x103,
//   // Smash-throwing a small item backward (?).
//   // (All?)
//   LightThrowB = 0x104,
//   // Smash-throwing a small item upward (?).
//   // (All?)
//   LightThrowHi = 0x105,
//   // Smash-throwing a small item downward (?).
//   // (All?)
//   LightThrowLw = 0x106,
//   // Throwing a small item while running.
//   // (All?)
//   LightThrowDash = 0x107,
//   // Throwing a small item forward while in the air.
//   // (All?)
//   LightThrowAirF = 0x108,
//   // Throwing a small item backward while in the air.
//   // (All?)
//   LightThrowAirB = 0x109,
//   // Throwing a small item upward while in the air.
//   // (All?)
//   LightThrowAirHi = 0x10A,
//   // Throwing a small item downward while in the air.
//   // (All?)
//   LightThrowAirLw = 0x10B,
//   // Smash-throwing a small item forward while in the air (?).
//   // (All?)
//   LightThrowAirF = 0x10C,
//   // Smash-throwing a small item backward while in the air (?).
//   // (All?)
//   LightThrowAirB = 0x10D,
//   // Smash-throwing a small item upward while in the air (?).
//   // (All?)
//   LightThrowAirHi = 0x10E,
//   // Smash-throwing a small item downward while in the air (?).
//   // (All?)
//   LightThrowAirLw = 0x10F,
//   // Throwing a heavy item forward.
//   // (All?)
//   HeavyThrowF = 0x110,
//   // Throwing a heavy item backward.
//   // (All?)
//   HeavyThrowB = 0x111,
//   // Throwing a heavy item upward.
//   // (All?)
//   HeavyThrowHi = 0x112,
//   // Throwing a heavy item downward.
//   // (All?)
//   HeavyThrowLw = 0x113,
//   // Smash-throwing a heavy item forward (?).
//   // (All?)
//   HeavyThrowF = 0x114,
//   // Smash-throwing a heavy item backward (?).
//   // (All?)
//   HeavyThrowB = 0x115,
//   // Smash-throwing a heavy item upward (?).
//   // (All?)
//   HeavyThrowHi = 0x116,
//   // Smash-throwing a heavy item downward (?).
//   // (All?)
//   HeavyThrowLw = 0x117,
//   // Throwing a Trophy Stand forward.
//   // (All?)
//   SmashThrowF = 0x118,
//   // Throwing a Trophy Stand backward.
//   // (All?)
//   SmashThrowB = 0x119,
//   // Throwing a Trophy Stand upward.
//   // (All?)
//   SmashThrowHi = 0x11A,
//   // Throwing a Trophy Stand downward.
//   // (All?)
//   SmashThrowLw = 0x11B,
//   // Throwing a Trophy Stand while running.
//   // (All?)
//   SmashThrowDash = 0x11C,
//   // Throwing a Trophy Stand forward while in the air.
//   // (All?)
//   SmashThrowAirF = 0x11D,
//   // Throwing a Trophy Stand backward while in the air.
//   // (All?)
//   SmashThrowAirB = 0x11E,
//   // Throwing a Trophy Stand upward while in the air.
//   // (All?)
//   SmashThrowAirHi = 0x11F,
//   // Throwing a Trophy Stand downward while in the air.
//   // (All?)
//   SmashThrowAirLw = 0x120,
//   // Swinging a Beam Sword (neutral attack).
//   // (All?)
//   Swing1 = 0x121,
//   // Swinging a Beam Sword (f-tilt).
//   // (All?)
//   Swing3 = 0x122,
//   // Winding up a Beam Sword (f-smash).
//   // (All?)
//   Swing4Start = 0x123,
//   // Swinging a Beam Sword (f-smash).
//   // (All?)
//   Swing4 = 0x124,
//   // Swinging a Beam Sword a second time (f-smash).
//   // (Captain Falcon)
//   Swing42 = 0x125,
//   // Charging a Beam Sword (f-smash).
//   // (All?)
//   Swing4Hold = 0x126,
//   // Swinging a Beam Sword while running.
//   // (All?)
//   SwingDash = 0x127,
//   // Swinging a Home-Run Bat (neutral attack).
//   // (All?)
//   Swing1 = 0x128,
//   // Swinging a Home-Run Bat (f-tilt).
//   // (All?)
//   Swing3 = 0x129,
//   // Swinging a Home-Run Bat 4 REALZ (f-smash).
//   // (All?)
//   Swing4Bat = 0x12A,
//   // Swinging a Home-Run Bat while running.
//   // (All?)
//   SwingDash = 0x12B,
//   // Swinging a Fan (neutral attack).
//   // (All?)
//   Swing1 = 0x12C,
//   // Swing a Fan (f-tilt).
//   // (All?)
//   Swing3 = 0x12D,
//   // Winding up a Fan (f-smash).
//   // (All?)
//   Swing4Start = 0x12E,
//   // Swinging a Fan (f-smash).
//   // (All?)
//   Swing4 = 0x12F,
//   // Swinging a Fan a second time (f-smash).
//   // (Captain Falcon)
//   Swing42 = 0x130,
//   // Charging a Fan (f-smash).
//   // (All?)
//   Swing4Hold = 0x131,
//   // Swinging a Fan while running.
//   // (All?)
//   SwingDash = 0x132,
//   // Swinging a Star Rod (neutral attack).
//   // (All?)
//   Swing1 = 0x133,
//   // Swinging a Star Rod (f-tilt).
//   // (All?)
//   Swing3 = 0x134,
//   // Winding up a Star Rod (f-smash).
//   // (All?)
//   Swing4Start = 0x135,
//   // Swinging a Star Rod (f-smash).
//   // (All?)
//   Swing4 = 0x136,
//   // Swinging a Star Rod a second time (f-smash).
//   // (Captain Falcon)
//   Swing42 = 0x137,
//   // Charging a Star Rod (f-smash).
//   // (All?)
//   Swing4Hold = 0x138,
//   // Swinging a Star Rod while running.
//   // (All?)
//   SwingDash = 0x139,
//   // Swinging a Lip's Stick (neutral attack).
//   // (All?)
//   Swing1 = 0x13A,
//   // Swing a Lip's Stick (f-tilt).
//   // (All?)
//   Swing3 = 0x13B,
//   // Winding up a Lip's Stick (f-smash).
//   // (All?)
//   Swing4Start = 0x13C,
//   // Swinging a Lip's Stick (f-smash).
//   // (All?)
//   Swing4 = 0x13D,
//   // Swinging a Lip's Stick a second time (f-smash).
//   // (Captain Falcon)
//   Swing42 = 0x13E,
//   // Charging a Lip's Stick (f-smash).
//   // (All?)
//   Swing4Hold = 0x13F,
//   // Swinging a Lip's Stick while running.
//   // (All?)
//   SwingDash = 0x140,
//   // Standing with a Hammer.
//   // (All?)
//   ItemHammerWait = 0x141,
//   // Walking with a Hammer.
//   // (All?)
//   ItemHammerMove = 0x142,
//   // Jumping or falling with a Hammer.
//   // (All?)
//   ItemHammerAir = 0x143,
//   // Standing with a Golden Hammer (?).
//   // (All?)
//   ItemHammerWait = 0x144,
//   // Walking with a Golden Hammer (?).
//   // (All?)
//   ItemHammerMove = 0x145,
//   // Jumping or falling with a Golden Hammer (?).
//   // (All?)
//   ItemHammerAir = 0x146,
//   // Riding a Warp Star (?).
//   // (All?)
//   ItemDragoonRide = 0x147,
//   // Jumping with a Screw Attack.
//   // (All?)
//   ItemScrew = 0x148,
//   // Double jumping with a Screw Attack (?).
//   // (All?)
//   ItemScrew = 0x149,
//   // Falling after using a Screw Attack.
//   // (All?)
//   ItemScrewFall = 0x14A,
//   // All?
//   ItemKick = 0x14B 
//   // Jumping after completing the Dragoon.
//   // (All?)
//   ItemDragoonGet = 0x14C,
//   // Riding the Dragoon.
//   // (All?)
//   ItemDragoonRide = 0x14D,
//   // Growing from a Super Mushroom/Lightning.
//   // (All?)
//   ItemBig = 0x14E,
//   // Shrinking from a Poison Mushroom/Lightning.
//   // (All?)
//   ItemSmall = 0x14F,
//   // Standing while holding a shooting item.
//   // (All?)
//   ItemLegsWait = 0x150,
//   // Walking slowly while holding a shooting item.
//   // (All?)
//   ItemLegsSlowF = 0x151,
//   // Walking medium speed while holding a shooting item.
//   // (All?)
//   ItemLegsMiddleF = 0x152,
//   // Walking quickly while holding a shooting item.
//   // (All?)
//   ItemLegsFastF = 0x153,
//   // Stopping while holding a shooting item.
//   // (All?)
//   ItemLegsBrakeF = 0x154,
//   // Dashing while holding a shooting item.
//   // (All?)
//   ItemLegsDashF = 0x155,
//   // Reversing slowly while holding a shooting item.
//   // (All?)
//   ItemLegsSlowB = 0x156,
//   // Reversing medium speed while holding a shooting item.
//   // (All?)
//   ItemLegsMiddleB = 0x157,
//   // Reversing quickly while holding a shooting item.
//   // (All?)
//   ItemLegsFastB = 0x158,
//   // Stopping from reversing while holding a shooting item.
//   // (All?)
//   ItemLegsBrakeB = 0x159,
//   // Dashing backwards while holding a shooting item.
//   // (All?)
//   ItemLegsDashB = 0x15A,
//   // Preparing to jump while holding a shooting item.
//   // (All?)
//   ItemLegsJumpSquat = 0x15B,
//   // Landing while holding a shooting item.
//   // (All?)
//   ItemLegsLanding = 0x15C,
//   // Shooting a Ray Gun (?).
//   // (All?)
//   ItemShoot = 0x15D,
//   // Shooting a Ray Gun (?) while in the air.
//   // (All?)
//   ItemShootAir = 0x15E,
//   // 	All?
//   ItemShoot = 0x15F,
//   // 	All?
//   ItemShootAir = 0x160,
//   // Shooting a Fire Flower (?).
//   // (All?)
//   ItemShoot = 0x161,
//   // Shooting a Fire Flower (?) while in the air.
//   // (All?)
//   ItemShootAir = 0x162,
//   // Taking hold of a Super Scope.
//   // (All?)
//   ItemScopeStart = 0x163,
//   // Shooting weak Super Scope shots.
//   // (All?)
//   ItemScopeRapid = 0x164,
//   // Shooting a charged Super Scope shot.
//   // (All?)
//   ItemScopeFire = 0x165,
//   // Putting down a Super Scope.
//   // (All?)
//   ItemScopeEnd = 0x166,
//   // Taking hold of a Super Scope while in the air.
//   // (All?)
//   ItemScopeAirStart = 0x167,
//   // Shooting weak Super Scope shots while in the air.
//   // (All?)
//   ItemScopeAirRapid = 0x168,
//   // Shooting a charged Super Scope shot while in the air.
//   // (All?)
//   ItemScopeAirFire = 0x169,
//   // Putting down a Super Scope while in the air.
//   // (All?)
//   ItemScopeAirEnd = 0x16A,
//   // 	All?
//   ItemScopeStart = 0x16B,
//   // 	All?
//   ItemScopeRapid = 0x16C,
//   // 	All?
//   ItemScopeFire = 0x16D,
//   // 	All?
//   ItemScopeEnd = 0x16E,
//   // 	All?
//   ItemScopeAirStart = 0x16F,
//   // 	All?
//   ItemScopeAirRapid = 0x170,
//   // 	All?
//   ItemScopeAirFire = 0x171,
//   // 	All?
//   ItemScopeAirEnd = 0x172,
//   // Holding a Cracker Launcher.
//   // (All?)
//   ItemLauncher = 0x173,
//   // Firing a Cracker Launcher.
//   // (All?)
//   ItemLauncherFire = 0x174,
//   // Firing a Cracker Launcher while in the air.
//   // (All?)
//   ItemLauncherAirFire = 0x175,
//   // 	All?
//   ItemLauncher = 0x176,
//   // 	All?
//   ItemLauncherFire = 0x177,
//   // 	All?
//   ItemLauncherAirFire = 0x178,
//   // Falling while holding a Cracker Launcher.
//   // (All?)
//   ItemLauncherFall = 0x179,
//   // 	All?
//   ItemLauncherAir = 0x17A,
//   // Throwing up an assist trophy.
//   // (All?)
//   ItemAssist = 0x17B,
//   // Idle with curry.
//   // (All?)
//   GekikaraWait = 0x17C,
//   // Screw Attack jump.
//   // (All?)
//   ItemScrew = 0x17D,
//   // Riding Slide Launcher? (the thing in SSE and Target Test 3)
//   // (All?)
//   Guard = 0x17E,
//   // Idle on ladder.
//   // (All?)
//   LadderWait = 0x17F,
//   // Climbing a ladder.
//   // (All?)
//   LadderUp = 0x180,
//   // Descending a ladder.
//   // (All?)
//   LadderDown = 0x181,
//   // Getting onto a ladder (facing right).
//   // (All?)
//   LadderCatchR = 0x182,
//   // Getting onto a ladder (facing left).
//   // (All?)
//   LadderCatchL = 0x183,
//   // Getting onto a ladder from the air (facing right).
//   // (All?)
//   LadderCatchAirR = 0x184,
//   // Getting onto a ladder from the air (facing left).
//   // (All?)
//   LadderCatchAirL = 0x185,
//   // Reaching the top of a ladder (facing right).
//   // (All?)
//   LadderCatchEndR = 0x186,
//   // Reaching the top of a ladder (facing left).
//   // (All?)
//   LadderCatchEndL = 0x187,
//   // 	All?
//   RopeCatch = 0x188,
//   // 	All?
//   RopeFishing = 0x189,
//   // Caught in Wario's Chomp (?)
//   // (All?)
//   SpecialNBittenStart = 0x18A,
//   // 	All?
//   SpecialNBitten = 0x18B,
//   // 	All?
//   SpecialNBittenEnd = 0x18C,
//   // 	All?
//   SpecialAirNBittenStart = 0x18D,
//   // 	All?
//   SpecialAirNBitten = 0x18E,
//   // 	All?
//   SpecialAirNBittenEnd = 0x18F,
//   // 	All?
//   SpecialNDxBittenStart = 0x190,
//   // 	All?
//   SpecialNDxBitten = 0x191,
//   // 	All?
//   SpecialNDxBittenEnd = 0x192,
//   // 	All?
//   SpecialAirNDxBittenStart = 0x193,
//   // 	All?
//   SpecialAirNDxBitten = 0x194,
//   // 	All?
//   SpecialAirNDxBittenEnd = 0x195,
//   // 	All?
//   SpecialNBigBittenStart = 0x196,
//   // 	All?
//   SpecialNBigBitten = 0x197,
//   // 	All?
//   SpecialNBigBittenEnd = 0x198,
//   // 	All?
//   SpecialAirNBigBittenStart = 0x199,
//   // 	All?
//   SpecialAirNBigBitten = 0x19A,
//   // 	All?
//   SpecialAirNBigBittenEnd = 0x19B,
//   // Caught in Falcon Dive (?)
//   // (All?)
//   SpecialHiCapture = 0x19C,
//   // 	All?
//   SpecialHiDxCapture = 0x19D,
//   // Caught in Monkey Flip (?)
//   // (All?)
//   SpecialSStickCapture = 0x19E,
//   // Being attacked by Diddy during a Monkey Flip (?)
//   // (All?)
//   SpecialSStickAttackCapture = 0x19F,
//   // Being jumped on by Diddy during a Monkey Flip (?)
//   // (All?)
//   SpecialSStickJumpCapture = 0x1A0,
//   // 	All?
//   SpecialSDxStickCapture = 0x1A1,
//   // 	All?
//   SpecialSDxStickAttackCapture = 0x1A2,
//   // 	All?
//   SpecialSDXStickJumpCapture = 0x1A3,
//   // Held in DK's Cargo
//   // (All?)
//   ThrownZitabata = 0x1A4,
//   // 	All?
//   ThrownDxZitabata = 0x1A5,
//   // 	All?
//   ThrownGirlZitabata = 0x1A6,
//   // Being thrown out of DK's Cargo
//   // (All?)
//   ThrownFF = 0x1A7,
//   // 	All?
//   ThrownFB = 0x1A8,
//   // 	All?
//   ThrownFHi = 0x1A9,
//   // 	All?
//   ThrownFLw = 0x1AA,
//   // 	All?
//   ThrownDxFF = 0x1AB,
//   // 	All?
//   ThrownDxFB = 0x1AC,
//   // 	All?
//   ThrownDxFHi = 0x1AD,
//   // 	All?
//   ThrownDxFLw = 0x1AE,
//   // Caught in Dark Dive
//   // (All?)
//   GanonSpecialHiCapture = 0x1AF,
//   // 	All?
//   GanonSpecialHiDxCapture = 0x1B0,
//   // Caught in grounded Flame Choke (?)
//   // (All?)
//   SpecialSCapture = 0x1B1,
//   // Caught in aerial Flame Choke (?)
//   // (All?)
//   SpecailAirSCatchCapture = 0x1B2,
//   // 	All?
//   SpecialAirSFallCapture = 0x1B3,
//   // 	All?
//   SpecialAirSCapture = 0x1B4,
//   // 	All?
//   SpecialSDxCapture = 0x1B5,
//   // 	All?
//   SpecailAirSDxCatchCapture = 0x1B6,
//   // 	All?
//   SpecialAirSDxFallCapture = 0x1B7,
//   // 	All?
//   SpecialAirSDxCapture = 0x1B8,
//   // Trapped in Yoshi's Egg.
//   // (All?)
//   SpecialNEgg = 0x1B9,
//   // Something to do with being held in a side special move that's not in the process of throwing (Flying Slam?)
//   // (All?)
//   SpecialSZitabata = 0x1BA,
//   // 	All?
//   SpecialSDxZitabata = 0x1BB,
//   // Up taunt, facing right.
//   // (All?)
//   AppealHiR = 0x1BC,
//   // Up taunt, facing left.
//   // (All?)
//   AppealHiL = 0x1BD,
//   // Side taunt, facing right.
//   // (All?)
//   AppealSR = 0x1BE,
//   // Side taunt, facing left.
//   // (All?)
//   AppealSL = 0x1BF,
//   // Down taunt, facing right.
//   // (All?)
//   AppealLwR = 0x1C0,
//   // Down taunt, facing left.
//   // (All?)
//   AppealLwL = 0x1C1,
//   // Entrance, facing right.
//   // (All?)
//   EntryR = 0x1C2,
//   // Entrance, facing left.
//   // (All?)
//   EntryL = 0x1C3,
//   // Victory 1 animation on results screen.
//   // (All?)
//   Win1 = 0x1C4,
//   // Victory 1 looping animation, comes after Win1.
//   // (All?)
//   Win1Wait = 0x1C5,
//   // Victory 2 animation on results screen.
//   // (All?)
//   Win2 = 0x1C6,
//   // Victory 2 looping animation, comes after Win2.
//   // (All?)
//   Win2Wait = 0x1C7,
//   // Victory 3 animation on results screen.
//   // (All?)
//   Win3 = 0x1C8,
//   // Victory 3 looping animation, comes after Win3.
//   // (All?)
//   Win3Wait = 0x1C9,
//   // Losing animation on results screen.
//   // (All?)
//   Lose = 0x1CA,
//   // Facial expression when being hit/tumbling.
//   // (All?)
//   DamageFace = 0x1CB,
//   // 	All?
//   Dark = 0x1CC,
//   // 	All?
//   Spycloak = 0x1CD,
// };

enum CHAR_ID {
  Mario = 0x0,
  Donkey = 0x1,
  Link = 0x2,
  Samus = 0x3,
  Yoshi = 0x4,
  Kirby = 0x5,
  Fox = 0x6,
  Pikachu = 0x7,
  Luigi = 0x8,
  Falcon = 0x9,
  Ness = 0xa,
  Bowser = 0xb,
  Peach = 0xc,
  Zelda = 0xd,
  Sheik = 0xe,
  Popo = 0xf,
  Nana = 0x10,
  Marth = 0x11,
  Gamewatch = 0x12,
  Falco = 0x13,
  Ganondorf = 0x14,
  Wario = 0x15,
  Metaknight = 0x16,
  Pit = 0x17,
  ZSS = 0x18,
  Pikmin = 0x19,
  Lucas = 0x1a,
  Diddy = 0x1b,
  Trainer = 0x1c,
  Lizardon = 0x1d,
  Zenigame = 0x1e,
  Fushigisou_Mewtwo = 0x1f,
  DDD = 0x20,
  Lucario = 0x21,
  Ike = 0x22,
  Robot = 0x23,
  Purin = 0x24,
  Roy = 0x28,
  Toonlink = 0x29,
  Knuckles = 0x2b,
  Wolf = 0x2c,
  Snake = 0x2e,
  Sonic = 0x2f
};

//A unique id given when loading the character
typedef int EntryID;

struct ftCancelModule;
struct FtOwner;
struct Fighter : public StageObject {
	
	ftCancelModule* getCancelModule();
	
	//can't use constructor because Brawl constructor already calls base class constructor
	//so it would be called multiple times
	//Fighter* ct(EntryID id, int, int);
	//IpHuman* getInput();
	FtOwner* getOwner();

	int getFtKind();
	void setMetal(double health, bool setEffect, int unk3);
	void setCurry(bool setEffect, int unk2);
	void setHammer(bool setEffect, int unk2);
	void setSuperStar(bool setEffect, unsigned int unk2, int unk3);
	void setFlower(double rate, bool setEffect, int unk3, double size, int unk5);
	void setHeartSwap(EntryID givingPlayerEntryId, int unk2, bool setEffect);
	void setSlow(bool setEffect, int slowStrength, int duration, int unk4);
	void startScaling(bool setEffect, bool isPoison);

	void endFinal(int unk1, int unk2, int unk3);

	char _spacer_vt18[0x100 - sizeof(StageObject)];
	Fighter_vtable18* vtable18;

	char _spacer_ft[0x110 - 0x100 - 4];
};

#define _getFtKind_Fighter ((int (*)(Fighter * This)) 0x8076b00c)

//first int is fighter id or somethign
//second int seems to not be used (immediatly overwritten)
//#define ct_Fighter ((Fighter* (*)(Fighter* This, EntryID, int, int, soModuleAccesser*)) 0x80830200)
//#define getInput_Fighter ((IpHuman* (*)(Fighter* This)) 0x8083ae38)
#define getOwner_Fighter ((FtOwner * const (*)(Fighter * This)) 0x8083ae24)

#define setMetal_Fighter ((void (*)(Fighter* self, double health, bool setEffect, int unk3)) 0x80843110)
#define setCurry_Fighter ((void (*)(Fighter* self, bool setEffect, int unk2)) 0x80843314)
#define setHammer_Fighter ((void (*)(Fighter* self, bool setEffect, int unk2)) 0x808436d0)
#define setSuperStar_Fighter ((void (*)(Fighter* self, bool setEffect, unsigned int unk2, int unk3)) 0x80843978)
#define setFlower_Fighter ((void (*)(Fighter* self, double rate, bool setEffect, int unk3, double size, int unk5)) 0x80843ce8)
#define setHeartSwap_Fighter ((void (*)(Fighter* self, EntryID givingPlayerEntryId, int unk2, bool setEffect)) 0x80844144)
#define setSlow_Fighter ((void (*)(Fighter* self, bool setEffect, int slowStrength, int duration, int unk4)) 0x80842e6c)
#define startScaling_Fighter ((void (*)(Fighter* self, bool setEffect, bool isPoison)) 0x80841ad4)
#define endFinal_Fighter ((void (*)(Fighter* self, int unk1, int unk2, int unk3)) 0x80838318)

#endif