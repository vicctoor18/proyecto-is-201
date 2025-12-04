cmake_minimum_required(VERSION 3.10)
project(SistemaTutorizacion)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Sql REQUIRED)

add_executable(SistemaTutorizacion
    src/main.cpp
    src/database/DatabaseManager.cpp
    src/database/DatabaseManager.h
    src/models/UsuarioUCO.cpp
    src/models/UsuarioUCO.h
    src/models/Alumno.cpp
    src/models/Alumno.h
    src/models/Tutor.cpp
    src/models/Tutor.h
    src/gui/LoginWindow.cpp
    src/gui/LoginWindow.h
    src/gui/RegisterWindow.cpp
    src/gui/RegisterWindow.h
    src/gui/StudentWindow.cpp
    src/gui/StudentWindow.h
    src/gui/TutorWindow.cpp
    src/gui/TutorWindow.h
)

target_link_libraries(SistemaTutorizacion Qt5::Widgets Qt5::Sql)

target_include_directories(SistemaTutorizacion PRIVATE src)
