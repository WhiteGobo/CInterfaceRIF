include(CTest)
include(tools.cmake)

add_executable(CInterfaceRIF_testdriverA
	testa.c
)

target_link_libraries(CInterfaceRIF_testdriverA
	CInterfaceRIF
	BasicRDFGraphComparator
	NQuadsRDF
)

set(testsuffix "CInterfaceRIF::")
set(tmpbase "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(manifest "${tmpbase}/manifest.json")
CInterfaceRIF_read_manifest(${manifest} testlist testlist_length)
math(EXPR range "${testlist_length} - 1")
foreach(x RANGE 0 ${range})
	string(JSON data GET ${testlist} ${x})

	string(JSON id GET ${data} "name")
	string(JSON premise GET ${data} "premise")
	string(JSON conclusion GET ${data} "conclusion")
	set(premise_file "${tmpbase}/${premise}")
	set(conclusion_file "${tmpbase}/${conclusion}")
	set(testname "${testsuffix}${id}")

	add_test(NAME "${testname}" COMMAND CInterfaceRIF_testdriverA
		ARGS
		"--premise" "${premise_file}"
		"--conclusion" "${conclusion_file}"
	)
	set_property(TEST ${testname} PROPERTY LABELS "CInterfaceRIF")
endforeach()
