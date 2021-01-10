#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

static const unsigned long kNMostFrequentWords{10UL};

// Note: all these functions are pure at their interface. There isn't any mutable state in the main program.
std::string LoadText(const std::string& filename);
std::string ToLowerCase(const std::string& text);
std::string WithoutSpecialCharacters(const std::string& text);
std::unordered_map<std::string, int> CountOccurrences(const std::string& text);
void Print(const std::vector<std::pair<int, std::string>>& pairs);

// Reverses any container of pairs.
//
// Note: for the template argument 'First', it is needed to remove the const qualifier. This is necessary because
//       std::unordered_map stores elements of type std::pair<const Key, T>.
// Note: example of function lifting. We define a lambda that is able to swap a single pair; this function is passed to
//       an higher order function std::transform() that applies the lambda to every item in the input container.
template<typename Pairs,
         typename First = typename std::remove_cv_t<typename Pairs::value_type::first_type>,
         typename Second = typename Pairs::value_type::second_type>
std::vector<std::pair<Second, First>> ReversePairs(const Pairs& pairs)
{
   std::vector<std::pair<Second, First>> swapped_pairs{pairs.size()};

   std::transform(std::begin(pairs), std::end(pairs), std::begin(swapped_pairs), [](const auto& pair){
      return std::make_pair(pair.second, pair.first);
   });

   return swapped_pairs;
}

// Sorts pairs within a container in a pure ways.
template<typename Pairs>
Pairs Sort(const Pairs& pairs)
{
   Pairs sorted_pairs{pairs.size()};
   std::partial_sort_copy(
         std::begin(pairs), std::end(pairs), std::begin(sorted_pairs), std::end(sorted_pairs), std::greater());

   return sorted_pairs;
}

int main()
{
   const auto text_path{std::filesystem::path{__FILE__}.parent_path() / "sample_text.txt"};
   Print(Sort(ReversePairs(CountOccurrences(WithoutSpecialCharacters(ToLowerCase(LoadText(text_path)))))));

   return 0;
}

std::string LoadText(const std::string& filename)
{
    std::ifstream file{filename};
    const std::string text{(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};

   return text;
}

std::string ToLowerCase(const std::string& text)
{
   std::string lower_case_text{};

   std::transform(std::begin(text), std::end(text), std::back_inserter(lower_case_text), [](unsigned char c){
      return std::tolower(c);
   });

   return lower_case_text;
}

std::string WithoutSpecialCharacters(const std::string& text)
{
   std::string text_result{text};

   text_result.erase(std::remove_if(std::begin(text_result), std::end(text_result), [](unsigned char c){
      return !(std::isalpha(c) || std::isspace(c));
   }), std::end(text_result));

   return text_result;
}

std::unordered_map<std::string, int> CountOccurrences(const std::string& text)
{
   std::unordered_map<std::string, int> occurrences{};

   std::istringstream text_stream(text);
   auto start_it{std::istream_iterator<std::string>(text_stream)};
   auto end_it{std::istream_iterator<std::string>()};

   std::for_each(start_it, end_it, [&occurrences](const std::string& word){

      auto it{occurrences.find(word)};
      const auto occurrence_found{it != std::end(occurrences)};

      if (occurrence_found)
      {
         ++(it->second);
      }
      else
      {
         occurrences.insert(std::make_pair(word, 1));
      }
   });

   return occurrences;
}

void Print(const std::vector<std::pair<int, std::string>>& pairs)
{
   for (auto i{0UL}; i < std::min(kNMostFrequentWords, pairs.size()); ++i)
   {
      const auto& pair{pairs.at(i)};
      std::cout << pair.second << " : " << pair.first << std::endl;
   }
}
