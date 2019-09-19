include_directories(../include)

file(GLOB SRC_FILES "*.c")

add_executable(bdf_gpb ${SRC_FILES})
target_link_libraries(bdf_gpb uart)