include(FetchContent)

FetchContent_Declare(
	BasicRDFGraphComparator
	GIT_REPOSITORY https://github.com/WhiteGobo/BasicRDFGraphComparator.git
	#GIT_TAG 8b244ca1826c024a641d98f0bb52e3c649e79972
	FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(BasicRDFGraphComparator)

FetchContent_Declare(
	NQuadsRDF
	GIT_REPOSITORY https://github.com/WhiteGobo/NQuadsRDF.git
	#GIT_TAG 7f424c76698a0df4b500d30a5917eaa2e038e65a
	FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(NQuadsRDF)


FetchContent_Declare(
	hashtable
	GIT_REPOSITORY https://github.com/WhiteGobo/hashtable
	#CMAKE_CACHE_ARGS -DBUILD_SHARED_LIBS=OFF
	FIND_PACKAGE_ARGS
	#OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(hashtable)
