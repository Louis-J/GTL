#include <iostream>
#include <utility>
#include <Switch.hpp>
#include <boost/timer.hpp>
#include <mpark/patterns.hpp>

using namespace std;
using namespace GTL::Switch;

template <std::size_t N>
void more();

constexpr auto SwitchJ1 = [](auto inp, auto cases) {
    //constexpr auto [repeatKey, arr] = cases();
    static_assert(std::get<0>(cases()) == false, " ");
    //static constexpr auto pridict = 1;
    return;
};

constexpr auto SwitchJ2 = [](auto inp, auto cases) {
    static_assert(std::get<0>(cases()) == false, " ");
    //static constexpr auto pridict = 1;
    return;
};

constexpr auto SwitchJ3 = [](auto inp, auto cases) {
    static_assert(std::get<0>(cases()) == false, "键值有重复！");
    return std::get<1>(cases());
};

constexpr auto makePredicJ4 = [](auto cases){
    constexpr auto arr = std::get<1>(cases());
    using TypeC = decltype(arr[0].key);
    using TypeD = decltype(arr[0].deal);
    constexpr auto _N = arr.size();
    constexpr auto offset = arr[0].key;
    constexpr std::size_t length =  arr[_N-1].key - arr[0].key + 1;

    return GTL::Switch::detail::PredicJ<length, TypeC, TypeD, offset> {
        [&](){
            std::array<TypeC, length> pat{};
            for(auto c : arr) {
                pat[c.key - offset] = c.deal;
            }
            return pat;
        }(),
        TypeD(),
    };
};

inline auto SwitchJ4 = [](auto inp, auto cases) {
    static_assert(std::get<0>(cases()) == false, "键值有重复！");
    static constexpr auto pridict = makePredicJ4(cases);
    return pridict(inp);
};

template <>
void more<0>() {
    int a = 100;
    SwitchJ1(a, CONVALS(
        false, 
        Case<int, int>{1, 1},
    ));
    SwitchJ2(a, CASES(
        Case<int, int>{2, 2},
        Case<int, int>{1, 1},
        //Case<int, int>{1, 3},
    ));
    //constexpr auto cs2 = Cases(array{
    //    Case<int, int>{2, 2},
    //    Case<int, int>{1, 1},
    //    Case<int, int>{4, 4},
    //    Case<int, int>{3, 3},
    //    Case<int, int>{2, 22},
    //});
    //cout << "casearray 1:\n";
    //cout << cs2 << endl;


    //constexpr auto cs3 = Cases2(array{
    //    Case<int, int>{2, 2},
    //    Case<int, int>{1, 1},
    //    Case<int, int>{4, 4},
    //    Case<int, int>{3, 3},
    //    Case<int, int>{2, 22},
    //});
    //auto cs3a = get<1>(cs3);
    //bool cs3i = get<0>(cs3);
    //cout << "casearray 2:";
    //cout << cs3i << endl;
    //for(auto &cs3ac :cs3a)
    //    cout << cs3ac.key << ":" << cs3ac.deal << std::endl;


    cout << "casearray 3:\n";
    cout << SwitchJ3(a, CASES(
        Case<int, int>{2, 2},
        Case<int, int>{1, 1},
        Case<int, int>{4, 4},
        Case<int, int>{3, 3},
        //Case<int, int>{2, 22}, 成功！
    ));

    cout << "casearray 4:\n";
    cout << SwitchJ4(1, CASES(
        Case<int, int>{2, 2},
        Case<int, int>{1, 1},
        Case<int, int>{4, 4},
        Case<int, int>{3, 3},
        //Case<int, int>{2, 22}, 成功！
    )) << endl;


}

template<typename T, std::size_t N>
std::size_t BS (std::array<T, N> &arr, T &target)
{
    std::size_t bot = 0, top = N;

    while (bot < top) {
        std::size_t mid = (bot + top) / 2;
        if (arr[mid] < std::move(target))
            bot = mid + 1;
        else
            top = mid;
    }
    if (bot == N || arr[bot] != target) {
        return -1;
    } else {
        return bot;
    }
}

template <>
void more<1>() {

    auto cs0 = std::array{1, 5, 9, 13, 100};
    cout << "\ncasearray 0:\n";
    for(auto i = 0; i < 10; i++) {
        cout << "find:" << i << "=" << BS(cs0, i) << std::endl;
    }

    constexpr auto cs1 = CASES(
       Case<int, int>{2, 2},
       Case<int, int>{1, 1},
       Case<int, int>{4, 4},
       Case<int, int>{3, 3},
       Case<int, int>{2, 22},
    );

    bool cs1i = get<0>(cs1());
    auto cs1a = get<1>(cs1());
    cout << "\ncasearray 1:";
    cout << cs1i << endl;
    // for(auto &cs1ac :cs1a)
    //     cout << "key:" << cs1ac.key << "=" << cs1ac.deal << std::endl;
    for(auto i = 0; i < 10; i++) {
        auto cs1d = Case<int, int>{i, 0};
        cout << "find" << i << ":" << BS(cs1a, cs1d) << std::endl;
    }
}

template<std::size_t N>
int BS2 (std::array<Case<int, int>, N> &arr, int &target)
{
    std::size_t bot = 0, top = N;

    while (bot < top) {
        cout << "Q";
        std::size_t mid = (bot + top) / 2;
        cout << "Q";
        if (arr[mid].key < target)
            bot = mid + 1;
        else
            top = mid;
    }
    cout << "Q";
    if (bot == N) {
        return -1;
    } else {
        cout << "Q";
        if (arr[bot].key != target) {
            return -1;
        } else {
            return arr[bot].deal;
        }
    }
}

int BS2OUHE (int val)
{
    cout << "Q";
    if (val < 0 || val > 9) {
        return -1;
    }
    cout << "Q";
    if (val < 6) {
        cout << "Q";
        if (val == 0) {
            return 0;
        } else {
            cout << "Q";
            if (val == 3) {
                return 3;
            }
        }
    } else {
        cout << "Q";
        if (val < 9) {
            cout << "Q";
            if (val == 6) {
                return 6;
            }
        } else {
            return 9;
        }
    }
    return -1;
}

template<typename TypeC, typename TypeD, typename TypeV, std::size_t N>
struct CTBS_helper {
    std::array<Case<TypeC, TypeD>, N> arr;
    TypeD dft;
    constexpr auto operator()() {
        if constexpr (N == 2) {
            return [=](TypeV val){
                if (val == arr[0].key) {
                    return arr[0].deal;
                } else if (val == arr[1].key) {
                    return arr[1].deal;
                } else {
                    return dft;
                }
            };
        } else if constexpr (N == 1) {
            return [=](TypeV val){
                if (val == arr[0].key) {
                    return arr[0].deal;
                } else {
                    return dft;
                }
            };
        } else {
            constexpr auto mid = N/2;
            constexpr auto lhs = CTBS_helper<TypeC, TypeD, TypeV, mid>{
                [=]{
                    std::array<Case<TypeC, TypeD>, mid> nxt{};
                    for(auto i = 0; i < mid; i++) {
                        nxt[i] = arr[i];
                    }
                    return nxt;
                }(),
                dft,
            };
            constexpr auto rhs = CTBS_helper<TypeC, TypeD, TypeV, N - mid>{
                [=]{
                    //std::array<Case<TypeC, TypeD>, N - mid> nxt(
                    //    arr.begin() + mid, arr.end());
                    std::array<Case<TypeC, TypeD>, N - mid> nxt{};
                    for(auto i = 0; i < N - mid; i++) {
                        nxt[i] = arr[i + mid];
                    }
                    return nxt;
                }(),
                dft,
            };
            return [&](TypeV val){
                if (val < arr[mid].key) {
                    return lhs()(val);
                } else {
                    return rhs()(val);
                }
            };
        }
    }
};

template<typename TypeC, typename TypeD, std::size_t N>
struct CTBS_helper2 {
    std::array<Case<TypeC, TypeD>, N> arr;
    TypeD dft;
    constexpr auto operator()() {
        if constexpr (N == 2) {
            return [&](auto val){
                return dft;
            };
        } else {
            return [&](auto val){
                return dft;
            };
        }
    }
};

template<typename TypeC, typename TypeD, typename TypeV, std::size_t N>
struct CTBS_helper3 {
    std::array<Case<TypeC, TypeD>, N> arr;
    TypeD dft;
    constexpr auto operator()() {
        if constexpr (N == 2) {
            auto ret = [=](TypeV val) ->TypeD {
                if (val == arr[0].key) {
                    return arr[0].deal;
                } else if (val == arr[1].key) {
                    return arr[1].deal;
                } else {
                    return dft;
                }
            };
            return ret;
        } else {
            auto ret = [=](TypeV val) ->TypeD {
                if (val == arr[0].key) {
                    return arr[0].deal;
                } else {
                    return dft;
                }
            };
            return ret;
        }
    }
};

// template <class CASES, class DFT, size_t lhs, size_t rhs>
// constexpr auto makePredicBBB(CASES cases, DFT dft) {
//     constexpr auto Rcases = cases();
//     constexpr auto Rdft = dft();
//     constexpr auto arr = std::get<1>(Rcases);
//     using TypeC = decltype(arr[0].key);
//     using TypeD = decltype(arr[0].deal);

//     static_assert(typeid(arr[0].deal) == typeid(Rdft), "default类型不匹配！");
//     static_assert(std::get<0>(Rcases) == false, "键值有重复！");

//     constexpr auto _N = arr.size();
//     constexpr auto offset = arr[0].key;
//     constexpr std::size_t length =  arr[_N-1].key - arr[0].key + 1;

//     return PredicJ<length, TypeC, TypeD, offset> {
//         [&](){
//             std::array<TypeC, length> pat{};
//             pat.fill(Rdft);
//             for(auto c : arr) {
//                 pat[c.key - offset] = c.deal;
//             }
//             return pat;
//         }(),
//         Rdft,
//     };
// };

constexpr auto addition()
{
    return [](int a, int b){ return a+b; };
}

template <>
void more<2>() {
    auto cs0 = std::array{
        Case<int, int>{0, 0},
        Case<int, int>{3, 3},
        Case<int, int>{6, 6},
        Case<int, int>{9, 9},
    };

    cout << "\ncasearray 0:\n";
    for(auto i = 0; i < 10; i++) {
        cout << "find:" << i << "\t= " << BS2(cs0, i) << std::endl;
    }
    cout << "\ncasearray 1:\n";
    for(auto i = 0; i < 10; i++) {
        cout << "find:" << i << "\t= " << BS2OUHE(i) << std::endl;
    }

    auto BS2 = CTBS_helper<int, int, int, 2>{
        std::array{
            Case<int, int>{0, 0},
            Case<int, int>{3, 3},
        },
        -1,
    }();

    cout << "\ncasearray 2:\n";
    for(auto i = 0; i < 10; i++) {
        cout << "find:" << i << "\t= " << BS2(i) << std::endl;
    }
    
    auto CTBS3 = CTBS_helper<int, int, int, 2>{
        std::array{
            Case<int, int>{0, 0},
            Case<int, int>{3, 3},
        },
        -1,
    };
    auto BS3 = CTBS3();

    cout << "\ncasearray 3:\n";
    for(auto i = 0; i < 10; i++) {
        cout << "find:" << i << "\t= " << BS3(i) << std::endl;
    }


    auto BS4 = CTBS_helper3<int, int, int, 2>{
        std::array{
            Case<int, int>{0, 0},
            Case<int, int>{3, 3},
        },
        -1,
    }();

    cout << "\ncasearray 4:\n";
    for(auto i = 0; i < 10; i++) {
        cout << "find:" << i << "\t= " << BS4(i) << std::endl;
    }

    constexpr auto a = addition();
    cout << a(100, 200);
}


template <typename X>
constexpr auto f1(X x) {
    static_assert(std::get<0>(X::value()) == 101, "");
    static_assert(std::get<1>(X::value()) == 202, "");
    return "haha1\n";
}

struct S1 {
    static constexpr auto value() { return std::make_tuple(101, 202); }
};

template <typename X>
constexpr auto f2(X x) {
    static_assert(std::get<0>(x()) == 101, "");
    static_assert(std::get<1>(x()) == 202, "");
    static_assert(std::get<2>(x()) == 303, "");
    static_assert(std::get<3>(x()) == 404, "");
    return "haha2\n";
}

struct S2 {
    static constexpr array<int, 4> arr{101, 202, 303, 404};
    //array<int, 2> arr{101, 202};
    constexpr auto operator()() {return std::make_tuple(arr[0], arr[1], arr[2], arr[3]);}
};
// static constexpr array<int, 2> S2::arr{101, 102};

template <>
void more<3>() {
    cout << f1(S1{});
    cout << f2(S2{});
}


struct infos {
    static constexpr array<int, 4> arr{0, 3, 6, 9};
    static constexpr int dft{-1};
    //array<int, 2> arr{101, 202};
    //constexpr auto operator()() {return std::make_tuple(arr[0], arr[1], arr[2], arr[3]);}
    constexpr auto getarr() {return arr;}
    constexpr auto getdft() {return dft;}
};

template <size_t lhs, size_t rhs, typename X>
constexpr auto retLbd0(X x) {
    constexpr auto arr = x.getarr();
    constexpr auto dft = x.getdft();
    constexpr auto N = rhs - lhs;

    if constexpr (N == 2) {
        auto ret = [=](int val) ->int {
            if (val == arr[lhs]) {
                return arr[lhs];
            } else if (val == arr[lhs + 1]) {
                return arr[lhs + 1];
            } else {
                return dft;
            }
        };
        return ret;
    } else if constexpr (N == 1) {
        auto ret = [=](int val) ->int {
            if (val == arr[lhs]) {
                return arr[lhs];
            } else {
                return dft;
            }
        };
        return ret;
    } else {
        constexpr auto cmpl = retLbd0<lhs, lhs + N/2>(x);
        constexpr auto cmpr = retLbd0<lhs + N/2, lhs + N>(x);
        auto ret = [=](int val) ->int {
            if (val < arr[lhs + N/2]) {
                return cmpl(val);
            } else {
                return cmpr(val);
            }
        };
        return ret;
    }
}

template <size_t shs = 0, size_t off = 0, typename X>
constexpr auto retLbd1(X x) {
    constexpr auto arr = x.getarr();
    constexpr auto dft = x.getdft();
    //constexpr auto N = off;
    constexpr size_t N = (off != 0) ? off : arr.size();

    if constexpr (N == 2) {
        auto ret = [=](int val) ->int {
            if (val == arr[shs]) {
                return arr[shs];
            } else if (val == arr[shs + 1]) {
                return arr[shs + 1];
            } else {
                return dft;
            }
        };
        return ret;
    } else if constexpr (N == 1) {
        auto ret = [=](int val) ->int {
            if (val == arr[shs]) {
                return arr[shs];
            } else {
                return dft;
            }
        };
        return ret;
    } else {
        if constexpr (N == 5) {
            static_assert(shs + N/2 == 2);
            static_assert(N - N/2 == 3);
        }
        constexpr auto cmpl = retLbd1<shs, N/2>(x);
        constexpr auto cmpr = retLbd1<shs + N/2, N - N/2>(x);
        auto ret = [=](int val) ->int {
            if (val < arr[shs + N/2]) {
                return cmpl(val);
            } else {
                return cmpr(val);
            }
        };
        return ret;
    }
}



template <size_t lhs = 0, size_t rhs = 0, typename X>
constexpr auto retLbdCase0(X x) {
    constexpr auto arr = x.getarr();
    constexpr auto dft = x.getdft();
    constexpr size_t N = (rhs == 0) ? arr.size() : rhs - lhs;

    using TypeD = decltype(dft);

    if constexpr (N == 2) {
        constexpr auto ret = [=](int val) ->TypeD {
            if (val == arr[lhs].key) {
                return arr[lhs].deal;
            } else if (val == arr[lhs + 1].key) {
                return arr[lhs + 1].deal;
            } else {
                return dft;
            }
        };
        return ret;
    } else if constexpr (N == 1) {
        constexpr auto ret = [=](int val) ->TypeD {
            if (val == arr[lhs].key) {
                return arr[lhs].deal;
            } else {
                return dft;
            }
        };
        return ret;
    } else {
        constexpr auto cmpl = retLbdCase0<lhs, lhs + N/2>(x);
        constexpr auto cmpr = retLbdCase0<lhs + N/2, lhs + N>(x);
        auto ret = [=](int val) ->TypeD {
            if (val < arr[lhs + N/2].key) {
                return cmpl(val);
            } else {
                return cmpr(val);
            }
        };
        return ret;
    }
}

template <size_t shs = 0, size_t off = 0, typename X>
constexpr auto retLbdCase1(X x) {
    constexpr auto arr = x.getarr();
    constexpr auto dft = x.getdft();
    constexpr auto N = (off != 0) ? off : arr.size();

    using TypeD = decltype(dft);

    if constexpr (N == 2) {
        auto ret = [=](int val) ->TypeD {
            if (val == arr[shs].key) {
                return arr[shs].deal;
            } else if (val == arr[shs + 1].key) {
                return arr[shs + 1].deal;
            } else {
                return dft;
            }
        };
        return ret;
    } else if constexpr (N == 1) {
        auto ret = [=](int val) ->TypeD {
            if (val == arr[shs].key) {
                return arr[shs].deal;
            } else {
                return dft;
            }
        };
        return ret;
    } else {
        constexpr auto cmpl = retLbdCase1<shs, N/2>(x);
        constexpr auto cmpr = retLbdCase1<shs + N/2, N - N/2>(x);
        auto ret = [=](int val) ->TypeD {
            if (val < arr[shs + N/2].key) {
                return cmpl(val);
            } else {
                return cmpr(val);
            }
        };
        return ret;
    }
}



//template <>
//void more<4>() {
void more4() {
    constexpr auto bs0 = retLbd0<0, 4>(infos{});
    // cout << "\ncasearray 0:\n";
    // for(auto i = 0; i < 20; i++) {
    //     cout << "find:" << i << "\t= " << bs0(i) << std::endl;
    // }

    constexpr auto bs1 = retLbd1([](){
        using S = struct{
            //static constexpr array<int, 4> arr{0, 3, 6, 9};
            //static constexpr int dft{-1};
            constexpr auto getarr() {return array<int, 5>{//1 ,2 ,3 ,4 ,6 ,8 ,10,13,16,19,
                0,
                1,
                2,
                3,
                4,
            };}
            constexpr auto getdft() {return (int)-1;}
        };
        return S{};
    }());
    cout << "\ncasearray 1:\n";
    for(auto i = 0; i < 20; i++) {
        cout << "find:" << i << "\t= " << bs1(i) << std::endl;
    }

    constexpr auto bs2 = retLbdCase0([](){
        using S = struct{
            //static constexpr array<int, 4> arr{0, 3, 6, 9};
            //static constexpr int dft{-1};
            constexpr auto getarr() {return array{
                Case<int, int>{1 , 19},
                Case<int, int>{2 , 18},
                Case<int, int>{3 , 17},
                Case<int, int>{4 , 16},
                Case<int, int>{6 , 15},
                Case<int, int>{8 , 14},
                Case<int, int>{10, 13},
                Case<int, int>{13, 12},
                Case<int, int>{16, 11},
                Case<int, int>{19, 10},
                
                Case<int, int>{1  + 20, 19 + 20},
                Case<int, int>{2  + 20, 18 + 20},
                Case<int, int>{3  + 20, 17 + 20},
                Case<int, int>{4  + 20, 16 + 20},
                Case<int, int>{6  + 20, 15 + 20},
                Case<int, int>{8  + 20, 14 + 20},
                Case<int, int>{10 + 20, 13 + 20},
                Case<int, int>{13 + 20, 12 + 20},
                Case<int, int>{16 + 20, 11 + 20},
                Case<int, int>{19 + 20, 10 + 20},
                
                Case<int, int>{1  + 40, 19 + 40},
                Case<int, int>{2  + 40, 18 + 40},
                Case<int, int>{3  + 40, 17 + 40},
                Case<int, int>{4  + 40, 16 + 40},
                Case<int, int>{6  + 40, 15 + 40},
                Case<int, int>{8  + 40, 14 + 40},
                Case<int, int>{10 + 40, 13 + 40},
                Case<int, int>{13 + 40, 12 + 40},
                Case<int, int>{16 + 40, 11 + 40},
                Case<int, int>{19 + 40, 10 + 40},
                
                Case<int, int>{1  + 60, 19 + 60},
                Case<int, int>{2  + 60, 18 + 60},
                Case<int, int>{3  + 60, 17 + 60},
                Case<int, int>{4  + 60, 16 + 60},
                Case<int, int>{6  + 60, 15 + 60},
                Case<int, int>{8  + 60, 14 + 60},
                Case<int, int>{10 + 60, 13 + 60},
                Case<int, int>{13 + 60, 12 + 60},
                Case<int, int>{16 + 60, 11 + 60},
                Case<int, int>{19 + 60, 10 + 60},
                
                Case<int, int>{1  + 80, 19 + 80},
                Case<int, int>{2  + 80, 18 + 80},
                Case<int, int>{3  + 80, 17 + 80},
                Case<int, int>{4  + 80, 16 + 80},
                Case<int, int>{6  + 80, 15 + 80},
                Case<int, int>{8  + 80, 14 + 80},
                Case<int, int>{10 + 80, 13 + 80},
                Case<int, int>{13 + 80, 12 + 80},
                Case<int, int>{16 + 80, 11 + 80},
                Case<int, int>{19 + 80, 10 + 80},
                
            };}
            constexpr auto getdft() {return (int)0;}
        };
        return S{};
    }());
    // cout << "\ncasearray 2:\n";
    // for(auto i = 0; i < 20; i++) {
    //     cout << "find:" << i << "\t= " << bs2(i) << std::endl;
    // }

    constexpr auto bs3 = retLbdCase1([](){
        using S = struct{
            //static constexpr array<int, 4> arr{0, 3, 6, 9};
            //static constexpr int dft{-1};
            constexpr auto getarr() {return array{
                Case<int, int>{1 , 19},
                Case<int, int>{2 , 18},
                Case<int, int>{3 , 17},
                Case<int, int>{4 , 16},
                Case<int, int>{6 , 15},
                Case<int, int>{8 , 14},
                Case<int, int>{10, 13},
                Case<int, int>{13, 12},
                Case<int, int>{16, 11},
                Case<int, int>{19, 10},
                
                Case<int, int>{1  + 20, 19 + 20},
                Case<int, int>{2  + 20, 18 + 20},
                Case<int, int>{3  + 20, 17 + 20},
                Case<int, int>{4  + 20, 16 + 20},
                Case<int, int>{6  + 20, 15 + 20},
                Case<int, int>{8  + 20, 14 + 20},
                Case<int, int>{10 + 20, 13 + 20},
                Case<int, int>{13 + 20, 12 + 20},
                Case<int, int>{16 + 20, 11 + 20},
                Case<int, int>{19 + 20, 10 + 20},
                
                Case<int, int>{1  + 40, 19 + 40},
                Case<int, int>{2  + 40, 18 + 40},
                Case<int, int>{3  + 40, 17 + 40},
                Case<int, int>{4  + 40, 16 + 40},
                Case<int, int>{6  + 40, 15 + 40},
                Case<int, int>{8  + 40, 14 + 40},
                Case<int, int>{10 + 40, 13 + 40},
                Case<int, int>{13 + 40, 12 + 40},
                Case<int, int>{16 + 40, 11 + 40},
                Case<int, int>{19 + 40, 10 + 40},
                
                Case<int, int>{1  + 60, 19 + 60},
                Case<int, int>{2  + 60, 18 + 60},
                Case<int, int>{3  + 60, 17 + 60},
                Case<int, int>{4  + 60, 16 + 60},
                Case<int, int>{6  + 60, 15 + 60},
                Case<int, int>{8  + 60, 14 + 60},
                Case<int, int>{10 + 60, 13 + 60},
                Case<int, int>{13 + 60, 12 + 60},
                Case<int, int>{16 + 60, 11 + 60},
                Case<int, int>{19 + 60, 10 + 60},
                
                Case<int, int>{1  + 80, 19 + 80},
                Case<int, int>{2  + 80, 18 + 80},
                Case<int, int>{3  + 80, 17 + 80},
                Case<int, int>{4  + 80, 16 + 80},
                Case<int, int>{6  + 80, 15 + 80},
                Case<int, int>{8  + 80, 14 + 80},
                Case<int, int>{10 + 80, 13 + 80},
                Case<int, int>{13 + 80, 12 + 80},
                Case<int, int>{16 + 80, 11 + 80},
                Case<int, int>{19 + 80, 10 + 80},
                
            };}
            constexpr auto getdft() {return (int)0;}
        };
        return S{};
    }());
    // cout << "\ncasearray 3:\n";
    // for(auto i = 0; i < 20; i++) {
    //     cout << "find:" << i << "\t= " << bs3(i) << std::endl;
    // }

    // cout << "haha1";
}



























int main(int argc,char* argv[])
{
    //普通探索
    constexpr Case<int, int> a{1, 1};
    constexpr Case<int, int> b{2, 2}; 
    constexpr int ak = a.key;

    //不带参数的lambda
    constexpr int akl1 = [&](){return a.key;}();
    //constexpr auto aklf1 = [&](){return a.key;}; error in clang
    constexpr auto aklf1 = [=](){return a.key;};
    constexpr int aklr1 = aklf1();

    //带参数的lambda
    constexpr int akl2 = [](const Case<int, int> &aa){return aa.key;}(a);
    constexpr auto aklf2 = [](const Case<int, int> &aa){return aa.key;};
    constexpr int aklr2 = aklf2(a);

    //比较
    //constexpr Case<int, int> &ref_c = a < b ? a : b; error
    constexpr Case<int, int> c = a < b ? a : b; 
    constexpr int ck = c.key;

    //不带参数的labmda内比较
    constexpr int ckl1 = [&](){
        //constexpr Case<int, int> &c = a < b ? a : b;
        constexpr Case<int, int> c = a < b ? a : b;
        return c.key;
    }();
    constexpr auto cklf1 = [=](){
        constexpr Case<int, int> c = a < b ? a : b;
        return c.key;
    };
    constexpr int cklr1 = cklf1();

    //带参数的labmda内比较
    constexpr int ckl2 = [](const Case<int, int> aa, const Case<int, int> bb){
        Case<int, int> cc = aa < bb ? aa : bb;
        return cc.key;
    }(a, b);
    constexpr auto cklf2 = [](const Case<int, int> aa, const Case<int, int> bb){
        Case<int, int> cc = aa < bb ? aa : bb;
        return cc.key;
    };
    constexpr int cklr2 = cklf2(a, b);

    //直接比较key
    constexpr Case<int, int> d = a.key < b.key ? a : b; 
    constexpr int dk = d.key;

    constexpr int eak = a.key;
    constexpr int ebk = b.key;
    constexpr Case<int, int> e = eak < ebk ? a : b;
    constexpr Case<int, int> e2 = a.key < b.key ? a : b;

    //Case array排序
    //constexpr auto s2 = Cases(array{
    //    Case<int, int> {2, 2},
    //    Case<int, int> {1, 1},
    //});
    //std::copy(std::cbegin(s2), std::cend(s2), std::ostream_iterator<Case<int, int>>{std::cout, " "});

    //constexpr int s2ak = s2[0].key;
    //
    //constexpr int s2mink = s2[0].key;
    //constexpr int s2maxk = s2[1].key;
    //constexpr int s2maxk0 = s2[s2.size() - 1].key;

    //constexpr参数
    //https://mpark.github.io/programming/2017/05/26/constexpr-function-parameters/
    //以下两个不能用
    constexpr auto cep_l1 = [](const Case<int, int> a, const Case<int, int> b){
        //static_assert(a.key != b.key); error
        return a < b ? a : b;
    }(a, b);
    constexpr auto cep_l2 = [](const std::tuple<Case<int, int>> a, const std::tuple<Case<int, int>> b){
        //static_assert(std::get<0>(a).key != std::get<0>(b).key, "bad"); error
        return a < b ? a : b;
    }(a, b);

    constexpr auto cep_l3 = [](auto x) {
        static_assert(std::get<0>(x()) == 101, "");
        static_assert(std::get<1>(x()) == 202, "");
        return x();
    }([]{return std::make_tuple(101, 202);});

    constexpr auto cep_l4 = [](auto x) {
        static_assert(std::get<0>(x()).key == 2, "");
        static_assert(std::get<1>(x()).key == 1, "");
        return x();
    }([]{return std::tuple{Case<int, int>{2, 2}, Case<int, int>{1, 1}};});



    constexpr auto cep_l5 = [](auto x, auto y) {
        static_assert(std::get<0>(x()).key == 2, "");
        static_assert(std::get<1>(x()).key == 1, "");

        static_assert(std::get<0>(y()).key == 2, "");
        static_assert(std::get<1>(y()).key == 1, "");
        return std::tuple{x(), y()};
    }(CONVALS(Case<int, int>{2, 2}, Case<int, int>{1, 1},), CONARRY(Case<int, int>{2, 2}, Case<int, int>{1, 1},));



    //more<0>();
    //more<1>();
    //more<2>();
    //more<3>();
    //more<4>();
    more4();








    return 0;
}
