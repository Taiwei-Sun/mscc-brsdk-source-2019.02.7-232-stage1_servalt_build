# CMake generated Testfile for 
# Source directory: /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/FindThreads
# Build directory: /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/FindThreads
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(FindThreads.C-only "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/bin/ctest" "--build-and-test" "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/FindThreads/C-only" "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/FindThreads/C-only" "--build-generator" "Unix Makefiles" "--build-project" "FindThreads_C-only" "--build-options" "-DCMAKE_MAKE_PROGRAM:FILEPATH=/usr/bin/make" "--test-command" "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/bin/ctest" "-V")
add_test(FindThreads.CXX-only "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/bin/ctest" "--build-and-test" "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/FindThreads/CXX-only" "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/FindThreads/CXX-only" "--build-generator" "Unix Makefiles" "--build-project" "FindThreads_CXX-only" "--build-options" "-DCMAKE_MAKE_PROGRAM:FILEPATH=/usr/bin/make" "--test-command" "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/bin/ctest" "-V")