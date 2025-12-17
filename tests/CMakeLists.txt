cmake_minimum_required(VERSION 3.10)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
# Para Windows: Prevenir que se sobrescriban las configuraciones del compilador/enlazador del proyecto padre
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Definir el ejecutable de pruebas
add_executable(run_tests
    main_test.cpp
    LogicTests.cpp
    ../src/database/DatabaseManager.cpp
    ../src/models/UsuarioUCO.cpp
    ../src/models/Alumno.cpp
    ../src/models/Tutor.cpp
)

# Enlazar con Google Test y Qt
target_link_libraries(run_tests
    GTest::gtest_main
    Qt5::Widgets
    Qt5::Sql
)

# Directorios de inclusión
target_include_directories(run_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# Registrar pruebas
include(GoogleTest)
gtest_discover_tests(run_tests)
