project (TEARDROP)
cmake_minimum_required(VERSION 2.4)
#set( CMAKE_VERBOSE_MAKEFILE ON )

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
find_package(Qt4)
find_package(Java)
find_package(GWT)

include_directories( "/usr/include" ${CURL_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} )
link_directories ( "/usr/lib" ${CURL_LIBRARY_DIR} ${LIBXML2_LIBRARY_DIR} )

# Turn on warnings
ADD_DEFINITIONS(-Wall -Werror)

# Teardrop special definitions
ADD_DEFINITIONS(-DHOME_LINUX_DEV=\\"${TEARDROP_SOURCE_DIR}/tdengines/\\")
ADD_DEFINITIONS(-DWORKSPACE=\\"${TEARDROP_SOURCE_DIR}/\\")

add_subdirectory(shttpd)
add_subdirectory(tdcommon)
add_subdirectory(tdwrapper)
add_subdirectory(tdresults)
add_subdirectory(tddaemon)
add_subdirectory(tdconsole)

IF (QT4_FOUND)
	MESSAGE(STATUS "QT4 found, so systray will be build")
	INCLUDE(${QT_USE_FILE})
	add_subdirectory(tdsystray)
ELSE (QT4_FOUND)
	MESSAGE(STATUS "QT4 not found, so systray won't be build")
ENDIF(QT4_FOUND)

IF (JAVA_RUNTIME AND GWT_FOUND)
    MESSAGE(STATUS "GWT and Java found, the client will be build")
	add_subdirectory(tddaemonclient)
ELSE  (JAVA_RUNTIME AND GWT_FOUND)
	MESSAGE(STATUS "GWT or Java not found, the client won't be build")
ENDIF  (JAVA_RUNTIME AND GWT_FOUND)