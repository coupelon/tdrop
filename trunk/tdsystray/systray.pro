HEADERS       = systray.h daemonThread.h 
SOURCES       = main.cpp systray.cpp daemonThread.cpp
RESOURCES     = systray.qrc
INCLUDEPATH   = ../tddaemon ../tdcommon ../tdcommon/xml ../tdcommon/regexp ../tdwrapper 
QMAKE_CXXFLAGS  = $$system(xml2-config --cflags)
LIBS          = ../tdcommon/Debug/libtdcommon.so ../tddaemon/Debug/libtddaemon.so ../tdwrapper/Debug/libtdwrapper.so \
		$$system(xml2-config --libs) $$system(curl-config --libs)  $$system(pcre-config --libs)
