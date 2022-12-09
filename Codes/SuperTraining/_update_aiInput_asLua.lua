-- FIN = IpHuman[!update]() _then_ return

-- ---

-- cpu level values
-- #const LV1 = 0
-- #const LV2 = 15
-- #const LV3 = 21
-- #const LV4 = 31
-- #const LV5 = 42
-- #const LV6 = 48
-- #const LV7 = 60
-- #const LV8 = 75
-- #const LV9 = 100

-- ---

local nullptr
local aiInput
local cpuType
local fighterNum
local AIPD
local childAiInput
local aiStat
local transition_bitfield
local target
local myself

function change_md(mode, target, script, unk)
end

function switch(t)
  t.case = function (self,x)
    local f=self[x] or self.default
    if f then
      if type(f)=="function" then
        f(x,self)
      else
        error("case "..tostring(x).." not a function")
      end
    end
  end
  return t
end

local floorInfo
local aiMd
local MD = {
  none = 0x0,
  attack = 0x1,
  defend = 0x2,
  _return = 0x3,
  down = 0x4,
  away_5 = 0x5,
  chase_6 = 0x6,
  away_7 = 0x7,
  unique = 0x8,
  ladder = 0x9,
  piyo = 0xa,
  chase_b = 0xb,
  drop_get = 0xc,
  item_shoot = 0xd,
  dragoon = 0xe,
  final = 0xf,
  hammer = 0x10,
  catch = 0x11,
  swim = 0x12,
  glide = 0x13,
  relax = 0x14,
  slide = 0x15,
  away_16 = 0x16,
}
local AIRoutine = {
  Nothing = 0x0,
  Unk10_act_m_wait = 0x10,
  Unk20 = 0x20,
  Unk21 = 0x21,
  Unk22 = 0x22,
  Unk23 = 0x23,
  Unk24 = 0x24,
  RunAway30 = 0x30,
  RunAway31 = 0x31,
  RunAway32 = 0x32,
  RunAway33 = 0x33,
  WalkUp = 0x40,
  Hop = 0x50,
  SHopTowards = 0x51,
  FHopTowards = 0x52,
  SHopNeutral = 0x53,
  DJNeutral = 0x54,
  FHopAway = 0x55,
  SHopAway = 0x56,
  SHopDJ = 0x57,
  FHopDJ = 0x58,
  DashUp = 0x60,
  RollBase = 0x70,
  RollF = 0x71,
  RollB = 0x72,
  GetBehind0 = 0x80,
  Unk90_act_m_stroll = 0x90,
  Gr_Atk_1010 = 0x1010,
  Gr_Atk_1020 = 0x1020,
  Gr_Atk_1030 = 0x1030,
  Gr_Atk_1040 = 0x1040,
  Gr_Atk_1050 = 0x1050,
  Gr_Atk_Fall_Chase = 0x1060,
  Air_1070 = 0x1070,
  Air_1071 = 0x1071,
  Air_1072 = 0x1072,
  Air_1073 = 0x1073,
  Air_1074 = 0x1074,
  Air_1075 = 0x1075,
  Air_1076 = 0x1076,
  Air_1077 = 0x1077,
  Air_1078 = 0x1078,
  SlotExec_1080 = 0x1080,
  SlotExec_1090 = 0x1090,
  BlockRecovery_10A0 = 0x10a0,
  BlockRecovery_10A1 = 0x10a1,
  BlockRecovery_10A2 = 0x10a2,
  BlockRecovery_10A3 = 0x10a3,
  HitBehindMaybe_10D0 = 0x10d0,
  InterceptCliff = 0x10f0,
  Unk1100_act_a_goro = 0x1100,
  GrabSuccess = 0x1120,
  Unk1130 = 0x1130,
  JumpOffstageEdgeguard = 0x1140,
  Unk1150_act_a_brk_wall = 0x1150,
  AIPDDef10 = 0x1160,
  Unk1170 = 0x1170,
  JumpOnStage_act_r_jump_aft_shot = 0x2010,
  JumpOnStage1 = 0x2011,
  JumpOnStage2 = 0x2012,
  JumpOnStage3 = 0x2013,
  JumpOnStage4 = 0x2014,
  JumpOnStage5 = 0x2015,
  JumpOnStage6 = 0x2016,
  JumpOnStage7 = 0x2017,
  JumpOnStage8 = 0x2018,
  JumpOnStage9 = 0x2019,
  SideBLedge = 0x2020,
  JumpDirLedge = 0x2030,
  EdgeRecover_act_r_jump_aft_shot = 0x2040,
  EdgeRecover1 = 0x2041,
  EdgeRecover2 = 0x2042,
  EdgeRecover3 = 0x2043,
  EdgeRecover4 = 0x2044,
  EdgeRecover5 = 0x2045,
  EdgeRecover6 = 0x2046,
  EdgeRecover7 = 0x2047,
  EdgeRecover8 = 0x2048,
  EdgeRecover9 = 0x2049,
  OnGotDamaged_act_r_fall = 0x2050,
  OnGotDamaged1 = 0x2051,
  LedgeHang_act_r_dfjump = 0x2060,
  Lying = 0x2070,
  Defensive_act_d_wait_3010 = 0x3010,
  Defensive_Shield = 0x3020,
  Defensive_act_d_wait_3030 = 0x3030,
  Defensive_act_d_wait_3040 = 0x3040,
  Defensive_act_d_wait_3050 = 0x3050,
  Unk3060_act_d_air_sway = 0x3060,
  Defensive_item_3080 = 0x3080,
  Defensive_item_3090 = 0x3090,
  Spotdodge = 0x30a0,
  DodgeWhenAttacked = 0x30b0,
  AirDodgeWhenAttacked = 0x30d0,
  Reflect = 0x30e0,
  GroundCombo = 0x4010,
  AerialCombo = 0x4020,
  SlotBase = 0x6010,
  Slot0 = 0x6011,
  Slot1 = 0x6012,
  Slot2 = 0x6013,
  Slot3 = 0x6014,
  Slot4 = 0x6015,
  Slot5 = 0x6016,
  Slot6 = 0x6017,
  Slot7 = 0x6018,
  Slot8 = 0x6019,
  Slot9 = 0x601a,
  Slot10 = 0x601b,
  Slot11 = 0x601c,
  Slot12 = 0x601d,
  Slot13 = 0x601e,
  Slot14 = 0x601f,
  Slot15 = 0x6020,
  Slot11_2 = 0x6021,
  Slot12_2 = 0x6022,
  Slot13_2 = 0x6023,
  Slot14_2 = 0x6024,
  Slot15_2 = 0x6025,
  Slot16 = 0x6026,
  Slot17 = 0x6027,
  Slot18 = 0x6028,
  Slot19 = 0x6029,
  Slot20 = 0x602a,
  Slot21 = 0x602b,
  Slot22 = 0x602c,
  Slot23 = 0x602d,
  GroundBase = 0x6030,
  Jab123 = 0x6031,
  FTilt = 0x6032,
  UTilt = 0x6033,
  DTilt = 0x6034,
  FSmash = 0x6035,
  USmash = 0x6036,
  DSmash = 0x6037,
  NSpecial = 0x6038,
  SSpecial = 0x6039,
  USpecial = 0x603a,
  DSpecial = 0x603b,
  Grab = 0x603c,
  Unk603D = 0x603d,
  AerialBase = 0x6040,
  Nair = 0x6041,
  FAir = 0x6042,
  BAir = 0x6043,
  UAir = 0x6044,
  DAir = 0x6045,
  NSpecialAir = 0x6046,
  SSpecialAir = 0x6047,
  USpecialAir = 0x6048,
  DSpecialAir = 0x6049,
  AutoThrow = 0x6100,
}

if aiInput[!0x3C] & 0x8000 == 0 then
  if "input not already updated"
  or cpuType == 0x19 then
    -- update inputs
    goto _END
  end
end
-- maybe for nana??
if aiInput[!0x3c] & 0x8 == 0
and fighterNum > 0 then
  goto _END
end

if "aiMgr not 'ready'" then
  if "human not updated" -- CHK LINE 185
  and cpuType ~= 0x19
  and AIPD ~= nullptr then
    if "ai can update" then
      -- move in some direction // CHK
      aiInput[!0xC] = 0x8000
      if "has child (nana)" then
        -- nana move towards popo
        childAiInput[!0xC] = 0x8000
      end
    end
  else
    -- update inputs
  end

  goto _END
end

if aiStat[!floorInfo] & 0x80000000 == 0 then
  goto _END
end

if "human not updated" -- CHK
and cpuType ~= 0x19
and AIPD ~= nullptr then
  -- clear buttons
  -- clear stick
  -- something with MadeinAiData

  if aiMd ~= MD.down
  and (aiStat[!0x1ba] == (0x4 or 0x6)
  or aiStat[!transition_bitfield] == 0xbe) then
    change_md(MD.down, -1, AIRoutine.Nothing, 0)
  end
  if aiMd ~= MD.ladder
  and aiStat[!0x1ba] == 0xb then
    change_md(MD.ladder, -1, AIRoutine.Nothing, 0)
  end

  if aiMd ~= MD.piyo
  and (cpuType == 0
    or cpuType > 3)
  and aiStat[!0x1ba] == 0x9 then
    if aiScript == Defensive_Shield (0x3020) then
      -- modifications to:
      --   - aiInput[!0x78]
      --   - aiInput[!0x7C]
      --   - aiInput[!0x80]
      change_md(MD.piyo, -1, AIRoutine.Nothing, 0)
    end
  if aiMd ~= MD.dragoon
  and aiStat[!0x1ba] == 0x0f then
    change_md(MD.dragoon, -1, AIRoutine.Nothing, 0)
  end
  if aiScript == AIRoutine.Unk1100_act_a_goro
  and aiStat[!floorInfo] & 0x20 ~= 0 then
    
    res = switch {
      [default] = function()
        change_md(0x65, get_nearest_target(), Unk1100_act_a_goro, 0)
      end
      MD.defend:
      MD.away_5:
      MD.away_7:
      MD.slide:
      MD.away_16:
        change_md(0x66, get_nearest_target(), Unk1100_act_a_goro, 0)
    }
    res:case(aiMd)

    aiAct[!0x111] = 0
  if aiScript ~= JumpOnStage_act_r_jump_aft_shot
  and aiScript ~= OnGotDamaged_act_r_fall
  and aiStat[!0x1ba] == 0x0e
    aiScript => JumpOnStage_act_r_jump_aft_shot
    if over ground
      aiScript => OnGotDamaged_act_r_fall
    change_md(MD.defend, -1, aiScript, 0)
  if aiMd ~= MD.catch
  and aiStat[!0x1ba] == 0xA
    change_md(MD.catch, -1, AIRoutine.Nothing, 0)
  if aiMd ~= MD.swim
  and aiStat[!0x1ba] == 0xC
    change_md(MD.swim, -1, AIRoutine.Nothing, 0)
  if aiMd ~= MD.glide
  and aiStat[!0x1ba] == 0x10
    change_md(MD.glide, -1, AIRoutine.Nothing, 0)
  if aiStat[!0x1ba] == 0xD
  and rand() < UNKNOWN_DOUBLE // CHK
    inputs |= Jump
  if aiMd ~=
    MD.away_7,
    MD.away_16,
    MD.return,
    MD.down,
    MD.ladder,
    MD.piyo,
    MD.dragoon,
    MD.catch,
    MD.swim,
    MD.glide,
    MD.slide,
    MD.final
  and aiStat[!floorInfo] & 0x800 == 0
    if aiMd == MD.hammer
    or aiStat[!floorInfo] & 0x1000 == 0
      // CHK THIS WHOLE THING
      if aiInput[!recoveryTimer] < 1
        * bool unk = true
        * float ownWidth

        if final (golden hammer)
          unk = false

        if (((unk)
            and aiStat[!floorInfo] & 0x10000 ~= 0)
          or aiStat[!position].y < (DATA + aiMgr[!0x36c]))
        or (aiStat[!position].x < (widthThing + aiMgr[!0x360])
          or (aiMgr[!0x364] - widthThing) < aiStat[!position].x)
          * targetAiStat
          * targetWidth

          if ((DATA + targetWidth) + aiMgr[!0x360]) <= aiStat[!position].x
          or (aiStat[!position].x <= targetAiStat[!position].x
            and aiStat[!position].x <= (aiMgr[!0x364] - (DATA + targetWidth)))
          or targetAiStat[!position].x <= aiStat0[!0x5c]
            change_md(MD.away_7, -1, AIRoutine.Nothing, 0)
        else
          * int unk2 = aiMgr[!0x370]

          if unk2 == 8
            if aiStat[!position].y < aiInput[!0xb8]
              change_md(MD.away_7, -1, AIRoutine.Nothing 0)
          else if unk2 < 8
            if unk2 == 5
            and aiStat[!position].x < aiInput[!0xbc]
              change_md(MD.away_7, -1, AIRoutine.Nothing, 0)
          else if unk2 == 0x1c
          and ptr(aiStat[!0xbc] + 0x40) & 0x1000 ~= 0
            change_md(MD.away_7, -1, AIRoutine.Nothing, 0)
      if aiMd == MD.away_7
        goto _END
    else
      aiInput[!recoveryTimer] -= 1
    if aiInput[!0x63] < 0x1
      if (aiStat[!floorInfo] & 0x40 ~= 0
          and ((grFree or airFree) 
        or aiStat[!0x1ac] == 0xd))
      and (aiMd ~= MD.item_shoot
        or aiInput[!0x50] > 1 
        and aiInput[!0x50] < 10
        and aiInput[!0x54] > 0x46)
        * int unk3 = 0
        if aiStat[!0x1ac] == 0xd
          unk3 = 0xd
        else
          * targetAiStat
          if aiMd == MD.chase_6
            if aiStat[!0x1b0] & 1 ~= 0
            and aiStat[!0x1ac] ~= 0
            and aiStat[!0x1ac] ~= 0x2a
              unk3 = 0xd
          // regular ai modes
          else if 0 < aiMd < 3
            * iVar9 = aiStat[!0x1ac]
            if iVar9 == 0x21
              goto $LAB_CCA0
            if iVar9 < 0x21
              if iVar9 == 0x12
                goto $LAB_CEA4
              if iVar9 < 0x12
                if iVar9 == 0x4
                $LAB_CCA0:
                if iVar9 == 0x21
                  if DATA < targetAiStat[!damage]
                  and rand() < DATA(0.7)
                    unk3 = 0xd
                else if iVar9 < 0x21
                  if iVar9 == 0x1f
                    * levelValue = aiParam.levelValue
                    * unk6 = rand()
                    if unk6 < DATA * DOUBLE_CONVERT(levelValue)
                      unk3 = 0xd
                  else if iVar9 < 0x1f
                  and iVar9 == 4
                  and DATA < aiStat[!damage]
                  and (unk6 = rand()) < DATA
                    unk3 = 0xd
                else if iVar9 == 0x3a
                  $LAB_CD7C:
                  if aiStat[!0x1b4] < 1
                    unk3 = 0xd
                else if iVar9 < 0x3a and iVar9 == 0x2e
                  if targetAiStat[!damage] < DATA
                    goto $LAB_CD7C
                  unk3 = 0xd
                  if unk3 == 0xd
                    * levelValue = aiParam.levelValue
                    * relTargetXPos = 0
                    * relTargetYPos = 0
                    if levelValue < 0x14
                      relTargetXPos = targetAiStat[!TopNPos].x
                      relTargetYPos = targetAiStat[!TopNPos].y
                    else
                      * unk10 = levelValue - 0x14
                      * xVelDiff = targetAiStat[!Velocity].x - aiStat[!Velocity].x
                      * yVelDiff = targetAiStat[!Velocity].y - aiStat[!Velocity].y
                      // CHK 0.375
                      relTargetXPos = xVelDiff * 0.375 + targetAIStat[!TopNPos].x
                      relTargetYPos = yVelDiff * 0.375 + targetAIStat[!TopNPos].y
                    targetWidth = get_width(targetAiStat)
                    ownWidth = get_width(aiStat)
                    * fVar4 = ownAiStat[!direction] * (relTargetXPos - ownAiStat[!TopNPos].x) - (ownWidth + targetWidth)
                    if 20 <= fVar4 <= 100
                    or 15 * targetAiStat[!Scale].y <= ABS(aiStat[!TopNPos].y - relTargetYPos)
                      unk3 = 0
                  goto $LAB_CF8C
              else if iVar9 == 0x1f
                goto $LAB_CCA0
            else
              if iVar9 ~= 0x36
                if iVar9 < 0x36
                  if (iVar9 == 0x2e)
                    goto $LAB_CCA0
                else if iVar9 == 0x3a
                  goto $LAB_CCA0
                goto $LAB_CF38
end
:: _END ::


-- #############################################
-- UPDATE AISTAT                
-- #############################################
