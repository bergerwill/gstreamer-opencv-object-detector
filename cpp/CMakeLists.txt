cmake_minimum_required(VERSION 3.10)
# cmake_minimum_required(VERSION 3.16)

project(ObjectDetector) #Project name

set(CMAKE_CXX_STANDARD 14) #setting C++ 14 standard
find_package(PkgConfig) #finding pkg-config is a helper tool

#using pkg-config to getting Gstreamer
pkg_check_modules(GSTREAMER REQUIRED 
                glib-2.0
                gstreamer-1.0
                gstreamer-base-1.0
                # gstreamer-video-1.0
                # gstreamer-bad-video-1.0 
                )

pkg_check_modules(OPENCV REQUIRED
                  opencv4 )


# set(OPENCV_PREFIX "/usr/local")

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


# building gstreamer plugin
add_library(gstfaceblur2 SHARED src/gstreamer/gstfaceblur.cpp)
# target_link_libraries(gstfaceblur2  ${GSTREAMER_LIBRARIES} ${OPENCV_LIBRARIES})
target_link_libraries(gstfaceblur2  ${GSTREAMER_LIBRARIES} ${OPENCV_LIBRARIES} "gstopencv-1.0")


#building target executable
add_executable(${PROJECT_NAME} src/main.cpp)

#linking Gstreamer library with target executable
target_link_libraries(${PROJECT_NAME} ${GSTREAMER_LIBRARIES})