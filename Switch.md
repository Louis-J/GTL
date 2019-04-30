# Template Switch 0.1
��Ϊһ����ʱ��������ϸ��µ�׷���������ԣ�c++ȴ��һ��������ʱ���ģ�ȴ����Ҫ���﷨��switch case��java��switch������string��lisp�������ģʽƥ��[^rust][^java12]����c++��switch����ֻ����䣬ֻ�ܶ����õ�����ʹ�á���ˣ�������c++ 17���������£�ͨ�������ڽ����ģ�廯��ʵ��һ�������ܵġ������ڷǻ������͵�Switch���ʽ��

## ��������
1. ʵ�������ԭ��switch case�Ĺ��ܣ��Ҽ���û�г�����
2. ��Ϊ���ʽ���������ʹ�ã���˿�����Ϊ����ֵ
3. ʵ�������Ͱ�ȫ��֧���Զ������Ƶ�����case�����Ͳ�ƥ����Ա����ڼ�Ⲣ��ֹ����
4. ʵ����caseֵ���߼���ȫ��case���������벢�Զ�����caseֵ�����ظ��������ظ����ڱ����ڼ��


## ��������switch���ܷ���

switch��Ƶı��⣬���ڶ�ͬһֵ�ڲ�ͬ����½��и����ܵķ�֧���������if else��˳��Ƚϣ�switchʹ����ת��(O(1))����ұ�(O(log n))�����ͷ�֧�жϵĸ��Ӷȡ�������Ҫ������������switch���ڱ����ڿɽ����������ܽ�������switch����ʵ�ָ����ܵı����ڿɽ���Ķ��֧������c����û�ж�����������֧�֣����Ҳֻ���ü����������͡�����������c���ԵĴ�����Ȼc++�����رȽ��������������switch��Ȼû�иĽ���

������gcc��clang������ķ��������֣���case��ֵ��Ϊ���У���Ƚ�Сʱ��switchʹ����ת����д����ں����������˶����ַ֮���ֵ��ͬ�ı�ǵ㣬��Ϊ��ת�������ת����case��ֵ��Ϊ��ɢ����Ƚϴ�ʱ��ʹ�ö��ֲ��ң����Һ����ж�Ӧ�Ĳ��֡�

java��switch����֧���ַ�����switch��ʹ�õķ����Ǳ�����ͨ��hashcode���ַ��������ϣ���������ڲ�����ʹ��һ��׼ȷ�Ƚ���ȷ����֧��������ڹ�ϣ��ͻ���ȹ�ϣ����ٶԳ�ͻ�ļ���ֵ����˳����ҡ��Դ˿���ʵ���ַ�����switch

����������������Ĵ�����Ʒ���Ϊ��
1. ��ɱ����ڶ�����ķ�֧�������ʶ���ظ�������ƥ���жϣ���֤�߼���ȷ
2. ��ɱ����ڽ����ܣ�������ת��(O(1))�Ͳ��ұ�(O(log n))����ת��ʹ������ʵ�֣����ұ�ʹ�ö��ֲ��ҵķ�ʽʵ��
3. ͨ����װһ���Ƚ��࣬ʵ��hashcode��׼ȷ�Ƚϡ���ȷ����֧�Ĺ���


## ģ��Switch�����ʵ��
### 0. �������
�����������

|ģ��|Common|SwitchJ|SwitchB|SwitchC(δ��)|
|:-:|:-:|:-:|:-:|:-:|
|����|�ṩ��������������Case��Default;|��������õķ�֧����O(1)����ת��|��������õķ�֧����(O(log n))�Ĳ��ұ�|���ཨ�����ұ�(δ��)|
||�ṩ���������������֧���ܣ����ж��Ƿ����ظ�;|У���֧���Ƿ��ظ�����֧�����Ƿ���ͬ|У���֧���Ƿ��ظ�����֧�����Ƿ���ͬ||

### 1. ������Ԥ����case����
��һ���Ǳ�������Ԥ����case���ݡ�������Ҫ����İ��������ݴ洢���������򲢼���ظ�
+ ���ݴ洢��
    ���ڵ��Ե�CaseKey/CaseDealʹ��Case�ṹ��洢�����£�
    ```c++
    template <class TypeC, class TypeD>
    struct Case {
        TypeC key;
        TypeD deal;
    }
    ```
    ͨ��ʹ��c++�ṩ��constexpr�ؼ��֣����Խ��Ƚϡ������ȴ����ڱ�����ʵ��
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
+ ��������
    ��򵥵Ŀ��ţ�������Ƿ����ظ���Case
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

### 2. ���졢�洢�ʹ���Case������
��Ҫ����������⣺1�����ʹ���׶��ĸ�ʽ 2���ڱ����ڴ�������ֵ���� 3����ι����ʼ�� 4����Ԥ�������������Ϸ��Լ��
+ ����ڱ����ڴ�������ֵ���ݣ�
    ����һ��constexpr���������������Ὣ����������������֪������ֵ������Ҫ�����ֽ�
    �������洢��
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
    ���ݣ�
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
    ȡ�ã�
    ```c++
    template <class CASES, class DFT>
    constexpr auto makePredicJ(CASES cases, DFT dft) {
        constexpr auto Rcases = cases();
        constexpr auto Rdft = dft();
        constexpr auto arr = std::get<1>(Rcases);
        using TypeC = decltype(arr[0].key);
        using TypeD = decltype(arr[0].deal);
    ```
+ ���ݺϷ��Լ�鼰����
    CASESʹ��array��֤���е�case������ȣ����ֻ����case��key��default������ͬ������CASES��û���ظ���ֵ����
    ```c++
    template <class CASES, class DFT>
    constexpr auto makePredicJ(CASES cases, DFT dft) {
        constexpr auto Rcases = cases();
        constexpr auto Rdft = dft();
        constexpr auto arr = std::get<1>(Rcases);
        using TypeC = decltype(arr[0].key);
        using TypeD = decltype(arr[0].deal);

        static_assert(TypeEqual(arr[0].deal, Rdft.deal), "default���Ͳ�ƥ�䣡");
        static_assert(std::get<0>(Rcases) == false, "��ֵ���ظ���");
    ```
### 3. ������ת��
��Ҫ�����O(1)����ת�����������������鹹�죬�����⻹��Ҫ��װ��Ӧ�ļ����ת��������Ϊ���ڵ��ö��������ķ�װ
+ ��ת���װΪPredicJ��
    �������£���װ��operator()���ڴ����ⲿ����������ת
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
+ ���ķ�װ
    makePredicJ���ںϷ��Լ�鲢������ת������֧���Ĭ��ֵ��λ������ת��
    ```c++
    template <class CASES>
    constexpr auto makePredicJ(CASES cases) {
        constexpr auto Rcases = cases();
        static_assert(std::get<0>(Rcases) == false, "��ֵ���ظ���");
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
    g++�²�ʹ�þ�̬��������������ʧ����Ҫ���η�װ(clang�޴�����)
    ```c++
    template <class INPUT, class CASES>
    inline auto SwitchJ(INPUT inp, CASES cases) {
        static constexpr auto predic = detail::makePredicJ(cases);
        return predic(inp);
    };
    ```
    �������ԣ�һ���ڴ���ת�£�gcc����ԭ��switch�������16��������ʱ��2.8s��clang�º�ʱԼ1s����ʱclangԭ��switch��ʱ29s
    ���ڰ��ڴ���ת������������ʵ�ʳ����������ԭ����ת�����ܿ��Ժ��Բ���
### 4. ������ұ�
��Ҫ�����O(logn)�Ĳ��ұ����ж��ַ�����
+ ģ��PredicJ��ķ�������װΪһ���࣬�洢����õķ�֧����ʹ��ѭ�����ֲ��ҷ����в���
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
    �������ԣ����һ���ʱ��ԼΪswitch����2~3������
    ��ϸ˼������ȥ��ѭ���������ڶ�̬�����Ż���ǰ���ݵĲ���������߲����ٶ�
+ ���ݵ�ǰ�ķ�֧�������ڶ�̬����һ����Ե�ǰ�����Ż��Ĳ��Һ���
    ʹ�õݹ�ķ�������lambda���Һ��������Һ�������������һ�׵Ĳ��Һ���
    ```c++
    template <size_t shs = 0, size_t off = 0, typename Xarr, typename Xdft>
    constexpr auto PredicB2Real(Xarr a, Xdft d) {
        constexpr auto arr = std::get<1>(a());
        constexpr auto dft = d();
        constexpr auto N = off;

        using TypeD = decltype(dft);

        if constexpr (N == 0) {
            // Ĭ�ϲ������γ���Ϊ0��˵�����ⲿ���á��賤��Ϊarr.size()
            constexpr auto cmpinner = PredicB2Real<0, arr.size()>(a, d);
            return [=](int val) ->TypeD {
                if (val < arr[0].key || val > arr[arr.size() - 1].key) {
                    return dft;
                } else {
                    return cmpinner(val);
                }
            };
        } else if constexpr (N == 2) {
            // �������γ���Ϊ2�������ж������ֵ�Ƿ���ȵ�lambda
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
            // �������γ���Ϊ1�������ж����ֵ�Ƿ���ȵ�lambda
            return [=](int val) ->TypeD {
                if (val == arr[shs].key) {
                    return arr[shs].deal;
                } else {
                    return dft;
                }
            };
        } else {
            // �������γ��ȴ��ڵ���3�������ж���߶��ֺ��ұ߶��ֵ�lambda
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
    �������ԣ����һ���ʱ�����40%����
    �ٴ�˼�����������ݶ���ʱ�����в�������������������Ϊ1���������֣��м����ʡȥ�ü����ж���ȣ�ʡȥ��Ĭ��ֵ�ķ�֧
+ ���ݵ�ǰ�ķ�֧�������ڶ�̬����һ����Ե�ǰ�����Ż��Ĳ��Һ���
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
    �������ԣ����һ���ʱ�����нϴ���ߣ�gcc�±�ԭ��switch��20%���ң�clang�±�ԭ��switch��30%���ң�����ԭ���ݲ����
### 5. ����case����
ԭ��switch������case����Ҫ������䣬��Ҳ��switch��õĹ��ܣ����Ҳ��Ҫ֧�ֶԲ�ͬ�ķ�֧ʹ�ò�ͬ����䡣�ⲿ����lambdaʵ��
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
    ��˴�������������С�������Ҫע�⣺
    1. û�в����б��lambda������Ϊ��ͨ����ָ��ʹ�ã����в����б��lambda���������Ա�������ƣ����������������;���һ��������޷�ʹ�á�ֻ��ͨ����������ķ�ʽ���
    2. ���ں���ָ���Ĭ��ֵΪnullptr����0ֵ���������Switchʹ�ú��������Ҫָ��Ĭ��ֵ����Ȼ����ֵ�п���Ϊnullptr������ɵ���nullptr��������³������
    3. c++20��ȷ֧��constexpr�麯������std::function��ʹ���麯��ͳһ��װ�����ĺ��������c++20���ܿ���ʹ��ĳ�ַ�ʽ�����в����б��lambdaʹ��
### 6. ȱ��
1. �����ڱ����ڵı����ڵݹ���ȣ���˷�֧����������ʱ��ʧ��(100����������case��ʧ��)
2. �Է�֧h�ں�����䴦�������ƣ�����ʹ��return, continue, break���
3. ��ʱ�޷�ʹ�������Ƶ���Ҳ
### 7. ��¼�����ܶԱȲ�������
    ������������4��16g win7��gcc 8.2��clang 7.0.1
    (gcc 7.1 7.2 8.1����bug������SwitchB3�޷������ִ�г���)
    1. 1000000*100  ��ѭ���£�gcc:SwitchB3��ʱ442ms��ԭ����ʱ544ms��clang:SwitchB3��ʱ731ms��ԭ����ʱ449ms
    2. 100000000*100��ѭ���£�gcc:SwitchJ��ʱ5832ms��ԭ����ʱ323ms��clang:SwitchJ����֧��0��ʼ��ʱ0ms������0��ʼ��ʱ1232ms��ԭ����ʱ33010ms
    �������ݿ����б������

[^rust]: [pattern match in rust](https://doc.rust-lang.org/book/ch18-03-pattern-syntax.html)
[^java12]: [switch expression in java12](http://openjdk.java.net/jeps/325)