FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG origin/master
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/Unity
    )
FetchContent_MakeAvailable(unity)

if(NOT unity_POPULATED)
    FetchContent_Populate(unity)
endif()

FetchContent_GetProperties(unity)

add_executable(
    unittests
    test_bstree.c
    test_hmap.c
    test_llist.c
    test_matrix.c
    test_vector.c
    ${unity_SOURCE_DIR}/src/unity.c
    )

target_include_directories(
    unittests
    ${PROJECT_SOURCE_DIR}/src/include
    ${unity_SOURCE_DIR}/src
    )
