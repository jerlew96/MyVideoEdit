cmake_minimum_required(VERSION 3.26)
project(MyVideoEdit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(RESOURCE_FILES Resources/MyVideoUI.qrc)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        OpenGL
        OpenGLWidgets
        REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src/Headers
)

add_executable(MyVideoEdit src/main.cpp
        src/myvideoeditui.cpp
        src/Headers/myvideoeditui.h
        src/Forms/myvideoeditui.ui
        src/MyVideoEditWidget.cpp
        src/Headers/MyVideoEditWidget.h
        src/MyVideoEditThread.cpp
        src/Headers/MyVideoEditThread.h
        src/MyImagePro.cpp
        src/Headers/MyImagePro.h
        src/MyFilter.cpp
        src/Headers/MyFilter.h
        ${RESOURCE_FILES}
        src/MyAudio.cpp
        src/Headers/MyAudio.h)
target_link_libraries(MyVideoEdit
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        Qt::OpenGLWidgets
)
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( MyVideoEdit  ${OpenCV_LIBS})

