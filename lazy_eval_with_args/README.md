Creates a memoized wrapper from a function pointer.

The general structure for caching, when it is not possible to predict what arguments the function will be invoked with,
is a map. Any pure function is a mapping from its arguments to its value, so the cache will be able to cover any pure
function without problems.