cmake_minimum_required(VERSION 3.10)
project(Minimv_Test C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -DCMSIS_MCU_H=\"<stdint.h>\"")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/imlib
    ${CMAKE_CURRENT_SOURCE_DIR}/common
)

set(SRC_FILES
    main.c
    common/array.c
    common/queue.c
    common/trace.c
    imlib/imlib.c
    imlib/hough.c
    imlib/fmath.c
    imlib/edge.c
    imlib/mathop.c
    imlib/fast.c
    imlib/point.c
    imlib/rectangle.c
    imlib/collections.c
    imlib/sincos_tab.c  # <--- 补上这块拼图
    imlib/xyz_tab.c     # <--- 补上这块拼图
)

add_executable(test_run ${SRC_FILES})
target_link_libraries(test_run m)
find_package(Python3 REQUIRED COMPONENTS Development)

add_library(image MODULE
    modules/py_image.c

    common/array.c
    common/queue.c
    common/trace.c

    imlib/imlib.c
    imlib/hough.c
    imlib/fmath.c
    imlib/edge.c
    imlib/mathop.c
    imlib/fast.c
    imlib/point.c
    imlib/rectangle.c
    imlib/collections.c
    imlib/sincos_tab.c
    imlib/xyz_tab.c
)

target_include_directories(image PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/imlib
    ${CMAKE_CURRENT_SOURCE_DIR}/py
)

target_link_libraries(image PRIVATE m ${Python3_LIBRARIES})

set_target_properties(image PROPERTIES
    PREFIX ""
    OUTPUT_NAME "image"
)