#ifndef UTILS_HPP
#define UTILS_HPP

#include "config.hpp"
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

// SHA256
std::string sha256(const std::string &str);

// Cache
bool is_cache_valid(const std::string &filepath, int max_age_seconds = 86400);

// Download com cache
std::string fetch_url(const std::string &url);
std::string fetch_url_with_cache(const std::string &url,
                                 const std::string &cache_dir);

// Extração e manipulação de palavras
std::vector<std::string> extract_alpha_words_from_text(const std::string &text);

std::unordered_set<std::string> load_stopwords(const std::string &filename);
bool is_stopword(const std::string &word,
                 const std::unordered_set<std::string> &stopwords);

std::vector<std::string> read_words_from_file(const std::string &filename);
std::vector<std::string> read_words_from_json(const std::string &filename);
std::vector<std::string> read_words_from_csv(const std::string &filename);

std::string get_file_extension(const std::string &filename);

// Adicione esta linha:
void save_wordlist(const std::unordered_set<std::string> &wordset,
                   const std::string &filename);

// Geração de script de ataque com Hashcat e John the Ripper
void gerar_script_cracking(const std::string &wordlist_path,
                           const std::string &hashfile_path,
                           const std::string &output_script_path);

#endif // UTILS_HPP
