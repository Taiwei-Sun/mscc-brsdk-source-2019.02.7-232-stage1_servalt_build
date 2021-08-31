if (${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})

option(ENABLE_ADDRESS_SANATIZE "Enable address sanatizer" off)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

string (REPLACE " -" ";-" CXX_FLAGS        "${CMAKE_CXX_FLAGS}")
string (REPLACE " -" ";-" C_FLAGS          "${CMAKE_C_FLAGS}")
string (REPLACE " -" ";-" EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

LIST(APPEND CXX_FLAGS "-Wall")
LIST(APPEND CXX_FLAGS "-std=c++11")
LIST(APPEND CXX_FLAGS "-fno-exceptions")
LIST(APPEND CXX_FLAGS "-fno-rtti")
LIST(APPEND C_FLAGS   "-Wall")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    LIST(APPEND CXX_FLAGS "-stdlib=libstdc++")
    LIST(APPEND CXX_FLAGS "-Wno-overloaded-virtual")
    LIST(APPEND CXX_FLAGS "-Wno-mismatched-tags")
    LIST(APPEND CXX_FLAGS "-Wno-keyword-compat")
endif()

if (${ENABLE_ADDRESS_SANATIZE})
    LIST(APPEND CXX_FLAGS "-fsanitize=address")
    LIST(APPEND CXX_FLAGS "-fno-omit-frame-pointer")
    LIST(APPEND C_FLAGS "-fsanitize=address")
    LIST(APPEND C_FLAGS "-fno-omit-frame-pointer")
    LIST(APPEND EXE_LINKER_FLAGS "-fsanitize=address")
endif()

list(REMOVE_DUPLICATES CXX_FLAGS)
list(REMOVE_DUPLICATES C_FLAGS)
list(REMOVE_DUPLICATES EXE_LINKER_FLAGS)

string (REPLACE ";-" " -" CXX_FLAGS        "${CXX_FLAGS}")
string (REPLACE ";-" " -" C_FLAGS          "${C_FLAGS}")
string (REPLACE ";-" " -" EXE_LINKER_FLAGS "${EXE_LINKER_FLAGS}")

SET(CMAKE_CXX_FLAGS        "${CXX_FLAGS}")
SET(CMAKE_C_FLAGS          "${C_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${EXE_LINKER_FLAGS}")

message(STATUS "Project name = ${PROJECT_NAME}")
message(STATUS "  Branch     = ${${PROJECT_NAME}_BRANCH_NAME}")
message(STATUS "  Version    = ${${PROJECT_NAME}_VERSION}")
message(STATUS "  Hash       = ${${PROJECT_NAME}_VERSION_HASH}")
message(STATUS "  Type       = ${CMAKE_BUILD_TYPE}")
message(STATUS "  cxx_flags  = ${CMAKE_CXX_FLAGS}")
message(STATUS "  c_flags    = ${CMAKE_C_FLAGS}")
endif()
