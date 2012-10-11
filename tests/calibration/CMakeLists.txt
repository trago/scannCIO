set(calibration_SRC
  main.cc
)
set(calibration_LIBS
  ${OpenCV_LIBS})

add_executable(calibration ${calibration_SRC})
target_link_libraries(calibration ${calibration_LIBS})

# -------------------------------------------------------

add_executable(Capture 
	main.cpp
	CameraCalibrator.cpp
	CameraCalibrator.h
	linefinder.h
)

target_link_libraries(Capture ${OpenCV_LIBS})

# -------------------------------------------------------
# Scanner's Project without GUI
# All the lines belong JM (--/--/--)

add_executable(Escaner
        Escaner.cpp
        scann.cpp       
        camera.cpp
        reconstructor.cpp
)

target_link_libraries(Escaner ${OpenCV_LIBS})

# -------------------------------------------------------
# Scanner's Project with GUI (Powered in Qt)
# All the lines belong JM (23/09/12)

set(GUI1_SOURCES 
	maingui.cpp
  	mainwindow.cpp
        mainwindow.ui
        camera.cpp
        reconstructor.cpp
        scann.cpp        
)

set(GUI1_HEADERS
	mainwindow.h
        camera.h
        reconstructor.h
        scann.h
)

set(GUI1_FORMS mainwindow.ui)

qt4_wrap_cpp(GUI1_HEADERS_MOC ${GUI1_HEADERS})
qt4_wrap_ui(GUI1_FORMS_HEADERS ${GUI1_FORMS})
#qt4_add_resources(GUI1_RESOURCES_RCC ${GUI1_RESOURCES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable( GUI1 ${GUI1_SOURCES}
               ${GUI1_HEADERS_MOC}
               ${GUI1_FORMS_HEADERS}
               ${GUI1_RESOURCES_RCC})

target_link_libraries( GUI1 ${QT_LIBRARIES} ${OpenCV_LIBS})
