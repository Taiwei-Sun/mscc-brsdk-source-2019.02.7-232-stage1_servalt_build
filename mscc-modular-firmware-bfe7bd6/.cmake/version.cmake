execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/.cmake/version.sh" ${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)

include(${CMAKE_CURRENT_BINARY_DIR}/buildinfo_${PROJECT_NAME}.cmake)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(reruntarget_${PROJECT_NAME} ALL)
add_custom_command(OUTPUTS "file-which-does-not-exists"
                   COMMAND "touch" ${CMAKE_CURRENT_BINARY_DIR}/buildinfo_${PROJECT_NAME}.cmake
                   TARGET  reruntarget_${PROJECT_NAME})


# Offer the user the choice of overriding the installation directories
set(PROJECT_VERSION ${${PROJECT_NAME}_VERSION1})
set(PROJECT_VERSION_FULL ${${PROJECT_NAME}_VERSION})
set(PROJECT_VNAME ${PROJECT_NAME}${${PROJECT_NAME}_VERSION1})

set(INSTALL_LIB_DIR     "lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR     "bin" CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR "include" CACHE PATH "Installation directory for header files")
set(INSTALL_DATA_DIR    "share/${PROJECT_VNAME}" CACHE PATH "Installation directory for data files")
set(INSTALL_CMAKE_DIR   "${INSTALL_DATA_DIR}/cmake" CACHE PATH "Installation directory for cmake files")

