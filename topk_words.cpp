// Read files and prints top k word by frequency

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "word_counter.hpp"

const size_t TOPK = 10;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    }

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::string> files;
    for (int i = 1; i < argc; ++i) {
        files.push_back(argv[i]);
    }
    WordCounter counter(files);
    counter.Start(TOPK);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time is " << elapsed_ms.count() << " us\n";
}
