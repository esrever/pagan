PROJECT(qt-test1-console)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC OFF)
#QT5_WRAP_CPP(helloworld_HEADERS_MOC ${helloworld_HEADERS})

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies.
find_package(Qt5Widgets REQUIRED)

add_executable(qt-test1-console main.cpp)

qt5_use_modules(qt-test1-console Widgets)
#qt5_wrap_ui
#qt5_add_resources
#qt5_wrap_cpp