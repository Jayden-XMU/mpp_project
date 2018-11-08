# Install script for directory: F:/mpp_debug/mpp/mpp/hal

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("F:/mpp_debug/mpp/build/mpp/hal/vpu/cmake_install.cmake")
  include("F:/mpp_debug/mpp/build/mpp/hal/rkdec/cmake_install.cmake")
  include("F:/mpp_debug/mpp/build/mpp/hal/rkenc/cmake_install.cmake")
  include("F:/mpp_debug/mpp/build/mpp/hal/dummy/cmake_install.cmake")
  include("F:/mpp_debug/mpp/build/mpp/hal/rkdec/avsd/cmake_install.cmake")
  include("F:/mpp_debug/mpp/build/mpp/hal/worker/libvpu/cmake_install.cmake")
  include("F:/mpp_debug/mpp/build/mpp/hal/worker/libv4l2/cmake_install.cmake")

endif()
