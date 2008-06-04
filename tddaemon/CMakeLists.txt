include_directories( "../tdcommon" "../tdcommon/xml" "../tdcommon/regexp" "../tdwrapper" "../tdresults" "../shttpd")

link_directories(${TEARDROP_BINARY_DIR}/shttpd)

add_library(tddaemon SHARED tddaemon UIDSession users)
target_link_libraries(tddaemon shttpd tdcommon tdwrapper tdresults ${ICONV_LIBRARY} ${CURL_LIBRARY} ${LIBXML2_LIBRARY} ${PCRE_LIBRARIES} ${LOG4CXX_LIBRARY})
