cmake_minimum_required(VERSION 3.16)

project(my-render)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include(CTest)

file(GLOB ENGINE_SOURCES CONFIGURE_DEPENDS
    include/engine/*.cpp
)

file(GLOB GLM_LIKE_SOURCES CONFIGURE_DEPENDS
    include/glm-like-lib/*.cpp
)

add_executable(my-render
    main.cpp
    src/glad.c
    ${ENGINE_SOURCES}
    ${GLM_LIKE_SOURCES}
)

target_include_directories(my-render PRIVATE
    include
)

target_link_libraries(my-render PRIVATE
    glfw
    OpenGL::GL
)

if(BUILD_TESTING)
    find_package(GTest REQUIRED)
    include(GoogleTest)

    file(GLOB UNIT_TEST_SOURCES CONFIGURE_DEPENDS
        include/glm-like-lib/unit_tests/*.cpp
    )

    add_executable(my-render-tests
        ${UNIT_TEST_SOURCES}
        ${GLM_LIKE_SOURCES}
    )

    target_include_directories(my-render-tests PRIVATE
        include
    )

    target_link_libraries(my-render-tests PRIVATE
        GTest::gtest_main
    )

    gtest_discover_tests(my-render-tests)
endif()
