project( idlib CXX )
add_library( ${PROJECT_NAME} STATIC )

file( GLOB SOURCE_FILES
    "bv/*.cpp*"
    "bv/*.h*"
)
source_group( "BV" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "containers/*.cpp*"
    "containers/*.h*"
)
source_group( "Containers" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "geometry/*.cpp*"
    "geometry/*.h*"
)
source_group( "Geometry" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )
set_source_files_properties( "geometry/RenderMatrix.cpp" PROPERTIES SKIP_PRECOMPILE_HEADERS ON )

file( GLOB SOURCE_FILES
    "hashing/*.cpp*"
    "hashing/*.h*"
)
source_group( "Hashing" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "math/*.cpp*"
    "math/*.h*"
)
source_group( "Math" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "sys/*.cpp*"
    "sys/*.h*"
)
source_group( "Sys" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

file( GLOB SOURCE_FILES
    "sys/win32/*.cpp*"
)
source_group( "Sys//Win32" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

set( SOURCE_FILES
    "Base64.cpp"
    "Base64.h"
    "CmdArgs.cpp"
    "CmdArgs.h"
    "Lexer.cpp"
    "Lexer.h"
    "Parser.cpp"
    "Parser.h"
    "Str.cpp"
    "Str.h"
    "StrStatic.h"
    "Token.cpp"
    "Token.h"
)
source_group( "Text" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

set( SOURCE_FILES
    "BitMsg.cpp"
    "BitMsg.h"
    "Callback.h"
    "CommandLink.cpp"
    "DataQueue.h"
    "Dict.cpp"
    "Dict.h"
    "Heap.cpp"
    "Heap.h"
    "LangDict.cpp"
    "LangDict.h"
    "Lib.cpp"
    "Lib.h"
    "MapFile.cpp"
    "MapFile.h"
    "ParallelJobList.cpp"
    "ParallelJobList.h"
    "ParallelJobList_JobHeaders.h"
    "precompiled.cpp"
    "precompiled.h"
    "RectAllocator.cpp"
    "SoftwareCache.cpp"
    "SoftwareCache.h"    
    "Thread.cpp"
    "Swap.h"
    "Thread.h"
    "Timer.cpp"
    "Timer.h"
)
source_group( "" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )
set_source_files_properties( "SoftwareCache.cpp" PROPERTIES SKIP_PRECOMPILE_HEADERS ON )
set_source_files_properties( "precompiled.cpp" PROPERTIES COMPILE_FLAGS "/Yuprecompiled.h" OBJECT_DEPENDS "precompiled.pch" )

target_compile_definitions( ${PROJECT_NAME} PRIVATE __IDLIB__ )
target_include_directories( ${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/external" "${CMAKE_CURRENT_SOURCE_DIR}" )
target_precompile_headers( ${PROJECT_NAME} PRIVATE "precompiled.h" )