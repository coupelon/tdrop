ADD_CUSTOM_TARGET( all
    COMMAND ${JAVA_RUNTIME} -Xmx256M -cp \"${CMAKE_CURRENT_BINARY_DIR}/src:${CMAKE_CURRENT_BINARY_DIR}/bin:${GWT_CP}:${CMAKE_CURRENT_BINARY_DIR}/src/com/teardrop/public/lib/gwtext.jar\" ${GWT_COMPILER} -out "${CMAKE_CURRENT_BINARY_DIR}/www" \"\$\@\" ${GWT_STYLE} com.teardrop.WebInterface;
)
