cmake_minimum_required(VERSION 3.13)



################################################################################
## PROJECT
## name and version
################################################################################
project(examples LANGUAGES CXX)



################################################################################
## CONFIGURATION
## project configuration
################################################################################
SET(${PARENT}_DAL_EXAMPLE_BASE                          ON CACHE BOOL  "" FORCE)

# Tests
SET(${PARENT}_DAL_STRING_TO_NUM                         ON CACHE BOOL  "" FORCE)


################################################################################
## INCLUDING SUBDIRECTORIES
## Adding subdirectories according to the configuration
################################################################################
if (${PARENT}_DAL_EXAMPLE_BASE)
    add_subdirectory(dal_example_base)
endif()
# Tests
if (${PARENT}_DAL_STRING_TO_NUM)
    add_subdirectory(dal_string_to_num)
endif()
