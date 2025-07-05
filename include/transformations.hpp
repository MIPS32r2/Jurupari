#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP

#include "config.hpp"
#include <set>
#include <string>
#include <vector>

void generate_leetspeak_variants(const std::string &word, size_t index,
                                 std::string current,
                                 std::set<std::string> &output,
                                 const Config &cfg);

std::vector<std::string> transform_word(const std::string &word,
                                        const Config &cfg);

#endif // TRANSFORMATIONS_HPP
