cmake_minimum_required (VERSION 3.15)

add_code(Brawl MU/muSelCharPlayerArea.cpp MU/MuSelCharHand.cpp MU/muCollectionViewer.cpp GF/gfFileIO.cpp FT/ftManager.cpp AI/aiMgr.cpp GF/gfCamera.cpp FT/Fighter.cpp SO/soMotionModule.cpp SO/soPostureModuleImpl.cpp
        GF/gfDraw.cpp Message.cpp FT/ftOwner.cpp SO/soGroundModuleImpl.cpp AI/aiAct.cpp GF/gfFileIOManager.cpp SC/scMelee.cpp GF/gfCryptoESP.cpp GF/gfFileIOHandle.cpp GF/gfCollectionIO.cpp
        ReplayCollectionDataHeader.h GF/gfMemoryPool.cpp IP/ipKeyRecorder.cpp FT/ftCommonDataAccessor.cpp FT/ftEntry.cpp)