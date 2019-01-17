add_executable(settings
        main.cpp
        SettingsEntry.cpp SettingsEntry.h
        ../../commons/node_globals.h ../../commons/node_globals.cpp
        ../../commons/LocalStorageManager.cpp ../../commons/LocalStorageManager.h
        ../../commons/ElectronGlobals.h ../../commons/ElectronGlobals.cpp
        ../../commons/SRWindows.cpp ../../commons/SRWindows.h)