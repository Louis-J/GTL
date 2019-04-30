# Template Switch 0.1
作为一个与时俱进，不断更新的追求潮流的语言，c++却有一个跟不上时代的，却又重要的语法：switch case。java中switch可用于string，lisp类的能做模式匹配[^rust][^java12]，但c++的switch依旧只是语句，只能对内置的类型使用。因此，本文在c++ 17的新特性下，通过编译期建表和模板化，实现一个高性能的、可用于非基础类型的Switch表达式。

## 功能描述
1. 实现了替代原有switch case的功能，且几乎没有抽象开销
2. 作为表达式而不是语句使用，因此可以作为返回值
3. 实现了类型安全，支持自动类型推导，若case的类型不匹配可以编译期检测并阻止编译
4. 实现了case值的逻辑安全，case可乱序填入并自动排序，case值不可重复，若有重复可于编译期检测


## 语言特性switch功能分析

switch设计的本意，用于对同一值在不同情况下进行高性能的分支处理。相较于if else的顺序比较，switch使用跳转表(O(1))或查找表(O(log n))来降低分支判断的复杂度。由于需要建表操作，因此switch用于编译期可建表的情况。总结下来，switch用于实现高性能的编译期可建表的多分支处理。而c语言没有对运算符和类的支持，因此也只能用几个基础类型。而由于沿用c语言的处理，虽然c++能重载比较运算符，但对于switch依然没有改进。

经过对gcc、clang汇编代码的分析，发现：当case的值较为集中，跨度较小时，switch使用跳转表进行处理，在函数中设置了多个地址之间差值相同的标记点，作为跳转点进行跳转；当case的值较为分散，跨度较大时，使用二分查找，查找后运行对应的部分。

java的switch可以支持字符串的switch，使用的方法是编译期通过hashcode对字符串算出哈希建表，运行期查表后再使用一次准确比较来确定分支情况。对于哈希冲突则先哈希查表，再对冲突的几个值依次顺序查找。以此可以实现字符串的switch

经过分析，对整体的大致设计分析为：
1. 完成编译期对输入的分支表排序和识别重复、类型匹配判断，保证逻辑正确
2. 完成编译期建表功能，包括跳转表(O(1))和查找表(O(log n))，跳转表使用数组实现，查找表使用二分查找的方式实现
3. 通过封装一个比较类，实现hashcode、准确比较、和确定分支的功能


## 模板Switch的设计实现
### 0. 整体设计
整体设计如下

|模块|Common|SwitchJ|SwitchB|SwitchC(未完)|
|:-:|:-:|:-:|:-:|:-:|
|功能|提供基本的数据类型Case和Default;|根据排序好的分支表建立O(1)的跳转表|根据排序好的分支表建立(O(log n))的查找表|对类建立查找表(未完)|
||提供编译期排序输出分支表功能，并判断是否有重复;|校验分支表是否重复、分支类型是否相同|校验分支表是否重复、分支类型是否相同||

### 1. 编译期预处理case数据
第一步是编译期能预处理case数据。编译期要处理的包括：数据存储、数据排序并检查重复
+ 数据存储：
    对于单对的CaseKey/CaseDeal使用Case结构体存储。如下：
    ```c++
    template <class TypeC, class TypeD>
    struct Case {
        TypeC key;
        TypeD deal;
    }
    ```
    通过使用c++提供的constexpr关键字，可以将比较、交换等处理在编译期实现
    ```c++
    friend constexpr bool operator< (Case<TypeC, TypeD> &&lhs, Case<TypeC, TypeD> &&rhs)  {
        return lhs.key < rhs.key;
    }

    friend constexpr void iter_swap(Case<TypeC, TypeD> &&lhs, Case<TypeC, TypeD> &&rhs) {
        Case<TypeC, TypeD> tmp = std::move(lhs);
        lhs = std::move(rhs);
        rhs = std::move(tmp);
    }
    ```
+ 数据排序：
    最简单的快排，并输出是否有重复的Case
    ```c++
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
    ...
    ```

### 2. 构造、存储和传递Case表数据
需要解决几个问题：1、如何使用易读的格式 2、在编译期传递字面值数据 3、如何构造初始表 4、对预处理后的数据做合法性检查
+ 如何在编译期传递字面值数据：
    对于一个constexpr函数，编译器不会将参数当做编译期已知的字面值处理，需要做点手脚
    声明及存储：
    ```c++
    #define CASES(...) []{                                        \
        auto arr = std::array{__VA_ARGS__};                       \
        bool repeatKey = CaseSort(std::begin(arr), std::end(arr));\
        return std::tuple{repeatKey, arr};                        \
    }

    #define DEFAULT(...) []{                                      \
        return Default __VA_ARGS__;                               \
    }
    ```
    传递：
    ```c++
    return SwitchJ(n, CASES(
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
    ```
    取用：
    ```c++
    template <class CASES, class DFT>
    constexpr auto makePredicJ(CASES cases, DFT dft) {
        constexpr auto Rcases = cases();
        constexpr auto Rdft = dft();
        constexpr auto arr = std::get<1>(Rcases);
        using TypeC = decltype(arr[0].key);
        using TypeD = decltype(arr[0].deal);
    ```
+ 数据合法性检查及断言
    CASES使用array保证所有的case类型相等，因此只需检查case的key和default类型相同，还有CASES中没有重复键值即可
    ```c++
    template <class CASES, class DFT>
    constexpr auto makePredicJ(CASES cases, DFT dft) {
        constexpr auto Rcases = cases();
        constexpr auto Rdft = dft();
        constexpr auto arr = std::get<1>(Rcases);
        using TypeC = decltype(arr[0].key);
        using TypeD = decltype(arr[0].deal);

        static_assert(TypeEqual(arr[0].deal, Rdft.deal), "default类型不匹配！");
        static_assert(std::get<0>(Rcases) == false, "键值有重复！");
    ```
### 3. 构造跳转表
需要构造出O(1)的跳转表，上文已述可用数组构造，除此外还需要封装对应的检查跳转方法，和为便于调用而进行外层的封装
+ 跳转表封装为PredicJ类
    代码如下，封装了operator()用于传入外部参数进行跳转
    ```c++
    template <std::size_t length, class TypeC, class TypeD, TypeC offset>
    struct PredicJ {
        std::array<TypeD, length> seq;
        TypeD dft;

        template <typename TypeV>
        decltype(dft) operator()(TypeV &&val) const {
            if (val - offset < 0 || val - offset >= length) {
                return dft;
            } else {
                return seq[val - offset];
            }
        }
    };
    ```
+ 外层的封装
    makePredicJ用于合法性检查并构建跳转表，将分支表和默认值按位填入跳转表
    ```c++
    template <class CASES>
    constexpr auto makePredicJ(CASES cases) {
        constexpr auto Rcases = cases();
        static_assert(std::get<0>(Rcases) == false, "键值有重复！");
        constexpr auto arr = std::get<1>(Rcases);
        using TypeC = decltype(arr[0].key);
        using TypeD = decltype(arr[0].deal);
        constexpr auto _N = arr.size();
        constexpr auto offset = arr[0].key;
        constexpr std::size_t length =  arr[_N-1].key - arr[0].key + 1;
        
        return PredicJ<length, TypeC, TypeD, offset> {
            [&](){
                std::array<TypeD, length> pat{};
                for(auto c : arr) {
                    pat[c.key - offset] = c.deal;
                }
                return pat;
            }(),
            TypeD(),
        };
    };
    ```
    g++下不使用静态变量则有性能损失，需要二次封装(clang无此问题)
    ```c++
    template <class INPUT, class CASES>
    inline auto SwitchJ(INPUT inp, CASES cases) {
        static constexpr auto predic = detail::makePredicJ(cases);
        return predic(inp);
    };
    ```
    经过测试，一百亿次跳转下，gcc下与原生switch性能相差16倍，但耗时仅2.8s，clang下耗时约1s，此时clang原生switch耗时29s
    由于百亿次跳转几乎不出现于实际场景，因此与原生跳转表性能可以忽略不计
### 4. 构造查找表
需要构造出O(logn)的查找表。可有多种方法。
+ 模仿PredicJ类的方法，封装为一个类，存储排序好的分支表，并使用循环二分查找法进行查找
    ```c++
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
    }
    ```
    经过测试，查找花费时间约为switch本身2~3倍左右
    仔细思考可以去掉循环，编译期动态生成优化当前数据的查找树，提高查找速度
+ 根据当前的分支表，编译期动态生成一个针对当前数据优化的查找函数
    使用递归的方法生成lambda查找函数，查找函数本身即调用下一阶的查找函数
    ```c++
    template <size_t shs = 0, size_t off = 0, typename Xarr, typename Xdft>
    constexpr auto PredicB2Real(Xarr a, Xdft d) {
        constexpr auto arr = std::get<1>(a());
        constexpr auto dft = d();
        constexpr auto N = off;

        using TypeD = decltype(dft);

        if constexpr (N == 0) {
            // 默认查找区段长度为0，说明是外部调用。设长度为arr.size()
            constexpr auto cmpinner = PredicB2Real<0, arr.size()>(a, d);
            return [=](int val) ->TypeD {
                if (val < arr[0].key || val > arr[arr.size() - 1].key) {
                    return dft;
                } else {
                    return cmpinner(val);
                }
            };
        } else if constexpr (N == 2) {
            // 查找区段长度为2，返回判断与此两值是否相等的lambda
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
            // 查找区段长度为1，返回判断与此值是否相等的lambda
            return [=](int val) ->TypeD {
                if (val == arr[shs].key) {
                    return arr[shs].deal;
                } else {
                    return dft;
                }
            };
        } else {
            // 查找区段长度大于等于3，返回判断左边二分和右边二分的lambda
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
    ```
    经过测试，查找花费时间提高40%左右
    再次思考，部分数据二分时不会有不等情况，比如三个相差为1的数做二分，中间可以省去好几次判断相等，省去走默认值的分支
+ 根据当前的分支表，编译期动态生成一个针对当前数据优化的查找函数
    ```c++
    template <size_t shs = 0, size_t off = 0, bool lbrd = false, bool rbrd = false, typename Xarr, typename Xdft>
    constexpr auto PredicB3Real(Xarr a, Xdft d) {
    ...
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
    ...
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
    ```
    经过测试，查找花费时间又有较大提高，gcc下比原生switch快20%左右，clang下比原生switch慢30%左右，具体原因暂不清楚
### 5. 函数case处理
原生switch本身在case处即要处理语句，这也是switch最常用的功能，因此也需要支持对不同的分支使用不同的语句。这部分用lambda实现
    ```c++
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
    ```
    如此处理可以正常运行。但是需要注意：
    1. 没有捕获列表的lambda可以作为普通函数指针使用，但有捕获列表的lambda表现与类成员函数相似，而匿名函数的类型均不一样，因此无法使用。只能通过传入参数的方式规避
    2. 由于函数指针的默认值为nullptr，即0值，因此若对Switch使用函数则必须要指定默认值，不然返回值有可能为nullptr，易造成调用nullptr情况，导致程序崩溃
    3. c++20明确支持constexpr虚函数，而std::function即使用虚函数统一包装真正的函数，因此c++20可能可以使用某种方式传入有捕获列表的lambda使用
### 6. 缺陷
1. 依赖于编译期的编译期递归深度，因此分支过多且乱序时会失败(100个左右乱序case会失败)
2. 对分支h内函数语句处理不够友善，不能使用return, continue, break语句
3. 暂时无法使用类型推倒，也
### 7. 附录：性能对比测试数据
    环境：桌面云4核16g win7，gcc 8.2，clang 7.0.1
    (gcc 7.1 7.2 8.1各有bug，导致SwitchB3无法编译或执行出错)
    1. 1000000*100  次循环下，gcc:SwitchB3耗时442ms，原生耗时544ms，clang:SwitchB3耗时731ms，原生耗时449ms
    2. 100000000*100次循环下，gcc:SwitchJ耗时5832ms，原生耗时323ms，clang:SwitchJ若分支从0开始耗时0ms，不从0开始耗时1232ms，原生耗时33010ms
    其他数据可自行编译算出

[^rust]: [pattern match in rust](https://doc.rust-lang.org/book/ch18-03-pattern-syntax.html)
[^java12]: [switch expression in java12](http://openjdk.java.net/jeps/325)