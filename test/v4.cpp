#include <iostream>
#include <utility>
#include <Switch.hpp>
#include <boost/timer.hpp>
#include <mpark/patterns.hpp>

using namespace std;
using namespace GTL::Switch;

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
            cout << "a ";
            if (val == arr[lhs].key) {
                cout << "l ";
                cout << " k = " << arr[lhs].key <<  "\tv = " << arr[lhs].deal << "\t";
                return arr[lhs].deal;
            } else if (val == arr[lhs + 1].key) {
                cout << "r ";
                cout << " k = " << arr[lhs + 1].key <<  "\tv = " << arr[lhs + 1].deal << "\t";
                return arr[lhs + 1].deal;
            } else {
                cout << "d ";
                cout << " k = no\tv = 0\t";
                return dft;
            }
        };
        return ret;
    } else if constexpr (N == 1) {
        constexpr auto ret = [=](int val) ->TypeD {
            cout << "b ";
            if (val == arr[lhs].key) {
                cout << "l ";
                cout << " k = " << arr[lhs].key <<  "\tv = " << arr[lhs].deal << "\t";
                return arr[lhs].deal;
            } else {
                cout << "d ";
                cout << " k = no\tv = 0\t";
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
            cout << "a ";
            if (val == arr[shs].key) {
                cout << "l ";
                cout << " k = " << arr[shs].key <<  "\tv = " << arr[shs].deal << "\t";
                return arr[shs].deal;
            } else if (val == arr[shs + 1].key) {
                cout << "r ";
                cout << " k = " << arr[shs + 1].key <<  "\tv = " << arr[shs + 1].deal << "\t";
                return arr[shs + 1].deal;
            } else {
                cout << "d ";
                cout << " k = no\tv = 0\t";
                return dft;
            }
        };
        return ret;
    } else if constexpr (N == 1) {
        auto ret = [=](int val) ->TypeD {
            cout << "b ";
            if (val == arr[shs].key) {
                cout << "l ";
                cout << " k = " << arr[shs].key <<  "\tv = " << arr[shs].deal << "\t";
                return arr[shs].deal;
            } else {
                cout << "d ";
                cout << " k = no\tv = 0\t";
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



void more4() {

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
    cout << "\ncasearray 2:\n";
    for(auto i = 0; i < 20; i++) {
        cout << "find:" << i << "\t= " << bs2(i) << std::endl;
    }

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
    cout << "\ncasearray 3:\n";
    for(auto i = 0; i < 20; i++) {
        cout << "find:" << i << "\t= " << bs3(i) << std::endl;
    }

    cout << "haha1";
}



























int main(int argc,char* argv[])
{
    more4();
    return 0;
}
