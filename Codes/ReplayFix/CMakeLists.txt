cmake_minimum_required(VERSION 3.15)

add_code(ReplayFix ReplayFix.cpp ReplayFrameData.cpp ReplayGameStartInfo.cpp ReplayFrameEventManager.cpp ReplayEvent.cpp ReplayFrameStartEvent.cpp ReplayPreFrameFighterEvent.cpp ReplayGameStartEvent.cpp
        AsyncFileIO.cpp ReplayGameEndEvent.cpp ReplayFrameEndEvent.cpp ReplayLoadEvent.cpp CircularBuffer.cpp)