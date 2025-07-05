#include "main.hpp"
#include "config.hpp"
#include "frequencia_combinacoes.hpp"
#include "mutator.hpp"
#include "transformations.hpp"
#include "utils.hpp"
#include <algorithm>
#include <exception>
#include <future>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int run(const Config &cfg) {
  auto stopwords = load_stopwords(cfg.stopwords_file);
  std::unordered_map<std::string, size_t> words_per_input;
  size_t total_extracted = 0, total_filtered = 0, total_combinations = 0,
         total_final = 0;

  std::vector<std::string> base_words;

  // Lambda para processar cada input
  auto process_input =
      [&](const std::string &input) -> std::vector<std::string> {
    if (cfg.mode == "--from-url") {
      return extract_alpha_words_from_text(
          fetch_url_with_cache(input, cfg.cache_dir));
    }

    std::string ext = get_file_extension(input);
    if (ext == "json")
      return read_words_from_json(input);
    if (ext == "csv")
      return read_words_from_csv(input);
    return read_words_from_file(input);
  };

  // Processamento paralelo
  std::vector<std::future<std::vector<std::string>>> futures;
  for (const auto &input : cfg.inputs) {
    futures.emplace_back(std::async(std::launch::async, process_input, input));
  }

  for (size_t i = 0; i < futures.size(); ++i) {
    std::vector<std::string> words = futures[i].get();
    words_per_input[cfg.inputs[i]] = words.size();
    total_extracted += words.size();
    base_words.insert(base_words.end(), words.begin(), words.end());
  }

  // Frequência
  auto freq = count_frequencies(base_words);

  // Filtrar palavras
  std::vector<std::string> filtered_words;
  filtered_words.reserve(base_words.size());
  for (const auto &w : base_words) {
    if ((int)w.length() < cfg.min_len || (int)w.length() > cfg.max_len)
      continue;
    if (is_stopword(w, stopwords))
      continue;
    if (freq[w] > cfg.max_frequency)
      continue;
    filtered_words.push_back(w);
  }
  total_filtered = filtered_words.size();

  // Combinações simples
  auto combos = generate_combinations(filtered_words, cfg.max_len);
  total_combinations = combos.size();

  // Transformações e variantes
  std::unordered_set<std::string> wordset;
  for (const auto &w : filtered_words) {
    auto variants = transform_word(w, cfg);
    for (const auto &v : variants) {
      if ((int)v.length() >= cfg.min_len && (int)v.length() <= cfg.max_len) {
        wordset.insert(v);
      }
    }
  }

  // Adicionar combos (sem transformações por enquanto)
  for (const auto &c : combos) {
    if ((int)c.length() >= cfg.min_len && (int)c.length() <= cfg.max_len) {
      wordset.insert(c);
    }
  }

  total_final = wordset.size();
  wordset = mutate_wordlist(wordset);
  save_wordlist(wordset, cfg.output_file);

  // Relatório
  std::cout << "\n--- Relatório ---\n";
  for (const auto &p : words_per_input)
    std::cout << "Entrada: " << p.first << " | Palavras extraídas: " << p.second
              << "\n";
  std::cout << "Total palavras extraídas: " << total_extracted << "\n";
  std::cout << "Palavras após filtro (stopwords, freq, tamanho): "
            << total_filtered << "\n";
  std::cout << "Combinações geradas: " << total_combinations << "\n";
  std::cout << "Total final de palavras na wordlist: " << total_final << "\n";
  std::cout << "Arquivo salvo em: " << cfg.output_file << "\n";

  return 0;
}

int main(int argc, char *argv[]) {
  try {
    Config cfg = parse_args(argc, argv);
    return run(cfg);
  } catch (const std::exception &ex) {
    std::cerr << "[x] Erro: " << ex.what() << "\n";
    return 1;
  }
}
