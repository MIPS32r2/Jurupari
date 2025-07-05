#ifndef MUTATOR_HPP
#define MUTATOR_HPP

#include <string>
#include <unordered_set>
#include <vector>

std::unordered_set<std::string> mutate_word(const std::string &word,
                                            int max_subs = 2);
std::unordered_set<std::string>
mutate_wordlist(const std::unordered_set<std::string> &wordlist);

#endif // MUTATOR_HPP
