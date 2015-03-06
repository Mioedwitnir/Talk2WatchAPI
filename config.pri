# Config.pri file version 2.0. Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR = $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            LIBS += -lbbsystem

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        } else {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            LIBS += -lbbsystem

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }

    }

    CONFIG(release, debug|release) {
        !profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            LIBS += -lbbsystem

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

simulator {
    CONFIG(debug, debug|release) {
        !profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            LIBS += -lbbsystem

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

config_pri_assets {
    OTHER_FILES += \
        $$quote($$BASEDIR/assets/T2W_API.zip) \
        $$quote($$BASEDIR/assets/main.qml) \
        $$quote($$BASEDIR/assets/t2w_api_icon.png)
}

config_pri_source_group1 {
    SOURCES += \
        $$quote($$BASEDIR/src/T2W API/PebbleNotifications/pebbleaction.cpp) \
        $$quote($$BASEDIR/src/T2W API/PebbleNotifications/pebbleattribute.cpp) \
        $$quote($$BASEDIR/src/T2W API/PebbleNotifications/pebblenotification.cpp) \
        $$quote($$BASEDIR/src/T2W API/Serializer.cpp) \
        $$quote($$BASEDIR/src/T2W API/Talk2WatchInterface.cpp) \
        $$quote($$BASEDIR/src/T2W API/UdpModule.cpp) \
        $$quote($$BASEDIR/src/applicationui.cpp) \
        $$quote($$BASEDIR/src/main.cpp)

    HEADERS += \
        $$quote($$BASEDIR/src/T2W API/PebbleNotifications/pebbleaction.h) \
        $$quote($$BASEDIR/src/T2W API/PebbleNotifications/pebbleattribute.h) \
        $$quote($$BASEDIR/src/T2W API/PebbleNotifications/pebblenotification.h) \
        $$quote($$BASEDIR/src/T2W API/Serializer.h) \
        $$quote($$BASEDIR/src/T2W API/Talk2WatchInterface.h) \
        $$quote($$BASEDIR/src/T2W API/UdpModule.h) \
        $$quote($$BASEDIR/src/applicationui.hpp)
}

INCLUDEPATH += $$quote($$BASEDIR/src/T2W API) \
    $$quote($$BASEDIR/src) \
    $$quote($$BASEDIR/src/T2W API/PebbleNotifications)

CONFIG += precompile_header

PRECOMPILED_HEADER = $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES += \
        $$quote($$BASEDIR/../src/*.c) \
        $$quote($$BASEDIR/../src/*.c++) \
        $$quote($$BASEDIR/../src/*.cc) \
        $$quote($$BASEDIR/../src/*.cpp) \
        $$quote($$BASEDIR/../src/*.cxx) \
        $$quote($$BASEDIR/../src/T2W API/*.c) \
        $$quote($$BASEDIR/../src/T2W API/*.c++) \
        $$quote($$BASEDIR/../src/T2W API/*.cc) \
        $$quote($$BASEDIR/../src/T2W API/*.cpp) \
        $$quote($$BASEDIR/../src/T2W API/*.cxx) \
        $$quote($$BASEDIR/../src/T2W API/PebbleNotifications/*.c) \
        $$quote($$BASEDIR/../src/T2W API/PebbleNotifications/*.c++) \
        $$quote($$BASEDIR/../src/T2W API/PebbleNotifications/*.cc) \
        $$quote($$BASEDIR/../src/T2W API/PebbleNotifications/*.cpp) \
        $$quote($$BASEDIR/../src/T2W API/PebbleNotifications/*.cxx) \
        $$quote($$BASEDIR/../assets/*.qml) \
        $$quote($$BASEDIR/../assets/*.js) \
        $$quote($$BASEDIR/../assets/*.qs)

    HEADERS += \
        $$quote($$BASEDIR/../src/*.h) \
        $$quote($$BASEDIR/../src/*.h++) \
        $$quote($$BASEDIR/../src/*.hh) \
        $$quote($$BASEDIR/../src/*.hpp) \
        $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS = $$quote($${TARGET}.ts)
