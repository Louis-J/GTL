#include <iostream>
#include <PatMat.hpp>
#include <Switch.hpp>
#include <boost/timer.hpp>
//#include <mpark/patterns.hpp>

//大循环测试跳转表
#define JUMP_TEST true
#define LOOKUP_TEST false

using namespace std;
using namespace GTL::PatMat;
using namespace GTL::Switch;

int which = 0;
#define DOZEN(func, off)    \
    func(19 + off, 10 + off)\
    func(16 + off, 11 + off)\
    func(13 + off, 12 + off)\
    func(10 + off, 13 + off)\
    func(8  + off, 14 + off)\
    func(6  + off, 15 + off)\
    func(4  + off, 16 + off)\
    func(3  + off, 17 + off)\
    func(2  + off, 18 + off)\
    func(1  + off, 19 + off)\

#define STEP 20

#define CYCLE (STEP * 5)

#if (LOOKUP_TEST == false)
#define MANY(func)          \
    DOZEN(func, (0 * STEP)) \
    DOZEN(func, (1 * STEP)) \
    DOZEN(func, (2 * STEP)) \
    DOZEN(func, (3 * STEP)) \
    DOZEN(func, (4 * STEP)) \

#else
#define MANY(func)          \
    DOZEN(func, (0 * STEP)) \
    DOZEN(func, (1 * STEP)) \
    DOZEN(func, (2 * STEP)) \
    DOZEN(func, (3 * STEP)) \
    DOZEN(func, (4 * STEP)) \
    func(10000, 10000)      \

#endif

template <std::size_t _N>
const uint64_t matchCore(int n);

template <std::size_t _N>
const tuple<uint64_t, uint32_t> matchAdd(const size_t circle, int n) {
    const auto timeBegin = boost::timer();
    uint64_t ret = 0;
    for (size_t i = 0; i < circle; ++i)
        ret += matchCore<_N>(n);
    const uint32_t timeBy = timeBegin.elapsed() * 1000;
    return tuple(ret, timeBy);
}

template <std::size_t... Is>
constexpr auto make_funcs(std::index_sequence<Is...>) {
    return std::array{matchAdd<Is>...};
}

#define MPARK_PATTERN(key, value) pattern(key) = [](){return value;},
template <>
const uint64_t matchCore<0>(int n) {
    which++;
    // using namespace mpark::patterns;
    // return match(n)(
    //     DOZEN(MPARK_PATTERN, 00)
    //     DOZEN(MPARK_PATTERN, 20)
    //     DOZEN(MPARK_PATTERN, 40)
    //     DOZEN(MPARK_PATTERN, 60)
    //     DOZEN(MPARK_PATTERN, 80)
    //     pattern(_) = [](){return 0 ;}
    // );
    return 0;
};

#define IF_RETURN(key, value) if(n == key) return value;
template <>
const uint64_t matchCore<1>(int n) {
    which++;
#if (JUMP_TEST == true)
    return 0;
#endif
    MANY(IF_RETURN)
    return 0 ;
};

#define IF_ELSE_RETURN(key, value) else if(n == key) return value;
template <>
const uint64_t matchCore<2>(int n) {
    which++;
#if (JUMP_TEST == true)
    return 0;
#endif
    if (false)      return  0;
    MANY(IF_ELSE_RETURN)
    else            return  0;
};

#define CONDITION(key, value) (n == key) ? value:
template <>
const uint64_t matchCore<3>(int n) {
    which++;
#if (JUMP_TEST == true)
    return 0;
#endif
    return  MANY(CONDITION)
            0 ;
};

#define LOCALE_SWITCH(key, value) case key: return value;
template <>
const uint64_t matchCore<4>(int n) {
    which++;
    switch(n) {
        MANY(LOCALE_SWITCH)
        default: return 0 ;
    };
};

#define ARRAY_SUBSCRIPT(key, value) pat[key] = value;
template <>
const uint64_t matchCore<5>(int n) {
    constexpr static auto pat = [](){
        std::array<uint64_t, 20*5> pat{};
        MANY(ARRAY_SUBSCRIPT)
        return pat;
    }();
    which++;
    if(n < 0 || n >= 20*5) {
        return (uint64_t)0;
    }
    return pat[n];
};

#define PREDICTJ_NO_OFF(key, value) pat[key] = value;
template <>
const uint64_t matchCore<6>(int n) {
    which++;
    constexpr static GTL::Switch::detail::PredicJ<20*5, int, uint64_t, 0> predic{
        [](){
            std::array<uint64_t, 20*5> pat{};
            MANY(PREDICTJ_NO_OFF)
            return pat;
        }(),
        0,
    };

    return predic(n);
};

#define PREDICTJ_OFF(key, value) pat[key - 1] = value;
template <>
const uint64_t matchCore<7>(int n) {
    which++;
    constexpr static GTL::Switch::detail::PredicJ<20*5 - 1, int, uint64_t, 1> predic{
        [](){
            std::array<uint64_t, 20*5 - 1> pat{};
            MANY(PREDICTJ_OFF)
            return pat;
        }(),
        0,
    };

    return predic(n);
};

#define GTL_CASE(key, value) Case<int, uint64_t>{key, value},
template <>
const uint64_t matchCore<8>(int n) {
    which++;
    return SwitchJ(n, CASES(
        MANY(GTL_CASE)
    ));
};

template <>
const uint64_t matchCore<9>(int n) {
    which++;
#if (JUMP_TEST == true)
    return 0;
#endif
    constexpr auto arr = std::get<1>(CASES(
        MANY(GTL_CASE)
    )());
    constexpr auto length = arr.size();
    constexpr static GTL::Switch::detail::PredicB<length, int, uint64_t> predic{
        arr,
        0,
    };

    return predic(n);
};

template <>
const uint64_t matchCore<10>(int n) {
    which++;
#if (JUMP_TEST == true)
    return 0;
#endif
    return SwitchB(n, CASES(
        MANY(GTL_CASE)
    ));
};

template <>
const uint64_t matchCore<11>(int n) {
    which++;
#if (JUMP_TEST == true)
    return 0;
#endif
    constexpr auto bs2 = GTL::Switch::detail::PredicB2Real(CASES(
        MANY(GTL_CASE)
        ),[](){return (int)0;}
    );
    return bs2(n);
};

template <>
const uint64_t matchCore<12>(int n) {
    which++;
#if (JUMP_TEST == true)
    return 0;
#endif
    return SwitchB2(n, CASES(
        MANY(GTL_CASE)
    ));
};

template <>
const uint64_t matchCore<13>(int n) {
    which++;
#if (JUMP_TEST == true)
    return 0;
#endif
    constexpr auto bs3 = GTL::Switch::detail::PredicB3Real(CASES(
        MANY(GTL_CASE)
        ),[](){return (int)0;}
    );
    return bs3(n);
};

template <>
const uint64_t matchCore<14>(int n) {
    which++;
#if (JUMP_TEST == true)
    return 0;
#endif
    return SwitchB3(n, CASES(
        MANY(GTL_CASE)
    ));
};


constexpr auto MATCH_FUNC_NUM = 15;


int main(int argc,char* argv[])
{
    size_t CIRCLE;
    if (argc == 2 && std::atoi(argv[1]) > 0) {
        CIRCLE = std::atoi(argv[1]);
    } else {
        while(!(cin >> CIRCLE)) {
            cin.clear();
            cin.sync();
        }
    }
    
    auto matchAddi = make_funcs(std::make_index_sequence<MATCH_FUNC_NUM>());
    uint64_t timeAll[MATCH_FUNC_NUM] = {0};

    for (int i = 0; i < CYCLE; i++) {
        cout << "输入:" << i << endl;
        for (size_t j = 0; j < MATCH_FUNC_NUM; j++) {
            const auto [ret, time] = matchAddi[j](CIRCLE, i);
            timeAll[j] += time;
            cout << "    结果" << j << ":" << ret << "    耗时" << j << ":" << time << "ms" << endl;
        }
    }
    for (size_t j = 0; j < MATCH_FUNC_NUM; j++) {
        cout << "总耗时" << j << ":" << timeAll[j] << "ms" << endl;
    }

    return 0;
}