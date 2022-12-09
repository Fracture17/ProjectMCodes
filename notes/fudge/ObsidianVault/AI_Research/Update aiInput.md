the `aiInput` update function is the most important for handling all input that flows into a fighter. This doesn't only handle input from an AI, but it also controls a vast amount of decision making for the AI and is what enables human controller inputs to go through.

This document will attempt to display how the function works by breaking it down into various steps.

---

### Parameters:
The function only takes its own instance of `aiInput` as a parameter, dubbed `this` in the source code.

---

## Breaking it down:

```c
if ((this->unk_thing & 0x8000) == 0) {
	if (aiMgr::chkIpHumanUpdate(&param_1->aiChrIdx) || this->cpuType == 0x19) {
		IpHuman::update(this);
	}
	goto end;
}
```

```c
if (((selfAiInput->unk_thing & 8) == 0) &&
 	(iVar13 = aiChrIdx::GetFighterNo(&selfAiInput->aiChrIdx), 0 < iVar13))
	goto end;
```

```c
  if (!aiMgr::isReady()) {
    if ((!aiMgr::chkIpHumanUpdate(&selfAiInput->aiChrIdx) 
	    && selfAiInput->cpuType != '\x19') 
	    && (selfAiInput->aiParam).AIPDPtr != nullptr) {
      ownStickY = 0.0;
      if (aiMgr::isReadyUpdate()) {
        fVar3 = (aiStat->Position).x;
        ownStickX = 0.0;
        local_cc = 0.0;
        if (50.0 + GLOBAL_AIMgr->unk_flt_360 <= fVar3) {
          if (GLOBAL_AIMgr->unk_flt_364 - 50.0 < fVar3) {
            ownStickX = -0.6;
          }
        }
        else {
          ownStickX = 0.6;
        }
        local_108 = ownStickX;
        childAiInput = selfAiInput->childAI;
        local_104 = 0.0;
        selfAiInput->leftStickX = ownStickX;
        selfAiInput->leftStickY = ownStickY;
        *(undefined4 *)&selfAiInput->field9_0xc = 0x8000;
        if ((childAiInput != nullptr) && ((childAiInput->unk_thing & 8) == 0)) {
          childAiStat = aiMgr::get_aiStat(&childAiInput->aiChrIdx);
          ownAiStat = aiMgr::get_aiStat(&selfAiInput->aiChrIdx);
          fVar6 = 0.0;
          fVar4 = (ownAiStat->Position).x;
          fVar3 = (childAiStat->Position).x;
          local_168 = 0.0;
          if (fVar4 - 10.0 <= fVar3) {
            if (10.0 + fVar4 < fVar3) {
              local_168 = -0.6;
            }
          }
          else {
            local_168 = 0.6;
          }
          childAiInput->leftStickX = local_168;
          childAiInput->leftStickY = fVar6;
          *(undefined4 *)&paVar31->field9_0xc = 0x8000;
        }
      }
    }
    else {
      update/[IpHuman]/(ip_human.o)(selfAiInput);
    }
    goto LAB_809113d4;
  }

```