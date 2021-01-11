Implements a generic functor that binds to a computation function (that has no arguments). The functor caches the
function result after its first execution (memoization). Successive calls to the functor return the cached value instead
of calling the binded function again.
