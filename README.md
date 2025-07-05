# Jurupari

### Wordlist Generator & OSINT Enrichment Tool

Jurupari é um gerador avançado de wordlists escrito em **C++**, projetado para pentesters, red teamers e pesquisadores de segurança que precisam de listas otimizadas, personalizadas e enriquecidas com informações públicas (OSINT) para cracking de senhas e testes de segurança.

---

## 🚀 **Funcionalidades**

✅ Geração de wordlists a partir de:

- Palavras básicas com mutações (leet substitutions)
- URLs e scraping HTML simples
- Arquivos locais (txt, csv, json)
- Filtros de stopwords

✅ Mutações inteligentes:

- Substituições (`a` → `@`, `e` → `3`, `s` → `$`, etc)
- Prefixos, sufixos e combinações
- Combinação recursiva controlada

✅ Análise de frequência:

- Contagem de palavras, bigramas, trigramas
- Ordenação por frequência

✅ Cache de downloads para scraping

✅ Compatibilidade com Hashcat e John the Ripper

---

## 🔧 **Como instalar**

Requisitos:

- CMake >= 3.10
- g++ >= 10
- libcurl
- OpenSSL
- nlohmann/json

```bash
git clone https://github.com/seuusuario/jurupari.git
cd jurupari
mkdir build && cd build
cmake ..
make```bash

---

## ⚙️ **Instalação**

### **Pré-requisitos**

- g++ >= 10
- cmake >= 3.10
- libcurl
- OpenSSL
- nlohmann/json

### **Linux (Ubuntu/Debian)**

```bash
sudo apt update
sudo apt install g++ cmake libcurl4-openssl-dev libssl-dev

