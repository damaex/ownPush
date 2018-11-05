# - Try to find Visual Leak Detector
#  (only for WIN32 and MSVC)
#
# Once done this will define
#
# Read-Only variables:
#  VLD_INCLUDE_DIR  - the VLD include directory
#  VLD_LIBRARY      - Link these to use VLD
#
# compile definitions:
#  VLD_ENABLED
#
#=============================================================================
#  MIT License
#  Copyright (c) 2017      daMaex <damaex@live.de>
#
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  and/or sell copies of the Software, and to permit persons to whom the
#  Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included
#  in all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
#  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#  DEALINGS IN THE SOFTWARE.
#=============================================================================
#

if (WIN32 AND MSVC)
    find_path(VLD_INCLUDE_DIR
            NAMES "vld.h"
            PATHS
            "C:/Program Files (x86)/Visual Leak Detector/include"
            )

    SET(CMAKE_FIND_LIBRARY_PREFIXES "")
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")

    find_library(VLD_LIBRARY
            NAMES "vld"
            PATHS
            "C:/Program Files (x86)/Visual Leak Detector/lib/Win64")

endif (WIN32 AND MSVC)

if (VLD_INCLUDE_DIR AND VLD_LIBRARY)
    message(STATUS "FindVLD: Found VLD header directory: ${VLD_INCLUDE_DIR} and library: ${VLD_LIBRARY}.")
    mark_as_advanced(VLD_INCLUDE_DIR VLD_LIBRARY)
    add_definitions(-DVLD_ENABLED)
else (VLD_INCLUDE_DIR AND VLD_LIBRARY)
    if (WIN32 AND MSVC)
        if (VLD_INCLUDE_DIR)
            message(STATUS "FindVLD: Could not find VLD Library.")
        else (VLD_INCLUDE_DIR)
            message(STATUS "FindVLD: Could not find VLD Header.")
        endif (VLD_INCLUDE_DIR)
    else (WIN32 AND MSVC)
        message(STATUS "FindVLD: only supported on msvc")
    endif (WIN32 AND MSVC)
endif (VLD_INCLUDE_DIR AND VLD_LIBRARY)