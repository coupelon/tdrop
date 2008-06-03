include_directories( "../tdcommon" "../tdcommon/xml" "../tdcommon/regexp" "../tdwrapper" "../tdresults" "../tddaemon")

# Make sure the linker can find the Hello library once it is built.
link_directories (${TEARDROP_BINARY_DIR}/tdcommon
                  ${TEARDROP_BINARY_DIR}/tdwrapper
                  ${TEARDROP_BINARY_DIR}/tdresults
                  ${TEARDROP_BINARY_DIR}/tddaemon
                  ${TEARDROP_BINARY_DIR}/shttpd)

# Add executable called "helloDemo" that is built from the source files
# "demo.cxx" and "demo_b.cxx".  The extensions are automatically found.
add_executable (tdconsole teardrop)

# Link the executable to the Hello library.
target_link_libraries(tdconsole tdcommon tdwrapper tdresults tddaemon ${CURL_LIBRARY} ${LIBXML2_LIBRARY} log4cxx xml2 pcre)

