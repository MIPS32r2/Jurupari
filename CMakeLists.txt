cmake_minimum_required(VERSION 3.15)

# Nome do projeto, versão e linguagem
project(
  Jurupari
  VERSION 2.0
  LANGUAGES CXX)

# Usar padrão C++17 moderno
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Procurar bibliotecas do sistema
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

# Incluir diretórios (inclui json.hpp em include/nlohmann)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Lista de arquivos fonte
set(SOURCES
    src/main.cpp
    src/config.cpp
    src/utils.cpp
    src/frequencia_combinacoes.cpp
    src/transformations.cpp
    src/mutator.cpp
    src/paralelo.cpp)

# Define o executável que será gerado
add_executable(jurupari ${SOURCES})

# Linkar as bibliotecas externas necessárias
target_link_libraries(jurupari OpenSSL::SSL OpenSSL::Crypto CURL::libcurl)

# Mensagens para debug
message(STATUS "Projeto: ${PROJECT_NAME}")
message(STATUS "Fontes: ${SOURCES}")
message(STATUS "Include dirs: ${PROJECT_SOURCE_DIR}/include"
