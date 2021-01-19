#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

#include <range/v3/view.hpp>
#include <range/v3/action.hpp>
#include <range/v3/view/istream.hpp>
#include <range/v3/range/conversion.hpp>

std::string ExtractPlainWord(const std::string& word)
{
    return  ranges::views::all(word)
            | ranges::views::trim(ranges::not_fn(isalpha))
            | ranges::views::transform(tolower)
            | ranges::to<std::string>();
}

int main()
{
    const auto file_path{std::filesystem::path{__FILE__}.parent_path() / "sample_text.txt"};
    std::ifstream file_stream{file_path};

    auto sorted_words{ranges::istream_view<std::string>{file_stream}
                      | ranges::views::transform(ExtractPlainWord)
                      | ranges::views::filter([](const auto& word){ return !word.empty(); })
                      | ranges::to_vector
                      | ranges::actions::sort};

    auto make_group_pair = [](const auto& group){ return std::make_pair(ranges::distance(group), *std::begin(group)); };
    auto most_frequent_words{sorted_words
                             | ranges::views::group_by(std::equal_to<>{})
                             | ranges::views::transform(make_group_pair)
                             | ranges::to<std::multimap>};

    for (const auto& [occurrence, word] : most_frequent_words | ranges::views::reverse | ranges::views::take(10))
    {
        std::cout << occurrence << " - " << word << std::endl;
    }
}
