cmake_minimum_required(VERSION 3.19)
project(walpp LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui)

qt_standard_project_setup()

qt_add_executable(walpp
        src/main.cpp
        include/color.h src/color.cpp
        include/util.h src/util.cpp
        include/appexception.h src/appexception.cpp
        include/wallpaper.h src/wallpaper.cpp
        include/settings.h
        include/image.h src/image.cpp
        include/sequences.h src/sequences.cpp
        include/types.h
        src/settings.cpp
        include/reload.h src/reload.cpp
        include/backend.h src/backend.cpp
        include/theme.h src/theme.cpp
        include/export.h src/export.cpp
        include/logging.h src/logging.cpp)

target_link_libraries(walpp
    PRIVATE
        Qt::Core
        Qt::Gui
)

include(GNUInstallDirs)

install(TARGETS walpp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET walpp
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
