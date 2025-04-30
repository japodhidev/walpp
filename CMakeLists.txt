cmake_minimum_required(VERSION 3.19)
project(walpp LANGUAGES CXX)


find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui)
find_package(ImageMagick REQUIRED COMPONENTS Magick++)
include_directories("/usr/include/ImageMagick-7/")
add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )

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
        include/logging.h src/logging.cpp
        include/HaishokuAlgo.h
        include/HaishokuImage.h src/HaishokuImage.cpp src/HaishokuAlgo.cpp)

target_link_libraries(walpp
    PRIVATE
        Qt::Core
        Qt::Gui
        ${ImageMagick_LIBRARIES}
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
