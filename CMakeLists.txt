cmake_minimum_required (VERSION 3.2)

project (GeoAlgoVisual)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/adm/cmake" ${CMAKE_MODULE_PATH})
set (APP_VERSION_MAJOR 1)
set (APP_VERSION_MINOR 0)

# compiler flags
set (CMAKE_CXX_STANDARD 11)
if (MSVC)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:precise /EHa /MP")
  string (REGEX REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  add_definitions (-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE -DUNICODE)
endif()

if (MSVC)
  if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string (REGEX REPLACE "/W[0-4]" "/W3" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
  endif()
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshorten-64-to-32")
  endif()
  if (BUILD_SHARED_LIBS)
    if (APPLE)
      set (CMAKE_SHARED_LINKER_FLAGS "-lm ${CMAKE_SHARED_LINKER_FLAGS}")
    elseif(NOT WIN32)
      set (CMAKE_SHARED_LINKER_FLAGS "-lm ${CMAKE_SHARED_LINKER_FLAGS}")
    endif()
  endif()
endif()
# dependencies dir
set (OpenCASCADE_DIR "D:/OpenCASCADE-7.7.0-vc14-64/opencascade-7.7.0-build" CACHE PATH "Path to Open CASCADE libraries.")
set (3RDPARTY_DLL_DIRS "${OpenCASCADE_DIR}/../freeimage-3.17.0-vc14-64/bin;${OpenCASCADE_DIR}/../freetype-2.5.5-vc14-64/bin;${OpenCASCADE_DIR}/../ffmpeg-3.3.4-64/bin;${OpenCASCADE_DIR}/../tbb_2021.5-vc14-64/bin;" CACHE STRING "Paths to external DLLs separated by semicolon (FreeImage, FreeType, etc.)")
set (QT_PATH "Z:/ProgramData/QT/6.5.2/msvc2019_64")
list(APPEND CMAKE_PREFIX_PATH "${QT_PATH}/lib/cmake/Qt6/")

# Find OpenGL
find_package (OpenGL REQUIRED)
# Find Qt6
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

set_property (GLOBAL PROPERTY AUTOMOC_SOURCE_GROUP "/Generated Files/Moc")
set_property (GLOBAL PROPERTY AUTORCC_SOURCE_GROUP "/Generated Files/Resources")
set_property (GLOBAL PROPERTY AUTOUIC_SOURCE_GROUP "/Generated Files/UIC")
set_property (GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "/Generated Files/GEN")

find_package(Qt6 REQUIRED COMPONENTS
  Core
  Gui
  Widgets
  OpenGLWidgets
  OpenGL
  )
qt_standard_project_setup()

# Find Open CASCADE Technology
find_package (OpenCASCADE REQUIRED)
message (STATUS "Using OpenCASCADE from \"${OpenCASCADE_INSTALL_PREFIX}\"" )
message (STATUS "OpenCASCADE_INCLUDE_DIR=${OpenCASCADE_INCLUDE_DIR}")
message (STATUS "OpenCASCADE_LIBRARY_DIR=${OpenCASCADE_LIBRARY_DIR}")
include_directories(${OpenCASCADE_INCLUDE_DIR})
link_directories   (
  ${OpenCASCADE_LIBRARY_DIR}
  )
set (OpenCASCADE_LIBS TKRWMesh TKBinXCAF TKBin TKBinL TKOpenGl TKXCAF TKVCAF TKCAF TKV3d TKHLR TKMesh TKService TKShHealing TKPrim TKTopAlgo TKGeomAlgo TKBRep TKGeomBase TKG3d TKG2d TKMath TKLCAF TKCDF TKernel)

# main project target
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
file(
  GLOB
  sample_SOURCE_FILES
  ${SOURCE_DIR}/*.hxx
  ${SOURCE_DIR}/*.cxx
  ${SOURCE_DIR}/*.cpp
  ${SOURCE_DIR}/*.h
  ${SOURCE_DIR}/*.ui
  )
qt_add_executable(${PROJECT_NAME}
${sample_SOURCE_FILES}
)
target_link_libraries (${PROJECT_NAME} 
PRIVATE 
Qt6::Core
Qt6::Gui
Qt6::Widgets
Qt6::OpenGLWidgets
Qt6::OpenGL
${OpenCASCADE_LIBS}
${OPENGL_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
)
if (MSVC)
  set (X_COMPILER_BITNESS x64)
  get_target_property (QtCore_location Qt6::Core LOCATION)
  get_filename_component (QT_BINARY_DIR ${QtCore_location} DIRECTORY)
  set (QT_PLUGINS_DIR)
  if(EXISTS "${QT_BINARY_DIR}/../plugins")
    set (QT_PLUGINS_DIR "${QT_BINARY_DIR}/../plugins")
  endif()
  configure_file("${CMAKE_SOURCE_DIR}/adm/cmake/${PROJECT_NAME}.vcxproj.user.in" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.vcxproj.user" @ONLY)
endif()