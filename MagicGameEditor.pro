#-------------------------------------------------
#
# Project created by QtCreator 2010-11-17T00:08:34
#
#-------------------------------------------------
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
QMAKE_CXXFLAGS += -std=c++11

win32:RC_FILE = MagicGameEditor/icons.rc
macx:ICON = MagicGameEditor/Icons/icons.icns
macx:QMAKE_INFO_PLIST = MagicGameEditor/Info.plist

QT += opengl

TARGET = MagicGameEditor
TEMPLATE = app
DESTDIR = ../magic3d/game

win32:OBJECTS_DIR = ./MGE_Windows
unix:!macx:OBJECTS_DIR = ./MGE_Linux
macx:OBJECTS_DIR = ./MGE_MacOS

UI_DIR = $$OBJECTS_DIR
MOC_DIR = $$OBJECTS_DIR

CONFIG += app_bundle
win32:LIBS += -lwsock32 -lwinmm -lws2_32 -lOpenAL32 -lgdi32 -lopengl32
unix:!macx:LIBS += -lopenal -lpng
macx:LIBS += -framework OpenAL

DEFINES += _MGE_ MAGIC3D_LOG #DEBUG

MAGIC3D = ../magic3d/

INCLUDEPATH += \
    ./MagicGameEditor \

include($$MAGIC3D/Magic3D_Base.pro)

SOURCES += \
    MagicGameEditor/*.cpp \
    MagicGameEditor/MGE/*.cpp \

HEADERS  += \
    MagicGameEditor/*.h \
    MagicGameEditor/MGE/*.h \

FORMS += \
    MagicGameEditor/*.ui \

RESOURCES += \
    MagicGameEditor/MagicGameEditor.qrc

OTHER_FILES += \
    $$MAGIC3D/game/data/shader/*.* \
    $$MAGIC3D/game/mge.style \
    $$MAGIC3D/game/config.xml

DISTFILES += \
    MagicGameEditor/icons.rc
