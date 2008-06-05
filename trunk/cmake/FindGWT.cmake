# Copyright 2008 Olivier COUPELON
# Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
# You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and limitations under the License.

FIND_PATH(GWT_INCLUDE_DIR gwt-user.jar /opt/gwt /opt/gwt-1.5 /opt/gwt-1.4)

IF (GWT_INCLUDE_DIR)
   SET(GWT_FOUND TRUE)
ENDIF (GWT_INCLUDE_DIR)

IF (GWT_FOUND)
   SET (GWT_USER "gwt-user.jar")

   SET (GWT_COMPILER "com.google.gwt.dev.GWTCompiler")

   #set GWT_DEV according to the platform (3 supported currently)
   IF (APPLE)
      #mac os x
      SET (GWT_DEV gwt-dev-mac.jar)
   ELSEIF(WIN32)
      #windows
      SET (GWT_DEV gwt-dev-win.jar)
   ELSE(APPLE)
      #consider other environment defaulting to linux
      SET (GWT_DEV gwt-dev-linux.jar)
   ENDIF(APPLE)

   SET (GWT_CP ${GWT_INCLUDE_DIR}/${GWT_USER}:${GWT_INCLUDE_DIR}/${GWT_DEV})

   IF (NOT GWT_FIND_QUIETLY)
      MESSAGE(STATUS "Found GWT: ${GWT_INCLUDE_DIR}")
   ENDIF (NOT GWT_FIND_QUIETLY)
ELSE (GWT_FOUND)
   IF (GWT_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find GWT")
   ENDIF (GWT_FIND_REQUIRED)
ENDIF (GWT_FOUND)