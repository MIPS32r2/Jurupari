#include "mutator.hpp"
#include <unordered_map>

static const std::unordered_map<char, std::vector<char>> substitutions = {
    {'a', {'@', '4'}}, {'e', {'3'}}, {'i', {'1', '!'}}, {'o', {'0'}},
    {'s', {'$', '5'}}, {'g', {'9'}}, {'t', {'7'}}};

static const std::vector<std::string> sufixos = {"123", "!", "2024", "@"};
static const std::vector<std::string> prefixos = {"!", "123", "@"};

void recursive_mutate(const std::string &word, int index, int subs_left,
                      std::string current,
                      std::unordered_set<std::string> &output) {
  if (index >= word.length() || subs_left < 0) {
    output.insert(current);
    return;
  }

  char c = word[index];
  current.push_back(c);
  recursive_mutate(word, index + 1, subs_left, current, output);
  current.pop_back();

  auto it = substitutions.find(c);
  if (it != substitutions.end() && subs_left > 0) {
    for (char sub : it->second) {
      current.push_back(sub);
      recursive_mutate(word, index + 1, subs_left - 1, current, output);
      current.pop_back();
    }
  }
}

std::unordered_set<std::string> mutate_word(const std::string &word,
                                            int max_subs) {
  std::unordered_set<std::string> result;
  recursive_mutate(word, 0, max_subs, "", result);

  std::unordered_set<std::string> final_result;

  for (const std::string &base : result) {
    final_result.insert(base);
    for (const auto &suf : sufixos)
      final_result.insert(base + suf);
    for (const auto &pre : prefixos)
      final_result.insert(pre + base);
  }

  return final_result;
}

std::unordered_set<std::string>
mutate_wordlist(const std::unordered_set<std::string> &wordlist) {
  std::unordered_set<std::string> mutated;

  for (const auto &word : wordlist) {
    auto variations = mutate_word(word);
    mutated.insert(variations.begin(), variations.end());
  }

  return mutated;
}
