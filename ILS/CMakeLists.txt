cmake_minimum_required(VERSION 3.22.1)
project( ModernCMakeLogger VERSION 1.0 LANGUAGES CXX)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(Loggerr /
ILS_Defines.h       ILS_StdLog.cpp
ILS_Logger.h	    main.cpp
ILS_LoggerStream.h
ILS_StdLog.h )


set_target_properties(Loggerr PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
