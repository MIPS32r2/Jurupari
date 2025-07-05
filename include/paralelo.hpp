#ifndef PARALLEL_HPP
#define PARALLEL_HPP

#include <string>
#include <unordered_set>
#include <vector>

std::unordered_set<std::string>
processar_em_paralelo(const std::vector<std::string> &textos,
                      int num_threads = 4);

#endif
