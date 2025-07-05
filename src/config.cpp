#include "config.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

// Construtor com valores padrão
Config::Config()
    : stopwords_file("stopwords.txt"), max_frequency(10), min_len(4),
      max_len(32), use_leet(true), use_reverse(true), use_caps(true),
      output_file("wordlist.txt"), cache_dir("./cache") {}

// Função auxiliar
static bool starts_with(const std::string &s, const std::string &prefix) {
  return s.compare(0, prefix.size(), prefix) == 0;
}

Config parse_args(int argc, char *argv[]) {
  Config cfg;
  if (argc < 3)
    throw std::runtime_error("Uso mínimo: ./jurupari --from-url|--from-file "
                             "<inputs...> [opções]");

  cfg.mode = argv[1];
  if (cfg.mode != "--from-url" && cfg.mode != "--from-file")
    throw std::runtime_error("Modo inválido. Use --from-url ou --from-file");

  int i = 2;
  for (; i < argc; ++i) {
    std::string arg = argv[i];
    if (starts_with(arg, "--"))
      break;
    cfg.inputs.push_back(arg);
  }

  for (; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--stopwords") {
      if (i + 1 >= argc)
        throw std::runtime_error("--stopwords requer um arquivo");
      cfg.stopwords_file = argv[++i];
    } else if (arg == "--max-freq") {
      if (i + 1 >= argc)
        throw std::runtime_error("--max-freq requer um número");
      cfg.max_frequency = std::stoi(argv[++i]);
    } else if (arg == "--min-len") {
      if (i + 1 >= argc)
        throw std::runtime_error("--min-len requer um número");
      cfg.min_len = std::stoi(argv[++i]);
    } else if (arg == "--max-len") {
      if (i + 1 >= argc)
        throw std::runtime_error("--max-len requer um número");
      cfg.max_len = std::stoi(argv[++i]);
    } else if (arg == "--no-leet") {
      cfg.use_leet = false;
    } else if (arg == "--no-reverse") {
      cfg.use_reverse = false;
    } else if (arg == "--no-caps") {
      cfg.use_caps = false;
    } else if (arg == "--output") {
      if (i + 1 >= argc)
        throw std::runtime_error("--output requer nome de arquivo");
      cfg.output_file = argv[++i];
    } else {
      throw std::runtime_error("Opção desconhecida: " + arg);
    }
  }

  if (cfg.inputs.empty())
    throw std::runtime_error("Nenhuma entrada especificada (URLs ou arquivos)");

  return cfg;
}
