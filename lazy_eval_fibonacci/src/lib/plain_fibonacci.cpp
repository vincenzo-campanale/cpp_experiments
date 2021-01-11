//
// Note: this implementation is inefficient. In the common case there are two recursive calls, and each performs
//       duplicate work because both Fibonacci(n) and Fibonacci(n - 1) need to calculate Fibonacci(n - 2). Both
//       Fibonacci(n - 1) and Fibonacci(n - 2) need to calculate Fibonacci(n - 3), and so on. The number of recursive
//       calls grows exponentially with the number n.
//
unsigned int PlainFibonacci(unsigned int num)
{
   if (num < 2)
   {
      return num;
   }
   else
   {
      return PlainFibonacci(num - 1) + PlainFibonacci(num - 2);
   }
}
