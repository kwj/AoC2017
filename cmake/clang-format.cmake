find_program(CLANG_FORMAT_EXE clang-format)

if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE SOURCES
        "src/*.cpp"
        "test/*.cpp"
    )

    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i ${SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXE} --dry-run ${SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()
