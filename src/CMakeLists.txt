set(includepath ${CMAKE_CURRENT_SOURCE_DIR}/include)

#remove bison and flex defined public symbols, like yyset_debug.

#temporary to warn because of switch missing all values from enumerator
add_compile_options(-Wall -Wextra -Wpedantic)

set (CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/export_symbols")
#TODO: print warning when linker doesnt support version-script

FIND_PACKAGE(BISON 3.4 REQUIRED)
FIND_PACKAGE(FLEX 2.5.19 REQUIRED)

set(SOURCE_SCANNER
	"${CMAKE_CURRENT_BINARY_DIR}/CInterfaceRIF_scanner.c"
)
set(SOURCE_PARSER
	"${CMAKE_CURRENT_BINARY_DIR}/CInterfaceRIF_parser.c"
)

flex_target(CInterfaceRIF_scanner "CInterfaceRIF_scanner.l"
	"${SOURCE_SCANNER}"
)

if (NOT FLEX_CInterfaceRIF_scanner_DEFINED)
	message(FATAL_ERROR "flex failed.")
endif()

bison_target(CInterfaceRIF_parser "CInterfaceRIF_parser.y"
	"${SOURCE_PARSER}"
	DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/CInterfaceRIF_parser.h"
)

if (NOT BISON_CInterfaceRIF_parser_DEFINED)
	message(FATAL_ERROR "bison failed.")
endif()
add_flex_bison_dependency(CInterfaceRIF_scanner CInterfaceRIF_parser)


add_library(CInterfaceRIF SHARED
	"${includepath}/CInterfaceRIF.h"
	memory.c
	memory.h
	to_rdf.c
	to_rdf.h
	prefix.c
	prefix.h
	qq.c
	${FLEX_CInterfaceRIF_scanner_OUTPUTS}
	${BISON_CInterfaceRIF_parser_OUTPUTS}
)


set_target_properties(CInterfaceRIF PROPERTIES
	LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/export_symbols"
)
target_link_libraries(CInterfaceRIF
	hashtable
)
target_include_directories(CInterfaceRIF PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

target_include_directories(CInterfaceRIF PUBLIC
	"${includepath}/"
	"${CMAKE_CURRENT_BINARY_DIR}/"
)
