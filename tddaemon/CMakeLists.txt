include_directories( "../tdcommon" "../tdcommon/xml" "../tdcommon/regexp" "../tdwrapper" "../tdresults" "../shttpd")

link_directories(${TEARDROP_BINARY_DIR}/shttpd)

add_library(tddaemon SHARED tddaemon UIDSession users)
target_link_libraries(tddaemon shttpd tdcommon tdwrapper tdresults curl log4cxx xml2 pcre iconv)
