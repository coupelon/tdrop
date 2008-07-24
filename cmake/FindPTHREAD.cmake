# Copyright 2008 Olivier COUPELON
# Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
# You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and limitations under the License.

# - Try to find the pthread library

FIND_PATH(PTHREAD_INCLUDE_DIR pthread.h PATHS /include /usr/include /usr/local/include )
FIND_LIBRARY(PTHREAD_LIBRARIES NAMES pthread pthreadGC2 pthreadGCE2 PATHS /lib /usr/lib /usr/local/lib )

IF(PTHREAD_INCLUDE_DIR AND PTHREAD_LIBRARIES)
  SET(PTHREAD_FOUND 1)
  if(NOT PTHREAD_FIND_QUIETLY)
   message(STATUS "Found pthread: ${PTHREAD_LIBRARIES}")
  endif(NOT PTHREAD_FIND_QUIETLY)
ELSE(PTHREAD_INCLUDE_DIR AND PTHREAD_LIBRARIES)
  SET(PTHREAD_FOUND 0 CACHE BOOL "Not found pthread library")
  message(STATUS "NOT Found pthread, disabling it")
ENDIF(PTHREAD_INCLUDE_DIR AND PTHREAD_LIBRARIES)

MARK_AS_ADVANCED(PTHREAD_INCLUDE_DIR PTHREAD_LIBRARIES)
