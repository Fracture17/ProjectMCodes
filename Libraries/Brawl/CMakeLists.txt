cmake_minimum_required (VERSION 3.15)

add_code(Brawl sndSystem.cpp MU/muSelCharPlayerArea.cpp MU/MuSelCharHand.cpp MU/muCollectionViewer.cpp GF/gfFileIO.cpp FT/ftManager.cpp AI/aiMgr.cpp AI/AICEPac.cpp AI/aiStat.cpp GF/gfApplication.cpp GF/gfCamera.cpp FT/Fighter.cpp 
        SO/soKineticModuleGenericImpl.cpp
        SO/soMotionModule.cpp SO/soStatusModuleImpl.cpp SO/soPostureModuleImpl.cpp SO/soGenerateArticleManageModuleImpl.cpp SO/soAnimCmdModuleImpl.cpp SO/soWorkManageModuleImpl.cpp SO/soItemManageModuleImpl.cpp
        GF/gfTask.cpp GF/gfDraw.cpp Message.cpp FT/ftOwner.cpp SO/StageObject.cpp SO/soGroundModuleImpl.cpp AI/aiScriptData.cpp GF/gfFileIOManager.cpp SC/scMelee.cpp GF/gfCryptoESP.cpp GF/gfFileIOHandle.cpp GF/gfCollectionIO.cpp AI/aiParam.cpp
        ReplayCollectionDataHeader.h GF/gfMemoryPool.cpp IP/ipKeyRecorder.cpp FT/ftCommonDataAccessor.cpp FT/ftEntry.cpp FT/ftEntryManager.cpp FT/ftCancelModule.cpp FT/ftSlotManager.cpp IT/itManager.cpp IT/BaseItem.cpp GF/gfApplication.cpp
        ST/Stage.cpp ST/stLoaderStage.cpp GR/grYakumono.cpp GR/grGimmick.cpp GR/Ground.cpp Events/EventMatchNode.cpp)