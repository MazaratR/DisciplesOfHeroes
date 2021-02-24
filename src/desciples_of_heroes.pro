QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

CONFIG -= debug_and_release

OBJECTS_DIR = ./tmp
MOC_DIR = ./tmp
UI_HEADERS_DIR = ./tmp
UI_DIR = ./tmp
RCC_DIR += ./tmp

DESTDIR = ../

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
    battle_manager.cpp \
    battle_queue_element.cpp \
    calendar.cpp \
    events/event_handler.cpp \
    events/event_manager.cpp \
    game_logic.cpp \
    grid_maker.cpp \
    hero.cpp \
    hero_functions.cpp \
    heroes_manager.cpp \
    main.cpp \
    map_element.cpp \
    map_manager.cpp \
    path.cpp \
    player.cpp \
    players_manager.cpp \
    resource.cpp \
    town.cpp \
    towns_manager.cpp \
    ui/army_layout.cpp \
    ui/battle_queue.cpp \
    ui/battle_result_view.cpp \
    ui/battlefield.cpp \
    ui/canvas.cpp \
    ui/notification_view.cpp \
    ui/player_settings.cpp \
    ui/resources_view.cpp \
    ui/town_view.cpp \
    ui/unit_layout.cpp \
    ui/unit_stats_view.cpp \
    ui/user_interface.cpp \
    units/army.cpp \
    units/dwarf/axemaster.cpp \
    units/dwarf/axethrower.cpp \
    units/dwarf/runemaster.cpp \
    units/dwarf/shieldman.cpp \
    units/elf/green_dragon.cpp \
    units/elf/pegas.cpp \
    units/elf/sniper.cpp \
    units/elf/wisp.cpp \
    units/human/angel.cpp \
    units/human/archer.cpp \
    units/human/monk.cpp \
    units/human/swordsman.cpp \
    units/undead/bone_dragon.cpp \
    units/undead/lich.cpp \
    units/undead/skeleton.cpp \
    units/undead/vampire.cpp \
    units/unit.cpp \
    units/unit_factory.cpp \

HEADERS += \
    attack_result.h \
    battle_manager.h \
    battle_queue_element.h \
    battle_result.h \
    calendar.h \
    colors.h \
    control_side.h \
    events/event_arrival_at_town.h \
    events/event_base.h \
    events/event_battle.h \
    events/event_battle_end.h \
    events/event_handler.h \
    events/event_manager.h \
    events/event_notify.h \
    events/event_types.h \
    forward/battle_queue_element_forward.h \
    forward/i_hero_forward.h \
    forward/i_interactable_object_forward.h \
    forward/i_player_forward.h \
    forward/i_town_forward.h \
    forward/i_unit_forward.h \
    game_logic.h \
    grid_maker.h \
    hero.h \
    hero_functions.h \
    hero_parameters.h \
    heroes_manager.h \
    images_paths.h \
    interfaces/i_battle_manager.h \
    interfaces/i_event_handler.h \
    interfaces/i_game_logic.h \
    interfaces/i_hero.h \
    interfaces/i_heroes_manager.h \
    interfaces/i_interactable_object.h \
    interfaces/i_map_manager.h \
    interfaces/i_player.h \
    interfaces/i_players_manager.h \
    interfaces/i_town.h \
    interfaces/i_towns_manager.h \
    interfaces/i_unit.h \
    interfaces/i_unit_creator.h \
    interfaces/i_unit_factory.h \
    interfaces/i_user_interface.h \
    map_element.h \
    map_manager.h \
    move_directions.h \
    object_types.h \
    path.h \
    player.h \
    players_manager.h \
    point_2d.h \
    races.h \
    randomizer.h \
    range.h \
    resource.h \
    resource_types.h \
    scale_value.h \
    terrain.h \
    town.h \
    towns_manager.h \
    ui/army_layout.h \
    ui/battle_queue.h \
    ui/battle_result_view.h \
    ui/battlefield.h \
    ui/canvas.h \
    ui/notification_view.h \
    ui/player_settings.h \
    ui/resources_view.h \
    ui/town_view.h \
    ui/unit_highlight_types.h \
    ui/unit_layout.h \
    ui/unit_stats_view.h \
    ui/user_interface.h \
    units/army.h \
    units/dwarf/axemaster.h \
    units/dwarf/axethrower.h \
    units/dwarf/runemaster.h \
    units/dwarf/shieldman.h \
    units/elf/green_dragon.h \
    units/elf/pegas.h \
    units/elf/sniper.h \
    units/elf/wisp.h \
    units/human/angel.h \
    units/human/archer.h \
    units/human/monk.h \
    units/human/swordsman.h \
    units/undead/bone_dragon.h \
    units/undead/lich.h \
    units/undead/skeleton.h \
    units/undead/vampire.h \
    units/unit.h \
    units/unit_creator.h \
    units/unit_factory.h \
    units/unit_levels.h \
    units/unit_position.h \
    units/unit_stats.h \
    units/unit_types.h \

RESOURCES += \
    ../fonts/fonts.qrc \
    ../images/images.qrc

FORMS += \
    ui/army_layout.ui \
    ui/battle_queue.ui \
    ui/battle_result_view.ui \
    ui/battlefield.ui \
    ui/notification_view.ui \
    ui/player_settings.ui \
    ui/resources_view.ui \
    ui/town_view.ui \
    ui/unit_layout.ui \
    ui/unit_stats_view.ui \
    ui/user_interface.ui
