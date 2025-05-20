cmake_minimum_required(VERSION 3.15)
project(Arqui_TPE C ASM)

set(CMAKE_C_STANDARD 11)

# Incluye headers
include_directories(${CMAKE_SOURCE_DIR}/Kernel/include)

# Archivos fuente C
set(SOURCES
        Kernel/exceptionDispatcher.c
        Kernel/idtLoader.c
        Kernel/irqDispatcher.c
        Kernel/kernel.c
        Kernel/keyboard.c
        Kernel/lib.c
        Kernel/moduleLoader.c
        Kernel/naiveConsole.c
        Kernel/sysCallDispatcher.c
        Kernel/time.c
        Kernel/time_display.c
)

# Archivos ASM
enable_language(ASM)
set(ASM_SOURCES
        Kernel/loader.asm
)

# Crea un ejecutable falso para análisis
add_executable(dummy ${SOURCES} ${ASM_SOURCES})
