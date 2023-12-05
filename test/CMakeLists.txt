set(SAVE_C_CLANG_TIDY_OPTION ${CMAKE_C_CLANG_TIDY})
set(CMAKE_C_CLANG_TIDY "")
FetchContent_Declare(
    unity
    GIT_REPOSITORY "https://github.com/ThrowTheSwitch/Unity.git"
    GIT_TAG "origin/master"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/vendor/Unity"
    )
FetchContent_MakeAvailable(unity)

if(NOT unity_POPULATED)
    FetchContent_Populate(unity)
endif()

FetchContent_GetProperties(unity)
set(CMAKE_C_CLANG_TIDY ${SAVE_C_CLANG_TIDY_OPTION})

add_library(
    libunity
    STATIC
    "${unity_SOURCE_DIR}/src/unity.c"
    )
set_target_properties(libunity PROPERTIES C_CLANG_TIDY "")
target_compile_definitions(libunity PUBLIC UNITY_INCLUDE_DOUBLE=1)
target_include_directories(
    libunity
    PUBLIC
    "${unity_SOURCE_DIR}/src"
    )

macro(test NAME SRC)
    add_executable(unittest_${NAME} ${SRC})
    target_link_libraries(unittest_${NAME} PRIVATE libeagle libunity)
    target_include_directories(
        unittest_${NAME}
        PRIVATE
        "${PROJECT_SOURCE_DIR}/src/include"
        )
    add_test(NAME unittest_${NAME} COMMAND unittest_${NAME})
endmacro()


test(bstree test_bstree.c)
test(hmap test_hmap.c)
test(llist test_llist.c)
test(matrix test_matrix.c)
test(vector test_vector.c)
