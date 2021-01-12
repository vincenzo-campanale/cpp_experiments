int Func(int a, double b)
{
    int result{0};

    for (auto i{0}; i < a; ++i)
    {
        result += (i * b);
    }

    return result;
}
