project( Doom3BFG )
add_executable( ${PROJECT_NAME} WIN32 )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/aas/*.cpp*"
    "${CMAKE_SOURCE_DIR}/aas/*.h*"
)
source_group( "AAS" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/cm/*.cpp*"
    "${CMAKE_SOURCE_DIR}/cm/*.h*"
)
source_group( "CM" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/framework/*.cpp*"
    "${CMAKE_SOURCE_DIR}/framework/*.h*"
)
source_group( "Framework" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )
set_source_files_properties( "${CMAKE_SOURCE_DIR}/framework/precompiled.cpp" PROPERTIES COMPILE_FLAGS "/Yuprecompiled.h" OBJECT_DEPENDS "precompiled.pch" )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/renderer/*.cpp*"
    "${CMAKE_SOURCE_DIR}/renderer/*.h*"
)
source_group( "Renderer" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/renderer/Color/*.cpp*"
    "${CMAKE_SOURCE_DIR}/renderer/Color/*.h*"
)
source_group( "Renderer//Color" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/renderer/DXT/*.cpp*"
    "${CMAKE_SOURCE_DIR}/renderer/DXT/*.h*"
)
source_group( "Renderer//DXT" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/renderer/jobs/*.cpp*"
    "${CMAKE_SOURCE_DIR}/renderer/jobs/*.h*"
)
source_group( "Renderer//Jobs" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )
set_source_files_properties( "${CMAKE_SOURCE_DIR}/renderer/jobs/ShadowShared.cpp" PROPERTIES SKIP_PRECOMPILE_HEADERS ON )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/renderer/jobs/dynamicshadowvolume/*.cpp*"
    "${CMAKE_SOURCE_DIR}/renderer/jobs/dynamicshadowvolume/*.h*"
)
source_group( "Renderer//Jobs//DynamicShadowVolume" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )
set_source_files_properties( "${CMAKE_SOURCE_DIR}/renderer/jobs/dynamicshadowvolume/DynamicShadowVolume.cpp" PROPERTIES SKIP_PRECOMPILE_HEADERS ON )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/renderer/jobs/prelightshadowvolume/*.cpp*"
    "${CMAKE_SOURCE_DIR}/renderer/jobs/prelightshadowvolume/*.h*"
)
source_group( "Renderer//Jobs//PreLightShadowVolume" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )
set_source_files_properties( "${CMAKE_SOURCE_DIR}/renderer/jobs/prelightshadowvolume/PreLightShadowVolume.cpp" PROPERTIES SKIP_PRECOMPILE_HEADERS ON )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/renderer/jobs/staticshadowvolume/*.cpp*"
    "${CMAKE_SOURCE_DIR}/renderer/jobs/staticshadowvolume/*.h*"
)
source_group( "Renderer//Jobs//StaticShadowVolume" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )
set_source_files_properties( "${CMAKE_SOURCE_DIR}/renderer/jobs/staticshadowvolume/StaticShadowVolume.cpp" PROPERTIES SKIP_PRECOMPILE_HEADERS ON )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/renderer/OpenGL/*.cpp*"
    "${CMAKE_SOURCE_DIR}/renderer/OpenGL/*.h*"
)
source_group( "Renderer//OpenGL" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/sound/*.cpp*"
    "${CMAKE_SOURCE_DIR}/sound/*.h*"
)
source_group( "Sound" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/sound/*.cpp*"
    "${CMAKE_SOURCE_DIR}/sound/*.h*"
)
source_group( "Sound" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/sound/XAudio2/*.cpp*"
    "${CMAKE_SOURCE_DIR}/sound/XAudio2/*.h*"
)
source_group( "Sound//XAudio2" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/swf/*.cpp*"
    "${CMAKE_SOURCE_DIR}/swf/*.h*"
)
source_group( "SWF" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "*.cpp*"
    "*.h*"
)
source_group( "Sys" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "win32/*.cpp*"
    "win32/*.h*"
)
source_group( "Sys//Win32" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "win32/rc/res/*.ico*"
    "win32/rc/*.rc*"
    "win32/rc/*.h*"
    "win32/rc/*.manifest*"
)
source_group( "Sys//RC" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/ui/*.cpp*"
    "${CMAKE_SOURCE_DIR}/ui/*.h*"
)
source_group( "UI" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

# D3XP Code
include( "${CMAKE_SOURCE_DIR}/d3xp/Game.cmake" )

target_include_directories( ${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/idlib" "${CMAKE_SOURCE_DIR}/external" )
add_dependencies( ${PROJECT_NAME} idlib )
target_precompile_headers( ${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/idlib/precompiled.h" )
target_link_libraries( ${PROJECT_NAME} idlib external xaudio2 xinput dinput8 dxguid opengl32 winmm dbghelp ws2_32 GLEW::GLEW )
set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_FLAGS "/STACK:16777216,16777216 /SUBSYSTEM:WINDOWS" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY $<1:>${GAME_FOLDER} )

