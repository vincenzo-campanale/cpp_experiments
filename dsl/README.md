Example of domain-specific language (DSL) that uses template metaprogramming.

It allows to create something like this:

With(car)
(
    model = "Fiat 500",
    color = "red"
)

to update the fields of a car object.
