#include "word_counter.hpp"

WordCounter::WordCounter(std::vector<std::string>& files)
        : files_{files}
        , tasks_{}
        , res_counter_{} {}

WordCounter::~WordCounter() {}

void WordCounter::Start(const size_t topk) {
    for (const auto& file : files_) {
        std::cout << "Procc file " << file << std::endl;
        std::ifstream input{file};
        if (!input.is_open()) {
            std::cerr << "Failed to open file " << file << '\n';
        } else {
            tasks_.emplace_back(std::async(
                std::launch::async,
                [this](std::istream&& stream) -> Counter {
                    return CountWords(std::move(stream));
                },
                std::move(input)));
        }
    }
    bool success = true;
    for (size_t i = 0; i < tasks_.size(); ++i) {
        try {
            Counter count = tasks_[i].get();
            MergeCounters(res_counter_, count);
        } catch (std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            success = false;
        }
    }
    if (success) {
        PrintTopk(std::cout, res_counter_, topk);
    }
}

Counter WordCounter::CountWords(std::istream&& stream) {
    Counter counter;

    std::for_each(std::istream_iterator<std::string>(stream),
                  std::istream_iterator<std::string>(),
                  [&counter](const std::string& s) { ++counter[Tolower(s)]; });

    return counter;
}

void WordCounter::PrintTopk(std::ostream& stream, const Counter& counter,
                            const size_t k) {
    std::vector<Counter::const_iterator> words;
    words.reserve(std::min(counter.size(), k));
    for (auto it = std::cbegin(counter); it != std::cend(counter); ++it) {
        words.push_back(it);
    }

    std::partial_sort(std::begin(words), std::begin(words) + k, std::end(words),
                      [](auto lhs, auto& rhs) {
                          return lhs->second > rhs->second;
                      });
    std::for_each(std::begin(words), std::begin(words) + k,
                  [&stream](const Counter::const_iterator& pair) {
                      stream << std::setw(4) << pair->second << " "
                             << pair->first << '\n';
                  });
}

void WordCounter::MergeCounters(Counter& dst_counter,
                                const Counter& src_counter) {
    for (auto src_it : src_counter) {
        dst_counter[src_it.first] += src_it.second;
    }
}