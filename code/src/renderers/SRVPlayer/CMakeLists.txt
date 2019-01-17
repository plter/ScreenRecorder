add_executable(SRVPlayer
        main.cpp
        SRVPlayerEntry.cpp SRVPlayerEntry.h
        ../../commons/node_globals.h ../../commons/node_globals.cpp
        ../../commons/LocalStorageManager.cpp ../../commons/LocalStorageManager.h
        ../../commons/SRGlobals.cpp ../../commons/SRGlobals.h
        ../../commons/worker/SRVReader.h ../../commons/worker/SRVReader.cpp
        ../../commons/SRVReaderTask.cpp ../../commons/SRVReaderTask.h)