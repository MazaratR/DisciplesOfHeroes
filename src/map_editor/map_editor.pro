QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

CONFIG -= debug_and_release

OBJECTS_DIR = ./tmp
MOC_DIR = ./tmp
UI_HEADERS_DIR = ./tmp
UI_DIR = ./tmp
RCC_DIR += ./tmp

DESTDIR = ../../

INCLUDEPATH += ../ \

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../path.cpp \
    main.cpp \
    map_editor.cpp \
    ../events/event_manager.cpp \
    ../grid_maker.cpp \
    ../hero.cpp \
    ../hero_functions.cpp \
    ../heroes_manager.cpp \
    ../map_element.cpp \
    ../map_manager.cpp \
    ../resource.cpp \
    ../town.cpp \
    ../towns_manager.cpp \
    ../ui/canvas.cpp \
    ../units/army.cpp \
    ../units/dwarf/axemaster.cpp \
    ../units/dwarf/axethrower.cpp \
    ../units/dwarf/runemaster.cpp \
    ../units/dwarf/shieldman.cpp \
    ../units/elf/green_dragon.cpp \
    ../units/elf/pegas.cpp \
    ../units/elf/sniper.cpp \
    ../units/elf/wisp.cpp \
    ../units/human/angel.cpp \
    ../units/human/archer.cpp \
    ../units/human/monk.cpp \
    ../units/human/swordsman.cpp \
    ../units/undead/bone_dragon.cpp \
    ../units/undead/lich.cpp \
    ../units/undead/skeleton.cpp \
    ../units/undead/vampire.cpp \
    ../units/unit.cpp \
    ../units/unit_factory.cpp

HEADERS += \
    map_editor.h \
    ../events/event_manager.h \
    ../grid_maker.h \
    ../hero.h \
    ../hero_functions.h \
    ../heroes_manager.h \
    ../map_element.h \
    ../map_file_format.h \
    ../map_manager.h \
    ../path.h \
    ../resource.h \
    ../resource_types.h \
    ../town.h \
    ../towns_manager.h \
    ../ui/canvas.h \
    ../units/army.h \
    ../units/dwarf/axemaster.h \
    ../units/dwarf/axethrower.h \
    ../units/dwarf/runemaster.h \
    ../units/dwarf/shieldman.h \
    ../units/elf/green_dragon.h \
    ../units/elf/pegas.h \
    ../units/elf/sniper.h \
    ../units/elf/wisp.h \
    ../units/human/angel.h \
    ../units/human/archer.h \
    ../units/human/monk.h \
    ../units/human/swordsman.h \
    ../units/undead/bone_dragon.h \
    ../units/undead/lich.h \
    ../units/undead/skeleton.h \
    ../units/undead/vampire.h \
    ../units/unit.h \
    ../units/unit_creator.h \
    ../units/unit_factory.h

RESOURCES += \
    ../../fonts/fonts.qrc \
    ../../images/images.qrc

FORMS += \
    map_editor.ui
