Lazy string concatenation with expression template.

Expression templates allow to generate expression definitions instead of calculating the value of an expression. In case
of string concatenation it allows to avoid multiple memory allocations for each concatenation step. Buffer allocation
happens just once, when the final result needs to be generated.
