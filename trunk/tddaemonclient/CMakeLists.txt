set (GWT_PATH "/home/olivier/tmp/gwt-linux-1.4.61/")
set (GWT_USER "gwt-user.jar")
set (GWT_DEV "gwt-dev-linux.jar")
set (GWT_STYLE "DETAILED")

ADD_CUSTOM_TARGET( COMPILE_GWT
    COMMAND ${JAVA_RUNTIME} -Xmx256M -cp \"${CMAKE_CURRENT_BINARY_DIR}/src:${CMAKE_CURRENT_BINARY_DIR}/bin:${GWT_PATH}${GWT_USER}:${GWT_PATH}${GWT_DEV}:${CMAKE_CURRENT_BINARY_DIR}/src/com/teardrop/public/lib/gwtext.jar\" com.google.gwt.dev.GWTCompiler -out "${CMAKE_CURRENT_BINARY_DIR}/www" \"\$\@\" -style ${GWT_STYLE} com.teardrop.WebInterface;
)
