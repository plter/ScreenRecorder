add_executable(index
        main.cpp
        ../../commons/node_globals.h ../../commons/node_globals.cpp
        Entry.cpp Entry.h
        ../../commons/LocalStorageManager.cpp ../../commons/LocalStorageManager.h
        SelectAudioInput.cpp SelectAudioInput.h StreamQueue.cpp StreamQueue.h
        ../../commons/SRWindows.cpp ../../commons/SRWindows.h
        ../../commons/md5.cpp ../../commons/md5.h
        ../../commons/worker/SRVReader.cpp ../../commons/worker/SRVReader.h
        ../../commons/worker/SRVWebM.cpp ../../commons/worker/SRVWebM.h
        ../../commons/worker/SRVConfig.cpp ../../commons/worker/SRVConfig.h
        ../../commons/ElectronGlobals.cpp ../../commons/ElectronGlobals.h)