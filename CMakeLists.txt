cmake_minimum_required(VERSION 2.8)
# set project's name
PROJECT( scannCIO )
  
  # enable warnings
ADD_DEFINITIONS( -Wall )

set(CMAKE_BUILD_TYPE "Debug")
  
  # by default only QtCore and QtGui modules are enabled
  # other modules must be enabled like this:
SET( QT_USE_QTXML TRUE )
set(QT_USE_QTOPENGL TRUE) #Added by JM (23/09/12)
  
  # this command finds Qt4 libraries and sets all required variables
  # note that it's Qt4, not QT4 or qt4
FIND_PACKAGE( Qt4 REQUIRED )
# Look for required libraries and tools
find_package(OpenCV REQUIRED)  

  # add some useful macros and variables
  # (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
INCLUDE( ${QT_USE_FILE} )
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )

add_definitions(${QT_DEFINITIONS}) #Added by JM (23/09/12)
add_subdirectory(tests)  
