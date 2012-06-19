set(calibration_SRC 
  main.cc
)
set(calibration_LIBS
  ${OpenCV_LIBS})

add_executable(calibration ${calibration_SRC})
target_link_libraries(calibration ${calibration_LIBS})
