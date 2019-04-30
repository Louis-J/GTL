#include <iostream>
#include <utility>
#include <functional>
#include <Switch.hpp>
#include <boost/timer.hpp>
#include <mpark/patterns.hpp>

using namespace std;
using namespace GTL::Switch;

typedef int (*add0)(int);
const auto switchfunc0 = [](int n)
{
    return SwitchJ(n, CASES(
        Case<int, add0>{3 ,[](int v){
            return v + 3;
        }},
        Case<int, add0>{2 ,[](int v){
            return v + 2;
        }},
        Case<int, add0>{1 ,[](int v){
            return v + 1;
        }},
        ),
        DEFAULT(<add0>{[](int v){
            return -1;
    }}))(1);
};


typedef int (*add1)(int&, int);
const auto switchfunc1 = [](int n)
{
    static int which = 0;
    return SwitchJ(n, CASES(
        Case<int, add1>{3 ,[](auto &which, int v){
            which ++;
            return v + 3;
        }},
        Case<int, add1>{2 ,[](auto &which, int v){
            which ++;
            return v + 2;
        }},
        Case<int, add1>{1 ,[](auto &which, int v){
            which ++;
            return v + 1;
        }},
        ),DEFAULT(<add1>{[](auto &which, int v){
            return which;
    }}))(which, 1);
};



// typedef int (*add2)(int);
// const auto switchfunc2 = [](int n)
// {
//     static int which = 0;
//     auto deal3 = [&](int v){
//         which ++;
//         return v + 3;
//     };
//     auto deal2 = [&](int v){
//         which ++;
//         return v + 2;
//     };
//     auto deal1 = [&](int v){
//         which ++;
//         return v + 1;
//     };
//     auto deal_ = [&](int v){
//         return which;
//     };
//     return SwitchJ(n, CASES(
//         Case<int, add2>{3 ,[](int v){
//             return deal3(v);
//         }},
//         Case<int, add2>{2 ,[](int v){
//             return deal2(v);
//         }},
//         Case<int, add2>{1 ,[](int v){
//             return deal1(v);
//         }},
//         ),DEFAULT(<add2>{[](int v){
//             return deal_(v);
//     }}))(which, 1);
// };



void more5() {

    cout << "\ncasearray 0:\n";
    for(auto i = 0; i < 5; i++) {
        cout << "find:" << i << "\t= " << switchfunc0(i) << std::endl;
    }
    cout << "\ncasearray 1:\n";
    for(auto i = 0; i < 5; i++) {
        cout << "find:" << i << "\t= " << switchfunc1(i) << std::endl;
    }
    // cout << "\ncasearray 2:\n";
    // for(auto i = 0; i < 5; i++) {
    //     cout << "find:" << i << "\t= " << switchfunc2(i) << std::endl;
    // }

    cout << "haha1";
}



























int main(int argc,char* argv[])
{
    more5();
    return 0;
}
