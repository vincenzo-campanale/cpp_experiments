
int SampleFunction()
{
   auto result{0UL};

   for (auto i{0UL}; i < 100000UL; ++i)
   {
      result += i;
   }

   return result;
}
