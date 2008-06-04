include_directories( "../tdcommon" "../tdcommon/xml" "../tdcommon/regexp" "../tdwrapper" "../tdresults" "../tddaemon")

# Make sure the linker can find the Hello library once it is built.
link_directories (${TEARDROP_BINARY_DIR}/tdcommon
                  ${TEARDROP_BINARY_DIR}/tdwrapper
                  ${TEARDROP_BINARY_DIR}/tdresults
                  ${TEARDROP_BINARY_DIR}/tddaemon
                  ${TEARDROP_BINARY_DIR}/shttpd)

add_executable (tdconsole teardrop)

target_link_libraries(tdconsole tddaemon)

