file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/d3xp/ai/*.cpp*"
    "${CMAKE_SOURCE_DIR}/d3xp/ai/*.h*"
)
source_group( "Game-d3xp//AI" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/d3xp/anim/*.cpp*"
    "${CMAKE_SOURCE_DIR}/d3xp/anim/*.h*"
)
source_group( "Game-d3xp//Animation" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

set( SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/d3xp/gamesys/Class.cpp"
    "${CMAKE_SOURCE_DIR}/d3xp/gamesys/Class.h"
    "${CMAKE_SOURCE_DIR}/d3xp/gamesys/Event.cpp"
    "${CMAKE_SOURCE_DIR}/d3xp/gamesys/Event.h"
    "${CMAKE_SOURCE_DIR}/d3xp/gamesys/NoGameTypeInfo.h"
    "${CMAKE_SOURCE_DIR}/d3xp/gamesys/SaveGame.cpp"
    "${CMAKE_SOURCE_DIR}/d3xp/gamesys/SaveGame.h"
    "${CMAKE_SOURCE_DIR}/d3xp/gamesys/SysCmds.cpp"
    "${CMAKE_SOURCE_DIR}/d3xp/gamesys/SysCmds.h"
    "${CMAKE_SOURCE_DIR}/d3xp/gamesys/SysCvar.cpp"
    "${CMAKE_SOURCE_DIR}/d3xp/gamesys/SysCvar.h"
)
source_group( "Game-d3xp//GameSys" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/d3xp/menus/*.cpp*"
    "${CMAKE_SOURCE_DIR}/d3xp/menus/*.h*"
)
source_group( "Game-d3xp//Menus" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/d3xp/physics/*.cpp*"
    "${CMAKE_SOURCE_DIR}/d3xp/physics/*.h*"
)
source_group( "Game-d3xp//Physics" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/d3xp/script/*.cpp*"
    "${CMAKE_SOURCE_DIR}/d3xp/script/*.h*"
)
source_group( "Game-d3xp//Script" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/d3xp/*.cpp*"
    "${CMAKE_SOURCE_DIR}/d3xp/*.h*"
    "${CMAKE_SOURCE_DIR}/d3xp/*.def*"
)
source_group( "Game-d3xp" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )
set_source_files_properties( "${CMAKE_SOURCE_DIR}/d3xp/precompiled.cpp" PROPERTIES COMPILE_FLAGS "/Yuprecompiled.h" OBJECT_DEPENDS "precompiled.pch" )

target_compile_definitions( ${PROJECT_NAME} PRIVATE _D3XP CTF )