cmake_minimum_required(VERSION 3.29)
project(GestionHotel C)

set(CMAKE_C_STANDARD 11)

add_executable(GestionHotel main.c
        client.c
        directeur.c
        io.c
        headers/structures.h
        headers/io.h
        headers/client.h
        headers/directeur.h
        display.c
        headers/display.h)
