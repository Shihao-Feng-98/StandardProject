cmake_minimum_required(VERSION 3.13)

# option(<variable> "<help_text> [value]") 给用户提供一个布尔值的可选项
option(BUILD_PYTHON "Build python pybinds" OFF)
option(BUILD_EXAMPLE "Build c++ use examples" ON)
option(BUILD_TEST "Build c++ doc tests" OFF)

set(CMAKE_CXX_STANDARD 11) # 设置cpp标准，相当于-std=c+=11
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 指定CMAKE_CXX_STANDARD变量的值为必须
set(CMAKE_CXX_EXTENSIONS OFF) # 禁止编译器特定的扩展，仅使用ISO的cpp标准

# 设置项目名称，储存在PROJECT_NAME变量
# 同时会设置PROJECT_SOURCE_DIR变量为源目录的绝对路径
# 设置PROJECT_BINARY_DIR变量为二进制目录的绝对路径
set(project_name StandardProject)
project(${project_name} 
        VERSION 1.0.0
        LANGUAGES CXX)
message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
# 最近一次调用project的CMakeList.txt所在的源和二进制目录
message(DEBUG "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message("PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
# 当前CMakeList.txt所在的源和二进制目录
message(DEBUG "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(DEBUG "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

# 应该使用 "cmake .." 而不是 "cmake ." 否则配置构建系统抛出错误
if (PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
  message(WARNING "The binary directory of CMake should not be the same as source directory!")
endif()

# 设置默认为Release
if (NOT CMAKE_BUILD_TYPE)
  # Release相当于-O3 / Debug相当于-O0
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "build type")
endif()

# 用ccache作为编译器的包装器，提升编译速度
find_package(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
  message(STATUS "Found CCache: ${CCACHE_FOUND}")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_FOUND})
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_FOUND})
else()
  message(WARNING "ccahe is strongly suggested to reduce re-compiling time.")
endif()

# 指定创建位置无关的可执行文件和共享库，相当于-fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON) 
# 在Debug生成的动态库后加上"_d"
set(CMAKE_DEBUG_POSTFIX "_d") 
# 采用外部构建的方式，CMAKE_BINARY_DIR表示编译的绝对路径，一般是Build路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin) # 指定可执行文件路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib) # 指定.so动态库路径
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib) # 指定.a静态库路径

# 用于指定make install时的相对路径前缀
set(CMAKE_INSTALL_PREFIX
    $ENV{HOME}/StandardProjectFramework/Install/${project_name}
    CACHE PATH "install prefix")

# 使用以下cmake命令，提供GNU标准预定义的安装目录变量，包括
# 可执行文件安装目录 ${CMAKE_INSTALL_BINDIR} -> /usr/local/bin
# 库文件安装目录    ${CMAKE_INSTALL_LIBDIR} -> /usr/local/lib
# 头文件安装目录    ${CMAKE_INSTALL_INCLUDEDIR} -> /usr/local/include
include(GNUInstallDirs)

########### 请查看RPS确认 ###########
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

# 向编译器添加编译选项
# -Wall -Wextra 用于启用严格的警告，会有大量的警告输出
add_compile_options(-Wall -Wextra)

########### 请查看RPS确认 ###########
# 向编译器添加预处理宏定义
# 可以在源码中使用条件编译，根据预定义的宏选择性地编译不同的代码段，或者定义配置选项和功能开关
if(ENABLE_ENTITLEMENT_CHECK)
  add_definitions(-DENABLE_ENTITLEMENT_CHECK)
endif()

########### 请查看RPS确认 ###########
# list(APPEND <list_variable> <element1> <element1>) 添加新的元素到现有的列表中
# CMAKE_MODULE_PATH是cmake模块文件路径，向它添加新的文件路径
# 模块文件通常有.cmake扩展名，包含一些定义、函数或宏，用于配置和构建项目
# 例如，有自定义模块文件MyModule.cmake放置在CMake子目录中，可以使用include(MyModule)来引入自定义模块
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
# 添加Macros.cmake模块文件
include(CMake/Macros.cmake)
# CMAKE_PREFIX_PATH是第三方库和工具的根路径，向它添加新的文件路径
# 例如，有第三方库ThirdPartyLibrary放置在目录中，可以使用find_package(ThirdPartyLibrary REQUIRED)来搜索，
# 并自动配置ThirdPartyLibrary_INCLUDE_DIRS和ThirdPartyLibrary_LIBRARIES等变量
list(APPEND CMAKE_PREFIX_PATH $ENV{HOME}/Rvbust/Install)

########### 请查看RPS确认 ###########
# find_package(<package_variable> REQUIRED) 查找指定的软件包或库，REQUIRE表示包是必须的，找不到会报错
# 查找最低版本为3.3.7的Eigen库，不仅查找名为Eigen3还查找名为Eigen的库 
find_package(Eigen3 3.3.7 REQUIRED NAMES Eigen Eigen3)
find_package(pybind11 REQUIRED)
find_package(spdlog 1.3.0 REQUIRED)
find_package(fcl REQUIRED)
find_package(ompl REQUIRED)

########### 请查看RPS确认 ###########
# 包含CMake提供的CMakePackageConfigHelpers模块
include(CMakePackageConfigHelpers)
# 生成一个.cmake文件，包含当前项目的版本信息
# COMPATIBILITY SameMajorVersion -> 表示生成的版本文件将于当前项目的主版本号兼容
write_basic_package_version_file(
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)
# 配置生成的包配置文件
configure_package_config_file(
  CMake/Config.cmake.in                                                # 包配置文件的输入模板路径
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake                      # 包配置文件的输出路径
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}    # 包配置文件的安装路径
)
configure_package_config_file(
  CMake/ControllerConfig.cmake.in 
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ControllerConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
configure_package_config_file(
  CMake/PlannerConfig.cmake.in 
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}PlannerConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

########### 请查看RPS确认 ###########
# 添加头文件搜索路径
include_directories(Externals)
# 添加子目录路径，通过该命令CMake会进入子目录，并继续处理目录下的CMakeLists.txt文件
add_subdirectory(Externals)
add_subdirectory(Modules)

########### 请查看RPS确认 ###########
# install(FILES DESTINATION COMPONENT) 用于将指定文件复制到指定目录
# 安装文件到默认安装路径中
install(
  FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake            # 需要安装的文件列表
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ControllerConfig.cmake
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}PlannerConfig.cmake
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}       # 指定安装目标路径
  COMPONENT ${PROJECT_NAME}_Development                           # 指定目标安装组件的名称
  # 用于指示安装目标所属的组件，使得用户可以按需安装所需的文件，同时也方便进行卸载和更新操作
)
# file(GLOB <variable> <globbing-expressions>) 用于匹配文件路径，将符合条件的路径保存到变量中
file(GLOB FIND_PKG_CMAKE_FILES ${CMAKE_SOURCE_DIR}/CMake/Find*.cmake)
# 安装Findxxx.cmake到默认安装路径中
install(FILES ${FIND_PKG_CMAKE_FILES}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        COMPONENT ${PROJECT_NAME}_Development)
# 安装CMake/Macros.cmake到默认安装路径中
install(FILES CMake/Macros.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        COMPONENT ${PROJECT_NAME}_Development)

########### 请查看RPS确认 ###########
# configure_file() 用于配置文件，根据模板内容和变量值生成目标文件
# 
configure_file(${CMAKE_SOURCE_DIR}/CMake/CMakeUninstall.cmake.in   # 模板文件路径
               ${CMAKE_BINARY_DIR}/CMakeUninstall.cmake            # 目标文件路径
               IMMEDIATE @ONLY                                     # 立即生成目标文件，@ONLY 选项表示仅替换模板文件中的 @VAR@ 形式的变量，并忽略其他 CMake 语法
)
# 创建自定义目标，该目标可以指执行指定的命令或脚本
add_custom_target(uninstall COMMAND                                             # uninstall ->  自定义目标名称  
                  ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/CMakeUninstall.cmake  # 具体要执行的命令
                 # CMake执行 解释器模式下执行 要执行的命令或脚本 
)
# 添加Packaging.cmake模块文件
include(CMake/Packaging.cmake)
