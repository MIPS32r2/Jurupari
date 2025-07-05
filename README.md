![Jurupari_Icon](https://github.com/user-attachments/assets/fcbfeaa4-6857-48d8-ba32-b0f4e0804f00)

# Jurupari

### Wordlist Generator

Jurupari é um gerador avançado de wordlists escrito em **C++**, projetado para pentesters, red teamers e pesquisadores de segurança que precisam de listas otimizadas para cracking de senhas e testes de segurança.

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
git clone https://github.com/MIPS32r2/Jurupari.git
cd Jurupari
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
| Parâmetro               | Descrição                                                          | Valor padrão       |
| ----------------------- | ------------------------------------------------------------------ | ------------------ |
| `--stopwords <arquivo>` | Caminho para o arquivo de stopwords a serem removidas da wordlist. | `stopwords.txt`    |
| `--max-freq <n>`        | Define a frequência máxima de ocorrência de palavras (filtragem).  | `10`               |
| `--min-len <n>`         | Tamanho mínimo das palavras incluídas.                             | `4`                |
| `--max-len <n>`         | Tamanho máximo das palavras incluídas.                             | `32`               |
| `--no-leet`             | Desativa substituições *leet* (ex: `a`→`@`, `e`→`3`).              | Ativado por padrão |
| `--no-reverse`          | Desativa uso de palavras invertidas.                               | Ativado por padrão |
| `--no-caps`             | Desativa variações de capitalização (ex: `Palavra`, `PALAVRA`).    | Ativado por padrão |
| `--output <arquivo>`    | Define o nome do arquivo de saída da wordlist gerada.              | `wordlist.txt`     |

## **✅ Exemplos práticos**

### **Gerar wordlist de URLs desativando leet e definindo arquivo de saída**
```bash
./jurupari --from-url https://exemplo.com --no-leet --output minha_wordlist.txt
```
### **Gerar wordlist de arquivos locais com tamanho mínimo e máximo personalizado**
```bash
./jurupari --from-file dados.txt --min-len 6 --max-len 20
```
### **Usar stopwords customizadas e desativar inversão**
```bash
./jurupari --from-file lista.txt --stopwords minhas_stopwords.txt --no-reverse
```
