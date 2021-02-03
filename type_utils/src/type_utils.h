#include <iterator>
#include <numeric>
#include <type_traits>

///
/// @brief Utility class for debugging deduced types.
///
template <typename T>
class error;

///
/// @brief Meta-function that checks whether two types are identical.
///
template <typename T1, typename T2>
struct is_same: std::false_type {};

template <typename T>
struct is_same<T, T>: std::true_type {};

///
/// @brief Metafunction that returns the contained type of an iterable collection.
///
// - std::declval is a metafunction that takes any type T and pretends to create an instance of that type so that is can
//   be used in metafunctions when one needs values instead of types, i.e. it is not needed to call the constructor with
//   its specific arguments.
// - iterator dereferencing returns a const reference to the pointed value; remove_cv_t and remove_reference_t remove,
//   respectively, the const qualifier and the reference from the type.
//
template<typename T>
using contained_type_t = std::remove_cv_t<std::remove_reference_t<decltype(*std::begin(std::declval<T>()))>>;

///
/// @brief Meta-function that returns a type with references stripped.
///
template <typename T>
struct remove_reference {
    using type = T;
};

template <typename T>
struct remove_reference<T&> {
    using type = T;
};

template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

///
/// @brief Metafunction that detects whether a type has a nested 'value_type' type definition.
///
template<typename T, typename = std::void_t<>>   //< General case: assumes an arbitrary type doesn't have a nested
struct has_value_type : std::false_type {};      //< value_type type definition.

template<typename C>                                                                 //< Specialized case: considered
struct has_value_type<C, std::void_t<typename C::value_type>> : std::true_type {};   //< only if typename C::value_type
                                                                                     //< is an existing type (if C has
                                                                                     //< a nested value_type)

///
/// @brief Metafunction that detects whether a type is iterable.
///
template<typename T, typename = std::void_t<>>   //< The general case: assumes an arbitrary type isn't iterable.
struct is_iterable : std::false_type {};

template<typename C>
struct is_iterable<C, std::void_t<decltype(*std::begin(std::declval<C>())), decltype(std::end(std::declval<C>()))>>
    : std::true_type {};   //< Specialized case: considered only if C is iterable, and if its begin iterator can be
                           //< dereferenced.

///
/// @brief Meta function that always returns false. Useful with static_assert and if-constexpr.
///
template <typename...>
struct false_: std::false_type {};
