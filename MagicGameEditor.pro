#-------------------------------------------------
#
# Project created by QtCreator 2010-11-17T00:08:34
#
#-------------------------------------------------
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

MAGIC3D = ../magic3d
CONTRIB = $$MAGIC3D/contrib

CONFIG += app_bundle c++11

QMAKE_LFLAGS += -F/Library/Frameworks

LIBS += -L$$MAGIC3D/game -lmagic3d_contrib
win32:LIBS += -lwsock32 -lwinmm -lws2_32 -lgdi32 -limm32 -lopengl32 -lOpenAL32 -lSDL2
unix:!macx:LIBS += -lopenal -lpng
macx:LIBS += -framework OpenAL -framework SDL2

DEFINES += _MGE_ MAGIC3D_LOG

INCLUDEPATH += \
    ./MagicGameEditor \

include($$CONTRIB/Magic3D_Base.pro)
include($$CONTRIB/Magic3D_Base_Include.pro)
include($$MAGIC3D/Magic3D_Engine.pro)

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
