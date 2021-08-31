
find_library(FOO_LIBRARY NAMES foo HINTS "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/FindPackageModeMakefileTest" )
find_path(FOO_INCLUDE_DIR NAMES foo.h HINTS "/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-cmake-3.8.2/Tests/FindPackageModeMakefileTest" )

set(FOO_LIBRARIES ${FOO_LIBRARY})
set(FOO_INCLUDE_DIRS "${FOO_INCLUDE_DIR}"  "/some/path/with a space/include" )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Foo  DEFAULT_MSG  FOO_LIBRARY FOO_INCLUDE_DIR )
