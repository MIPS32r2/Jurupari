#include "utils.hpp"
#include <algorithm>
#include <cctype>
#include <curl/curl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <sstream>
#include <sys/stat.h>

// --- SHA256 ---
std::string sha256(const std::string &str) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256(reinterpret_cast<const unsigned char *>(str.c_str()), str.size(),
         hash);
  std::stringstream ss;
  for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
  return ss.str();
}

// --- Cache validity ---
bool is_cache_valid(const std::string &filepath, int max_age_seconds) {
  struct stat st;
  if (stat(filepath.c_str(), &st) != 0)
    return false;
  time_t now = time(nullptr);
  double diff = difftime(now, st.st_mtime);
  return diff <= max_age_seconds;
}

// --- CURL write callback ---
size_t write_callback(void *contents, size_t size, size_t nmemb,
                      std::string *output) {
  size_t totalSize = size * nmemb;
  output->append(reinterpret_cast<char *>(contents), totalSize);
  return totalSize;
}

// --- Fetch URL ---
std::string fetch_url(const std::string &url) {
  CURL *curl = curl_easy_init();
  std::string buffer;
  if (!curl) {
    throw std::runtime_error("Erro ao inicializar CURL");
  }
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    curl_easy_cleanup(curl);
    throw std::runtime_error(std::string("Erro CURL: ") +
                             curl_easy_strerror(res));
  }
  curl_easy_cleanup(curl);
  return buffer;
}

// --- Fetch with cache ---
std::string fetch_url_with_cache(const std::string &url,
                                 const std::string &cache_dir) {
  std::string hash_name = sha256(url);
  std::string cache_file = cache_dir + "/" + hash_name + ".cache";

  if (!std::filesystem::exists(cache_dir)) {
    std::filesystem::create_directories(cache_dir);
  }

  if (std::filesystem::exists(cache_file) && is_cache_valid(cache_file)) {
    std::ifstream infile(cache_file);
    std::string content((std::istreambuf_iterator<char>(infile)),
                        std::istreambuf_iterator<char>());
    std::cout << "[*] Usando cache para " << url << std::endl;
    return content;
  }

  std::string html = fetch_url(url);
  std::ofstream outfile(cache_file);
  outfile << html;
  outfile.close();
  std::cout << "[*] Conteúdo salvo no cache: " << cache_file << std::endl;
  return html;
}

// --- Extract words ---
std::vector<std::string>
extract_alpha_words_from_text(const std::string &text) {
  std::vector<std::string> words;
  std::string word;
  for (char c : text) {
    if (std::isalpha(static_cast<unsigned char>(c))) {
      word += std::tolower(static_cast<unsigned char>(c));
    } else if (!word.empty()) {
      if (word.length() >= 4)
        words.push_back(word);
      word.clear();
    }
  }
  if (!word.empty() && word.length() >= 4)
    words.push_back(word);
  return words;
}

// --- Load stopwords ---
std::unordered_set<std::string> load_stopwords(const std::string &filename) {
  std::unordered_set<std::string> stopwords;
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    std::cerr << "[!] Aviso: arquivo de stopwords '" << filename
              << "' não encontrado. Ignorando stopwords.\n";
    return stopwords;
  }
  std::string line;
  while (std::getline(infile, line)) {
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    stopwords.insert(line);
  }
  return stopwords;
}

bool is_stopword(const std::string &word,
                 const std::unordered_set<std::string> &stopwords) {
  return stopwords.find(word) != stopwords.end();
}

// --- Read words from file ---
std::vector<std::string> read_words_from_file(const std::string &filename) {
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    throw std::runtime_error("Não foi possível abrir o arquivo: " + filename);
  }
  std::string content((std::istreambuf_iterator<char>(infile)),
                      std::istreambuf_iterator<char>());
  return extract_alpha_words_from_text(content);
}

// --- Read words from JSON ---
void extract_strings_from_json(const nlohmann::json &j,
                               std::vector<std::string> &words) {
  if (j.is_string()) {
    auto w = extract_alpha_words_from_text(j.get<std::string>());
    words.insert(words.end(), w.begin(), w.end());
  } else if (j.is_array()) {
    for (auto &el : j)
      extract_strings_from_json(el, words);
  } else if (j.is_object()) {
    for (auto it = j.begin(); it != j.end(); ++it)
      extract_strings_from_json(it.value(), words);
  }
}

std::vector<std::string> read_words_from_json(const std::string &filename) {
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    throw std::runtime_error("Não foi possível abrir o arquivo JSON: " +
                             filename);
  }
  nlohmann::json j;
  infile >> j;
  std::vector<std::string> words;
  extract_strings_from_json(j, words);
  return words;
}

// --- Read words from CSV ---
std::vector<std::string> read_words_from_csv(const std::string &filename) {
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    throw std::runtime_error("Não foi possível abrir o arquivo CSV: " +
                             filename);
  }
  std::string line;
  std::vector<std::string> words;
  while (std::getline(infile, line)) {
    std::stringstream ss(line);
    std::string cell;
    while (std::getline(ss, cell, ',')) {
      auto w = extract_alpha_words_from_text(cell);
      words.insert(words.end(), w.begin(), w.end());
    }
  }
  return words;
}

// --- Get file extension ---
std::string get_file_extension(const std::string &filename) {
  auto pos = filename.rfind('.');
  if (pos == std::string::npos)
    return "";
  std::string ext = filename.substr(pos + 1);
  std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
  return ext;
}
// --- Save wordlist to file ---
void save_wordlist(const std::unordered_set<std::string> &wordset,
                   const std::string &filename) {
  std::ofstream outfile(filename);
  if (!outfile.is_open()) {
    throw std::runtime_error("Não foi possível salvar a wordlist em: " +
                             filename);
  }

  for (const auto &word : wordset) {
    outfile << word << "\n";
  }

  std::cout << "[*] Wordlist salva em: " << filename << std::endl;
}
void gerar_script_cracking(const std::string &wordlist_path,
                           const std::string &hashfile_path,
                           const std::string &output_script_path) {
  std::ofstream script(output_script_path);
  if (!script.is_open()) {
    throw std::runtime_error("Não foi possível gerar script: " +
                             output_script_path);
  }

  script << "#!/bin/bash\n\n";
  script << "echo \"[*] Executando Hashcat...\"\n";
  script << "hashcat -m 0 -a 0 -o cracked_hashes.txt " << hashfile_path << " "
         << wordlist_path << "\n\n";

  script << "echo \"[*] Executando John the Ripper...\"\n";
  script << "john --wordlist=" << wordlist_path << " --format=raw-md5 "
         << hashfile_path << "\n";

  script.close();

  // Torna o script executável
  chmod(output_script_path.c_str(), 0755);

  std::cout << "[*] Script de cracking salvo em: " << output_script_path
            << std::endl;
}
