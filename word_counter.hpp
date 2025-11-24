#pragma once

#include <algorithm>
#include <cctype>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

using Counter = std::map<std::string, std::size_t>;

class WordCounter {
public:
    explicit WordCounter(std::vector<std::string>& files);
    virtual ~WordCounter();
    void Start(const size_t topk);

private:
    std::vector<std::string>& files_;
    std::vector<std::future<Counter>> tasks_;
    Counter res_counter_;

    Counter CountWords(std::istream&& stream);
    void PrintTopk(std::ostream& stream, const Counter& counter, const size_t k);
    void MergeCounters(Counter& dst_counter, const Counter& src_counter);
    static std::string Tolower(const std::string& str) {
        std::string lower_str;
        std::transform(std::cbegin(str), std::cend(str),
                       std::back_inserter(lower_str),
                       [](unsigned char ch) { return std::tolower(ch); });
        return lower_str;
    };
};