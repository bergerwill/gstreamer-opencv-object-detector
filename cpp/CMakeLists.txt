cmake_minimum_required(VERSION 3.16)

project(ObjectDetector) #Project name

set(CMAKE_CXX_STANDARD 14) #setting C++ 14 standard
find_package(PkgConfig) #finding pkg-config is a helper tool

#using pkg-config to getting Gstreamer
pkg_check_modules(GSTREAMER REQUIRED 
                glib-2.0
                gstreamer-1.0
                gstreamer-base-1.0
                gstreamer-video-1.0
                )

pkg_check_modules(OPENCV REQUIRED
                  opencv4 )



message(STATUS "GSTREAMER_INCLUDE_DIRS: ${GSTREAMER_INCLUDE_DIRS}")
message(STATUS "GSTREAMER_LIBRARY_DIRS: ${GSTREAMER_LIBRARY_DIRS}")
message(STATUS "GSTREAMER_LINK_LIBRARIES: ${GSTREAMER_LINK_LIBRARIES}")
message(STATUS "GSTREAMER_LIBRARIES: ${GSTREAMER_LIBRARIES}")

message(STATUS "OPENCV_INCLUDE_DIRS: ${OPENCV_INCLUDE_DIRS}")
message(STATUS "OPENCV_LIBRARIES: ${OPENCV_LIBRARIES}")



#including GStreamer header files directory
include_directories(
        ${OPENCV_INCLUDE_DIRS}
        ${GSTREAMER_INCLUDE_DIRS}
)

#linking GStreamer library directory
link_directories(
        ${GSTREAMER_LIBRARIES}
)


# build image blur element 
add_library(gstimageblur SHARED src/gstreamer/gstimageblur.cpp )
target_link_libraries(gstimageblur  ${GSTREAMER_LIBRARIES} ${OPENCV_LIBRARIES})


#building target executable
add_executable(${PROJECT_NAME} src/main.cpp)

#linking Gstreamer library with target executable
target_link_libraries(${PROJECT_NAME} ${GSTREAMER_LIBRARIES})