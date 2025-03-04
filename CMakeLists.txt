cmake_minimum_required(VERSION 3.10)

project(VoiceAssistant VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(SOURCES
    src/Valera.cpp
    src/database.cpp
    src/commandhandler.cpp
    src/levenshtein.cpp
    src/speechProcessing.cpp
)


add_executable(voice_command ${SOURCES})

target_link_libraries(voice_command
    portaudio
    vosk
    jsoncpp
    sqlite3
)
