set(url "file:///opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/CMakeTests/FileDownloadInput.png")
set(dir "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/CMakeTests/downloads")

file(DOWNLOAD
  ${url}
  ${dir}/file3.png
  TIMEOUT 2
  STATUS status
  EXPECTED_HASH SHA1=5555555555555555555555555555555555555555
  )
