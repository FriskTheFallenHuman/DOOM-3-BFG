cmake_minimum_required( VERSION 3.27.8 )

# Check if vcpkg exist in our source folder
if ( EXISTS "${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake" )
	set( CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file" )
else()
	message( FATAL_ERROR "Vcpkg wasn't not found, aborting, make sure to git clone this with all submodules!" )
endif()

set_property( GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER  "" )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules" )

project( Doom3BFG C CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" )

set( GAME_FOLDER ${CMAKE_SOURCE_DIR}/../build )

if( WIN32 )
    set( CMAKE_SHARED_LINKER_FLAGS /MANIFEST:NO )
endif()

# Enable floating-point optimizations.
add_compile_options( "/fp:fast" )

# Enable Run-Time Type Information.
add_compile_options( "/GR" )

# Turn off stack protection checks.
add_compile_options( "/GS-" )

# Perform multi-process builds.
add_compile_options( "/MP" )

# Allow addressing more memory on a 64-bit host.
add_link_options( "/LARGEADDRESSAWARE" )

# Project is not compatible with data execution prevention.
add_link_options( "/NXCOMPAT:NO" )

# Project is not compatible with safe exception handlers. 
add_link_options( "/SAFESEH:NO" )

add_compile_definitions(
    # Supress deprecation warnings
    $<$<COMPILE_LANGUAGE:C,CXX>:_CRT_SECURE_NO_DEPRECATE>

    # Supress deprecation warnings for non-standar C library functions
    $<$<COMPILE_LANGUAGE:C,CXX>:_CRT_NONSTDC_NO_DEPRECATE>

    # Time
    $<$<COMPILE_LANGUAGE:C,CXX>:_USE_32BIT_TIME_T>

    $<$<COMPILE_LANGUAGE:C,CXX>:_HAS_AUTO_PTR_ETC>
)

# Stb
find_package( Stb REQUIRED )
include_directories( ${Stb_INCLUDE_DIR} )

# Glew
find_package( GLEW REQUIRED )
include_directories( ${GLEW_INCLUDE_DIRS} )

add_subdirectory(external)
#add_subdirectory(d3xp)
add_subdirectory(idlib)
add_subdirectory(sys)