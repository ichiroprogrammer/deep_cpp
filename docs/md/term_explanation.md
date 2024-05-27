<!-- ./md/term_explanation.md -->
# 用語解説 <a id="SS_5"></a>

この章では、このドキュメントで使用する用語の解説をする。

## 型とインスタンス <a id="SS_5_1"></a>

### 算術型 <a id="SS_5_1_1"></a>
算術型とは下記の型の総称である。

* [汎整数型](term_explanation.md#SS_5_1_2)(bool, char, int, unsigned int, long long等)
* 浮動小数点型(float、double、long double)

算術型のサイズは下記のように規定されている。

* 1 == sizeof(bool) == sizeof(char)
* sizeof(char) <= sizeof(short) <= sizeof(int) <= sizeof(long) <= sizeof(long long)
* 4 <= sizeof(long)
* 8 <= sizeof(long long)
* 4 == sizeof(float)
* 8 == sizeof(double) <= sizeof(long double)

### 汎整数型 <a id="SS_5_1_2"></a>
汎整数型とは下記の型の総称である。

* 論理型(bool)
* 文字型(char、wchar_t等)
* [整数型](term_explanation.md#SS_5_1_3)(int、unsigned int、long等)

### 整数型 <a id="SS_5_1_3"></a>
整数型とは下記の型の総称である。

* char
* signed char
* unsigned char
* short
* unsigned short
* int
* unsigned int
* long
* unsigned long
* long long
* unsigned long long

### 算術変換 <a id="SS_5_1_4"></a>
C++における算術変換とは、算術演算の1つのオペランドが他のオペランドと同じ型でない場合、
1つのオペランドを他のオペランドと同じ型に変換するプロセスのことを指す。

算術変換は、[汎整数拡張](term_explanation.md#SS_5_1_5)と通常算術変換に分けられる。

```cpp
    // @@@ example/term_explanation/integral_promotion_ut.cpp 11

    bool           bval{};
    char           cval{};
    short          sval{};
    unsigned short usval{};
    int            ival{};
    unsigned int   uival{};
    long           lval{};
    unsigned long  ulval{};
    float          fval{};
    double         dval{};

    auto ret_0 = 3.14159 + 'a';  // 'a'は汎整数拡張でintになった後、さらに通常算術変換でdoubleに
    static_assert(std::is_same<decltype(ret_0), double>::value, "");

    auto ret_1 = dval + ival;  // ivalは通常算術変換でdoubleに
    static_assert(std::is_same<decltype(ret_1), double>::value, "");

    auto ret_2 = dval + fval;  // fvalは通常算術変換でdoubleに
    static_assert(std::is_same<decltype(ret_2), double>::value, "");

    auto ret_3 = ival = dval;  // dvalは通常算術変換でintに
    static_assert(std::is_same<decltype(ret_3), int>::value, "");

    bval = dval;  // dvalは通常算術変換でboolに
    ASSERT_FALSE(bval);

    auto ret_4 = cval + fval;  // cvalは汎整数拡張でintになった後、さらに通常算術変換でfloatに
    static_assert(std::is_same<decltype(ret_4), float>::value, "");

    auto ret_5 = sval + cval;  // svalとcvalは汎整数拡張でintに
    static_assert(std::is_same<decltype(ret_5), int>::value, "");

    auto ret_6 = cval + lval;  // cvalはは汎整数拡張でintになった後、通常算術変換でlongに
    static_assert(std::is_same<decltype(ret_6), long>::value, "");

    auto ret_7 = ival + ulval;  // ivalは通常算術変換でunsigned longに
    static_assert(std::is_same<decltype(ret_7), unsigned long>::value, "");

    auto ret_8 = usval + ival;  // usvalは汎整数拡張でintに
                                // ただし、この変換はunsigned shortとintのサイズに依存する
    static_assert(std::is_same<decltype(ret_8), int>::value, "");

    auto ret_9 = uival + lval;  // uivalは通常算術変換でlongに
                                // ただし、この変換はunsigned intとlongのサイズに依存する
    static_assert(std::is_same<decltype(ret_9), long>::value, "");
```

一様初期を使用することで、
変数定義時の算術変換による意図しない値の変換(縮小型変換)を防ぐことができる。

```cpp
    // @@@ example/term_explanation/integral_promotion_ut.cpp 62

    int i{-1};
    // int8_t i8 {i};  縮小型変換によりコンパイル不可
    int8_t i8 = i;  // intからint8_tへの型変換
    // これには問題ないが

    ASSERT_EQ(-1, i8);

    // uint8_t ui8 {i};  縮小型変換によりコンパイル不可
    uint8_t ui8 = i;  // intからuint8_tへの型変換
    // おそらく意図通りではない

    ASSERT_EQ(255, ui8);
```

以下に示すように、算術変換の結果は直感に反することがあるため、注意が必要である。

```cpp
    // @@@ example/term_explanation/integral_promotion_ut.cpp 81

    int          i{-1};
    unsigned int ui{1};

    // ASSERT_TRUE(i < ui);
    ASSERT_TRUE(i > ui);  // 算術変換の影響で、-1 < 1が成立しない

    signed short   s{-1};
    unsigned short us{1};

    ASSERT_TRUE(s < us);  // 汎整数拡張により、-1 < 1が成立
```

### 汎整数拡張 <a id="SS_5_1_5"></a>
bool、char、signed char、unsigned char、short、unsigned short型の変数が、
算術のオペランドとして使用される場合、

* その変数の型の取り得る値全てがintで表現できるのならば、int型に変換される。
* そうでなければ、その変数はunsigned int型に変換される。

この変換を汎整数拡張と呼ぶ。

従って、sizof(short) < sizeof(int)である処理系では、
bool、char、signed char、unsigned char、short、unsigned short型の変数は、
下記のようにintに変換される。

```cpp
    // @@@ example/term_explanation/integral_promotion_ut.cpp 100

    bool bval;
    static_assert(std::is_same<int, decltype(bval + bval)>::value, "");

    char cval;
    static_assert(std::is_same<int, decltype(cval + cval)>::value, "");

    unsigned char ucval = 128;
    static_assert(std::is_same<int, decltype(ucval + ucval)>::value, "");
    ASSERT_EQ(256, ucval + ucval);  // 汎整数拡張により256になる

    static_assert(std::is_same<int, decltype(cval + ucval)>::value, "");

    short sval;
    static_assert(std::is_same<int, decltype(sval + sval)>::value, "");

    unsigned short usval;
    static_assert(std::is_same<int, decltype(usval + usval)>::value, "");

    static_assert(std::is_same<int, decltype(sval + usval)>::value, "");
```

### POD <a id="SS_5_1_6"></a>
PODとは、 Plain Old Dataの略語であり、

```cpp
    std::is_pod<T>::value
```

がtrueとなる型Tを指す。下記のコードはその使用例である。

```cpp
    // @@@ example/term_explanation/pod_ut.cpp 7

    static_assert(std::is_pod<int>::value, "");
    static_assert(std::is_pod<int const>::value, "");
    static_assert(std::is_pod<int*>::value, "");
    static_assert(std::is_pod<int[3]>::value, "");
    static_assert(!std::is_pod<int&>::value, "");  // リファレンスはPODではない

    struct Pod {};

    static_assert(std::is_pod<Pod>::value, "");
    static_assert(std::is_pod<Pod const>::value, "");
    static_assert(std::is_pod<Pod*>::value, "");
    static_assert(std::is_pod<Pod[3]>::value, "");
    static_assert(!std::is_pod<Pod&>::value, "");

    struct NonPod {  // コンストラクタがあるためPODではない
        NonPod();
    };

    static_assert(!std::is_pod<NonPod>::value, "");
```

概ね、C言語と互換性のある型を指すと思って良い。

「型が[トリビアル型](term_explanation.md#SS_5_1_8)且つ[標準レイアウト型](term_explanation.md#SS_5_1_7)であること」と
「型が[POD](term_explanation.md#SS_5_1_6)であること」は等価であるため、C++20では、
[PODという用語は非推奨](https://cpprefjp.github.io/lang/cpp20/deprecate_pod.html)となった。

### 標準レイアウト型 <a id="SS_5_1_7"></a>
標準レイアウト型とは、

```cpp
    std::is_standard_layout<T>::value
```

がtrueとなる型Tを指す。下記のコードはその使用例である。

```cpp
    // @@@ example/term_explanation/pod_ut.cpp 31

    static_assert(std::is_standard_layout<int>::value, "");
    static_assert(std::is_standard_layout<int*>::value, "");
    static_assert(std::is_standard_layout<int[1]>::value, "");
    static_assert(!std::is_standard_layout<int&>::value, "");

    enum class SizeUndefined { su_0, su_1 };

    struct StanderdLayout {  // 標準レイアウトだがトリビアルではない
        StanderdLayout() : a{0}, b{SizeUndefined::su_0} {}
        int           a;
        SizeUndefined b;
    };

    static_assert(std::is_standard_layout<StanderdLayout>::value, "");
    static_assert(!std::is_trivial<StanderdLayout>::value, "");
    static_assert(!std::is_pod<StanderdLayout>::value, "");
```

型がPODである場合、その型は標準レイアウト型である。

### トリビアル型 <a id="SS_5_1_8"></a>
トリビアル型とは、

```cpp
    std::is_trivial<T>::value
```

がtrueとなる型Tを指す。下記のコードはその使用例である。

```cpp
    // @@@ example/term_explanation/pod_ut.cpp 52

    static_assert(std::is_trivial<int>::value, "");
    static_assert(std::is_trivial<int*>::value, "");
    static_assert(std::is_trivial<int[1]>::value, "");
    static_assert(!std::is_trivial<int&>::value, "");

    enum class SizeUndefined { su_0, su_1 };

    struct Trivial {      // トリビアルだが標準レイアウトではない
        int&          a;  // リファレンスは標準レイアウトではない
        SizeUndefined b;
    };

    static_assert(!std::is_standard_layout<Trivial>::value, "");
    static_assert(std::is_trivial<Trivial>::value, "");
    static_assert(!std::is_pod<Trivial>::value, "");
```

型がPODである場合、その型はトリビアル型である。

### underlying type <a id="SS_5_1_9"></a>
underlying typeとは、enumやenum classの汎整数表現を指定できるようにするために、
C++11で導入されたシンタックスである。

```cpp
    // @@@ example/term_explanation/underlying_type_ut.cpp 9

    // 従来のenum
    enum NormalEnum {
        ...
    };

    // enum underlying typeがint8_tに指定された従来のenum
    enum NormalEnumWithUnderlyingType : int8_t {
        ...
    };

    // enum class
    enum class EnumClass {
        ...
    };

    // enum underlying typeがint64_tに指定されたenum class
    enum class EnumClassWithUnderlyingType : int64_t {
        ...
    };
```

```cpp
    // @@@ example/term_explanation/underlying_type_ut.cpp 38

    ASSERT_EQ(4, sizeof(NormalEnum));  // 列挙子の値を表現するのに十分なサイズの整数型で処理系依存

    // NormalEnumWithUnderlyingTypeのunderlying typeはint8_t
    static_assert(std::is_same_v<int8_t, std::underlying_type_t<NormalEnumWithUnderlyingType>>);
    ASSERT_EQ(sizeof(int8_t), sizeof(NormalEnumWithUnderlyingType));

    ASSERT_EQ(4, sizeof(EnumClass));  // 列挙子の値を表現するのに十分なサイズの整数型で処理系依存

    // EnumClassWithUnderlyingTypeのunderlying typeはint64_t
    static_assert(std::is_same_v<int64_t, std::underlying_type_t<EnumClassWithUnderlyingType>>);
    ASSERT_EQ(sizeof(int64_t), sizeof(EnumClassWithUnderlyingType));
```


### 不完全型 <a id="SS_5_1_10"></a>
不完全型とは、型のサイズや構造が不明な型を指す。
下記のような場合、不完全型となる。

```cpp
    // @@@ example/term_explanation/incomplete_type_ut.cpp 6

    class A;  // Aの前方宣言
              // これ以降、Aは不完全型となる

    // auto a = sizeof(A);  Aが不完全型であるため、コンパイルエラー

    class A {  // この宣言により、この行以降はAは完全型になる
    public:
        // 何らかの宣言
    };

    auto a = sizeof(A);  // Aが完全型であるため、コンパイル可能
```

### 完全型 <a id="SS_5_1_11"></a>
[不完全型](term_explanation.md#SS_5_1_10)ではない型を指す。

### ポリモーフィックなクラス <a id="SS_5_1_12"></a>
ポリモーフィックなクラスとは、仮想関数を持つクラスを指す。
なお、純粋仮想関数を持つクラスは、仮想クラスと呼ばれれる。

### インターフェースクラス <a id="SS_5_1_13"></a>
インターフェースクラスとは、純粋仮想関数のみを持つ抽象クラスのことを指す。
インターフェースクラスは、クラスの実装を提供することなく、
クラスのインターフェースを定義するために使用される。
インターフェースクラスは、クラスの仕様を定義するために使用されるため、
多くの場合、抽象基底クラスとして使用される。


```cpp
    // @@@ example/term_explanation/interface_class.cpp 8

    class InterfaceClass {  // インターフェースクラス
    public:
        virtual void DoSomething(int32_t) = 0;
        virtual bool IsXxx() const        = 0;
        virtual ~InterfaceClass()         = 0;
    };

    class NotInterfaceClass {  // メンバ変数があるためインターフェースクラスではない
    public:
        NotInterfaceClass();
        virtual void DoSomething(int32_t) = 0;
        virtual bool IsXxx() const        = 0;
        virtual ~NotInterfaceClass()      = 0;

    private:
        int32_t num_;
    };
```

### constインスタンス <a id="SS_5_1_14"></a>

constインスタンスとはランタイムの初期化時に値が確定し、その後、状態が不変であるインスタンスである。

### constexprインスタンスと関数 <a id="SS_5_1_15"></a>
constexprインスタンスとはコンパイル時に値が確定するインスタンスである。
当然、ランタイム時でも不変である。

```cpp
    // @@@ example/term_explanation/constexpr_ut.cpp 6

    constexpr double PI{3.14159265358979323846};  // PIはconstexpr
```

constexprとして宣言された関数の戻り値がコンパイル時に確定する場合、
その関数の呼び出し式はconstexprと扱われる。
従って、この値はテンプレートパラメータやstatic_assertのオペランドに使用することができる。

```cpp
    // @@@ example/term_explanation/constexpr_ut.cpp 10

    constexpr int f(int a) noexcept { return a * 3; }  // aがconstexprならばf(a)もconstexpr

    int g(int a) noexcept { return a * 3; }  // aがconstexprであってもg(a)は非constexpr

    template <int N>
    struct Templ {
        static constexpr auto value = N;
    };
```
```cpp
    // @@@ example/term_explanation/constexpr_ut.cpp 25

    auto x = int{0};

    constexpr auto a = f(3);  // f(3)はconstexprなのでaの初期化が可能
    // constexpr auto b = f(x);  // xは非constexprなのでbの初期化はコンパイルエラー
    auto const c = f(3);  // cはconstexprとすべき
    // constexpr auto d = g(3);  // g(3)は非constexprなのでdの初期化はコンパイルエラー
    auto const e = g(x);  // eはここで初期化して、この後不変

    constexpr auto pi = PI;  // PIもconstexprなので初期化が可能

    auto templ_a = Templ<a>{};     // aはconstexprなのでaの初期化が可能
    auto templ_f = Templ<f(a)>{};  // f(a)はconstexprなのでaの初期化が可能
    // auto templ_x = Templ<x>{};  // xは非constexprなのでテンプレートパラメータに指定できない

    static_assert(templ_a.value == 9);
    // static_assert(x == 0);  // xは非constexprなのでstatic_assertで使用できない
```

下記のようなリカーシブな関数でも場合によってはconstexprにできる。
これによりこの関数の実行速度は劇的に向上する。

```cpp
    // @@@ example/term_explanation/constexpr_ut.cpp 48

    inline constexpr uint32_t BitMask(uint32_t bit_len) noexcept
    {
        if (bit_len == 0) {
            return 0x0;
        }

        return BitMask(bit_len - 1) | (0x01 << (bit_len - 1));
    }
```

下記の単体テストが示すように、

* 引数がconstexprである場合、上記constexpr関数の戻り値はconstexprになるため、
  static_assertのオペランド式としても利用できる。
* 引数がconstexprでない場合、上記constexpr関数は通常の関数として振舞うため、
  戻り値はconstexprとならない。

```cpp
    // @@@ example/term_explanation/constexpr_ut.cpp 63

    constexpr auto b_0x00000000 = BitMask(0);
    constexpr auto b_0x000000ff = BitMask(8);

    static_assert(b_0x00000000 == 0x00000000);
    static_assert(b_0x000000ff == 0x000000ff);
    static_assert(BitMask(16) == 0x0000'ffff);

    constexpr auto bit_len_constexpr = 24U;
    static_assert(BitMask(bit_len_constexpr) == 0x00ff'ffff);

    auto bit_len = 24U;

    // bit_lenがconstexprでないことによりBitMask(bit_len)もconstexprでないため、
    // コンパイルできない
    // constexpr auto b_0x00ffffff = BitMask(bit_len);

    // b_0x00ffffffの定義からconstexprを外せばコンパイル可能
    // ただし、コンパイル時でなくランタイム時動作になるため動作が遅い
    auto b_0x00ffffff = BitMask(bit_len);

    ASSERT_EQ(b_0x00ffffff, 0x00ff'ffff);
```

下記のようにクラスのコンストラクタをconstexprとすることで、
コンパイル時にリテラルとして振る舞うクラスを定義することができる。

```cpp
    // @@@ example/term_explanation/constexpr_ut.cpp 90

    class Integer {
    public:
        constexpr Integer(int32_t integer) noexcept : integer_{integer} {}
        constexpr int32_t Get() const noexcept { return integer_; }  // constexprメンバ関数はconst
        constexpr int32_t Allways2() const noexcept { return 2; }    // constexprメンバ関数はconst
        static constexpr int32_t Allways3() noexcept { return 3; }   // static関数のconstexpr化

    private:
        int32_t integer_;
    };
```
```cpp
    // @@@ example/term_explanation/constexpr_ut.cpp 107

    constexpr auto int3     = 3;                         // int3はconstexpr
    constexpr auto integer3 = Integer{int3};             // integer3自体がconstexpr
    static_assert(integer3.Get() == 3, "wrong number");  // integer3.Get()もconstexpr

    auto integer4 = Integer{4};
    // integer4は非constexprであるため、integer4.Get()も非constexprとなり、コンパイル不可
    // static_assert(integer4.Get() == 4, "wrong number");

    // integer4は非constexprだが、integer4.Allway2()はconstexprであるため、コンパイル可能
    static_assert(integer4.Allways2() == 2, "wrong number");
```

### ユーザ定義リテラル演算子 <a id="SS_5_1_16"></a>
ユーザ定義リテラル演算子とは以下のようなものである。

```cpp
    // @@@ example/term_explanation/user_defined_literal_ut.cpp 4

    constexpr int32_t one_km = 1000;

    // ユーザ定義リテラル演算子の定義
    constexpr int32_t operator""_kilo_meter(unsigned long long num_by_mk) { return num_by_mk * one_km; }
    constexpr int32_t operator""_meter(unsigned long long num_by_m) { return num_by_m; }
```
```cpp
    // @@@ example/term_explanation/user_defined_literal_ut.cpp 15

    int32_t km = 3_kilo_meter;  // ユーザ定義リテラル演算子の利用
    int32_t m  = 3000_meter;    // ユーザ定義リテラル演算子の利用

    ASSERT_EQ(m, km);
```

### std::string型リテラル <a id="SS_5_1_17"></a>
"xxx"sとすることで、std::string型のリテラルを作ることができる。

```cpp
    // @@@ example/term_explanation/user_defined_literal_ut.cpp 26

    using namespace std::literals::string_literals;

    auto a = "str"s;  // aはstd::string
    auto b = "str";   // bはconst char*

    static_assert(std::is_same_v<decltype(a), std::string>);
    ASSERT_EQ(std::string{"str"}, a);

    static_assert(std::is_same_v<decltype(b), char const*>);
    ASSERT_STREQ("str", b);
```

## オブジェクトと生成 <a id="SS_5_2"></a>

### 特殊メンバ関数 <a id="SS_5_2_1"></a>
特殊メンバ関数とは下記の関数を指す。

* デフォルトコンストラクタ
* copyコンストラクタ
* copy代入演算子
* moveコンストラクタ
* move代入演算子
* デストラクタ

ユーザがこれらを一切定義しない場合、または一部のみを定義する場合、
コンパイラは、下記のテーブル等で示すルールに従い、特殊関数メンバの宣言、定義の状態をを定める。

左1列目がユーザによる各関数の宣言を表し、2列目以降はユーザ宣言の影響による各関数の宣言の状態を表す。  
下記表において、

* 「= default」とは、「コンパイラによってその関数が= defaultと宣言された」状態であることを表す。
* 「~~= default~~」とは、= defaultと同じであるが、バグが発生しやすいので推奨されない。
* 「宣言無し」とは、「コンパイラによってその関数が= defaultと宣言された状態ではない」ことを表す。
    * 「moveコンストラクタが= defaultと宣言された状態ではない」且つ
      「copyコンストラクタが宣言されている」場合、
      rvalueを使用したオブジェクトの初期化には、
      moveコンストラクタの代わりにcopyコンストラクタが使われる。
    * 「move代入演算子が= defaultと宣言された状態ではない」且つ
      「copy代入演算子が宣言されている」場合、
      rvalueを使用したオブジェクトの代入には、
      move代入演算子の代わりにcopy代入演算子が使われる。
* 「= delete」とは「コンパイラによってその関数が= deleteと宣言された」状態であることを表す。

|ユーザによる特殊関数の宣言|デフォルト<br>コンストラクタ |デストラクタ|copy<br>コンストラクタ|copy<br>代入演算子|move<br>コンストラクタ|move<br>代入演算子|
|:------------------------:|:-------------:|:-----------:|:------------:|:------------:|:---------:|:---------:|
|宣言無し                  | = default     | = default   | = default    | = default    | = default | = default |
|非デフォルトコンストラクタ| 宣言なし      | = default   | = default    | = default    | = default | = default |
|デフォルトコンストラクタ  | -             | = default   | = default    | = default    | = default | = default |
|デストラクタ              | = default     | -           | ~~= default~~| ~~= default~~| 宣言なし  | 宣言なし  |
|copyコンストラクタ        | 宣言なし      | = default   | -            | ~~= default~~| 宣言なし  | 宣言なし  |
|copy代入演算子            | = default     | = default   | ~~= default~~| -            | 宣言なし  | 宣言なし  |
|moveコンストラクタ        | 宣言なし      | = default   | = delete     | = delete     | -         | 宣言なし  |
|move代入演算子            | = default     | = default   | = delete     | = delete     | 宣言なし  | -         |


上記表より、下記のようなことがわかる。

* ユーザが上記6メンバ関数を一切宣言しない場合、それらはコンパイラにより暗黙に宣言、定義される。
* ユーザがcopyコンストラクタを宣言した場合、デフォルトコンストラクタは暗黙に宣言、定義されない。
* ユーザがcopyコンストラクタを宣言した場合、copy代入演算子はコンパイラにより暗黙に宣言、
  定義されるが、そのことは推奨されない(~~= default~~は非推奨のdefault宣言を指す)。
* moveコンストラクタ、move代入演算子は、
  以下のいずれもが明示的に宣言されていない場合にのみ暗黙に宣言、定義される。
    * copyコンストラクタ
    * copy代入演算子(operator =)
    * moveコンストラクタ
    * move代入演算子
    * デストラクタ

* ユーザがmoveコンストラクタまたはmove代入演算子を宣言した場合、
  copyコンストラクタ、copy代入演算子は= deleteされる。


### 初期化子リストコンストラクタ <a id="SS_5_2_2"></a>
初期化子リストコンストラクタ(リスト初期化用のコンストラクタ)とは、
{}によるリスト初期化をサポートするためのコンストラクタである。
下記コードでは、 E::E(std::initializer_list\<uint32_t>)が初期化子リストコンストラクタである。

```cpp
    // @@@ example/term_explanation/constructor_ut.cpp 6

    class E {
    public:
        E() : str_{"default constructor"} {}

        // 初期化子リストコンストラクタ
        explicit E(std::initializer_list<uint32_t>) : str_{"initializer list constructor"} {}

        explicit E(uint32_t, uint32_t) : str_{"uint32_t uint32_t constructor"} {}

        std::string const& GetString() const { return str_; }

    private:
        std::string const str_;
    };

    TEST(Constructor, initializer_list_constructor)
    {
        E const e0;
        ASSERT_EQ("default constructor", e0.GetString());

        E const e1{};
        ASSERT_EQ("default constructor", e1.GetString());

        E const e2{3, 4};  // E::E(uint32_t, uint32_t)の呼び出しと区別が困難
        ASSERT_EQ("initializer list constructor", e2.GetString());

        E const e3(3, 4);  // E::E(std::initializer_list<uint32_t>)の呼び出しと区別が困難
        ASSERT_EQ("uint32_t uint32_t constructor", e3.GetString());
    }
```

デフォルトコンストラクタと初期化子リストコンストラクタが、
それぞれに定義されているクラスの初期化時に空の初期化子リストが渡された場合、
デフォルトコンストラクタが呼び出される。

初期化子リストコンストラクタと、
「その初期化子リストの要素型と同じ型の仮引数のみを受け取るコンストラクタ
(上記コードのE::E(uint32_t, uint32_t))」
の両方を持つクラスの初期化時にどちらでも呼び出せる初期化子リストが渡された場合({}を使った呼び出し)、
初期化子コンストラクタが呼び出される。

### 継承コンストラクタ <a id="SS_5_2_3"></a>
継承コンストラクタとは、基底クラスで定義したコンストラクタ群を、
派生クラスのインターフェースとしても使用できるようにするための機能である。
下記コードのように、継承コンストラクタは派生クラス内でusingを用いて宣言される。

```cpp
    // @@@ example/term_explanation/constructor_ut.cpp 40

    class Base {
    public:
        explicit Base(int32_t b) noexcept : b_{b} {}
        virtual ~Base() = default;
        ...
    };

    class Derived : public Base {
    public:
        using Base::Base;  // 継承コンストラクタ
    #if 0
        Derived(int32_t b) : Base{b} {}  // オールドスタイル
    #endif
    };

    void f() noexcept
    {
        Derived d{1};  // Derived::Derived(int32_t)が使える
        ...
    }
```

### 委譲コンストラクタ <a id="SS_5_2_4"></a>
委譲コンストラクタとは、コンストラクタから同じクラスの他のコンストラクタに処理を委譲する機能である。
以下のコード中では、委譲コンストラクタを使い、
A::A(uint32_t)の処理をA::A(std::string const&)へ委譲している。

```cpp
    // @@@ example/term_explanation/constructor_ut.cpp 72

    class A {
    public:
        explicit A(std::string str) : str_{std::move(str)}
        {
            ...
        }

        explicit A(uint32_t num) : A{std::to_string(num)}  // 委譲コンストラクタ
        {
        }

    private:
        std::string str_;
    };
```

### 非explitなコンストラクタによる暗黙の型変換 <a id="SS_5_2_5"></a>
非explitなコンストラクタによる暗黙の型変換とは、

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp 6

    class Person {
    public:
        Person(char const* name, uint32_t age = 0) : name_{name}, age_{age} {}
        Person(Person const&)            = default;
        Person& operator=(Person const&) = default;

        std::string const& GetName() const noexcept { return name_; }
        uint32_t           GetAge() const noexcept { return age_; }

    private:
        std::string name_;  // コピーをするため非const
        uint32_t    age_;
    };

    bool operator==(Person const& lhs, Person const& rhs) noexcept
    {
        return (lhs.GetName() == rhs.GetName()) && (lhs.GetAge() == rhs.GetAge());
    }
```

上記のクラスPersonを使用して、下記のようなコードをコンパイルできるようにする機能である。

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp 27

    void f(Person const& person) noexcept
    {
        ...
    }

    void using_implicit_coversion()
    {
        f("Ohtani");  // "Ohtani"はPerson型ではないが、コンパイル可能
    }
```

この記法は下記コードの短縮形であり、コードの見た目をシンプルに保つ効果がある。

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp 41

    void not_using_implicit_coversion()
    {
        f(Person{"Ohtani"});  // 本来は、fの引数はPerson型
    }
```

この記法は下記のようにstd::string等のSTLでも多用され、その効果は十分に発揮されているものの、

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp 53

    auto otani = std::string{"Ohtani"};

    ...

    if (otani == "Ohtani") {  // 暗黙の型変換によりコンパイルできる
        ...
    }
```

以下のようなコードがコンパイルできてしまうため、わかりづらいバグの元にもなる。

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp 67

    auto otani = Person{"Ohtani", 26};

    ...

    if (otani == "Otani") {  // このコードがコンパイルされる。
        ...
    }
```

下記のようにコンストラクタにexplicitを付けて宣言することにより、この問題を防ぐことができる。

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp 94

    class Person {
    public:
        explicit Person(char const* name, uint32_t age = 0) : name_{name}, age_{age} {}
        Person(Person const&)            = default;
        Person& operator=(Person const&) = default;

        ...
    };

    void prohibit_implicit_coversion()
    {
    #if 0  // explicit付きのコンストラクタを持つPersonと違い、コンパイルできない。
        f("Ohtani");
    #else
        f(Person{"Ohtani"});
    #endif

        auto otani = Person{"Ohtani", 26};

        ...

    #if 0
        if (otani == "Otani") {  // このコードもコンパイルできない。
            ...
        }
    #else
        if (otani == Person{"Otani", 26}) {  // この記述を強制できる。
            ...
        }
    #endif
    }
```

std::stringは暗黙の型変換を許して良く、(多くの場合)Personには暗黙の型変換をしない方が良い理由は、

* std::stringの役割は文字列の管理と演算のみであるため、
  std::stringを文字列リテラルと等価なもののように扱っても違和感がない
* Personは、明らかに文字列リテラルと等価なものではない

といったセマンティクス的観点(「[シンタックス、セマンティクス](term_explanation.md#SS_5_8)」参照)によるものである。

クラスPersonと同様に、
ほとんどのユーザ定義クラスには非explitなコンストラクタによる暗黙の型変換は必要ない。


### NSDMI <a id="SS_5_2_6"></a>
NSDMIとは、non-static data member initializerの略語であり、
下記のような非静的なメンバ変数の初期化子を指す。

```cpp
    // @@@ example/term_explanation/nsdmi.cpp 8

    class A {
    public:
        A() : a_{1}  // NSDMIではなく、非静的なメンバ初期化子による初期化
        {
        }

    private:
        int32_t     a_;
        int32_t     b_ = 0;        // NSDMI
        std::string str_{"init"};  // NSDMI
    };
```

### 一様初期化 <a id="SS_5_2_7"></a>
一様初期化(uniform initialization)とは、
C++11で導入された、コンストラクタの呼び出しをリスト初期化と合わせて{}で記述する構文である。

```cpp
    // @@@ example/term_explanation/uniform_initialization_ut.cpp 12

    struct X {
        X(int) {}
    };

    X x0(0);   // 通常従来のコンストラクタ呼び出し
    X x1 = 0;  // 暗黙の型変換を使用した従来のコンストラクタ呼び出し

    X x2{0};     // 一様初期化
    X x3 = {0};  // 暗黙の型変換を使用した一様初期化

    struct Y {
        Y(int, double, std::string) {}
    };

    auto lamda = [](int, double, std::string) -> Y {
        return {1, 3.14, "hello"};  // 暗黙の型変換を使用した一様初期化でのYの生成
    };
```

変数による一様初期化が縮小型変換を起こす場合や、
リテラルによる一様初期化がその値を変更する場合、コンパイルエラーとなるため、
この機能を積極的に使用することで、縮小型変換による初期化のバグを未然に防ぐことができる。

```cpp
    // @@@ example/term_explanation/uniform_initialization_ut.cpp 34

    int i{0};  // 一様初期化

    bool b0 = 7;  // 縮小型変換のため、b0の値はtrue(通常は1)となる
    ASSERT_EQ(b0, 1);

    // bool b1{7};  // 縮小型変換のため、コンパイルエラー
    // bool b2{i};  // 縮小型変換のため、コンパイルエラー

    uint8_t u8_0 = 256;  // 縮小型変換のためu8_0は0となる
    ASSERT_EQ(u8_0, 0);

    // uint8_t u8_1{256};  // 縮小型変換のため、コンパイルエラー
    // uint8_t u8_2{i};    // 縮小型変換のため、コンパイルエラー

    uint8_t array0[3]{1, 2, 255};  // 一様初期化
    // uint8_t array1[3] = {1, 2, 256};  // 縮小型変換のため、コンパイルエラー
    // uint8_t array2[3]{1, 2, 256};     // 縮小型変換のため、コンパイルエラー
    // uint8_t array2[3]{1, 2, i};       // 縮小型変換のため、コンパイルエラー

    int i0 = 1.0;  // 縮小型変換のため、i0の値は1
    ASSERT_EQ(i0, 1);

    // int i1{1.0};  // 縮小型変換のため、コンパイルエラー

    double d{1};  // 縮小型変換は起こらないのでコンパイル可能
    // int i2{d};  // 縮小型変換のため、コンパイルエラー
```


### AAAスタイル <a id="SS_5_2_8"></a>
このドキュメントでのAAAとは、単体テストのパターンarrange-act-assertではなく、
almost always autoを指し、
AAAスタイルとは、「可能な場合、型を左辺に明示して変数を宣言する代わりに、autoを使用する」
というコーディングスタイルである。
この用語は、Andrei Alexandrescuによって造られ、Herb Sutterによって広く推奨されている。

特定の型を明示して使用する必要がない場合、下記のように書く。

```cpp
    // @@@ example/term_explanation/aaa.cpp 11

    auto i  = 1;
    auto ui = 1U;
    auto d  = 1.0;
    auto s  = "str";
    auto v  = {0, 1, 2};

    for (auto i : v) {
        // 何らかの処理
    }

    auto add = [](auto lhs, auto rhs) {  // -> return_typeのような記述は不要
        return lhs + rhs;                // addの型もautoで良い
    };

    // 上記変数の型の確認
    static_assert(std::is_same_v<decltype(i), int>);
    static_assert(std::is_same_v<decltype(ui), unsigned int>);
    static_assert(std::is_same_v<decltype(d), double>);
    static_assert(std::is_same_v<decltype(s), char const*>);
    static_assert(std::is_same_v<decltype(v), std::initializer_list<int>>);

    char s2[] = "str";  // 配列の宣言には、AAAは使えない
    static_assert(std::is_same_v<decltype(s2), char[4]>);

    int* p0 = nullptr;  // 初期値がnullptrであるポインタの初期化には、AAAは使うべきではない
    auto p1 = static_cast<int*>(nullptr);  // NG
    auto p2 = p0;                          // OK
    auto p3 = nullptr;                     // NG 通常、想定通りにならない
    static_assert(std::is_same_v<decltype(p3), std::nullptr_t>);
```

特定の型を明示して使用する必要がある場合、下記のように書く。

```cpp
    // @@@ example/term_explanation/aaa.cpp 51

    auto b  = new char[10]{0};
    auto v  = std::vector<int>{0, 1, 2};
    auto s  = std::string{"str"};
    auto sv = std::string_view{"str"};

    static_assert(std::is_same_v<decltype(b), char*>);
    static_assert(std::is_same_v<decltype(v), std::vector<int>>);
    static_assert(std::is_same_v<decltype(s), std::string>);
    static_assert(std::is_same_v<decltype(sv), std::string_view>);

    // 大量のstd::stringオブジェクトを定義する場合
    using std::literals::string_literals::operator""s;

    auto s_0 = "222"s;  // OK
    // ...
    auto s_N = "222"s;  // OK

    static_assert(std::is_same_v<decltype(s_0), std::string>);
    static_assert(std::is_same_v<decltype(s_N), std::string>);

    // 大量のstd::string_viewオブジェクトを定義する場合
    using std::literals::string_view_literals::operator""sv;

    auto sv_0 = "222"sv;  // OK
    // ...
    auto sv_N = "222"sv;  // OK

    static_assert(std::is_same_v<decltype(sv_0), std::string_view>);
    static_assert(std::is_same_v<decltype(sv_N), std::string_view>);

    std::mutex mtx;  // std::mutexはmove出来ないので、AAAスタイル不可
    auto       lock = std::lock_guard{mtx};

    static_assert(std::is_same_v<decltype(lock), std::lock_guard<std::mutex>>);
```

関数の戻り値を受け取る変数を宣言する場合、下記のように書く。

```cpp
    // @@@ example/term_explanation/aaa.cpp 94

    auto v = std::vector<int>{0, 1, 2};

    // AAAを使わない例
    std::vector<int>::size_type t0{v.size()};      // 正確に書くとこうなる
    std::vector<int>::iterator  itr0 = v.begin();  // 正確に書くとこうなる

    std::unique_ptr<int> p0 = std::make_unique<int>(3);

    // 上記をAAAにした例
    auto t1   = v.size();   // size()の戻りは算術型であると推測できる
    auto itr1 = v.begin();  // begin()の戻りはイテレータであると推測できる

    auto p1 = std::make_unique<int>(3);  // make_uniqueの戻りはstd::unique_ptrであると推測できる
```

ただし、関数の戻り値型が容易に推測しがたい下記のような場合、
型を明示しないAAAスタイルは使うべきではない。

```cpp
    // @@@ example/term_explanation/aaa.cpp 121

    extern std::map<std::string, int> gen_map();

    // 上記のような複雑な型を戻す関数の場合、AAAを使うと可読性が落ちる
    auto map0 = gen_map();

    for (auto [str, i] : gen_map()) {
        // 何らかの処理
    }

    // 上記のような複雑な型を戻す関数の場合、AAAを使うと可読性が落ちるため、AAAにしない
    std::map<std::string, int> map1 = gen_map();  // 型がコメントとして役に立つ

    for (std::pair<std::string, int> str_i : gen_map()) {
        // 何らかの処理
    }

    // 型を明示したAAAスタイルでも良い
    auto map2 = std::map<std::string, int>{gen_map()};  // 型がコメントとして役に立つ
```

インライン関数や関数テンプレートの宣言は、下記のように書く。

```cpp
    // @@@ example/term_explanation/aaa.cpp 148

    template <typename F, typename T>
    auto apply_0(F&& f, T value)
    {
        return f(value);
    }
```

ただし、インライン関数や関数テンプレートが複雑な下記のような場合、
AAAスタイルは出来る限り避けるべきである。

```cpp
    // @@@ example/term_explanation/aaa.cpp 156

    template <typename F, typename T>
    auto apply_1(F&& f, T value) -> decltype(f(std::declval<T>()))  // autoを使用しているが、AAAではない
    {
        auto cond  = false;
        auto param = value;

        // 複雑な処理

        if (cond) {
            return f(param);
        }
        else {
            return f(value);
        }
    }
```

このスタイルには下記のような狙いがある。

* コードの安全性の向上  
  autoで宣言された変数は未初期化にすることができないため、未初期化変数によるバグを防げる。
  また、下記のように縮小型変換(下記では、unsignedからsignedの変換)を防ぐこともできる。

```cpp
    // @@@ example/term_explanation/aaa.cpp 183

    auto v = std::vector<int>{0, 1, 2};

    int t0 = v.size();  // 縮小型変換されるため、バグが発生する可能性がある
    // int t1{v.size()};   縮小型変換のため、コンパイルエラー
    auto t2 = v.size();  // t2は正確な型
```

* コードの可読性の向上  
  冗長なコードを排除することで、可読性の向上が見込める。

* コードの保守性の向上  
  「変数宣言時での左辺と右辺を同一の型にする」非AAAスタイルは
  [DRYの原則](https://ja.wikipedia.org/wiki/Don%27t_repeat_yourself#:~:text=Don't%20repeat%20yourself%EF%BC%88DRY,%E3%81%A7%E3%81%AA%E3%81%84%E3%81%93%E3%81%A8%E3%82%92%E5%BC%B7%E8%AA%BF%E3%81%99%E3%82%8B%E3%80%82)
  に反するが、この観点において、AAAスタイルはDRYの原則に沿うため、
  コード修正時に型の変更があった場合でも、それに付随したコード修正を最小限に留められる。


AAAスタイルでは、以下のような場合に注意が必要である。

* 関数の戻り値をautoで宣言された変数で受ける場合  
  上記で述べた通り、AAAの過剰な仕様は、可読性を下げてしまう。

* autoで推論された型が直感に反する場合  
  下記のような型推論は、直感に反する場合があるため、autoの使い方に対する習熟が必要である。

```cpp
    // @@@ example/term_explanation/aaa.cpp 197

    auto str0 = "str";
    static_assert(std::is_same_v<char const*, decltype(str0)>);  // str0はchar[4]ではない

    // char[]が必要ならば、AAAを使わずに下記のように書く
    char str1[] = "str";
    static_assert(std::is_same_v<char[4], decltype(str1)>);

    // &が必要になるパターン
    class X {
    public:
        explicit X(int32_t a) : a_{a} {}
        int32_t& Get() { return a_; }

    private:
        int32_t a_;
    };

    X x{3};

    auto a0 = x.Get();
    ASSERT_EQ(3, a0);

    a0 = 4;
    ASSERT_EQ(4, a0);
    ASSERT_EQ(3, x.Get());  // a0はリファレンスではないため、このような結果になる

    // X::a_のリファレンスが必要ならば、下記のように書く
    auto& a1 = x.Get();
    a1       = 4;
    ASSERT_EQ(4, a1);
    ASSERT_EQ(4, x.Get());  // a1はリファレンスであるため、このような結果になる

    // constが必要になるパターン
    class Y {
    public:
        std::string&       Name() { return name_; }
        std::string const& Name() const { return name_; }

    private:
        std::string name_{"str"};
    };

    auto const y = Y{};

    auto        name0 = y.Name();  // std::stringがコピーされる
    auto&       name1 = y.Name();  // name1はconstに見えない
    auto const& name2 = y.Name();  // このように書くべき

    static_assert(std::is_same_v<std::string, decltype(name0)>);
    static_assert(std::is_same_v<std::string const&, decltype(name1)>);
    static_assert(std::is_same_v<std::string const&, decltype(name2)>);

    // 範囲for文でのauto const&
    auto const v = std::vector<std::string>{"0", "1", "2"};

    for (auto s : v) {  // sはコピー生成される
        static_assert(std::is_same_v<std::string, decltype(s)>);
    }

    for (auto& s : v) {  // sはconstに見えない
        static_assert(std::is_same_v<std::string const&, decltype(s)>);
    }

    for (auto const& s : v) {  // このように書くべき
        static_assert(std::is_same_v<std::string const&, decltype(s)>);
    }
```

### オブジェクトの所有権 <a id="SS_5_2_9"></a>
オブジェクトxがオブジェクトaの解放責務を持つ場合、
xはaの所有権を持つ(もしくは、所有する) という。

定義から明らかな通り、ダイナミックに生成されたaをxが所有する場合、
xはa(へのポインタ)をdeleteする責務を持つ。

xがaを所有し、且つxがaを他のオブジェクトと共有しない場合、「xはaを排他所有する」という。

オブジェクト群x0、x1、...、xNがaを所有する場合、
「x0、x1、...、xNはaを共有所有する」という。

x0、x1、...、xNがaを共有所有する場合、x0、x1、...、xN全体で、a(へのポインタ)をdeleteする責務を持つ。

下記で示したような状況では、
ダイナミックに生成されたオブジェクトの所有権の所在をコードから直ちに読み取ることは困難であり、
その解放責務も曖昧となる。

```cpp
    // @@@ example/term_explanation/ambiguous_ownership_ut.cpp 11

    class A {
        // 何らかの宣言
    };

    class X {
    public:
        explicit X(A* a) : a_{a} {}
        A* GetA() { return a_; }

    private:
        A* a_;
    };

    auto* a = new A;
    auto  x = X{a};
    // aがxに排他所有されているのか否かの判断は難しい

    auto x0 = X{new A};
    auto x1 = X{x0.GetA()};
    // x0生成時にnewされたオブジェクトがx0とx1に共有所有されているのか否かの判断は難しい
```

こういった問題に対処するためのプログラミングパターンを以下の
「[オブジェクトの排他所有](term_explanation.md#SS_5_2_9_1)」と「[オブジェクトの共有所有](term_explanation.md#SS_5_2_9_2)」で解説する。

#### オブジェクトの排他所有 <a id="SS_5_2_9_1"></a>
オブジェクトの排他所有や、それを容易に実現するための
[std::unique_ptr](https://cpprefjp.github.io/reference/memory/unique_ptr.html)
の仕様をを説明するために、下記のようにクラスA、Xを定義する。

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp 7

    class A final {
    public:
        explicit A(int32_t n) noexcept : num_{n} { last_constructed_num_ = num_; }
        ~A() { last_destructed_num_ = num_; }

        int32_t GetNum() const noexcept { return num_; }

        static int32_t LastConstructedNum() noexcept { return last_constructed_num_; }
        static int32_t LastDestructedNum() noexcept { return last_destructed_num_; }

    private:
        int32_t const  num_;
        static int32_t last_constructed_num_;
        static int32_t last_destructed_num_;
    };

    int32_t A::last_constructed_num_ = -1;
    int32_t A::last_destructed_num_  = -1;

    class X final {
    public:
        // Xオブジェクトの生成と、ptrからptr_へ所有権の移動
        explicit X(std::unique_ptr<A>&& ptr) : ptr_{std::move(ptr)} {}

        // ptrからptr_へ所有権の移動
        void Move(std::unique_ptr<A>&& ptr) noexcept { ptr_ = std::move(ptr); }

        // ptr_から外部への所有権の移動
        std::unique_ptr<A> Release() noexcept { return std::move(ptr_); }

        A const* GetA() const noexcept { return ptr_ ? ptr_.get() : nullptr; }

    private:
        std::unique_ptr<A> ptr_{};
    };
```

下記に示した上記クラスの単体テストにより、
オブジェクトの所有権やその移動、
std::unique_ptr、std::move()、[rvalue](term_explanation.md#SS_5_5_3)の関係を解説する。

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp 48

    // ステップ0
    // まだ、クラスAオブジェクトは生成されていないため、
    // A::LastConstructedNum()、A::LastDestructedNum()は初期値である-1である。
    ASSERT_EQ(-1, A::LastConstructedNum());     // まだ、A::A()は呼ばれてない
    ASSERT_EQ(-1, A::LastDestructedNum());      // まだ、A::~A()は呼ばれてない
```

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp 57

    // ステップ1
    // a0、a1がそれぞれ初期化される。
    auto a0 = std::make_unique<A>(0);           // a0はA{0}を所有
    auto a1 = std::make_unique<A>(1);           // a1はA{1}を所有

    ASSERT_EQ(1,  A::LastConstructedNum());     // A{1}は生成された
    ASSERT_EQ(-1, A::LastDestructedNum());      // まだ、A::~A()は呼ばれてない
```

<p><img src="data:image/png;base64,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" /></p>

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp 67

    // ステップ2
    // xが生成され、オブジェクトA{0}の所有がa0からxへ移動する。
    ASSERT_EQ(0, a0->GetNum());                 // a0はA{0}を所有
    auto x = X{std::move(a0)};                  // xの生成と、a0からxへA{0}の所有権の移動
    ASSERT_FALSE(a0);                           // a0は何も所有していない
```

<p><img src="data:image/png;base64,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" /></p>

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp 75

    // ステップ3
    // オブジェクトA{1}の所有がa1からxへ移動する。
    // xは以前保持していたA{0}オブジェクトへのポインタをdeleteするため
    // (std::unique_ptrによる自動delete)、A::LastDestructedNum()の値が0になる。
    ASSERT_EQ(1, a1->GetNum());                 // a1はA{1}を所有
    x.Move(std::move(a1));                      // xによるA{0}の解放
                                                // a1からxへA{1}の所有権の移動
    ASSERT_EQ(0, A::LastDestructedNum());       // A{0}は解放された
    ASSERT_FALSE(a1);                           // a1は何も所有していない
    ASSERT_EQ(1, x.GetA()->GetNum());           // xはA{1}を所有
```
<p><img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAjAAAAGICAIAAADQ4k29AABDO0lEQVR4Xu2dC3wU1dn/hyQkJOEWkkAIxCACBoQgIKW8KISLxVdFFBACilwqCopoAUutKPSPFWrtWxC8wCsgSkE0lIoY8FJAiS9CBbGhVQSUhdAgEFwICYmE5P+wjzkznNldNju7YS6/7yeffM55zjOXM/Oc5zdnZnZXqQIAAABMgCIbAAAAgCsBBAkAAIApUAWpEgAAAKh1IEgAAABMAQQJAACAKYAgAQAAMAUQJAAAAKYAggQAAMAUQJAAAACYAggSAAAAUwBBAgAAYAogSAAAAEwBBAkAAIApgCABAAAwBRAkAAAApgCCBAAAwBRAkAAAAJgCCBIAAABTAEECAABgCiBIAAAATAEECQAAgCmAIAEAADAFECQAAACmAIIEAADAFECQAAAAmAIIEgAAAFMAQQIAAGAKIEgAAABMAQQpGI4cOVJeXi5bAQAAGACCVGPmzp2rKEpmZmZZWZncBgAAIFhsIkglJSUv6di5cye3knLIbS+9tHv37kvX4YXS0tJVOlJSUlauXNm0adMVK1ZkZGRER0fXq1eve/fun376qbw8AACAgLGJIJ08ebLHpURFRc2bN49bz5w500vHG2+8cek6KouKirhQXFx87tw5Khw/fvxaHQ0aNBgxYgSJ0MaNG2fNmvX6668//vjjNGeidV6yOgAAADXBJoIk8fe//z02NvbIkSOVnlnOP31QUFBADtu3bx8/fjxJWkxMzOnTp8mSlZW1cOFCaZ2CX/ziFzRJevbZZ6lcXl5+7NixDRs2kCCNGTNGdgUAABAwNhQkmta0atXqmWee4erOnTsVH/zyl78kh8LCQprunD17tnPnzu+88w5ZMjIy1q5dq12nlpYtW86cOZPL69ev51V16NBBTLAAAAAEgd0E6dSpU927d7/55psrKirkNs9sicRj165dkj0zM/P999+fOnXqtGnTaKoUGRl56NAhyYeh6RSt4aOPPuIqiV9ubu7EiRPJ+OCDD17qCwAAoAbYSpC++eab6667jqZHycnJX3zxhdzsmdDExsaWlJRIdpKZY8eOnThxori4+Omnn+7Zs6fkwJSVlVFTp06dJLU7c+YMCVK3bt20RgAAADXCJoJ04cKFl19+uUGDBhMmTCDZmDVrVkJCgjQT+vDDD1NTU3/9619rjVpOnz5NCzZs2PAf//iH3FZZuXfv3t69e9MmhNQtXLjwiSeeeO2117Kzs0mQxo0bd+kSAAAAaoBNBKlfv35paWnr1q0TltmzZzdp0oTmTFwloYqLi5s5c6bXW3nff/89TX3q168/ePBgEh5tE82cHnnkEZr9kORkZWX9+9//Fk0rV6686qqrIiMjSfzuuecePEMCAAAj2ESQPv3009LSUsm4evXqH3/8kcv79u374YcfLm1XKS8vp/mTVweSmbFjxz7//POSUAEAAAgtNhEkAAAAVgeCBAAAwBRAkAAAAJgCCBIAAABTAEECAABgCiBIAAAATAEECQAAgCmAIAEAADAFECQAAACmwFaClJiYWP3LEgDUAIocOZgCBlEHgsNI1NkVWwkSnWPRCwAChyLH7XYXFxeXlpaWl5d7/cJDXyDqQHAYiTq7oh4cUZJdrANSAwgOihyXy1VYWFhUVEQJgrKDHFu+QdSB4DASdXZFPTiiJLtYB6QGEBwUOfn5+fv37y8oKKDsoP+iXj8g6kBwGIk6u6IeHFGSXawDUgMIDoqcvLy8PXv2UHagK1a6XJVjyzeIOhAcRqLOrqgHR5RkF+uA1ACCgyInNzeXsgNdsbpcLrfbLceWbxB1IDiMRJ1dUQ+OKMku1gGpAQQHRc7q1as3bdq0c+dOulyt0W8tIupAcBiJOruiHhxRkl2sA1IDCA4jqQFRB4LDSNTZFfXgiJLsYh1qJzVMmzZN8SA3hAfeFm1UbgChw0hqqJ1IQNTZDyNRZ1fUgyNKsot1MD5cAxmHtZwaenh44YUX5AYTEMjhChoaq7x+xfMyktwcUhQDqcF4JARyGBF1gkAOV9BYJersinpwREl2sQ7Gh2sgsV7LqcHMBHK4zp07J5s0fP/9988///x///d/yw1VVb/4xS94/V43kZubO2zYsI0bN164cEFqCgLFQGowHgm++qgFUScI5HDZPursinpwREl2sQ6BDNd//vOfAwcObNSoUWRkZHJyct++fTdv3sxNIhAFYqk//OEP5ExLPepBauWqNnwlSyDVyZMnP/zww7SJhISE3/3ud9ykd67yuzNe1yyqpaWlTzzxROvWrevWrZuSkvLQQw+dOXOGm7zCi9OOTZw4sWHDhrRR2rTUqkVrnzRp0r333hsXFzdr1iyxiKCiomLDhg133XUX7UmdOnWoIDkcOXIkIiKC1nPDDTfQ/2bNmp0/f17rQGOYukBNaWlpTz311LfffqttrSmKgdQgOu4HRB2iTo9iIOrsinpwREl2sQ4iOv2QkZFBbpQXOnfu3K5dO4q/P//5z9zUo0cPDuvmzZvzLQu2v/7662xv0aJFampqfHw8V8U6uWowNdAgoZXT8OPqJ5984tVZ2hkae1z16ixVBwwYQNXo6OjMzMzY2Fgq33jjjZWaIJDgxWNiYlp44CrtALf6OlxspK1QNqGDPGfOHHWNVVUHDhz47W9/S3uueEb1k08+SRatA/PMM8+QA2XJr7/+mnIHld955x3Jh5LF+vXr77zzTs4v/fv3X7Vqlf9LY18oBlKDgqi71FmqIup8oRiIOruiHhxRkl2sgxJAaqCQJbdRo0Zx9cSJEwcPHhStHNbaQU7w5RLlkXMeqMBuwuFaD9rrOMkirdZrNT09na4laZxwVYwoydn/zkjO2ipdklOZhtCuXbuo+uWXX3Jrbm4uO+thB7q21W6re/fukoN0uNjYtGnT77//nqri5gYN8qysLNoByjUjRoygcejnvkebNm1oJXSNTOWbbrqJypQCZKdqjh8//qc//aljx47k1rhx49/85jeyx+VQDKQGBVF3qbO2iqjzg2Ig6uyKenBESXaxDkoAqeGOO+7g2KXLJbp2oxF46tQp0cpNUqzXq1ePjA899BBXqcBuWh//SKv1Wp0wYUKVZyBxdfr06V6d/e+M5KytPvfcc1yVmD17NjvrYYcHH3yQq7wt2gHJwWtqGDdunNZIrFu3jpvWrFnjJykQ27ZtY8/t27dTdcmSJYrnWp5SgOyqgbI8ZXzFk2TltsuhGEgNSgCRgKiTQNRVGYs6u6IeHFGSXayDEsBwLSsrW7p06X333UeTfb5fTBdQopUjMkypYerUqVQ+c+aMtJUaVf3vDJe9bohTQ2RkJN/oEPh5k4oXl7YVYGqQjFWeK8onn3ySb8JcddVVTz31lHaWoGX8+PG8kkYe6tevz1Vxm0sLPxgYMmSIuIWSk5MjO10OxUBqUAKIBEQdok6PYiDq7Ip6cERJdrEOSgDD9eOPPxYXSvfcc4/iuQgSrVFRUYrm6ozh+xXXXnttmQcqcKQKh8vePGnYsCH5jxgxgsoUuLy4r8Hvv9q9e3fF98742RDfPCG2bNnCzufPn3///fcLCwu5qof9pW1pb554PVzajeqhkfzee++JkUx5+fXXXy8pKREOZ8+eFblAIjMzU7Omi3djZsyY0bx5c8WTa55++unvvvtO6xA4ioHUoCDqfG8IUecHxUDU2RX14IiS7GIdlABSg+K5Arr++uuvu+46fm7ZtWtX0dqpUyeyUMSTsUf189LXXnuNQ7Nly5Z0qRUTE8NVsRRXtYNBsgwaNEjx3EmnKykevdrWGlXF42XeGenxsv8N8eNlam3fvn3nzp35OfkXX3zBrXp4cdqE9vEyHQ3h4PVwSRv1BV26Pv/88x06dCDnjIwMYRdH+/PPPxfGlStXspEfRRAbN25UPA+x7777bv8PBgJBMZAaFESd3w0h6nyhGIg6u6IeHFGSXayDEkBqoAsrHhURERFNmzalyzrtBQ5dyVK48yWYdm1z585NSkpq0KDBAw88MHnyZKmVq35SA22iV69edHVGY/Kdd96RWmtUrap+AZdGPu2J9AKu/w2VlpY++eST11xzDTnQEejWrdsTTzzhdrvFmiV48SlTpjzyyCO0OToCzz77rNbB6+GSNnpZtm/f/tRTT4lqnz59FN0dedpJfi+AusyWzz777M9//vPJkye1bkGjGEgNouN+QNQh6vQoBqLOrqgHR5RkF+sgotM5TAvn5yVrOsiti5HUEKaDb2YQdSHBSNTZFfXgiJLsYh3CNELMTEhSw6XPm3+iCqkhMAwefCuCqAsJRqLOrqgHR5RkF+tgcIRYkZCkBl6DhLAjNfiHj5WjQNSFBMVA1NkV9eCIkuxiHTigAagpRlIDog4Eh5GosyvqwREl2cU6IDWA4DCSGhB1l2X8+PHXX3993bp1xTQIVBmLOruiHhxRkl2sA2L9snzyySdZWVnx8fFxcXFUyMvLkz0ciZHUgKi7LHSIkpKS+LM7OFwCI1FnV9SDI0qyi3VArPuH4p6vUocOHTps2DDF84kK8RkLJ2MkNSDqdu/e3adPn5SUlKioqJiYmIyMjDlz5mi/J/vQoUNVIXryZCeMRJ1dUQ+OKMku1gGxTnz66afdunVr1KhRREREw4YNKVN8/PHH3DR48GCl+hP1xPDhw6mq/xJ+B2IkNSDq1q1bR8F2zTXX3HDDDeKjrPovG4UgSRiJOruiHhxRkl2sA2Kd+Otf/0oiNGHChMmTJ/fq1UvxfOidvyAyKSmJquJrxBYsWEDV5OTkS5Z3JEZSA6Lu2LFjJ06cEFW+7klLS9O4XASCJGEk6uyKenBESXaxDoh15qOPPpozZ87UqVMffvhhTgFr164le2RkJJWXLVvGbkuXLlU8X8FyycKOxEhqQNQdPXp07NixpEDi2xMUzxeqSm4QJAkjUWdX1IMjSrKLdUCsE5MmTRJJQcBfBZaYmEhlmhixJ2ZIAsVAalAcH3U33ngjh1m7du169Ojh6+UFCJKEkaizK+rBESXZxTog1gn+Uc5f/epXZWVl//nPfzgFLF++vKr6V3mGDx/OnvwMyc9PkDkHI6kBUcdvymRnZ1P5xx9/5O+F0x8WCJKEkaizK+rBESXZxTog1quqf/XymmuumThxYvv27TkFsCB99tlnfFNFvGVHqYQGg7wK52EkNSDq+JcyIiIiMjMzmzZtyhNx7WG5xkPjxo3ZzlXNChyKkaizK+rBESXZxTogNVR5VIfyAgkPKZP46mUWJGLr1q29e/eOi4ujiRQVxAt4DsdIakDUff311xRL0dHRKSkps2bNmjp1qiRIXJXQrMChKAaizq6oB0eUZBfrgCgHwWEkNSDqQHAYiTq7oh4cUZJdrANSAwgOI6kBUQeCw0jU2RX14IiS7GIdkBpAcBhJDYg6EBxGos6uqAdHlGQX64DUAILDSGpA1IHgMBJ1dkU9OKIku1gHpAYQHEZSA6IOBIeRqLMr6sERJdnFOiA1gOAwkhoQdSA4jESdXVEPjijJLtYBqQEEh5HUgKgDwWEk6uyKenBESXaxDkgNIDiMpAZEHQgOI1FnV9SDI0qyi3VAagDBYSQ1IOpAcBiJOruiHhxRkl2sg/jOEgBqRHx8fNCpAVEHgsNI1NkVWwkS4Xa7XS5Xfn5+Xl5ebm7uagejeK6/QIBQtFDMUORQ/FAUyYHlF0SdAFFXI4xEnS2xmyAVFxcXFhbS5caePXvoTG9yMJQaZBPwDUULxQxFDsUPRZEcWH5B1AkQdTXCSNTZErsJUmlpKc18CwoK6BzTdcdOB0OpQTYB31C0UMxQ5FD8UBTJgeUXRJ0AUVcjjESdLbGbIJWXl9OFBp1duuKgWfB+B0OpQTYB31C0UMxQ5FD8UBTJgeUXRJ0AUVcjjESdLbGbIFVUVNB5pWsNOsFut7vIwVBqkE3ANxQtFDMUORQ/FEVyYPkFUSdA1NUII1FnS+wmSEBAqUE2ARBmEHXACBAk24LUAGofRB0wAgTJtiA1gNoHUQeMAEGyLUgNoPZB1AEjQJBsC1IDqH0QdcAIECTbgtQAah9EHTACBMm2IDWA2gdRB4wAQbItSA2g9kHUASNAkGwLUgOofRB1wAgQJNuC1ABqH0QdMAIEybYgNYDaB1EHjABBsi1IDaD2QdQBI0CQbAtSA6h9EHXACBAk24LUAGofRB0wAgTJtiA1gNoHUQeMAEGyD5mZmYoPqEn2BiAUIOpACIEg2Yd58+bJKaEaapK9AQgFiDoQQiBI9sHlckVERMhZQVHISE2yNwChAFEHQggEyVZkZWXJiUFRyCj7ARA6EHUgVECQbMWSJUvkxKAoZJT9AAgdiDoQKiBItuLUqVMxMTHavEBVMsp+AIQORB0IFRAkuzFkyBBtaqCq7AFAqEHUgZAAQbIbOTk52tRAVdkDgFCDqAMhAYJkN86dO5eQkMB5gQpUlT0ACDWIOhASIEg25P777+fUQAW5DYDwgKgDxoEg2ZDNmzdzaqCC3AZAeEDUAeNAkGzIhQsX0jxQQW4DIDwg6oBxIEj2ZIYH2QpAOEHUAYNAkOzJlx5kKwDhBFEHDAJBcjR8079///5eq5fltddem+VBbgAgWBCTTgaC5GgMDn7y5EXkBgCCBTHpZCBIjqZ2Bn9xcbFsAsAHiEknA0GyOWI8r1ixonXr1jExMbfeeuvJkyelVv/VlStXtmnThpbt27fvN998o22VkBZcvHhxenp6nTp12A4cggiAQKJOkhCp1VcVMWlLIEg2h8dhw4YN1QGqKA888IC2VT/atdWEhAQavVwmrr76ar66FBYt2gUbN24s2YFD4JMeYNQFIUiISbsCQbI5Yvi9+eabR44c4QGZmpqqbfU/+Am6Gi0pKZk0aRJXX3rpJXaQUolALDhz5syzZ89+9dVXkgOwNyIAAom6IARJQUzaFAiSzeFB2LZtW6726tVL8fyap7bV/+DPyMjgKmUWtowaNYot/gd/SkpKRUWF1AScAAdAgFEXhCAhJu0KBMnm8Djs168fV4MY/H379uXq+fPn2XLLLbewxf/g79Onj2QHDoEDIMCoIzc/rV6riEm7AkGyOTwOw3Q1OmDAALZwVSCtBzgNKQCkqKOpEpV79+7N1fbt22tbpWW9VhGTdgWCZHOkcRiEIBGrVq0qKSl56KGHuPryyy+zwx133MGW3bt3s0W7IAa/Y5ECQIq65s2bUzk5Odntdm/ZsoWbRKu0rNeqgpi0KRAkmyONwyAEiRIHX9Iy4o0mYu7cucLOeF0PcBpSAEhRN2HCBK7Wr18/MjIyNjZW2yot67WKmLQrECSbI43DIASJqm+99Va7du1iYmKysrL27dvHrURpaSkll6SkJPEOrtf1AKchBYAUdSdPnrztttsonNLT0xcvXoyYBAIIEvAJxjAwG4hJewNBAj7B4AdmAzFpbyBIwCcY/MBsICbtDQQJAACAKYAgAQAAMAUQJAAAAKYAggQAAMAUQJAAAACYAggSAAAAU2ArQUpMTOS3Qi0E7bPcDQBALTJr1izZBK4QthIkyu+iF1aB9tntdhcXF5eWlpaXl+PHWgCoZRTdV4ODK4WaGEVJdrEOFhUkl8tVWFhYVFREskSaJPcKABBOIEjmQU2MoiS7WAeLClJ+fv7+/fsLCgpIk2ieJPcKABBOIEjmQU2MoiS7WAeLClJeXt6ePXtIk2ieJL5FHwBQO0CQzIOaGEVJdrEOFhWk3Nxc0iSaJ7lcLrfbLfcKABBO8FKDeVAToyjJLtbBooK0evXqTZs27dy5kyZJRUVFcq8AAMAZqIlRlGQX6wBBAgAA66ImRlGSXawDBAkAAKyLmhhFSXaxDhAkAACwLmpiFCXZxTqEQ5Dee+89xcPcuXOpeuHChZ///OdUbdGixQ8//CB71xwIEgBXFrzUYB7UxChKsot1CIcgEePGjaM1x8bGfvvtty+88ALrU25uruwXFBAkAK4sCl77Ng1qYhQl2cU6hEmQ3G53y5YtaeW9evVq0KABFe6//37ZKVggSABcWSBI5kFNjKIku1iHMAkS8f777/PEiEhPTz9z5ozsESwQJACuLBAk86AmRlGSXaxD+ASJVp6RkcGC9Nvf/lZuNgAECYArCwTJPKiJUZRkF+sQPkFatGhR9QRJiYuLO3DggOwRLBAkAK4seKnBPKiJUZRkF+sQJkE6ePBgfHw8rXzkyJEtWrSgwk033XThwgXZLyggSAAAwKiJUZRkF+sQDkGi1fbu3ZvW3Lx58x9++GH9+vU8T/qf//kf2TUoIEgAAMCoiVGUZBfrEA5BWrBgASvQ2rVr2ZKdna143gLft2/fpb7BAEECAABGTYyiJLtYh3AIUriBIAEAAKMmRlGSXawDBAkAUFPwUoN5UBOjKMku1gGCBACoKUr4X/vm2/7EDTfcoLV369ZNNGnt4aOkpKR169a0uaVLl8ptPpg2bVqXLl0iIiK87ueoUaPIOHr0aMkeHGpiFCXZxTooECQAQA3RJ9mQI1SH2Lt3Lxvz8/O19kuXCBdz5sxRPK9olZWVyW0+0O6kfj937dpFxjp16uzYsUNqCgI1MYqS7GIdFAgSAKCG6JNsyNEm9F//+tdsfPzxx7X2S5cIC8XFxQkJCbStqVOnym2+mTx5ck5OTs+ePX3tZ6dOncg+aNAguaHmqIlRlGQX66BAkAAANcRrkvXDhg0bODU/++yzVK2oqBC/AHDq1CnZ2wP781dipqamnvdA0xRh1O6Dy+UaN24cudWtW5d8xowZ891333HTpEmTFM8rviQtbNm6dSsv/uqrr+oXT0tLmzJlitvt5qalS5ey87Zt29hCUNq5/fbbyZP8Y2JiOnbsOH/+/AsXLggHpn///tJ+CmbNmkX2iIiII0eOyG01RE2MoiS7WAcFggQAqCFBvNQgfgHg4MGD4pMh7733nuxXDTtQTu/evbvi+a0A/l2bn/3sZ1KiJzlp1qwZWwRNmzZlTSIhYcubb77J/o888ghV69Wrx6rjdfEuXbqUlpZS65AhQ6hKwnPu3Lmf9qyycvPmzZI/QZokHBg/grRx40ZueuWVV+S2GqImRlGSXayDAkECAISfH374Qf8LALKTBs7XlNMXLlxIhezs7BEjRlBh0aJFUqIfO3YsVxcvXkyyQf+5SvMkdrj66qupevfdd1OZ5jH83TFcFYuTcdeuXbR4Tk4OL/7iiy9Sa3p6OpUzMjLYmTl8+PCWLVuoRzRpO3DgQKtWrciH5klan0q/gkQTI26aMGGC3FZD1MQoSrKLdVAgSACAWoHGLGdhxfMLAKdPn5Y9NLAb5fSTJ09GR0fX8xATE0NVKdGnpKRQuW3btmJZKiue1xC4+uSTT1I1Pj6+pKTk008/5WX/9re/cSvfBtQzfPhwaqW5keIRUbFygvZ8ypQpV111VVRUlPCnfdP6VPoVJJp+cdOtt94qt9UQNTGKkuxiHRITE8UBtQoUWBAkACwHzU60vwAgN18Ku1FOp/LQoUO5OmzYsEpdomdVyMrKEsv26dOHLGTn6r///W/2f/vtt6dOnUoFynvl5eXaxfUMGDCg0ocg8XvberQ+lbr91ELSyE0QJBm32+1yufLz8/Py8nJzc1eHCMUzjwkTtJ+0t7TPtOfi8SMAwMzwzTcmLi6OriZlDw3sxoL0zjvvcHX9+vWVukTPM6R27dqJZXmGRHZh6dq1q+K578e31yZNmiSaeHGWHy38koLXW3Z8y7Fly5aUf6jq6206P4J0+PBhbsItO5ni4uLCwkIKjj179lCW3xQi6FjLptBB+0l7S/tMey5engEA1A5BvNRw4MAB/S8AVFRUCAdO0KxAUvXHH38c7oEKlbpEP2bMGK7+7//+b1lZGf3n6n333ffTqisr//SnPymeORM3UQIRTfwMqU6dOosXL6aJC13gfvDBByRda9asqdS81KD9EBKpKRlJ244fP75x48bIyEherXA47oG/Y5rgKiEc6JKam/BSg0xpaWlRUVFBQQHld5pz7AwRdKxlU+ig/aS9pX2mPeeXYQAAtYY2+QYCzTbELwCcOnVKzHhIJ4QPW7wKkoQkSN99911ycjJbBElJSd9++61Y5OjRo0I2WrduLezEoUOH9G/ZEatWraLWZcuWcfX//u//xCKDBw8WbqRV+tfQRauEcMBr3z4pLy+nSQZldppt0Ax0f4igwy2bQgftJ+0t7TPtubgXDACoHbS5NRDmz5/PGTknJ4ct4hcAvv76a7awQxCCVOnRJJonpaSk0ByI/o8ePVqrRszNN9/MSz311FNS0+HDhydMmJCWlkaLN2jQoEePHiQYx44dq/TcQGrSpAktNX36dOH/n//8Z+DAgdHR0aRtb7/9tn5/uKpHOHTs2FHBB2O9QrNmyuk0z6BDT9PVohBBh1s2hQ7aT9pb2mfac+2sHwBQC2hzq+3hrw5KTU0N1bXv559/zvr02WefyW01x26CFCYcFbIAOApHje6SkhL+JNOyZcvktqAYOXKkgi9XrWUcFbIAOIogXmoAYQKCFBAQJAAACDcQpICAIAEAQLiBIAUEBAkAAMINBCkgIEgAABBuIEgBAUECwK7gpQbzAEEKCAgSAHYFo9s8QJACAiELgF3B6DYPEKSAQMgCYFcwus0DBCkgELIA2BWMbvMAQQoIhCwAdgUvNZgHCFJAQJAAACDcQJACAoIEAADhBoIUEBAkAAAINxCkgIAgAQBAuIEgBQQECQC7gpcazAMEKSAgSADYFYxu8wBBCgiELAB2BaPbPECQAgIhC4Bdweg2DxCkgEDIAmBXMLrNAwQpIBCyANgVvNRgHiBIAZGYmKgAUHMocuRgAgD4AIIEQBhRMLcGIGAgSACEEQgSAIEDQQIgjECQAAgcCBIAYQSCZH7wUoN5gCABEEYgSOYH58g8QJAACCNIduYH58g8QJAACCNIdiYkMzNTvJcvQU2yN6hFIEgAhBEFgmQ+5s2bJwtRNdQke4NaBIIEQBhRIEjmw+VyRUREyFqkKGSkJtkb1CIQJADCiAJBMiVZWVmyHCkKGWU/ULtAkAAIIwoEyZQsWbJEliNFIaPsB2oXCBIAYUSBIJmSU6dOxcTEaNWIqmSU/UDtAkECIIxAkEzLkCFDtIJEVdkD1DoQJADCCATJtOTk5GgFiaqyB6h1IEgAhBEIkmk5d+5cQkICqxEVqCp7gFoHggRAGIEgmZn777+fBYkKchu4EkCQAAgjECQzs3nzZhYkKsht4EoAQQIgjECQzMyFCxfSPFBBbgNXAggSAGEEgmRyZniQreAKAUECIIxAkEzOlx5kK7hCQJAACCOWEKSRI0fiM6HADECQAAgjlhAk2sm0tLS///3vcgMAtQsECYAwYhVBUjzfdT1t2jR8HAdcQSBIAIQRCwkSk5mZiWcq4EoBQQIgjFhOkBTP14w+//zzeBMa1D4QJADCiBUFienXrx9+rQ7UMhAkAMKIYllBIho3bvyXv/xF9gYgbECQAiIxMXHWrFlaC1W1Q9dyrdQjbRMIE3ScNSfEYgQhSPGN4+W1WByMlNoEghQQihWuc2uE/XoEgkbOwR6Cu2VHCy4+uNhOf9Qjt9tdXFxcWlpaXl5eUVEh9xmEDghSQCi2S9/26xEImkuVyNBLDYodBYmEubCwsKioiGSJNEnuMwgdEKSAsF/6tl+PQNBo1cjga9+2FKT8/Pz9+/cXFBSQJtE8Se4zCB0QpICQHsbYAAgSELAUheSDsbYUpLy8vD179pAm0TyJJklyn0HogCA5FPtJLAgaJXRfHWRLQcrNzSVNonmSy+Vyu91yn0HogCAB4HRC+OWqthSk1atXb9q0aefOnTRJKioqkvsMQgcECQQP3+rp37+/3OCNGjkTr7322iwPcgMwMRAkYAQIEgieGmlMjZwJ8uRF5AZgYiBIwAgQpIAI33X6hg0bOnToEBUVRXG/efNmudkb6enpderUSUpKeuyxx4J7NzdU1EhjauRcCUGyJuETpEeWPtK8TfOIyAjaxLRV0/QO0l9ii0QaJvUT6vcf1/+VA6/oHQL8gyDVJhCkgAg6LY4bN46z6l133SW3eWjevLnieaT80EMPffPNN8L+8ccfZ2VlxcfHx8XFUWHbtm2iafbs2X379uXVvv/++8JeI4KTWJLPjIyMevXq3XjjjXv37uV90GqMy+WiLqemptatW5c6NWXKFPEQOAhniUAWBFcWJVhB+q9h/8Vnucsvuuhb6a9R00bUmtA8IeverDmb57Cx6y1dGzdrzAuOeW6M1n/QY4Ou/fm13PTYisf0KwzwT4Eg1SIQpIDQZsPAOX36NCkKDwlKnYWFhbJHdeZ98cUXtcYdO3aQP9mHDh06bNgwKkRHR3/++efC4ezZs7zg8uXLNcvVgCB69I9//IP3iklJSeGC0BjSiWbNmgkHpkuXLvzRDa7WyFkikAXBlUUJSpAW/HNBTFwMn8rIqMg/7vij3odbR/2/UVojqdH1N1/PTZIg0d/CvQu5aewfx+pXGOCfAkGqRSBIAaHUPH0Tr7zyCi2YlJTUqlUrKsybN09yqKio4AEj6crgwYPJOGLECK4OHz5c0c2xeMGlS5dqjYETRI+GDBnCG121ahUp4qRJk7gqNGbs2LFUbdGixa5du86dO5eTk8MOLLc1cq70fcvusguCK4gSlCDd+/t7acH6CfUTW1786r8hM4ZIDq/svziUFB/Swk16QVKb/uClKcA/BYJUi0CQAkLRpcVA6N69Oy04ceLEGTNmUKFdu3baVlKjN998kwfMO++8o20iDSPjggULuDp//nyqJicna32aNGlCxnvvvTe4mUEQPaIdoKUyMjK4euTIEd55oTF8+1EPCWqlTpD8O1f6FqTLLgiuIEpQgtQqsxUt2HtU74EPDqRCs6ubaVtJjSa8MIHP8sNLHtYvzk1eBYm/7LXHnT0W/XuRvjWQPwWCVItAkAIiiCcuX375JY+TLVu27N69m8tbt24VDmwhxo8fL31BVmRkpKKZ/bz66qtUjYqK0vr89a9/bdTo4l11RZeyAyGIpXiv+vbty9Xz58/z1oXG8KsZegYMGFCpEyT/zpW+BemyC4IriFJzQXo692k+g9NWTZv57kwuT189XTiIU9zr7l4v7XtJvwZu9SpIk16ZFNsglh30rYH8KRCkWgSCFC4eeeQRCuXU1FT+emCaWFB19OjRwmHy5Mnt27cnY8OGDb/99lthr6z+zQKaGHHV6wxp0KBBZCRNeuCBB7T2AAlCYnmGRPvMVf0MiZ8q6YWBXwWskTNBTbzIpe2XXxBcQZSa5/2+9118Q6dxs8Y0E6JqyjUXz+/P7/q51qF5m4vT4nr16z378bP6NXCceBWkzP6Z1ESadFP2TfrWQP4UCFItAkEKC+fOneNbahKxsbE//PCDcDt06BDbV65cqVm68o477lA096D4GdKdd96p9eH3C+bOnas1hpWhQ4fy3tL4LCkp8fUMqU6dOosXLyYHt9v9wQcfZGdnr1mzplInSP6dK6sPAkHzS7YEuCC4gig1FKQXv3rR608o1a1Xd/6e+cJt7ra5bP/ln3+pXwk3eRWkyKiL0/ohv5YfSgX+p0CQahEIUlgggeFBcttttw32QOmVcihZFi1apPVkt2XLlmmN27dv5xtT4i07kp8dO3ZofbwuGFakt+yE4gqNIX3Vv/+meF6CqNQJkn9ngrRWagpwQXAFUWooSCQwfPo69e3UeUBn/uORMvJ3I7We7CapTs8hPemPm67pdg1XL7tUjf4UCFItAkEKC/w5oaysLK2RpjiK5wVlrZHH3gsvvKA1Elu2bOndu3dcXBxNqqigffhU6XmhnEfaa6+9prWHm3fffTcjIyMmJqZnz57iwZjQGOLw4cMTJkxIS0sjQW3QoEGPHj1mzZp17NixSp0g+XcmSktLqTUpKYkPkaK5d+d/QXAFUWooSPxRoXY92mmN/CZ3Woc0rZHDIHtWttbIgSEhWhf8cwFbxj0/TrtUjf4UCFItAkEKiCCeuARI69atFc+jJumDsX6YPXs2SRSPtE8++URuBuDKodWD0P4lpV189bRxs8baD8b6+Rv02KC2P2vLw+TxNY/rHQL8gyDVJhCkgFB0j9ZDxUcffdSxY8cgvjooOTl5xowZclvAhE9igZMJnyD9auWvUtul1vSrgxo0aTDwwYH61sD/IEi1CQQpIMInSFcK+/UImIHwCdKV+oMg1SYQpICwX/q2X4+AGYAgASNAkALCfunbfj0CZgCCBIwAQQoI+z1xgSCBcABBAkaAIDkU+0ksMAMQJGAECBIAIGRAkIARIEgAgJABQQJGgCABAEIGBAkYAYIUEHjiAkAgQJCAESBIAWG/d9IgsSAcQJCAESBIAWE/QbJfj4AZgCABI0CQAsJ+6dt+PQJmAIIEjABBCgj7pW/79QiYAQgSMAIEKSDs98QFggTCAQQJGAGC5FDsJ7HADECQgBEgSACAkAFBAkaAIAEAQgYECRgBggQACBkQJGAECFJA4IkLAIEAQQJGgCAFhP3eSYPEgnAAQQJGgCAFhP0EyX49AmYAggSMAEEKiMTERIpLaVZBVUWDtVqpR1o7ACGBR4qdiI+PhyDVGhAkAEAocbvdLpcrPz8/Ly8vNzd3tfWhXlBfqEfUL+qd3GEQOiBIAIBQUlxcXFhYSJOJPXv2UB7fZH2oF9QX6hH1i3ondxiEDggSACCUlJaWFhUVFRQUUAanWcVO60O9oL5Qj6hf1Du5wyB0QJAAAKGkvLycphGUu2k+4XK59lsf6gX1hXpE/aLeyR0GoQOCBAAIJRUVFZS1aSZB6dvtdhdZH+oF9YV6RP2i3skdBqEDggQAAMAUQJAAAACYAggSAAAAUwBBAgAAYAogSAAAAEwBBAkAAIApgCABAAAwBRAkAAAApsBWghTgF12jFa1oRSswIbYSJAAAANYFggQAAMAUQJAAAACYAggSAAAAUwBBAgAAYAogSAAAAEwBBAkAAIApgCABAAAwBRAkAAAApgCCBAAAwBRAkAAAAJgCCBIAAABTAEECAABgCiBIP5GYmKj9PmAALApFsgOjWttrYF0gSD9BMS2OAADWhSLZ7XYXFxeXlpY6J6q1vS4vL6+oqJBHOLAC6gkVJdnFGThn6AJ7Q5HscrkKCwuLioqcE9XaXpMskSbJIxxYAfWEipLs4gycM3SBvaFIzs/P379/f0FBgXOiWttr0iSaJ8kjHFgB9YSKkuziDJwzdIG9oUjOy8vbs2cPZWfnRLW21zRPokmSPMKBFVBPqCjJLs7AOUMX2BuK5NzcXMrONGNwTlRre+1yudxutzzCgRVQT6goyS7OwDlDF9gbiuTVq1dv2rRp586dzolqba9pklRUVCSPcGAF1BMqSrKLM/A/dE+dOlW3bl3FQ3Z2ttwcALxs//795QbDrFixYpYHucHDkSNHpk6d2qFDh7i4uOjo6LZt2z744INff/217Bdq/O9VyAnV5qTTxFVi0aJFvnxCS2lpaevWrWn9y5YtY8uoUaOoOnr06EsdfaLYUZCmTZvWpUuXiIgIPvhyMwTJLqgnVJRkF2fgNcoFS5cu5ZFA1K9fn7KG7HE5eNlwZDFaJ69cbqiq+uijjxo1aiT2XGA8cV8WP3sVDkK1OV6JXpBIJCoqKrz6hJZnnnmGVt68efPy8nK27N69myx16tShVHupr3cUOwqSOBGM3AxBsgvqCRUl2cUZeI1ywcCBA7XjIScnR/a4HLxgOLKYr1x8+PBhVqN69eotXLjwxIkTZWVllN0effTR3//+95JzyPG1VxJnz56VTUER4OYuC69EL0iKJ9959QkhdDQSEhJo5TSp1do7depExkGDBmmNvuBdtZkgTZ48ee3atT179uSDLzdDkOyCekJFSXZxBl6jnDl58mRUVBQ5DBs2jJI7Fe6++27JR5+kJIveQcuBAwduv/32tLS0unXrxsTEdOzYccGCBZXVp6ewsHDs2LF01Uy7ERcX1759+9GjR7vd7irdlSPDSz322GNcXb58efV2fuL8+fNcINEaN25camoqbZfWP2bMmEOHDgk3Xpz2+fXXX6cpAu3YrbfeSkOdW4PbKy7TOpcsWZKenk4X/pKdq3qBoe1Onz69Xbt2tBu0ua5du7777rvaZSXEgto+0hGeMmXK6dOnRet7772XkZFBp/XGG2/817/+xctKZy0yMpL+0xa1Rl9nVtpz0bpixYpWrVrFx8dnZ2fTDqxfv75Dhw7Ulx49euTn57PzsmXL2D8vL48tzOzZs8kYERFRUFCgtXtFsaMgMfqoEGh7DUGyLuoJFSXZxRl4jXKGUicPA5oYUUamAiXEkpISrQ87BC1IW7ZsYQctpEncmpWVJbcpynfffVflNxdTnlU8X6ki7jVJUKZu1qyZtGzTpk2FJrGlYcOGWocHHniAW4PbKy43btzYq91XWj927NjVV18tFmHEjUfJznCT1z526dLl3Llz1Pr555+LR4NESkoKF6SzRlpFukuFDz74QBh9nVmvgkTzHpJeLhN0pc8ix5AysfOQIUOoSrtEc1m2MJRn2XPx4sVau1cUCBIEybKoJ1SUZBdn4DXKmQEDBlArXcwWFxe//PLLPCTWrFmj9WFj0IJ05MiRrVu30vSCxOPgwYN0KU3ONE/iVk6abdu2PX78eHl5+d69e5977jkqc6uvURobG0vGXr16SXYBzW94QVJcyoBCd2mexA5cVTydpWtzVhGaanBrcHsl1jlz5kwSdfF6BRt9pXVSQa5OmDCBxInkhCR8w4YN3Kr3F3AfW7RosXv3burj2rVr2e2ll16qqhYAxZPLaGcmTZrEVf1ZmzdvHhUoErRGyYerXgWJePvtt48ePcp35AiaqFE49evXj6t8HGjKSGW6kuBlBfwRV8XTfalJjwJBgiBZFvWEipLs4gy8RjlBGZYvZm+55ZYqTWqgXCa7+oWX8iVIZ86coQx11VVX8b1BhiSQW9u0aaN4HgU9/PDDr776Kl3XX7hwQSzra5ReVpB4QkCKIixUVjxP1LnKqxUOtCrFc+OIq8HtFRtp01pnYfeV1nmCQlroa7bna3O8oJ7hw4dTa3JysqIRAHFy9WJD1wo8U9y1a5cvH656FaR27dpxlY8hQZcgVJ0zZw5XSV+rqjVef8pIgNmNJuhSkx4FggRBsizqCRUl2cUZeI1yQkyJZsyYke+BEzGle7rClb19wyvxJUj8aq8ebqVsJd2wIpHYt28ft/oapZe9Zcfil5WVJSx9+vQhC9m5yqulq3iuShsKbq/YSBvyahfHR0wduMq7ql9K4GtzWoHXwnMdvtTo27cvO9OB4lavYvP4449TecSIEX58qnR7zmX9Maz0jL25c+dy9cMPP6zyLUj8NakKBMnHWa6CINkF9YSKkuziDLxGOUHZiseAnlWrVsnevuFFfAlSgwYNqLVly5aHDx+mqte3ib766qt169aJVxXGjh3Ldr6jKDlXaV5qWLFihdTELzXwDElcvFdVz5DIzlVe3Ne1P1PTvZLWKeCPmPTu3Zur7du31y5OcyPFc+fNl7j62hz3keVHS6Un9HmGRNtio58ZEpWPHj0aHR0tnv0IH/97LjlLx1ASJF+37Gg6xW64Zac9elq0vYYgWRf1hIqS7OIMvEb5sWPHtA+fJQYPHiw82aLNs5KFq9dff/3GS+HWuLg4am3VqtWJEydoUImNcuvUqVO3bdvmdrtJSMTrD7fddhu33nHHHWz54osv2MJQFuPXvmky9+KLL548ebKsrIx8pkyZwq99jxkzhhd89dVXy8vL6T9X77vvPl4DV30l0+D2SlqngO+tkUKcPn1669at7KZUb0s8Q5o4ceL3339PHfnkk0+0z5B8bY6fIdWpU2fJkiU0z6CVU+rPzs5+6623qHXo0KG81Jtvvkmtfp4hcXX8+PFs0Rr977nk7F+QxEsN4kNIDMUJuzn2pYYTHkj1+ThwlRAO2l5DkKyLekJFSXZxBl6H7qJFi3gAzJ8/X2vv3Lmz4nl8cubMGbaw22UFSQ+3krYJC+UjmippW0WTloULF3IrP2/XwvYqzwdjpXfkGH4/7dChQzxF0JKUlMRvylXpuiAlU3UZDZfdKy7rBYmu/bmpfv36pMf8AEws5f8tuyrfm3O5XPq37BRP8qrSvWXXpEkTLvgSJJoOipflhNH/nkvO/gVp+fLlXN2+fTs7MNRTxdmvffNh0aN1gCDZAPWEipLs4gy0wS3gKzLKMoWFhVq7yCNvvPEGW7gatCDR+gcOHBgdHd26deucnBwpbU2fPr1Hjx4JCQmUkmgu1a1bN5rxcFOV54k35UQSEpErRVNV9VcHtW/fnhIlrb9NmzY02xDvtpEm0TwpJSUlKiqK/o8ePVqoUZWuCyHZK2mdAsogNLuKiYlJT0+n2Yy0LXbgzyFRL+hSIDMzc/369aLV1+aqPEeAmtLS0qiPDRo0oH2ePXs2TbO4laZZGRkZtN2ePXvS7EraPf3e3nnnnZLR/55Lzv4F6ezZsyyK1FN2YDp27Kg4+4OxfJT0aB0gSDZAPaGiJLs4A21wA3Cl4K8OSk1N/fHHH9ki3uvbsWPHpb7e0aZm50Q1BMkeqCdUlGQXZ+CcoQvMTGlpKd+cFN+vMXLkSMXxX656WSBI9kA9oaIkuzgD5wxdYG8gSBAk66KeUFGSXZyBc4YusDcQJAiSdVFPqCjJLs7AOUMX2BsIEgTJuqgnVJRkF2fgnKEL7A0ECYJkXdQTKkqyizNwztAF9gaCBEGyLuoJFSXZxRkkJiYqAFif+Ph4kZq1P/Nhb7S9hiBZFwiSitvtdrlc+fn5eXl5ubm5qwGwJhS9FMMUya5qnBDV2l7TWJaHN7ACECSV4uLiwsJCurzas2cPRfYmAKwJRS/FMEVyYTVOiGptr2ksy8MbWAEIkkppaSnN9AsKCiim6TprJwDWhKKXYpgiuagaJ0S1ttc0luXhDawABOkn8AwJ2IPGjRu7XC6aJVBeTvT8rIkT0Paapkfl5eXyCAdWAIL0E4pj3kcC9oYiWaTmi1H91ltO+NP2GoJkXdQwFiXZxRlAkIA9gCBBkKyLGsaiJLs4AwgSsAcUyeJpiqMECc+QbIAaxqIkuzgDCBKwBxTJ4n0zRwkS3rKzAWoYi5Ls4gz8C9KpU6fET4tmZ2fLzQHAy+p/mM44K1asmOVBsvMWFc8PDDZq1Khjx44PPfSQ+Gk+4/jabpgI1eakEyGO0qJFi3z5hBa6eG/dujWtf9myZWwZNWqUUpMfmPAPrUp8IkexiyCVvPHGE3fdlZ6cHB0VRf+pXLpypdZB22t8Dsm6qGEsSrKLM7g4dH2zdOnS6sR18ZeqKafIHpeDlw1HjtP/uCojdlhLvXr11q1bJ3kGh6/tholQbY5XohckEomKigqvPqGFf4KvefPm5eXlbNm9ezdZ6tSps3Pnzkt9g0GRvjpIl9wt91e5Zs2ATp3EmWJuzswku/DR9hrf1GBd1DAWJdnFGVwcur4ZOHCgdjDk5OTIHpeDFwxHjvOVqbVbPH78+OLFi0lKFc+XrHz77beScxD42q7E2bNnZVNQBLi5y6I9LKLKUEbz6hNC6GgkJCTQyqdOnaq1d/Ik3AB/pNw/3BE7CdKqRx/lMzLvnnvO/eUvcz0TyovdfPRR4aPtNQTJuqhhLEqyizO4OHR9cPLkyaioKHIYNmwYzTCocPfdd0s+PEK0KUyy6B20HDhw4Pbbb09LS6tbt25MTEzHjh0XLFhQWX16CgsLx44dS9fUtBtxcXHt27cfPXq02+2u8jEN4qW4rN3i22+/zcYpU6YI4+HDh8eNG5eamkqbph2gptOnT3OTwe0uWbIkPT2dLvwlO1f1AkNJZPr06e3ataMjQJvr2rXru+++q11WQizopwvEe++9l5GRQSfuxhtv/Ne//sXLSuclMjKS/tMWtUZf507ac9G6YsWKVq1akd5nZ2fTDqxfv75Dhw7Ulx49euTn57PzsmXL2D8vL48tzOzZs8kYERFRUFCgtQeBYjtBur1bN+pIg9jY83TF8NZb9J/KZBnUrZvw0fYagmRd1DAWJdnFGYj8oocSKycRmhjdeuutVKB0WVJSovVhh6AFacuWLeyghTSJW7OysuQ2Rfnuu++q/GZqLmu3SN1s2rQpGa+99lq2UCpv1qzZJQsrSpcuXc6dO1dlbLvar/XU2n2l9WPHjvFPd2sRT4wkO8NN/rvw+eefi4d/REpKChek80JaRbpLhQ8++EAYfZ07r4JE8x6SXi4TPXv2ZJFjSJnYeciQIVSlXSorK2MLQ5mUPWkiq7UHgWI7QUr1zCm7Xn21sHT1hErLxERh0fYagmRd1DAWJdnFGVwcuj4YMGAAtdKlbnFx8csvv8yJY82aNVofNgYtSEeOHNm6dStNPioqKg4ePEgX2uRM8yRu5ZTatm3b48ePl5eX792797nnnqMyt0r5UeB1i7169SJjbGwsV2kCRNUWLVrs3r2bUuTatWt5qZdeeqnK2HaJmTNnkmyL1yjY6CutP/DAA1ydMGECiRPJCYn0hg0buFXvL/DfBRYAxZOtaGcmTZrEVf15mTdvHhXoXGuNkg9XvQoSQRPQo0eP8h05xTMNpYDp168fV/k40JSRyjRj42UF/Iq24um+1FRTFNsJUrTn/kTf664TlqzrriML2YVF22sIknVRw1iUZBdncHHoeoPyL1/q3nLLLVWaxEGZTnb1Cy/lS5DOnDlD+euqq67ie4MMSSC3tmnTRvG8j/Dwww+/+uqrdNV/4cIFsayvTO11i5Ig8bRAz/Dhw6uMbZfmIlpnYfeV1nlPUlNTxZsFEr42578LycnJikYAxOnTiw1dDTRs2JDKu3bt8uXDVa+C1K5dO67yESboIoOqc+bM4Srpa1W1xpMPOwtIgNmNpuBSU01RHCBIfTp0IEtM3brCou01BMm6qGEsSrKLM7g4dL0hpkQzZszI98BpmnI6Xf/K3r7hlfgSJH7xVw+3Ui6TbmfRrGXfvn3c6itTs1G7ReqmlKC1+qeFJwpGttunTx+vdrE/YurAVd4T/VICX5vz3wW+mOjbty87k9pxq1exefzxx6k8YsQIPz5Vuj3nMhm5Kvaz0jO65s6dy9UPP/ywyrcglZaWshsESf+nv2XXBbfsbIoaxqIkuziDi0PXG5TLOFPoWbVqleztG17ElyA18HwJZsuWLQ8fPkzVnj17sr/W56uvvlq3bt1jjz3GTWPHjmU731GUnKu8bVG81PDoo4+yhZ+pcO7WUqmJDOPbZSIiIsjeu3dvrrZv3167OM2NFM+dN18zJF+b898FFmDaFhv9zJCofPTo0ejoaPHsR/j433PJWRJOSZB83bKj6RS74Zad/k//UkN9z7tFeKnBfqhhLEqyizMQGUTLsWPHtI+mJQYPHiw82aLNwpKFq9dff/3GS+HWuLg4am3VqtWJEydoUImNcuvUqVO3bdvmdrvPnz8vXn+47bbbuPWOO+5gyxdffMEWho28A7TaJUuWsOzVr1+fX0yoqn4AU6dOHWqli/TTp09T3szOzn6LBnkotquF762RQtBWtm7dym5KdR/FM6SJEyd+//33ZWVln3zyifYZkq/N+e/C0KFDeak333yTWv08Q+Lq+PHj2aI1+t9zydm/IImXGsSHkBiKBHbDSw36P+m1b/rPVbz2bT/UMBYl2cUZXBy6OhYtWsShP3/+fK29c+fOiufhypkzZ9jCbpcVJD3cStomLJStaKqkbRVNWhYuXMit/DRei5+lYmNj//a3v7ED4XK59K+oKZ6x7WsNAW5XL0h07c9NpIikuLGeN3fFUv7fsqvyvTn/XZDesmvSpAkXfAkSTQfFy3LC6H/PJWf/grR8+XKubt++nR0Y6qmC1759/OGDsc5BDWNRkl2cwcWhq6N3796K50MqhYWFWrvIMm+88QZbuBq0INH6Bw4cGB0d3bp165ycHCmpTZ8+vUePHgkJCZSwaC7VrVu3F198kZuqPM/DKWMmJSWJTMp2sQlaSnx10DfffCMWZI4cOUKLp6WlRUVF0RSKNjR79myao1QZ265ekChH0OwqJiYmPT2dZjNSH9mBP4dEx4HEPjMzc/369aLV1+aq/HaBoGlWRkYGbbdnz540u5J2T7+3d955p2T0v+eSs39BOnv2LIsi9ZQdGDo7Cj4Y6/uv5I03fnPnnenJyXUjI+k/lcmiddD2GoJkXdQwFiXZxRmIDAJAWOGvDkpNTf3xxx/ZIt7r27Fjx6W+wWBLQbrsHwTJHqhhLEqyizOAIIHaobS0lG9OLl++nC0jR45UQvrlqhAkCJJFUcNYlGQXZwBBAvYAggRBsi5qGIuS7OIMIEjAHkCQIEjWRQ1jUZJdnAEECdgDCBIEybqoYSxKsoszgCABewBBgiBZFzWMRUl2cQYQJGAPIEgQJOuihrEoyS7OAIIE7AEECYJkXdQwFiXZxRlAkIA9gCBBkKyLGsaiJLs4g8TERAUA6xMfHy9Ss/ZnEu2NttcQJOsCQVJxu90ulys/Pz8vLy83N3c1ANaEopdimCLZVY0TolrbaxrL8vAGVgCCpFJcXFxYWEiXV3v27KHI3gSANaHopRimSC6sxglRre01jWV5eAMrAEFSKS0tpZl+QUEBxTRdZ+0EwJpQ9FIMUyQXVeOEqNb2msayPLyBFYAgqZSXl9OFFUUzXWHRrH8/ANaEopdimCK5uBonRLW21zSW5eENrAAESaWiooLimK6tKKDdbre4ugTAWlD0UgxTJJdX44So1vaaxrI8vIEVgCABAAAwBRAkAAAApgCCBAAAwBRAkAAAAJgCCBIAAABTAEECAABgCiBIAAAATAEECQAAgCmAIAEAADAFECQAAACmAIIEAADAFECQAAAAmAIIEgAAAFMAQQIAAGAKIEgAAABMAQQJAACAKYAgAQAAMAUQJAAAAKYAggQAAMAUQJAAAACYAggSAAAAUwBBAgAAYAogSAAAAEwBBAkAAIApgCABAAAwBRAkAAAApsCLIAEAAABXEAgSAAAAUwBBAgAAYAr+P3Md/o6LXhdmAAAAAElFTkSuQmCC" /></p>

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp 88

    // ステップ4
    // x.ptr_はstd::unique_ptr<A>であるため、ステップ3の状態では、
    // x.ptr_はA{1}オブジェクトのポインタを保持しているが、
    // x.Release()はそれをrvalueに変換し戻り値にする。
    // その戻り値をa2で受け取るため、A{1}の所有はxからa2に移動する。
    std::unique_ptr<A> a2{x.Release()};         // xからa2へA{1}の所有権の移動
    ASSERT_EQ(nullptr, x.GetA());               // xは何も所有していない
    ASSERT_EQ(1, a2->GetNum());                 // a2はA{1}を所有
```

<p><img src="data:image/png;base64,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" /></p>

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp 99

    // ステップ5
    // a2をstd::move()によりrvalueに変換し、ブロック内のa3に渡すことで、
    // A{1}の所有はa2からa3に移動する。
    {
        std::unique_ptr<A> a3{std::move(a2)};
        ASSERT_FALSE(a2);                       // a2は何も所有していない
        ASSERT_EQ(1, a3->GetNum());             // a3はA{1}を所有
```

<p><img src="data:image/png;base64,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" /></p>

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp 109

        // ステップ6
        // このブロックが終了することで、std::unique_ptrであるa3のデストラクタが呼び出される。
        // これはA{1}オブジェクトへのポインタをdeleteする。
    }                                      // a3によるA{1}の解放
    ASSERT_EQ(1, A::LastDestructedNum());  // A{1}が解放されたことの確認
```

<p><img src="data:image/png;base64,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" /></p>


また、以下に見るようにstd::unique_ptrはcopy生成やcopy代入を許可しない。

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp 123

    auto a0 = std::make_unique<A>(0);

    // auto a1 = a0;                            // 下記のようなメッセージでコンパイルエラー
    //      unique_ptr_ownership_ut.cpp:125:15: error: use of deleted function ‘std::unique_ptr ...

    auto a1 = std::move(a0);                    // すでに示したようにmove生成は可能
    
    auto a2 = std::unique_ptr<A>{};

    // a2 = a1;                                 // 下記のようなメッセージでコンパイルエラー
    //      unique_ptr_ownership_ut.cpp:131:10: error: use of deleted function ‘std::unique_ptr ...

    a2 = std::move(a1);                         // すでに示したようにmove代入は可能

    //
    auto x0 = X{std::make_unique<A>(0)};

    // auto x1 = x0;                            // Xはstd::unique_ptrをメンバとするため、
                                                // デフォルトのcopyコンストラクタによる生成は
                                                // コンパイルエラー
    auto x1 = std::move(x0);                    // デフォルトのmove生成は可能

    auto x2 = X{std::make_unique<A>(0)};

    // x2 = x1;                                 // Xはstd::unique_ptrをメンバとするため、
                                                // デフォルトのcopy代入子の呼び出しは
                                                // コンパイルエラー
    x2 = std::move(x1);                         // デフォルトのmove代入は可能
```

以上で示したstd::unique_ptrの仕様の要点をまとめると、以下のようになる。

* std::unique_ptrはダイナミックに生成されたオブジェクトを保持する。
* ダイナミックに生成されたオブジェクトを保持するstd::unique_ptrがスコープアウトすると、
  保持中のオブジェクトは自動的にdeleteされる。
* 保持中のオブジェクトを他のstd::unique_ptrにmoveすることはできるが、
  copyすることはできない。このため、下記に示すような不正な方法以外で、
  複数のstd::unique_ptrが1つのオブジェクトを共有することはできない。

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp 161

    // 以下のようなコードを書いてはならない

    auto a0 = std::make_unique<A>(0);
    auto a1 = std::unique_ptr<A>{a0.get()};  // a1もa0が保持するオブジェクトを保持するが、
                                             // 保持されたオブジェクトは二重解放される

    auto a_ptr = new A{0};

    auto a2 = std::unique_ptr<A>{a_ptr};
    auto a3 = std::unique_ptr<A>{a_ptr};  // a3もa2が保持するオブジェクトを保持するが、
                                          // 保持されたオブジェクトは二重解放される
```

こういった機能によりstd::unique_ptrはオブジェクトの排他所有を実現している。

#### オブジェクトの共有所有 <a id="SS_5_2_9_2"></a>
オブジェクトの共有所有や、それを容易に実現するための
[std::shared_ptr](https://cpprefjp.github.io/reference/memory/shared_ptr.html)
の仕様をを説明するために、下記のようにクラスA、Xを定義する。

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp 7

    class A final {
    public:
        explicit A(int32_t n) noexcept : num_{n} { last_constructed_num_ = num_; }
        ~A() { last_destructed_num_ = num_; }

        int32_t GetNum() const noexcept { return num_; }

        static int32_t LastConstructedNum() noexcept { return last_constructed_num_; }
        static int32_t LastDestructedNum() noexcept { return last_destructed_num_; }

    private:
        int32_t const  num_;
        static int32_t last_constructed_num_;
        static int32_t last_destructed_num_;
    };

    int32_t A::last_constructed_num_ = -1;
    int32_t A::last_destructed_num_  = -1;

    class X final {
    public:
        // Xオブジェクトの生成と、ptrからptr_へ所有権の移動もしくは共有
        explicit X(std::shared_ptr<A> ptr) : ptr_{std::move(ptr)} {}

        // ptrからptr_へ所有権の移動
        void Move(std::shared_ptr<A>&& ptr) noexcept { ptr_ = std::move(ptr); }

        int32_t UseCount() const noexcept { return ptr_.use_count(); }

        A const* GetA() const noexcept { return ptr_ ? ptr_.get() : nullptr; }

    private:
        std::shared_ptr<A> ptr_{};
    };
```

下記に示した上記クラスの単体テストにより、
オブジェクトの所有権やその移動、共有、
std::shared_ptr、std::move()、[rvalue](term_explanation.md#SS_5_5_3)の関係を解説する。

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp 47

    // ステップ0
    // まだ、クラスAオブジェクトは生成されていないため、
    // A::LastConstructedNum()、A::LastDestructedNum()は初期値である-1である。
    ASSERT_EQ(-1, A::LastConstructedNum());     // まだ、A::A()は呼ばれてない
    ASSERT_EQ(-1, A::LastDestructedNum());      // まだ、A::~A()は呼ばれてない
```

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp 56

    // ステップ1
    // a0、a1がそれぞれ初期化される。
    auto a0 = std::make_shared<A>(0);           // a0はA{0}を所有
    auto a1 = std::make_shared<A>(1);           // a1はA{1}を所有
    ASSERT_EQ(1, a0.use_count());               // A{0}の共有所有カウント数は1
    ASSERT_EQ(1, a1.use_count());               // A{1}の共有所有カウント数は1

    ASSERT_EQ(1,  A::LastConstructedNum());     // A{1}は生成された
    ASSERT_EQ(-1, A::LastDestructedNum());      // まだ、A::~A()は呼ばれてない
```

<p><img src="data:image/png;base64,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" /></p>


```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp 68

    // ステップ2
    // x0が生成され、オブジェクトA{0}がa0とx0に共同所有される。
    ASSERT_EQ(0, a0->GetNum());                 // a0はA{0}を所有
    ASSERT_EQ(1, a0.use_count());               // A{0}の共有所有カウントは1
    auto x0 = X{a0};                            // x0の生成と、a0とx0によるA{0}の共有所有
    ASSERT_EQ(2, a0.use_count());               // A{0}の共有所有カウント数は2
    ASSERT_EQ(2, x0.UseCount());
    ASSERT_EQ(x0.GetA(), a0.get());
```

<p><img src="data:image/png;base64,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" /></p>

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp 79

    // ステップ３
    // x1が生成され、オブジェクトA{0}の所有がa0からx1へ移動する。
    auto x1 = X{std::move(a0)};                 // x1の生成と、a0からx1へA{0}の所有権の移動
    ASSERT_EQ(x1.GetA(), x1.GetA());            // x0、x1がA{0}を共有所有
    ASSERT_EQ(2, x0.UseCount());                // A{0}の共有所有カウント数は2
    ASSERT_EQ(2, x1.UseCount());
    ASSERT_FALSE(a0);                           // a0は何も所有していない
```

<p><img src="data:image/png;base64,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" /></p>

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp 89

    // ステップ４
    // オブジェクトA{1}の所有がa1からx1へ移動する。
    // この時、x1::ptr_は下記のような手順で以前保持していたA{0}オブジェクトへの所有を放棄する。
    //  1. x1::ptr_の共有所有カウント(ptr_.use_count()の戻り値)をデクリメント
    //  2. 共有所有カウントが0ならば、ptr_で保持しているオブジェクト(この場合、A{0})をdelete
    //  3. x1::ptr_の管理対象をに新規オブジェクト(この場合、A{1})に変更
    //
    // ここでは、x0::ptr_がA{0}を所有しているため、共有所有カウントは1であり、
    // 従って、A{0}はdeleteされず、A::LastDestructedNum()の値は-1のまま。
    ASSERT_EQ(1, a1->GetNum());                 // a1はA{1}を所有
    ASSERT_EQ(0, x1.GetA()->GetNum());          // x1はA{0}を所有
    ASSERT_EQ(2, x1.UseCount());                // A{0}の共有所有カウント数は２
    x1.Move(std::move(a1));                     // x1はA{0}の代わりに、A{1}を所有
                                                // a1からx1へA{1}の所有権の移動
    ASSERT_EQ(-1, A::LastDestructedNum());      // x0がA{0}を所有するため、A{0}は未解放
    ASSERT_FALSE(a1);                           // a1は何も所有していない
    ASSERT_EQ(1, x1.GetA()->GetNum());          // x1はA{1}を所有
    ASSERT_EQ(1, x1.UseCount());                // A{0}の共有所有カウント数は1
```

<p><img src="data:image/png;base64,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" /></p>

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp 110

    // ステップ５
    // 現時点でx1はA{1}オブジェクトを保持している。
    // x1::Moveに空のstd::shared_ptrを渡すことにより、A{1}を解放する。
    x1.Move(std::shared_ptr<A>{});              // x1に空のstd::shared_ptr<A>を代入することで、
                                                // A{1}を解放
    ASSERT_EQ(nullptr, x1.GetA());              // x1は何も保持していない
    ASSERT_EQ(1, A::LastDestructedNum());       // A{1}が解放された
```

<p><img src="data:image/png;base64,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" /></p>


```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp 120

    // ステップ６
    // 現時点でx0はA{0}オブジェクトを保持している。
    //
    // ここでは、x0からx2、x3をそれぞれcopy、move生成し、
    // この次のステップ７では、x2、x3がスコープアウトすることでA{0}を解放する。
    {
        ASSERT_EQ(0, x0.GetA()->GetNum());      // x0はA{0}を所有
        ASSERT_EQ(1, x0.UseCount());            // A{0}の共有所有カウント数は1
        auto x2 = x0;                           // x0からx2をcopy生成
        ASSERT_EQ(x0.GetA(), x2.GetA());
        ASSERT_EQ(2, x0.UseCount());            // A{0}の共有所有カウント数は２

        auto x3 = std::move(x0);                // x0からx2をmove生成、x0はA{0}の所有を放棄
        ASSERT_EQ(nullptr, x0.GetA());
        ASSERT_EQ(0, x2.GetA()->GetNum());      // x2はA{0}を保有
        ASSERT_EQ(x2.GetA(), x3.GetA());        // x2、x3はA{0}を共有保有
        ASSERT_EQ(2, x2.UseCount());            // A{1}の共有所有カウント数は２
```

<p><img src="data:image/png;base64,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zpTdZ555xu+tKerAefPmlZSUfP3111oDIopnjOAGkLDr16+f7I4cOdLw/B0Nc204ETxgwADZRb5LyY033iglwSO4S5culZWVWhURxT9GcANI3o0ZM0Z2g4csmgWp9bs7evRo2T179qyUXHnllVISPIIvv/xyrZyIbIER3ACSd4HWufK35kaNGiW7AwcONNdqx/rdDbIKHjt2rJTIrqLdDhHZCyO4AbS80yK4a9eu2O7UqZPL5Xr//felStVqx/rdhZdffrm0tPTOO++U3WeffVYaXH311VKye/duKTEfyAgmsilGcANoeadF8PTp02W3TZs2zZo1a9WqlblWO9bvLuLb/Geb1TciYOHChapc+L0dIrIXRnADaHmnRfCJEycmTJiQnJzcq1evZcuWabXasYF2169f379/f9xIenr63r17pRbKysoQ8R07dlTfWvN7O0RkL4zguMAkJUpMjOC4wAgmSkyM4LjACCZKTIxgIiLLMIKJiCzDCCYisgwjmIjIMoxgIiLLMIKJiCzDCCYisgwjmIjIMoxgIiLLMIIda/78+XoREcUZRrBjGT6/352I4g0j2LEYwUTxjxHsWIxgovjHCHYsRjBR/GMEOxY/jiOKf4xgIiLLMIKJiCzDCCYisgwjmIjIMoxgx+LHcUTxjxHsWPxSGlH8YwQ7FiOYKP4xgh2LEUwU/xjBjsUIJop/jGDH4sdxRPGPEUxEZBlGMBGRZRIigh999NH09PROnTolJSV16NDh2muv/de//qU3ipWSkpL/+I//6NWrV4sWLfA/tktLS/VG5BQbNmz49a9/jVHXrFmzdu3ajRw5ct26dXqjmHj++efne+gVPgyPK664Qq+wCUyoPn36oAsrV67U6wIIPitvvPFG3NrUqVNNR0RMQkSwDCkzTIZvv/1Wbxd9VVVVY8eO1R7Mb3/7W5TrTckR5syZo11uQC7r7aIPkSr3rlf4kGb2jWAsufD4u3btWl5ertf5U++s3LVrF0qaNGny6aef1j00AhIigrt3775kyZKCggKXyzVjxgw5xbNmzdLbRd9LL70k975w4cKysrLHH39cdl9++WW9adhCWe9QtG3cuPGdd97BwDt16tRf/vIXudzXX3+93i76EiSC3W53amoqHv/s2bP1ugBCmZWDBw9Gye9+9zvTcZHhhAh+88035ZTh3GG3srLyV7/6FXaRvEVFRSjB6FeNT548KY3Hjx+vCiOl3kdy1VVXYbdt27ZnzpzBLv7HdpQurRHCZKMw1XvFzU6fPi2NJ06cqFVFxI8//jht2jSs/pKSklJSUgYOHIjXzhjw1f5eCBqm4YFeDBgwoGXLlpdddtmePXuk1hzB9Zao3eeff753796tW7fOyMjAE8/rr79+0UUXJScnDx8+3PzuX15e3i233NKtW7fmzZvjAd98880//PCDVM2cORM31apVK4SplOzYsUNuf8WKFb6H9+zZE8sp3JdUrVy5Uhp/8MEHUlLvNQplVmJBg5KmTZseOnRIFUaEEyIYcD0Mz2X77rvvFi9eLGf8rbfe0ttVV+fn50vt7bffrtfVOnbs2M2ByYUMJPgjwaDB7iWXXKLaYxslPXr0UCWRYjCCYyL4FVewDlDLq7///e9arRLO2EtPT5fbN/v++++rg0bwZ599hiBThV26dJGNIIHrWyK7WH7i1bpsw4gRI5o1a6Z2kcXSGAF67rnnqnLRuXNnSWFEp5S88sor0v6ee+7BLp4hJGf9Hj5s2DAsYFGLpzfsokd4wpPDq+u7RqHMyrfffluO+t///V9VGBEOiWA81eN84QSNHDlSnsECJewdd9whp3Lnzp16XS2MWmnj1+WXX64fYBL8kbRo0QIlo0ePViUybVCuSiLFYATHRPArDrt371aDB5H05JNPmms14Yw9SdJ+/fodPXq0vLw8Nzf3P//zP7EttYHeiJDMMjyvu0tKSmQRagQNXN8S2YX169djlSNvBRiet/vw3DNmzBjZ/frrr9EYS3XZXbZsGYIS/8sunmPk1s4//3zsTpo0qdrzRi2Wq2pXHY7CXbt24fANGzbI4U8//TRqe/XqhW0s6qWxCH6NQpmVWPzKvUyfPl0VRoRDIhi2bNki5whwGYqLi/UW1dV//vOfpcEDDzyg19V1JrCzZ8/qresK8kh8LzYmFUrwSk2VRIrhM9koSoJc8eq6EQznnHPOP/7xD3MDjT7gTIKPvb59+xqe1eJdd9313HPPYXmLF92qNlAEd+rUyTBllsqaBr0XLIf0799fdpF0UnLw4MFq07zbvn07dmWhjacKdTi2Dc8HaLL7xz/+EbutW7cuLS398MMP5dhNmzZJLZpJiWby5MnVtc9DeADqxkWQaxTKrMQSW46N+BuYzolgPFtiGMlpeuihh/Tq6upHHnlEam+77baofgMhyCPxfcmDF1BGdN6I4MdxMRPkiitut3vdunWY1WgzdOhQvToSEHCyflQQbd98843UBopgea9ABRBSXpo1IoKx2pVddV8y0dQ7MFu3bsVuUlIStrHSVIdL5KFcdr/66itp/+qrr86ePRsbaWlpFRUVUiuH+xo7dmx14AgOco1CmZV4MpBjGcEBLVmy5OdLYRgpKSn79u0z1y5YsECqQsnfcN6Pqw76SHzf+G/Tpo0RnY/jKGaCXHGNLA8RE3pFrTDHXrUnv/7v//7vvvvuk8eDl+1Srr53Vbf5z6vggQMHym44q2B1iBb3WgTLKlgtmatrV8EoVyXybmxGRkbv3r2xMXPmTFUlh0vgmsm89vtGRHXQaxTKrMRyXo7lGxH+7d+/Hy9bcIKmTJki7xz95je/Ua/C5NNMOX315m91eO/HBX8k2tdf8L/sRuNLaRQbwa/43XffjZfSJSUluNwbN25s2bIlGlxwwQV1b8MrnLGHBePOnTtPnjyJEMGKWA6ZMGGC1F599dVSsnv3bvNR1113nZSvXbsWa71w3gsOMYLxXCK7zz33XHl5Of6X3Ztuuknaw3//938bnnWxVGVlZakqeS+4SZMmy5YtwwN2uVy4WYS1/MyL+jjO/KXg4NcolFmZmZkphfw4zg+k6qhRowzPe0lFRUWvv/66nCxcRWkgu5oGPcmHqN5HUu+XwMle6r3ida50rVWrVtW5lQjR78bjb3/7m9SqZFGkXPtGRIcOHWQjSOD6lmi7wSP4hx9+kKW3WceOHeXLG+Lw4cPq2xR9+vRR5XDgwAHfb0QYtYmJcyu7H330kbSv9xqFMiv5pbRgnnrqKTlr6oeO8JRoeL6DIm+Emc+sEo0IrveRVHt+FPLBBx/EyyWMe/yPbZR4b4Jspd4rjkXlxRdfjFe+CJTOnTtjTfr222/XuYnImTt37vDhw1NTU5EUuMdLL7106dKlqhbrO7wKRNKp742pqjfeeAOv3JOTk0eMGKE+PAwSuL4l2m7wCK72pDDWwl26dME6F/9PnTrVnL8CIShH/elPf9KqDh48iL707NkTh7dt2xa9RkQeOXKk2vOeuzyL4GxI43qvUXUIs3LQoEFGdN4wdEIEk1/8OI4Sk/yAcrdu3dQneGH6/PPPJcQ/+eQTvS5sjGDHMnw+eCFKBKWlpfLNkEi94TNlyhSDv6aHGooRTBT/GMGOxQgmin+MYMdiBBPFP0awY/HjOKL4xwgmIrIMI5iIyDKMYCIiyzgqgtPS0uQb1I6BHumdpDjDUUfhcFQEY/SoXjgDeuRyudxud1lZWUVFhfnXv9aLH8fFBkcdhcN72tWW3sQ+HDkZ8vLyCgoKCgsLMSUa9AOXBr+UFhMcdRQO72lXW3oT+3DkZMjNzd23b19+fj7mg/x1rBAxgmODo47C4T3taktvYh+OnAxZWVk5OTmYD1iVqL8pGwpGcGxw1FE4vKddbelN7MORkyEzMxPzAasSvDZUf6k7FIzg2OCoo3B4T7va0pvYhyMnw9q1a7ds2ZKdnY0lCV4V6n0OjB/HxQZHHYXDe9rVlt7EPmI5GR577LH09PROnTolJSV16NDh2muvxZJBbxQ2Tob4F8tR99prr/3617/GeGvWrFm7du1Gjhy5fv16vVHYOOpiyXva1ZbexD5iORkMH5gSGK96u/AYnAxxz4jhqJszZ44+7AwDuay3C4/BURdD3tOutvQm9mHEcDJ079596dKlR44cKS4unjFjhkyGWbNm6e3Cw8kQ/2I56jZt2rR161YMObfbrf4a0PXXX6+3Cw9HXSx5T7va0pvYRwQnw1tvvSXje+HChditqqr61a9+hV0k78mTJ1GCOaAau1wuaTx+/HhVGBGcDPEvlqPOrLy8XBpPnDhRqwoTR10seU+72tKb2EcEJwPccssthudv/H3//fd/+9vfZLhnZmbq7WpqDh8+LLW33367XheecCYDP46LDUtGHVYA6i8iv/DCC1ptmMIZddRQ3tOutvQm9hHZyYC1bY8ePXCbI0eObNu2bZCEveOOO2QyfPDBB3pdeMKZDAa/lBYTMR51X3zxhQw2aNas2f/8z/+YayMinFFHDeU97WpLb2IfkZ0M8M4776jh3qtXr1OnTuktamrkz7XCAw88oNeFLZzJYDCCYyLGo84cwXDOOefs3LnT3CB8RhijjhrKe9rVlt7EPoxITwbc5oABA2SsP/TQQ3p1Tc2f//xnqb3tttuqTWczUsKZDAYjOCZiP+qgpKRk/fr1ycnJaDN06FC9OjzhjDpqKO9pV1t6E/uI+GRYunSpzARISUnZv3+/ufaRRx6Rqijlb014k8FgBMdEjEed2ciRI9GmefPmekV4whl11FDe06629Cb2EdnJ8N1337Vu3Rq3OWXKlO7du2PjN7/5TVVVldQuWLBAJsn06dOro5O/NeFNBn4cFxuxHHV33333Rx99VFpaevr06U2bNrVs2RINLrjggrq3Ea5wRh01lPe0qy29iX1EcDLg1kaNGoUb7Nq168mTJzdv3iyB++STT0oD2dVcccUVdW8mXAYnQ9wzrB51q1evrnMrYTM46mLIe9rVlt7EPozITYbFixfL+FY/epSRkWF4vi20d+/emgCTgRGcgIwYjrqZM2defPHFKSkpzZo169y584QJEzA26txEJHDUxZL3tKstvYl9RHAyxAlOhvjHUUfh8J52taU3sQ9OBoo9jjoKh/e0qy29iX1wMpjx47jY4KijcHhPu9rSm9gHJ4OZwS+lxQRHHYXDe9rVlt7EPjgZzBjBscFRR+Hwnna1pTexD04GM0ZwbHDUUTi8p11t6U3sg5PBjBEcGxx1FA7vaVdbehP74GQw48dxscFRR+Hwnna1pTexD04Gij2OOgqH97SrLb2JfaSlpRnO0rp1a06GOMdRR+FwVASDy+XKy8vLzc3NysrKzMxcGyGGZ11gCfQCfUGP0C/0Tu8wxQGOOmo0p0Ww2+0uKCjAU3dOTg7G0JYIwWTQi2IFvUBf0CP0C73TO0xxgKOOGs1pEVxWVobXTfn5+Rg9eA7PjhBMBr0oVtAL9AU9Qr/QO73DgfHjuJjhqKNGc1oEV1RU4Ekb4wbP3ngNtS9CMBn0olhBL9AX9Aj9Qu/0Dgdm8EtpscJRR43mtAiurKzEiMHzNoaOy+UqjBBMBr0oVtAL9AU9Qr/QO73DgTGCY4ajjhrNaREcJXaMMzs+ZjLjFUwEjOCQ2HEy2PExkxmvYCJgBIfEjpOBH8fZnR1HHTUUIzgknAwUexx1iYARHBJOBoo9jrpEwAgOCScDxR5HXSJgBIeEk4Fij6MuETCCQ2LHycCP4+zOjqOOGooRHBI7TgY7PmYy4xVMBIzgkNhxMtjxMZMZr2AiYASHxI6TwY6Pmcx4BRMBIzgkdpwMdnzMZMYrmAgYwSGx42Tgx3F2Z8dRRw3FCA4JJwPFHkddImAEh4STgWKPoy4RMIL9GzJkiBEAqvTW8cGOj5mCMBjBCYAR7N+iRYv0GKuFKr11fLDjY6YgDEZwAmAE+5eXl9e0aVM9yQwDhajSW8cHOz5mCsJgBCcARnBA6enpepgZBgr1dvHEjo+ZAjEYwQmAERzQ8uXL9TAzDBTq7eKJHR8zBWIwghMAIzigoqKi5ORkc5ZhF4V6u3hix8dMgRiM4ATACA5m4sSJ5jjDrt4i/tjxMZNfBiM4ATCCg9mwYYM5zrCrt4g/dnzM5JfBCE4AjOBgTp8+nZqaKlmGDezqLeKPHR8z+cUITgSM4HrcfvvtEmfY0OvilR0fM/liBCcCRnA9tm/fLnGGDb0uXtnxMZMvRnAiYATXo6qqqqcHNvS6eGXHx0y+GMGJgBFcvwc89NL4ZsfHTBpGcCJgBNfvnx56aXyz42MmDSM4ETCCKZiioqIpU6bopRQTjOBEwAimgN57772ePXsyCKzCM58IGMHkx+nTp+fMmaN+75peTTHBM58IGMGk++c//6n99ne9BcUEz3wiYASTV1VV1V//+lftF/0wCKzCM58IGMH0s7y8vDFjxmjhK/SmFBM884mAEUw/eemll9q3b69Hby29NcUEz3wiYASHpEWLdnos2VxaWpq5g8EjmCyhXSNyJEZwSDAfJk3a5qR/6JHL5XK73WVlZRUVFZWVlXwjgij2GMEhMZwYwcjcgoKCwsJCBDFSuJofxxHFHCM4JI6M4Nzc3H379uXn5yOFsRZWneWX0ohihhEcEkdGcFZWVk5ODlIYa2EshM395Y9mEMUGIzgkjozgzMxMpDDWwnl5eS6XS+8zf0CZKPoYwSFxZASvXbt2y5Yt2dnZWAgXFhbqffbgr+khiipGcEgSNoKJKKoYwSFhBBNRNDCCQxK9CH788S8OHSqprPzpSixY8LlvA99/x46dxoU7derMm2/mTZ6s14b4jxFMFA8YwSFpdARv335Yzuqnnx71rcW/oqJy1J44cXrLlkP33POhKn/44c/27CkqL6/EP2zMm/eZqlq37juUyM0++ugu39sM5R8jmCgeqOBlBAfTuAi+6abtCFA5q1jn3n77P3zbSO1zz31tLnzwwU9lXfzJJ0c//vgoNs6erfrDHz5RDf7f/9suBy5dusf3NkP5xwgmigcykWsYwcE1LoKXLfsK5/PUqTPHjpVh48UXv9UaTJ78rpxzLUmzs4+h8MMPj8juRx8dqfFZR8uBzzzzpXabIf5jBBPFA5nINYzg4BoXwfv3F+N8bt16aNOmH7Dx44+l5lrk75NP/kvO+aJFOeYqpDYKV636Rnaxgd3i4gpzG7f7pzY7d/54443vmctD/McIJooHtbnLCA6qERE8Z87Hcj7nz//8/vs/ke2HH/Z+4KZO+PbthzMy3jUfW1X104VRK1xs1HjeyjC3eeKJnNLSs3IL5vIQ/zGCieKBygFGcDCNiODMzIM4mUVF5VjtYjc/vwS7//jHj6rB228flMKysrN33pllPjaUVfDnn//0ZgVSeNu2fHN5iP8YwUTxQAUvIziYhkbwjTe+J28UaCoqqm6++X3VbObMD6R88eJc8+GfffZTvH70UZ33grOzj5nbyOd1L764z1wY+j9GMFE8UOHACA6moRGMSJWTuWvXcUQn/iFV5WyvWFHnyw/S7Omn63yq9tBD2do3IrD74IOf1ntg6P8YwUTxQCZyDSM4uIZGsHxvF/+bC+V7Dt9/f8pcKJdg5cqf33NQ/+bP//yrr376XjAWztgwv4k8yfN1N7lY/FIaka2p4GUEB9PQCA7935EjP31fraioXPvRjCD/1q37DqEsF+tPf/L+yEaD/jGCieKBCl5GcDDRi+BHHtl18KC7ET+gXFxcsWnTD761If5jBBPFA0ZwSKIXwVb9YwQTxQNGcEgYwUQUDYzgkDCCiSgaGMEhYQQTUTQwgkPCCCaiaGAEh4QRTETRwAgOCSOYiKKBERwSRjARRQMjOCSMYCKKBkZwSBjBRBQNjOCQMIKJKBoYwSFhBBNRNDCCQ8IIJqJoYASHhBFMRNHACA4JI5iIooERHBJGMBFFAyM4JIxgIooGRnBIGMFEFA2M4JAwgokoGhjBIWEEE1E0MIJDwggmomhgBIckLS3NcJbWrVszgoksxwgOlcvlysvLy83NzcrKyszMXGt/6AX6gh6hX+id3mEiij5GcKjcbndBQQEWjDk5OUiuLfaHXqAv6BH6hd7pHSai6GMEh6qsrAyv1vPz85FZWDlm2x96gb6gR+gXeqd3mIiijxEcqoqKCiwVkVZYM+KV+z77Qy/QF/QI/ULv9A4TUfQxgkNVWVmJnMJqEYHlcrkK7Q+9QF/QI/QLvdM7TETRxwgmIrIMI5iIyDKMYCIiyzCCiYgswwgmIrIMI5iIyDKMYCIiyzCCiYgs46gInj9/vvmXgWGXtaxlLWuD11rLURFMRGQvjGAiIsswgomILMMIJiKyDCOYiMgyjGAiIsswgomILMMIJiKyDCOYiMgyjGAiIsswgomILMMIJiKyDCOYiMgyjOCfpaWlmX+XEpFNYSQn4Kg299peGME/w1VUZ4DIvjCSXS6X2+0uKytLnFFt7nVFRUVlZaU+w+OVtwtqS2+SGBJnsJKzYSTn5eUVFBQUFhYmzqg29xpBjBTWZ3i88nZBbelNEkPiDFZyNozk3Nzcffv25efnJ86oNvcaKYy1sD7D45W3C2pLb5IYEmewkrNhJGdlZeXk5CCPEmdUm3uNtTAWwvoMj1feLqgtvUliSJzBSs6GkZyZmYk8wqowcUa1udd5eXkul0uf4fHK2wW1pTdJDIkzWMnZMJLXrl27ZcuW7OzsxBnV5l5jIVxYWKjP8Hjl7YLa0pskhuCDtaioqHnz5oZHRkaGXh0COfaKK67QK8K2Zs2a+R56hcehQ4dmz5590UUXpaSktGjRol+/fnfcccc333yjt4u04I8q4iJ1d9plkl1YunRpoDaRVVZW1qdPH9z+qlWrpOTGG2/E7tSpU+s2DMhwYgTPmTNn2LBhTZs2lZOvVzOCHcDvdVVWrlwp1x7atGmDeaK3qI8cG415i9uUG9cramrefffdc845Rz1yJfyoqleQRxUNkbo7uRHfCEYsVlZW+m0TWY899hhuvGvXrhUVFVKye/dulDRp0gThUretf4YTI1hdCKFXM4IdwO91VcaNG2ceARs2bNBb1EcOjMa8DZQ+Bw8elPxt2bLlkiVLjh8/Xl5ejvl87733/uUvf9EaR1ygR6UpKSnRixolxLurl9yIbwQbnhnut00E4WykpqbixvHCxVw+ePBgFP7ud78zFwYiD9VhEXz33Xe/9tprI0aMkJOvVzOCHcDvdRUnTpxISkpCg+uvvx5xho1JkyZpbXynpVbi28Bs//79V111Vc+ePZs3b56cnDxo0KDFixdX116egoKCadOmYWWEh5GSkjJw4EC8LHW5XDU+qwMhR913332yu3r16tr7+dnZs2dlAzF9yy23dOvWDfeL27/55psPHDigmsnheMx///vfsQzEAxs/fjwGt9Q27lHJNm5z+fLlvXr1wuJOK5dd30jF/c6dO7d///54GLi7Sy655I033jAfq1EHmvuIMzxr1qzi4mJV+9Zbbw0YMACX9bLLLvvyyy/lWO2qNWvWDP/jHs2Fga6s9shV7Zo1a3r37t26deuMjAw8gM2bN1900UXoy/Dhw3Nzc6XxqlWrpH1WVpaUiAULFqAQL8Pz8/PN5X4ZToxg4TsqFHOvGcG25Pe6CoSFXHgsfpFB2EAElJaWmttIg0ZH8Pvvvy8NzJDCUpuenq7XGcYPP/xQEzR9kCyG5wc31StoDbLp3HPP1Y7t3LmzSmEpadeunbnBjBkzpLZxj0q227dv77c8UJAdOXLk/PPPV4cI9XaKVi6kym8fhw0bdvr0adR+/vnn6i1+6NKli2xoVw3pjGcabGzdulUVBrqyfiMYa1s82cg2YDUnsS6QxdJ44sSJ2MVDwusVKRFIFmm5bNkyc7lfBiOYEWxHfq+rGDt2LGqxYHG73c8++6wMgnXr1pnbSGGjI/jQoUM7duzAEhJx+d1332G5hMZYC0utxES/fv2OHTtWUVGxZ8+eJ554AttSG2hctmrVCoUjR47UyhWsYeVAPMdgzqtnGqyFpYHsGp7OYv0luYnlpNQ27lGp25w3bx6extQHg1IYKMiQ+7I7ffp0xDECFE9ab775ptT6tlekj927d9+9ezf6iBez0uyZZ56pqY08wzN78WBmzpwpu75XbdGiRdjASDAXam1k128Ew6uvvnr48GF5nwGwGMdwGjNmjOzKecDLAmzjuVOOVeSHLAxP97UqXwYjmBFsR36vKyBTZMFy5ZVX1pgmA2av3jQoOSpQBJ86dQpz8rzzzpN3PARCX2r79u1reN7Sveuuu1asWIG1W1VVlTo20LisN4Jl0YcMVSXYNjyfBcmu3KxqgJsyPC+HZbdxj0oKcdfmxqo8UJDJIhTpH2hFH+ju5EBfkydPRm2nTp0MU+Spi+sbr3h2lFcDu3btCtRGdv1GcP/+/WVXziHgSRe7jz76qOziGaWm9lnN95LhKUea4UWYVuXLYAQzgu3I73UFtex94IEHcj0kehBwWMXorQOTGwkUwfLFI19Si/mpvQxHLO7du1dqA43Let+IkLhPT09XJZdffjlKUC67crNYqcmudkeNe1RSiDvyW67Oj1oeyq48VN+jlEB3Z35KM5P1rDy5jh49WhrjREmt33i9//77sX3DDTcEaVPj88hl2/ccVnvm3sKFC2V327ZtNYEjWH7hjsEIDnCVaxjBDuD3ugLmp1x1Xy+//LLeOjA5JFAEt23bFrU9evQ4ePAgdv1+8vv1119v3LhRfciGl9hSLu+TaI1rTB/HrVmzRquSj+NkFawWaDW1q2CUy64cHmh9Jxr6qLTbVOQrn6NGjZLdgQMHmg/H+tfwvJ8Q6Okk0N1JHyVwzao9Q19WwbgvKQyyCsb24cOHW7Rood7DVW2CP3KtsXYOtQgO9EYElszSjG9EmM+embnXjGBb8ntdjxw5Yv7YRHPNNdeollJiThatRHZ/8YtfvF2X1KakpKC2d+/ex48fxzBSdyq1s2fP/uCDD/BaGNGpPribMGGC1F599dVS8sUXX0iJwLyVL6Vhwf7000+fOHGivLwcbWbNmiVfSrv55pvlwBUrVlRUVOB/2b3pppvkFmQ3UHw07lFpt6nIOwbIxOLi4h07dkgzo/a+1HvBv//9748ePYqO7Ny50/xecKC7k/eCmzRpsnz5cqwlceMIu4yMjPXr16P2uuuuk6NeeeUV1AZ5L1h2b731VikxFwZ/5Frj4BGsPo5TXwoWGCfSLGE/jjvugec5OQ+yC6qBudeMYFvyO1iXLl0ql/ypp54ylw8dOtTwvA166tQpKZFm9UawL6lFmqsSzEAsh821qspsyZIlUiufFJlJeY3nRzO07zMI+S7BgQMHZBlo1rFjR/lWQ41PF7T48B5jUu+jkm3fCMb6TqratGmDZyB5I1sdFfwbETWB7y4vL8/3GxGGZ7rW+HwjokOHDrIRKIKx5FdfbFCFwR+51jh4BK9evVp2P/74Y2kg0FMjsb+UJqfFl7kBI9jezJdTkWddzKuCggJzuZo5L7zwgpTIbqMjGLc/btw4vM7t06fPhg0btIk6d+7c4cOHp6amYhJivXzppZdiVStVNZ7PapACiE6VDqqqpvYHlPHqGNGA2+/bty9WlOp7CEhhrIXxaj0pKQn/T506VeVvjU8XIvKotNtUMGewgk5OTsaLcaxYtfuSBvK9YPQCT35DhgzZvHmzqg10dzWeM4Cqnj17oo9t27bFY16wYAGW0lKLpTRe+ON+R4wYgRW09vB8H+21116rFQZ/5Frj4BFcUlIiTwPoqTQQgwYNMhL7RzPkLPkyN2AE25v5chJZRX5AuVu3bmfOnJES9R2MTz/9tG5b/8xhlDijmhFse4kzWCmelZWVyVsu6mcap0yZYiT8r+mpFyPY9hJnsJKzMYIZwbaUOIOVnI0RzAi2pcQZrORsjGBGsC0lzmAlZ2MEM4JtKXEGKzkbI5gRbEtpaWkGkf21bt1ahZH5l4I6m7nXjGC7crlceXl5ubm5WVlZmZmZa4nsyfy3hEUijGr+BWXbc7vdBQUFeArNycnBtdxCZE8YvRjDGMkFtRJhVJt7jbmsT+94xQj2Kisrw+uX/Px8XEU8l2YT2RNGL8YwRnJhrUQY1eZeYy7r0zteMYJ/xveCyRnat2+PV+JYCSKJEmdUm3uNJXBFRYU+w+MVI/hnRsJ8dkzOhpGswihxRrW514xgW0qcwUrOxghmBNtS4gxWcjaMZPWuaOKManOv+V6wLSXOYCVnw0hW3w1InFFt7jW/EWFLwQdrUVGR+vMKGRkZenUI5FjfX1UevjVr1sz30MrlHg3Pr5w/55xzBg0adOedd6pf1h6+QPcbJZG6O+1CqLO0dOnSQG0iCwu0Pn364PZXrVolJfLHW0P/dZTB4abUN2SNoKPaRubMmTNs2DD5M31+O2XuNb8XbEt+r6uycuVKufaG5+/TYBbpLeojx0ZjVvv+gQmhHrBZy5YtN27cqLVsnED3GyWRuju5Ed8IRiyqvw2qtYks+aXsXbt2VX8gbvfu3YbnD9xlZ2fXbdsYhhN/QFldJqFX1+01fzrOlvxeV2XcuHHmEbBhwwa9RX3kwGjM6kDZZL7HY8eOLVu2DE8ehudHOb///nutcSMEul9NSUmJXtQoId5dvcynRe2KtZ4/KOfbJoJwNlJTU3Hjs2fPNpcPHjzYCPlPEwUnHXFYBN99992vvfaa378sLsy9ZgTbkt/rKk6cOJGUlIQG119/PVaR2Jg0aZLWxnfSaiW+Dcz2799/1VVX9ezZs3nz5snJyYMGDVq8eHF17eUpKCiYNm0a1k14GCkpKQMHDsSLVrzUqvFZHQg5SrbN9/jqq69K4axZs1ThwYMHb7nllm7duuGu8QBQVVxcLFVh3u/y5ct79eqFxZ1WLru+kVpY+9fhcAZwd5dccskbb7xhPlajDgzSBXjrrbcGDBiAC3fZZZd9+eWXcqx2XeRPVuMezYWBrp32yFXtmjVrevfujWe4jIwMPIDNmzdfdNFF6Mvw4cPx6lgar1q1StrjJbOUiAULFhgh/4HO4AwnRrDwHTOKudeMYFvye10FokQuPBa/48ePxwYCorS01NxGGjQ6gtUfgTdDCkttenq6XmcYP/zwQ03QbJJt8z2im507d0bhhRdeKCUIL98/MDxs2LDTp0/XhHe/5l8QYy4PFGTB/0ayVi6kKngXtL+R3KVLF9nQrgvSWf4W/datW1VhoGvnN4KxtlV/ORSwXpNYF8hiaaz+TH15ebmUCGSHtAzlz9QHZzCCGcF25Pe6irFjx6IWyxm32/3ss8/KIFi3bp25jRQ2OoIPHTq0Y8cOLDArKyu/++47LKbQGGthqZUQ6dev37FjxyoqKvbs2fPEE09gW2oDjUu/9zhy5EgUtmrVSnaxyMVu9+7dd+/ejVDAaz056plnnqkJ735h3rx5eKJSHwBKYaAgmzFjhuxOnz4dcYwAxdPSm2++KbW+7ZXgXZDIMzzzEw9m5syZsut7XRYtWoQNXGtzodZGdv1GMOBFxuHDh+V9BsPzUgMDZsyYMbIr5wEvC7CNVbkcq8gXyAxP97WqhjIYwYxgO/J7XWs876LKcubKK6+sMU0VzG29aVByVKAIPnXqFGbseeedJ+94CIS+1Pbt29fwfJJ21113rVixAiu7qqoqdWygcen3HrUIlqWfr8mTJ9eEd79Yb5obq/JAQSaPpFu3buozMU2guwvehU6dOhmmyFOXzzde8fzXrl07bKu/WOzbRnb9RnD//v1lV84w4GkVu48++qjs4hmlpvZZDW2ksYKnHGmGl1laVUMZjGBGsB35va6glr0PPPBArocEE1IMaxy9dWByI4EiWL6W5EtqMXu1F+lYme7du1dqA41LKTTfI7qpRZI58c1kMRjO/V5++eV+y9XjUctD2ZVH4nuUEujugndBnj5Hjx4tjZHvUus3Xu+//35s33DDDUHa1Pg8ctlGoeyqx1ntmV0LFy6U3W3bttUEjuCysjJpxggOItAYqGEEO4Df6wqYvXLVfb388st668DkkEAR3LZtW9T26NHj4MGD2PX7ye/XX3+9cePG++67T6rwAlzK5X0SrXGNv3tUH8fde++9UiLvjUpamVWbRkb49yvkS52jRo2S3YEDB5oPx/rX8LyfEGgVHOjugndBnnJwX1IYZBWM7cOHD7do0UK9h6vaBH/kWmMtJrQIDvRGBJbM0oxvRATBCHYyv9f1yJEj5g9VNNdcc41qKSXm3NFKZPcXv/jF23VJbUpKCmp79+59/PhxDCN1p1I7e/bsDz74AK+Uz549qz64mzBhgtReffXVUvLFF19IiZBCeQC42eXLl0vQt2nTRj5Sq6l9I7VJkyaoxUKsuLgYSZGRkbF+/fqaSNyvmbxjgEzEvezYsUOaGbV9VO8F//73vz969Gh5efnOnTvN7wUHurvgXbjuuuvkqFdeeQW1Qd4Llt1bb71VSsyFwR+51jh4BKuP49SXggVGgjTjx3F+HffAs6CcJdkF1cDca0awLfkdrEuXLpVL/tRTT5nLhw4danjeJD116pSUSLN6I9iX1CLNVQnmJ5bD5lpVZbZkyRKplc+RzIIc1apVq02bNkkDyMvL8/06geEZzYFuIcT79Y1grO+kCs8BeI7BIzEfFfwbETWB7y54F7RvRHTo0EE2AkUwlvzqiw2qMPgj1xoHj+DVq1fL7scffywNBHpq8EtpgclJ82VuwAi2N/PlVORZF7OuoKDAXK7m1QsvvCAlstvoCMbtjxs3Dq+C+/Tps2HDBm0az507d/jw4ampqZiiWC9feumlTz/9tFTVeD7JQUZ07NhRZYeUq7vAUeoHlL/99lt1oMBLYBzes2fPpKQkLJNxRwsWLMA6tCa8+/WNYMwKrKCTk5PxYhwrVq2P0kC+F4zzgKe3IUOGbN68WdUGuruaoF0ALKXxwh/3O2LECKygtYfn+2ivvfZarTD4I9caB4/gkpISeRpAT6WBwNUx+KMZgck59GVuwAi2N/PlJIoe+QHlbt26nTlzRkrUdzA+/fTTum0bwxxGiTOqGcG2lziDlaxVVlYmb7msXr1aSqZMmWJE9Nf0MIIZwfaTOIOVnI0RzAi2pcQZrORsjGBGsC0lzmAlZ2MEM4JtKXEGKzkbI5gRbEuJM1jJ2RjBjGBbSpzBSs7GCGYE21LiDFZyNkYwI9iW0tLSDCL7a926tQoj8y/OdzZzrxnBduVyufLy8nJzc7OysjIzM9cS2ZP5bwmLRBjV/AvKtud2uwsKCvAUmpOTg2u5hcieMHoxhjGSC2olwqg29xpzWZ/e8YoR7FVWVobXL/n5+biKeC7NJrInjF6MYYzkwlqJMKrNvcZc1qd3vGIEe1VUVODJE9cPz6J4LbOPyJ4wejGGMZLdtRJhVJt7jbmsT+94xQj2qqysxJXD8ycuocvlUisIInvB6MUYxkiuqJUIo9rca8xlfXrHK0YwEZFlGMFERJZhBBMRWYYRTERkGUYwEZFlGMFERJZhBBMRWYYRTERkGUYwEZFlGMFERJZhBBMRWYYRTERkGUYwEZFlGMFERJZhBBMRWYYRTERkGUYwEZFlGMFERJZhBBMRWYYRTERkGUYwEZFlGMFERJZhBBMRWYYRTERkGUYwEZFlGMFERJbxE8FERBRjjGAiIsswgomILPP/AdePK6+0A2epAAAAAElFTkSuQmCC" /></p>


```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp 142

        // ステップ７
        // このブロックが終了することで、x2、x3はスコープアウトする。
        // デストラクタ呼び出しの順序はコンストラクタ呼び出しの逆になるため、
        // 最初にx3::~X()が呼び出され、この延長でx3::ptr_のデストラクタが呼び出される。
        // これによりA{0}のの共有所有カウントは1になる。
        // 次にx2::~X()が呼び出され、この延長でx2::ptr_のデストラクタが呼び出される。
        // これによりA{0}のの共有所有カウントは0になり、A{0}はdeleteされる。
        // 
    }   // x2、x3のスコープアウト
    ASSERT_EQ(0, A::LastDestructedNum());  // A{0}が解放された
    
```

<p><img src="data:image/png;base64,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" /></p>

以上で示したstd::shared_ptrの仕様の要点をまとめると、以下のようになる。

* std::shared_ptrはダイナミックに生成されたオブジェクトを保持する。
* ダイナミックに生成されたオブジェクトを保持するstd::shared_ptrがスコープアウトすると、
  共有所有カウントはデクリメントされ、その値が0ならば保持しているオブジェクトはdeleteされる。
* std::shared_ptrを他のstd::shared_ptrに、
    * moveすることことで、保持中のオブジェクトの所有権を移動できる。
    * copyすることことで、保持中のオブジェクトの所有権を共有できる。
* 下記のようなコードはstd::shared_ptrの仕様が想定するセマンティクスに沿っておらず、
  未定義動作に繋がる。

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp 162

    // 以下のようなコードを書いてはならない

    auto a0 = std::make_shared<A>(0);
    auto a1 = std::shared_ptr<A>{a0.get()};  // a1もa0が保持するオブジェクトを保持するが、
                                             // 保持されたオブジェクトは二重解放される

    auto a_ptr = new A{0};

    auto a2 = std::shared_ptr<A>{a_ptr};
    auto a3 = std::shared_ptr<A>{a_ptr};  // a3もa2が保持するオブジェクトを保持するが、
                                          // 保持されたオブジェクトは二重解放される
```

こういった機能によりstd::shared_ptrはオブジェクトの共有所有を実現している。


### オブジェクトのライフタイム <a id="SS_5_2_10"></a>
オブジェクトは、以下のような種類のライフタイムを持つ。

* 静的に生成されたオブジェクトのライフタイム
* thread_localに生成されたオブジェクトのライフタイム
* newで生成されたオブジェクトのライフタイム
* スタック上に生成されたオブジェクトのライフタイム
* prvalue(「[rvalue](term_explanation.md#SS_5_5_3)」参照)のライフタイム

なお、リファレンスの初期化をrvalueで行った場合、
そのrvalueはリファレンスがスコープを抜けるまで存続し続ける。


### クラスのレイアウト <a id="SS_5_2_11"></a>
クラス(やそのクラスが継承した基底クラス)が仮想関数を持たない場合、
そのクラスは、非静的なメンバ変数が定義された順にメモリ上に配置されたレイアウトを持つ
(CPUアーキテクチャに依存したパディング領域が変数間に挿入されることもある)。
このようなクラスは[POD](term_explanation.md#SS_5_1_6)
(C++20では、[PODという用語は非推奨](https://cpprefjp.github.io/lang/cpp20/deprecate_pod.html)
となり、トリビアル型とスタンダードレイアウト型に用語が分割された)とも呼ばれ、
C言語の構造体のレイアウトと互換性を持つことが一般的である。

クラス(やそのクラスが継承したクラス)が仮想関数を持つ場合、
仮想関数呼び出しを行う(「[オーバーライドとオーバーロードの違い](term_explanation.md#SS_5_10_1)」参照)
ためのメモリレイアウトが必要になる。
それを示すために、まずは下記のようにクラスX、Y、Zを定義する。


```cpp
    // @@@ example/term_explanation/class_layout_ut.cpp 4

    class X {
    public:
        virtual int64_t GetX() { return x_; }
        virtual ~X() {}

    private:
        int64_t x_{1};
    };

    class Y : public X {
    public:
        virtual int64_t GetX() override { return X::GetX() + y_; }
        virtual int64_t GetY() { return y_; }
        virtual ~Y() override {}

    private:
        int64_t y_{2};
    };

    class Z : public Y {
    public:
        virtual int64_t GetX() override { return Y::GetX() + z_; }
        virtual int64_t GetY() override { return Y::GetY() + z_; }
        virtual int64_t GetZ() { return z_; }
        virtual ~Z() override {}

    private:
        int64_t z_{3};
    };
```

通常のC++コンパイラが作り出すX、Y、Zの概念的なメモリレイアウトは下記のようになる。

<p><img src="data:image/png;base64,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" /></p>

各クラスがvtblへのポインタを保持するため、このドキュメントで使用している[g++](term_explanation.md#SS_5_9_1)では、
sizeof(X)は8ではなく16、sizeof(Y)は16ではなく24、sizeof(Z)は24ではなく32となる。

g++の場合、以下のオプションを使用し、クラスのメモリレイアウトをファイルに出力することができる。

```cpp
    // @@@ example/term_explanation/Makefile 19

    CCFLAGS_ADD:=-fdump-lang-class
```

X、Y、Zのメモリレイアウトは以下の様に出力される。

```
    Vtable for X
    X::_ZTV1X: 5 entries
    0     (int (*)(...))0
    8     (int (*)(...))(& _ZTI1X)
    16    (int (*)(...))X::GetX
    24    (int (*)(...))X::~X
    32    (int (*)(...))X::~X

    Class X
       size=16 align=8
       base size=16 base align=8
    X (0x0x7f54bbc23a80) 0
        vptr=((& X::_ZTV1X) + 16)

    Vtable for Y
    Y::_ZTV1Y: 6 entries
    0     (int (*)(...))0
    8     (int (*)(...))(& _ZTI1Y)
    16    (int (*)(...))Y::GetX
    24    (int (*)(...))Y::~Y
    32    (int (*)(...))Y::~Y
    40    (int (*)(...))Y::GetY

    Class Y
       size=24 align=8
       base size=24 base align=8
    Y (0x0x7f54bbc3f000) 0
        vptr=((& Y::_ZTV1Y) + 16)
      X (0x0x7f54bbc23d20) 0
          primary-for Y (0x0x7f54bbc3f000)

    Vtable for Z
    Z::_ZTV1Z: 7 entries
    0     (int (*)(...))0
    8     (int (*)(...))(& _ZTI1Z)
    16    (int (*)(...))Z::GetX
    24    (int (*)(...))Z::~Z
    32    (int (*)(...))Z::~Z
    40    (int (*)(...))Z::GetY
    48    (int (*)(...))Z::GetZ

    Class Z
       size=32 align=8
       base size=32 base align=8
    Z (0x0x7f54bbc3f068) 0
        vptr=((& Z::_ZTV1Z) + 16)
      Y (0x0x7f54bbc3f0d0) 0
          primary-for Z (0x0x7f54bbc3f068)
        X (0x0x7f54bbc43060) 0
            primary-for Y (0x0x7f54bbc3f0d0)
```

このようなメモリレイアウトは、

```cpp
    // @@@ example/term_explanation/class_layout_ut.cpp 40

    auto z_ptr = new Z;
```

のようなオブジェクト生成に密接に関係する。その手順を下記の疑似コードにより示す。

```cpp
    // ステップ1  メモリアロケーション
    void* ptr = malloc(sizeof(Z));

    // ステップ2  ZオブジェクトのX部分の初期化
    X* x_ptr = (X*)ptr;
    x_ptr->vtbl = &vtbl_for_X       // Xのコンストラクタ呼び出し処理
    x_ptr->x_ = 1;                  // Xのコンストラクタ呼び出し処理

    // ステップ3  ZオブジェクトのY部分の初期化
    Y* y_ptr = (Y*)ptr;
    y_ptr->vtbl = &vtbl_for_Y       // Yのコンストラクタ呼び出し処理
    y_ptr->y_ = 2;                  // Yのコンストラクタ呼び出し処理

    // ステップ4  ZオブジェクトのZ部分の初期化
    Z* z_ptr = (Z*)ptr;
    z_ptr->vtbl = &vtbl_for_Z       // Zのコンストラクタ呼び出し処理
    z_ptr->z_ = 3;                  // Zのコンストラクタ呼び出し処理
```

オブジェクトの生成がこのように行われるため、Xのコンストラクタ内で仮想関数GetX()を呼び出した場合、
その時のvtblへのポインタはXのvtblを指しており(上記ステップ2)、X::GetX()の呼び出しとなる
(Z::GetX()の呼び出しとはならない)。

なお、オブジェクトの解放は生成とは逆の順番で行われる。


## オブジェクトのコピー <a id="SS_5_3"></a>

### シャローコピー <a id="SS_5_3_1"></a>
シャローコピー(浅いコピー)とは、暗黙的、
もしくは=defaultによってコンパイラが生成するようなcopyコンストラクタ、
copy代入演算子が行うコピーであり、[ディープコピー](term_explanation.md#SS_5_3_2)と対比的に使われる概念である。

以下のクラスShallowOKには、コンパイラが生成するcopyコンストラクタ、
copy代入演算子と同等なものを定義したが、これは問題のないシャローコピーである
(が、正しく自動生成される関数を実装すると、メンバ変数が増えた際にバグを生み出すことがあるため、
実践的にはこのようなことはすべきではない)。

```cpp
    // @@@ example/term_explanation/deep_shallow_copy_ut.cpp 7

    class ShallowOK {
    public:
        explicit ShallowOK(char const* str = "") : str_{std::string{str}} {}
        std::string const& GetString() const noexcept { return str_; }

        // 下記2関数を定義しなければ、以下と同等なcopyコンストラクタ、copy代入演算子が定義される。
        ShallowOK(ShallowOK const& rhs) : str_{rhs.str_} {}

        ShallowOK& operator=(ShallowOK const& rhs)
        {
            str_ = rhs.str_;
            return *this;
        }

    private:
        std::string str_;
    };
```

コンストラクタでポインタのようなリソースを確保し、
デストラクタでそれらを解放するようなクラスの場合、シャローコピーは良く知られた問題を起こす。

下記のShallowNGはその例である。

```cpp
    // @@@ example/term_explanation/deep_shallow_copy_ut.cpp 43

    class ShallowNG {
    public:
        explicit ShallowNG(char const* str = "") : str_{new std::string{str}} {}
        ~ShallowNG() { delete str_; }
        std::string const& GetString() const noexcept { return *str_; }

    private:
        std::string* str_;
    };
```

シャローコピーにより、メンバで保持していたポインタ(ポインタが指しているオブジェクトではない)
がコピーされてしまうため、下記のコード内のコメントで示した通り、
メモリリークや2重解放を起こしてしまう。

```cpp
    // @@@ example/term_explanation/deep_shallow_copy_ut.cpp 60

    auto const s0 = ShallowNG{"s0"};

    // NG s0.str_とs1.str_は同じメモリを指すため~ShallowNG()に2重解放される。
    auto const s1 = ShallowNG{s0};

    auto s2 = ShallowNG{"s2"};

    // NG s2.str_が元々保持していたメモリは、解放できなくなる。
    s2 = s0;

    // NG s0.str_とs2.str_は同じメモリを指すため、
    //    s0、s2のスコープアウト時に、~ShallowNG()により、2重解放される。
```

### ディープコピー <a id="SS_5_3_2"></a>
ディープコピーとは、[シャローコピー](term_explanation.md#SS_5_3_1)が発生させる問題を回避したコピーである。

以下に例を示す。

```cpp
    // @@@ example/term_explanation/deep_shallow_copy_ut.cpp 79

    class Deep {
    public:
        explicit Deep(char const* str = "") : str_{new std::string{str}} {}
        ~Deep() { delete str_; }
        std::string const& GetString() const noexcept { return *str_; }

        // copyコンストラクタの実装例
        Deep(Deep const& rhs) : str_{new std::string{*rhs.str_}} {}

        // copy代入演算子の実装例
        Deep& operator=(Deep const& rhs)
        {
            *str_ = *(rhs.str_);
            return *this;
        }

    private:
        std::string* str_;
    };

    class Deep2 {  // std::unique_ptrを使いDeepをリファクタリング
    public:
        explicit Deep2(char const* str = "") : str_{std::make_unique<std::string>(str)} {}
        std::string const& GetString() const { return *str_; }

        // copyコンストラクタの実装例
        Deep2(Deep2 const& rhs) : str_{std::make_unique<std::string>(*rhs.str_)} {}

        // copy代入演算子の実装例
        Deep2& operator=(Deep2 const& rhs)
        {
            *str_ = *(rhs.str_);
            return *this;
        }

    private:
        std::unique_ptr<std::string> str_;
    };
```

上記クラスのDeepは、copyコンストラクタ、copy代入演算子でポインタをコピーするのではなく、
ポインタが指しているオブジェクトを複製することにより、シャローコピーの問題を防ぐ。


### スライシング <a id="SS_5_3_3"></a>
オブジェクトのスライシングとは、

* クラスBaseとその派生クラスDerived
* クラスDerivedのインスタンスd1、d2(解説のために下記例ではd0も定義)
* d2により初期化されたBase&型のd2_ref(クラスBase型のリファレンス)

が宣言されたとした場合、 

```cpp
    d2_ref = d1;    // オブジェクトの代入
```

を実行した時に発生するようなオブジェクトの部分コピーのことである
(この問題はリファレンスをポインタに代えた場合にも起こる)。

以下のクラスと単体テストはこの現象を表している。

```cpp
    // @@@ example/term_explanation/slice_ut.cpp 10

    class Base {
    public:
        explicit Base(char const* name) noexcept : name0_{name} {}
        char const* Name0() const noexcept { return name0_; }

        ...
    private:
        char const* name0_;
    };

    class Derived final : public Base {
    public:
        Derived(char const* name0, char const* name1) noexcept : Base{name0}, name1_{name1} {}
        char const* Name1() const noexcept { return name1_; }

        ...
    private:
        char const* name1_;
    };

    TEST(Slicing, reference)
    {
        auto const d0     = Derived{"d0", "d0"};
        auto const d1     = Derived{"d1", "d1"};
        auto       d2     = Derived{"d2", "d2"};
        Base&      d2_ref = d2;

        ASSERT_STREQ("d2", d2.Name0());  // OK
        ASSERT_STREQ("d2", d2.Name1());  // OK

        d2 = d0;
        ASSERT_STREQ("d0", d2.Name0());  // OK
        ASSERT_STREQ("d0", d2.Name1());  // OK

        d2_ref = d1;                     // d2_refはBase&型で、d2へのリファレンス
        ASSERT_STREQ("d1", d2.Name0());  // OK
    #if 0
        ASSERT_STREQ("d1", d2.Name1());  // 本来ならこうなってほしいが、
    #else
        ASSERT_STREQ("d0", d2.Name1());  // スライシングの影響でDerived::name1_はコピーされない
    #endif
    }
```

copy代入演算子(=)によりコピーが行われた場合、=の両辺のオブジェクトは等価になるべきだが
(copy代入演算子をオーバーロードした場合も、そうなるように定義すべきである)、
スライシングが起こった場合、そうならないことが問題である(「[等価性のセマンティクス](term_explanation.md#SS_5_8_1)」参照)。

下記にこの現象の発生メカニズムについて解説する。

1. 上記クラスBase、Derivedのメモリ上のレイアウトは下記のようになる。

<p><img src="data:image/png;base64,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" /></p>

2. 上記インスタンスd0、d1、d2、d2_refのメモリ上のレイアウトは下記のようになる。

<p><img src="data:image/png;base64,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" /></p>

3. d2 = d0をした場合の状態は下記のようになる。

<p><img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAcwAAAEYCAIAAABeFlHoAAApCElEQVR4Xu2dCXRUVZrHH0llEYQGYiQBwhLATmyHIzBnEEEJiy2Ntks4hAyLBkkgtLZHBYG2GUDtI4jOqCSsx+k+Ot2ADjZn2BrZBUS2RpYEZLegMAFMqCSQhSRkvspHXi73JhUqyXupevX/ne/Uuffde997fLfql1uvXhVaBQAAAMPQ5A0AAAAaD0gWAAAMpFqytwAAADQSkCwAABgIJAsAAAYCyQIAgIFAsgAAYCCQLAAAGAgkCwAABgLJAgCAgUCyAABgIJAsAAAYCCQLAAAGAskCAICBQLIAAGAgkCwAABgIJAsAAAYCyQIAgIFAsgAAYCCQLAAAGAgkCwAABgLJAgCAgUCyAABgIJAsAAAYCCQLAAAGAskCAICBQLIAAGAgkCwAABgIJAsAAAYCyQIAgIFAsgAAYCCQLAAAGAgkCwAABgLJAgCAgVhKsmFhYRqwIjSz+iwHB7eSm/0MMRstWreQm/0SMSfehqUkS7nW/xXAStDMOp3OgoKCwsJCKo8cudmfQ8rG0rNLEWJOSkpKysrKZDs0HdVPY70kd/EdIFmrQjNrt9uzsrJycnIgWSkbqnH8MMSckGrJs7Idmo7qp7Fekrv4DpCsVaGZPXbs2OnTpx0OByQrZUM1jh+GmBPyLK1nZTs0HdVPY70kd/EdIFmrQjO7e/fuw4cP06sIkpWyoRrHD0PMCa1naTEr26HpqH4a6yW5i+8AyVoVmtkNGzbQq4hWK5CslA3VOH4YYk7sdrvT6ZTt0HRUP431ktzFd4BkrQrN7IoVKzZu3Lh//35IVsqGahw/DDEntJjNycmR7dB0VD+N9ZLcxXeAZK0KJCsGJKsGJGsSkKxVgWTFgGTVgGRNApK1KpCsGJCsGpCsSUCyVgWSFQOSVQOSNQlI1qpAsmJAsmpAsiYByVoVSFYMSFYNSNYkIFmrAsmKAcmqAcmaBCRrVSBZMSBZNSBZk4BkrQokKwYkqwYkaxKQrFWBZMWAZNWAZE0CkrUqkKwYkKwakKxJQLJWBZIVA5JVA5I1CUjWqkCyYkCyakCyJgHJWhVIVgxIVg1I1iQaIlmtkiFDhsgNnvPZZ5/NrkRu8FbUf/uNGzf+8Ic/dO7cOTg4mB6pXFhYKIwwG0hWDEhWDUjWJLxEsrQT3pvc4K1I/3bK5NChQ3mjzhNPPHFLeLqYjAbJCiFlQzWOH4aYE0jWQDTv8JqvS3b58uW8Zd68eUVFRXPnzuUqPYnvHGcefHRIlkPKhmocPwwxJ5CsgWgN8Bp7RBeNXv3888+jo6NDQkKGDx9OM8etWVlZSUlJkZGRNputefPmsbGx48aNczqd+kAJHnXmzJmnn346KioqKCiIdvjQQw998sknt6rmgHvWdkSCylOnTn3ggQeoiQ7au3fvtWvX6q0XLlwYP358+/btaed0iFdffTUvL09vlVi/fn1MTExoaOiAAQMyMzP1Q3MrnSRVW7ZsWVpaSlV6pDJt+e1vf3vHXkxE803Jzpp1ICMjt7i4jIIKM2ceUPvUI6RsqMbxzug9rHfrdq35yfbi/BfVDg0JMSeQrIFojS3ZVq1acYGZOHEit8bFxYnbmfPnz+sDJXjU9u3b5QZNI8/ezRGzs7O7du0qNhH6ZV8ybLt27aTWXr160TqUO4gcPHiQRKx3i4iI4IL+bydTU5Ukrg+hMm3p2LGjvsVkNB+U7IwZ+8rKXC+wvXsvf/fd5QrXn6vyadP2qj09DSkbqnG8M8iwDz/xMD/ZIFlfRWtsyRJffPGFw+Fo3dr1F5jsw60sqR49ely5cqWkpCQjI2P+/PlU5tbaLhdcvHhxx44dtOAtKys7e/Zsly5dqA+tZ7nV/RHJttyakpJCwiV7krLXrVvHrbSspqYOHTocOnSouLj4q6++4s6LFi3iDiLx8fHcSk/KGzduTJ48mav6vz04OJiqgwYN0ofwHxXarm8xGT7bekiWh69eff748VxynMNxXdfczJn7z57Nu3GjlJ45hYWlmZm5s2YdFEetWnWORt28WX7uXP6UKd+tXHnG6SyhNenWrZcSE7dwz+Tkb7ZsceTmFpNP6XHTpotJSTu4af9+1/Ph22+zubpnTzZV9+27rJ6kpyFlQzWOm+C5HpY6rMe/9bAF2SK7R/5xzR+5adr/Tuv8UOd7Wt7TLKBZ6L2hD/R9YOqKqeKo4S8Pd40KtnX6VadZG2Y9+8azre5rFdI8pH9C/8WnFnPPD/Z9MGDUAPJpQGDAL+7/xeOjH//o0EfqCUCyvoqmeO3u4bmXJEsa5Wr//v2pGhAQwNXu3btTld5uv/zyy59++imtDcvLy7mponbJ5ufn07v4Tp062Ww27kDQe39u5WptR4yMjNQqnUuC5i0i3KqSkJAgd62oCA8Pp6aYmBiuktC5sxvJDhw4UBNO1Xy0hkmWDLh9+6WcnGIqnzrl5KYPPjhCYiVF/uMfF3744Ro1kUAnTPhGH0V63bHjp6tXXe8GqCk7u/Cbb37il8zixZnUbcyYrT/9dIOq58/nf/31RVI27z8hwaXg/PybVP3zn3/gw1GBqnl5JepJehpSNlTjuAme60Bb4KMjHuU3710f7spNqYtTSayPJT426IVB3fp0o6bge4I/PPChPor02i++X9v2bV1PhuYh4Z3CH3n+kWbNmlH1hbkvULf0zPR2XV3vqKIejBo4ZiApm/e/5PQS6QQgWV9FU7x29/DcS5IdPHgwVyVv0ipSevNOcjx58mSNnXVGjx4tDtHhVi7XdkT2MsmOqxKitUWGDh0qd62oCAwM1ASHkrW5s5vLBb169dJ883IBD9+w4cLIKs3RelZvfeedf9L6dO1a+8aNF7jnhx8ekUb95S+3p5WXwGfOuEy6ebODygsWHKtwvV4qyLDr19tpGcs9Z892rYjLy12vrkWLXDqmoEJFpe6lM6xHSNlQjeMmeK5Jo1ROnJ1IZVrP6q2v/8/rtD4dOmFo3Lg47pm6KFUfNfjFwVRO+I8Ers5cO5Oqnf/FZVJSM5Vf+q+XqEzaJcMOSRpCy1juOWX5FOkEIFlfRVO8dvfw3EuS1as1evPEiROrV69+7bXXuInes/N2/f4nsTPBHx+Rqi5ccL2k+/XrJ3bjcm1HZPF16NChxpUsX1dVlXqrppuueCUbGxvLVXUlq37wde+992q++cEXD1+y5DiVFy50aY7gJjJj9QGqSE/PqG0UXyLIyMilMq1wqbx8+Rl9oEhammsn3rySHffeOCqT6bjKTWRGrookfZBU2yi+RECLXyrTCpfKz015Th8oMv7D8dIJQLK+iqZ47e7hub9Lyb7xxhu7du1yOp0kIP3jrKeeeopbn3nmGd7y/fff8xamefPmtLFLly5Xr16lZwOvKPV9crm2I+rXZFNTUy9fvlxcXLxz507pmiytIJYtW1ZYWJiXl7d58+bExMQvv/ySO4iMGDGCd7Vy5UrqrF6TlW7hokeurvDBW7h4OIlypCLZkhLXFZ516+yjR2+dOHGn2NPNqMzMaskuWJBBZVqxpqbu1I84efKuUaNcOj5wwHVNds+eO67J7t9/Re9Z75CyoRrHTfBUsuMkyQaFuj5pGPrS0IUnFs7/br7Y080oUbIv/adrJRsQGDDv23n6EefumqtfsZV21YihQbLmoJklWS5LpKWlcatuJR3e/uyzz+pbgoKCaEkrtnK5tiO6v7vAbrerdxdotWhRurugbVvXJTZNOPQtC30ZgYfXqMusLNfl1OzsQnqb73BcF3u6GSVKduzYbXxN9tq1kq1bL23Z4jh1yklZGjduG7W+9dZ+6e4Cqs6YsU89SU9DyoZqHDfBs1mjLu/vfD+VwzuF09v8yO6RYk83o0TJpmWk3d/FtZNW4a36J/R/LPGx6F7RzQKaLTi2gFqpDwWP7danG1fVM6xfaJCsOWhmSXbq1Kl9+/Zt06ZNQEAArU/79OmzcOFCbiJo9ZeSknLffffxZwL6qKysrCeffDI4ODg6OnrVqlU1iru2I1YI98nSHkJDQ3v27LlmzRq99eLFi3TQqKgom81G7/Tp9ObMmUNrXr2DCC2BY2JiQkJC+vXrR8tt6dAVlV+rnTFjRufOnUnH9Ehl2iLswGw0AyRLEvzxxwISH9n2/fcPiz3djBIlO7Ly7oJNmxw//1xM69ni4rJz5/LXrPlRv/dg9uyDx4+77pOlVTMV9LsXGhhSNlTjuAme6xp1OeOrGR1jOtI6lGz7u6W/E3u6GSVKdmnl3QWP//vjbSLa0H5CmodEPRj165Rf80qWR0moZ1i/0CBZc9AaIFngzYgvIc0TyVoypGyoxvHDgGRNApK1KhaT7Pr1djXUbrWFxSQ7JGmIGmo39wHJmgQka1WMkyzt/NKlG1Sgx4qqawJq8GkcPZqjbxk7dtvf/37+ypWi0tJyeqTymDFb73K31f8wAamPmzBOsrS3dtHtqECPbvasVRLzaIy+JS0j7TeTfxPWIcwWZKNHKqdnpt/lbnlvElKfOkODZM1Bg2QtivgS0rxAsgkJm6ksniFx5EgObfdot/ULKRuqceodmlsbit00QbJLziyJ7R/LG3ViB8TSdo9225DQIFlz0CBZiyK+hDQvkOzHH7u+hkD89a+nR4/e+re/nebqxx8f9Wi39QspG6px6h13aUPWqC7Z5I+TeUv8tPj04+nPv/k8V5M/SfZotw0JDZI1Bw2StSjiS0gzRbLvvfe9w3H95s3yEyeuvf76Hj4NXbIHD16lamFhGd9IQI9UrnDdHnf7NtjadtsoIWVDNU69ozYb/v6/fx/RLSIoJKj7v3af8/Ucdqgu2Z6De1I1tEUo30VAj1SmLT2H9HS/20YMMSeQrIFokKxFkbSieqdxY/r02z+gxVy7VsIFXbK5ua6fQTh3Ll8fQmXa8vPPxereGj2kbKjGadx46//eCrTd/uKMVnkPLBd0yfJvIHT6VSd9CJVpS5uINureDAoxJ5CsgWiQrEURX0Ka8ZLdu/f2/cX09n/s2G36F3B1yZaWur4tlpGRqw/hr9uKP4xgXEjZUI3TuNHrSdcvV2iVb//TMtL0b9/qkrUFuX4645eP/FIfwjfPir+KYHRAsiYByVoVkyWbl+daujoc17k6adLtL926kSx/SeHmTQtKtmVb129uRHSL4Or7e96/W8kGQ7Iuqp/Geknu4jtAslbFZMnyD2jpDh01agufhn9eLggIDBAduvjUYkmy6uWCqAejNFwuqKL6aayX5C6+AyRrVUyWbJ0rWfWDr6KiOz74MjRMliyvZCO7R3JVXcmqH3yFNA/RhA++TAhI1iQgWatismT1a7IffXR0zJit6jVZ6RYueuSqfguXoWGyZHsPc/3nQ0TKgpT0zHT1mqx0Cxc9clW/hcuE0CBZc9AgWYsivoQ04yUr3V1QUOD6ZdgKT76MYGhI2VCN07gh3V3QonULLtz9lxFMCA2SNQcNkrUo4ktIM16yFHPn3r5P9uRJ55tv7uXTkL5Wu3q162u1pGN6pDJtUfdjREjZUI3T6PHKp69EdIuwBduie0XPXDuTNSp9rXZY6rCwDmGkY3qkMm1R92NcQLImAclaFfMl681hvmS9PyBZk4BkrQokKwYkqwYkaxKQrFWBZMWAZNWAZE0CkrUqkKwYkKwakKxJQLJWBZIVA5JVA5I1CUjWqkCyYkCyakCyJgHJWhVIVgxIVg1I1iQgWasCyYoByaoByZoEJGtVIFkxIFk1IFmTCAsL04AVadGihf4Sat3a9ZtP/gyyoSLmBJI1FqfTabfbjx07tnv37g0bNqwAVoFmk+aUZtZehT/PMrKhIuaEPCCroemwmmQLCgqysrLoT9nhw4cp4xuBVaDZpDmlmc2qwp9nGdlQEXNCHpDV0HRYTbKFhYX0TsHhcFCu6W/afmAVaDZpTmlmc6rw51lGNlTEnJAHZDU0HVaTbElJCf0RoyzTXzN613AaWAWaTZpTmtmCKvx5lpENFTEn5AFZDU2H1SRbVlZG+aW/Y5Rop9Op/50Hvg7NJs0pzWxJFf48y8iGipgT8oCshqbDapIFAACvApIFAAADgWQBAMBAIFkAADAQSBYAAAwEkgUAAAOBZAEAwEAgWQAAMBBIFoA66NGjh7zJj0E2PAWSBaAONE2TN/kxyIanQLIA1AG0IoJseAokC0AdQCsiyIanQLIA1AG0IoJseAokC0AdQCsiyIanQLIA1AG0IoJseAokC0AdQCsiyIanQLIA1AG0IoJseAokC0AdQCsiyIanQLIA1AG0IoJseAokC0AdQCsiyIanQLIA1AG0IoJseAokC0AdQCsiyIanWEqyPXr00OoLflsI1IYGrQggG55iKckCYATQigiy4SmQLAB1AK2IIBueAskCUAfQigiy4SmQLAB1AK2IIBueAskCUAfQigiy4SmQLAB1AK2IIBueAskCINOzZ0/XbX01QU1yb6uDbDQQSBYAmXnz5sk6qYKa5N5WB9loIJAsADJ2uz0gIEA2iqbRRmqSe1sdZKOBQLIA1EBcXJwsFU2jjXI//wDZaAiQLAA1sGzZMlkqmkYb5X7+AbLRECBZAGogNzc3JCREdApVaaPczz9ANhoCJAtAzcTHx4taoarcw59ANuoNJAtAzaxatUrUClXlHv4EslFvIFkAaqaoqKhNmzbsFCpQVe7hTyAb9QaSBaBWkpOTWStUkNv8D2SjfkCyANTKtm3bWCtUkNv8D2SjfkCyANRKeXl5VCVUkNv8D2SjfkCyALhjeiXyVn8F2agHkCwA7jhSibzVX0E26gEkCwAABgLJAgCAgUCyAABgIJAsAAAYCCQLAAAGAskCAICBQLIAAGAglpJsWFgYf+0PWAyaWX2Wg4Nbyc1+hpiNFq1byM1+iZgTb8NSkqVc6/8KYCVoZp1OZ0FBQWFhIZVHjtzszyFlY+nZpQgxJyUlJWVlZbIdmo7qp7Fekrv4DpCsVaGZtdvtWVlZOTk5kKyUDdU4fhhiTki15FnZDk1H9dNYL8ldfAdI1qrQzB47duz06dMOhwOSlbKhGscPQ8wJeZbWs7Idmo7qp7Fekrv4DpCsVaGZ3b179+HDh+lVBMlK2VCN44ch5oTWs7SYle3QdFQ/jfWS3MV3gGStCs3shg0b6FVEqxVIVsqGahw/DDEndrvd6XTKdmg6qp/Geknu4jtAslaFZnbFihUbN27cv38/JCtlQzWOH4aYE1rM5uTkyHZoOqqfxnpJ7uI7QLJWBZIVA5JVA5I1CUjWqkCyYkCyakCyJgHJWhVIVgxIVg1I1iQgWasCyYoByaoByZoEJGtVIFkxIFk1IFmTgGStCiQrBiSrBiRrEpCsVYFkxYBk1YBkTQKStSqQrBiQrBqQrElAslYFkhUDklUDkjUJSNaqQLJiQLJqQLImAclaFUhWDEhWDUjWJCBZqwLJigHJqgHJmgQka1UgWTEgWTUgWZOAZK0KJCsGJKsGJGsSDZSsVsmQIUPkBs/57LPPZlciN3gr0r/9T3/6U1xcXHh4uM1ma9u27XPPPXfs2LE7R5gKJCsGJKsGJGsS3iNZ2gnvTW7wVqR/O1dFWrVqRc/dOweZhwbJCiFlQzWOH4aYE0jWQDSvkZqvS7ZDhw7p6enZ2dl5eXkTJ07k1ldfffXOQeYhaUX1jl+FlA3VOH4YYk4gWQNpoNQk0ejVzz//PDo6OiQkZPjw4TR53JqVlZWUlBQZGUlvqJs3bx4bGztu3Din06kPlOBRZ86cefrpp6OiooKCgmiHDz300CeffHKrahq4Z21HJKg8derUBx54gJrooL179167dq3eeuHChfHjx7dv3552TocgJ5Ii9VaJ9evXx8TEhIaGDhgwIDMzUz80txYUFOg96R/FrXQy+kaT0XxTsrNmHcjIyC0uLqOgwsyZB9Q+9QgpG6pxvDN6D+vdul1rfi69OP9FtUNDQswJJGsgmgGSpbfJXGBoWcetcXFx4nbm/Pnz+kAJHrV9+3a5QdPIs3dzRFpXdu3aVWwi9Mu+ZNh27dpJrb169SoqKuIOIgcPHiQR690iIiK4UOOlkkuXLnFrcnKy3GYWmg9KdsaMfWVlrhfY3r2Xv/vuMhVKS8unTdur9vQ0pGyoxvHOIMM+/MTD/FyCZH0VzQDJEl988YXD4Wjd2vVHmNaJ3MqS6tGjx5UrV0pKSjIyMubPn09lbq3tcsHFixd37NhBa8OysrKzZ8926dKF+tB6llvdH1F/256SkkLCJXuSstetW8ettKymJnqbf+jQoeLi4q+++oo7L1q0iDuIxMfHcys9L2/cuDF58mSu1ijZSZMmceuuXbvkNrPgU62HZHn46tXnjx/PJcc5HNd1zc2cuf/s2bwbN0rpmVNYWJqZmTtr1kFx1KpV52jUzZvl587lT5ny3cqVZ5zOElqTbt16KTFxC/dMTv5myxZHbm4x+ZQeN226mJS0g5v273c9Gb79Npure/ZkU3XfvsvqSXoaUjZU47gJnsphqcN6/FsPW5AtsnvkH9f8kZum/e+0zg91vqflPc0CmoXeG/pA3wemrpgqjhr+8nDXqGBbp191mrVh1rNvPNvqvlYhzUP6J/RffGox9/xg3wcDRg0gnwYEBvzi/l88Pvrxjw59pJ4AJOuraIrUPIKnX5IsaZSr/fv3p2pAQABXu3fvTlV6u/3yyy9/+umntDYsLy/nporaJZufn0/v4jt16mSz2bgDQe/9uZWrtR0xMjJSq3QuCZq3iHCrSkJCgty1oiI8PJyaYmJiuEpC586qZN99911umj59utRkJlrDJEsG3L79Uk5OMZVPnXJy0wcfHCGxkiL/8Y8LP/xwjZpIoBMmfKOPIr3u2PHT1auutwLUlJ1d+M03P/FLZvHiTOo2ZszWn366QdXz5/O//voiKZv3n5DgUnB+/k2q/vnPP/DhqEDVvLwS9SQ9DSkbqnHcBM9moC3w0RGP8pv3rg935abUxakk1scSHxv0wqBufbpRU/A9wR8e+FAfRXrtF9+vbfu2VCa3hncKf+T5R5o1a0bVF+a+QN3SM9PbdXW9nYp6MGrgmIGkbN7/ktNLpBOAZH0VTZGaR/D0S5IdPHgwVyVv0ipSevNOcjx58mSNnXVGjx4tDtHhVi7XdkT28sCBA7kqIVpbZOjQoXLXiorAwEBqGjRoEFfJ2txZkuw777zD2ydMmHBLeK6Yj9YwyW7YcGFkleZoPau3vvPOP2l9unatfePGC9zzww+PSKP+8pfbc8pL4DNnXCbdvNlB5QULXLe1UWLIsOvX22kZyz1nz3atiMvLXRlbtMilYwoqVFTqXjrDeoSUDdU4boInlDRK5cTZiVSm9aze+vr/vE7r06EThsaNi+OeqYtS9VGDXxxM5YT/SODqzLUzqdr5X1wmJTVT+aX/eonKpF0y7JCkIbSM5Z5Tlk+RTgCS9VU0RWoewdMvSVav1ujNEydOrF69+rXXXuMmes/O20ltameiZcuWtLFjx44XLrhe1f369RO7cbm2I9IaVqu8IFDjSpavq6pKvVWTHHklGxsby9UaV7Jvv/02b2xyw1Y0WLJLlhyn8sKFLs0R3ERmrD5AFenpGbWN4ksEGRm5VKYVLpWXLz+jDxRJS3PtxJtXsuPeG0dlMh1XuYnMyFWRpA+SahvFlwho8UtlWuFS+bkpz+kDRcZ/OF46AUjWV9EUqXkET/9dSvaNN97YtWuX0+ksLS3VP8566qmnuPWZZ57hLd9//z1vYZo3b04bu3TpcvXqVXpC8IpS3yeXazuifk02NTX18uXLxcXFO3fulK7J0iJi2bJlhYWFeXl5mzdvTkxM/PLLL7mDyIgRI3hXK1eupM7qNdk5c+bwlpSUlFtNbdiKBkuWRDlSkWxJievyzrp19tGjt06cuFPs6WZUZma1ZBcsyKAyrVhTU3fqR5w8edeoUS4dHzjguia7Z88d12T377+i96x3SNlQjeMmeFrZcZJkg0JdHzMMfWnowhML5383X+zpZpQo2Zf+07WSDQgMmPftPP2Ic3fN1a/YSrtqxNAgWXPQTJQslyXS0tK4dd68eVITb3/22Wf1LUFBQbSkFVu5XNsR3d9dYLfb1bsLtMpnHncQke4uaNvWdZVNU/7tEuI612Q0AySbleW6nJqdXUhv8x2O62JPN6NEyY4du42vyV67VrJ166UtWxynTjnpeThu3DZqfeut/dLdBVSdMWOfepKehpQN1ThugqeyRl3e3/l+Kod3Cqe3+ZHdI8WebkaJkk3LSLu/i2snrcJb9U/o/1jiY9G9opsFNFtwbAG1Uh8KHtutTzeuqmdYv9AgWXPQTJTs1KlT+/bt26ZNm4CAAFqf9unTZ+HChdxEFBUV0Rrwvvvu448F9FFZWVlPPvlkcHBwdHT0qlWrahR3bUesEO6TpT2Ehob27NlzzZo1euvFixfpoFFRUTabrWXLlnR6tCClNa/eQYSWwDExMSEhIf369aPltnRorkpYTLIkwR9/LCDxkW3ff/+w2NPNKFGyIyvvLti0yfHzz8W0ni0uLjt3Ln/Nmh/1ew9mzz54/LjrPllaNVNBv3uhgSFlQzWOm+CprFGXM76a0TGmI61Dyba/W/o7saebUaJkl1beXfD4vz/eJqIN7SekeUjUg1G/Tvk1r2R5lIR6hvULDZI1B61hkgVei/gS0jyRrCVDyoZqHD8MSNYkIFmrYjHJrl9vV0PtVltYTLJDkoaooXZzH5CsSUCyVsU4yVa4vtJ2gwr0WFF1TUANPo2jR3P0LWPHbvv7389fuVJUWlpOj1QeM2brXe62+h8mIPVxE8ZJlvbWLrodFejRzZ61SmIejdG3pGWk/Wbyb8I6hNmCbPRI5fTM9LvcLe9NQupTZ2iQrDlokKxFEV9CmhdINiFhM5XFMySOHMmh7R7ttn4hZUM1Tr1Dc2tDsZsmSHbJmSWx/WN5o07sgFja7tFuGxIaJGsOGiRrUcSXkOYFkv3449u/rvvXv54ePXrr3/52mqsff3zUo93WL6RsqMapd9ylDVmjumSTP07mLfHT4tOPpz//5vNcTf4k2aPdNiQ0SNYcNEjWoogvIc0Uyb733vcOx/WbN8tPnLj2+ut7+DR0yR48eJWqhYVlfCMBPVK5wnVv3O3bYGvbbaOElA3VOPWO2mz4+//+fUS3iKCQoO7/2n3O13PYobpkew7uSdXQFqF8FwE9Upm29BzS0/1uGzHEnECyBqJBshZF0orqncaN6dNv/4AWc+1aCRd0yebmun4G4dy5fH0IlWnLzz8Xq3tr9JCyoRqnceOt/3sr0Hb7WzNa5T2wXNAly7+B0OlXnfQhVKYtbSLaqHszKMScQLIGokGyFkV8CWnGS3bv3ts3F9Pb/7Fjt+lfwNUlW1rq+rZYRkauPoS/biv+MIJxIWVDNU7jRq8ne7FV6e1/Wkaa/u1bXbK2INfvZvzykV/qQ/jmWfFXEYwOSNYkIFmrYrJk8/JcS1eH4zpXJ026/aVbN5LlLyncvGlBybZs6/rBjYhuEVx9f8/7dyvZYEjWRfXTWC/JXXwHSNaqmCxZ/gEt3aGjRm3h0/DPywUBgQGiQxefWixJVr1cEPVglIbLBVVUP431ktzFd4BkrYrJkq1zJat+8FVUdMcHX4aGyZLllWxk90iuqitZ9YOvkOYhmvDBlwkByZoEJGtVTJasfk32o4+OjhmzVb0mK93CRY9c1W/hMjRMlmzvYb3ZqikLUtIz09VrstItXPTIVf0WLhNCg2TNQYNkLYr4EtKMl6x0d0FBgeuXYSs8+TKCoSFlQzVO44Z0d0GL1i24cPdfRjAhNEjWHDRI1qKILyHNeMlSzJ17+z7Zkyedb765l09D+lrt6tWur9WSjumRyrRF3Y8RIWVDNU6jxyufvhLRLcIWbIvuFT1z7UzWqPS12mGpw8I6hJGO6ZHKtEXdj3EByZoEJGtVzJesN4f5kvX+gGRNApK1KpCsGJCsGpCsSUCyVgWSFQOSVQOSNQlI1qpAsmJAsmpAsiYByVoVSFYMSFYNSNYkIFmrAsmKAcmqAcmaBCRrVSBZMSBZNSBZk4BkrQokKwYkqwYkaxKQrFWBZMWAZNWAZE0iLCxMA1akRYsW+kuodWvXbz75M8iGipgTSNZYnE6n3W4/duzY7t27N2zYsAJYBZpNmlOaWXsV/jzLyIaKmBPygKyGpsNqki0oKMjKyqI/ZYcPH6aMbwRWgWaT5pRmNqsKf55lZENFzAl5QFZD02E1yRYWFtI7BYfDQbmmv2n7gVWg2aQ5pZnNqcKfZxnZUBFzQh6Q1dB0WE2yJSUl9EeMskx/zehdw2lgFWg2aU5pZguq8OdZRjZUxJyQB2Q1NB1Wk2xZWRnll/6OUaKdTqf+dx74OjSbNKc0syVV+PMsIxsqYk7IA7Iamg6rSRYAALwKSBb4BkcqkbcC4PVAssA3mF6JvBUArweSBT5AeXl5VCVUkNsA8G4gWeADbNu2jb/YQwW5DQDvBpIFPkBy8u3/dJoKchsA3g0kC7ydoqKiNm3asGSpQFW5BwBeDCQLvJ1Vq1axYRmqyj0A8GIgWeDtxMfHi5KlqtwDAC8GkgVeTW5ubkhIiChZqtJGuR8A3gokC7yaZcuWiYZlaKPcDwBvBZIFXk1cXJysWE2jjXI/ALwVSBZ4L3a7PSAgQFasptFGapJ7A+CVQLLAe5k3b57s1yqoSe4NgFcCyQLvpWfPnrJcq6AmuTcAXgkkC3wGcqu8CQCvB5IFPgMkC3wRSBb4DJAs8EUgWeAzQLLAF4Fkgc8AyQJfBJIFPgMkC3wRSBb4DJAs8EUgWeAzQLLAF4Fkgc8AyQJfBJIFPgMkC3wRSBb4DJAs8EUgWeAzQLLAF4Fkgc8AyQJfBJIFPgMkC3wR/5Ws6/fyNG3IkCFcfffdd+Pi4sLDw202W9u2bZ977rmjR4/eOaIxyc7OfuaZZ+69914+DbkZ1AQSBXwRSPa2ZLkq0qpVq1OnTt05qNF45ZVXxGPJzaAmkCjgi0CytyXboUOHtLS0rKwsp9M5ceJEbn311VfvHFQHBQUF8qZaeOKJJ/gQV65ckdtALUCywBfxI8muW7cuJiYmNDR0wIABGRkZkmTz8/P1nteuXePW4cOH6xtrRN/J0qVLO3fu3KxZM73JbrePHz++ffv2QUFBUVFR5GvStzhKRB8F3IBEAV/EXyR74MABkp0utYiICC7okhVxOBzcmpycLLfdCXdr3bo1F7QqC5Bh27Vrp29kevXqVVhYqI8SuWOnoBaQKOCL+Itk4+PjWWfLly+/fv365MmTuVqjZCdNmsStO3fulNvuhLsRM2fOpN2eOHGCtyclJdHGDh06/POf/ywqKlq1ahV3W7hwIXeg4/KW6n2BukC6gC/iL5INDw+nl2hMTAxXL168yI5TJfvOO+9w0/Tp06UmFe5J6+KysjJxe2RkJDdJJCQkcAdIth4gXcAX8RfJBgYG0kt00KBBXC0tLWXHSZJ9++23efuECRPKy8vFphrhzgMHDpS222w2bpIYOnQod2gUyfbo0ePO3VucsLAwOQUAeD3+IlleycbGxnK1xpXsnDlzeONdGvaWcouCDl/z1ZWqo++2USQLAPB+/EWyI0aMYKmtWLHixo0b6jXZ2bNn85aUlJS7NOyt2iXL12SbNWu2dOlSOpzT6dy0aVNiYuIXX3zBHSBZAPwEf5GsdHdB27ZtuaD7UW8SUe0pUVu3H3/8Ub27QKv82I07QLIA+An+Illi7dq1MTExISEh/fr1O3ToEDvOIMkSFy5coEVxVFSUzWZr2bJl3759abGcnZ3NrZAsAH6CH0kWAADMB5IFAAADgWTrpvrywZ3I/QAAQAGSrRtZrlXI/QAAQAGSBQAAA4FkAQDAQCBZAAAwEEgWAAAMBJIFAAADgWQBAMBAIFkAADAQSBYAAAwEkgUAAAOBZAEAwEAgWQAAMBBIFgAADASSBQAAA4FkAQDAQCBZAAAwEEgWAAAMBJIFAAADgWQBAMBAIFkAADAQSBYAAAwEkgUAAAOBZAEAwEAgWQAAMJAaJAsAAKDRgWQBAMBAIFkAADCQ/wdk2Ct8iRi/ugAAAABJRU5ErkJggg==" /></p>

4. 上記の状態でd2_ref = d1をした場合の状態は下記のようになる。

<p><img src="data:image/png;base64,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" /></p>

d2.name1\_の値が元のままであるが(これがスライシングである)、その理由は下記の疑似コードが示す通り、
「d2_refの表層型がクラスBaseであるためd1もクラスBase(正確にはBase型へのリファレンス)へ変換された後、
d2_refが指しているオブジェクト(d2)へコピーされた」からである。

```cpp
    d2_ref.Base::operator=(d1);   // Base::operator=(Base const&)が呼び出される
```

次に示すのは、
「オブジェクトの配列をその基底クラスへのポインタに代入し、
そのポインタを配列のように使用した場合に発生する」スライシングと類似の現象である。

```cpp
    // @@@ example/term_explanation/slice_ut.cpp 61

    TEST(Slicing, array)
    {
        Derived d_array[]{{"0", "1"}, {"2", "3"}};
        Base*   b_ptr = d_array;  // この代入までは問題ないが、b_ptr[1]でのアクセスで問題が起こる

        ASSERT_STREQ("0", d_array[0].Name0());  // OK
        ASSERT_STREQ("0", b_ptr[0].Name0());    // OK

        ASSERT_STREQ("2", d_array[1].Name0());  // OK
    #if 0  // スライシングに類似した問題で、以下のテストは失敗する。
        ASSERT_STREQ("2", b_ptr[1].Name0());  // NG
    #else  // こうすればテストは通るが、、、
        ASSERT_STREQ("1", b_ptr[1].Name0());  // NG
    #endif
    }
```

<p><img src="data:image/png;base64,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" /></p>

## name lookupと名前空間 <a id="SS_5_4"></a>
ここではname lookupとそれに影響を与える名前空間について解説する。

### ルックアップ <a id="SS_5_4_1"></a>
このドキュメントでのルックアップとは[name lookup](term_explanation.md#SS_5_4_2)を指す。

### name lookup <a id="SS_5_4_2"></a>
[name lookup](https://en.cppreference.com/w/cpp/language/lookup)
とはソースコードで名前が検出された時に、その名前をその宣言と関連付けることである。
以下、name lookupの例を上げる。

下記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp 5

    namespace NS_LU {
    int f() noexcept { return 0; }
    }  // namespace NS_LU
```

以下のコードでの関数呼び出しf()のname lookupは、


```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp 29

    NS_LU::f();
```

1. NS_LUをその前方で宣言された名前空間と関連付けする
2. f()呼び出しをその前方の名前空間NS_LUで宣言された関数fと関連付ける

という手順で行われる。

下記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp 11

    namespace NS_LU {
    bool g(int i) noexcept { return i < 0; }

    char g(std::string_view str) noexcept { return str[0]; }

    template <typename T, size_t N>
    size_t g(T const (&)[N]) noexcept
    {
        return N;
    }
```

以下のコードでの関数呼び出しg()のname lookupは、


```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp 37
    int a[3]{1, 2, 3};
    NS_LU::g(a);
```

1. NS_LUをその前方で宣言された名前空間と関連付けする
2. 名前空間NS_LU内で宣言された複数のgを見つける
3. g()呼び出しを、
   すでに見つけたgの中からベストマッチしたg(T const (&)[N])と関連付ける

という手順で行われる。

下記記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp 44

    // グローバル名前空間
    std::string ToString(int i) { return std::to_string(i) + " in Global"; }

    namespace NS_LU {
    struct X {
        int i;
    };

    std::string ToString(X const& x) { return std::to_string(x.i) + " in NS_LU"; }
    }  // namespace NS_LU

    namespace NS2 {
    std::string ToString(NS_LU::X const& x) { return std::to_string(x.i) + " in NS2"; }
    }  // namespace NS2
```

以下のコードでの関数呼び出しToString()のname lookupは、

```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp 65

    auto x = NS_LU::X{1};

    ASSERT_EQ("1 in NS_LU", ToString(x));
```

1. ToString()呼び出しの引数xの型Xが名前空間NS_LUで定義されているため、
   ToStringを探索する名前空間にNS_LUを組み入れる(「[関連名前空間](term_explanation.md#SS_5_4_6)」参照)
2. ToString()呼び出しより前方で宣言されたグローバル名前空間とNS_LUの中から、
   複数のToStringの定義を見つける
3. ToString()呼び出しを、
   すでに見つけたToStringの中からベストマッチしたNS_LU::ToStringと関連付ける

という手順で行われる。


### two phase name lookup <a id="SS_5_4_3"></a>
[two phase name lookup](https://en.cppreference.com/w/cpp/language/two-phase_lookup)
とはテンプレートをインスタンス化するときに使用される、下記のような2段階でのname lookupである。

1. テンプレート定義内でname lookupを行う(通常のname lookupと同じ)。
   この時、テンプレートパラメータに依存した名前
   ([dependent_name](https://en.cppreference.com/w/cpp/language/dependent_name))は
   name lookupの対象外となる(name lookupの対象が確定しないため)。
2. 1の後、テンプレートパラメータを展開した関数内で、
   [関連名前空間](term_explanation.md#SS_5_4_6)の宣言も含めたname lookupを行う。

以下の議論では、

* 上記1のname lookupを1st name lookup
* 上記2のname lookupを2nd name lookup

と呼ぶことにする。

下記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp 5

    namespace NS_TPLU {
    struct X {
        int i;
    };
    }  // namespace NS_TPLU

    // グローバル名前空間
    inline std::string ToType(NS_TPLU::X const&) { return "X in global"; }
    inline std::string ToType(int const&) { return "int in global"; }

    // 再びNS_TPLU
    namespace NS_TPLU {

    std::string Header(long) { return "type:"; }  //  下記にもオーバーロードあり

    template <typename T>
    std::string ToType(T const&)  //  下記にもオーバーロードあり
    {
        return "unknown";
    }

    template <typename T>
    std::string TypeName(T const& t)  // オーバーロードなし
    {
        return Header(int{}) + ToType(t);
    }

    std::string Header(int) { return "TYPE:"; }  // 上記にもオーバーロードあり

    std::string ToType(X const&) { return "X"; }      // 上記にもオーバーロードあり
    std::string ToType(int const&) { return "int"; }  // 上記にもオーバーロードあり
    }  // namespace NS_TPLU
```

以下のコードでのTypeNameのインスタンス化に伴うname lookupは、

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp 44

    auto x = NS_TPLU::X{1};

    ASSERT_EQ("type:X", TypeName(x));
```

1. TypeName()呼び出しの引数xの型Xが名前空間NS_TPLUで宣言されているため、
   NS_TPLUをTypeNameを探索する[関連名前空間](term_explanation.md#SS_5_4_6)にする。
2. TypeName()呼び出しより前方で宣言されたグローバル名前空間とNS_TPLUの中からTypeNameを見つける。
3. TypeNameは関数テンプレートであるためtwo phase lookupが以下のように行われる。
    1. TypeName内でのHeader(int{})の呼び出しは、1st name lookupにより、
       Header(long)の宣言と関連付けられる。
       Header(int)はHeader(long)よりもマッチ率が高い、
       TypeNameの定義より後方で宣言されているため、name lookupの対象外となる。
    2. TypeName内でのToType(t)の呼び出しに対しては、2nd name lookupが行われる。
       このためTypeName定義より前方で宣言されたグローバル名前空間と、
       tの型がNS_TPLU::Xであるため[関連名前空間](term_explanation.md#SS_5_4_6)となったNS_TPLUがname lookupの対象となるが、
       グローバル名前空間内のToTypeは、
       NS_TPLU内でTypeNameより前に宣言されたtemplate<> ToTypeによって[name-hiding](term_explanation.md#SS_5_4_8)が起こり、
       TypeNameからは非可視となるためname lookupの対象から外れる。
       このため、ToType(t)の呼び出しは、NS_TPLU::ToType(X const&)の宣言と関連付けられる。

という手順で行われる。

上と同じ定義、宣言がある場合の以下のコードでのTypeNameのインスタンス化に伴うname lookupは、

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp 50

    ASSERT_EQ("type:unknown", NS_TPLU::TypeName(int{}));
```

1. NS_TPLUを名前空間と関連付けする
   (引数の型がintなのでNS_TPLUは[関連名前空間](term_explanation.md#SS_5_4_6)とならず、NS_TPLUを明示する必要がある)。
2. TypeName()呼び出しより前方で宣言されたNS_TPLUの中からTypeNameを見つける。
3. TypeNameは関数テンプレートであるためtwo phase lookupが以下のように行われる。
    1. TypeName内でのHeader(int{})の呼び出しは、1st name lookupにより、
       前例と同じ理由で、Header(long)の宣言と関連付けられる。
    2. TypeName内でのToType(t)の呼び出しに対しては、2nd name lookupが行われる。
       tの型がintであるためNS_TPLUは[関連名前空間](term_explanation.md#SS_5_4_6)とならず、通常のname lookupと同様に
       ToType(t)の呼び出し前方のグローバル名前空間とNS_TPLUがname lookupの対象になるが、
       グローバル名前空間内のToTypeは、
       NS_TPLU内でTypeNameより前に宣言されたtemplate<> ToTypeによって[name-hiding](term_explanation.md#SS_5_4_8)が起こり、
       TypeNameからは非可視となるためname lookupの対象から外れる。
       また、ToType(int const&)は、TypeNameの定義より後方で宣言されているため、
       name lookupの対象外となり、
       その結果、ToType(t)の呼び出しは、NS_TPLU内のtemplate<> ToTypeの宣言と関連付けられる。

という手順で行われる。

以上の理由から、先に示した例でのToTypeの戻り値は"X"となり、
後に示した例でのToTypeの戻り値は"unknown"となる。
これはtwo phase lookupの結果であり、
two phase lookupが実装されていないコンパイラ(こういったコンパイラは存在する)では、
結果が異なるため注意が必要である
(本ドキュメントではこのような問題をできる限り避けるために、
サンプルコードを[g++](term_explanation.md#SS_5_9_1)と[clang++](term_explanation.md#SS_5_9_2)でコンパイルしている)。

以下に、two phase lookupにまつわるさらに驚くべきコード例を紹介する。
上と同じ定義、宣言がある場合の以下のコードの動作を考える。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp 54

    ASSERT_EQ("type:long", NS_TPLU::TypeName(long{}));
```

NS_TPLU::TypeName(int{})のintをlongにしただけなので、この単体テストはパスしないが、
この単体テストコードの後(実際にはこのファイルのコンパイル単位の中のNS_TPLU内で、
且つtemplate<> ToTypeの宣言の後方であればどこでもよい)
に以下のコードを追加するとパスしてしまう。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp 61

    namespace NS_TPLU {
    template <>
    std::string ToType<long>(long const&)
    {
        return "long";
    }
    }  // namespace NS_TPLU
```

この理由は、関数テンプレート内での2nd name lookupで選択された名前が関数テンプレートであった場合、
その特殊化の検索範囲はコンパイル単位内になることがあるからである
([template_specialization](https://en.cppreference.com/w/cpp/language/template_specialization)
によるとこの動作は未定義のようだが、
[g++](term_explanation.md#SS_5_9_1)/[clang++](term_explanation.md#SS_5_9_2)両方ともこのコードを警告なしでコンパイルする)。

TypeName(long{})内でのtwo phase name lookupは、TypeName(int{})とほぼ同様に進み、
template<> ToTypeの宣言を探し出すが、
さらに前述したようにこのコンパイル単位のNS_TPLU内からその特殊化も探し出す。
その結果、ToType(t)の呼び出しは、NS_TPLU内のtemplate<> ToType\<long>の定義と関連付けられる。

以上の議論からわかる通り、関数テンプレートとその特殊化の組み合わせは、
そのインスタンス化箇所(この場合単体テストコード内)の後方から、
name lookupでバインドされる関数を変更することができるため、
極めて分かりづらいコードを生み出す。ここから、

* 関数テンプレートとその特殊化はソースコード上なるべく近い位置で宣言するべきである
* STL関数テンプレートの特殊化は行うべきではない

という教訓が得られる。

なお、関数とその関数オーバーロードのname lookupの対象は、呼び出し箇所前方の宣言のみであるため、
関数テンプレートToType(T const& t)の代わりに、関数ToType(...)を使うことで、
上記問題は回避可能である。

次に示す例は、一見2nd name lookupで関連付けされるように見える関数ToType(NS_TPLU2::Y const&)が、
実際には関連付けされないコードである。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp 71

    namespace NS_TPLU2 {
    struct Y {
        int i;
    };
    }  // namespace NS_TPLU2
```
```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp 79

    // global名前空間
    template <typename T>
    std::string ToType(T const&)
    {
        return "unknown";
    }

    template <typename T>
    std::string TypeName(T const& t)
    {
        return "type:" + ToType(t);
    }

    std::string ToType(NS_TPLU2::Y const&) { return "Y"; }
```

これは先に示したNS_TPLU::Xの例と極めて似ている。本質的な違いは、
TypeNameやToTypeがグローバル名前空間で宣言されていることのみである。
だが、下記の単体テストで示す通り、
TypeName内でのname lookupで関数オーバーライドToType(NS_TPLU2::Y const&)が選択されないのである。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp 100

    auto y = NS_TPLU2::Y{1};

    // ASSERT_EQ("type:Y", TypeName(y));
    ASSERT_EQ("type:unknown", TypeName(y));  // ToType(NS_TPLU2::Y const&)は使われない
```

ここまでの現象を正確に理解するには、
「two phase lookupの対象となる宣言」を下記のように、より厳密に認識する必要がある。

* TypeNameの中で行われる1st name lookupの対象となる宣言は下記の積集合である。
    * TypeNameと同じ名前空間内かグローバル名前空間内の宣言
    * TypeName定義位置より前方の宣言

* TypeNameの中で行われる2nd name lookupの対象となる宣言は下記の和集合である。
    * 1st name lookupで使われた宣言
    * TypeName呼び出しより前方にある[関連名前空間](term_explanation.md#SS_5_4_6)内の宣言

この認識に基づくNS_TPLU2::Yに対するグローバルなTypeName内でのtwo phase name lookupは、

1. TypeName内に1st name lookupの対象がないため何もしない。
2. TypeName内の2nd name lookupに使用される[関連名前空間](term_explanation.md#SS_5_4_6)NS_TPLU2は、
   ToType(NS_TPLU2::Y const&)の宣言を含まないため、この宣言は2nd name lookupの対象とならない。
   その結果、ToType(t)の呼び出しは関数テンプレートToType(T const&)と関連付けられる。

という手順で行われる。

以上が、TypeNameからToType(NS_TPLU2::Y const&)が使われない理由である。

ここまでで示したようにtwo phase name lookupは理解しがたく、
理解したとしてもその使いこなしはさらに難しい。

次のコードは、この難解さに翻弄されるのが現場のプログラマのみではないことを示す。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp 71

    namespace NS_TPLU2 {
    struct Y {
        int i;
    };
    }  // namespace NS_TPLU2
```
```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp 110

    // global名前空間
    template <typename T>
    int operator+(T const&, int i)
    {
        return i;
    }

    template <typename T>
    int TypeNum(T const& t)
    {
        return t + 0;
    }

    int operator+(NS_TPLU2::Y const& y, int i) { return y.i + i; }
```

上記の宣言、定義があった場合、operator+の単体テストは以下のようになる。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp 132

    auto y = NS_TPLU2::Y{1};

    ASSERT_EQ(1, y + 0);  // 2つ目のoperator+が選択される
```

このテストは当然パスするが、次はどうだろう？

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp 142

    auto y = NS_TPLU2::Y{1};

    ASSERT_EQ(1, TypeNum(y));  // g++ではoperator+(NS_TPLU2::Y const&, int i)がname lookupされる
```

これまでのtwo phase name lookupの説明では、
operator+(NS_TPLU2::Y const& y, int i)はTypeNum内でのname lookupの対象にはならないため、
このテストはエラーとならなければならないが、[g++](term_explanation.md#SS_5_9_1)ではパスしてしまう。
2nd name lookupのロジックにバグがあるようである。

有難いことに、[clang++](term_explanation.md#SS_5_9_2)では仕様通りこのテストはエラーとなり、
当然ながら以下のテストはパスする(つまり、g++ではエラーする)。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp 151

    auto y = NS_TPLU2::Y{1};

    ASSERT_EQ(0, TypeNum(y));  // clang++ではoperator+(T const&, int i)がname lookupされる
```

なお、TypeNum内のコードである

```cpp
    return t + 0;
```

を下記のように変更することで

```cpp
    return operator+(t, 0);
```

g++のname lookupはclang++と同じように動作するため、
記法に違和感があるものの、この方法はg++のバグのワークアランドとして使用できる。

また、operator+(NS_TPLU2::Y const& y, int i)をNS_TPLU2で宣言することで、
g++ではパスしたテストをclang++でもパスさせられるようになる(これは正しい動作)。
これにより、型とその2項演算子オーバーロードは同じ名前空間で宣言するべきである、
という教訓が得られる。

以上で見てきたようにtwo phase name lookupは、現場プログラマのみではなく、
コンパイラを開発するプログラマをも混乱させるほど難解ではあるが、
STLを含むテンプレートメタプログラミングを支える重要な機能であるため、
C++プログラマには、最低でもこれを理解し、出来れば使いこなせるようになってほしい。


### 実引数依存探索 <a id="SS_5_4_4"></a>
実引数依存探索とは、argument-dependent lookupの和訳語であり、
通常はその略語である[ADL](term_explanation.md#SS_5_4_5)と呼ばれる。

### ADL <a id="SS_5_4_5"></a>
ADLとは、関数の実引数の型が宣言されている名前空間(これを[関連名前空間](term_explanation.md#SS_5_4_6)と呼ぶ)内の宣言が、
その関数の[name lookup](term_explanation.md#SS_5_4_2)の対象になることである。

下記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/name_lookup_adl_ut.cpp 5
    namespace NS_ADL {
    struct A {
        int i;
    };

    std::string ToString(A const& a) { return std::string{"A:"} + std::to_string(a.i); }
    }  // namespace NS_ADL
```

以下のコードでのToStringの呼び出しに対するのname lookupは、

```cpp
    // @@@ example/term_explanation/name_lookup_adl_ut.cpp 18

    auto a = NS_ADL::A{0};

    ASSERT_EQ("A:0", ToString(a));  // ADLの効果により、ToStringはNS_ADLを指定しなくても見つかる
```

* ToStringの呼び出しより前方で行われているグローバル名前空間内の宣言
* ToStringの呼び出しより前方で行われているNS_ADL内の宣言

の両方を対象として行われる。
NS_ADL内の宣言がToStringの呼び出しに対するのname lookupの対象になる理由は、
ToStringの呼び出しに使われている実引数aの型AがNS_ADLで宣言されているからである。
すでに述べたようにこれをADLと呼び、この場合のNS_ADLを[関連名前空間](term_explanation.md#SS_5_4_6)と呼ぶ。

ADLは思わぬname lookupによるバグを誘発することもあるが、
下記コードを見れば明らかなように、また、
多くのプログラマはそれと気づかずに使っていることからもわかる通り、
コードをより自然に、より簡潔に記述するための重要な機能となっている。

```cpp
    // @@@ example/term_explanation/name_lookup_adl_ut.cpp 28

    // 下記operator <<は、std::operator<<(ostream&, string const&)であり、
    // namespace stdで定義されている。

    // ADLがあるため、operator <<は名前空間修飾無しで呼び出せる。
    std::cout << std::string{__func__};

    // ADLが無いと下記のような呼び出しになる。
    std::operator<<(std::cout, std::string{__func__});
```

### 関連名前空間 <a id="SS_5_4_6"></a>
関連名前空間(associated namespace)とは、
[ADL](term_explanation.md#SS_5_4_5)(実引数依存探索)によってname lookupの対象になった宣言を含む名前空間のことである。


### SFINAE <a id="SS_5_4_7"></a>
[SFINAE](https://cpprefjp.github.io/lang/cpp11/sfinae_expressions.html)
(Substitution Failure Is Not An Errorの略称、スフィネェと読む)とは、
「テンプレートのパラメータ置き換えに失敗した([ill-formed](term_explanation.md#SS_5_10_8)になった)際に、
即時にコンパイルエラーとはせず、置き換えに失敗したテンプレートを
[name lookup](term_explanation.md#SS_5_4_2)の候補から除外する」
という言語機能である。

### name-hiding <a id="SS_5_4_8"></a>
name-hidingとは
「前方の識別子が、その後方に同一の名前をもつ識別子があるために、
[name lookup](term_explanation.md#SS_5_4_2)の対象外になる」現象一般をを指す通称である
([namespace](https://en.cppreference.com/w/cpp/language/namespace)参照)。

まずは、クラスとその派生クラスでのname-hidingの例を示す。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp 4

    struct Base {
        void f() noexcept {}
    };

    struct Derived : Base {
        // void f(int) { f(); }     // f()では、Baseのf()をname lookupできないため、
        void f(int) noexcept { Base::f(); }  // Base::でf()を修飾した
    };
```

上記の関数fは一見オーバーロードに見えるが、そうではない。下記のコードで示したように、
Base::f()には、修飾しない形式でのDerivedクラス経由のアクセスはできない。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp 18

    {
        auto d = Derived{};
    #if 0 
        d.f(); // コンパイルできない
    #else
        d.Base::f();  // Base::での修飾が必要
    #endif
    }
```

これは前述したように、
Base::fがその後方にあるDerived::f(int)によりname-hidingされたために起こる現象である
(name lookupによる探索には識別子が使われるため、シグネチャの違いはname-hidingに影響しない)。

下記のように[using宣言](term_explanation.md#SS_5_4_13)を使用することで、
修飾しない形式でのDerivedクラス経由のBase::f()へのアクセスが可能となる。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp 34

    struct Derived : Base {
        using Base::f;  // using宣言によりDerivedにBase::fを導入
        void f(int) noexcept { Base::f(); }
    };
```
```cpp
    // @@@ example/term_explanation/name_hiding.cpp 45

    auto d = Derived{};
    d.f();  // using宣言によりコンパイルできる
```

下記コードは、名前空間でも似たような現象が起こることを示している。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp 54

    // global名前空間
    void f() noexcept {}

    namespace NS_A {
    void f(int) noexcept {}

    void g() noexcept
    {
    #if 0
        f();  // NS_A::fによりname-hidingされたため、コンパイルできない
    #endif
    }
    }  // namespace NS_A
```

この問題に対しては、下記のようにf(int)の定義位置を後方に移動することで回避できる。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp 70

    namespace NS_A_fixed_0 {
    void g() noexcept
    {
        // グローバルなfの呼び出し
        f();  // NS_A::fは後方に移動されたためコンパイルできる
    }

    void f(int) noexcept {}
    }  // namespace NS_A_fixed_0
```

また、先述のクラスでの方法と同様にusing宣言を使い、下記のようにすることもできる。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp 82

    namespace NS_A_fixed_1 {
    void f(int) noexcept {}

    void g() noexcept
    {
        using ::f;

        // グローバルなfの呼び出し
        f();  // using宣言によりコンパイルできる
    }
    }  // namespace NS_A_fixed_1
```

当然ながら、下記のようにf()の呼び出しを::で修飾することもできる。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp 96

    namespace NS_A_fixed_2 {
    void f(int) noexcept {}

    void g() noexcept
    {
        // グローバルなfの呼び出し
        ::f();  // ::で修飾すればコンパイルできる
    }
    }  // namespace NS_A_fixed_2
```

修飾の副作用として「[two phase name lookup](term_explanation.md#SS_5_4_3)」の例で示したような
[ADL](term_explanation.md#SS_5_4_5)を利用した高度な静的ディスパッチが使用できなくなるが、
通常のソフトウェア開発では、ADLが必要な場面は限られているため、
デフォルトでは名前空間を使用して修飾を行うことにするのが、
無用の混乱をさけるための安全な記法であると言えるだろう。

次に、そういった混乱を引き起こすであろうコードを示す。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp 108

    namespace NS_B {
    struct S_in_B {};

    void f(S_in_B) noexcept {}
    void f(int) noexcept {}

    namespace NS_B_Inner {
    void g() noexcept
    {
        f(int{});  // コンパイルでき、NS_B::f(int)が呼ばれる
    }

    void f() noexcept {}

    void h() noexcept
    {
        // f(int{});     // コンパイルできない
        NS_B::f(int{});  // 名前空間で修飾することでコンパイルできる

        f(S_in_B{});  // ADLによりコンパイルできる
    }
    }  // namespace NS_B_Inner
    }  // namespace NS_B
```

NS_B_Inner::g()内のf(int)の呼び出しはコンパイルできるが、
name-hidingが原因で、NS_B_Inner::h()内のf(int)の呼び出しはコンパイルできず、
名前空間で修飾することが必要になる。
一方で、ADLの効果で名前空間での修飾をしていないf(S_in_B)の呼び出しはコンパイルできる。

全チームメンバがこういったname lookupを正しく扱えると確信できないのであれば、
前述の通り、デフォルトでは名前空間を使用して修飾を行うのが良いだろう。

### ドミナンス <a id="SS_5_4_9"></a>
[ドミナンス(Dominance、支配性)](https://en.wikipedia.org/wiki/Dominance_(C%2B%2B))とは、
「探索対称の名前が継承の中にも存在するような場合の[name lookup](term_explanation.md#SS_5_4_2)の仕様の一部」
を指す慣用句である。

以下に

* [ダイヤモンド継承を含まない場合](term_explanation.md#SS_5_4_9_1)
* [ダイヤモンド継承かつそれが仮想継承でない場合](term_explanation.md#SS_5_4_9_2)
* [ダイヤモンド継承かつそれが仮想継承である場合](term_explanation.md#SS_5_4_9_3)

のドミナンスについてのコードを例示する。

この例で示したように、[ダイヤモンド継承](term_explanation.md#SS_5_4_10)を通常の継承で行うか、
[仮想継承](term_explanation.md#SS_5_4_11)で行うかでは結果が全く異なるため、注意が必要である。

#### ダイヤモンド継承を含まない場合 <a id="SS_5_4_9_1"></a>

```cpp
    // @@@ example/term_explanation/dominance_ut.cpp 9

    int32_t f(double) noexcept { return 0; }

    struct Base {
        int32_t f(int32_t) const noexcept { return 1; }
        int32_t f(double) const noexcept { return 2; }
    };

    struct Derived : Base {
        int32_t f(int32_t) const noexcept { return 3; }  // Base::fを隠蔽する(name-hiding)
    };

    struct DerivedDerived : Derived {
        int32_t g() const noexcept { return f(2.14); }
    };
```
```cpp
    // @@@ example/term_explanation/dominance_ut.cpp 29

    Base b;

    ASSERT_EQ(2, b.f(2.14));  // オーバーロード解決により、B::f(double)が呼ばれる

    DerivedDerived dd;

    // Derivedのドミナンスにより、B::fは、DerivedDerived::gでのfのname lookupの対象にならず、
    // DerivedDerived::gはDerived::fを呼び出す。
    ASSERT_EQ(3, dd.g());
```

この[name lookup](term_explanation.md#SS_5_4_2)については、[name-hiding](term_explanation.md#SS_5_4_8)で説明した通りである。

#### ダイヤモンド継承かつそれが仮想継承でない場合 <a id="SS_5_4_9_2"></a>

```cpp
    // @@@ example/term_explanation/dominance_ut.cpp 45

    struct Base {
        int32_t f(int32_t) const noexcept { return 1; }
        int32_t f(double) const noexcept { return 2; }
    };

    struct Derived_0 : Base {
        int32_t f(int32_t) const noexcept { return 3; }  // Base::fを隠蔽する(name-hiding)
    };

    struct Derived_1 : Base {};

    struct DerivedDerived : Derived_0, Derived_1 {
        int32_t g() const noexcept { return f(2.14); }  // Derived_0::f or Derived_1::f ?
    };

    // dominance_ut.cpp:58:41: error: reference to ‘f’ is ambiguous
    //    58 |     int32_t g() const noexcept { return f(2.14); }  // Derived_0::f or Derived_1::f ?
    //       |                                         ^
```

上記コードはコードブロック内のコメントのようなメッセージが原因でコンパイルできない。

Derived_0のドミナンスにより、DerivedDerived::gはDerived_0::fを呼び出すように見えるが、
もう一つの継承元であるDerived_1が導入したDerived_1::f(実際には、Derived_1::Base::f)があるため、
Derived_1によるドミナンスも働き、その結果として、呼び出しが曖昧(ambiguous)になることで、
このような結果となる。

#### ダイヤモンド継承かつそれが仮想継承である場合 <a id="SS_5_4_9_3"></a>

```cpp
    // @@@ example/term_explanation/dominance_ut.cpp 71

    struct Base {
        int32_t f(int32_t) const noexcept { return 1; }
        int32_t f(double) const noexcept { return 2; }
    };

    struct Derived_0 : virtual Base {
        int32_t f(int32_t) const noexcept { return 3; }  // Base::fを隠蔽する(name-hiding)
    };

    struct Derived_1 : virtual Base {};

    struct DerivedDerived : Derived_0, Derived_1 {
        int32_t g() const noexcept { return f(2.14); }
    };
```
```cpp
    // @@@ example/term_explanation/dominance_ut.cpp 92

    DerivedDerived dd;

    // Derived_0のドミナンスと仮想継承の効果により、
    // B::fは、DerivedDerived::gでのfのname lookupの対象にならず、
    // DerivedDerived::gはDerived_0::fを呼び出す。
    ASSERT_EQ(3, dd.g());
```

これまでと同様にDerived_0のドミナンスによりBase::fは[name-hiding](term_explanation.md#SS_5_4_8)されることになる。
この時、Derived_0、Derived_1がBaseから[仮想継承](term_explanation.md#SS_5_4_11)した効果により、
この継承ヒエラルキーの中でBaseは１つのみ存在することになるため、
Derived_1により導入されたBase::fも併せて[name-hiding](term_explanation.md#SS_5_4_8)される。
結果として、曖昧性は排除され、コンパイルエラーにはならず、このような結果となる。

### ダイヤモンド継承 <a id="SS_5_4_10"></a>
ダイヤモンド継承(Diamond Inheritance)とは、以下のような構造のクラス継承を指す。

* 基底クラス(Base)が一つ存在し、その基底クラスから二つのクラス(Derived_0、Derived_1)が派生する。
* Derived_0とDerived_1からさらに一つのクラス(DerivedDerived)が派生する。
  したがって、DerivedDerivedはBaseの孫クラスとなる。

この継承は、多重継承の一形態であり、クラス図で表すと下記のようになるため、
ダイヤモンド継承と呼ばれる。

<p><img src="data:image/png;base64,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" /></p>

ダイヤモンド継承は、
[仮想継承](term_explanation.md#SS_5_4_11)(virtual inheritance)を使ったものと、使わないものに分類できる。

[仮想継承](term_explanation.md#SS_5_4_11)を使わないダイヤモンド継承のコードを以下に示す。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp 6

    class Base {
    public:
        int32_t get() const noexcept { return x_; }
        void    set(int32_t x) noexcept { x_ = x; }

    private:
        int32_t x_ = 0;
    };

    class Derived_0 : public Base {};

    class Derived_1 : public Base {};

    class DerivedDerived : public Derived_0, public Derived_1 {};
```
```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp 26

    auto dd = DerivedDerived{};

    Base& b0 = static_cast<Derived_0&>(dd);  // Derived_0::Baseのリファレンス
    Base& b1 = static_cast<Derived_1&>(dd);  // Derived_1::Baseのリファレンス

    ASSERT_NE(&b0, &b1);  // ddの中には、Baseインスタンスが2つできる
```

これからわかるように、DerivedDerivedインスタンスの中に2つのBaseインスタンスが存在する。

下記コードは、それが原因で名前解決が曖昧になりコンパイルできない。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp 36

    Base& b = dd;  // Derived_0::Base or Derived_1::Base ?

    dd.get();  // Derived_0::get or  Derived_1::get ?

    // 下記のようなエラーが発生する
    //  diamond_inheritance_ut.cpp:37:15: error: ‘Base’ is an ambiguous base of ‘DerivedDerived’
    //     37 |     Base& b = dd;  // Derived_0::Base or Derived_1::Base ?
    //        |               ^~
    //  diamond_inheritance_ut.cpp:39:8: error: request for member ‘get’ is ambiguous
    //     39 |     dd.get();  // Derived_0::get or  Derived_1::get ?
    //        |        ^~~
```

この問題に対処するには、クラス名による修飾が必要になるが、
Baseインスタンスが2つ存在するため、下記に示すようなわかりづらいバグの温床となる。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp 53

    ASSERT_EQ(0, dd.Derived_0::get());  // クラス名による名前修飾
    ASSERT_EQ(0, dd.Derived_1::get());

    dd.Derived_0::set(1);
    ASSERT_EQ(1, dd.Derived_0::get());  // Derived_0::Base::x_は1に変更
    ASSERT_EQ(0, dd.Derived_1::get());  // Derived_1::Base::x_は0のまま

    dd.Derived_1::set(2);
    ASSERT_EQ(1, dd.Derived_0::get());  // Derived_0::Base::x_は1のまま
    ASSERT_EQ(2, dd.Derived_1::get());  // Derived_1::Base::x_は2に変更
```

次に示すのは、[仮想継承](term_explanation.md#SS_5_4_11)を使用したダイヤモンド継承の例である。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp 70

    class Base {
    public:
        int32_t get() const noexcept { return x_; }
        void    set(int32_t x) noexcept { x_ = x; }

    private:
        int32_t x_ = 0;
    };

    class Derived_0 : public virtual Base {};  // 仮想継承

    class Derived_1 : public virtual Base {};  // 仮想継承

    class DerivedDerived : public Derived_0, public Derived_1 {};
```
```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp 90

    auto dd = DerivedDerived{};

    Base& b0 = static_cast<Derived_0&>(dd);  // Derived_0::Baseのリファレンス
    Base& b1 = static_cast<Derived_1&>(dd);  // Derived_1::Baseのリファレンス

    ASSERT_EQ(&b0, &b1);  // ddの中には、Baseインスタンスが1つできる
```

仮想継承の効果で、DerivedDerivedインスタンスの中に存在するBaseインスタンスは1つになるため、
上で示した仮想継承を使わないダイヤモンド継承での問題は解消される
(が、[仮想継承](term_explanation.md#SS_5_4_11)による別の問題が発生する)。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp 99

    Base& b = dd;  // Baseインスタンスは1つであるため、コンパイルできる

    dd.get();  // Baseインスタンスは1つであるため、コンパイルできる

    dd.Derived_0::set(1);               // クラス名による修飾
    ASSERT_EQ(1, dd.Derived_1::get());  // Derived_1::BaseとDerived_1::Baseは同一であるため

    dd.set(2);
    ASSERT_EQ(2, dd.get());
```

### 仮想継承 <a id="SS_5_4_11"></a>
下記に示した継承方法を仮想継承、仮想継承の基底クラスを仮想基底クラスと呼ぶ。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp 9

    class Base {
    public:
        explicit Base(int32_t x = 0) noexcept : x_{x} {}
        int32_t get() const noexcept { return x_; }

    private:
        int32_t x_;
    };

    class DerivedVirtual : public virtual Base {  // 仮想継承
    public:
        explicit DerivedVirtual(int32_t x) noexcept : Base{x} {}
    };
```

仮想継承は、[ダイヤモンド継承](term_explanation.md#SS_5_4_10)の基底クラスのインスタンスを、
その継承ヒエラルキーの中で1つのみにするための言語機能である。

仮想継承の独特の動作を示すため、
上記コードに加え、仮想継承クラス、通常の継承クラス、
それぞれを通常の継承したクラスを下記のように定義する。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp 25

    class DerivedDerivedVirtual : public DerivedVirtual {  // 仮想継承を通常の継承
    public:
        explicit DerivedDerivedVirtual(int32_t x) noexcept : DerivedVirtual{x} {}
    };

    class DerivedNormal : public Base {  // 通常の継承
    public:
        explicit DerivedNormal(int32_t x) noexcept : Base{x} {}
    };

    class DerivedDerivedNormal : public DerivedNormal {  // 通常の継承を通常の継承
    public:
        explicit DerivedDerivedNormal(int32_t x) noexcept : DerivedNormal{x} {}
    };
```

この場合、継承ヒエラルキーに仮想継承を含むクラスと、含まないクラスでは、
以下に示したような違いが発生する。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp 45

    auto dv = DerivedVirtual{1};  // 仮想継承クラス
    auto dn = DerivedNormal{1};   // 通常の継承クラス

    ASSERT_EQ(1, dv.get());  // これは非仮想継承と同じ動作
    ASSERT_EQ(1, dn.get());

    auto ddv = DerivedDerivedVirtual{1};  // 仮想継承クラスを継承したクラス
    auto ddn = DerivedDerivedNormal{1};   // 通常の継承クラスを継承したクラス

    ASSERT_EQ(0, ddv.get());  // Baseのデフォルトコンストラクタが呼ばれる
    ASSERT_EQ(1, ddn.get());
```

これは、「仮想継承クラスを継承したクラスが、
仮想継承クラスの基底クラスのコンストラクタを明示的に呼び出さない場合、
引数なしで呼び出せる基底クラスのコンストラクタが呼ばれる」仕様に起因している
(引数なしで呼び出せる基底クラスのコンストラクタがない場合はコンパイルエラー)。
以下では、これを「仮想継承のコンストラクタ呼び出し」仕様と呼ぶことにする。

仮想継承クラスが、基底クラスのコンストラクタを呼び出したとしても、この仕様が優先されるため、
上記コードのような動作となる。

これを通常の継承クラスと同様な動作にするには、下記のようにしなければならない。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp 61

    class DerivedDerivedVirtualFixed : public DerivedVirtual {  // DerivedDerivedNormalと同じように動作
    public:
        explicit DerivedDerivedVirtualFixed(int32_t x) noexcept : Base{x}, DerivedVirtual{x} {}
        //                     基底クラスのコンストラクタ呼び出し ^^^^^^^
    };
```
```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp 72

    DerivedDerivedVirtual ddv{1};  // 仮想継承クラスを継承したクラス
    DerivedDerivedVirtualFixed ddvf{1};  // 上記クラスのコンストラクタを修正したクラス
    DerivedDerivedNormal ddn{1};         // 通常の継承クラスを継承したクラス

    ASSERT_EQ(0, ddv.get());  // 仮想継承独特の動作
    ASSERT_EQ(1, ddvf.get());
    ASSERT_EQ(1, ddn.get());
```
「仮想継承のコンストラクタ呼び出し」仕様は、
[ダイヤモンド継承](term_explanation.md#SS_5_4_10)での基底クラスのコンストラクタ呼び出しを一度にするために存在する。

もし、この機能がなければ、下記のコードでの基底クラスのコンストラクタ呼び出しは2度になるため、
デバッグ困難なバグが発生してしまうことは容易に想像できるだろう。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp 87

    int32_t base_called;

    class Base {
    public:
        explicit Base(int32_t x = 0) noexcept : x_{x} { ++base_called; }
        int32_t get() const noexcept { return x_; }

    private:
        int32_t x_;
    };

    class Derived_0 : public virtual Base {  // 仮想継承
    public:
        explicit Derived_0(int32_t x) noexcept : Base{x} { assert(base_called == 1); }
    };

    class Derived_1 : public virtual Base {  // 仮想継承
    public:
        explicit Derived_1(int32_t x) noexcept : Base{x} { assert(base_called == 1); }
    };

    class DerivedDerived : public Derived_0, public Derived_1 {
    public:
        DerivedDerived(int32_t x0, int32_t x1) noexcept : Derived_0{x0}, Derived_1{x1} {}
        // 「仮想継承のコンストラクタ呼び出し」仕様がなければ、このコンストラクタは、
        //    Base::Base -> Derived_0::Derived_0 ->
        //      Base::Base -> Derived_0::Derived_0 ->
        //        DerivedDerived::DerivedDerived
        //  という呼び出しになるため、Base::Baseが2度呼び出されてしまう。
    };
```
```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp 123

    ASSERT_EQ(0, base_called);

    auto dd = DerivedDerived{2, 3};  // Base::Baseが最初に呼ばれないとassertion failする

    ASSERT_EQ(1, base_called);  // 「仮想継承のコンストラクタ呼び出し」仕様のため
    ASSERT_EQ(0, dd.get());  // Baseのデフォルトコンストラクタは、x_を0にする
```

基底クラスのコンストラクタ呼び出しは、下記のコードのようにした場合でも、
単体テストが示すように、一番最初に行われる。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp 138

    class DerivedDerived : public Derived_0, public Derived_1 {
    public:
        DerivedDerived(int32_t x0, int32_t x1) noexcept : Derived_0{x0}, Derived_1{x1}, Base{1} {}
    };
```
```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp 150

    ASSERT_EQ(0, base_called);

    auto dd = DerivedDerived{2, 3};  // Base::Baseが最初に呼ばれないとassertion failする

    ASSERT_EQ(1, base_called);  // 「仮想継承のコンストラクタ呼び出し」仕様のため
    ASSERT_EQ(1, dd.get());  // Base{1}呼び出しの効果
```

このため、基底クラスのコンストラクタ呼び出しは下記のような順番で行うべきである。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp 163

    class DerivedDerived : public Derived_0, public Derived_1 {
    public:
        DerivedDerived(int32_t x0, int32_t x1) noexcept : Base{1}, Derived_0{x0}, Derived_1{x1} {}
    };
```

### 仮想基底 <a id="SS_5_4_12"></a>
仮想基底(クラス)とは、[仮想継承](term_explanation.md#SS_5_4_11)の基底クラス指す。

### using宣言 <a id="SS_5_4_13"></a>
using宣言とは、"using XXX::func"のような記述である。
この記述が行われたスコープでは、この記述後の行から名前空間XXXでの修飾をすることなく、
funcが使用できる。

```cpp
    // @@@ example/term_explanation/namespace_ut.cpp 6
    namespace XXX {
    void func() noexcept {}
    void gunc() noexcept {}
    }  // namespace XXX
```
```cpp
    // @@@ example/term_explanation/namespace_ut.cpp 12

    // global namespace
    void using_declaration() noexcept
    {
        using XXX::func;  // using宣言

        func();       // XXX::不要
        XXX::gunc();  // XXX::必要
    }

```

### usingディレクティブ <a id="SS_5_4_14"></a>
usingディレクティブとは、"using namespace XXX"のような記述である。
この記述が行われたスコープでは、下記例のように、この記述後から名前空間XXXでの修飾をすることなく、
XXXの識別子が使用できる。

```cpp
    // @@@ example/term_explanation/namespace_ut.cpp 6
    namespace XXX {
    void func() noexcept {}
    void gunc() noexcept {}
    }  // namespace XXX
```
```cpp
    // @@@ example/term_explanation/namespace_ut.cpp 24

    // global namespace
    void using_directive() noexcept
    {
        using namespace XXX;  // usingディレクティブ

        func();  // XXX::不要
        gunc();  // XXX::不要
    }
```

より多くの識別子が名前空間の修飾無しで使えるようになる点において、
[using宣言](term_explanation.md#SS_5_4_13)よりも危険であり、また、
下記のように[name-hiding](term_explanation.md#SS_5_4_8)された識別子の導入には効果がない。

```cpp
    // @@@ example/term_explanation/namespace_ut.cpp 6
    namespace XXX {
    void func() noexcept {}
    void gunc() noexcept {}
    }  // namespace XXX
```
```cpp
    // @@@ example/term_explanation/namespace_ut.cpp 35

    namespace XXX_Inner {
    void func(int) noexcept {}
    void using_declaration() noexcept
    {
    #if 0
        using namespace XXX;  // name-hidingのため効果がない
    #else
        using XXX::func;  // using宣言
    #endif

        func();  // XXX::不要
    }
```

従って、usingディレクティブの使用は避けるべきである。


## expressionと値カテゴリ <a id="SS_5_5"></a>
ここでは、expression(式)の値カテゴリや、それに付随した機能についての解説を行う。

### expression <a id="SS_5_5_1"></a>

C++においてexpression、lvalue、rvalue、xvalue、glvalue、prvalueは以下のように定められている。

* [expression](https://ja.cppreference.com/w/cpp/language/expressions)(式)
  とは「演算子とそのオペランドの並び」である(オペランドのみの記述も式である)。
  演算子とは以下のようなものである。
    * 四則演算、代入(a = b、a += b ...)、インクリメント、比較、論理式
    * 明示的キャストや型変換
    * メンバアクセス(a.b、a->b、a[x]、 \*a、&a ...)
    * 関数呼び出し演算子(f(...))、sizeof、decltype等

* expressionは、以下のいずれかに分類される。lvalueでないexpressionがrvalueである。
    * lvalue
    * rvalue

* lvalueとは、関数もしくはオブジェクトを指す。

* rvalueは、以下のいずれかに分類される。
    * xvalue
    * prvalue

* xvalueとは以下のようなものである。
    * 戻り値の型がT&&(Tは任意の型)である関数の呼び出し式(std::move(x))
    * オブジェクトへのT&&へのキャスト式(static_cast<char&&>(x))
    * aを配列のrvalueとした場合のa[N]や、
      cをクラス型のrvalueとした場合のc.m(mはaの非staticメンバ)等

* prvalueとは、オブジェクトやビットフィールドを初期化する、
  もしくはオペランドの値を計算する式であり、以下のようなものである。
    * 文字列リテラルを除くリテラル
    * 戻り値の型が非リファレンスである関数呼び出し式、
      または前置++と前置--を除くオーバーロードされた演算子式(path.string()、str1 + str2、it++ ...)
    * 組み込み型インスタンスaのa++、a\-\-(++a、\-\-aはlvalue)
    * 組み込み型インスタンスa、bに対する
      a + b、 a % b、 a & b、 a && b、 a || b、 !a、 a < b、 a == b等

* glvalueは、以下のいずれかに分類される。
    * lvalue
    * xvalue

<p><img src="data:image/png;base64,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" /></p>

ざっくりと言えば、lvalueとは代入式の左辺になり得る式、rvalueとは代入式の左辺にはなり得ない式である。
T const&は左辺になり得ないが、lvalueである。rvalueリファレンス(T&&)もlvalueであるため、
rvalueであることとrvalueリファレンスであることとは全く異なる。

xvalueとは、多くの場合、「std::move()の呼び出し式のことである」と考えても差し支えない。

prvalueとは、いわゆるテンポラリオブジェクトのことであるため
(下記のstd::string()で作られるようなオブジェクト)、名前はない。
また、アドレス演算子(&)のオペランドになれない。

```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp 8

    {
        // sを初期化するためにstd::string{}により生成されるオブジェクトはprvalue
        // sはlvalue

        auto s = std::string{};

    #if 0
        // 下記はコンパイルエラー

        auto* sp = &std::string{};
        // 下記はg++のエラーメッセージ
        // programming_convention_type.cpp|709 col 29| error: taking address of rvalue [-fpermissive]
        // ||   709 |     auto* sp = &std::string{};
    #else
        // 下記のようにすればアドレスを取得できるが、このようなことはすべきではない。
        auto&& rvalue_ref = std::string{};
        auto   sp         = &rvalue_ref;
    #endif
        static_assert(std::is_same_v<std::string*, decltype(sp)>);
    }
```

C++11でrvalueの概念の整理やstd::move()の導入が行われた目的はプログラム実行速度の向上である。

* lvalueからの代入
* rvalueからの代入
* std::move(lvalue)からの代入

の処理がどのように違うのかを見ることでrvalueの効果について説明する。

1. 下記コードにより「lvalueからの代入」を説明する。

    ```.cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp 35

    auto str0 = std::string{};        // str0はlvalue
    auto str1 = std::string{"hehe"};  // str1もlvalue
    str0      = str1;                 // lvalueからの代入
    ```

    * ステップ1。
      str0、str1がそれぞれ初期化される
      ("hehe"を保持するバッファが生成され、それをstr1オブジェクトが所有する)。

    * ステップ2。
      st1が所有している文字列バッファと等価のバッファが作られ
      (文字列バッファ用のメモリをnewし、文字列を代入)、
      str0がそれを所有する。従って、"hehe"を保持するバッファが2つできる。
      この代入をcopy代入と呼ぶ。

<p><img src="data:image/png;base64,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iJyoEDkgcoACiJyoEDkgcoACiJyoEDkgcoACiJyoEDkgcoACiJyoEDkgcoACiJyoEDkgcoACiJyoEDkgcoACiJyoEDkgcoACiJyoEDkgcoACiJyoEDkgcoACiJyoEDkgcoACiJyouPczuauqqoicohA5QAEBkROdTCbT0tLS3Nzc2NjY0NCwzn6yClmLrEjWJaszF4wjETlAAUROhDo6Otra2uSGYMuWLXKl3mg/WYWsRVYk65LVmQvGkYgcoAAiJ0JdXV3t7e2tra1yjZY7gyb7ySpkLbIiWZeszlwwjkTkAAUQORHq7u6WWwG5Oss9QUtLyw77ySpkLbIiWZeszlwwjkTkAAUQORHq6emR67LcDcgFOpPJtNtPViFrkRXJumR15oJxJCIHKIDIAaJC5AAFEDlAVIgcoAAiB4gKkQMUQOQAUSFygAKIHCAqRA5QAJEDRIXIAQogcoCoEDlAAUQOEJWyRM7dd9/d91NWvyZP2cveZPeWIighco7+XbGXvfHvTVZZIgdwSVBC5ADIRuQABRA5QFSIHKAAIgeICpEDFEDkAFEhcoACiBwgKkQOUACRA0SFyAEKIHKAqNgdOTt37uS/4UO5ETlAVCyOnBUrVsi1oKamZv/+/eY+IDpEDhCViCOns7Pz4RxNTU1qr2SDue/hh998880jv0Y/urq6ns4xceLEp556asKECZs2bVq8ePG4ceMGDRo0Z86c119/3TweKAGRA0Ql4sjZvXv32UeSGKivr1d7v/jii3NzPPnkk0d+jd729nY16Ojo2Ldvnwx27dp1ao6RI0deeeWVQ4cObW5uPu2001atWnXHHXfI1UHGR3w5oDREDhCViCPH8OKLLw4bNmznzp29fXcqb+fR2toqE1599dVrrrlGQquysvLzzz+XLbW1tatXrza+pnbRRRfJjc79998vY/03OtOmTZPDj5gHlIbIAaJSxsiRW5MTTzzx3nvvVU+bmpq++glz/bn22mtlQltbm9yy7N2794wzznj22Wdly8yZM9evX5/9NbOdcMIJdXV12Vvef/99ualauHBh9kagRAGRA0SkXJGzZ8+euXPnLlq0qKenx9zXd8cjbfzGG28Y22tqap5//vlbb711+fLlcrszcODAjz76yJijyC2RfIVNmzbpLR9//PGpp546fvx4CZ6siUCpiBwgKmWJHLnon3baaXKL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" /></p>


2. 下記コードにより「rvalueからの代入」を説明する。

    ```.cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp 46

    auto str0 = std::string{};        // str0はlvalue
    str0      = std::string{"hehe"};  // rvalueからの代入
    ```

    * ステップ1。str0、「std::string()により作られたテンポラリオブジェクト」がそれぞれ初期化される
      ("hehe"を保持するバッファが生成され、それをテンポラリオブジェクトが所有する)。

    * ステップ2。
      "hehe"を保持する文字列バッファをもう1つ作る代わりに、
      テンポラリオブジェクトが所有している文字列バッファをstr0の所有にする。
      この代入をmove代入と呼ぶ。

    * ステップ3。
      テンポラリオブジェクトが解体されるが、
      文字列バッファはstr0の所有であるためdeleteする必要がなく、実際には何もしない。
      move代入によって、文字列バッファの生成と破棄の回数がそれぞれ1回少なくなったため、
      実行速度は向上する(通常、new/deleteの処理コストは高い)。

<p><img src="data:image/png;base64,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" /></p>

3. 下記コードにより「std::move(lvalue)からの代入」を説明する。

    ```.cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp 56

    auto str0 = std::string{};        // str0はlvalue
    auto str1 = std::string{"hehe"};  // str1もlvalue
    str0      = std::move(str1);      // str1はこれ以降使われないとする
    ```

    * ステップ1。「lvalueからの代入」のステップ1と同じである。

    * ステップ2。
      std::move()の効果により(実際にはrvalueリファレンスへのキャストが行われるだけなので、
      実行時速度に影響はない)、"hehe"を保持する文字列バッファをもう1つ作る代わりに、
      str1が所有している文字列バッファをstr0の所有にする。
      この代入もmove代入と呼ぶ。
      この動作は「rvalueからの代入」と同じであり、同様に速度が向上するが、その副作用として、
      str1.size() == 0となる。

<p><img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAiYAAAGWCAIAAADDjt/5AAAtOElEQVR4Xu3dD3BV5Z3/8ZM/EEj4kxhAYKEhKBaWin+QteJPCKBufwJ2prsztvbPMmqwap0y6nR1JiP97XRLtEynLIKF6czK6kKnU8epncnQ8Q/7G+N2f/EPccOKQDt6h3RikeDVkGBqEn/f5oFzbp6byyW9555znud5v+YZ5rnnPOfCuef7PZ8ciNH7DACASHj6BgAAioPIAQBEJIicIQAAioDIAQBEhMgBAESEyAEARITIAQBEhMgBAESEyAEARITIAQBEhMgBAESEyAEARITIAQBEhMgBAESEyAEARITIAQBEhMgBAESEyAEARITIAQBEhMgBAESEyAEARITIAQBEhMgBAESEyAEARITIAQBEhMgBAESEyAEARITIAQBExOzIOX78eH9/v74VAJBIBkfOli1bPM9bsmTJJ598ou8DACRPyJHT29u7M0tbW5vaK9mg79u588033xz5HqPo6+vbm2XmzJnPPPPMjBkzXnzxxVtuuUVeSgItXrxYPxgAkAwhR87JkyevHam8vLy5uVnt/fjjj6/P8vTTT498j6Hu7m416enpOXPmjExOnDjx+SyTJ0++7bbbJkyY0NHRsW7dukcffZTIAYAkCzlyNC+99NLEiROPHz8+NPyk8t85dHZ2yoLf/va3d9xxh4RWRUXFRx99JFsaGhq2b9+uvafv5ptvliebH/7wh/4WIgcAkqyIkSOPJvPmzfvBD36gXra1tXk53HnnnbKgq6tLHllOnz59xRVX/OpXv5ItCxcufPbZZzPfM9OcOXOampoyt3hEDgAkWLEi59SpU8uWLbvpppsGBgb0fcNPPBIPb7zxhrZ9yZIlv/nNbx544IEHH3xQHnfKysree+89bY0ij0TyDi+++GLmRiIHAJKsKJFz9OhRufXLI8706dMPHjyo7x4aev755ydOnNjb26ttlyB5//33P/jgg56enkcfffS6667TFiiffPKJ7Lr88su1PCNyACDJQo6cwcHBJ598cvLkyY2NjRIMmzdvrqmp0Z5mXnjhhdmzZ3/ve9/L3Jjpo48+kgOnTJny2muv6fuGhg4dOrRixQr5LTLDbPcwiRx5Z5lkLAcAJEXIkbN69eq5c+c+99xz/pbvf//7F110kTz3qJcSRZWVlU1NTaP+hdsf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iyZyenp69E0AnETkGM/PiT179syfP7+iouKWW245efKktle91EJC25vr5TPPPHPppZfKO69atero0aOZezXagbt27aqrqyspKVHbATiOyDGeur9PmTIluPF73saNGzP3FhI5NTU1khlqLurr69VTi78lU+aB1dXV2nYAjiNyjOff1n/+858fP35c3ehnz56dubeQyBHylNPb23vPPfeolzt37hz13Xz+gU1NTadPnz58+LC2AICbiBzjqZv7ggUL1Mvrr7/eG/7/KmbuLSRyFi5cqF5Knqktt99+u9py/siZOXPmwMCAtguAy4gc46n7++rVq9XL84eKLDvP3lFfrlq1Sr389NNP1ZYvfelLasv5I2flypXadjiuqrpK1YY1amtr9ZPEeRE5xlOln+s5Rv2/5VesWKFeLlq0KHOvduyoL8/zlHPjjTeqLeqlT3sfQJGq2PX7XTYNOaN0Ot3T09PX19ff389jfV5EjvG0+7sWObNmzZL59OnTpTEOHDigdvl7tWNHfSn27t3b29t77733qpdPPvmkWnDrrbeqLW+++abaknkgkQONZ2PkpFKprq6u7u5uCR5JHf2cMRKRYzzt/q5FTmNjo3o5adKksrKyiRMnZu7Vjh31pcSVelRS/O9YE1u2bPG3K6O+D6B4NkZOR0fHsWPHOjs7JXXkWUc/Z4xE5BhPu79rkXPy5Mm1a9dWVFTU1dXt2rVL26sdm+vlL37xi8suu0zepKGh4ciRI2qvkAaTSJs2bZr/XdSjvg+geDZGTmtra3t7u6SOPOvwXz3nReQgJ5ID4bIyclpaWiR15FknlUql02n9nDESkYOciByEy8rI2bdv3/79+9va2uRBp7u7Wz9njETkICciB+HKFTmy/eL5F8tEfvXXqPJbuHxh9vq8o8Bjs/8wuYZH5IwRkQMgIrnu4KPe5QuMjUKOzf7D5BpEzlgROQAikusOPupdvsDYKOTY7D9MrkHkjBWRAyAiue7go97l/di4e8fdM+bNKB9f/oWGL/z4jR/7RzW3Ni//++XVF1eXlZfVzKpZ/Q+rt721LZRjs/8wuQaRM1ZEDtylbkxbtmyZO3futGnT9uzZs2vXrunTp6u5WjM4OPijH/3okksuGTdu3KxZszZt2tTb2yvbv/Wtb8mxskst27Bhg7x8/PHH1cudO3cuWrRIDpG3amxs/PDDD9V2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" /></p>


エッセンシャルタイプがTであるlvalue、xvalue、prvalueに対して
(例えば、std::string const&のエッセンシャルタイプはstd::stringである)、
decltypeの算出結果は下表のようになる。

|decltype           |算出された型|
|:------------------|:-----------|
|decltype(lvalue)   |T           |
|decltype((lvalue)) |T&          |
|decltype(xvalue)   |T&&         |
|decltype((xvalue)) |T&&         |
|decltype(prvalue)  |T           |
|decltype((prvalue))|T           |

この表の結果を使用した下記の関数型マクロ群により式を分類できる。
定義から明らかな通り、これらは
[テンプレートメタプログラミング](https://ja.wikipedia.org/wiki/%E3%83%86%E3%83%B3%E3%83%97%E3%83%AC%E3%83%BC%E3%83%88%E3%83%A1%E3%82%BF%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0)
に有効に活用できる。

```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp 65

    #define IS_LVALUE(EXPR_) std::is_lvalue_reference_v<decltype((EXPR_))>
    #define IS_XVALUE(EXPR_) std::is_rvalue_reference_v<decltype((EXPR_))>
    #define IS_PRVALUE(EXPR_) !std::is_reference_v<decltype((EXPR_))>
    #define IS_RVALUE(EXPR_) (IS_PRVALUE(EXPR_) || IS_XVALUE(EXPR_))

    TEST(Expression, rvalue)
    {
        auto str = std::string{};

        static_assert(IS_LVALUE(str), "EXPR_ must be lvalue");
        static_assert(!IS_RVALUE(str), "EXPR_ must NOT be rvalue");

        static_assert(IS_XVALUE(std::move(str)), "EXPR_ must be xvalue");
        static_assert(!IS_PRVALUE(std::move(str)), "EXPR_ must NOT be prvalue");

        static_assert(IS_PRVALUE(std::string{}), "EXPR_ must be prvalue");
        static_assert(IS_RVALUE(std::string{}), "EXPR_ must be rvalue");
        static_assert(!IS_LVALUE(std::string{}), "EXPR_ must NOT be lvalue");
    }
```

### lvalue <a id="SS_5_5_2"></a>
「[expression](term_explanation.md#SS_5_5_1)」を参照せよ。

### rvalue <a id="SS_5_5_3"></a>
「[expression](term_explanation.md#SS_5_5_1)」を参照せよ。

#### xvalue <a id="SS_5_5_3_1"></a>
「[expression](term_explanation.md#SS_5_5_1)」を参照せよ。

#### prvalue <a id="SS_5_5_3_2"></a>
「[expression](term_explanation.md#SS_5_5_1)」を参照せよ。

### rvalue修飾 <a id="SS_5_5_4"></a>
下記GetString0()のような関数が返すオブジェクトの内部メンバに対するハンドルは、
オブジェクトのライフタイム終了後にもアクセスすることができるため、
そのハンドルを通じて、
ライフタイム終了後のオブジェクトのメンバオブジェクトにもアクセスできてしまう。

ライフタイム終了後のオブジェクトにアクセスすることは未定義動作であり、
特にそのオブジェクトがrvalueであった場合、さらにその危険性は高まる。

こういったコードに対処するためのシンタックスが、lvalue修飾、rvalue修飾である。

下記GetString1()、GetString3()、GetString4()のようにメンバ関数をlvalue修飾やrvalue修飾することで、
rvalueの内部ハンドルを返さないようにすることが可能となり、上記の危険性を緩和することができる。

```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp 91

    class C {
    public:
        explicit C(char const* str) : str_{str} {}

        // lvalue修飾なし、rvalue修飾なし
        std::string& GetString0() noexcept { return str_; }

        // lvalue修飾
        std::string const& GetString1() const& noexcept { return str_; }

        // rvalue修飾
        // *thisがrvalueの場合でのGetString1()の呼び出しは、この関数を呼び出すため、
        // class内部のハンドルを返してはならない。
        // また、それによりstd::stringを生成するため、noexcept指定してはならない。
        std::string GetString1() const&& { return str_; }

        // lvalue修飾だが、const関数はrvalueからでも呼び出せる。
        // rvalueに対しての呼び出しを禁止したい場合には、GetString4のようにする。
        std::string const& GetString2() const& noexcept { return str_; }

        // lvalue修飾
        // 非constなのでrvalueからは呼び出せない。
        std::string const& GetString3() & noexcept { return str_; }

        // lvalue修飾
        std::string const& GetString4() const& noexcept { return str_; }

        // rvalue修飾
        // rvalueからこの関数を呼び出されるとrvalueオブジェクトの内部ハンドルを返してしまい、
        // 危険なので=deleteすべき。
        std::string const& GetString4() const&& = delete;

    private:
        std::string str_;
    };
```
```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp 132

    auto        c    = C{"c0"};
    auto const& s0_0 = c.GetString0();        // OK cが解放されるまでs0_0は有効
    auto        s0_1 = C{"c1"}.GetString0();  // NG 危険なコード
    // s0_1が指すオブジェクトは、次の行で無効になる

    auto const& s1_0 = c.GetString1();        // OK GetString1()&が呼び出される
    auto const& s1_1 = C{"c1"}.GetString1();  // OK GetString1()&&が呼び出される
    // s1_0が指すrvalueはs1_0がスコープアウトするまで有効

    auto const& s2_0 = c.GetString2();        // OK GetString2()&が呼び出される
    auto const& s2_1 = C{"c1"}.GetString2();  // NG const関数はlvalue修飾しても呼び出し可能
    // s2_1が指すオブジェクトは、次の行で無効になる

    auto const& s3_0 = c.GetString3();  // OK GetString3()&が呼び出される
    // auto const& s3_1 = C{"c1"}.GetString3();  // 危険なのでコンパイルさせない

    auto const& s4_0 = c.GetString4();  // OK GetString4()&が呼び出される
    // auto const& s4_1 = C{"c1"}.GetString4();  // 危険なのでコンパイルさせない
```

### lvalue修飾 <a id="SS_5_5_5"></a>
[rvalue修飾](term_explanation.md#SS_5_5_4)を参照せよ。


### リファレンス修飾 <a id="SS_5_5_6"></a>
[rvalue修飾](term_explanation.md#SS_5_5_4)と[lvalue修飾](term_explanation.md#SS_5_5_5)とを併せて、リファレンス修飾と呼ぶ。

### decltype <a id="SS_5_5_7"></a>
decltypeはオペランドに[expression](term_explanation.md#SS_5_5_1)を取り、その型を算出する機能である。
decltype(auto)はそのオペランドの省略形である。
autoとdecltypeでは、以下に示す通りリファレンスの扱いが異なることに注意する必要がある。

```cpp
    // @@@ example/term_explanation/decltype_ut.cpp 7

    int32_t  x{3};
    int32_t& r{x};

    auto           a = r;  // aの型はint32_t
    decltype(r)    b = r;  // bの型はint32_t&
    decltype(auto) c = r;  // cの型はint32_t&   C++14からサポート
                           // decltype(auto)は、decltypeに右辺の式を与えるための構文

    // std::is_sameはオペランドの型が同じか否かを返すメタ関数
    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), int&>);
    static_assert(std::is_same_v<decltype(c), int&>);
```

decltypeは、テンプレートプログラミングに多用されるが、
クロージャ型(「[ラムダ式](term_explanation.md#SS_5_10_4)」参照)
のような記述不可能な型をオブジェクトから算出できるため、
下記例のような場合にも有用である。

```cpp
    // @@@ example/term_explanation/decltype_ut.cpp 26

    //  本来ならばA::dataは、
    //      * A::Aでメモリ割り当て
    //      * A::~Aでメモリ解放
    //  すべきだが、何らかの理由でそれが出来ないとする
    struct A {
        size_t   len;
        uint8_t* data;
    };

    void do_something(size_t len)
    {
        auto deallocate = [](A* p) {
            delete[](p->data);
            delete p;
        };

        auto a_ptr = std::unique_ptr<A, decltype(deallocate)>{new A, deallocate};

        a_ptr->len  = len;
        a_ptr->data = new uint8_t[10];

        ...
        // do something for a_ptr
        ...

        // a_ptrによるメモリの自動解放
    }
```

## リファレンス <a id="SS_5_6"></a>
ここでは、C++11から導入された

* [ユニバーサルリファレンス](term_explanation.md#SS_5_6_1)
* [リファレンスcollapsing](term_explanation.md#SS_5_6_4)

について解説する。

### ユニバーサルリファレンス <a id="SS_5_6_1"></a>
関数テンプレートの型パラメータや型推論autoに&&をつけて宣言された変数を、
ユニバーサルリファレンスと呼ぶ(C++17から「forwardingリファレンス」という正式名称が与えられた)。
ユニバーサルリファレンスは一見rvalueリファレンスのように見えるが、
下記に示す通り、lvalueにもrvalueにもバインドできる。

```cpp
    // @@@ example/term_explanation/universal_ref_ut.cpp 8

    template <typename T>
    void f(T&& t) noexcept  // tはユニバーサルリファレンス
    {
        ...
    }

    template <typename T>
    void g(std::vector<T>&& t) noexcept  // tはrvalueリファレンス
    {
        ...
    }
```
```cpp
    // @@@ example/term_explanation/universal_ref_ut.cpp 29

    auto       vec  = std::vector<std::string>{"lvalue"};   // vecはlvalue
    auto const cvec = std::vector<std::string>{"clvalue"};  // cvecはconstなlvalue

    f(vec);                                 // 引数はlvalue
    f(cvec);                                // 引数はconstなlvalue
    f(std::vector<std::string>{"rvalue"});  // 引数はrvalue

    // g(vec);  // 引数がlvalueなのでコンパイルエラー
    // g(cvec); // 引数がconst lvalueなのでコンパイルエラー
    g(std::vector<std::string>{"rvalue"});  // 引数はrvalue
```

下記のコードはジェネリックラムダ(「[ラムダ式](term_explanation.md#SS_5_10_4)」参照)
の引数をユニバーサルリファレンスにした例である。

```cpp
    // @@@ example/term_explanation/universal_ref_ut.cpp 47

    // sはユニバーサルリファレンス
    auto value_type = [](auto&& s) noexcept {
        if (std::is_same_v<std::string&, decltype(s)>) {
            return 0;
        }
        if (std::is_same_v<std::string const&, decltype(s)>) {
            return 1;
        }
        if (std::is_same_v<std::string&&, decltype(s)>) {
            return 2;
        }
        return 3;
    };

    auto       str  = std::string{"lvalue"};
    auto const cstr = std::string{"const lvalue"};

    ASSERT_EQ(0, value_type(str));
    ASSERT_EQ(1, value_type(cstr));
    ASSERT_EQ(2, value_type(std::string{"rvalue"}));
```

通常、ユニバーサルリファレンスはstd::forwardと組み合わせて使用される。


### forwardingリファレンス <a id="SS_5_6_2"></a>
「[ユニバーサルリファレンス](term_explanation.md#SS_5_6_1)」を参照せよ。

### perfect forwarding <a id="SS_5_6_3"></a>
perfect forwarding とは、引数の[rvalue](term_explanation.md#SS_5_5_3)性や
[lvalue](term_explanation.md#SS_5_5_2)性を損失することなく、
その引数を別の関数に転送する技術のことを指す。
通常は、[ユニバーサルリファレンス](term_explanation.md#SS_5_6_1)である関数の仮引数をstd::forwardを用いて、
他の関数に渡すことで実現される。

### リファレンスcollapsing <a id="SS_5_6_4"></a>
Tを任意の型とし、TRを下記のように宣言した場合、

```cpp
    using TR = T&;
```

下記のようなコードは、C++03ではコンパイルエラーとなったが、
C++11からはエラーとならず、TRRはT&となる。

```cpp
    using TRR = TR&;
```

2つの&を1つに折り畳む、このような機能をリファレンスcollapsingと呼ぶ。

下記はTをintとした場合のリファレンスcollapsingの動きを示している。

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp 7

    int i;

    using IR  = int&;
    using IRR = IR&;  // IRRはint& &となり、int&に変換される

    IR  ir  = i;
    IRR irr = ir;

    static_assert(std::is_same_v<int&, decltype(ir)>);   // lvalueリファレンス
    static_assert(std::is_same_v<int&, decltype(irr)>);  // lvalueリファレンス
```

リファレンスcollapsingは、型エイリアス、型であるテンプレートパラメータ、decltypeに対して行われる。
詳細な変換則は、下記のようになる。

```
    T& &   -> T&
    T& &&  -> T&
    T&& &  -> T&
    T&& && -> T&&
```

下記のようなクラステンプレートを定義した場合、

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp 26

    template <typename T>
    struct Ref {
        T&  t;
        T&& u;
    };
```

下記のコードにより、テンプレートパラメータに対するこの変換則を確かめることができる。

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp 38

    static_assert(std::is_same_v<int&, decltype(Ref<int>::t)>);    // lvalueリファレンス
    static_assert(std::is_same_v<int&&, decltype(Ref<int>::u)>);   // rvalueリファレンス

    static_assert(std::is_same_v<int&, decltype(Ref<int&>::t)>);   // lvalueリファレンス
    static_assert(std::is_same_v<int&, decltype(Ref<int&>::u)>);   // lvalueリファレンス

    static_assert(std::is_same_v<int&, decltype(Ref<int&&>::t)>);  // lvalueリファレンス
    static_assert(std::is_same_v<int&&, decltype(Ref<int&&>::u)>); // rvalueリファレンス
```

この機能がないC++03では、

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp 52

    template <typename T>
    struct AddRef {
        using type = T&;
    };
```

ようなクラステンプレートに下記コードのようにリファレンス型を渡すとコンパイルエラーとなる。

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp 69

    static_assert(std::is_same_v<int&, AddRef<int&>::type>);
```

この問題を回避するためには下記のようなテンプレートの特殊化が必要になる。

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp 59

    template <typename T>
    struct AddRef<T&> {
        using type = T&;
    };
```

上記したようなクラステンプレートでのメンバエイリアスの宣言は、
[テンプレートメタプログラミング](https://ja.wikipedia.org/wiki/%E3%83%86%E3%83%B3%E3%83%97%E3%83%AC%E3%83%BC%E3%83%88%E3%83%A1%E3%82%BF%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0)
で頻繁に使用されるため、
このようなテンプレートの特殊化を不要にするリファレンスcollapsingは、
有用な機能拡張であると言える。

### danglingリファレンス <a id="SS_5_6_5"></a>
Dangling リファレンスとは、破棄後のオブジェクトを指しているリファレンスを指す。
このようなリファレンスにアクセスすると、未定義動作動作に繋がるに繋がる。

```cpp
    // @@@ example/term_explanation/dangling_ut.cpp 9

    bool X_destructed;
    class X {
    public:
        X() { X_destructed = false; }
        ~X() { X_destructed = true; }
    };

    bool A_destructed;
    class A {
    public:
        A() { A_destructed = false; }
        ~A() { A_destructed = true; }

        X const& GetX() const noexcept { return x_; }

    private:
        X x_;
    };

    // @@@ example/term_explanation/dangling_ut.cpp 34

    auto a = A{};

    auto const& x_safe = a.GetX();  // x_safeはダングリングリファレンスではない
    ASSERT_FALSE(A_destructed || X_destructed);

    auto const& x_dangling = A{}.GetX();  // 次の行でxが指すオブジェクトは解放される
    // この行ではxはdangngling リファレンスになる。
    ASSERT_TRUE(A_destructed && X_destructed);

    auto const* x_ptr_dangling = &A{}.GetX();  // 次の行でxが指すオブジェクトは解放される
    // この行ではxはdangngling ポインタになる。
    ASSERT_TRUE(A_destructed && X_destructed);
```

### danglingポインタ <a id="SS_5_6_6"></a>
danglingポインタとは、[danglingリファレンス](term_explanation.md#SS_5_6_5)と同じような状態になったポインタを指す。

## エクセプション安全性の保証 <a id="SS_5_7"></a>
関数のエクセプション発生時の安全性の保証には以下の3つのレベルが規定されている。

### no-fail保証 <a id="SS_5_7_1"></a>
「no-fail保証」を満たす関数はエクセプションをthrowしない。

### 強い保証 <a id="SS_5_7_2"></a>
「強い保証」を満たす関数は、この関数がエクセプションによりスコープから外れた場合でも、
この関数が呼ばれなかった状態と同じ(プログラムカウンタ以外の状態は同じ)であることを保証する。
従って、この関数呼び出しは成功したか、完全な無効だったかのどちらかになる。

### 基本保証 <a id="SS_5_7_3"></a>
「基本保障」を満たす関数は、この関数がエクセプションによりスコープから外れた場合でも、
メモリ等のリソースリークは起こさず、
オブジェクトは(変更されたかもしれないが)引き続き使えることを保証する。


## シンタックス、セマンティクス <a id="SS_5_8"></a>
直訳すれば、シンタックスとは構文論のことであり、セマンティクスとは意味論のことである。
この二つの概念の違いをはっきりと際立たせる有名な文を例示する。

```
    Colorless green ideas sleep furiously(直訳:無色の緑の考えが猛烈に眠る)
```

この文は構文的には正しい(シンタックスは問題ない)が、
意味不明である(セマンティクスは誤り)。

C++プログラミングにおいては、コンパイルできることがシンタックス的な正しさであり、例えば

* クラスや関数がその名前から想起できる責務を持っている
    * 「[単一責任の原則(SRP)](solid.md#SS_1_1)」を満たしている
    * [Accessor](design_pattern.md#SS_2_4)を実装する関数の名前は、GetXxxやSetXxxになっている
    * コンテナクラスのメンバ関数beginやendは、
      そのクラスが保持する値集合の先頭や最後尾の次を指すイテレータを返す
    * 等

* 「[等価性のセマンティクス](term_explanation.md#SS_5_8_1)」を守ってる
* 「[copyセマンティクス](term_explanation.md#SS_5_8_2)」を守ってる
* 「[moveセマンティクス](term_explanation.md#SS_5_8_3)」を守っている

等がセマンティクス的な正しさである。

セマンティクス的に正しいソースコードは読みやすく、保守性、拡張性に優れている。

### 等価性のセマンティクス <a id="SS_5_8_1"></a>
純粋数学での実数の等号(=)は、任意の実数x、y、zに対して、

| 律   |意味                     |
|------|-------------------------|
|反射律|x = x                    |
|対称律|x = yならばy = x         |
|推移律|x = y且つy = zならばx = z|

を満たしている。x = yが成立する場合、「xはyと等しい」もしくは「xはyと同一」であると言う。

C++における組み込みの==も純粋数学の等号と同じ性質を満たしている。
下記のコードは、その性質を表している。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 12

    auto  a = 0;
    auto& b = a;

    ASSERT_TRUE(a == b);
    ASSERT_TRUE(&a == &b);  // aとbは同一
```

しかし、下記のコード内のa、bは同じ値を持つが、
アドレスが異なるため同一のオブジェクトではないにもかかわらず、組み込みの==の値はtrueとなる。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 22

    auto a = 0;
    auto b = 0;

    ASSERT_TRUE(a == b);
    ASSERT_FALSE(&a == &b);  // aとbは同一ではない
```

このような場合、aとbは等価であるという。同一ならば等価であるが、等価であっても同一とは限らない。

ポインタや配列をオペランドとする場合を除き、C++における組み込みの==は、
数学の等号とは違い、等価を表していると考えられるが、
上記した3つの律を守っている。従ってオーバーロードoperator==も同じ性質を守る必要がある。

組み込みの==やオーバーロードoperator==のこのような性質をここでは「等価性のセマンティクス」と呼ぶ。

クラスAを下記のように定義し、

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 33

    class A {
    public:
        explicit A(int num, char const* name) noexcept : num_{num}, name_{name} {}

        int         GetNum() const noexcept { return num_; }
        char const* GetName() const noexcept { return name_; }

    private:
        int const   num_;
        char const* name_;
    };
```

そのoperator==を下記のように定義した場合、

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 50

    inline bool operator==(A const& lhs, A const& rhs) noexcept
    {
        return lhs.GetNum() == rhs.GetNum() && lhs.GetName() == rhs.GetName();
    }
```

単体テストは下記のように書けるだろう。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 61

    auto a0 = A{0, "a"};
    auto a1 = A{0, "a"};

    ASSERT_TRUE(a0 == a1);
```

これは、一応パスするが(処理系定義の動作を前提とするため、必ず動作する保証はない)、
下記のようにすると、パスしなくなる。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 71

    char a0_name[] = "a";
    auto a0        = A{0, a0_name};

    char a1_name[] = "a";
    auto a1        = A{0, a1_name};

    ASSERT_TRUE(a0 == a1);  // テストが失敗する
```

一般にポインタの等価性は、その値の同一性ではなく、
そのポインタが指すオブジェクトの等価性で判断されるべきであるが、
先に示したoperator==はその考慮をしていないため、このような結果になった。

次に、これを修正した例を示す。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 90

    inline bool operator==(A const& lhs, A const& rhs) noexcept
    {
        return lhs.GetNum() == rhs.GetNum()
               && std::string_view{lhs.GetName()} == std::string_view{rhs.GetName()};
    }
```

ポインタをメンバに持つクラスのoperator==については、上記したような処理が必要となる。

次に示す例は、基底クラスBaseとそのoperator==である。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 113

    class Base {
    public:
        explicit Base(int b) noexcept : b_{b} {}
        virtual ~Base() = default;
        int GetB() const noexcept { return b_; }

    private:
        int b_;
    };

    inline bool operator==(Base const& lhs, Base const& rhs) noexcept
    {
        return lhs.GetB() == rhs.GetB();
    }
```

次の単体テストが示す通り、これ自体には問題がないように見える。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 133

    auto b0 = Base{0};
    auto b1 = Base{0};
    auto b2 = Base{1};

    ASSERT_TRUE(b0 == b0);
    ASSERT_TRUE(b0 == b1);
    ASSERT_FALSE(b0 == b2);
```

しかし、Baseから派生したクラスDerivedを

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 145

    class Derived : public Base {
    public:
        explicit Derived(int d) noexcept : Base{0}, d_{d} {}
        int GetD() const noexcept { return d_; }

    private:
        int d_;
    };
```

のように定義すると、下記の単体テストで示す通り、等価性のセマンティクスが破壊される。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 159

    {
        auto b = Base{0};
        auto d = Derived{1};

        ASSERT_TRUE(b == d);  // NG bとdは明らかに等価でない
    }
    {
        auto d0 = Derived{0};
        auto d1 = Derived{1};

        ASSERT_TRUE(d0 == d1);  // NG d0とd1は明らかに等価ではない
    }
```

Derived用のoperator==を

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 176

    bool operator==(Derived const& lhs, Derived const& rhs) noexcept
    {
        return lhs.GetB() == rhs.GetB() && lhs.GetD() == rhs.GetD();
    }
```

と定義しても、下記に示す通り部分的な効果しかない。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 186

    auto d0 = Derived{0};
    auto d1 = Derived{1};

    ASSERT_FALSE(d0 == d1);  // OK operator==(Derived const&, Derived const&)の効果で正しい判定

    Base& d0_b_ref = d0;

    ASSERT_TRUE(d0_b_ref == d1);  // NG d0_b_refの実態はd0なのでd1と等価でない
```

この問題は、[RTTI](term_explanation.md#SS_5_10_15)を使った下記のようなコードで対処できる。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 202

    class Base {
    public:
        explicit Base(int b) noexcept : b_{b} {}
        virtual ~Base() = default;
        int GetB() const noexcept { return b_; }

    protected:
        virtual bool is_equal(Base const& rhs) const noexcept { return b_ == rhs.b_; }

    private:
        int b_;

        friend inline bool operator==(Base const& lhs, Base const& rhs) noexcept
        {
            if (typeid(lhs) != typeid(rhs)) {
                return false;
            }

            return lhs.is_equal(rhs);
        }
    };

    class Derived : public Base {
    public:
        explicit Derived(int d) : Base{0}, d_{d} {}
        int GetD() const noexcept { return d_; }

    protected:
        virtual bool is_equal(Base const& rhs) const noexcept
        {
            // operator==によりrhsの型はDerivedであるため、下記のキャストは安全
            auto const& rhs_d = static_cast<Derived const&>(rhs);

            return Base::is_equal(rhs) && d_ == rhs_d.d_;
        }

    private:
        int d_;
    };
```

下記に示す通り、このコードは、
[オープン・クローズドの原則(OCP)](solid.md#SS_1_2)にも対応した柔軟な構造を実現している。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 269

    class DerivedDerived : public Derived {
    public:
        explicit DerivedDerived(int dd) noexcept : Derived{0}, dd_{dd} {}
        int GetDD() const noexcept { return dd_; }

    protected:
        virtual bool is_equal(Base const& rhs) const noexcept
        {
            // operator==によりrhsの型はDerivedDerivedであるため、下記のキャストは安全
            auto const& rhs_d = static_cast<DerivedDerived const&>(rhs);

            return Derived::is_equal(rhs) && dd_ == rhs_d.dd_;
        }

    private:
        int dd_;
    };
```

前例では「両辺の型が等しいこと」が「等価であること」の必要条件となるが、
この要件が、すべてのoperator==に求められるわけではない。

次に示すのは、一見すると両辺の型が違うにもかかわらず、
等価性のセマンティクスを満たしている例である。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 319

    auto abc = std::string{"abc"};

    ASSERT_TRUE("abc" == abc);
    ASSERT_TRUE(abc == "abc");
```

これは、文字列リテラルを第1引数に取るstd::stringのコンストラクタが非explicitであることによって、
文字列リテラルからstd::stringへの暗黙の型変換が起こるために成立する
(「[非explitなコンストラクタによる暗黙の型変換](term_explanation.md#SS_5_2_5)」参照)。

以上で見てきたように、等価性のセマンティクスを守ったoperator==の実装には多くの観点が必要になる。

### copyセマンティクス <a id="SS_5_8_2"></a>
copyセマンティクスとは以下を満たすようなセマンティクスである。

* a = bが行われた後に、aとbが等価である。
* a = bが行われた前後でbの値が変わっていない。

従って、これらのオブジェクトに対して[等価性のセマンティクス](term_explanation.md#SS_5_8_1)
を満たすoperator==が定義されている場合、
以下を満たすようなセマンティクスであると言い換えることができる。

* a = bが行われた後に、a == bがtrueになる。
* b == b_preがtrueの時に、a = bが行われた後でもb == b_preがtrueとなる。

下記に示す通り、std::stringはcopyセマンティクスを満たしている。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 333

    auto c_str = "string";
    auto str   = std::string{};

    str = c_str;
    ASSERT_TRUE(c_str == str);      // = 後には == が成立している
    ASSERT_STREQ("string", c_str);  // c_strの値は変わっていない
```

一方で、std::auto_ptrはcopyセマンティクスを満たしていない。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 346

    std::auto_ptr<std::string> str0{new std::string{"string"}};
    std::auto_ptr<std::string> str0_pre{new std::string{"string"}};

    ASSERT_TRUE(*str0 == *str0_pre);  // 前提は成立

    std::auto_ptr<std::string> str1;

    str1 = str0;

    // ASSERT_TRUE(*str0 == *str0_pre);  // これをするとクラッシュする
    ASSERT_TRUE(str0.get() == nullptr);  // str0の値がoperator ==で変わってしまった

    ASSERT_TRUE(*str1 == *str0_pre);  // これは成立

```

この仕様は極めて不自然であり、std::auto_ptrはC++11で非推奨となり、C++17で規格から排除された。

下記の単体テストから明らかな通り、
「[等価性のセマンティクス](term_explanation.md#SS_5_8_1)」で示した最後の例も、copyセマンティクスを満たしていない。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 366

    auto b = Base{1};
    auto d = Derived{1};

    b = d;  // スライシングが起こる

    ASSERT_FALSE(b == d);  // copyセマンティクスを満たしていない
```

原因は、copy代入で[スライシング](term_explanation.md#SS_5_3_3)が起こるためである。


### moveセマンティクス <a id="SS_5_8_3"></a>
moveセマンティクスとは以下を満たすようなセマンティクスである(operator==が定義されていると前提)。

* copy代入の実行コスト >= move代入の実行コスト
* a == bがtrueの時に、c = std::move(a)が行われた場合、
    * b == cがtrueになる。
    * a == cはtrueにならなくても良い(aはmove代入により破壊されるかもしれない)。

  必須ではないが、「aがポインタ等のリソースを保有している場合、move代入後には、
  そのリソースはcに移動している」ことが一般的である(「[rvalue](term_explanation.md#SS_5_5_3)」参照)。

* [no-fail保証](term_explanation.md#SS_5_7_1)をする(noexceptと宣言し、エクセプションをthrowしない)。

moveセマンティクスはcopy代入後に使用されなくなるオブジェクト(主にrvalue)
からのcopy代入の実行コストを下げるために導入されたため、
下記のようなコードは推奨されない。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 381

    class NotRecommended {
    public:
        NotRecommended(char const* name) : name_{name} {}
        std::string const& Name() const noexcept { return name_; }

        NotRecommended& operator=(NotRecommended&& rhs)  // move代入、非no-fail保証
        {
            name_ = rhs.name_;  // rhs.name_からname_へのcopy代入。パフォーマンス問題になるかも。
            return *this;
        }

    private:
        std::string name_;
    };

    bool operator==(NotRecommended const& lhs, NotRecommended const& rhs) noexcept
    {
        return lhs.Name() == rhs.Name();
    }

    TEST(Semantics, move1)
    {
        auto a = NotRecommended{"a"};
        auto b = NotRecommended{"a"};

        ASSERT_EQ("a", a.Name());
        ASSERT_TRUE(a == b);

        auto c = NotRecommended{"c"};
        ASSERT_EQ("c", c.Name());

        c = std::move(a);
        ASSERT_TRUE(b == c);  // 一応、moveセマンティクスは守っているが・・・
    }

```

下記のコードのようにメンバの代入もできる限りmove代入を使うことで、
パフォーマンスの良い代入ができる。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp 419

    class Recommended {
    public:
        Recommended(char const* name) : name_{name} {}
        std::string const& Name() const noexcept { return name_; }

        Recommended& operator=(Recommended&& rhs) noexcept  // move代入、no-fail保証
        {
            name_ = std::move(rhs.name_);  // rhs.name_からname_へのmove代入
            return *this;
        }

    private:
        std::string name_;
    };

    bool operator==(Recommended const& lhs, Recommended const& rhs) noexcept
    {
        return lhs.Name() == rhs.Name();
    }

    TEST(Semantics, move2)
    {
        auto a = Recommended{"a"};
        auto b = Recommended{"a"};

        ASSERT_EQ("a", a.Name());
        ASSERT_TRUE(a == b);

        auto c = Recommended{"c"};
        ASSERT_EQ("c", c.Name());

        c = std::move(a);     // これ以降aは使ってはならない
        ASSERT_TRUE(b == c);  // moveセマンティクスを正しく守っている
    }

```

## C++コンパイラ <a id="SS_5_9"></a>
本ドキュメントで使用するg++/clang++のバージョンは以下のとおりである。

### g++ <a id="SS_5_9_1"></a>
```
    g++ (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0
    Copyright (C) 2021 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

### clang++ <a id="SS_5_9_2"></a>
```
    Ubuntu clang version 14.0.0-1ubuntu1
    Target: x86_64-pc-linux-gnu
    Thread model: posix
    InstalledDir: /usr/bin
```

## C++その他 <a id="SS_5_10"></a>
### オーバーライドとオーバーロードの違い <a id="SS_5_10_1"></a>
下記例では、Base::g()がオーバーロードで、Derived::f()がオーバーライドである
(Derived::g()はオーバーロードでもオーバーライドでもない(「[name-hiding](term_explanation.md#SS_5_4_8)」参照))。


```cpp
    // @@@ example/term_explanation/override_overload_ut.cpp 5

    class Base {
    public:
        virtual ~Base() = default;
        virtual std::string f() { return "Base::f"; }
        std::string         g() { return "Base::g"; }

        // g()のオーバーロード
        std::string g(int) { return "Base::g(int)"; }
    };

    class Derived : public Base {
    public:
        // Base::fのオーバーライド
        virtual std::string f() override { return "Derived::f"; }

        // Base::gのname-hiding
        std::string g() { return "Derived::g"; }
    };
```

下記図の通り、

* BaseのインスタンスはBase用のvtblへのポインタを内部に持ち、
  そのvtblでBase::f()のアドレスを保持する。
* DerivedのインスタンスはDerived用のvtblへのポインタを内部に持ち、
  そのvtblでDerived::f()のアドレスを保持する。
* Base::g()、Base::g(int)、
  Derived::g()のアドレスはBaseやDerivedのインスタンスから辿ることはできない。

<p><img src="data:image/png;base64,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" /></p>

vtblとは仮想関数テーブルとも呼ばれる、仮想関数ポインタを保持するための上記のようなテーブルである
(「[クラスのレイアウト](term_explanation.md#SS_5_2_11)」参照)。

Base::f()、Derived::f()の呼び出し選択は、オブジェクトの表層の型ではなく、実際の型により決定される。
Base::g()、Derived::g()の呼び出し選択は、オブジェクトの表層の型により決定される。

```cpp
    // @@@ example/term_explanation/override_overload_ut.cpp 29

    auto  ret   = std::string{};
    auto  b     = Base{};
    auto  d     = Derived{};
    Base& d_ref = d;

    ret = b.f();  // Base::f()呼び出し
    ASSERT_EQ("Base::f", ret);

    ret = d.f();  // Derived::f()呼び出し
    ASSERT_EQ("Derived::f", ret);

    ret = b.g();  // Base::g()呼び出し
    ASSERT_EQ("Base::g", ret);

    ret = d.g();  // Derived::g()呼び出し
    ASSERT_EQ("Derived::g", ret);
    // ret = d.g(int{});   // Derived::gによって、Base::gが隠されるのでコンパイルエラー

    ret = d_ref.f();  // Base::fはDerived::fによってオーバーライドされたので、Derived::f()呼び出し
    ASSERT_EQ("Derived::f", ret);

    ret = d_ref.g();  // d_refの表層型はBaseなので、Base::g()呼び出し
    ASSERT_EQ("Base::g", ret);

    ret = d_ref.g(int{});  // d_refの表層型はBaseなので、Base::g(int)呼び出し
    ASSERT_EQ("Base::g(int)", ret);
```

上記のメンバ関数呼び出し

```cpp
    d_ref.f() 
```

がどのように解釈され、Derived::f()が選択されるかを以下に疑似コードで例示する。

```cpp
    vtbl = d_ref.vtbl             // d_refの実態はDerivedなのでvtblはDerivedのvtbl

    member_func = vtbl->f         // vtbl->fはDerived::f()のアドレス

    (d_ref.*member_func)(&d_ref)  // member_func()の呼び出し
```

このようなメカニズムにより仮想関数呼び出しが行われる。


### 実引数/仮引数 <a id="SS_5_10_2"></a>
引数(もしくは実引数、argument)、仮引数(parameter)とは下記のように定義される。

```cpp
    // @@@ example/term_explanation/argument.cpp 2

    int f0(int a, int& b) noexcept  // a, bは仮引数
    {
        ...
    }

    void f1() noexcept
    {
        ...

        f0(x, y);  // x, yは実引数
    }
```

### 範囲for文 <a id="SS_5_10_3"></a>
範囲for文は下記例のコメントで示されたようなfor文であり、
begin()、end()によって表される範囲内のすべての要素に対して付属するブロックの処理を行う。

```cpp
    // @@@ example/term_explanation/range_for_ut.cpp 9

    auto list = std::list{1, 2, 3};

    for (auto const a : list) {  // 範囲for文
        std::cout << a << std::endl;
    }

    // 上記for文は下記for文のシンタックスシュガー
    for (std::list<int32_t>::iterator it = std::begin(list); it != std::end(list); ++it) {
        std::cout << *it << std::endl;
    }
```
```cpp
    // @@@ example/term_explanation/range_for_ut.cpp 25

    int32_t array[3]{1, 2, 3};

    for (auto const a : array) {  // 範囲for文
        std::cout << a << std::endl;
    }

    // 上記for文は下記for文のシンタックスシュガー
    for (int32_t* it = std::begin(array); it != std::end(array); ++it) {
        std::cout << *it << std::endl;
    }
```

### ラムダ式 <a id="SS_5_10_4"></a>
ラムダ式に関する言葉の定義と例を示す。

* ラムダ式とは、その場で関数オブジェクトを定義する式。
* クロージャ(オブジェクト)とは、ラムダ式から生成された関数オブジェクト。
* クロージャ型とは、クロージャオブジェクトの型。
* キャプチャとは、ラムダ式外部の変数をラムダ式内にコピーかリファレンスとして定義する機能。
* ラムダ式からキャプチャできるのは、ラムダ式から可視である自動変数と仮引数(thisを含む)。
* [ジェネリックラムダ](term_explanation.md#SS_5_10_5)とは、C++11のラムダ式を拡張して、
  パラメータにautoを使用(型推測)できるようにした機能。

```cpp
    // @@@ example/term_explanation/lambda.cpp 10

    auto a = 0;

    // closureがクロージャ。それを初期化する式がラムダ式
    // [a = a]がキャプチャ
    // [a = a]内の右辺aは上記で定義されたa
    // [a = a]内の左辺aは右辺aで初期化された変数。ラムダ式内で使用されるaは左辺a。
    auto closure = [a = a](int32_t b) noexcept { return a + b; };

    auto ret = closure(3);  // クロージャの実行

    // g_closureはジェネリックラムダ
    auto g_closure = [](auto t0, auto t1) { return t0 + t1; };

    auto i = g_closure(1, 2);                                // t0、t1はint
    auto s = g_closure(std::string{"1"}, std::string{"2"});  // t0、t1はstd::string
```

### ジェネリックラムダ <a id="SS_5_10_5"></a>
ジェネリックラムダとは、C++11のラムダ式のパラメータの型にautoを指定できるようにした機能で、
C++14で導入された。

この機能により関数の中で関数テンプレートと同等のものが定義できるようになった。

ジェネリックラムダで定義されたクロージャは、通常のラムダと同様にオブジェクトであるため、
下記のように使用することもできる便利な記法である。

```cpp
    // @@@ example/term_explanation/generic_lambda_ut.cpp 4

    template <typename PUTTO>
    void f(PUTTO&& p)
    {
        p(1);
        p(2.71);
        p("str");
    }

    TEST(Template, generic_lambda)
    {
        std::ostringstream oss;

        f([&oss](auto const& elem) { oss << elem << std::endl; });

        ASSERT_EQ("1\n2.71\nstr\n", oss.str());
    }
```

なお、上記のジェネリックラムダは下記クラスのインスタンスの動きと同じである。

```cpp
    // @@@ example/term_explanation/generic_lambda_ut.cpp 23

    class Closure {
    public:
        Closure(std::ostream& os) : os_(os) {}

        template <typename T>
        void operator()(T&& t)
        {
            os_ << t << std::endl;
        }

    private:
        std::ostream& os_;
    };

    TEST(Template, generic_lambda_like)
    {
        std::ostringstream oss;

        Closure closure(oss);
        f(closure);

        ASSERT_EQ("1\n2.71\nstr\n", oss.str());
    }
```

### 関数tryブロック <a id="SS_5_10_6"></a>
関数tryブロックとはtry-catchを本体とした下記のような関数のブロックを指す。

```cpp
    // @@@ example/term_explanation/func_try_block.cpp 8

    void function_try_block()
    try {  // 関数tryブロック
        // 何らかの処理
        ...
    }
    catch (std::length_error const& e) {  // 関数tryブロックのエクセプションハンドラ
        ...
    }
    catch (std::logic_error const& e) {  // 関数tryブロックのエクセプションハンドラ
        ...
    }
```

### 単純代入 <a id="SS_5_10_7"></a>
代入は下記のように分類される。

* 単純代入(=)
* 複合代入(+=，++ 等)

### ill-formed <a id="SS_5_10_8"></a>
[標準規格と処理系](https://cpprefjp.github.io/implementation-compliance.html)に詳しい解説があるが、

* well-formed(適格)とはプログラムが全ての構文規則・診断対象の意味規則・
  単一定義規則を満たすことである。
* ill-formed(不適格)とはプログラムが適格でないことである。

プログラムがwell-formedになった場合、そのプログラムはコンパイルできる。
プログラムがill-formedになった場合、通常はコンパイルエラーになるが、
対象がテンプレートの場合、事情は少々異なり、[SFINAE](term_explanation.md#SS_5_4_7)によりコンパイルできることもある。


### well-formed <a id="SS_5_10_9"></a>
「[ill-formed](term_explanation.md#SS_5_10_8)」を参照せよ。

### one-definition rule <a id="SS_5_10_10"></a>
「[ODR](term_explanation.md#SS_5_10_11)」を参照せよ。

### ODR <a id="SS_5_10_11"></a>
ODRとは、One Definition Ruleの略語であり、下記のようなことを定めている。

* どの翻訳単位でも、テンプレート、型、関数、またはオブジェクトは、複数の定義を持つことができない。
* プログラム全体で、オブジェクトまたは非インライン関数は複数の定義を持つことはできない。
* 型、テンプレート、外部インライン関数等、いくつかのものは複数の翻訳単位で定義することができる。

より詳しい内容がが知りたい場合は、
[https://en.cppreference.com/w/cpp/language/definition](https://en.cppreference.com/w/cpp/language/definition)
が参考になる。

### RVO(Return Value Optimization) <a id="SS_5_10_12"></a>
関数の戻り値がオブジェクトである場合、
戻り値オブジェクトは、その関数の呼び出し元のオブジェクトにコピーされた後、すぐに破棄される。
この「オブジェクトをコピーして、その後すぐにそのオブジェクトを破棄する」動作は、
「関数の戻り値オブジェクトをそのままその関数の呼び出し元で使用する」ことで効率的になる。
RVOとはこのような最適化を指す。

なお、このような最適化は、
[C++17から規格化](https://cpprefjp.github.io/lang/cpp17/guaranteed_copy_elision.html)された。


### SSO(Small String Optimization) <a id="SS_5_10_13"></a>
一般にstd::stringで文字列を保持する場合、newしたメモリが使用される。
64ビット環境であれば、newしたメモリのアドレスを保持する領域は8バイトになる。
std::stringで保持する文字列が終端の'\0'も含め8バイト以下である場合、
アドレスを保持する領域をその文字列の格納に使用すれば、newする必要がない(当然deleteも不要)。
こうすることで、短い文字列を保持するstd::stringオブジェクトは効率的に動作できる。

SOOとはこのような最適化を指す。

### Most Vexing Parse <a id="SS_5_10_14"></a>
Most Vexing Parse(最も困惑させる構文解析)とは、C++の文法に関連する問題で、
Scott Meyersが彼の著書"Effective STL"の中でこの現象に名前をつけたことに由来する。

この問題はC++の文法が関数の宣言と変数の定義とを曖昧に扱うことによって生じる。
特にオブジェクトの初期化の文脈で発生し、意図に反して、その行は関数宣言になってしまう。

```cpp
    // @@@ example/term_explanation/most_vexing_parse_ut.cpp 6

    class Vexing {
    public:
        Vexing(int) {}
    };

    // @@@ example/term_explanation/most_vexing_parse_ut.cpp 19

    Vexing obj1();        // はローカルオブジェクトobj1の宣言ではない
    Vexing obj2(Vexing);  // ローカルオブジェクトobj2の宣言ではない

    ASSERT_EQ("Vexing ()", Nstd::Type2Str<decltype(obj1)>());
    ASSERT_EQ("Vexing (Vexing)", Nstd::Type2Str<decltype(obj2)>());
    // 上記単体テストが示すように、
    // * obj1はVexingを返す関数
    // * obj2はVexingを引数に取りVexingを返す関数
    // となる。
```

[初期化子リストコンストラクタ](term_explanation.md#SS_5_2_2)の呼び出しでオブジェクトの初期化を行うことで、
このような問題を回避できる。


### RTTI <a id="SS_5_10_15"></a>
RTTI(Run-time Type Information)とは、プログラム実行中のオブジェクトの型を導出するための機能であり、
具体的には下記の3つの要素を指す。

* dynamic_cast
* typeid
* std::type_info

下記のようなポリモーフィックな(virtual関数を持った)クラスに対しては、

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp 7

    class Polymorphic_Base {  // ポリモーフィックな基底クラス
    public:
        virtual ~Polymorphic_Base() = default;
    };

    class Polymorphic_Derived : public Polymorphic_Base {  // ポリモーフィックな派生クラス
    };
```

dynamic_cast、typeidやその戻り値であるstd::type_infoは、下記のように振舞う。

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp 21

    auto b = Polymorphic_Base{};
    auto d = Polymorphic_Derived{};

    Polymorphic_Base& b_ref_d = d;
    Polymorphic_Base& b_ref_b = b;

    // std::type_infoの比較
    ASSERT_EQ(typeid(b_ref_d), typeid(d));
    ASSERT_EQ(typeid(b_ref_b), typeid(b));

    // ポインタへのdynamic_cast
    auto* d_ptr = dynamic_cast<Polymorphic_Derived*>(&b_ref_d);
    ASSERT_EQ(d_ptr, &d);

    auto* d_ptr2 = dynamic_cast<Polymorphic_Derived*>(&b_ref_b);
    ASSERT_EQ(d_ptr2, nullptr);  // キャストできない場合、nullptrが返る

    // リファレンスへのdynamic_cast
    auto& d_ref = dynamic_cast<Polymorphic_Derived&>(b_ref_d);
    ASSERT_EQ(&d_ref, &d);

    // キャストできない場合、エクセプションのが発生する
    ASSERT_THROW(dynamic_cast<Polymorphic_Derived&>(b_ref_b), std::bad_cast);
```

下記のような非ポリモーフィックな(virtual関数を持たない)クラスに対しては、

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp 53

    class NonPolymorphic_Base {  // 非ポリモーフィックな基底クラス
    };

    class NonPolymorphic_Derived : public NonPolymorphic_Base {  // 非ポリモーフィックな派生クラス
    };
```

dynamic_cast、typeidやその戻り値であるstd::type_infoは、下記のように振舞う。

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp 65

    auto b = NonPolymorphic_Base{};
    auto d = NonPolymorphic_Derived{};

    NonPolymorphic_Base& b_ref_d = d;
    NonPolymorphic_Base& b_ref_b = b;

    // std::type_infoの比較
    ASSERT_EQ(typeid(b_ref_d), typeid(b));  // 実際の型ではなく、表層型のtype_infoが返る
    ASSERT_EQ(typeid(b_ref_b), typeid(b));

    // virtual関数を持たないため、ポインタへのdynamic_castはコンパイルできない
    // auto* d_ptr = dynamic_cast<NonPolymorphic_Derived*>(&b_ref_d);
    // auto* d_ptr2 = dynamic_cast<NonPolymorphic_Derived*>(&b_ref_b);

    // virtual関数を持たないため、リファレンスへのdynamic_castはコンパイルできない
    // auto& d_ref = dynamic_cast<NonPolymorphic_Derived&>(b_ref_d);
    // ASSERT_THROW(dynamic_cast<NonPolymorphic_Derived&>(b_ref_b), std::bad_cast);
```

### Run-time Type Information <a id="SS_5_10_16"></a>
「[RTTI](term_explanation.md#SS_5_10_15)」を参照せよ。

### simple-declaration <a id="SS_5_10_17"></a>
このための記述が
[simple-declaration](https://cpprefjp.github.io/lang/cpp17/selection_statements_with_initializer.html)
とは、C++17から導入された
「従来for文しか使用できなかった初期化をif文とswitch文でも使えるようにする」ための記述方法である。

```cpp
    // @@@ example/term_explanation/simple_declaration_ut.cpp 9
    int32_t f();
    int32_t g1()
    {
        if (auto ret = f(); ret != 0) {  // retがsimple-declaration
            return ret;
        }
        else {
            return 0;
        }
    }

    int32_t g2()
    {
        switch (auto ret = f()) {  // retがsimple-declaration
        case 0:
            return 0;
        case 1:
            return ret * 5;
        case 2:
            return ret + 3;
        default:
            return -1;
        }
    }
```

### typeid <a id="SS_5_10_18"></a>
「[RTTI](term_explanation.md#SS_5_10_15)」を参照せよ。

### トライグラフ <a id="SS_5_10_19"></a>
トライグラフとは、2つの疑問符とその後に続く1文字によって表される、下記の文字列である。

```
    ??=  ??/  ??'  ??(  ??)  ??!  ??<  ??>  ??-
```

### フリースタンディング環境 <a id="SS_5_10_20"></a>
[フリースタンディング環境](https://ja.wikipedia.org/wiki/%E3%83%95%E3%83%AA%E3%83%BC%E3%82%B9%E3%82%BF%E3%83%B3%E3%83%87%E3%82%A3%E3%83%B3%E3%82%B0%E7%92%B0%E5%A2%83)
とは、組み込みソフトウェアやOSのように、その実行にOSの補助を受けられないソフトウエアを指す。

## ソフトウェア一般 <a id="SS_5_11"></a>
### 凝集度 <a id="SS_5_11_1"></a>
[凝集度](https://ja.wikipedia.org/wiki/%E5%87%9D%E9%9B%86%E5%BA%A6)
とはクラス設計の妥当性を表す尺度の一種であり、
Lack of Cohesion in Methodsというメトリクスで計測される。

* Lack of Cohesion in Methodsの値が1に近ければ凝集度は低く、この値が0に近ければ凝集度は高い。
* 凝集度とは結合度とも呼ばれ、メンバ(メンバ変数、メンバ関数等)間の結びつきを表す。
  メンバ間の結びつきが強いほど、良い設計とされる。
* メンバ変数やメンバ関数が多くなれば、凝集度は低くなりやすい。
* 「[単一責任の原則(SRP)](solid.md#SS_1_1)」を守ると凝集度は高くなりやすい。
* 「[Accessor](design_pattern.md#SS_2_4)」を多用すれば凝集度は低くなる。従って、下記のようなクラスは凝集度が低い。
   言い換えれば、凝集度を下げることなく、より小さいクラスに分割できる。

```cpp
    // @@@ example/term_explanation/lack_of_cohesion_ut.cpp 7

    class ABC {
    public:
        explicit ABC(int32_t a, int32_t b, int32_t c) noexcept : a_{a}, b_{b}, c_{c} {}

        int32_t GetA() const noexcept { return a_; }
        int32_t GetB() const noexcept { return b_; }
        int32_t GetC() const noexcept { return c_; }
        void    SetA(int32_t a) noexcept { a_ = a; }
        void    SetB(int32_t b) noexcept { b_ = b; }
        void    SetC(int32_t c) noexcept { c_ = c; }

    private:
        int32_t a_;
        int32_t b_;
        int32_t c_;
    };
```

良く設計されたクラスは、下記のようにメンバが結合しあっているため凝集度が高い
(ただし、「[Immutable](design_pattern.md#SS_2_6)」の観点からは、QuadraticEquation::Set()がない方が良い)。
言い換えれば、凝集度を落とさずにクラスを分割することは難しい。

```cpp
    // @@@ example/term_explanation/lack_of_cohesion_ut.cpp 26

    class QuadraticEquation {  // 2次方程式
    public:
        explicit QuadraticEquation(int32_t a, int32_t b, int32_t c) noexcept : a_{a}, b_{b}, c_{c} {}

        void Set(int32_t a, int32_t b, int32_t c) noexcept
        {
            a_ = a;
            b_ = b;
            c_ = c;
        }

        int32_t Discriminant() const noexcept  // 判定式
        {
            return b_ * b_ - 4 * a_ * c_;
        }

        bool HasRealNumberSolution() const noexcept { return 0 <= Discriminant(); }

        std::pair<int32_t, int32_t> Solution() const;

    private:
        int32_t a_;
        int32_t b_;
        int32_t c_;
    };

    std::pair<int32_t, int32_t> QuadraticEquation::Solution() const
    {
        if (!HasRealNumberSolution()) {
            throw std::invalid_argument{"solution is an imaginary number"};
        }

        auto a0 = static_cast<int32_t>((-b_ - std::sqrt(Discriminant())) / 2);
        auto a1 = static_cast<int32_t>((-b_ + std::sqrt(Discriminant())) / 2);

        return {a0, a1};
    }
```


### サイクロマティック複雑度 <a id="SS_5_11_2"></a>
[サイクロマティック複雑度](https://ja.wikipedia.org/wiki/%E5%BE%AA%E7%92%B0%E7%9A%84%E8%A4%87%E9%9B%91%E5%BA%A6)
とは関数の複雑さを表すメトリクスである。
このメトリクスの解釈は諸説あるものの、概ね以下のテーブルのようなものである。

|サイクロマティック複雑度(CC)|複雑さの状態                              |
|:--------------------------:|:-----------------------------------------|
|           CC <= 10         |非常に良い構造                            |
|      11 < CC <  30         |やや複雑                                  |
|      31 < CC <  50         |構造的なリスクあり                        |
|      51 < CC               |テスト不可能、デグレードリスクが非常に高い|


### Spurious Wakeup <a id="SS_5_11_3"></a>
[Spurious Wakeup](https://en.wikipedia.org/wiki/Spurious_wakeup)とは、
条件変数に対する通知待ちの状態であるスレッドが、その通知がされていないにもかかわらず、
起き上がってしまう現象のことを指す。

下記のようなstd::condition_variableの使用で起こり得る。

```cpp
    // @@@ example/term_explanation/spurious_wakeup_ut.cpp 8

    namespace {
    std::mutex              mutex;
    std::condition_variable cond_var;
    }  // namespace

    void notify_wrong()  // 通知を行うスレッドが呼び出す関数
    {
        auto lock = std::lock_guard{mutex};

        cond_var.notify_all();  // wait()で待ち状態のスレッドを起こす。
    }

    void wait_wrong()  // 通知待ちスレッドが呼び出す関数
    {
        auto lock = std::unique_lock{mutex};

        // notifyされるのを待つ。
        cond_var.wait(lock);  // notify_allされなくても起き上がってしまうことがある。

        // do something
    }
```

std::condition_variable::wait()の第2引数を下記のようにすることでこの現象を回避できる。

```cpp
    // @@@ example/term_explanation/spurious_wakeup_ut.cpp 34

    namespace {
    bool                    event_occured{false};
    std::mutex              mutex;
    std::condition_variable cond_var;
    }  // namespace

    void notify_right()  // 通知を行うスレッドが呼び出す関数
    {
        auto lock = std::lock_guard{mutex};

        event_occured = true;

        cond_var.notify_all();  // wait()で待ち状態のスレッドを起こす。
    }

    void wait_right()  // 通知待ちスレッドが呼び出す関数
    {
        auto lock = std::unique_lock{mutex};

        // notifyされるのを待つ。
        cond_var.wait(lock, []() noexcept { return event_occured; });  // Spurious Wakeup対策

        event_occured = false;

        // do something
    }
```

### 副作用 <a id="SS_5_11_4"></a>
プログラミングにおいて、式の評価による作用には、
主たる作用とそれ以外の
[副作用](https://ja.wikipedia.org/wiki/%E5%89%AF%E4%BD%9C%E7%94%A8_(%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%A0))
(side effect)とがある。
式は、評価値を得ること(関数では「引数を受け取り値を返す」と表現する)が主たる作用とされ、
それ以外のコンピュータの論理的状態(ローカル環境以外の状態変数の値)を変化させる作用を副作用という。
副作用の例としては、グローバル変数や静的ローカル変数の変更、
ファイルの読み書き等のI/O実行、等がある。


### is-a <a id="SS_5_11_5"></a>
「is-a」の関係は、オブジェクト指向プログラミング（OOP）
においてクラス間の継承関係を説明する際に使われる概念である。
クラスDerivedとBaseが「is-a」の関係である場合、
DerivedがBaseの派生クラスであり、Baseの特性をDerivedが引き継いでいることを意味する。
C++でのOOPでは、DerivedはBaseのpublic継承として定義される。
通常DerivedやBaseは以下の条件を満たす必要がある。

* Baseはvirtualメンバ関数(Base::f)を持つ。
* DerivedはBase::fのオーバーライド関数を持つ。
* DerivedはBaseに対して
  [リスコフの置換原則](https://ja.wikipedia.org/wiki/%E3%83%AA%E3%82%B9%E3%82%B3%E3%83%95%E3%81%AE%E7%BD%AE%E6%8F%9B%E5%8E%9F%E5%89%87)
  を守る必要がある。
  この原則を簡単にに説明すると、
  「派生クラスのオブジェクトは、
  いつでもその基底クラスのオブジェクトと置き換えても、
  プログラムの動作に悪影響を与えずに問題が発生してはならない」という設計の制約である。

 「is-a」の関係とは「一種の～」と言い換えることができることが多い.
ペンギンや九官鳥 は一種の鳥であるため、この関係を使用したコード例を次に示す。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp 11

    class bird {
    public:
        //  事前条件: altitude  > 0 でなければならない
        //  事後条件: 呼び出しが成功した場合、is_flyingがtrueを返すことである
        virtual void fly(int altitude)
        {
            if (not(altitude > 0)) {  // 高度(altitude)は0より大きくなければ、飛べない
                throw std::invalid_argument{"altitude error"};
            }
            altitude_ = altitude;
        }

        bool is_flying() const noexcept
        {
            return altitude_ != 0;  // 高度が0でなければ、飛んでいると判断
        }

        virtual ~bird() = default;

    private:
        int altitude_ = 0;
    };

    class kyukancho : public bird {
    public:
        void speak()
        {
            // しゃべるため処理
        }

        // このクラスにget_nameを追加した理由はこの後を読めばわかる
        virtual std::string get_name() const  // その個体の名前を返す
        {
            return "no name";
        }
    };
```

bird::flyのオーバーライド(penguin::fly)について、 リスコフの置換原則に反した例を下記する。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp 50

    class penguin : public bird {
    public:
        void fly(int altitude) override
        {
            if (altitude != 0) {
                throw std::invalid_argument{"altitude error"};
            }
        }
    };

    // ...

    auto let_it_fly = [](bird& b, int altitude) {
        try {
            b.fly(altitude);
        }
        catch (std::exception const&) {
            return 0;  // エクセプションが発生した
        }

        return b.is_flying() ? 2 : 1;  // is_flyingがfalseなら1を返す
    };

    bird    b;
    penguin p;
    ASSERT_EQ(let_it_fly(p, 0), 1);  // パスする
    // birdからpenguinへの派生がリスコフ置換の原則を満たすのであれば、
    // 上記のテストのpをbで置き換えたテストがパスしなければならないが、
    // 実際には逆に下記テストがパスしてしまう
    ASSERT_NE(let_it_fly(b, 0), 1);
    // このことからpenguinへの派生はリスコフ置換の原則を満たさない

```

birdからpenguinへの派生がリスコフ置換の原則に反してしまった原因は以下のように考えることができる。

* bird::flyの事前条件penguin::flyが強めた
* bird::flyの事後条件をpenguin::flyが弱めた

penguinとbirdの関係はis-aの関係ではあるが、
上記コードの問題によって不適切なis-aの関係と言わざるを得ない。

上記の例では鳥全般と鳥の種類のis-a関係をpublic継承を使用して表した(一部不適切であるもの)。
さらにis-aの誤った適用例を示す。
自身が飼っている九官鳥に"キューちゃん"と名付けることははよくあることである。
キューちゃんという名前の九官鳥は一種の九官鳥であることは間違いのないことであるが、
このis-aの関係を表すためにpublic継承を使用するのは、is-aの関係の誤用になることが多い。
実際のコード例を以下に示す。この場合、型とインスタンスの概念の混乱が原因だと思われる。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp 92

    class q_chan : public kyukancho {
    public:
        std::string get_name() const override { return "キューちゃん"; }
    };
```

この誤用を改めた例を以下に示す。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp 114

    class kyukancho {
    public:
        kyukancho(std::string name) : name_{std::move(name)} {}

        std::string const& get_name() const  // 名称をメンバ変数で保持するため、virtualである必要はない
        {
            return name_;
        }

        virtual ~kyukancho() = default;

    private:
        std::string const name_;  // 名称の保持
    };

    // ...

    kyukancho q{"キューちゃん"};

    ASSERT_EQ("キューちゃん", q.get_name());
```

修正されたKyukancho はstd::string インスタンスをメンバ変数として持ち、
kyukanchoとstd::stringの関係を[has-a](term_explanation.md#SS_5_11_6)の関係と呼ぶ。


### has-a <a id="SS_5_11_6"></a>
「has-a」の関係は、
あるクラスのインスタンスが別のクラスのインスタンスを構成要素として含む関係を指す。
つまり、あるクラスのオブジェクトが別のクラスのオブジェクトを保持している関係である。

例えば、CarクラスとEngineクラスがあるとする。CarクラスはEngineクラスのインスタンスを含むので、
CarはEngineを「has-a」の関係にあると言える。
通常、has-aの関係はクラス内でメンバ変数またはメンバオブジェクトとして実装される。
Carクラスの例ではCarクラスにはEngine型のメンバ変数が存在する。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp 145

    class Engine {
    public:
        void start() {}  // エンジンを始動するための処理
        void stop() {}   // エンジンを停止するための処理

    private:
        // ...
    };

    class Car {
    public:
        Car() : engine_{} {}
        void start() { engine_.start(); }
        void stop() { engine_.stop(); }

    private:
        Engine engine_;  // Car は Engine を持っている（has-a）
    };
```

### is-implemented-in-terms-of <a id="SS_5_11_7"></a>
「is-implemented-in-terms-of」の関係は、
オブジェクト指向プログラミング（OOP）において、
あるクラスが別のクラスの機能を内部的に利用して実装されていることを示す概念である。
これは、あるクラスが他のクラスのインターフェースやメソッドを用いて、
自身の機能を提供する場合に使われる。
[has-a](term_explanation.md#SS_5_11_6)の関係は、is-implemented-in-terms-of の関係の一種である。

is-implemented-in-terms-ofは下記の手段1-3に示した方法がある。

*手段1.* [public継承によるis-implemented-in-terms-of](term_explanation.md#SS_5_11_7_1)  
*手段2.* [private継承によるis-implemented-in-terms-of](term_explanation.md#SS_5_11_7_2)  
*手段3.* [コンポジションによる(has-a)is-implemented-in-terms-of](term_explanation.md#SS_5_11_7_3)  

手段1-3にはそれぞれ、長所、短所があるため、必要に応じて手段を選択する必要がある。
以下の議論を単純にするため、下記のようにクラスS、C、CCを定める。

* S(サーバー): 実装を提供するクラス
* C(クライアント): Sの実装を利用するクラス
* CC(クライアントのクライアント): Cのメンバをを使用するクラス

コード量の観点から考えた場合、手段1が最も優れていることが多い。
依存関係の複雑さから考えた場合、CはSに強く依存する。
場合によっては、この依存はCCからSへの依存間にも影響をあたえる。
従って、手段3が依存関係を単純にしやすい。
手段1は[is-a](term_explanation.md#SS_5_11_5)に見え、以下に示すような問題も考慮する必要があるため、
可読性、保守性を劣化させる可能性がある。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp 261

    class MyString : public std::string {  // 手段1
    };

    // ...
    std::string* m_str = new MyString{"str"};

    // このようなpublic継承を行う場合、基底クラスのデストラクタは非virtualであるため、
    // 以下のコードではｈmy_stringのデストラクタは呼び出されない。
    // この問題はリソースリークを発生させる場合がある。
    delete m_str;
```

以上述べたように問題の多い手段1であるが、実践的には有用なパターンであり、
[CRTP(curiously recurring template pattern)](https://ja.wikibooks.org/wiki/More_C%2B%2B_Idioms/%E5%A5%87%E5%A6%99%E3%81%AB%E5%86%8D%E5%B8%B0%E3%81%97%E3%81%9F%E3%83%86%E3%83%B3%E3%83%97%E3%83%AC%E3%83%BC%E3%83%88%E3%83%91%E3%82%BF%E3%83%BC%E3%83%B3(Curiously_Recurring_Template_Pattern)
の実現手段でもあるため、一概にコーディング規約などで排除することもできない。


#### public継承によるis-implemented-in-terms-of <a id="SS_5_11_7_1"></a>
public継承によるis-implemented-in-terms-ofの実装例を以下に示す。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp 283

    class MyString : public std::string {};

    // ...
    MyString str{"str"};

    ASSERT_EQ(str[0], 's');
    ASSERT_STREQ(str.c_str(), "str");

    str.clear();
    ASSERT_EQ(str.size(), 0);
```

すでに述べたようにこの方法は、
[private継承によるis-implemented-in-terms-of](term_explanation.md#SS_5_11_7_2)や、
[コンポジションによる(has-a)is-implemented-in-terms-of](term_explanation.md#SS_5_11_7_3)
と比べコードがシンプルになる。 

#### private継承によるis-implemented-in-terms-of <a id="SS_5_11_7_2"></a>
private継承によるis-implemented-in-terms-ofの実装例を以下に示す。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp 180

    class MyString : std::string {
    public:
        using std::string::string;
        using std::string::operator[];
        using std::string::c_str;
        using std::string::clear;
        using std::string::size;
    };

    // ...
    MyString str{"str"};

    ASSERT_EQ(str[0], 's');
    ASSERT_STREQ(str.c_str(), "str");

    str.clear();
    ASSERT_EQ(str.size(), 0);
```

この方法は、[public継承によるis-implemented-in-terms-of](term_explanation.md#SS_5_11_7_1)が持つデストラクタ問題は発生せす、
[is-a](term_explanation.md#SS_5_11_5)と誤解してしまう問題も発生しない。


#### コンポジションによる(has-a)is-implemented-in-terms-of <a id="SS_5_11_7_3"></a>
コンポジションによる(has-a)is-implemented-in-terms-ofの実装例を示す。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp 208

    namespace is_implemented_in_terms_of_1 {
    class MyString {
    public:
        // コンストラクタ
        MyString() = default;
        MyString(const std::string& str) : str_(str) {}
        MyString(const char* cstr) : str_(cstr) {}

        // 文字列へのアクセス
        const char* c_str() const { return str_.c_str(); }

        using reference = std::string::reference;
        using size_type = std::string::size_type;

        reference operator[](size_type pos) { return str_[pos]; }

        // その他のメソッドも必要に応じて追加する
        // 以下は例
        std::size_t size() const { return str_.size(); }

        void clear() { str_.clear(); }

        MyString& operator+=(const MyString& rhs)
        {
            str_ += rhs.str_;
            return *this;
        }

    private:
        std::string str_;
    };

    // ...
    MyString str{"str"};

    ASSERT_EQ(str[0], 's');
    ASSERT_STREQ(str.c_str(), "str");

    str.clear();
    ASSERT_EQ(str.size(), 0);
```

この方は実装を利用するクラストの依存関係を他の2つに比べるとシンプルにできるが、
逆に実装例から昭なとおり、コード量が増えてしまう。


## 非ソフトウェア用語 <a id="SS_5_12"></a>
### 割れ窓理論 <a id="SS_5_12_1"></a>
[割れ窓理論](https://ja.wikipedia.org/wiki/%E5%89%B2%E3%82%8C%E7%AA%93%E7%90%86%E8%AB%96)とは、
軽微な犯罪も徹底的に取り締まることで、凶悪犯罪を含めた犯罪を抑止できるとする環境犯罪学上の理論。
アメリカの犯罪学者ジョージ・ケリングが考案した。
「建物の窓が壊れているのを放置すると、誰も注意を払っていないという象徴になり、
やがて他の窓もまもなく全て壊される」との考え方からこの名がある。

ソフトウェア開発での割れ窓とは、「朝会に数分遅刻する」、「プログラミング規約を守らない」
等の軽微なルール違反を指し、この理論の実践には、このような問題を放置しないことによって、

* チームのモラルハザードを防ぐ
* コードの品質を高く保つ

等の重要な狙いがある。


### 車輪の再発明 <a id="SS_5_12_2"></a>
[車輪の再発明](https://ja.wikipedia.org/wiki/%E8%BB%8A%E8%BC%AA%E3%81%AE%E5%86%8D%E7%99%BA%E6%98%8E)
とは、広く受け入れられ確立されている技術や解決法を（知らずに、または意図的に無視して）
再び一から作ること」を指すための慣用句である。
ソフトウェア開発では、STLのような優れたライブラリを使わずに、
それと同様なライブラリを自分たちで実装するような非効率な様を指すことが多い。


