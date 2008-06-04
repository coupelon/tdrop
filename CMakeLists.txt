project (TEARDROP)
cmake_minimum_required(VERSION 2.4)
set( CMAKE_VERBOSE_MAKEFILE ON )

set( CMAKE_MODULE_PATH "${TEARDROP_BINARY_DIR}/cmake/" )

find_package(LibXml2 REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ICONV)

IF( NOT ICONV_FOUND)
	SET(ICONV_LIBRARY "")
ENDIF( NOT ICONV_FOUND)

find_package(PCRE REQUIRED)
find_package(Log4Cxx REQUIRED)

include_directories( "/usr/include" ${CURL_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} )
link_directories ( "/usr/lib" ${CURL_LIBRARY_DIR} ${LIBXML2_LIBRARY_DIR} )

# Turn on warnings
ADD_DEFINITIONS(-Wall -Werror)

# Teardrop special definitions
ADD_DEFINITIONS(-DHOME_LINUX_DEV=\\"${TEARDROP_SOURCE_DIR}/tdengines/\\")
ADD_DEFINITIONS(-DWORKSPACE=\\"${TEARDROP_SOURCE_DIR}/\\")
ADD_DEFINITIONS(-DSSL_LIB=\\"${OPENSSL_LIBRARIES}\\")

add_subdirectory(shttpd)
add_subdirectory(tdcommon)
add_subdirectory(tdwrapper)
add_subdirectory(tdresults)
add_subdirectory(tddaemon)
add_subdirectory(tdconsole)
