#include "paralelo.hpp"
#include "utils.hpp"
#include <mutex>
#include <thread>

std::mutex mutex_global;

void processar_lote(const std::vector<std::string> &entradas, int inicio,
                    int fim, std::unordered_set<std::string> &resultado) {
  std::unordered_set<std::string> local;
  for (int i = inicio; i < fim; ++i) {
    auto palavras = extract_alpha_words_from_text(entradas[i]);
    local.insert(palavras.begin(), palavras.end());
  }

  std::lock_guard<std::mutex> lock(mutex_global);
  resultado.insert(local.begin(), local.end());
}

std::unordered_set<std::string>
processar_em_paralelo(const std::vector<std::string> &textos, int num_threads) {
  std::vector<std::thread> threads;
  std::unordered_set<std::string> resultado;

  int lote = textos.size() / num_threads;
  for (int i = 0; i < num_threads; ++i) {
    int inicio = i * lote;
    int fim = (i == num_threads - 1) ? textos.size() : (i + 1) * lote;

    threads.emplace_back(processar_lote, std::cref(textos), inicio, fim,
                         std::ref(resultado));
  }

  for (auto &t : threads)
    t.join();

  return resultado;
}
