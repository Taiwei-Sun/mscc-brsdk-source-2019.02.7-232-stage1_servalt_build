file(REMOVE_RECURSE "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/CMakeTests/uploads")

if(EXISTS "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/CMakeTests/uploads/file1.png")
  message(FATAL_ERROR "error: file1.png exists - should have been deleted")
endif()
if(EXISTS "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/CMakeTests/uploads/file2.png")
  message(FATAL_ERROR "error: file2.png exists - should have been deleted")
endif()

file(MAKE_DIRECTORY "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/CMakeTests/uploads")

set(filename "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/CMakeTests/FileDownloadInput.png")
set(urlbase "file:///opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/CMakeTests/uploads")

message(STATUS "FileUpload:1")
file(UPLOAD
  ${filename}
  ${urlbase}/file1.png
  TIMEOUT 2
  )

message(STATUS "FileUpload:2")
file(UPLOAD
  ${filename}
  ${urlbase}/file2.png
  STATUS status
  LOG log
  SHOW_PROGRESS
  )

execute_process(COMMAND ${CMAKE_COMMAND} -E md5sum
  "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/CMakeTests/uploads/file1.png"
  OUTPUT_VARIABLE sum1
  OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT sum1 MATCHES "^d16778650db435bda3a8c3435c3ff5d1  .*/uploads/file1.png$")
  message(FATAL_ERROR "file1.png did not upload correctly (sum1='${sum1}')")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E md5sum
  "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/CMakeTests/uploads/file2.png"
  OUTPUT_VARIABLE sum2
  OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT sum2 MATCHES "^d16778650db435bda3a8c3435c3ff5d1  .*/uploads/file2.png$")
  message(FATAL_ERROR "file2.png did not upload correctly (sum2='${sum2}')")
endif()

message(STATUS "log='${log}'")
message(STATUS "status='${status}'")
message(STATUS "DONE")
