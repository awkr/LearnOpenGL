file(GLOB_RECURSE SOURCE_FILES base/*.h)

add_library(base SHARED ${SOURCE_FILES})
set_target_properties(base PROPERTIES LINKER_LANGUAGE CXX)
