#pragma once

namespace imbo5
{
    template<class T, unsigned int N>
    class Point
    {
    public:
        Point(T x1, T x2)
        {
            static_assert(N == 2, "Constructor available only in two dimensions");
            _data[0] = x1;
            _data[1] = x2;
        }

        Point(T x1, T x2, T x3)
        {
            static_assert(N == 3, "Constructor available only in three dimensions");
            _data[0] = x1;
            _data[1] = x2;
            _data[2] = x3;
        }

        template<unsigned int M>
        const T& at() const
        {
            static_assert(M < N, "Invalid dimension specified");
            return _data[M];
        }

        template<unsigned int M>
        T& at()
        {
            return const_cast<T&>(static_cast<const Point<T, N>&>(*this).at<M>());
        }

    private:
        T _data[N];
    };
}
