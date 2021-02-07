#ifndef DSL_H
#define DSL_H

#include <algorithm>
#include <functional>
#include <type_traits>

///
/// @brief Functor that performs an update of a field within a record.
///
template <typename Member, typename Value>
struct Update
{
    Update(Member member, Value value)
        : member_{member},
          value_{value}
    {}

    template <typename Record>
    bool operator()(Record& record)
    {
        if constexpr (std::is_invocable_r<bool, Member, Record, Value>())  //< member_ is a callable object that returns
        {                                                                  //< a bool whether the assignment was successful.
            return std::invoke(member_, record, value_);
        }
        else if constexpr (std::is_invocable<Member, Record, Value>())     //< member_ is a callable object that returns void.
        {
            std::invoke(member_, record, value_);
            return true;
        }
        else                                                               //< member_ is a member variable.
        {
            std::invoke(member_, record) = value_;
            return true;
        }
    }

    Member member_;
    Value value_;
};

///
/// @brief Stores the pointer to a member variable or a member function. Defines an assignment operator for such field.
///        The assignment is not done immediately, instead an Update object is created that contains the information
///        needed for the assignment (lazy evaluation).
///
template<typename Member>
struct Field
{
    Field(Member member)
        : member_{member}
    {}

    template<typename Value>
    auto operator=(const Value& value) const
    {
        return Update{member_, value};
    }

    Member member_;
};

///
/// @brief Functor that performs a set of updates and changes record only if all the updates are successful
///        (copy-and-swap idiom).
///
template<typename Record>
class Transaction
{
  public:
    Transaction(Record& record)
        : record_{record}
    {}

    template<typename... Updates>
    bool operator()(Updates... updates)
    {
        auto temp{record_};

        if (All(updates(temp)...))
        {
            std::swap(record_, temp);
            return true;
        }

        return false;
    }

  private:
    template<typename... Updates>
    bool All(Updates... results) const
    {
        return (... && results);
    }

    Record& record_;
};

///
/// @brief Makes a transaction for a given record.
///
template<typename Record>
Transaction<Record> With(Record& record)
{
    return Transaction{record};
}

#endif // DSL_H
