cmake_minimum_required(VERSION 3.10)
project(tag_invoke VERSION 1.0.0 LANGUAGES CXX)

add_library(tag_invoke INTERFACE)
add_library(ju::tag_invoke ALIAS tag_invoke)

target_include_directories(tag_invoke INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(tag_invoke INTERFACE cxx_std_20)

include(GNUInstallDirs)
install(FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/include/ju/tag_invoke.hpp"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ju)
install(TARGETS tag_invoke EXPORT ${PROJECT_NAME}Targets)
install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ju::
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/ju)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/tag_invokeConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)
install(FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/tag_invokeConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/tag_invokeConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/ju
)

# example
option(TAG_INVOKE_BUILD_EXAMPLES "Build example programs" OFF)

if(TAG_INVOKE_BUILD_EXAMPLES)
  add_executable(tag_invoke_example example/example.cpp)
  target_link_libraries(tag_invoke_example PRIVATE tag_invoke)
endif()
