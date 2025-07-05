#include "frequencia_combinacoes.hpp"

// Conta a frequência de cada palavra na lista
std::unordered_map<std::string, int>
count_frequencies(const std::vector<std::string> &words) {
  std::unordered_map<std::string, int> freq;
  for (const auto &w : words)
    freq[w]++;
  return freq;
}

// Gera combinações simples (concatenação de pares de palavras)
std::vector<std::string>
generate_combinations(const std::vector<std::string> &words, int max_len) {
  std::vector<std::string> combos;
  const size_t size = words.size();
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = i + 1; j < size; ++j) {
      std::string combo = words[i] + words[j];
      if ((int)combo.length() <= max_len)
        combos.push_back(combo);
    }
  }
  return combos;
}
