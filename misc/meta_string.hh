#pragma once

namespace meta_string{

template<char>using charDummy = char;
template <int N>
constexpr char at(const char* a) { return a[N]; }

template<int... dummy>
struct F
{
    const char c_str[sizeof...(dummy) + 1];

    const size_t length;
    const size_t size;

    constexpr F(const char* a) : c_str{ at<dummy>(a)..., 0 }, length(sizeof...(dummy)), size(sizeof...(dummy) + 1){}
    constexpr F(charDummy<dummy>... a) : c_str{ a..., 0 }, length(sizeof...(dummy)), size(sizeof...(dummy) + 1){}

    constexpr F(const F& a) : c_str{ a.c_str[dummy]..., 0 }, length(a.length), size(a.size){}

    template<int... dummyB>
    constexpr F<dummy..., sizeof...(dummy) + dummyB...> operator + (F<dummyB...> b)const
    {
        return{ this->c_str[dummy]..., b.c_str[dummyB]... };
    }
    operator const char* ()const { return c_str; }
};

template<int I>
struct get_string
{
    constexpr static auto g(const char* a) -> decltype(get_string<I - 1>::g(a) + F<0>(a + I))
    {
        return get_string<I - 1>::g(a) + F<0>(a + I);
    }
};

template<>
struct get_string<0>
{
    constexpr static F<0> g(const char* a)
    {
        return{ a };
    }
};

template<int I>
constexpr auto str(const char(&a)[I]) -> decltype(get_string<I - 2>::g(a))
{
    return get_string<I - 2>::g(a);
}

}