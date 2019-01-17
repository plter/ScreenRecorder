add_executable(VideoLibrary
        main.cpp
        VideoLibraryEntry.cpp VideoLibraryEntry.h
        VideoFileListItem.cpp VideoFileListItem.h
        ../../commons/node_globals.h ../../commons/node_globals.cpp
        ../../commons/LocalStorageManager.cpp ../../commons/LocalStorageManager.h
        ../../commons/SRGlobals.cpp ../../commons/SRGlobals.h
        ExportVideoTask.cpp ExportVideoTask.h
        ../../commons/Res.cpp ../../commons/Res.h
        ../../commons/SRVReaderTask.h ../../commons/SRVReaderTask.cpp
        ../../commons/ElectronGlobals.h ../../commons/ElectronGlobals.cpp
        ../../commons/worker/WorkerGlobals.h ../../commons/worker/WorkerGlobals.cpp)