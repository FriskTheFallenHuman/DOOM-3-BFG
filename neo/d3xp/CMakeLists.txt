project( d3xp CXX )
add_library( ${PROJECT_NAME} STATIC )

file( GLOB SOURCE_FILES
    "ai/*.cpp*"
    "ai/*.h*"
)
source_group( "AI" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "anim/*.cpp*"
    "anim/*.h*"
)
source_group( "Animation" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

set( SOURCE_FILES
    "gamesys/Class.cpp"
    "gamesys/Class.h"
    "gamesys/Event.cpp"
    "gamesys/Event.h"
    "gamesys/NoGameTypeInfo.h"
    "gamesys/SaveGame.cpp"
    "gamesys/SaveGame.h"
    "gamesys/SysCmds.cpp"
    "gamesys/SysCmds.h"
    "gamesys/SysCvar.cpp"
    "gamesys/SysCvar.h"
)
source_group( "GameSys" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "menus/*.cpp*"
    "menus/*.h*"
)
source_group( "Menus" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "physics/*.cpp*"
    "physics/*.h*"
)
source_group( "Physics" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "script/*.cpp*"
    "script/*.h*"
)
source_group( "Script" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "*.cpp*"
    "*.h*"
    "*.def*"
)
source_group( "" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )
set_source_files_properties( "precompiled.cpp" PROPERTIES COMPILE_FLAGS "/Yuprecompiled.h" OBJECT_DEPENDS "precompiled.pch" )

target_include_directories( ${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/idlib" "${CMAKE_SOURCE_DIR}/external" )
target_precompile_headers( ${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/idlib/precompiled.h" )
target_compile_definitions( ${PROJECT_NAME} PRIVATE __DOOM__ _D3XP CTF )