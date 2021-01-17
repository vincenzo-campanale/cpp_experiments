#include <algorithm>
#include <string>

template<typename... Strings>
class LazyStringConcatHelper;

template<typename LastString, typename... Strings>
class LazyStringConcatHelper<LastString, Strings...>   //< Partial template specialization.
{
  private:
    LastString data_;                           //< Stores the copy of the original string.
    LazyStringConcatHelper<Strings...> tail_;   //< Stores the structure that contains other strings.

  public:
    LazyStringConcatHelper(LastString data, LazyStringConcatHelper<Strings...> tail)
      : data_{data},
        tail_{tail}
    {}

    int Size() const   //< Calculates the size of all strings combined.
    {
        return data_.size() + tail_.Size();
    }

    template<typename It>
    void Save(It end) const
    {
        const auto begin{end - data_.size()};                   //< The structure stores strings in reverse order: the
        std::copy(std::begin(data_), std::end(data_), begin);   //< data member variable contains the string that comes
        tail_.Save(begin);                                      //< last, so it needs to go to the end of the buffer.
    }

    operator std::string() const
    {
        std::string result(Size(), '\0');
        Save(std::end(result));
        return result;
    }

    LazyStringConcatHelper<std::string, LastString, Strings...>   //< Creates a new instance of the structure with one
    operator+(const std::string& other) const                     //< string added to it.
    {
        return LazyStringConcatHelper<std::string, LastString, Strings...>(other, *this);
    }
};

template<>
class LazyStringConcatHelper<>   //< Base class to interrupt the recursion.
{
  public:
    LazyStringConcatHelper() {}

    int Size() const
    {
        return 0;
    }

    template<typename It>
    void Save(It) const {}

    LazyStringConcatHelper<std::string>
    operator+(const std::string& other) const
    {
        return LazyStringConcatHelper<std::string>(other, *this);
    }
};
