add_executable(Blasteroids
    blasteroids.c
    asteroid.c
    asteroid.h
    blast.c
    blast.h
    spaceship.c
    spaceship.h
    calc.c
    calc.h
    boundingbox.c
    boundingbox.h
)

target_link_libraries(Blasteroids
    PRIVATE
        allegro
        allegro_primitives
        m
)

target_compile_options(Blasteroids
    PRIVATE
        -Wall
        -Wextra
)

option(DRAW_BOUNDING "Draw bounding boxes" OFF)
option(ROTATE_BOUNDING "Rotate bounding boxes" OFF)

if(DRAW_BOUNDING)
    set(DEF_DRAW_BOUNDING "DRAW_BOUNDING")
endif()

if(ROTATE_BOUNDING)
    set(DEF_ROTATE_BOUNDING "ROTATE_BOUNDING")
endif()

target_compile_definitions(Blasteroids
    PRIVATE
        ${DEF_DRAW_BOUNDING}
        ${DEF_ROTATE_BOUNDING}
)
