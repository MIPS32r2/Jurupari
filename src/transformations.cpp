#include "transformations.hpp"
#include <algorithm>
#include <cctype>
#include <unordered_map>

// Recursiva para gerar variantes leetspeak
void generate_leetspeak_variants(const std::string &word, size_t index,
                                 std::string current,
                                 std::set<std::string> &output,
                                 const Config &cfg) {
  if (index == word.size()) {
    output.insert(current);
    return;
  }
  char c = word[index];
  output.insert(current + c);

  if (cfg.use_leet) {
    static const std::unordered_map<char, std::vector<char>> leet_map = {
        {'a', {'@', '4'}}, {'e', {'3'}},      {'i', {'1', '!'}}, {'o', {'0'}},
        {'s', {'$', '5'}}, {'t', {'7', '+'}}, {'l', {'1', '|'}}};
    auto it = leet_map.find(c);
    if (it != leet_map.end()) {
      for (char sub : it->second) {
        generate_leetspeak_variants(word, index + 1, current + sub, output,
                                    cfg);
      }
    } else {
      generate_leetspeak_variants(word, index + 1, current + c, output, cfg);
    }
  } else {
    generate_leetspeak_variants(word, index + 1, current + c, output, cfg);
  }
}

// Aplica as transformações: variantes básicas, caps, leetspeak, reverso
std::vector<std::string> transform_word(const std::string &word,
                                        const Config &cfg) {
  std::set<std::string> result_set;

  // Variantes básicas
  result_set.insert(word);
  result_set.insert(word + "123");
  result_set.insert(word + "2025");
  result_set.insert("123" + word);
  result_set.insert(word + "!");
  result_set.insert(word + word);
  result_set.insert(word + word + word);

  // Caps e upper
  if (cfg.use_caps && !word.empty()) {
    std::string capitalized = word;
    capitalized[0] = std::toupper(static_cast<unsigned char>(capitalized[0]));
    result_set.insert(capitalized);

    std::string upper = word;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    result_set.insert(upper);
  }

  // Leetspeak e variantes
  if (cfg.use_leet) {
    generate_leetspeak_variants(word, 0, "", result_set, cfg);
  }

  // Reverse
  if (cfg.use_reverse) {
    std::string reversed = word;
    std::reverse(reversed.begin(), reversed.end());
    result_set.insert(reversed);
  }

  return std::vector<std::string>(result_set.begin(), result_set.end());
}
