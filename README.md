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
```
---

### **Passo a passo de Instalação**

```bash
git clone https://github.com/seuusuario/jurupari.git
cd jurupari
mkdir build && cd build
cmake ..
make
```
## **Uso Básico**

### **Gerar wordlist de arquivo local**
```bash
./jurupari --from-file lista.txt
```
### **Gerar wordlist de URL com cache**
```bash
./jurupari --from-url https://exemplo.com
```
### **Parâmetros adicionais**

| Parâmetro                   | Descrição                                                                     | Exemplo de uso              |
| --------------------------- | ----------------------------------------------------------------------------- | --------------------------- |
| `--min-length <n>`          | Define o tamanho **mínimo** das palavras a serem incluídas na wordlist.       | `--min-length 5`            |
| `--max-length <n>`          | Define o tamanho **máximo** das palavras a serem incluídas na wordlist.       | `--max-length 12`           |
| `--stopwords stopwords.txt` | Remove as **stopwords** definidas no arquivo especificado do resultado final. | `--stopwords stopwords.txt` |

## **✅ Exemplo combinado de uso:**
```bash
./jurupari --from-file input.txt --min-length 5 --max-length 12 --stopwords stopwords.txt``
