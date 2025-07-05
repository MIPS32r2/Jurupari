#ifndef FREQUENCY_COMBINATIONS_HPP
#define FREQUENCY_COMBINATIONS_HPP

#include <string>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, int>
count_frequencies(const std::vector<std::string> &words);

std::vector<std::string>
generate_combinations(const std::vector<std::string> &words, int max_len);

#endif // FREQUENCY_COMBINATIONS_HPP
