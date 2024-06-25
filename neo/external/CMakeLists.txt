project( external C CXX )
add_library( ${PROJECT_NAME} STATIC )

# Need to make sure these are compiled as C code
file( GLOB C_FILES CONFIGURE_DEPENDS "zlib/*.c" )
set_source_files_properties( ${C_FILES} PROPERTIES LANGUAGE C )

set( SOURCE_FILES
    "zlib/adler32.c"
    "zlib/compress.c"
    "zlib/crc32.c"
    "zlib/deflate.c"
    "zlib/gzio.c"
    "zlib/infback.c"
    "zlib/inffast.c"
    "zlib/inflate.c"
    "zlib/inftrees.c"
    "zlib/trees.c"
    "zlib/uncompr.c"
    "zlib/zutil.c"
)
source_group( "zlib//Source Files" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

set( SOURCE_FILES
    "zlib/crc32.h"
    "zlib/deflate.h"
    "zlib/inffast.h"
    "zlib/inflate.h"
    "zlib/inftrees.h"
    "zlib/trees.h"
    "zlib/zutil.h"
)
source_group( "zlib//Header Files" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )

set( SOURCE_FILES
    "zlib/algorithm.txt"
    "zlib/ChangeLog"
    "zlib/FAQ"
    "zlib/INDEX"
    "zlib/README"
)
source_group( "zlib" FILES ${SOURCE_FILES} )
target_sources( ${PROJECT_NAME} PRIVATE ${SOURCE_FILES} )