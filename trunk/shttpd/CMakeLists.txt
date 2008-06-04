IF (APPLE)
  # For Mac OS X, the valid SLL file is not the default one
    ADD_DEFINITIONS(-DSSL_LIB=\\"/usr/lib/libssl.0.9.7.dylib\\")
ELSE (APPLE)
	ADD_DEFINITIONS(-DSSL_LIB=\\"${OPENSSL_LIBRARIES}\\")
ENDIF (APPLE)

add_library(shttpd string shttpd log auth md5 cgi config io_ssi
		io_file io_socket io_ssl io_emb io_dir io_cgi compat_unix)
