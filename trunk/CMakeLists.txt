project (TEARDROP)
cmake_minimum_required(VERSION 2.4)
set( CMAKE_VERBOSE_MAKEFILE ON )

find_package(LibXml2 REQUIRED)
find_package(CURL REQUIRED)
#find_package(PCRE REQUIRED)

include_directories( "/usr/include" ${CURL_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} )
link_directories ( "/usr/lib" ${CURL_LIBRARY_DIR} ${LIBXML2_LIBRARY_DIR} )

# Turn on warnings
ADD_DEFINITIONS(-Wall -Werror)

# Teardrop special definitions
ADD_DEFINITIONS(-DHOME_LINUX_DEV=\\"${TEARDROP_SOURCE_DIR}/tdengines/\\")
ADD_DEFINITIONS(-DWORKSPACE=\\"${TEARDROP_SOURCE_DIR}/\\")

add_subdirectory(tdcommon)
add_subdirectory(tdwrapper)
add_subdirectory(tdresults)
add_subdirectory(tddaemon)
add_subdirectory(tdconsole)
