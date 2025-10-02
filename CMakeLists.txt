# Collect all .c files in src/
file(GLOB TM_TC_SRC CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_library(tm_tc ${TM_TC_SRC})

target_include_directories(tm_tc PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Make tm_tc depend on comms
# TODO: Use TimeInterface instead of ZephyrTime to remove zephyr_abs dep
target_link_libraries(tm_tc PUBLIC comms zephyr_abs)
