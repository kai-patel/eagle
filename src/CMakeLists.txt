add_library(
    libeagle
    STATIC
    egl_bstree.c
    egl_graph.c
    egl_hmap.c
    egl_llist.c
    egl_matrix.c
    egl_vector.c
    )

target_include_directories(libeagle PUBLIC "${PROJECT_SOURCE_DIR}/src/include")
set_target_properties(libeagle PROPERTIES PREFIX "" OUTPUT_NAME "libeagle")
