# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)

# O arquivo pico-vscode.cmake precisa ser incluído se estiver disponível
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if(EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

# Set the target board (adjust if necessary)
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Certifique-se de que o SDK está configurado corretamente
# Isso importa o CMake para o SDK do Raspberry Pi Pico
# Altere o caminho do SDK se necessário
set(PICO_SDK_PATH "caminho/para/o/pico-sdk")  # Substitua com o caminho correto
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(interrupcao2 C CXX ASM)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(interrupcao2 interrupcao2.c)

# Define program name and version
pico_set_program_name(interrupcao2 "interrupcao2")
pico_set_program_version(interrupcao2 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(interrupcao2 1)
pico_enable_stdio_usb(interrupcao2 1)

# Link with the Raspberry Pi Pico standard library
target_
