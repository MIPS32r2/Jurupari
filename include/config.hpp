#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>

struct Config {
  std::string mode;
  std::vector<std::string> inputs;
  std::string stopwords_file;
  int max_frequency;
  int min_len;
  int max_len;
  bool use_leet;
  bool use_reverse;
  bool use_caps;
  std::string output_file;
  std::string cache_dir;

  Config();
};

Config parse_args(int argc, char *argv[]);

#endif // CONFIG_HPP
