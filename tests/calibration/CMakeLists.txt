set(calibration_SRC 
  main.cc
)
set(calibration_LIBS
  ${OpenCV_LIBS})

add_executable(calibration ${calibration_SRC})
target_link_libraries(calibration ${calibration_LIBS})

add_executable(Capture 
	main.cpp
	CameraCalibrator.cpp
	CameraCalibrator.h
	linefinder.h
)
target_link_libraries(Capture ${OpenCV_LIBS})
