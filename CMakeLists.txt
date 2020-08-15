cmake_minimum_required (VERSION 3.8)

set(src_all
    blackhole
    cgl_shader
    tools
    draw_texture
    main
)

add_executable (Blackhole_Simulator ${src_all})

set(targets
    Blackhole_Simulator
)

set(copy_files
    lib/glut32.dll
    lib/glew32.dll
)

set(copy_diretories
    lib
    res
)


find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/gl/)

foreach(tg ${targets})

    target_link_libraries(${tg} ${OPENGL_LIBRARIES} ${CMAKE_SOURCE_DIR}/lib/glut32.lib ${CMAKE_SOURCE_DIR}/lib/glew32.lib)

    foreach(file ${copy_files})
        add_custom_command(
            TARGET ${tg} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_SOURCE_DIR}/${file}
                    ${CMAKE_CURRENT_BINARY_DIR})
    endforeach()

    foreach(diretory ${copy_diretories})
        add_custom_command(
                TARGET ${tg} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${CMAKE_SOURCE_DIR}/${diretory}
                        ${CMAKE_CURRENT_BINARY_DIR}/${diretory})
    endforeach()

endforeach()