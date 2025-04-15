cmake_minimum_required(VERSION 3.16.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

list(APPEND EXTRA_COMPONENT_DIRS components/shift_reg)
list(APPEND EXTRA_COMPONENT_DIRS components/shift_reg_led)

project(ESP32_Matrix_Demo)