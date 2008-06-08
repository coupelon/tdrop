set(QT_MT_REQUIRED TRUE)

add_definitions (${QT_DEFINITIONS}  -DHAVE_CONFIG_H=1)
include_directories (${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

# Headers list
set(HEADERS       systray.h daemonThread.h prefDialog.h)
# Sources list
set(SOURCES       main.cpp systray.cpp daemonThread.cpp prefDialog.cpp)
# Porject UIS
set(FORMS         preferences.ui)
# The projects resources
set(RESOURCES     systray.qrc)

QT4_ADD_RESOURCES(SOURCES ${RESOURCES})

QT4_WRAP_UI(FORMS_H ${FORMS})

QT4_WRAP_CPP(MOC_SOURCES ${HEADERS})

include_directories(../tddaemon ../tdcommon ../tdcommon/xml ../tdcommon/regexp ../tdwrapper ../tdresults)

add_executable (teardrop ${FORMS_H} ${SOURCES} ${MOC_SOURCES})

target_link_libraries(teardrop tddaemon ${QT_LIBRARIES})
