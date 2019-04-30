#ifndef GTL_SWITCH_SWITCHCOMMON_HPP
#define GTL_SWITCH_SWITCHCOMMON_HPP

#include <array>
#include <cstddef>
#include <type_traits>
#include <ostream>
#include <tuple>

namespace GTL::Switch {
// #define STRINGIFY(x) #x 
// #define TOSTRING(x) STRINGIFY(x) 
// #define AT __FILE__ ":" TOSTRING(__LINE__) 
// #define CONVALS(...) []{return std::tuple{__VA_ARGS__};}
// #define CONVAL(VAL)  [=]{return (VAL);}
// #define CONARRY(...) [=]{return std::array{__VA_ARGS__};}


#define CASES(...) []{                                        \
    auto arr = std::array{__VA_ARGS__};                       \
    bool repeatKey = CaseSort(std::begin(arr), std::end(arr));\
    return std::tuple{repeatKey, arr};                        \
}

#define DEFAULT(...) []{                                      \
    return Default __VA_ARGS__;                               \
}


template <typename TypeA, typename TypeB>
constexpr bool TypeEqual(TypeA a, TypeB b) {return false;};
template <typename Type>
constexpr bool TypeEqual(Type a, Type b) {return true;};


template <class TypeC, class TypeD>
struct Case {
    TypeC key;
    TypeD deal;

    friend constexpr bool operator< (Case<TypeC, TypeD> lhs, Case<TypeC, TypeD> rhs)      {return lhs.key < rhs.key;}
    friend constexpr bool operator< (Case<TypeC, TypeD> &lhs, Case<TypeC, TypeD> &rhs)    {return lhs.key < rhs.key;}
    friend constexpr bool operator< (Case<TypeC, TypeD> &&lhs, Case<TypeC, TypeD> &&rhs)  {return lhs.key < rhs.key;}

    friend constexpr bool operator!= (Case<TypeC, TypeD> &lhs, Case<TypeC, TypeD> &rhs)   {return lhs.key != rhs.key;}
    friend constexpr bool operator!= (Case<TypeC, TypeD> &&lhs, Case<TypeC, TypeD> &&rhs) {return lhs.key != rhs.key;}


    template <typename TypeV>
    friend bool operator< (Case<TypeC, TypeD> c, TypeV key)      {return c.key < key;}
    template <typename TypeV>
    friend bool operator< (Case<TypeC, TypeD> &c, TypeV &key)    {return c.key < key;}
    template <typename TypeV>
    friend bool operator< (Case<TypeC, TypeD> &&c, TypeV &&key)  {return c.key < key;}

    template <typename TypeV>
    friend bool operator!= (Case<TypeC, TypeD> &c, TypeV &key)   {return c.key != key;}
    template <typename TypeV>
    friend bool operator!= (Case<TypeC, TypeD> &&c, TypeV &&key) {return c.key != key;}

    //����sort
    friend constexpr void iter_swap(Case<TypeC, TypeD> &lhs, Case<TypeC, TypeD> &rhs) {
        Case<TypeC, TypeD> tmp = std::move(lhs);
        lhs = std::move(rhs);
        rhs = std::move(tmp);
    }
    //����constexpr sort
    friend constexpr void iter_swap(Case<TypeC, TypeD> &&lhs, Case<TypeC, TypeD> &&rhs) {
        Case<TypeC, TypeD> tmp = std::move(lhs);
        lhs = std::move(rhs);
        rhs = std::move(tmp);
    }

    friend std::ostream& operator<<(std::ostream& ostr, Case<TypeC, TypeD> c) {
        return ostr << c.key << ":" << c.deal << std::endl;
    }
    friend std::ostream& operator<<(std::ostream& ostr, Case<TypeC, TypeD> &c) {
        return ostr << c.key << ":" << c.deal << std::endl;
    }
    //friend std::ostream& operator<<(std::ostream& ostr, Case<TypeC, TypeD> &&c) {
    //    return ostr << c.key << ":" << c.deal << std::endl;
    //}

    template <std::size_t N>
    friend std::ostream& operator<<(std::ostream& ostr, std::array<Case<TypeC, TypeD>, N> a) {
        for(auto &c :a)
            ostr << c.key << ":" << c.deal << std::endl;
        return ostr;
    }
    template <std::size_t N>
    friend std::ostream& operator<<(std::ostream& ostr, std::array<Case<TypeC, TypeD>, N> &a) {
        for(auto &c :a)
            ostr << c.key << ":" << c.deal << std::endl;
        return ostr;
    }
};

template <class TypeD>
struct Default {
   TypeD deal;
};


template <typename RAIt>
constexpr RAIt next(RAIt it, typename std::iterator_traits<RAIt>::difference_type n = 1)
{
    return it + n;
}

template <typename RAIt>
constexpr auto distance(RAIt first, RAIt last)
{
    return last - first;
}

template<class ForwardIt1, class ForwardIt2>
constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b)
{
    auto temp = std::move(*a);
    *a = std::move(*b);
    *b = std::move(temp);
}

template<class InputIt, class UnaryPredicate>
constexpr InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate q)
{
    for (; first != last; ++first) {
        if (!q(*first)) {
            return first;
        }
    }
    return last;
}

template<class ForwardIt, class UnaryPredicate>
constexpr ForwardIt partition(ForwardIt first, ForwardIt last, UnaryPredicate p)
{
    first = find_if_not(first, last, p);
    if (first == last) return first;

    for(ForwardIt i = next(first); i != last; ++i){
        if(p(*i)){
            iter_swap(i, first);
            ++first;
        }
    }
    return first;
}


template<class RAIt>
constexpr bool CaseSort(RAIt first, RAIt last)
{
    auto const N = distance(first, last);
    if (N <= 1)
        return false;
    auto const pivot = *next(first, N / 2);
    auto const middle1 = partition(first, last, [=](auto const& elem){
        return std::less{}(elem, pivot);
    });
    auto const middle2 = partition(middle1, last, [=](auto const& elem){
        return !std::less{}(pivot, elem);
    });
    bool repeatKey = !((middle2 - 1) == middle1 || *middle1 != *(middle2 - 1));
    repeatKey |= CaseSort(first, middle1);
    repeatKey |= CaseSort(middle2, last);
    return repeatKey;
}


} // namespace GTL::Switch

#endif // GTL_SWITCH_SWITCHCOMMON_HPP
