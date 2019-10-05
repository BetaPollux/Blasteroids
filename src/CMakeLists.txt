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
