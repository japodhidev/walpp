cmake_minimum_required(VERSION 3.19)
project(walpp LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui)

qt_standard_project_setup()

qt_add_executable(walpp
    main.cpp
    color.h color.cpp
    util.h util.cpp
    appexception.h appexception.cpp
    wallpaper.h wallpaper.cpp
    settings.h
    image.h image.cpp
)

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
