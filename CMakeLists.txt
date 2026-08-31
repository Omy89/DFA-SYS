cmake_minimum_required(VERSION 3.16)
project(DFA-SYS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(dfa-sys
    main.cpp
    MainWindow.cpp
    MainWindow.h
    DFA.cpp
    DFA.h
    Transition.cpp
    Transition.h
    LinkedList.h
)

target_link_libraries(dfa-sys PRIVATE Qt6::Widgets)
