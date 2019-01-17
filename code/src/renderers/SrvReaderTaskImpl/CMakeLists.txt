add_executable(SrvReaderTaskImpl
        main.cpp
        ../../commons/node_globals.h ../../commons/node_globals.cpp
        ../../commons/LocalStorageManager.cpp ../../commons/LocalStorageManager.h
        ../../commons/SRGlobals.cpp ../../commons/SRGlobals.h
        ../../commons/worker/SRVReader.h ../../commons/worker/SRVReader.cpp
        ../../commons/worker/WorkerGlobals.h ../../commons/worker/WorkerGlobals.cpp)