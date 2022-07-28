cmake_minimum_required(VERSION 2.8)
project( face_detect )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( face_detect face_detect.cpp FaceDetector.cpp)
target_link_libraries( face_detect ${OpenCV_LIBS} )

# Introduce preprocessor variables to keep paths of asset files
set(FACE_DETECTION_CONFIGURATION "./assets/deploy.prototxt")
set(FACE_DETECTION_WEIGHTS "./assets/res10_300x300_ssd_iter_140000_fp16.caffemodel")

target_compile_definitions(${PROJECT_NAME} PRIVATE FACE_DETECTION_CONFIGURATION="${FACE_DETECTION_CONFIGURATION}")
target_compile_definitions(${PROJECT_NAME} PRIVATE FACE_DETECTION_WEIGHTS="${FACE_DETECTION_WEIGHTS}")
