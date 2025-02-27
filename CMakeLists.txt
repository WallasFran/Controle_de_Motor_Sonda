# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico CACHE STRING "Board type")

# Incluir o SDK da Raspberry Pi Pico
include(pico_sdk_import.cmake)

project(SONDA_MOTOR C CXX ASM)
pico_sdk_init()

# Criar o executável com o código fonte
add_executable(SONDA_MOTOR SONDA_MOTOR.c lib/ssd1306.c)

# Definir o nome e versão do programa
pico_set_program_name(SONDA_MOTOR "SONDA_MOTOR")
pico_set_program_version(SONDA_MOTOR "0.1")

# Desabilitar UART, mas habilitar USB para a saída padrão
pico_enable_stdio_uart(SONDA_MOTOR 0)
pico_enable_stdio_usb(SONDA_MOTOR 1)

# Adicionar as bibliotecas necessárias para o seu projeto
target_link_libraries(SONDA_MOTOR pico_stdlib hardware_i2c hardware_adc hardware_pwm)

# Incluir o diretório atual
target_include_directories(SONDA_MOTOR PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Adicionar saídas extras do Pico
pico_add_extra_outputs(SONDA_MOTOR)