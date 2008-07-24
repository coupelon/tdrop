# Copyright 2008 Olivier COUPELON
# Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
# You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and limitations under the License.

# Search the path of ws2_32 which is required under windows

FIND_LIBRARY(WS2_32_LIBRARIES NAMES ws2_32 PATHS /lib)

IF(WS2_32_LIBRARIES)
  SET(WS2_32_FOUND 1)
  if(NOT WS2_32_FIND_QUIETLY)
   message(STATUS "Found ws2_32: ${WS2_32_LIBRARIES}")
  endif(NOT WS2_32_FIND_QUIETLY)
ELSE(WS2_32_LIBRARIES)
  SET(WS2_32_FOUND 0 CACHE BOOL "Not found ws2_32 library")
  message(STATUS "NOT Found ws2_32, disabling it")
ENDIF(WS2_32_LIBRARIES)

MARK_AS_ADVANCED(WS2_32_LIBRARIES)
