#ifndef GTL_SWITCH_SWITCHB_HPP
#define GTL_SWITCH_SWITCHB_HPP

#include <array>
#include <experimental/array>
#include <algorithm>
#include <cstddef>
#include <type_traits>
#include "Switch/SwitchCommon.hpp"

namespace GTL::Switch {

namespace detail {
    template <std::size_t length, class TypeC, class TypeD>
    struct PredicB {
        std::array<Case<TypeC, TypeD>, length> seq;
        TypeD dft;

        template <typename TypeV>
        constexpr TypeD operator()(TypeV &&val) const {
            std::size_t bot = 0, top = length;
 
            while (bot < top) {
                std::size_t mid = (bot + top) / 2;
                if (seq[mid].key < val)
                    bot = mid + 1;
                else
                    top = mid;
            }
            if (bot == length || seq[bot].key != val) {
                return dft;
            } else {
                return seq[bot].deal;
            }
        }
 
        friend std::ostream& operator<<(std::ostream& ostr, PredicB pb) {
            ostr << "length = " << length << ", array =\n";
            for(auto &c :pb.seq)
                ostr << c;
            return ostr;
        }
        friend std::ostream& operator<<(std::ostream& ostr, PredicB &pb) {
            ostr << "length = " << length << ", array =\n";
            for(auto &c :pb.seq)
                ostr << c;
            return ostr;
        }
    };

    template <size_t shs = 0, size_t off = 0, typename Xarr, typename Xdft>
    constexpr auto PredicB2Real(Xarr a, Xdft d) {
        constexpr auto arr = std::get<1>(a());
        constexpr auto dft = d();
        constexpr auto N = off;

        using TypeD = decltype(dft);

        if constexpr (N == 0) {
            constexpr auto cmpinner = PredicB2Real<0, arr.size()>(a, d);
            return [=](int val) ->TypeD {
                if (val < arr[0].key || val > arr[arr.size() - 1].key) {
                    return dft;
                } else {
                    return cmpinner(val);
                }
            };
        } else if constexpr (N == 2) {
            return [=](int val) ->TypeD {
                if (val == arr[shs].key) {
                    return arr[shs].deal;
                } else if (val == arr[shs + 1].key) {
                    return arr[shs + 1].deal;
                } else {
                    return dft;
                }
            };
        } else if constexpr (N == 1) {
            return [=](int val) ->TypeD {
                if (val == arr[shs].key) {
                    return arr[shs].deal;
                } else {
                    return dft;
                }
            };
        } else {
            constexpr auto cmpl = PredicB2Real<shs, N/2>(a, d);
            constexpr auto cmpr = PredicB2Real<shs + N/2, N - N/2>(a, d);
            return [=](int val) ->TypeD {
                if (val < arr[shs + N/2].key) {
                    return cmpl(val);
                } else {
                    return cmpr(val);
                }
            };
        }
    }

    template <size_t shs = 0, size_t off = 0, bool lbrd = false, bool rbrd = false, typename Xarr, typename Xdft>
    constexpr auto PredicB3Real(Xarr a, Xdft d) {
        constexpr auto arr = std::get<1>(a());
        constexpr auto dft = d();
        constexpr auto N = off;

        using TypeD = decltype(dft);

        
        if constexpr (N == 0) {
            constexpr auto cmpinner = PredicB3Real<0, arr.size(), true, true>(a, d);
            return [=](int val) ->TypeD {
                if (val < arr[0].key || val > arr[arr.size() - 1].key) {
                    return dft;
                } else {
                    return cmpinner(val);
                }
            };
        } else if constexpr (N == 2) {
            if constexpr (lbrd == true && rbrd == true && arr[shs + 1].key - arr[shs].key == 1) {
                return [=](int val) ->TypeD {
                    if (val == arr[shs].key) {
                        return arr[shs].deal;
                    } else {
                        return arr[shs + 1].deal;
                    }
                };
            } else {
                return [=](int val) ->TypeD {
                    if (val == arr[shs].key) {
                        return arr[shs].deal;
                    } else if (val == arr[shs + 1].key) {
                        return arr[shs + 1].deal;
                    } else {
                        return dft;
                    }
                };
            }
        } else if constexpr (N == 1) {
            return [=](int val) ->TypeD {
                if (val == arr[shs].key) {
                    return arr[shs].deal;
                } else {
                    return dft;
                }
            };
        } else if constexpr (N == 3 && lbrd == true) {
            constexpr bool newbrd = (arr[shs + 1].key - arr[shs].key == 1);
            constexpr auto cmpr = PredicB3Real<shs + N/2, N - N/2, newbrd, rbrd>(a, d);
            return [=](int val) ->TypeD {
                if (val == arr[shs].key) {
                    return arr[shs].deal;
                } else {
                    return cmpr(val);
                }
            };
        } else if constexpr (N == 3 && rbrd == true) {
            constexpr bool newbrd = (arr[shs + 1].key - arr[shs].key == 1);
            constexpr auto cmpl = PredicB3Real<shs + N/2, N - N/2, lbrd, newbrd>(a, d);
            return [=](int val) ->TypeD {
                if (val == arr[shs + N - 1].key) {
                    return arr[shs + N - 1].deal;
                } else {
                    return cmpl(val);
                }
            };
        } else {
            constexpr auto cmpl = PredicB3Real<shs, N/2, lbrd, rbrd>(a, d);
            constexpr auto cmpr = PredicB3Real<shs + N/2, N - N/2, lbrd, rbrd>(a, d);
            return [=](int val) ->TypeD {
                if (val < arr[shs + N/2].key) {
                    return cmpl(val);
                } else {
                    return cmpr(val);
                }
            };
        }
    }

    template <class CASES>
    constexpr auto makePredicB(CASES cases) {
        constexpr auto Rcases = cases();
        static_assert(std::get<0>(Rcases) == false, "键值有重复！");
        constexpr auto arr = std::get<1>(Rcases);
        using TypeC = decltype(arr[0].key);
        using TypeD = decltype(arr[0].deal);
        constexpr auto length = arr.size();

        return PredicB<length, TypeC, TypeD> {
            arr,
            TypeD(),
        };
    };

    template <class CASES, class DFT>
    constexpr auto makePredicB(CASES cases, DFT dft) {
        constexpr auto Rcases = cases();
        constexpr auto Rdft = dft();
        constexpr auto arr = std::get<1>(Rcases);
        using TypeC = decltype(arr[0].key);
        using TypeD = decltype(arr[0].deal);

        static_assert(TypeEqual(arr[0].deal, Rdft.deal), "default类型不匹配！");
        static_assert(std::get<0>(Rcases) == false, "键值有重复！");

        constexpr auto length = arr.size();
        
        return PredicB<length, TypeC, TypeD> {
            arr,
            Rdft.deal,
        };
    };

    template <class CASES>
    constexpr auto makePredicB2(CASES cases) {
        // error in clang:: reference to local variable 'arr' declared in enclosing function
        // so only code in this
        constexpr auto Rcases = cases();
        static_assert(std::get<0>(Rcases) == false, "键值有重复！");
        constexpr auto arr = std::get<1>(Rcases);
        // using TypeC = decltype(arr[0].key);
        using TypeD = decltype(arr[0].deal);

        return PredicB2Real(cases, [=]{return TypeD();});
    };

    template <class CASES, class DFT>
    constexpr auto makePredicB2(CASES cases, DFT dft) {
        constexpr auto Rcases = cases();
        constexpr auto Rdft = dft();
        constexpr auto arr = std::get<1>(Rcases);

        static_assert(TypeEqual(arr[0].deal, Rdft.deal), "default类型不匹配！");
        static_assert(std::get<0>(Rcases) == false, "键值有重复！");

        return PredicB2Real(cases, [=]{return Rdft.deal;});
    };

    template <class CASES>
    constexpr auto makePredicB3(CASES cases) {
        // error in clang:: reference to local variable 'arr' declared in enclosing function
        // so only code in this
        constexpr auto Rcases = cases();
        static_assert(std::get<0>(Rcases) == false, "键值有重复！");
        constexpr auto arr = std::get<1>(Rcases);
        // using TypeC = decltype(arr[0].key);
        using TypeD = decltype(arr[0].deal);

        return PredicB3Real(cases, [=]{return TypeD();});
    };

    template <class CASES, class DFT>
    constexpr auto makePredicB3(CASES cases, DFT dft) {
        constexpr auto Rcases = cases();
        constexpr auto Rdft = dft();
        constexpr auto arr = std::get<1>(Rcases);

        static_assert(TypeEqual(arr[0].deal, Rdft.deal), "default类型不匹配！");
        static_assert(std::get<0>(Rcases) == false, "键值有重复！");

        return PredicB3Real(cases, [=]{return Rdft.deal;});
    };
    
}

template <class INPUT, class CASES>
inline auto SwitchB(INPUT inp, CASES cases) {
   static constexpr auto predic = detail::makePredicB(cases);
   return predic(inp);
};

template <class INPUT, class CASES, class DFT>
inline auto SwitchB(INPUT inp, CASES cases, DFT dft) {
   static constexpr auto predic = detail::makePredicB(cases, dft);
   return predic(inp);
};

template <class INPUT, class CASES>
inline auto SwitchB2(INPUT inp, CASES cases) {
    static constexpr auto predic = detail::makePredicB2(cases);
    return predic(inp);
};

template <class INPUT, class CASES, class DFT>
inline auto SwitchB2(INPUT inp, CASES cases, DFT dft) {
    static constexpr auto predic = detail::makePredicB2(cases, dft);
    return predic(inp);
};

template <class INPUT, class CASES>
inline auto SwitchB3(INPUT inp, CASES cases) {
    static constexpr auto predic = detail::makePredicB3(cases);
    return predic(inp);
};

template <class INPUT, class CASES, class DFT>
inline auto SwitchB3(INPUT inp, CASES cases, DFT dft) {
    static constexpr auto predic = detail::makePredicB3(cases, dft);
    return predic(inp);
};

inline auto SwitchB_Test(int val) noexcept
{
    constexpr auto cases = CASES(
        Case<int, int>{19, 10},
        Case<int, int>{16, 11},
        Case<int, int>{13, 12},
        Case<int, int>{10, 13},
        Case<int, int>{8 , 14},
        Case<int, int>{6 , 15},
        Case<int, int>{4 , 16},
        Case<int, int>{3 , 17},
        Case<int, int>{2 , 18},
        Case<int, int>{1 , 19},

        Case<int, int>{39, 20},
        Case<int, int>{36, 21},
        Case<int, int>{33, 22},
        Case<int, int>{30, 23},
        Case<int, int>{28, 24},
        Case<int, int>{26, 25},
        Case<int, int>{24, 26},
        Case<int, int>{23, 27},
        Case<int, int>{22, 28},
        Case<int, int>{21, 29},

        Case<int, int>{59, 30},
        Case<int, int>{56, 31},
        Case<int, int>{53, 32},
        Case<int, int>{50, 33},
        Case<int, int>{48, 34},
        Case<int, int>{46, 35},
        Case<int, int>{44, 36},
        Case<int, int>{43, 37},
        Case<int, int>{42, 38},
        Case<int, int>{41, 39},

        Case<int, int>{79, 40},
        Case<int, int>{76, 41},
        Case<int, int>{73, 42},
        Case<int, int>{70, 43},
        Case<int, int>{68, 44},
        Case<int, int>{66, 45},
        Case<int, int>{64, 46},
        Case<int, int>{63, 47},
        Case<int, int>{62, 48},
        Case<int, int>{61, 49},

        Case<int, int>{99, 50},
        Case<int, int>{96, 51},
        Case<int, int>{93, 52},
        Case<int, int>{90, 53},
        Case<int, int>{88, 54},
        Case<int, int>{86, 55},
        Case<int, int>{84, 56},
        Case<int, int>{83, 57},
        Case<int, int>{82, 58},
        Case<int, int>{81, 59},
    )();
    constexpr bool repeatKey = std::get<0>(cases);
    constexpr auto arr = std::get<1>(cases);
    constexpr auto length = arr.size();
    constexpr static detail::PredicB<length, int, int> predic{
        arr,
        0,
    };

    return predic(val);
}

//inline auto SwitchB_Test2(int val) noexcept
//{
//    constexpr static detail::SwitchB<99, int, int, 0> predic{
//        [](){
//            std::array<int, 99> pat{};
//            pat[19 - 1] = 10;
//            pat[16 - 1] = 11;
//            pat[13 - 1] = 12;
//            pat[10 - 1] = 13;
//            pat[8  - 1] = 14;
//            pat[6  - 1] = 15;
//            pat[4  - 1] = 16;
//            pat[3  - 1] = 17;
//            pat[2  - 1] = 18;
//            pat[1  - 1] = 19;
//
//            pat[39 - 1] = 20;
//            pat[36 - 1] = 21;
//            pat[33 - 1] = 22;
//            pat[30 - 1] = 23;
//            pat[28 - 1] = 24;
//            pat[26 - 1] = 25;
//            pat[24 - 1] = 26;
//            pat[23 - 1] = 27;
//            pat[22 - 1] = 28;
//            pat[21 - 1] = 29;
//
//            pat[59 - 1] = 30;
//            pat[56 - 1] = 31;
//            pat[53 - 1] = 32;
//            pat[50 - 1] = 33;
//            pat[48 - 1] = 34;
//            pat[46 - 1] = 35;
//            pat[44 - 1] = 36;
//            pat[43 - 1] = 37;
//            pat[42 - 1] = 38;
//            pat[41 - 1] = 39;
//
//            pat[79 - 1] = 40;
//            pat[76 - 1] = 41;
//            pat[73 - 1] = 42;
//            pat[70 - 1] = 43;
//            pat[68 - 1] = 44;
//            pat[66 - 1] = 45;
//            pat[64 - 1] = 46;
//            pat[63 - 1] = 47;
//            pat[62 - 1] = 48;
//            pat[61 - 1] = 49;
//
//            pat[99 - 1] = 50;
//            pat[96 - 1] = 51;
//            pat[93 - 1] = 52;
//            pat[90 - 1] = 53;
//            pat[88 - 1] = 54;
//            pat[86 - 1] = 55;
//            pat[84 - 1] = 56;
//            pat[83 - 1] = 57;
//            pat[82 - 1] = 58;
//            pat[81 - 1] = 59;
//
//            return pat;
//        }(),
//        0,
//    };
//
//    return predic(val - 1);
//}
//
//inline auto SwitchB_Test3(int val) noexcept
//{
//    constexpr static detail::SwitchB<99, int, int, 1> predic{
//        [](){
//            std::array<int, 99> pat{};
//            pat[19 - 1] = 10;
//            pat[16 - 1] = 11;
//            pat[13 - 1] = 12;
//            pat[10 - 1] = 13;
//            pat[8  - 1] = 14;
//            pat[6  - 1] = 15;
//            pat[4  - 1] = 16;
//            pat[3  - 1] = 17;
//            pat[2  - 1] = 18;
//            pat[1  - 1] = 19;
//
//            pat[39 - 1] = 20;
//            pat[36 - 1] = 21;
//            pat[33 - 1] = 22;
//            pat[30 - 1] = 23;
//            pat[28 - 1] = 24;
//            pat[26 - 1] = 25;
//            pat[24 - 1] = 26;
//            pat[23 - 1] = 27;
//            pat[22 - 1] = 28;
//            pat[21 - 1] = 29;
//
//            pat[59 - 1] = 30;
//            pat[56 - 1] = 31;
//            pat[53 - 1] = 32;
//            pat[50 - 1] = 33;
//            pat[48 - 1] = 34;
//            pat[46 - 1] = 35;
//            pat[44 - 1] = 36;
//            pat[43 - 1] = 37;
//            pat[42 - 1] = 38;
//            pat[41 - 1] = 39;
//
//            pat[79 - 1] = 40;
//            pat[76 - 1] = 41;
//            pat[73 - 1] = 42;
//            pat[70 - 1] = 43;
//            pat[68 - 1] = 44;
//            pat[66 - 1] = 45;
//            pat[64 - 1] = 46;
//            pat[63 - 1] = 47;
//            pat[62 - 1] = 48;
//            pat[61 - 1] = 49;
//
//            pat[99 - 1] = 50;
//            pat[96 - 1] = 51;
//            pat[93 - 1] = 52;
//            pat[90 - 1] = 53;
//            pat[88 - 1] = 54;
//            pat[86 - 1] = 55;
//            pat[84 - 1] = 56;
//            pat[83 - 1] = 57;
//            pat[82 - 1] = 58;
//            pat[81 - 1] = 59;
//
//            return pat;
//        }(),
//        0,
//    };
//
//    return predic(val);
//}

} // namespace GTL::Switch

#endif // GTL_SWITCH_SWITCHJ_HPP
