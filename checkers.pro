TEMPLATE = app
#win32 {
# QMAKE_POST_LINK += mt -nologo -manifest $$PWD/manifest.xml -outputresource:$$OUT_PWD/$$TARGET”.exe” $$escape_expand(\n\t)
#}
#mt -manifest manifest.xml -outputresource:checkers.exe
LIBS += -LC:/Users/Pawel/Desktop/SFML-2.5.0-windows-gcc-7.3.0-mingw-64-bit/SFML-2.5.0/lib

INCLUDEPATH += C:/Users/Pawel/Desktop/SFML-2.5.0-windows-gcc-7.3.0-mingw-64-bit/SFML-2.5.0/include
DEPENDPATH += C:/Users/Pawel/Desktop/SFML-2.5.0-windows-gcc-7.3.0-mingw-64-bit/SFML-2.5.0/include
#QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

SOURCES += main.cpp \
    Checkers.cpp \
    Player.cpp \
    Pawn.cpp \
    Computer.cpp

HEADERS += \
    Checkers.hpp \
    Player.hpp \
    Pawn.hpp \
    GameMode.hpp \
    Computer.hpp \
    Board.hpp \
    Tree.hpp
