<!-- ./md/template_meta_programming.md -->
# テンプレートメタプログラミング <a id="SS_3"></a>

本章でのテンプレートメタプログラミングとは、下記の2つを指す。

* ジェネリックプログラミング
* メタプログラミング

C++においては、この2つはテンプレートを用いたプログラミングとなる。

ジェネリックプログラミングとは、具体的なデータ型に依存しない抽象的プログラミングであり、
その代表的な成果物はSTLのコンテナやそれらを扱うアルゴリズム関数テンプレートである。

この利点は、

* i種の型
* j種のコンテナ
* k種のアルゴリズム

の開発を行うことを考えれば明らかである。

ジェネリックプログラミングが無ければ、コンテナの種類はi x j個必要になり、
それらに適用するアルゴリズム関数は、i x j x k個必要になる。
また、サポートする型の増加に伴いコンテナやアルゴリズム関数は指数関数的に増えて行く。
C言語のqsort()のように強引なキャストを使い、この増加をある程度食い止めることはできるが、
それによりコンパイラによる型チェックは無効化され、
静的な型付け言語を使うメリットの多くを失うことになる。

メタプログラミングとは、

* ジェネリックのサポート
* 実行時コードの最適化
* 関数やクラスを生成するコードのプログラミング

のような目的で行われるテンプレートプログラミングの総称である。

ジェネリックプログラミングとメタプログラミングに明確な境界はない、
また明確にしたところで大きなメリットはと思われるため、
本章では、これらをまとめた概念であるテンプレートメタプログラミングとして扱い、
ログ取得ライブラリやSTLを応用したNstdライブラリの実装を通して、
これらのテクニックや、使用上の注意点について解説する。

## ログ取得ライブラリの開発 <a id="SS_3_1"></a>
ここではログ取得ライブラリの開発を行う。

### 要件 <a id="SS_3_1_1"></a>

ログ取得ライブラリの要件は、

* ソースコードの場所とそこで指示されたオブジェクトの値を文字列で保持する
* 後からそれらを取り出せる

ことのみとする。下記はその文字列を取り出した例である。

```
    app/src/main.cpp: 96:Options
                         cmd       : GenPkg
                         in        : 
                         out       : 
                         recursive : true
                         src_as_pkg: false
        ...

    app/src/main.cpp: 51:start GenPkg

    file_utils/ut/path_utils.cpp: 38:1
    file_utils/ut/path_utils.cpp: 48:ut_data/app1
                         ut_data/app1/mod1
                         ut_data/app1/mod2

        ...

    app/src/main.cpp:100:Exit:0
```

単純化のためログの番号やタイムスタンプのサポートはしない。
また、実行速度や仕様メモリ量の制限等も本章の趣旨とは離れるため考慮しない。

### ログ取得ライブラリのインターフェース <a id="SS_3_1_2"></a>

ログ取得コードにより、コードクローンが増えたり、主なロジックの可読性が下がったのでは、
本末転倒であるため、下記のようにワンライナーで記述できるべきだろう。

```cpp
    LOGGER("start GenPkg", objA, objB, objC);
```

また、要件で述べた通り、ソースコード位置を特定できなければならないため、
上記LOGGERは下記のような関数型マクロにならざるを得ない。

```cpp
    #define LOGGER(...) CppLoggerFunc(__FILE__, __LINE__, __VA_ARGS__)
```

CppLoggerFuncをクラス外の関数として実装した場合、ログ保持のための静的なオブジェクトが必要になる。
これは避けるべきなので、「[Singleton](design_pattern.md#SS_2_12)」で述べた構造を導入すると、

```cpp
    #define LOGGER(...) Logger::Inst().Set(__FILE__, __LINE__, __VA_ARGS__)
```

のような関数型マクロとなる。これがログ取得ライブラリの主なインターフェースとなる。

C言語プログラミングばかりをやりすぎて、
視神経と手の運動神経が直結してしまった大脳レス・プログラマーは、

```cpp
    __VA_ARGS__
```

を見るとprintf(...)のような可変長引数を取る関数を思い浮かべる。
「人は一昨日も行ったことを昨日も行ったという理由で、今日もそれを行う」
という諺を思い出すと気持ちは分からなくもないが、 
C++ではprintf(...)のような危険な可変長引数を取る関数を作ってはならない。
パラメータパックを使って実装するべきである。

### パラメータパック <a id="SS_3_1_3"></a>
C++11で導入されたパラメータパックはやや複雑なシンタックスを持つため、
まずは単純な例から説明する。

次のような単体テストをパスする関数テンプレートsumをパラメータパックで実装することを考える。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp 26

    ASSERT_EQ(1, sum(1));
    ASSERT_EQ(3, sum(1, 2));
    ASSERT_EQ(6, sum(1, 2, 3));
    ASSERT_FLOAT_EQ(6.0, sum(1, 2.0, 3.0));
    ASSERT_EQ(10, sum(1, 2, 3, 4));

    ...

    ASSERT_EQ(55, sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));

    ...
```

sumの要件は、

* 可変長引数を持つ
* [算術型](term_explanation.md#SS_5_1_1)の引数と戻り値を持つ
* すべての引数の和を返す

のようなものになるため、関数テンプレートsumは下記のように書ける。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp 9

    template <typename HEAD>
    int sum(HEAD head)
    {
        return head;
    }

    template <typename HEAD, typename... TAIL>
    int sum(HEAD head, TAIL... tails)
    {
        return head + sum(tails...);
    }
```

1つ目の関数テンプレートsumは引数が一つの場合に対応する。
2つ目の関数テンプレートsumは引数が2つ以上の場合に対応する。

2つ目の関数テンプレートsumのテンプレートパラメータ

```cpp
    typename... TAIL
```

がパラメータパックであり、0個以上の型が指定されることを意味する。
これを関数の引数として表すシンタックスが

```cpp
    TAIL... tails
```

であり、同様に0個以上のインスタンスが指定されることを表している。

HEADとTAILより、2つ目のsumは1個以上の引数を取れることになるため、
引数が1つの場合、どちらのsumを呼び出すかが曖昧になるように思えるが、
ベストマッチの観点から1つ目のsumが呼び出される。

sum(1, 2, 3)の呼び出し時のsumの展開を見てみることでパラメータパックの振る舞いを解説する。

この呼び出しは、2つ目のsumにマッチする。従って下記のように展開される。

```cpp
    return 1 + sum(2, 3);
```

sum(2, 3)も同様に展開されるため、上記コードは下記のようになる。

```cpp
    return 1 + 2 + sum(3);
```

sum(3)は1つ目のsumにマッチするため、最終的には下記のように展開される。

```cpp
    return 1 + 2 + 3;
```

これで基本的な要件は満たしたが、このsumでは下記のようなコードもコンパイルできてしまう。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp 43

    ASSERT_EQ(2, sum(1, true, false));
```

これを認めるかどうかはsumの仕様次第だが、ここではこれらを認めないようにしたい。
また、引数に浮動小数が与えられた場合でも、sumの戻り値の型がintなる仕様には問題がある。
合わせてそれも修正する。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp 53

    template <typename HEAD>
    auto sum(HEAD head)
    {
        // std::is_sameの2パラメータが同一であれば、std::is_same<>::value == true
        static_assert(!std::is_same<HEAD, bool>::value, "arguemnt type must not be bool.");

        return head;
    }

    template <typename HEAD, typename... TAIL>
    auto sum(HEAD head, TAIL... tails)
    {
        // std::is_sameの2パラメータが同一であれば、std::is_same<>::value == true
        static_assert(!std::is_same<HEAD, bool>::value, "arguemnt type must not be bool.");

        return head + sum(tails...);
    }
```
```cpp
    // @@@ example/template/parameter_pack_ut.cpp 83

    // boolを除く算術型のみ認めるため、下記はコンパイルできない。
    // ASSERT_EQ(2, sum(1, true, false));

    auto i1 = sum(1);
    auto i2 = sum(1, 2);

    static_assert(std::is_same<int, decltype(i1)>::value);  // 1の型はint
    static_assert(std::is_same<int, decltype(i2)>::value);  // 1 + 2の型はint

    auto u1 = sum(1U);
    auto u2 = sum(1U, 2);

    static_assert(std::is_same<unsigned int, decltype(u1)>::value);  // 1Uの型はunsigned int
    static_assert(std::is_same<unsigned int, decltype(u2)>::value);  // 1U + 2の型はunsigned int

    auto f0 = sum(1.0, 1.2);
    static_assert(std::is_same<double, decltype(f0)>::value);

    // ただし、戻り型をautoにしたため、下記も認められるようになった。
    // これに対しての対処は別の関数で行う。
    auto str = sum(std::string{"1"}, std::string{"2"});

    ASSERT_EQ(str, "12");
    static_assert(std::is_same<std::string, decltype(str)>::value);
```

以上で示したようにパラメータパックにより、
C言語での可変長引数関数では不可能だった引数の型チェックができるようになったため、
C言語でのランタイムエラーがコンパイルエラーにできるようになった。

なお、上記コードで使用した[std::is_same](template_meta_programming.md#SS_3_3_1_3)は、
与えられた2つのテンプレートパラメータが同じ型であった場合、
valueをtrueで初期化するクラステンプレートであり、 type_traitsで定義されている
(後ほど使用するstd::is_same_vはstd::is_same<>::valueと等価な定数テンプレート)。
この実装については、後ほど説明する。


#### パラメータパックの畳みこみ式 <a id="SS_3_1_3_1"></a>
上記したsumは、パラメータパックの展開に汎用的な再帰構造を用いたが、
C++17で導入された畳みこみ式を用い、以下の様に簡潔に記述することもできる。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp 123

    template <typename... ARGS>
    auto sum(ARGS... args)
    {
        return (args + ...);  // 畳みこみ式は()で囲まなければならない。
    }
```
```cpp
    // @@@ example/template/parameter_pack_ut.cpp 134

    ASSERT_EQ(1, sum(1));
    ASSERT_EQ(3, sum(1, 2));
    ASSERT_EQ(6, sum(1, 2, 3));
    ASSERT_EQ(6.0, sum(1, 2.0, 3.0));
    ASSERT_EQ(10, sum(1, 2, 3, 4));
    ASSERT_EQ(55, sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
```

畳みこみ式で使用できる演算子を以下に示す。

```
    + - * / % ^ & | = < > << >> += -= *= /= %= ^= &= |= <<= >>= == != <= >= && || , .* ->*
```

これらの演算子がオーバーロードである場合でも、畳みこみ式は利用できる。


#### 前から演算するパラメータパック <a id="SS_3_1_3_2"></a>
パラメータパックを使うプログラミングでは、
上記したHEADとTAILによるリカーシブコールがよく使われるパターンであるが、
これには後ろから処理されるという、微妙な問題点がある。

これまでのsumに代えて下記のようなproduct(掛け算)を考える。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp 149

    template <typename HEAD>
    auto product(HEAD head)
    {
        static_assert(!std::is_same_v<HEAD, bool>, "arguemnt type must not be bool.");
        static_assert(std::is_arithmetic_v<HEAD>, "arguemnt type must be arithmetic.");

        return head;
    }

    template <typename HEAD, typename... TAIL>
    auto product(HEAD head, TAIL... tails)
    {
        static_assert(!std::is_same_v<HEAD, bool>, "arguemnt type must not be bool.");
        static_assert(std::is_arithmetic_v<HEAD>, "arguemnt type must be arithmetic.");

        return head * product(tails...);
    }
```

このコードの単体テストは、

```cpp
    // @@@ example/template/parameter_pack_ut.cpp 173

    ASSERT_EQ(1, product(100, 0.1, 0.1));
```

のようになるだろうが、
std::numeric_limits<>::epsilonを使用していないため
、このテストはパスしない。一方で、以下のテストはパスする。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp 178

    ASSERT_EQ(1, product(0.1, 0.1, 100));

```

一般に0.01の2進数表現は無限小数になるため、これを含む演算にはepsilon以下の演算誤差が発生する。
前者単体テストでは、後ろから演算されるために処理の途中に0.01が現れるが、
後者では現れないため、この誤差の有無が結果の差になる。

このような演算順序による微妙な誤差が問題になるような関数を開発する場合、
演算は見た目の順序通りに行われた方が良いだろう。
ということで、productを前から演算するように修正する。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp 196

    template <typename HEAD>
    auto product(HEAD head)
    {
        static_assert(!std::is_same_v<HEAD, bool>, "arguemnt type must not be bool.");
        static_assert(std::is_arithmetic_v<HEAD>, "arguemnt type must be arithmetic.");

        return head;
    }

    template <typename HEAD, typename HEAD2, typename... TAIL>
    auto product(HEAD head, HEAD2 head2, TAIL... tails)
    {
        static_assert(!std::is_same_v<HEAD, bool>, "arguemnt type must not be bool.");
        static_assert(std::is_arithmetic_v<HEAD>, "arguemnt type must be arithmetic.");

        return product(head * head2, tails...);
    }
```

HEAD、TAILに加えHEAD2を導入することで、前からの演算を実装できる
(引数が一つのproductに変更はない)。当然ながら、これにより、

```cpp
    // @@@ example/template/parameter_pack_ut.cpp 220

    ASSERT_EQ(1, product(100, 0.1, 0.1));
```

はパスし、下記はパスしなくなる。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp 225

    ASSERT_EQ(1, product(0.1, 0.1, 100));

```


### Loggerの実装 <a id="SS_3_1_4"></a>

パラメータパックを使用したログ取得コードは以下のようになる。

```cpp
    // @@@ example/template/logger_0.h 47

    #define LOGGER_P(...) Logging::Logger::Inst().Set(__FILE__, __LINE__)
    #define LOGGER(...) Logging::Logger::Inst().Set(__FILE__, __LINE__, __VA_ARGS__)
```

予定していたものと若干違う理由は、\_\_VA\_ARGS\_\_が1個以上の識別子を表しているからである。
従って、通過ポイントのみをロギングしたい場合、LOGGER_P()を使うことになる。
gcc拡張を使えば、LOGGER_PとLOGGERを統一できるが、そのようなことをすると別のコンパイラや、
静的解析ツールが使用できなくなることがあるため、残念だが上記のように実装するべきである。

Loggerクラスの実装は、下記のようになる。

```cpp
    // @@@ example/template/logger_0.h 5

    namespace Logging {
    class Logger {
    public:
        static Logger&       Inst();
        static Logger const& InstConst() { return Inst(); }

        std::string Get() const;  // ログデータの取得
        void        Clear();      // ログデータの消去

        template <typename... ARGS>  // ログの登録
        void Set(char const* filename, uint32_t line_no, ARGS const&... args)
        {
            oss_.width(32);
            oss_ << filename << ":";

            oss_.width(3);
            oss_ << line_no;

            set_inner(args...);
        }

        Logger(Logger const&)            = delete;
        Logger& operator=(Logger const&) = delete;

    private:
        void set_inner() { oss_ << std::endl; }

        template <typename HEAD, typename... TAIL>
        void set_inner(HEAD const& head, TAIL const&... tails)
        {
            oss_ << ":" << head;
            set_inner(tails...);
        }

        Logger() {}
        std::ostringstream oss_{};
    };
    }  // namespace Logging
```

すでに述べた通り、

* クラスはシングルトンにする
* パラメータパックにより可変長引数を実現する

ようにした。
また、識別子の衝突を避けるために、名前空間Loggingを導入し、Loggerはその中で宣言した。

次に、どのように動作するのかを単体テストで示す。

```cpp
    // @@@ example/template/logger_0_ut.cpp 16

    auto a = 1;
    auto b = std::string{"b"};

    LOGGER_P();               // (1)
    LOGGER(5, "hehe", a, b);  // (2)
    auto line_num = __LINE__;

    auto s = Logging::Logger::InstConst().Get();

    auto exp = std::string{log_str_exp(__FILE__, line_num - 2, "\n")};  // (1)のログ
    exp += log_str_exp(__FILE__, line_num - 1, ":5:hehe:1:b\n");        // (2)のログ
    ASSERT_EQ(exp, s);

    Logging::Logger::Inst().Clear();  // クリアの確認
    ASSERT_EQ("", Logging::Logger::InstConst().Get());
```

行を含む出力の期待値をソースコードに直接書くと行増減のたびにそれらを修正する必要ある。
期待値の一部を自動計算する下記コード(上記コードで使用)を単体テストに導入することで、
そういった修正を避けている。

```cpp
    // @@@ example/template/logger_ut.h 4

    inline std::string line_to_str(uint32_t line)
    {
        if (line < 10) {
            return ":  ";
        }
        else if (line < 100) {
            return ": ";
        }
        else if (line < 1000) {
            return ":";
        }
        else {
            assert(false);  // 1000行を超える単体テストファイルを認めない
            return "";
        }
    }

    inline std::string log_str_exp(char const* filename_cstr, uint32_t line, char const* str)
    {
        auto const filename = std::string{filename_cstr};
        auto const len      = 32 > filename.size() ? 32 - filename.size() : 0;
        auto       ret      = std::string(len, ' ');

        ret += filename;
        ret += line_to_str(line);
        ret += std::to_string(line);
        ret += str;

        return ret;
    }
```

アプリケーションの開発では、
下記のようなユーザが定義した名前空間とクラスを用いることがほとんどである。

```cpp
    // @@@ example/template/app_ints.h 12

    namespace App {

    class X {
    public:
        X(std::string str, int num) : str_{std::move(str)}, num_{num} {}
        std::string ToString() const { return str_ + "/" + std::to_string(num_); }
        ...
    };
    }  // namespace App
```

このApp::Xのインスタンスのログを取得できることも、当然Logging::Loggerの要件となる。
従って、下記の単体テストはコンパイルでき、且つパスすることが必要になる。


```cpp
    // @@@ example/template/logger_0_ut.cpp 42

    auto x = App::X{"name", 3};

    LOGGER(1, x);
    auto line_num = __LINE__;

    auto s = Logging::Logger::InstConst().Get();

    auto exp = std::string{log_str_exp(__FILE__, line_num - 1, ":1:name/3\n")};
    ASSERT_EQ(exp, s);
```

Logging::Loggerのコードからオブジェクトのログを登録するためには、
Logging::Logger::set_innerがコンパイルできなければならない。
つまり、

```cpp
    std::ostream& operator<<(std::ostream&, ログ登録オブジェクトの型)
```

の実装が必要条件となる。App::Xでは下記のようなコードになる。

```cpp
    // @@@ example/template/app_ints.h 28

    namespace App {

    inline std::ostream& operator<<(std::ostream& os, X const& x) { return os << x.ToString(); }
    }  // namespace App
```

他の任意のユーザ定義型に対しても、このようにすることでログ登録が可能になる。

なお、ヒューマンリーダブルな文字列でその状態を表示できる関数をユーザ定義型に与えることは、
デバッガを使用したデバッグ時にも有用である。

### ユーザ定義型とそのoperator\<\<のname lookup <a id="SS_3_1_5"></a>

ここで、一旦Logging::Loggerの開発を止め、
Logging::Logger::set_innerでのApp::operator<<の[name lookup](term_explanation.md#SS_5_4_2)について考えてみることにする。

ここまでで紹介したログ取得ライブラリやそれを使うユーザ定義型等の定義、宣言の順番は、

1. Logging::Logger
2. App::X
3. App::operator<<
4. 単体テスト(Logger::set_innerのインスタンス化される場所)

となっている。
name lookupの原則に従い、App::Xの宣言は、App::operator<<より前に行われている。
これを逆にするとコンパイルできない。
しかし、Logging::Loggerは、後から宣言されたApp::operator<<を使うことができる。
多くのプログラマは、これについて気づいていないか、その理由を間違っての認識している。

その認識とは、「テンプレート内の識別子の[name lookup](term_explanation.md#SS_5_4_2)は、
それがインスタンス化される時に行われる」というものであり、これにより
「 Logging::Loggerのname lookupは単体テスト内で行われる。
それはApp::operator<<宣言後であるためコンパイルできる」と考えることができるが、
[two phase name lookup](term_explanation.md#SS_5_4_3)で行われるプロセスと反するため誤りである。

まずは、この認識の誤りを下記のコードで説明する。

```cpp
    // @@@ example/template/logger_0_ut.cpp 68

    namespace App2 {
    class X {
    public:
        explicit X(std::string str, int num) : str_{std::move(str)}, num_{num} {}
        std::string ToString() const { return str_ + "/" + std::to_string(num_); }
        ...
    };
    }  // namespace App2

    namespace App3 {  // App3をApp2にすればコンパイルできる
    std::ostream& operator<<(std::ostream& os, App2::X const& x) { return os << x.ToString(); }
    }  // namespace App3

    namespace {

    TEST(Template, logger_0_X_in_AppX)
    {
        Logging::Logger::Inst().Clear();

        auto x = App2::X{"name", 3};

        using namespace App3;  // この記述は下記のエラーに効果がない

        LOGGER(1, x);  // ここがコンパイルエラーとなる
        auto line_num = __LINE__;

        auto s = Logging::Logger::InstConst().Get();

        auto exp = std::string{log_str_exp(__FILE__, line_num - 1, ":1:name/3\n")};
        ASSERT_EQ(exp, s);
    }
    }  // namespace
```

このコードは、もともとのコードの名前空間名をApp2とApp3にしただけのものである。
もし、前記した「認識」の内容が正しいのであれば、このコードもコンパイルできるはずであるが
(実際にApp3と書いた部分をApp2に書き換えればコンパイルできる)、
実際には下記のようなエラーが発生する。


```
    logger_0.h:37:21: error: no match for ‘operator<<’ 
    (operand types are ‘std::basic_ostream<char>’ and ‘const App2::X’)
       37 |         oss_ << ":" << head;
          |         ~~~~~~~~~~~~^~~~~~~

```

エラー内容からoperator<<が発見できないことは明らかである。
単体テスト内でのusing namespace App3はLogging::Logger::set_innerの宣言より後に書かれているため、
このエラーを防ぐ効果はない。

Logging::Logger::set_innerの中でusing namespace App3とした上で、
two phase name lookupの原則に従い、
App2::XとApp3::operator<<をLogging::Loggerの宣言より前に宣言することで、
ようやくコンパイルすることができる。

名前空間Appの例と名前空間App2、App3の例での本質的な違いは、
「型Xとそのoperator<<が同じ名前空間で宣言されているかどうか」である。

名前空間Appの例の場合、型Xとそのoperator<<が同じ名前空間で宣言されているため、
[ADL](term_explanation.md#SS_5_4_5)(実引数依存探索)が働く。
また、Logging::Logger::set_inner(x)はテンプレートであるため、[two phase name lookup](term_explanation.md#SS_5_4_3)
が使用される。
その結果、Logging::Logger::set_inner(x)でのname lookupの対象には、
「Logging::Logger::set_inner(x)がインスタンス化される場所
(単体テスト内でのLOGGER_PやLOGGERが使われている場所)より前方で宣言された名前空間App」も含まれる。
こういったメカニズムにより、
Logging::Logger::set_inner定義位置の後方で宣言されたApp::operator<<も発見できることになる。

一方で、名前空間App2、App3の例では、型XがApp2で宣言されているため、
Logging::Logger::set_inner(x)でのname lookupの対象にApp3は含ず、
App3::operator<<は発見されない(繰り返すが、インスタン化の場所直前でのusing nameには効果がない)。

型Xとそのoperator<<を同じ名前空間で宣言することは本質的に重要なことであるが、
名前空間を使用する場合、自然にそのような構造になるため、
その重要性の理由を知る必要はないように思われる。
しかし、次の例で示すようにこのメカニズムを知らずに解決することができないケースが存在する。

### Ints_tのログ登録 <a id="SS_3_1_6"></a>
話題はログ取得ライブラリの開発に戻る。
アプリケーションの開発では、下記のように宣言された型エイリアスを使うことは珍しくない。

```cpp
    // @@@ example/template/app_ints.h 6

    namespace App {
    using Ints_t = std::vector<int>;
    }
```

そのoperator<<を下記のように定義したとする。

```cpp
    // @@@ example/template/logger_0_ut.cpp 109

    namespace App {
    std::ostream& operator<<(std::ostream& os, Ints_t const& ints)
    {
        auto first = true;

        for (auto const i : ints) {
            if (!std::exchange(first, false)) {
                os << ", ";
            }
            os << i;
        }

        return os;
    }
    }  // namespace App
```

単体テストは下記のように書けるが、残念ながらコンパイルエラーになり、

```cpp
    // @@@ example/template/logger_0_ut.cpp 134

    auto ints = App::Ints_t{1, 2, 3};

    auto oss = std::ostringstream{};

    oss << ints;
    ASSERT_EQ("1, 2, 3", oss.str());
```

下記のようなエラーメッセージが表示される。

```
    logger_0_ut.cpp:140:9: error: no match for ‘operator<<’
        (operand types are ‘std::ostringstream’ {aka ‘std::basic_ostringstream<char>’}
         and ‘App::Ints_t’ {aka ‘std::vector<int>’})
      140 |     oss << ints;
          |     ~~~ ^~ ~~~~
          |     |      |
          |     |      App::Ints_t {aka std::vector<int>}
          |     std::ostringstream {aka std::basic_ostringstream<char>}
```

Ints_tはAppで定義されているが、実際の型はstdで定義されているため、
instsの[関連名前空間](term_explanation.md#SS_5_4_6)もstdであり、Appではない。
その結果App::operator<<は発見できず、このようなエラーになった。

LOGGERからApp::operator<<を使う場合の単体テストは下記のようになるが、
[ADL](term_explanation.md#SS_5_4_5)によってLogging::Logger::set_inner(ints)内に導入される名前空間はstdのみであり、
前記単体テスト同様にコンパイルできない。

```cpp
    // @@@ example/template/logger_0_ints_ut.h 8

    auto ints = App::Ints_t{1, 2, 3};

    LOGGER("Ints", ints);
    auto line_num = __LINE__;

    auto s = Logging::Logger::InstConst().Get();

    auto exp = std::string{log_str_exp(__FILE__, line_num - 1, ":Ints:1, 2, 3\n")};
    ASSERT_EQ(exp, s);
```

この解決方法は、

* [operator\<\<をstd内で宣言する](template_meta_programming.md#SS_3_1_6_1)
* [operator\<\<をグローバル名前空間内で宣言する](template_meta_programming.md#SS_3_1_6_2)
* [operator\<\<をLogging内で宣言する](template_meta_programming.md#SS_3_1_6_3)
* [Logging::Logger::set_inner(ints)内でusing namespace Appを行う](template_meta_programming.md#SS_3_1_6_4)
* [Ints_tを構造体としてApp内に宣言する](template_meta_programming.md#SS_3_1_6_5)
* [operator\<\<を使わない](template_meta_programming.md#SS_3_1_6_6)

のようにいくつか考えられる。以下では、順を追ってこれらの問題点について解説を行う。


#### operator\<\<をstd内で宣言する <a id="SS_3_1_6_1"></a>
ここで解決したい問題は、すでに示した通り、
「[ADL](term_explanation.md#SS_5_4_5)によってLogging::Logger::set_inner(ints)内に導入される名前空間はstdである」
ことにって発生する。であれば、App内でのoperator<<の宣言をstdで行えばコンパイルできるはずである。
下記はその変更を行ったコードである。

```cpp
    // @@@ example/template/app_ints.h 6

    namespace App {
    using Ints_t = std::vector<int>;
    }
```
```cpp
    // @@@ example/template/logger_0_std_ut.cpp 11

    namespace std {  // operator<<の定義をstdで行う
    std::ostream& operator<<(std::ostream& os, App::Ints_t const& ints)
    {
        auto first = true;

        for (auto const i : ints) {
            if (!std::exchange(first, false)) {
                os << ", ";
            }
            os << i;
        }

        return os;
    }
    }  // namespace std
```

上記コードは[two phase name lookup](term_explanation.md#SS_5_4_3)等の効果により、想定通りコンパイルできるが、
stdをユーザが拡張することは一部の例外を除き未定義動作を引き起こす可能性があり、
たとえこのコードがうまく動作したとしても
(実際、このコードはこのドキュメント作成時には正常動作している)、
未来においてその保証はなく、このようなプログラミングは厳に避けるべきである。


#### operator\<\<をグローバル名前空間内で宣言する <a id="SS_3_1_6_2"></a>
すでに述べた通り、
「ADLによってLogging::Logger::set_inner(ints)内に導入される名前空間はstdのみである」ため、
この関数の中でのname lookupに使用される名前空間は、std、グローバル名前空間、
Loggerを宣言しているLoggingの3つである。

ここでは、下記のコードのようにグローバル名前空間内でのoperator<<の宣言を試す。

```cpp
    // @@@ example/template/app_ints.h 6

    namespace App {
    using Ints_t = std::vector<int>;
    }
```
```cpp
    // @@@ example/template/logger_0_global_ut.cpp 10

    // グローバル名前空間
    std::ostream& operator<<(std::ostream& os, App::Ints_t const& ints)
    {
        auto first = true;

        for (auto const i : ints) {
            if (!std::exchange(first, false)) {
                os << ", ";
            }
            os << i;
        }

        return os;
    }
```

このドキュメントで使用している[g++](term_explanation.md#SS_5_9_1)ではこのコードはコンパイルでき、
動作も問題ないように思われるが、[clang++](term_explanation.md#SS_5_9_2)では以下のようなエラーが発生し、コンパイルできない。

```
    ./logger_0.h:37:21: error: call to function 'operator<<' that is neither 
    visible in the tem plate definition nor found by argument-dependent lookup
            oss_ << ":" << head;
```

この理由は「[two phase name lookup](term_explanation.md#SS_5_4_3)」の後半で詳しく解説したので、ここでは繰り返さないが、
このようなコードを使うと、コード解析ツール等が使用できなくなることがあるため、
避けるべきである
。

多くのプログラマは、コードに問題があるとしても、
それが意図通りに動くように見えるのであればその問題を無視する。
今回のような難題に対しては、なおさらそのような邪悪な欲求に負けやすい。
そのような観点でclang++が吐き出したエラーメッセージを眺めると、上記したメッセージの後に、
下記のような出力を見つけるかもしれない。

```
    logger_0_global_ut.cpp:13:15: note: 'operator<<' should be declared prior to the call site
    std::ostream& operator<<(std::ostream& os, App::Ints_t const& ints)
```

clang++は「LOGGERの前にoperator<<を宣言せよ」と言っている。
実際そうすれば、clang++でのコンパイルも通り、単体テストもパスする。
しかし、それには下記のような問題がある。

* operator<<(std::ostream& os, App::Ints_t const& ints)
  という名前空間Appローカルな宣言をグローバル名前空間で行うことによって、
  グローバル名前空間を汚染してしまう
  (このコードは名前空間を正しく使うことに対しての割れ窓
  (「[割れ窓理論](term_explanation.md#SS_5_12_1)」参照)になってしまうかもしれない)。
* 例示したコードでのoperator<<(std::ostream& os, App::Ints_t const& ints)の定義は、
  単体テストファイル内にあったが、実際には何らかのヘッダファイル内で定義されることになる。
  その場合、ロガーのヘッダファイルよりも、
  そのヘッダファイルを先にインクルードしなければならなくなる。
  これは大した問題ではないように見えるが、
  ヘッダファイル間の暗黙の依存関係を生み出し将来の保守作業を難しくさせる。


以上述べた理由からこのアイデアを選択するべきではない。


#### operator\<\<をLogging内で宣言する <a id="SS_3_1_6_3"></a>
前節でのグローバル名前空間内でのoperator<<の宣言はうまく行かなかったので、
同様のことをLoggingで試す。

```cpp
    // @@@ example/template/app_ints.h 6

    namespace App {
    using Ints_t = std::vector<int>;
    }
```
```cpp
    // @@@ example/template/logger_0_logging_ut.cpp 10

    namespace Logging {  // operator<<の定義をLoggingで行う
    std::ostream& operator<<(std::ostream& os, App::Ints_t const& ints)
    {
        auto first = true;

        for (auto const i : ints) {
            if (!std::exchange(first, false)) {
                os << ", ";
            }
            os << i;
        }

        return os;
    }
    }  // namespace Logging
```

動作はするものの、当然ながら結果は「[operator\<\<をグローバル名前空間内で宣言する](template_meta_programming.md#SS_3_1_6_2)」
で述べた状況とほぼ同様であるため、このアイデアを採用することはできない。

#### Logging::Logger::set_inner(ints)内でusing namespace Appを行う <a id="SS_3_1_6_4"></a>
Logging::Logger::set_inner(ints)内でusing namespace Appを行えば、
意図通りに動作させることができるが、App内のロギングは名前空間Loggingに依存するため、
AppとLoggingが循環した依存関係を持ってしまう。
また、LoggingはAppに対して上位概念であるため、[依存関係逆転の原則(DIP)](solid.md#SS_1_5)にも反する。
よって、このアイデアを採用することはできない。

#### Ints_tを構造体としてApp内に宣言する <a id="SS_3_1_6_5"></a>

App::Ints_t用のoperator<<がLogging::Logger::set_inner内でname lookup出来ない理由は、
これまで述べてきたようにApp::Inst_tの[関連名前空間](term_explanation.md#SS_5_4_6)がAppではなく、stdになってしまうからである。

これを回避するためにはその原因を取り払えばよく、
つまり、App::Inst_tの[関連名前空間](term_explanation.md#SS_5_4_6)がAppになるようにすればよい。
これを実現するために、次のコードを試してみる。

```cpp
    // @@@ example/template/logger_0_struct_ut.cpp 10

    namespace App {                     // Ints_tの宣言はApp
    struct Ints_t : std::vector<int> {  // エイリアスではなく、継承を使う
        using vector::vector;           // 継承コンストラクタ
    };

    // App内
    std::ostream& operator<<(std::ostream& os, Ints_t const& ints)
    {
        auto first = true;

        for (auto const i : ints) {
            if (!std::exchange(first, false)) {
                os << ", ";
            }
            os << i;
        }

        return os;
    }
    }  // namespace App
```

上記のコードでは、

* App::Ints_tをstd::vectorからpublic継承
* using宣言によりstd::vectorのすべてのコンストラクタをApp::Ints_tに導入
  (「[継承コンストラクタ](term_explanation.md#SS_5_2_3)」参照)

としているため、エイリアスで宣言されたInts_tと等価である。
C++03では、[継承コンストラクタ](term_explanation.md#SS_5_2_3)が使えなかったため、
上記のような構造体を定義するためには、
std::vectorのすべてのコンストラクタと等価なコンストラクタをApp::Ints_t内に定義することが必要で、
実践的にはこのようなアイデアは使い物にならなかったが、
C++11での改善により、実践的なアイデアとして使用できるようになった。

実際、名前空間の問題もなく、すでに示した単体テストもパスするので有力な候補となるが、
若干の「やりすぎ感」は否めない。


#### operator\<\<を使わない <a id="SS_3_1_6_6"></a>
色々なアイデアを試してみたが、これまでの議論ではこれといった解決方法を発見できなかった。
「[バーニーの祈り](https://ja.wikipedia.org/wiki/%E3%83%8B%E3%83%BC%E3%83%90%E3%83%BC%E3%81%AE%E7%A5%88%E3%82%8A)」
が言っている通り、時にはどうにもならないことを受け入れることも重要である。
LOGGERの中でname lookupできる、エイリアスApp::Ints_tのoperator<<の開発をあきらめ、
ここでは一旦、下記のような受け入れがたいコードを受け入れることにする。


```cpp
    // @@@ example/template/app_ints.h 6

    namespace App {
    using Ints_t = std::vector<int>;
    }
```
```cpp
    // @@@ example/template/logger_0_no_put_to_ut.cpp 10

    namespace App {  // App::Ints_tのoperator<<とToStringをApp内で定義
    namespace {      // operator<<は外部から使わない
    std::ostream& operator<<(std::ostream& os, Ints_t const& ints)
    {
        auto first = true;

        for (auto const i : ints) {
            if (!std::exchange(first, false)) {
                os << ", ";
            }
            os << i;
        }

        return os;
    }
    }  // namespace

    // Ints_tオブジェクトをstd::stringに変換する
    // この変換によりロガーに渡すことができる
    std::string ToString(Ints_t const& inst)
    {
        auto oss = std::ostringstream{};

        oss << inst;

        return oss.str();
    }
    }  // namespace App
```

当然だが、恥を忍んで受け入れたコードにも単体テストは必要である。

```cpp
    // @@@ example/template/logger_0_no_put_to_ut.cpp 47

    auto ints = App::Ints_t{1, 2, 3};

    // ToStringのテスト
    ASSERT_EQ("1, 2, 3", App::ToString(ints));

    // LOGGERのテスト
    LOGGER("Ints", App::ToString(ints));
    auto line_num = __LINE__;

    auto s = Logging::Logger::InstConst().Get();

    auto exp = std::string{log_str_exp(__FILE__, line_num - 1, ":Ints:1, 2, 3\n")};
    ASSERT_EQ(exp, s);
```

上記コードから明らかな通り、App::Ints_tのインスタンスをログ登録する場合、
App::ToString()によりstd::stringへ変換する必要があり、残念なインターフェースとなっている。


#### Ints_tのログ登録のまとめ <a id="SS_3_1_6_7"></a>
製品開発では、満足できる仕様の関数やクラスが作れず、妥協せざるを得ないことはよくあることである。
このような場合、将来、良いアイデアが見つかった時に備えて、
妥協コードを簡単に修正できるようなレベルにした後、捲土重来を期してさっさと退却するのがベストである。
ただし、漫然と過ごしても良いアイデアは浮かばない。
時間を作り、関連書籍やウェブドキュメント等を読み、学習を継続する必要があることは言うまでもない。


## Nstdライブラリの開発 <a id="SS_3_2"></a>

「[operator\<\<を使わない](template_meta_programming.md#SS_3_1_6_6)」で導入したコードは、短いながらも汎用性が高い。
このようなコードをローカルなファイルに閉じ込めてしまうと、
コードクローンや、[車輪の再発明](term_explanation.md#SS_5_12_2)による開発効率の低下につながることがある。

通常、プロジェクトの全ファイルから参照可能で且つ、
プロジェクトの他のパッケージに非依存なパッケージを用意することで、このような問題を回避できる。

ここでは、そのようなパッケージをNstd(not standard library)とし、名前空間も同様に宣言する。
そうした場合、この章の例題で使用している名前空間の依存関係は下記のようになる。

<p><img src="data:image/png;base64,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" /></p>

このように整理された依存関係は、
大規模ソフトウェア開発においては特に重要であり、決して循環しないように維持しなければならない。


### Nstdライブラリを使用したリファクタリング <a id="SS_3_2_1"></a>
すでに述べた通り、「[operator\<\<を使わない](template_meta_programming.md#SS_3_1_6_6)」で導入したコードは、Nstdで定義するべきである。
その場合、下記のようにさらに一般化するのが良いだろう。

```cpp
    // @@@ example/template/nstd_0.h 4

    namespace Nstd {

    template <typename T>
    std::ostream& operator<<(std::ostream& os, std::vector<T> const& vec)
    {
        auto first = true;

        for (auto const& i : vec) {
            if (!std::exchange(first, false)) {
                os << ", ";
            }
            os << i;
        }

        return os;
    }

    template <typename T>
    std::string ToString(std::vector<T> const& vec)
    {
        auto oss = std::ostringstream{};

        oss << vec;

        return oss.str();
    }
    }  // namespace Nstd
```

その単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_0_ut.cpp 13

    auto const ints = App::Ints_t{1, 2, 3};

    {
        auto oss = std::ostringstream{};

        using namespace Nstd;
        oss << ints << 4;
        ASSERT_EQ("1, 2, 34", oss.str());
    }
    {
        auto oss = std::ostringstream{};

        Nstd::operator<<(oss, ints) << 4;  // 念のためこの形式でもテスト
        ASSERT_EQ("1, 2, 34", oss.str());
    }

    ASSERT_EQ("1, 2, 3", Nstd::ToString(ints));
```

勘のいい読者なら、このコードをLOGGERから利用することで、
App:Ints_tのログ登録問題を解消できると思うかもしれない。
実際その通りなのであるが、そうした場合、
std::list等の他のコンテナや配列には対応できないという問題が残るため、
以降もしばらくNstdの開発を続ける。


### 安全なvector <a id="SS_3_2_2"></a>
std::vector、std::basic_string、std::array等の配列型コンテナは、

* operator[]経由でのメンバアクセスについて範囲の妥当性をチェックしない
* 範囲のチェックが必要ならばat()を使用する

という仕様になっているが、
ここではoperator[]にも範囲のチェックを行う配列型コンテナが必要になった場合について考える。

手始めにoperator[]にも範囲のチェックを行うstd::vector相当のコンテナSafeVectorを作ると、
下記のコードのようになる。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 9

namespace Nstd {

template <typename T>
struct SafeVector : std::vector<T> {
    using std::vector<T>::vector;  // 継承コンストラクタ

    using base_type = std::vector<T>;
    using size_type = typename base_type::size_type;

    typename base_type::reference       operator[](size_type i) { return this->at(i); }
    typename base_type::const_reference operator[](size_type i) const { return this->at(i); }
};
}  // namespace Nstd
```

このコードで行ったことは、

* std::vectorからSafeVectorをpublic継承する
* 継承コンストラクタの機能を使い、std::vectorのコンストラクタをSafeVectorで宣言する
* std::vector::atを使い、SafeVector::operator[]を定義する

である。単体テストは下記のようになる。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 29

    {
        auto v = Nstd::SafeVector<int>(10);  // ()での初期化

        ASSERT_EQ(10, v.size());
    }
    {
        auto const v = Nstd::SafeVector<int>{10};

        ASSERT_EQ(1, v.size());
        ASSERT_EQ(10, v[0]);
        ASSERT_THROW(v[1], std::out_of_range);  // エクセプションの発生
    }
    {
        auto v = Nstd::SafeVector<std::string>{"1", "2", "3"};

        ASSERT_EQ(3, v.size());
        ASSERT_EQ((std::vector<std::string>{"1", "2", "3"}), v);
        ASSERT_THROW(v[3], std::out_of_range);  // エクセプションの発生
    }
    {
        auto const v = Nstd::SafeVector<std::string>{"1", "2", "3"};

        ASSERT_EQ(3, v.size());
        ASSERT_EQ((std::vector<std::string>{"1", "2", "3"}), v);
        ASSERT_THROW(v[3], std::out_of_range);  // エクセプションの発生
    }
```


### 安全な配列型コンテナ <a id="SS_3_2_3"></a>
配列型コンテナはすでに述べたようにstd::vectorの他にすくなともstd::basic_string、
std::arrayがあるため、それらにも範囲チェックを導入する。

std::basic_stringはstd::vectorとほぼ同様に下記のようになる。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 61
    namespace Nstd {

    struct SafeString : std::string {
        using std::string::string;  // 継承コンストラクタ

        using base_type = std::string;
        using size_type = typename base_type::size_type;

        typename base_type::reference       operator[](size_type i) { return this->at(i); }
        typename base_type::const_reference operator[](size_type i) const { return this->at(i); }
    };
    }  // namespace Nstd
```

std::stringはstd::basic_string\<char>のエイリアスであるため、
上記では、通常使われる形式であるstd::stringを継承したSafeStringを定義した。

この単体テストはSafeVectorの場合と同様に下記のようになる。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 79

    {
        auto s = Nstd::SafeString{"0123456789"};

        ASSERT_EQ(10, s.size());
        ASSERT_EQ("0123456789", s);
        ASSERT_THROW(s[10], std::out_of_range);
    }
    {
        auto const s = Nstd::SafeString(3, 'c');  // ()での初期化が必要

        ASSERT_EQ(3, s.size());
        ASSERT_EQ("ccc", s);
    }
```

std::arrayでは少々事情が異なるが、
std::vectorのコードパターンをそのまま適用すると下記のようになる。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 100

    namespace Nstd {

    template <typename T, size_t N>
    struct SafeArray : std::array<T, N> {
        using std::array<T, N>::array;  // 継承コンストラクタ

        using base_type = std::array<T, N>;
        using size_type = typename base_type::size_type;

        typename base_type::reference       operator[](size_type i) { return this->at(i); }
        typename base_type::const_reference operator[](size_type i) const { return this->at(i); }
    };
    }  // namespace Nstd
```

ただし、この実装には問題がある。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 121

    auto sa_not_init = Nstd::SafeArray<int, 3>{};

    ASSERT_EQ(3, sa_not_init.size());
    ASSERT_THROW(sa_not_init[3], std::out_of_range);
```

上記コードでは、その問題が露見することはないが、以下のコードはコンパイルできない。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 131

    // std::initializer_listを引数とするコンストラクタが未定義
    auto sa_init = Nstd::SafeArray<int, 3>{1, 2, 3};

    // デフォルトコンストラクタがないため、未初期化
    Nstd::SafeArray<int, 3> const sa_const;
```

std::arrayにはコンストラクタが明示的に定義されていないため、
std::arrayにはデフォルトで自動生成される

* デフォルトコンストラクタ
* copyコンストラクタ
* moveコンストラクタ

以外のコンストラクタがないことが原因である。
従って、SafeArray(std::initializer_list)が定義されず前述したようにコンパイルエラーとなる。

この問題に対処したのが以下のコードである。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 145

    namespace Nstd {

    template <typename T, size_t N>
    struct SafeArray : std::array<T, N> {
        using std::array<T, N>::array;  // 継承コンストラクタ
        using base_type = std::array<T, N>;

        template <typename... ARGS>  // コンストラクタを定義
        SafeArray(ARGS... args) : base_type{args...}
        {
        }

        using size_type = typename base_type::size_type;

        typename base_type::reference       operator[](size_type i) { return this->at(i); }
        typename base_type::const_reference operator[](size_type i) const { return this->at(i); }
    };
    }  // namespace Nstd
```

上記コードで注目すべきは、パラメータパックをテンプレートパラメータとしたコンストラクタである。
これにより、前例ではコンパイルすらできなかった下記のような初期化子リストを用いた単体テストが、
このコンストラクタによりパスするようになった。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 180
    {
        auto sa_init = Nstd::SafeArray<int, 3>{1, 2, 3};

        ASSERT_EQ(3, sa_init.size());
        ASSERT_EQ(1, sa_init[0]);
        ASSERT_EQ(2, sa_init[1]);
        ASSERT_EQ(3, sa_init[2]);
        ASSERT_THROW(sa_init[3], std::out_of_range);
    }
    {
        auto const sa_string_const = Nstd::SafeArray<std::string, 5>{"1", "2", "3"};

        ASSERT_EQ(5, sa_string_const.size());
        ASSERT_EQ("1", sa_string_const[0]);
        ASSERT_EQ("2", sa_string_const[1]);
        ASSERT_EQ("3", sa_string_const[2]);
        ASSERT_EQ("", sa_string_const[3]);
        ASSERT_EQ("", sa_string_const[4]);
        ASSERT_THROW(sa_string_const[5], std::out_of_range);
    }
```

この効果を生み出した上記を抜粋した下記のコードには解説が必要だろう。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 154

    template <typename... ARGS>  // コンストラクタを定義
    SafeArray(ARGS... args) : base_type{args...}
    {
    }
```

一般にコンストラクタには「メンバ変数の初期化」と「基底クラスの初期化」が求められるが、
SafeArrayにはメンバ変数が存在しないため、
このコンストラクタの役割は「基底クラスの初期化」のみとなる。
基底クラスstd::array(上記例ではbase_typeにエイリアスしている)
には名前が非規定の配列メンバのみを持つため、
これを初期化するためには初期化子リスト
(「[初期化子リストコンストラクタ](term_explanation.md#SS_5_2_2)」、
「[一様初期化](term_explanation.md#SS_5_2_7)」参照)を用いるのが良い。

ということは、SafeArrayの初期化子リストコンストラクタには、
「基底クラスstd::arrayに初期子リストを与えて初期化する」形式が必要になる。
値を持つパラメータパックは初期化子リストに展開できるため、
ここで必要な形式はパラメータパックとなる。
これを実現したのが上記に抜粋したわずか数行のコードである。


### 初期化子リストの副作用 <a id="SS_3_2_4"></a>
上記SafeArrayの初期化子リストコンストラクタは以下のようなコードを許可しない。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 212
    {
        auto sa_init = Nstd::SafeArray<int, 3>{1.0, 2, 3};

        ASSERT_EQ(3, sa_init.size());
        ASSERT_EQ(1, sa_init[0]);
        ASSERT_EQ(2, sa_init[1]);
        ASSERT_EQ(3, sa_init[2]);
        ASSERT_THROW(sa_init[3], std::out_of_range);
    }
```

このコードをコンパイルすると、

```
    safe_vector_ut.cpp:147:41: error: narrowing conversion of ‘
                                        args#0’ from ‘double’ to ‘int’ -Werror=narrowing]
      147 |     SafeArray(ARGS... args) : base_type{args...}
          |                                         ^~~~
```

のようなエラーが出力されるが、

* double(上記例では1.0)をintに変換する際に縮小変換(narrowing conversio)nが起こる
* 初期化子リストでの縮小変換は許可されない

が原因である。これは意図しない縮小変換によるバグを防ぐ良い機能だと思うが、
ここではテンプレートメタプログラミングのテクニックを解説するため、
あえてこのコンパイルエラーを起こさないSafeArray2を開発する
(言うまでもないが、通常のソフトウェア開発では、
縮小変換によるコンパイルエラーを回避するようなコードを書いてはならない)。

SafeArray2のコードは、

* STLのtype_traitsの使用
* テンプレートの特殊化
* メンバ関数テンプレートとオーバーロードによる静的ディスパッチ(コンパイル時ディスパッチ)
* [SFINAE](term_explanation.md#SS_5_4_7)

等のメタ関数系のテクニックが必要になるため、
まずはこれらを含めたテンプレートのテクニックについて解説し、
その後SafeArray2を見ていくことにする。

## メタ関数のテクニック <a id="SS_3_3"></a>
本章で扱うメタ関数とは、型、定数、クラステンプレート等からなるテンプレート引数から、
型、エイリアス、定数等を宣言、定義するようなクラステンプレート、関数テンプレート、
定数テンプレート、エイリアステンプレートを指す
(本章ではこれらをまとめて単にテンプレート呼ぶことがある)。


### STLのtype_traits <a id="SS_3_3_1"></a>
メタ関数ライブラリの代表的実装例はSTLの
[type_traits](https://cpprefjp.github.io/reference/type_traits.html)である。

ここでは、よく使ういくつかのtype_traitsテンプレートの使用例や解説を示す。


#### std::true_type/std::false_type <a id="SS_3_3_1_1"></a>
std::true_type/std::false_typeは真/偽を返すSTLメタ関数群の戻り型となる型エイリアスであるため、
最も使われるテンプレートの一つである。

これらは、下記で確かめられる通り、後述する[std::integral_constant](template_meta_programming.md#SS_3_3_1_2)を使い定義されている。

```cpp
    // @@@ example/template/type_traits_ut.cpp 13

    // std::is_same_vの2パラメータが同一であれば、std::is_same_v<> == true
    static_assert(std::is_same_v<std::integral_constant<bool, true>, std::true_type>);
    static_assert(std::is_same_v<std::integral_constant<bool, false>, std::false_type>);
```

それぞれの型が持つvalue定数は、下記のように定義されている。

```cpp
    // @@@ example/template/type_traits_ut.cpp 20

    static_assert(std::true_type::value, "must be true");
    static_assert(!std::false_type::value, "must be false");
```

これらが何の役に立つのか直ちに理解することは難しいが、
true/falseのメタ関数版と考えれば、追々理解できるだろう。

以下に簡単な使用例を示す。

```cpp
    // @@@ example/template/type_traits_ut.cpp 29

    // 引数の型がintに変換できるかどうかを判定する関数
    // decltypeの中でのみ使用されるため、定義は不要
    constexpr std::true_type  IsCovertibleToInt(int);  // intに変換できる型はこちら
    constexpr std::false_type IsCovertibleToInt(...);  // それ以外はこちら
```

上記の単体テストは下記のようになる。

```cpp
    // @@@ example/template/type_traits_ut.cpp 40

    static_assert(decltype(IsCovertibleToInt(1))::value);
    static_assert(decltype(IsCovertibleToInt(1u))::value);
    static_assert(!decltype(IsCovertibleToInt(""))::value);  // ポインタはintに変換不可

    struct ConvertibleToInt {
        operator int();
    };

    struct NotConvertibleToInt {};

    static_assert(decltype(IsCovertibleToInt(ConvertibleToInt{}))::value);
    static_assert(!decltype(IsCovertibleToInt(NotConvertibleToInt{}))::value);

    // なお、IsCovertibleToInt()やConvertibleToInt::operator int()は実際に呼び出されるわけでは
    // ないため、定義は必要なく宣言のみがあれば良い。
```

IsCovertibleToIntの呼び出しをdecltypeのオペランドにすることで、
std::true_typeかstd::false_typeを受け取ることができる。

#### std::integral_constant <a id="SS_3_3_1_2"></a>
std::integral_constantは
「テンプレートパラメータとして与えられた型とその定数から新たな型を定義する」
クラステンプレートである。

以下に簡単な使用例を示す。

```cpp
    // @@@ example/template/type_traits_ut.cpp 62

    using int3 = std::integral_constant<int, 3>;

    // std::is_same_vの2パラメータが同一であれば、std::is_same_v<> == true
    static_assert(std::is_same_v<int, int3::value_type>);
    static_assert(std::is_same_v<std::integral_constant<int, 3>, int3::type>);
    static_assert(int3::value == 3);

    using bool_true = std::integral_constant<bool, true>;

    static_assert(std::is_same_v<bool, bool_true::value_type>);
    static_assert(std::is_same_v<std::integral_constant<bool, true>, bool_true::type>);
    static_assert(bool_true::value == true);
```

また、すでに示したようにstd::true_type/std::false_typeを実装するためのクラステンプレートでもある。

#### std::is_same <a id="SS_3_3_1_3"></a>

すでに上記の例でも使用したが、std::is_sameは2つのテンプレートパラメータが

* 同じ型である場合、std::true_type
* 違う型である場合、std::false_type

から派生した型となる。

以下に簡単な使用例を示す。

```cpp
    // @@@ example/template/type_traits_ut.cpp 99

    static_assert(std::is_same<int, int>::value);
    static_assert(std::is_same<int, int32_t>::value);   // 64ビットg++/clang++
    static_assert(!std::is_same<int, int64_t>::value);  // 64ビットg++/clang++
    static_assert(std::is_same<std::string, std::basic_string<char>>::value);
    static_assert(std::is_same<typename std::vector<int>::reference, int&>::value);
```

また、 C++17で導入されたstd::is_same_vは、定数テンプレートを使用し、
下記のように定義されている。

```cpp
    // @@@ example/template/type_traits_ut.cpp 90

    template <typename T, typename U>
    constexpr bool is_same_v{std::is_same<T, U>::value};
```

```cpp
    // @@@ example/template/type_traits_ut.cpp 108

    static_assert(is_same_v<int, int>);
    static_assert(is_same_v<int, int32_t>);   // 64ビットg++/clang++
    static_assert(!is_same_v<int, int64_t>);  // 64ビットg++/clang++
    static_assert(is_same_v<std::string, std::basic_string<char>>);
    static_assert(is_same_v<typename std::vector<int>::reference, int&>);
```

このような簡潔な記述の一般形式は、

```
   T::value  -> T_v
   T::type   -> T_t
```

のように定義されている(このドキュメントのほとんど場所では、簡潔な形式を用いる)。

第1テンプレートパラメータが第2テンプレートパラメータの基底クラスかどうかを判断する
std::is_base_ofを使うことで下記のようにstd::is_sameの基底クラス確認することもできる。

```cpp
    // @@@ example/template/type_traits_ut.cpp 117

    static_assert(std::is_base_of_v<std::true_type, std::is_same<int, int>>);
    static_assert(std::is_base_of_v<std::false_type, std::is_same<int, char>>);
```


#### std::enable_if <a id="SS_3_3_1_4"></a>
std::enable_ifは、bool値である第1テンプレートパラメータが

* trueである場合、型である第2テンプレートパラメータをメンバ型typeとして宣言する。
* falseである場合、メンバ型typeを持たない。

下記のコードはクラステンプレートの特殊化を用いたstd::enable_ifの実装例である。

```cpp
    // @@@ example/template/type_traits_ut.cpp 124

    template <bool T_F, typename T = void>
    struct enable_if;

    template <typename T>
    struct enable_if<true, T> {
        using type = T;
    };

    template <typename T>
    struct enable_if<false, T> {  // メンバエイリアスtypeを持たない
    };

    template <bool COND, typename T = void>
    using enable_if_t = typename enable_if<COND, T>::type;
```

std::enable_ifの使用例を下記に示す。

```cpp
    // @@@ example/template/type_traits_ut.cpp 148

    static_assert(std::is_same_v<void, std::enable_if_t<true>>);
    static_assert(std::is_same_v<int, std::enable_if_t<true, int>>);
```

実装例から明らかなように

* std::enable_if\<true>::typeは[well-formed](term_explanation.md#SS_5_10_9)
* std::enable_if\<false>::typeは[ill-formed](term_explanation.md#SS_5_10_8)

となるため、下記のコードはコンパイルできない。

```cpp
    // @@@ example/template/type_traits_ut.cpp 155

    // 下記はill-formedとなるため、コンパイルできない。
    static_assert(std::is_same_v<void, std::enable_if_t<false>>);
    static_assert(std::is_same_v<int, std::enable_if_t<false, int>>);
```

std::enable_ifのこの特性と後述する[SFINAE](term_explanation.md#SS_5_4_7)により、
様々な静的ディスパッチを行うことができる。


#### std::conditional <a id="SS_3_3_1_5"></a>

std::conditionalは、bool値である第1テンプレートパラメータが

* trueである場合、第2テンプレートパラメータ
* falseである場合、第3テンプレートパラメータ

をメンバ型typeとして宣言する。

下記のコードはクラステンプレートの特殊化を用いたstd::conditionalの実装例である。

```cpp
    // @@@ example/template/type_traits_ut.cpp 164

    template <bool T_F, typename, typename>
    struct conditional;

    template <typename T, typename U>
    struct conditional<true, T, U> {
        using type = T;
    };

    template <typename T, typename U>
    struct conditional<false, T, U> {
        using type = U;
    };

    template <bool COND, typename T, typename U>
    using conditional_t = typename conditional<COND, T, U>::type;
```

std::conditionalの使用例を下記に示す。

```cpp
    // @@@ example/template/type_traits_ut.cpp 189

    static_assert(std::is_same_v<int, std::conditional_t<true, int, char>>);
    static_assert(std::is_same_v<char, std::conditional_t<false, int, char>>);
```

#### std::is_void <a id="SS_3_3_1_6"></a>
std::is_voidはテンプレートパラメータの型が

* voidである場合、std::true_type
* voidでない場合、std::false_type

から派生した型となる。

以下に簡単な使用例を示す。

```cpp
    // @@@ example/template/type_traits_ut.cpp 82

    static_assert(std::is_void<void>::value);
    static_assert(!std::is_void<int>::value);
    static_assert(!std::is_void<std::string>::value);
```

### is_void_xxxの実装 <a id="SS_3_3_2"></a>
ここではstd::is_voidに似た以下のような仕様を持ついくつかのテンプレートis_void_xxxの実装を考える。

|テンプレートパラメータ|戻り値            |
|----------------------|------------------|
|void                  |true              |
|非void                |false             |

それぞれのis_void_xxxは下記テーブルで示した言語機能を使用して実装する。

|is_void_xxx      |実装方法                                               |
|-----------------|-------------------------------------------------------|
|is_void_f        |関数テンプレートの特殊化                               |
|is_void_s        |クラステンプレートの特殊化                             |
|is_void_sfinae_f |SFINAEと関数テンプレートのオーバーロード               |
|is_void_sfinae_s |SFINAEとクラステンプレートの特殊化                     |
|is_void_ena_s    |std::enable_ifによるSFINAEとクラステンプレートの特殊化 |
|is_void_cond_s   |std::conditionalと関数テンプレートの特殊化             |

なお、実装例をシンプルに保つため、
理解の妨げとなり得る下記のような正確性(例外条件の対応)等のためのコードを最低限に留めた。

* テンプレートパラメータの型のチェック
* テンプレートパラメータの型からのポインタ/リファレンス/const/volatileの削除
* 戻り型からのconst/volatileの削除

これは、「テンプレートプログラミングでの有用なテクニックの解説」
というここでの目的を見失わないための措置である。


#### is_void_fの実装 <a id="SS_3_3_2_1"></a>
関数テンプレートの特殊化を使用したis_void_fの実装は以下のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp 7

    template <typename T>
    constexpr bool is_void_f() noexcept
    {
        return false;
    }

    template <>
    constexpr bool is_void_f<void>() noexcept
    {
        return true;
    }

    template <typename T>
    constexpr bool is_void_f_v{is_void_f<T>()};
```

単純なので解説は不要だろう。これらの単体テストは下記のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp 27

    static_assert(!is_void_f_v<int>);
    static_assert(!is_void_f_v<std::string>);
    static_assert(is_void_f_v<void>);
```

関数テンプレートの特殊化には、

* 特殊化された関数テンプレートとそのプライマリテンプレートのシグネチャ、
  戻り値は一致しなければならない
* クラステンプレートのような部分特殊化は許可されない

のような制限があるため用途は限られるが、関数テンプレートはオーバーロードすることが可能である。


#### is_void_sの実装 <a id="SS_3_3_2_2"></a>
クラステンプレートの特殊化を使用したis_void_sの実装は以下のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp 35

    template <typename T>
    struct is_void_s {
        static constexpr bool value{false};
    };

    template <>
    struct is_void_s<void> {
        static constexpr bool value{true};
    };

    template <typename T>
    constexpr bool is_void_s_v{is_void_s<T>::value};
```

is_void_fと同様に単純なので解説は不要だろう。これらの単体テストは下記のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp 53

    static_assert(!is_void_s_v<int>);
    static_assert(!is_void_s_v<std::string>);
    static_assert(is_void_s_v<void>);
```


#### is_void_sfinae_fの実装 <a id="SS_3_3_2_3"></a>
[SFINAE](term_explanation.md#SS_5_4_7)を使用した関数テンプレートis_void_sfinae_fの実装は以下のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp 61

    namespace Inner_ {

    // T == void
    template <typename T>
    constexpr auto is_void_sfinae_f_detector(void const* v, T const* t) noexcept
        -> decltype(t = v, bool{})  // T != voidの場合、t = vはill-formed
                                    // T == voidの場合、well-formedでbool型生成
    {
        return true;
    }

    // T != void
    template <typename T>
    constexpr auto is_void_sfinae_f_detector(void const*, T const*) noexcept
        -> decltype(sizeof(T), bool{})  // T != voidの場合、well-formedでbool型生成
                                        // T == voidの場合、sizeof(T)はill-formed
    {
        return false;
    }
    }  // namespace Inner_

    template <typename T>
    constexpr bool is_void_sfinae_f() noexcept
    {
        return Inner_::is_void_sfinae_f_detector(nullptr, static_cast<T*>(nullptr));
    }

    template <typename T>
    constexpr bool is_void_sfinae_f_v{is_void_sfinae_f<T>()};
```

関数テンプレートである2つのis_void_sfinae_f_detectorのオーバーロードにSFINAEを使用している。

1つ目のis_void_sfinae_f_detectorでは、

| T       | t = v の診断(コンパイル) |
|---------|--------------------------|
| == void | well-formed              |
| != void | ill-formed               |

であるため、Tがvoidの時のみ[name lookup](term_explanation.md#SS_5_4_2)の対象になる。

2つ目のis_void_sfinae_f_detectorでは、

| T       | sizeof(T)の診断(コンパイル) |
|---------|-----------------------------|
| == void | ill-formed                  |
| != void | well-formed                 |

であるため、Tが非voidの時のみ[name lookup](term_explanation.md#SS_5_4_2)の対象になる。

is_void_sfinae_fはこの性質を利用し、

* T == voidの場合、1つ目のis_void_sfinae_f_detectorが選択され、戻り値はtrue
* T != voidの場合、2つ目のis_void_sfinae_f_detectorが選択され、戻り値はfalse

となる。念のため単体テストを示すと下記のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp 96

    static_assert(!is_void_sfinae_f_v<int>);
    static_assert(!is_void_sfinae_f_v<std::string>);
    static_assert(is_void_sfinae_f_v<void>);
```

一般にファイル外部に公開するテンプレートは、
コンパイルの都合上ヘッダファイルにその全実装を記述することになる。
これは、本来外部公開すべきでない実装の詳細である
is_void_sfinae_f_detectorのようなテンプレートに関しては大変都合が悪い。
というのは、外部から使用されたくない実装の詳細が使われてしまうことがあり得るからである。
上記の例では、こういうことに備え
「これは外部非公開である」ということを示す名前空間Inner\_
を導入した。

関数テンプレートはクラステンプレート内にも定義することができるため、
is_void_sfinae_fは下記のように実装することも可能である。この場合、名前空間Inner\_は不要になる。

```cpp
    // @@@ example/template/is_void_ut.cpp 105

    template <typename T>
    class is_void_sfinae_f {
        // U == void
        template <typename U>
        static constexpr auto detector(void const* v, U const* u) noexcept
            -> decltype(u = v, bool{})  // U != voidの場合、t = vはill-formed
                                        // U == voidの場合、well-formedでbool型生成
        {
            return true;
        }

        // U != void
        template <typename U>
        static constexpr auto detector(void const*, U const*) noexcept
            -> decltype(sizeof(U), bool{})  // U != voidの場合、well-formedでbool型生成
                                            // U == voidの場合、ill-formed
        {
            return false;
        }

    public:
        static constexpr bool value{is_void_sfinae_f::detector(nullptr, static_cast<T*>(nullptr))};
    };

    template <typename T>
    constexpr bool is_void_sfinae_f_v{is_void_sfinae_f<T>::value};
```

```cpp
    // @@@ example/template/is_void_ut.cpp 137

    static_assert(!is_void_sfinae_f_v<int>);
    static_assert(!is_void_sfinae_f_v<std::string>);
    static_assert(is_void_sfinae_f_v<void>);
```


#### is_void_sfinae_sの実装 <a id="SS_3_3_2_4"></a>
[SFINAE](term_explanation.md#SS_5_4_7)を使用したクラステンプレートis_void_sfinae_sの実装は以下のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp 146

    namespace Inner_ {
    template <typename T>
    T*& t2ptr();  // 定義は不要
    }  // namespace Inner_

    template <typename T, typename = void*&>
    struct is_void_sfinae_s : std::false_type {
    };

    template <typename T>
    struct is_void_sfinae_s<
        T,
        // T != voidの場合、ill-formed
        // T == voidの場合、well-formedでvoid*&生成
        decltype(Inner_::t2ptr<T>() = Inner_::t2ptr<void>())

        > : std::true_type {
    };

    template <typename T>
    constexpr bool is_void_sfinae_s_v{is_void_sfinae_s<T>::value};
```

1つ目のis_void_sfinae_sはプライマリテンプレートである。
is_void_sfinae_sの特殊化が[name lookup](term_explanation.md#SS_5_4_2)の対象の中に見つからなかった場合、
これが使われる。

2つ目のis_void_sfinae_sは、上記を抜粋した下記のコード

```cpp
    // @@@ example/template/is_void_ut.cpp 162

    // T != voidの場合、ill-formed
    // T == voidの場合、well-formedでvoid*&生成
    decltype(Inner_::t2ptr<T>() = Inner_::t2ptr<void>())
```

がT == voidの時のみ、well-formedになり、このテンプレートは下記のようにインスタンス化される。

```cpp
    struct is_void_sfinae_s<void, void*&>
```

この形状はプライマリテンプレートの

* 第1パラメータにvoidを与える
* 第2パラメータには何も与えない(デフォルトのまま)

とした場合の、つまりプライマリテンプレートを

```cpp
    struct is_void_sfinae_s<void>   // プライマリテンプレート
```

としてインスタンス化した場合と一致する。
プライマリと特殊化が一致した場合、特殊化されたものがname lookupで選択される。

T != voidの場合、 2つ目のis_void_sfinae_sはill-formedになり、name lookupの対象から外れるため、
プライマリが選択される。

以上をまとめると、

| T       | is_void_sfinae_sの基底クラス |
|---------|------------------------------|
| == void | std::true_type               |
| != void | std::false_type              |

となる。以下の単体テストによって、このことを確かめることができる。

```cpp
    // @@@ example/template/is_void_ut.cpp 179

    static_assert(!is_void_sfinae_s_v<int>);
    static_assert(std::is_base_of_v<std::false_type, is_void_sfinae_s<int>>);

    static_assert(!is_void_sfinae_s_v<std::string>);
    static_assert(std::is_base_of_v<std::false_type, is_void_sfinae_s<std::string>>);

    static_assert(is_void_sfinae_s_v<void>);
    static_assert(std::is_base_of_v<std::true_type, is_void_sfinae_s<void>>);
```

上記コードのように「プライマリテンプレートのデフォルトパラメータ」と、

```cpp
    // @@@ example/template/is_void_ut.cpp 162

    // T != voidの場合、ill-formed
    // T == voidの場合、well-formedでvoid*&生成
    decltype(Inner_::t2ptr<T>() = Inner_::t2ptr<void>())
```

が「well-formedであった場合に生成される型」が一致することを利用した静的ディスパッチは、
SFINAEとクラステンプレートの特殊化を組み合わせたメタ関数の典型的な実装パターンである。
ただし、一般にはill-formedを起こすためにst::enable_ifを使うことが多いため、
「[is_void_ena_sの実装](template_meta_programming.md#SS_3_3_2_5)」でその例を示す。


#### is_void_ena_sの実装 <a id="SS_3_3_2_5"></a>
[std::enable_if](template_meta_programming.md#SS_3_3_1_4)による[SFINAE](term_explanation.md#SS_5_4_7)とクラステンプレートの特殊化を使用した
is_void_ena_sの実装は以下のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp 192
    template <typename T, typename = void>
    struct is_void_ena_s : std::false_type {
    };

    template <typename T>
    struct is_void_ena_s<
        T,
        typename std::enable_if_t<is_void_f<T>()>
        > : std::true_type {
    };

    template <typename T>
    constexpr bool is_void_ena_s_v{is_void_ena_s<T>::value};
```

この例では、「[is_void_sfinae_sの実装](template_meta_programming.md#SS_3_3_2_4)」の

```cpp
    // @@@ example/template/is_void_ut.cpp 162

    // T != voidの場合、ill-formed
    // T == voidの場合、well-formedでvoid*&生成
    decltype(Inner_::t2ptr<T>() = Inner_::t2ptr<void>())
```

で示したSFINAEの処理を上記を抜粋した下記のコード

```cpp
    // @@@ example/template/is_void_ut.cpp 202

    typename std::enable_if_t<is_void_f<T>()>
```

で行っている。
std::enable_ifの値パラメータis_void_f\<T>()は、「[is_void_fの実装](template_meta_programming.md#SS_3_3_2_1)」で示したものである。

単体テストは、「[is_void_sfinae_sの実装](template_meta_programming.md#SS_3_3_2_4)」で示したものとほぼ同様で、以下のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp 216

    static_assert(!is_void_ena_s_v<int>);
    static_assert(std::is_base_of_v<std::false_type, is_void_ena_s<int>>);

    static_assert(!is_void_ena_s_v<std::string>);
    static_assert(std::is_base_of_v<std::false_type, is_void_ena_s<std::string>>);

    static_assert(is_void_ena_s_v<void>);
    static_assert(std::is_base_of_v<std::true_type, is_void_ena_s<void>>);
```


#### is_void_cond_sの実装 <a id="SS_3_3_2_6"></a>
[std::conditional](template_meta_programming.md#SS_3_3_1_5)と関数テンプレートの特殊化を使用したis_void_cond_sの実装は以下のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp 229
    template <typename T>
    struct is_void_cond_s : std::conditional_t<is_void_f<T>(), std::true_type, std::false_type> {
    };

    template <typename T>
    constexpr bool is_void_cond_s_v{is_void_cond_s<T>::value};
```

std::conditionalの値パラメータis_void_f\<T>()は、「[is_void_fの実装](template_meta_programming.md#SS_3_3_2_1)」で示したものである。
この例では、SFINAEもクラステンプレートの特殊化も使用していないが、
下記単体テストからわかる通り、「[is_void_sfinae_sの実装](template_meta_programming.md#SS_3_3_2_4)」と同じ機能を備えている。

```cpp
    // @@@ example/template/is_void_ut.cpp 240

    static_assert(!is_void_cond_s_v<int>);
    static_assert(std::is_base_of_v<std::false_type, is_void_cond_s<int>>);

    static_assert(!is_void_cond_s_v<std::string>);
    static_assert(std::is_base_of_v<std::false_type, is_void_cond_s<std::string>>);

    static_assert(is_void_cond_s_v<void>);
    static_assert(std::is_base_of_v<std::true_type, is_void_cond_s<void>>);
```


### is_same_xxxの実装 <a id="SS_3_3_3"></a>
ここではstd::is_same\<T, U>に似た、
以下のような仕様を持ついくつかのテンプレートis_same_xxxの実装を考える。

|テンプレートパラメータ|戻り値            |
|----------------------|------------------|
|T == U                |true              |
|T != U                |false             |

それぞれのis_same_xxxは下記テーブルで示された言語機能を使用して実装する。

|is_same_xxx      |実装方法                                               |
|-----------------|-------------------------------------------------------|
|is_same_f        |関数テンプレートのオーバーロード                       |
|is_same_v        |定数テンプレートの特殊化                               |
|is_same_s        |クラステンプレートの特殊化                             |
|is_same_sfinae_f |SFINAEと関数テンプレート/関数のオーバーロード          |
|is_same_sfinae_s |SFINAEとクラステンプレートの特殊化                     |
|is_same_templ    |テンプレートテンプレートパラメータ                     |
|IsSameSomeOf     |パラメータパックと再帰                                 |

#### is_same_fの実装 <a id="SS_3_3_3_1"></a>
関数テンプレートのオーバーロードを用いたis_same_fの実装は以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp 9

    template <typename T, typename U>
    constexpr bool is_same_f_helper(T const*, U const*) noexcept
    {
        return false;
    }

    template <typename T>
    constexpr bool is_same_f_helper(T const*, T const*) noexcept
    {
        return true;
    }

    template <typename T, typename U>
    constexpr bool is_same_f() noexcept
    {
        return is_same_f_helper(static_cast<T*>(nullptr), static_cast<U*>(nullptr));
    }

    template <typename T, typename U>
    constexpr bool is_same_f_v{is_same_f<T, U>()};
```

すでに述べたように関数テンプレートの部分特殊化は言語仕様として認められておらず、

```cpp
    // @@@ example/template/is_same_ut.cpp 34

    template <typename T, typename U>
    constexpr bool is_same_f()
    {
        return true;
    }

    template <typename T>
    constexpr bool is_same_f<T, T>()
    {
        return true;
    }
```

上記のようなのようなコードは、以下のようなコンパイルエラーになる
(g++/clang++のような優れたコンパイラを使えば、
以下のメッセージのように簡単に問題点が理解できることもある)。

```
    is_same_ut.cpp:35:32: error: non-class, non-variable partial specialization ‘
                                          is_same_f<T, T>’ is not allowed
        35 | constexpr bool is_same_f<T, T>()
```

関数テンプレートは部分特殊化が出来ない代わりに、
同じ識別子を持つ関数や関数テンプレートとのオーバーロードができる。関数とのオーバーロードの場合、
is_same_f_helper\<T>()のようなテンプレートパラメータを直接使用した静的ディスパッチが出来ないため、
常に型推測によるディスパッチが必要になる。

単体テストは以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp 52

    static_assert(!is_same_f_v<int, void>);
    static_assert(is_same_f_v<int, int>);
    static_assert(!is_same_f_v<int, uint32_t>);
    static_assert(is_same_f_v<std::string, std::basic_string<char>>);
```

#### is_same_vの実装 <a id="SS_3_3_3_2"></a>
定数テンプレートの特殊化を用いたis_same_vの実装は以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp 61

    template <typename T, typename U>
    constexpr bool is_same_v{false};

    template <typename T>
    constexpr bool is_same_v<T, T>{true};
```

単純であるため、解説は不要だろう。 単体テストは以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp 72

    static_assert(!is_same_v<int, void>);
    static_assert(is_same_v<int, int>);
    static_assert(!is_same_v<int, uint32_t>);
    static_assert(is_same_v<std::string, std::basic_string<char>>);
```


#### is_same_sの実装 <a id="SS_3_3_3_3"></a>
クラステンプレートの特殊化を用いたis_same_sの実装は以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp 81

    template <class T, class U>
    struct is_same_s {
        static constexpr bool value{false};
    };

    template <class T>
    struct is_same_s<T, T> {
        static constexpr bool value{true};
    };

    template <typename T, typename U>
    constexpr bool is_same_s_v{is_same_s<T, U>::value};
```

「[is_same_vの実装](template_meta_programming.md#SS_3_3_3_2)」と同様に単純であるため、解説は不要だろう。 単体テストは以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp 99

    static_assert(!is_same_s_v<int, void>);
    static_assert(is_same_s_v<int, int>);
    static_assert(!is_same_s_v<int, uint32_t>);
    static_assert(is_same_s_v<std::string, std::basic_string<char>>);
```


#### is_same_sfinae_fの実装 <a id="SS_3_3_3_4"></a>
SFINAEと関数テンプレート/関数のオーバーロードを用いたis_same_sfinae_f実装は以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp 108

    namespace Inner_ {
    template <typename T, typename U>
    constexpr auto is_same_sfinae_f_detector(T const* t, U const* u) noexcept
        -> decltype(t = u, u = t, bool{})  // T != Uの場合、t = u, u = tはill-formed
                                           // T == Uの場合、well-formedでbool型生成
    {
        return true;
    }

    constexpr bool is_same_sfinae_f_detector(...) noexcept { return false; }
    }  // namespace Inner_

    template <typename T, typename U>
    constexpr bool is_same_sfinae_f() noexcept
    {
        return Inner_::is_same_sfinae_f_detector(static_cast<T*>(nullptr), static_cast<U*>(nullptr));
    }

    template <typename T, typename U>
    constexpr bool is_same_sfinae_f_v{is_same_sfinae_f<T, U>()};
```

上記の抜粋である下記コードのコメントで示したように、

```cpp
    // @@@ example/template/is_same_ut.cpp 114

    -> decltype(t = u, u = t, bool{})  // T != Uの場合、t = u, u = tはill-formed
                                       // T == Uの場合、well-formedでbool型生成
```

T != Uの場合、この関数テンプレートはill-formedとなりname lookupの対象ではなくなる。
その結果、関数is_same_sfinae_f_detectorが選択される。
省略記号"..."(ellipsis)を引数とする関数は、そのオーバーロード群の中での最後の選択となるため、
T == Uの場合は、関数テンプレートis_same_sfinae_f_detectorが選択される。

単体テストは以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp 138

    static_assert(!is_same_sfinae_f_v<int, void>);
    static_assert(is_same_sfinae_f_v<int, int>);
    static_assert(!is_same_sfinae_f_v<int, uint32_t>);
    static_assert(is_same_sfinae_f_v<std::string, std::basic_string<char>>);
```


#### is_same_sfinae_sの実装 <a id="SS_3_3_3_5"></a>
SFINAEとクラステンプレートの特殊化を用いたis_same_sfinae_sの実装は以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp 147

    namespace Inner_ {
    template <typename T>
    T*& t2ptr();
    }

    template <typename T, typename U, typename = T*&>
    struct is_same_sfinae_s : std::false_type {
    };

    template <typename T, typename U>
    struct is_same_sfinae_s<
        T, U,

        // T != Uの場合、ill-formed
        // T == Uの場合、well-formedでT*&生成
        decltype(Inner_::t2ptr<T>() = Inner_::t2ptr<U>(), Inner_::t2ptr<U>() = Inner_::t2ptr<T>())
        > : std::true_type {
    };

    template <typename T, typename U>
    constexpr bool is_same_sfinae_s_v{is_same_sfinae_s<T, U>::value};
```

「[is_void_sfinae_sの実装](template_meta_programming.md#SS_3_3_2_4)」とほぼ同様であるため、解説は不要だろう。 
単体テストは以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp 176

    static_assert(!is_same_sfinae_s_v<int, void>);
    static_assert(is_same_sfinae_s_v<int, int>);
    static_assert(!is_same_sfinae_s_v<int, uint32_t>);
    static_assert(is_same_sfinae_s_v<std::string, std::basic_string<char>>);
```


#### is_same_templの実装 <a id="SS_3_3_3_6"></a>
例えば、std::stringとstd::basic_string\<T>が同じもしくは違う型であることを確認するためには、
すでに示したis_same_sを使用し、

```cpp
    // @@@ example/template/is_same_ut.cpp 197

    static_assert(is_same_s_v<std::string, std::basic_string<char>>);
    static_assert(!is_same_s_v<std::string, std::basic_string<signed char>>);
```

のようにすればよいが、
以下に示したコードのようにテンプレートテンプレートパラメータを使うことでも実装できる。

```cpp
    // @@@ example/template/is_same_ut.cpp 185

    template <typename T, template <class...> class TEMPL, typename... ARGS>
    struct is_same_templ : is_same_sfinae_s<T, TEMPL<ARGS...>> {
    };

    template <typename T, template <class...> class TEMPL, typename... ARGS>
    constexpr bool is_same_templ_v{is_same_templ<T, TEMPL, ARGS...>::value};
```

上記のis_same_templは、第2引数にクラステンプレート、
第3引数以降にそのクラステンプレートの1個以上の引数を取ることができる。
使用例を兼ねた単体テストは以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp 202

    static_assert(is_same_templ_v<std::string, std::basic_string, char>);
    static_assert(!is_same_templ_v<std::string, std::basic_string, signed char>);
```

これを応用したエイリアステンプレート

```cpp
    // @@@ example/template/is_same_ut.cpp 209

    template <typename T>
    using gen_std_string = is_same_templ<std::string, std::basic_string, T>;

    template <typename T>
    constexpr bool gen_std_string_v{gen_std_string<T>::value};
```

は与えられたテンプレートパラメータがstd::stringを生成するかどうかを判定することができる。

```cpp
    // @@@ example/template/is_same_ut.cpp 220

    static_assert(gen_std_string_v<char>);
    static_assert(!gen_std_string_v<signed char>);
```


#### IsSameSomeOfの実装 <a id="SS_3_3_3_7"></a>
IsSameSomeOfはこれまでの例とは少々異なり、

* 第1パラメータが第2パラメータ以降で指定された型の
    * どれかと同じであれば、std::true_typeから派生する
    * どれとも違えば、std::false_typeから派生する
* 2つの型の同一性の判定にはstd::is_sameを使用する
* 汎用性が高いため名前空間Nstdで定義し、命名はキャメルにする

のような特徴のを持つ。
このようなIsSameSomeOfをパラメータパックと再帰を使用して実装すると以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits.h 10

    namespace Nstd {
    namespace Inner_ {

    template <typename T, typename U, typename... Us>
    struct is_same_some_of {
        static constexpr bool value{std::is_same_v<T, U> ? true : is_same_some_of<T, Us...>::value};
    };

    template <typename T, typename U>
    struct is_same_some_of<T, U> {
        static constexpr bool value{std::is_same_v<T, U>};
    };

    template <typename T, typename... Us>
    constexpr bool is_same_some_of_v{is_same_some_of<T, Us...>::value};
    }  // namespace Inner_

    template <typename T, typename... Us>
    struct IsSameSomeOf
        : std::conditional_t<Inner_::is_same_some_of_v<T, Us...>, std::true_type, std::false_type> {
    };

    template <typename T, typename... Us>
    constexpr bool IsSameSomeOfV{IsSameSomeOf<T, Us...>::value};
    }  // namespace Nstd
```

IsSameSomeOfは、TがUsのいずれかと一致するかどうかの処理をInner\_::is_same_some_ofに移譲する。

Usが1つだった場合、特殊化されたInner\_::is_same_some_ofのvalueがstd::is_same::valueで初期化される。
Usが複数だった場合、 プライマリのInner\_::is_same_some_ofは、
IsSameSomeOfから渡されたパラメータパックUsを、UとパラメータパックUsに分割後、
TとUをstd::is_sameで比較し、

* 同じ場合、valueはtrueで初期化される
* 違う場合、valueは再帰的に読み出されたInner\_::is_same_some_of\<T, Us...>::valueで初期化される

再帰的なInner\_::is_same_some_of::valueの読み出しは、
IsSameSomeOfが受け取ったパラメータパックをひとつずつ左シフトしながら、
それが1つになるまで(特殊化されたInner\_::is_same_some_ofが使われるまで)、続けられる。

単体テストは以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp 14

    static_assert(!Nstd::IsSameSomeOfV<int, int8_t, int16_t, uint16_t>);
    static_assert(Nstd::IsSameSomeOfV<int, int8_t, int16_t, uint16_t, int32_t>);
    static_assert(Nstd::IsSameSomeOfV<int&, int8_t, int16_t, int32_t&, int32_t>);
    static_assert(!Nstd::IsSameSomeOfV<int&, int8_t, int16_t, uint32_t&, int32_t>);
    static_assert(Nstd::IsSameSomeOfV<std::string, int, char*, std::string>);
    static_assert(!Nstd::IsSameSomeOfV<std::string, int, char*>);
```


### AreConvertibleXxxの実装 <a id="SS_3_3_4"></a>
std::is_convertible\<FROM, TO>は、

* 型FROMが型TOに変換できる場合、std::true_typeから派生する
* 型FROMが型TOに変換できない場合、std::false_typeから派生する

のような仕様を持つテンプレートである。

ここでは、

* std::is_convertibleを複数のFROMが指定できるように拡張したNstd::AreConvertible
* 縮小無しでの型変換ができるかどうかを判定するAreConvertibleWithoutNarrowConv

の実装を考える。


#### AreConvertibleの実装 <a id="SS_3_3_4_1"></a>
AreConvertibleの実装は以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits.h 42

    namespace Nstd {
    namespace Inner_ {

    template <typename TO, typename FROM, typename... FROMs>
    struct are_convertible {
        static constexpr bool value{
            std::is_convertible_v<FROM, TO> && are_convertible<TO, FROMs...>::value};
    };

    template <typename TO, typename FROM>
    struct are_convertible<TO, FROM> {
        static constexpr bool value{std::is_convertible_v<FROM, TO>};
    };

    template <typename TO, typename... FROMs>
    constexpr bool are_convertible_v{are_convertible<TO, FROMs...>::value};
    }  // namespace Inner_

    template <typename TO, typename... FROMs>
    struct AreConvertible
        : std::conditional_t<Inner_::are_convertible_v<TO, FROMs...>, std::true_type, std::false_type> {
    };

    template <typename TO, typename... FROMs>
    constexpr bool AreConvertibleV{AreConvertible<TO, FROMs...>::value};
    }  // namespace Nstd
```


「[IsSameSomeOfの実装](template_meta_programming.md#SS_3_3_3_7)」のコードパターンとほぼ同様であるため、解説は不要だろうが、

* パラメータパックの都合上、TOとFROMのパラメータの位置がstd::is_convertibleとは逆になる
* IsSameSomeOfでは条件の一つがtrueであればIsSameSomeOf::valueがtrueとなるが、
  AreConvertibleでは全条件がtrueとならない限り、AreConvertible::valueがtrueとならない

ので注意が必要である。

単体テストは以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp 27

    static_assert(Nstd::AreConvertibleV<int, int8_t, int16_t, int>);
    static_assert(Nstd::AreConvertibleV<int, char, int, int>);
    static_assert(!Nstd::AreConvertibleV<int, char*, int, int>);
    static_assert(Nstd::AreConvertibleV<std::string, std::string, char*, char[3]>);
    static_assert(!Nstd::AreConvertibleV<std::string, std::string, char*, int>);
```


#### AreConvertibleWithoutNarrowConvの実装 <a id="SS_3_3_4_2"></a>
縮小無しの型変換ができるかどうかを判定するAreConvertibleWithoutNarrowConvは、
AreConvertibleと同じように実装できるが、
その場合、AreConvertibleに対してstd::is_convertibleが必要になったように、
AreConvertibleWithoutNarrowConvに対しis_convertible_without_narrow_convが必要になる。

縮小無しでFROMからTOへの型変換ができるかどうかを判定するis_convertible_without_narrow_convは、
SFINAEと関数テンプレート/関数のオーバーライドを使用し以下のように実装できる。

```cpp
    // @@@ example/template/nstd_type_traits.h 75

    namespace Nstd {
    namespace Inner_ {

    template <typename TO, typename FROM>
    class is_convertible_without_narrow_conv {
        template <typename T = TO, typename U = FROM>
        static constexpr auto detector(T* t, U* u) noexcept
            // 縮小無しでFROMからTOへ変換可能な場合、*t = T{*u}はwell-formed
            // 上記ではない場合、*t = T{*u}はill-formed
            -> decltype(*t = T{*u}, bool{})
        {
            return true;
        }

        static constexpr bool detector(...) noexcept { return false; }

    public:
        static constexpr bool value{is_convertible_without_narrow_conv::detector(
            static_cast<TO*>(nullptr), static_cast<FROM*>(nullptr))};
    };

    template <typename TO, typename FROM>
    constexpr bool is_convertible_without_narrow_conv_v{
        is_convertible_without_narrow_conv<TO, FROM>::value};
    }  // namespace Inner_
    }  // namespace Nstd
```

AreConvertibleWithoutNarrowConvはNstdで定義するため、その内部のみで用いる
is_convertible_without_narrow_convはNstd::Inner\_で定義している。

上記を抜粋した下記のコードは「縮小型変換を発生さる{}による初期化は[ill-formed](term_explanation.md#SS_5_10_8)になる」
ことをSFINAEに利用している。

```cpp
    // @@@ example/template/nstd_type_traits.h 85

    // 縮小無しでFROMからTOへ変換可能な場合、*t = T{*u}はwell-formed
    // 上記ではない場合、*t = T{*u}はill-formed
    -> decltype(*t = T{*u}, bool{})
```

単体テストは以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp 39

    static_assert(Nstd::Inner_::is_convertible_without_narrow_conv_v<int, int>);
    static_assert(Nstd::Inner_::is_convertible_without_narrow_conv_v<int, int16_t>);
    static_assert(!Nstd::Inner_::is_convertible_without_narrow_conv_v<int16_t, int>);
    static_assert(Nstd::Inner_::is_convertible_without_narrow_conv_v<std::string, char*>);
    static_assert(!Nstd::Inner_::is_convertible_without_narrow_conv_v<char*, std::string>);
```

is_convertible_without_narrow_convを利用したAreConvertibleWithoutNarrowConv
の実装は以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits.h 108

    namespace Nstd {
    namespace Inner_ {

    template <typename TO, typename FROM, typename... FROMs>
    struct are_convertible_without_narrow_conv {
        static constexpr bool value{
            is_convertible_without_narrow_conv_v<TO, FROM> 
                  && are_convertible_without_narrow_conv<TO, FROMs...>::value};
    };

    template <typename TO, typename FROM>
    struct are_convertible_without_narrow_conv<TO, FROM> {
        static constexpr bool value{is_convertible_without_narrow_conv_v<TO, FROM>};
    };

    template <typename TO, typename FROM, typename... FROMs>
    constexpr bool are_convertible_without_narrow_conv_v{
        are_convertible_without_narrow_conv<TO, FROM, FROMs...>::value};
    }  // namespace Inner_

    template <typename TO, typename FROM, typename... FROMs>
    struct AreConvertibleWithoutNarrowConv
        : std::conditional_t<Inner_::are_convertible_without_narrow_conv_v<TO, FROM, FROMs...>,
                             std::true_type, std::false_type> {
    };

    template <typename TO, typename FROM, typename... FROMs>
    constexpr bool AreConvertibleWithoutNarrowConvV{
        AreConvertibleWithoutNarrowConv<TO, FROM, FROMs...>::value};
    }  // namespace Nstd
```

単体テストは以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp 47

    static_assert(Nstd::AreConvertibleWithoutNarrowConvV<int, char, int16_t, uint16_t>);
    static_assert(!Nstd::AreConvertibleWithoutNarrowConvV<int, char, int16_t, uint32_t>);
    static_assert(Nstd::AreConvertibleWithoutNarrowConvV<std::string, char[5], char*>);
    static_assert(Nstd::AreConvertibleWithoutNarrowConvV<double, float>);

    // int8_t -> doubleは縮小型変換
    static_assert(!Nstd::AreConvertibleWithoutNarrowConvV<double, float, int8_t>);
```

### 関数の存在の診断 <a id="SS_3_3_5"></a>
Nstdライブラリの開発には関数の存在の診断が欠かせない。
例えば、

* テンプレートパラメータに特定のメンバ関数がある場合、特殊化を作る
* テンプレートパラメータに範囲for文が適用できる場合にのみoperator<<を適用する
* テンプレートパラメータに適用できるoperator<<がすでにあった場合、
  自作operator<<を不活性化する

等、応用範囲は多岐にわたる。
ここでは、上記の場合分けを可能とするようなメタ関数に必要なテクニックや、
それらを使用したNstdのメタ関数の実装を下記のように示す。

* テンプレートパラメータである型が、メンバ関数void func()を持つかどうかの診断について、
  次の表のように実装を示す。

|メタ関数名                |メタ関数の目的                                          |
|--------------------------|--------------------------------------------------------|
|exists_void_func_sfinae_f |メンバ関数void func()を持つかどうかの判断               |
|exists_void_func_sfinae_s |同上                                                    |
|exists_void_func_sfinae_s2|同上                                                    |

* テンプレートパラメータに範囲for文ができるかどうかの診断について、
  次の表のように実装を示す。

|メタ関数名                 |メタ関数の目的                                               |
|---------------------------|-------------------------------------------------------------|
|exists_begin               |std::begin(T)が存在するか否かの診断                          |
|exists_end                 |std::end(T)が存在するか否かの診断                            |
|IsRange                    |T const& tの時に、for(auto const& : t)ができるかどうかの診断 |

* テンプレートパラメータにoperator<<(put toと発音する)ができるかどうかの診断について、
  次の表のように実装を示す。

|メタ関数名                 |メタ関数の目的                                         |
|---------------------------|-------------------------------------------------------|
|exists_put_to_as_member    |std::ostream::operator<<(T)が存在するか否かの診断      |
|exists_put_to_as_non_member|operator<<(std::ostream&, T)が存在するか否かの診断     |
|ExistsPutTo                |std::ostream& << Tができるかどうかの診断               |

* テンプレートパラメータがT[N]やC\<T>の形式である時のTに、
  operator<<が適用できるかの診断については、Tの型を取り出す必要がある。
  そのようなメタ関数ValueTypeの実装を示す。


#### exists_void_func_sfinae_fの実装 <a id="SS_3_3_5_1"></a>
「テンプレートパラメータである型が、メンバ関数void func()を持つかどうかを診断する」
exists_void_func_sfinae_f
のSFINAEと関数テンプレート/関数のオーバーロードを用いた実装は以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp 13

    namespace Inner_ {

    template <typename T>
    using exists_void_func_void =
        // メンバvoid func(void)があれば、voidを生成
        // メンバvoid func(void)がなければ、ill-formed
        typename std::enable_if_t<std::is_same_v<decltype(std::declval<T>().func()), void>>;
    }  // namespace Inner_

    template <typename T, typename = Inner_::exists_void_func_void<T>>
    constexpr bool exists_void_func_sfinae_f(T) noexcept
    {
        return true;
    }

    constexpr bool exists_void_func_sfinae_f(...) noexcept { return false; }
```

decltypeの中での関数呼び出しは、実際には呼び出されず関数の戻り値の型になる。
上記の抜粋である下記のコードはこの性質を利用してSFINAEによる静的ディスパッチを行っている。

```cpp
    // @@@ example/template/exists_func_ut.cpp 20

    // メンバvoid func(void)があれば、voidを生成
    // メンバvoid func(void)がなければ、ill-formed
    typename std::enable_if_t<std::is_same_v<decltype(std::declval<T>().func()), void>>;
```

単体テストは以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp 40

    // テスト用クラス
    struct X {
        void func();
    };

    struct Y {
        int func();
    };

    struct Z {
    private:
        void func();  // privateなvoid func()は外部からは呼び出せない
    };
```

```cpp
    // @@@ example/template/exists_func_ut.cpp 60

    static_assert(!exists_void_func_sfinae_f(int{}));
    static_assert(exists_void_func_sfinae_f(X{}));
    static_assert(!exists_void_func_sfinae_f(Y{}));
    static_assert(!exists_void_func_sfinae_f(Z{}));
```


#### exists_void_func_sfinae_sの実装 <a id="SS_3_3_5_2"></a>
「テンプレートパラメータである型が、メンバ関数void func()を持つかどうかを診断」する
exists_void_func_sfinae_s
のSFINAEとクラステンプレートの特殊化を用いた実装は以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp 69

    template <typename T, typename U = void>
    struct exists_void_func_sfinae_s : std::false_type {
    };

    template <typename T>
    struct exists_void_func_sfinae_s<T,
        // メンバvoid func()が呼び出せれば、voidを生成
        // メンバvoid func()が呼び出せなければ、ill-formed
        decltype(std::declval<T>().func())
        > : std::true_type {
    };

    template <typename T>
    constexpr bool exists_void_func_sfinae_s_v{exists_void_func_sfinae_s<T>::value};
```

exists_void_func_sfinae_fとほぼ等しいSFINAEを利用したクラステンプレートの特殊化により、
静的ディスパッチを行っている。

単体テストは以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp 91

    static_assert(!exists_void_func_sfinae_s_v<int>);
    static_assert(exists_void_func_sfinae_s_v<X>);
    static_assert(!exists_void_func_sfinae_s_v<Y>);
    static_assert(!exists_void_func_sfinae_s_v<Z>);
```


#### exists_void_func_sfinae_s2の実装 <a id="SS_3_3_5_3"></a>
exists_void_func_sfinae_sとほぼ同様の仕様を持つexists_void_func_sfinae_s2の

* SFINAE
* メンバ関数テンプレート/メンバ関数のオーバーロード
* メンバ関数へのポインタ

を用いた実装は以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp 100

    template <typename T>
    class exists_void_func_sfinae_s2 {

        // メンバvoid func()が呼び出せれば、メンバ関数テンプレートはtrueを返す
        // メンバvoid func()が呼び出せなければ、ill-formed
        template <typename U, void (U::*)() = &U::func>
        static constexpr bool detector(U*) noexcept
        {
            return true;
        }

        static constexpr bool detector(...) noexcept { return false; }

    public:
        static constexpr bool value{exists_void_func_sfinae_s2::detector(static_cast<T*>(nullptr))};
    };

    template <typename T>
    constexpr bool exists_void_func_sfinae_s2_v{exists_void_func_sfinae_s2<T>::value};
```

前2例とは異なり、上記の抜粋である下記コードのように、
メンバ関数へのポインタを使用しSFINAEを実装している。

```cpp
    // @@@ example/template/exists_func_ut.cpp 105

    // メンバvoid func()が呼び出せれば、メンバ関数テンプレートはtrueを返す
    // メンバvoid func()が呼び出せなければ、ill-formed
    template <typename U, void (U::*)() = &U::func>
    static constexpr bool detector(U*) noexcept
    {
        return true;
    }
```

あまり応用範囲が広くない方法ではあるが、
decltypeを使っていないのでC++03コンパイラにも受け入れられるメリットがある。

exists_void_func_sfinae_fと同じテスト用クラスを用いた単体テストは以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp 129

    static_assert(!exists_void_func_sfinae_s2_v<int>);
    static_assert(exists_void_func_sfinae_s2_v<X>);
    static_assert(!exists_void_func_sfinae_s2_v<Y>);
    static_assert(!exists_void_func_sfinae_s2_v<Z>);
```


#### exists_begin/exsits_endの実装 <a id="SS_3_3_5_4"></a>
「テンプレートパラメータTに対して、std::begin(T)が存在するか否かの診断」をするexists_beginの実装は、
「[exists_void_func_sfinae_sの実装](template_meta_programming.md#SS_3_3_5_2)」
で用いたパターンのメンバ関数を非メンバ関数に置き換えて使えば以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp 140

    template <typename, typename = void>
    struct exists_begin : std::false_type {
    };

    template <typename T>
    struct exists_begin<T, std::void_t<decltype(std::begin(std::declval<T>()))>> : std::true_type {
    };

    template <typename T>
    constexpr bool exists_begin_v{exists_begin<T>::value};
```

上記で使用したstd::void_tは、テンプレートパラメータが

* [ill-formed](term_explanation.md#SS_5_10_8)ならばill-formedになる
* [well-formed](term_explanation.md#SS_5_10_9)ならvoidを生成する

テンプレートである。

下記単体テストでは問題ないように見えるが、

```cpp
    // @@@ example/template/exists_func_ut.cpp 156

    static_assert(exists_begin_v<std::string>);
    static_assert(!exists_begin_v<int>);
    static_assert(exists_begin_v<int const[3]>);
```

下記の単体テストはstatic_assertがフェールするためコンパイルできない。

```cpp
    // @@@ example/template/exists_func_ut.cpp 166

    // 以下が問題
    static_assert(exists_begin_v<int[3]>);
```

理由は、

```cpp
    std::declval<int[3]>())
```

の戻り型が配列型の[rvalue](term_explanation.md#SS_5_5_3)である"int (&&) [3]"となり、
これに対応するstd::beginが定義されていないためである。

これに対処する方法方はいくつかあるが、
すべての配列は常にstd::beginの引数になれることに気づけば、
テンプレートパラメータが配列か否かで場合分けしたクラステンプレートの特殊化を使い、
下記のように実装できることにも気付けるだろう。

```cpp
    // @@@ example/template/exists_func_ut.cpp 183

    template <typename, typename = void>
    struct exists_begin : std::false_type {
    };

    // Tが非配列の場合の特殊化
    template <typename T>
    struct exists_begin<T,
                        typename std::enable_if_t<!std::is_array_v<T>,
                                                  std::void_t<decltype(std::begin(std::declval<T>()))>>>
        : std::true_type {
    };

    // Tが配列の場合の特殊化
    template <typename T>
    struct exists_begin<T, typename std::enable_if_t<std::is_array_v<T>>> : std::true_type {
    };

    template <typename T>
    constexpr bool exists_begin_v{exists_begin<T>::value};
```

2個目のexists_beginはTが配列でない場合、
3個目のexists_beginはTが配列ある場合にそれぞれが対応しているが、複雑すぎて何とも醜い。
ということで、このコードは却下して、別のアイデアを試そう。

テンプレートパラメータが配列である場合でも、
そのオブジェクトが[lvalue](term_explanation.md#SS_5_5_2)(この例ではint (&)[3])であれば、
std::beginはそのオブジェクトを使用できるので、
decltype内で使用できるlvalueのT型オブジェクトを生成できれば、
と考えれば下記のような実装を思いつくだろう。

```cpp
    // @@@ example/template/nstd_type_traits.h 150

    template <typename, typename = void>
    struct exists_begin : std::false_type {
    };

    template <typename T>
    struct exists_begin<T, std::void_t<decltype(std::begin(std::declval<T&>()))>> : std::true_type {
    };

    template <typename T>
    constexpr bool exists_begin_v{exists_begin<T>::value};
```

十分にシンプルなのでこれを採用し、exists_endも同様に実装する。

```cpp
    // @@@ example/template/nstd_type_traits.h 163

    template <typename, typename = void>
    struct exists_end : std::false_type {
    };

    template <typename T>
    struct exists_end<T, std::void_t<decltype(std::end(std::declval<T&>()))>> : std::true_type {
    };
    template <typename T>
    constexpr bool exists_end_v{exists_end<T>::value};
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp 81

    static_assert(exists_begin_v<std::string>);
    static_assert(!exists_begin_v<int>);
    static_assert(exists_begin_v<int const[3]>);
    static_assert(exists_begin_v<int[3]>);  // 問題が解決

    static_assert(exists_end_v<std::string>);
    static_assert(!exists_end_v<int>);
    static_assert(exists_end_v<int const[3]>);
    static_assert(exists_end_v<int[3]>);
```


#### IsRangeの実装 <a id="SS_3_3_5_5"></a>
[範囲for文](https://cpprefjp.github.io/lang/cpp11/range_based_for.html)
文の":"の後ろにT型オブジェクトが指定できる要件は、

* std::begin(T)、std::end(T)がTのイテレータであるITOR型のオブジェクトを返す
* std::begin(T)が返すITORオブジェクトはTが保持する先頭の要素を指す
* std::end(T)が返すITORオブジェクトはTが保持する最後の要素の次を指す
* ++ITORによりTが保持する全要素にアクセスできる

ようなことである。多くの要件はセマンティクス的なものであり、
メタ関数で診断できることは前項で見たようなstd::begin(T)、std::end(T)の可否のみであると考えれば、
IsRangeの実装は以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits.h 177

    template <typename T>
    struct IsRange : std::conditional_t<Inner_::exists_begin_v<T> && Inner_::exists_end_v<T>,
                                        std::true_type, std::false_type> {
    };

    template <typename T>
    constexpr bool IsRangeV{IsRange<T>::value};
```

なお、上記のコードでは、exists_begin/exsits_endは、IsRangeの実装の詳細であるため、
名前空間Inner\_で宣言している。

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp 100

    static_assert(IsRangeV<std::string>);
    static_assert(!IsRangeV<int>);
    static_assert(IsRangeV<int const[3]>);
    static_assert(IsRangeV<int[3]>);
```


#### exists_put_to_as_memberの実装 <a id="SS_3_3_5_6"></a>
std::ostreamのメンバ関数operator<<の戻り型はstd::ostream&であるため、
exists_put_to_as_memberの実装は以下のようになる("<<"は英語で"put to"と発音する)。

```cpp
    // @@@ example/template/exists_func_ut.cpp 219

    template <typename, typename = std::ostream&>
    struct exists_put_to_as_member : std::false_type {
    };

    template <typename T>
    struct exists_put_to_as_member<T, decltype(std::declval<std::ostream&>().operator<<(
                                          std::declval<T>()))> : std::true_type {
    };

    template <typename T>
    constexpr bool exists_put_to_as_member_v{exists_put_to_as_member<T>::value};
```

「[exists_void_func_sfinae_fの実装](template_meta_programming.md#SS_3_3_5_1)」と同様のパターンを使用したので解説は不要だろう。

単体テストは以下のようになる。

```cpp
    // @@@ example/template/test_class.h 3

    class test_class_exits_put_to {
    public:
        test_class_exits_put_to(int i = 0) noexcept : i_{i} {}
        int get() const noexcept { return i_; }

    private:
        int i_;
    };

    inline std::ostream& operator<<(std::ostream& os, test_class_exits_put_to const& p)
    {
        return os << p.get();
    }

    class test_class_not_exits_put_to {};
```

```cpp
    // @@@ example/template/exists_func_ut.cpp 236

    static_assert(exists_put_to_as_member_v<bool>);
    static_assert(!exists_put_to_as_member_v<std::string>);
    static_assert(!exists_put_to_as_member_v<std::vector<int>>);
    static_assert(exists_put_to_as_member_v<std::vector<int>*>);
    static_assert(!exists_put_to_as_member_v<test_class_exits_put_to>);
    static_assert(!exists_put_to_as_member_v<test_class_not_exits_put_to>);
    static_assert(exists_put_to_as_member_v<test_class_not_exits_put_to[3]>);  // 驚き!
```

やや驚きなのは、上記の抜粋である下記コードがコンパイルできることである。

```cpp
    // @@@ example/template/exists_func_ut.cpp 245

    static_assert(exists_put_to_as_member_v<test_class_not_exits_put_to[3]>);  // 驚き!
```

これは、

```cpp
    std::ostream& std::ostream::operator<<(void const*)
```

が定義されているため、配列がポインタに変換されてこのメンバ関数にバインドした結果である。


#### exists_put_to_as_non_memberの実装 <a id="SS_3_3_5_7"></a>
exists_put_to_as_non_memberの実装は以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp 254

    template <typename, typename = std::ostream&>
    struct exists_put_to_as_non_member : std::false_type {
    };

    template <typename T>
    struct exists_put_to_as_non_member<T, decltype(operator<<(std::declval<std::ostream&>(),
                                                              std::declval<T>()))> : std::true_type {
    };

    template <typename T>
    constexpr bool exists_put_to_as_non_member_v{exists_put_to_as_non_member<T>::value};
```

「[exists_begin/exsits_endの実装](template_meta_programming.md#SS_3_3_5_4)」と「[exists_put_to_as_memberの実装](template_meta_programming.md#SS_3_3_5_6)」
で使用したパターンを混合しただけなので解説や単体テストは省略する。


#### ExistsPutToの実装 <a id="SS_3_3_5_8"></a>
テンプレートパラメータT、T型オブジェクトtに対して、
std::ostream << tができるかどうかを判断するExistsPutToの実装は以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp 283

    template <typename T>
    struct ExistsPutTo
        : std::conditional_t<
              Inner_::exists_put_to_as_member_v<T> || Inner_::exists_put_to_as_non_member_v<T>,
              std::true_type, std::false_type> {
    };

    template <typename T>
    constexpr bool ExistsPutToV{ExistsPutTo<T>::value};
```

「[IsRangeの実装](template_meta_programming.md#SS_3_3_5_5)」に影響されて、一旦このように実装したが、先に書いた通り、
そもそものExistsPutToの役割はstd::ostream << tができるかどうかの診断であることを思い出せば、
下記のように、もっとシンプルに実装できることに気づくだろう。

```cpp
    // @@@ example/template/nstd_type_traits.h 192

    namespace Nstd {

    template <typename, typename = std::ostream&>
    struct ExistsPutTo : std::false_type {
    };

    template <typename T>
    struct ExistsPutTo<T, decltype(std::declval<std::ostream&>() << std::declval<T>())>
        : std::true_type {
    };

    template <typename T>
    constexpr bool ExistsPutToV{ExistsPutTo<T>::value};
    }  // namespace Nstd
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp 111

    static_assert(Nstd::ExistsPutToV<bool>);
    static_assert(Nstd::ExistsPutToV<std::string>);
    static_assert(!Nstd::ExistsPutToV<std::vector<int>>);
    static_assert(Nstd::ExistsPutToV<std::vector<int>*>);
    static_assert(Nstd::ExistsPutToV<test_class_exits_put_to>);
    static_assert(!Nstd::ExistsPutToV<test_class_not_exits_put_to>);
    static_assert(Nstd::ExistsPutToV<test_class_not_exits_put_to[3]>);
```

#### ValueTypeの実装 <a id="SS_3_3_5_9"></a>
下記で示す通り、

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp 129

    struct T {};

    std::ostream& operator<<(std::ostream& os, std::vector<T> const& x)
    {
        return os << "T:" << x.size();
    }

    std::ostream& operator<<(std::ostream&, T const&) = delete;

    static_assert(!Nstd::ExistsPutToV<T>);              // std::cout << T{} はできない
    static_assert(Nstd::ExistsPutToV<std::vector<T>>);  // std::cout << std::vector<T>{} はできる
    static_assert(Nstd::ExistsPutToV<T[3]>);            // std::cout << T[3]{} はできる
```

型Xが与えれ、その形式が、

* クラステンプレートCとその型パラメータTにより、C\<T>
* 型Tと定数整数Nにより、T[N]

のような場合、ExistsPutToV\<X>がtrueであっても、ExistsPutToV\<T>の真偽はわからない。
従って上記のようなTに対して、ExistsPutToV\<T>がtrueかどうかを診断するためには、
XからTを導出することが必要になる。ここでは、そのようなメタ関数ValueTypeの実装を考える。
このValueTypeは上記のX、Tに対して、

```cpp
    std::is_same<ValueType<X>::type, T>::value == true
```

となるような機能を持たなければならないことは明らかだろう。
その他の機能については実装しながら決定していく。

一見、難しそうなテンプレートを作るコツは、条件を絞って少しずつ作っていくことである。
いきなり大量のテンプレートを書いてしまうと、
その何十倍ものコンパイルエラーに打ちのめされること必至である。

ということで、まずは、1次元の配列に対してのみ動作するValueTypeの実装を示す
(下記で使用するstd::remove_extent_t\<T>は、テンプレートパラメータが配列だった場合に、
その次元を一つだけ除去するメタ関数である)。

```cpp
    // @@@ example/template/value_type_ut.cpp 14

    template <typename T, typename = void>
    struct ValueType {
        using type = void;
    };

    template <typename T>
    struct ValueType<T, typename std::enable_if_t<std::is_array_v<T>>> {
        using type = typename std::remove_extent_t<T>;
    };

    template <typename T>
    using ValueTypeT = typename ValueType<T>::type;
```

このコードは問題なく動作するが、下記の通り、2次元配列に対するValueType::typeは1次元配列となる。

```cpp
    // @@@ example/template/value_type_ut.cpp 32

    static_assert(std::is_same_v<int, ValueTypeT<int[1]>>);
    static_assert(std::is_same_v<void, ValueTypeT<int>>);
    static_assert(std::is_same_v<int[2], ValueTypeT<int[1][2]>>);
```

これを多次元配列に拡張する前に、配列の次元をValueType::Nestで返す機能を追加することにすると、
コードは下記のようになるだろう。

```cpp
    // @@@ example/template/value_type_ut.cpp 44

    template <typename T, typename = void>
    struct ValueType {
        using type = void;
        static constexpr size_t Nest{0};
    };

    template <typename T>
    struct ValueType<T, typename std::enable_if_t<std::is_array_v<T>>> {
        using type = typename std::remove_extent_t<T>;

        static constexpr size_t Nest{ValueType<type>::Nest + 1};
    };

    template <typename T>
    using ValueTypeT = typename ValueType<T>::type;
```

動作は下記のようになる。

```cpp
    // @@@ example/template/value_type_ut.cpp 69

    static_assert(0 == ValueType<int>::Nest);
    static_assert(1 == ValueType<int[1]>::Nest);
    static_assert(2 == ValueType<int[1][2]>::Nest);
```

ここで、下記のような仕様をもつValueType::type_n\<N>を考える。

```
    ValueType<int[1][2][3]>::type_n<0>が表す型は、int[1][2][3]
    ValueType<int[1][2][3]>::type_n<1>が表す型は、int[2][3]
    ValueType<int[1][2][3]>::type_n<2>が表す型は、int[3]
    ValueType<int[1][2][3]>::type_n<3>が表す型は、int
```

ValueType::type_n\<N>は玉ねぎの皮を一枚ずつむくようなメンバエイリアステンプレートになる。
プライマリの実装は以下のようになる。

```cpp
    // @@@ example/template/value_type_ut.cpp 82

    template <typename T, typename = void>
    struct ValueType {
        using type = void;
        static constexpr size_t Nest{0};

        template <size_t N>
        using type_n = typename std::conditional_t<N == 0, T, void>;
    };

```

Nが非0の場合、Value::type_n\<N>はvoidになる仕様にした。

配列に対する特殊化は以下のようになる。

```cpp
    // @@@ example/template/value_type_ut.cpp 94

    template <typename T, size_t N>
    struct ConditionalValueTypeN {
        using type = typename std::conditional_t<
            ValueType<T>::Nest != 0,
            typename ValueType<typename ValueType<T>::type>::template type_n<N - 1>, T>;
    };

    template <typename T>
    struct ConditionalValueTypeN<T, 0> {
        using type = T;
    };

    template <typename T>
    struct ValueType<T, typename std::enable_if_t<std::is_array_v<T>>> {
        using type = typename std::remove_extent_t<T>;

        static constexpr size_t Nest{ValueType<type>::Nest + 1};

        template <size_t N>
        using type_n = typename ConditionalValueTypeN<T, N>::type;
    };

    template <typename T>
    using ValueTypeT = typename ValueType<T>::type;

    template <typename T, size_t N>
    using ValueTypeT_n = typename ValueType<T>::template type_n<N>;
```

下記コードのコンパイル時の展開を説明することで、上記の解説を行う。

```cpp
    // @@@ example/template/value_type_ut.cpp 128

    using T = ValueTypeT_n<int[1][2][3], 3>;
```

1. ValueTypeのテンプレートパラメータが配列であるため、配列への特殊化であるValueTypeが選択され、
   下記の疑似コードのように展開される。

```cpp
    ValueType<int[1][2][3], void> {
        using type = int[2][3];
        static constexpr size_t Nest = ...
        using type_n = ConditionalValueTypeN<int[1][2][3], 3>::type;
    };
```

2. ConditionalValueTypeNは下記のように展開される。
   なお、下記のコードの中のtype_n前で使われているキーワードtemplateは、
   「外部のクラステンプレートのメンバテンプレートにアクセスする」際に必要になる記法である。

```cpp
    struct ConditionalValueTypeN<int[1][2][3], 3> {
        using type = typename std::conditional<
            true,   // ValueType<int[1][2][3]>::Nest == 3であるためtrue
            ValueType<ValueType<int[1][2][3]>::type>::template type_n<3 - 1>,
            int[1][2][3]>::type;
    };
```

3.  ValueType\<int\[1]\[2]\[3]>::typeは一枚皮をむいたint\[2]\[3]なので、
    上記はさらに下記のように展開される。

```cpp
    struct ConditionalValueTypeN<int[1][2][3], 3> {
        using type = ValueType<int[2][3]>::template type_n<2>;
    };
```

4. ConditionalValueTypeN\<int\[1]\[2]\[3], 3>::typeを展開するため、
   ValueType\<int\[2]\[3]>::template type_n<2>の展開が上記1 - 3のように繰り返される。
   この繰り返しはN == 0になるまで続く。

5. 3回の皮むきによりN == 0となる。
   この時点で、下記の特殊化が選択されるため再帰は終了し、ConditionalValueTypeN<>::typeはintとなる。

```cpp
    struct ConditionalValueTypeN<int, 0> {
        using type = int;
    };
```

6. 1 - 5により最終的には下記のように展開される。

```cpp
    ValueType<int[1][2][3]> {
        using type = int[2][3];
        static constexpr size_t Nest = 3;
        using type_n = int;
    };
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/value_type_ut.cpp 136

    using T = int[1][2][3];

    static_assert(ValueType<T>::Nest == 3);
    static_assert(std::is_same_v<int[1][2][3], ValueTypeT_n<T, 0>>);
    static_assert(std::is_same_v<int[2][3], ValueTypeT_n<T, 1>>);
    static_assert(std::is_same_v<int[3], ValueTypeT_n<T, 2>>);
    static_assert(std::is_same_v<int, ValueTypeT_n<T, 3>>);
    static_assert(std::is_same_v<void, ValueTypeT_n<T, 4>>);
    static_assert(std::is_same_v<void, ValueTypeT_n<T, 5>>);
```

また、ValueType::NestとValueType::type_n<>の関係に注目すれば、
上記エイリアスTに対して下記が成立する。

```cpp
    // @@@ example/template/value_type_ut.cpp 148

    static_assert(std::is_same_v<int, ValueTypeT_n<T, ValueType<T>::Nest>>);
```


このテンプレートにコンテナが渡された時の特殊化を与えることができればValueTypeは完成するが、
その前に名前の整理をした方が良いため、下記のような変更を行う。

* この例では、typeは配列が直接保持する型を表すが、
  この名前は慣例的にメタ関数の戻り型を表すことが多いため、現在の仕様では混乱を招く。
  また名は体を表す方が良いため、typeを改めtype_directとする。
* ValueTypeの結果は、上記のようにNestとtype_nの組み合わせで得られるが、このままでは使い勝手が悪い。
  慣例に従いこれをtypeとする。
* ConditionalValueTypeNは実装の詳細であるため外部から使われたくない。
  これまで通り、名前空間Inner\_で定義し、名前を小文字と\_で生成する。

これによりValueTypeは下記のようになる。

```cpp
    // @@@ example/template/value_type_ut.cpp 182

    template <typename T, typename = void>
    struct ValueType {
        using type_direct = void;

        static constexpr size_t Nest{0};

        template <size_t N>
        using type_n = typename std::conditional_t<N == 0, T, void>;

        using type = type_n<Nest>;
    };

    namespace Inner_ {

    template <typename T, size_t N>
    struct conditional_value_type_n {
        using type = typename std::conditional_t<
            ValueType<T>::Nest != 0,
            typename ValueType<typename ValueType<T>::type_direct>::template type_n<N - 1>, T>;
    };

    template <typename T>
    struct conditional_value_type_n<T, 0> {
        using type = T;
    };
    }  // namespace Inner_

    template <typename T>
    struct ValueType<T, typename std::enable_if_t<std::is_array_v<T>>> {
        using type_direct = typename std::remove_extent_t<T>;

        static constexpr size_t Nest{ValueType<type_direct>::Nest + 1};

        template <size_t N>
        using type_n = typename Inner_::conditional_value_type_n<T, N>::type;

        using type = type_n<Nest>;
    };

    template <typename T>
    using ValueTypeT = typename ValueType<T>::type;

    template <typename T, size_t N>
    using ValueTypeT_n = typename ValueType<T>::template type_n<N>;
```

準備は整ったので上記のValueTypeに下記のようなコンテナ用特殊化を追加する。

```cpp
    // @@@ example/template/value_type_ut.cpp 276

    namespace Inner_ {

    // Tが配列でなく、且つIsRrangeV<T>が真ならばコンテナと診断する
    template <typename T>
    constexpr bool is_container_v{Nstd::IsRangeV<T> && !std::is_array_v<T>};
    }  // namespace Inner_

    template <typename T>
    struct ValueType<T, typename std::enable_if_t<Inner_::is_container_v<T>>> {
        using type_direct = typename T::value_type;

        static constexpr size_t Nest{ValueType<type_direct>::Nest + 1};

        template <size_t N>
        using type_n = typename Inner_::conditional_value_type_n<T, N>::type;

        using type = type_n<Nest>;
    };

    template <typename T>
    using ValueTypeT = typename ValueType<T>::type;

    template <typename T, size_t N>
    using ValueTypeT_n = typename ValueType<T>::template type_n<N>;
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/value_type_ut.cpp 307

    using T = std::vector<std::list<int*>[3]>;

    static_assert(std::is_same_v<int*, ValueTypeT<T>>);

    static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
    static_assert(std::is_same_v<std::list<int*>[3], ValueTypeT_n<T, 1>>);
    static_assert(std::is_same_v<std::list<int*>, ValueTypeT_n<T, 2>>);
    static_assert(std::is_same_v<int*, ValueTypeT_n<T, 3>>);
    static_assert(std::is_same_v<void, ValueTypeT_n<T, 4>>);
```

最初のValueTypeには、その単純さとは不釣り合いな、やや複雑なSFINAE用のコードを記述をしたが、
ここまで来ればその理由は明らかだろう。
今回のように限定的な機能を作ってから、一般化して行く開発スタイルでも、
静的ディスパッチにはSFINAE等の汎用的手法を使った方が後の修正が少なく済むことが多い。
一方で、完成時にその静的ディスパッチが不要に複雑であると気づいた場合は、
リファクタリングを行い、コードを程よいレベルに留めなければならないことは言うまでもない。

ValueTypeの開発はまだ終わらない。静的ディスパッチは最初のカンが当り修正の必要はないと思うが、
「配列用特殊化とコンテナ用特殊化のほとんどがコードクローンになっている」という問題がある。
この程度のクローンは問題のないレベルであるとも言えるが、演習のため修正する。
また、合わせてTが配列かどうかを示すための定数IsBuiltinArrayも追加すると下記のようなコードになる。

```cpp
    // @@@ example/template/nstd_type_traits.h 213

    namespace Nstd {

    template <typename T, typename = void>  // ValueTypeのプライマリ
    struct ValueType {
        using type_direct = void;

        static constexpr bool   IsBuiltinArray{false};
        static constexpr size_t Nest{0};

        template <size_t N>
        using type_n = typename std::conditional_t<N == 0, T, void>;

        using type = type_n<Nest>;
    };

    namespace Inner_ {

    template <typename T, size_t N>
    struct conditional_value_type_n {
        using type = typename std::conditional_t<
            ValueType<T>::Nest != 0,
            typename ValueType<typename ValueType<T>::type_direct>::template type_n<N - 1>, T>;
    };

    template <typename T>
    struct conditional_value_type_n<T, 0> {
        using type = T;
    };

    template <typename T, typename = void>
    struct array_or_container : std::false_type {
    };

    template <typename T>
    struct array_or_container<T, typename std::enable_if_t<std::is_array_v<T>>> : std::true_type {
        using type = typename std::remove_extent_t<T>;
    };

    // Tが配列でなく、且つT型インスタンスに範囲for文が適用できるならばstdコンテナと診断する
    template <typename T>
    constexpr bool is_container_v{Nstd::IsRange<T>::value && !std::is_array_v<T>};

    template <typename T>
    struct array_or_container<T, typename std::enable_if_t<is_container_v<T>>> : std::true_type {
        using type = typename T::value_type;
    };

    template <typename T>
    constexpr bool array_or_container_v{array_or_container<T>::value};
    }  // namespace Inner_

    template <typename T>  // ValueTypeの特殊化
    struct ValueType<T, typename std::enable_if_t<Inner_::array_or_container_v<T>>> {
        using type_direct = typename Inner_::array_or_container<T>::type;

        static constexpr bool   IsBuiltinArray{std::is_array_v<T>};
        static constexpr size_t Nest{ValueType<type_direct>::Nest + 1};

        template <size_t N>
        using type_n = typename Inner_::conditional_value_type_n<T, N>::type;

        using type = type_n<Nest>;
    };

    template <typename T>
    using ValueTypeT = typename ValueType<T>::type;

    template <typename T, size_t N>
    using ValueTypeT_n = typename ValueType<T>::template type_n<N>;
    }  // namespace Nstd
```


## Nstdライブラリの開発2 <a id="SS_3_4"></a>
ここでは予定していた通りSafeArray2を開発し、その後Nstdに必要なライブラリの開発を続ける。


### SafeArray2の開発 <a id="SS_3_4_1"></a>
「[安全な配列型コンテナ](template_meta_programming.md#SS_3_2_3)」で断念したSafeArray2の開発を再開する前に、
SafeArray2の要件をまとめると、

* std::arrayを基底クラスとする
* operator[]に範囲チェックを行う
* SafeArrayでのパラメータパックによる初期化機能はそのまま残す
* SafeArrayではできなかった縮小型変換が起こる初期化にも対応する
* 新規要件として、
  縮小型変換により初期化されたかどうかを示すメンバ関数InitializedWithNarrowConv()を持つ。

となる。この要件を満たすためには、SafeArrayが

```cpp
    // @@@ example/template/safe_vector_ut.cpp 154

    template <typename... ARGS>  // コンストラクタを定義
    SafeArray(ARGS... args) : base_type{args...}
    {
    }
```

で行っていた初期化を、SafeArray2では、
「縮小型変換が起こるか否かによる場合分けを行い、
それぞれの場合に対応するコンストラクタテンプレートによって初期化」
するようにすれば良いことがわかる。

パラメータパックによるコンストラクタのシグネチャは上記した一種類しかないため、
関数のシグネチャの差異よるオーバーロードは使えない。
とすれば、テンプレートパラメータの型の差異によるオーバーロードを使うしか方法がない。
縮小型変換が起こるか否かの場合分けはSFINAEで実現させることができる。
という風な思考の変遷により以下のコードにたどり着く。


```cpp
    // @@@ example/template/safe_vector_ut.cpp 227
    namespace Nstd {

    template <typename T, size_t N>
    struct SafeArray2 : std::array<T, N> {
        using std::array<T, N>::array;  // 継承コンストラクタ
        using base_type = std::array<T, N>;

        // 縮小型変換した場合には、ill-formedになるコンストラクタ
        template <typename... ARGS,
                  typename = 
                      typename std::enable_if_t<
                           AreConvertibleWithoutNarrowConvV<T, ARGS...>>>
        SafeArray2(ARGS... args) : base_type{args...} // 初期化子リストによるarrayの初期化
        {
        }

        // 縮小型変換しない場合には、ill-formedになるコンストラクタ
        template <typename... ARGS, 
                  typename std::enable_if_t<
                      !AreConvertibleWithoutNarrowConvV<T, ARGS...>>* = nullptr>
        SafeArray2(ARGS... args) :
            base_type{T(args)...},  // 縮小型変換を抑止するため、T(args)が必要
            is_with_narrow_conv_{true}
        {
        }

        using size_type = typename base_type::size_type;

        typename base_type::reference       operator[](size_type i) { return this->at(i); }
        typename base_type::const_reference operator[](size_type i) const { return this->at(i); }

        bool InitializedWithNarrowConv() const noexcept { return is_with_narrow_conv_; }

    private:
        bool const is_with_narrow_conv_{false};
    };
    }  // namespace Nstd
```

下記のようなコードでのコンストラクタ呼び出しには、

```cpp
    // @@@ example/template/safe_vector_ut.cpp 290

    auto sa_init = Nstd::SafeArray2<int, 3>{1, 2, 3};
```

上記の抜粋である下記のコンストラクタが置換失敗により排除される(SFINAE)。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 247

    // 縮小型変換しない場合には、ill-formedになるコンストラクタ
    template <typename... ARGS, 
              typename std::enable_if_t<
                  !AreConvertibleWithoutNarrowConvV<T, ARGS...>>* = nullptr>
    SafeArray2(ARGS... args) :
        base_type{T(args)...},  // 縮小型変換を抑止するため、T(args)が必要
        is_with_narrow_conv_{true}
    {
    }
```

従って、マッチするコンストラクタは

```cpp
    // @@@ example/template/safe_vector_ut.cpp 236

    // 縮小型変換した場合には、ill-formedになるコンストラクタ
    template <typename... ARGS,
              typename = 
                  typename std::enable_if_t<
                       AreConvertibleWithoutNarrowConvV<T, ARGS...>>>
    SafeArray2(ARGS... args) : base_type{args...} // 初期化子リストによるarrayの初期化
    {
    }
```

のみとなり、無事にコンパイルが成功し、下記の単体テストもパスする。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 290

    auto sa_init = Nstd::SafeArray2<int, 3>{1, 2, 3};

    ASSERT_FALSE(sa_init.InitializedWithNarrowConv());  // 縮小型変換なし
    ASSERT_EQ(3, sa_init.size());
    ASSERT_EQ(1, sa_init[0]);
    ASSERT_EQ(2, sa_init[1]);
    ASSERT_EQ(3, sa_init[2]);
    ASSERT_THROW(sa_init[3], std::out_of_range);
```

下記の単体テストの場合、SFINAEにより、先述の例とは逆のコンストラクタが選択され、
コンパイルも単体テストもパスする。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 305
    auto const sa_init = Nstd::SafeArray2<int, 3>{10, 20, 30.0};  // 30.0はintに縮小型変換される

    ASSERT_TRUE(sa_init.InitializedWithNarrowConv());  // 縮小型変換あり
    ASSERT_EQ(3, sa_init.size());
    ASSERT_EQ(10, sa_init[0]);
    ASSERT_EQ(20, sa_init[1]);
    ASSERT_EQ(30, sa_init[2]);
    ASSERT_THROW(sa_init[3], std::out_of_range);
```

ここで紹介した2つのコンストラクタテンプレートの最後のパラメータには、かなりの違和感があるだろうが、
引数や戻り値に制限の多いコンストラクタテンプレートでSFINAEを起こすためには、
このような記述が必要になる。

なお、2つ目のコンストラクタテンプレートの中で使用した下記のコードは、
パラメータパックで与えられた全引数をそれぞれにT型オブジェクトに変換するための記法である。

```cpp
    // @@@ example/template/safe_vector_ut.cpp 255

    base_type{T(args)...},  // 縮小型変換を抑止するため、T(args)が必要
```

これにより、std::array\<T, N>のstd::initializer_listによる初期化が縮小変換を検出しなくなる。


### Nstd::SafeIndexの開発 <a id="SS_3_4_2"></a>
「[安全なvector](template_meta_programming.md#SS_3_2_2)」、「[安全な配列型コンテナ](template_meta_programming.md#SS_3_2_3)」等の中で、

* Nstd::SafeVector
* Nstd::SafeString
* Nstd::SafeArray

を定義した。これらは少しだけランタイム速度を犠牲にすることで、
安全な(未定義動作を起こさない)インデックスアクセスを保障するため、
一般のソフトウェア開発にも有用であると思われるが、コードクローンして作ったため、
リファクタリングを行う必要がある。

まずは、Nstd::SafeVectorとNstd::SafeStringの統一を考える。

std::stringは、実際にはstd::basic_string\<char>のエイリアスであることに注目すれば、
Nstd::SafeStringの基底クラスはstd::basic_string\<char>であることがわかる。
この形式は、std::vector\<T>と同形であるため、
Nstd::SafeVectorとNstd::SafeStringの共通コードはテンプレートテンプレートパラメータ
(「[is_same_templの実装](template_meta_programming.md#SS_3_3_3_6)」参照)を使用し下記のように書ける。


```cpp
    // @@@ example/template/nstd_safe_index.h 8

    namespace Nstd {

    template <template <class...> class C, typename... Ts>
    struct SafeIndex : C<Ts...> {
        using C<Ts...>::C;

        using base_type = C<Ts...>;
        using size_type = typename base_type::size_type;

        typename base_type::reference       operator[](size_type i) { return this->at(i); }
        typename base_type::const_reference operator[](size_type i) const { return this->at(i); }
    };
    }  // namespace Nstd
```

このコードの使用例を兼ねた単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_safe_index_ut.cpp 8

    auto v_i = Nstd::SafeIndex<std::vector, int>{1, 2};

    static_assert(std::is_same_v<int&, decltype(v_i[0])>);
    static_assert(std::is_base_of_v<std::vector<int>, decltype(v_i)>);
    ASSERT_EQ(1, v_i[0]);
    ASSERT_EQ(2, v_i[1]);
    ASSERT_THROW(v_i[2], std::out_of_range);

    auto str = Nstd::SafeIndex<std::basic_string, char>{"123"};

    static_assert(std::is_same_v<char&, decltype(str[0])>);
    static_assert(std::is_base_of_v<std::string, decltype(str)>);
    ASSERT_EQ(3, str.size());
    ASSERT_EQ("123", str);
    ASSERT_THROW(str[3], std::out_of_range);
```

このままでは使いづらいので下記のようにエイリアスを使い、元のテンプレートと同じ名前を与える。

```cpp
    // @@@ example/template/nstd_safe_index.h 24

    namespace Nstd {

    template <typename T>
    using SafeVector = Nstd::SafeIndex<std::vector, T>;

    using SafeString = Nstd::SafeIndex<std::basic_string, char>;
    }  // namespace Nstd
```

このコードの単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_safe_index_ut.cpp 54

    auto v_i = Nstd::SafeVector<int>{1, 2};

    static_assert(std::is_same_v<int&, decltype(v_i[0])>);
    static_assert(std::is_base_of_v<std::vector<int>, decltype(v_i)>);
    ASSERT_EQ(1, v_i[0]);
    ASSERT_EQ(2, v_i[1]);
    ASSERT_THROW(v_i[2], std::out_of_range);

    auto str = Nstd::SafeString{"123"};

    static_assert(std::is_same_v<char&, decltype(str[0])>);
    static_assert(std::is_base_of_v<std::string, decltype(str)>);
    ASSERT_EQ(3, str.size());
    ASSERT_EQ("123", str);
    ASSERT_THROW(str[3], std::out_of_range);
```

これで、Nstd::SafeVectorとNstd::SafeStringは統一できたので、
Nstd::SafeIndexにNstd::SafeArrayの実装が取り込めれば、リファクタリングは終了となるが、
残念ながら、下記のコードはコンパイルできない。

```cpp
    // @@@ example/template/nstd_safe_index_ut.cpp 44

    // 下記のように書きたいが、パラメータパックは型と値を混在できないのでコンパイルエラー
    auto a_i = Nstd::SafeIndex<std::array, int, 5>{};
```

理由は、パラメータパックにはそのすべてに型を指定するか、そのすべてに値を指定しなければならず、
上記のコードのような型と値の混在が許されていないからである。

値を型に変換する[std::integral_constant](template_meta_programming.md#SS_3_3_1_2)を使用し、この問題を解決できる。
std::arrayから派生した下記のStdArrayLikeは、std::integral_constant::valueから値を取り出し、
基底クラスstd::arrayの第2テンプレートパラメータとする。
この仕組みにより、StdArrayLikeは、
Nstd::SafeIndexのテンプレートテンプレートパラメータとして使用できるようになる。

```cpp
    // @@@ example/template/nstd_safe_index.h 34

    namespace Nstd {
    namespace Inner_ {

    template <typename T, typename U>
    struct std_array_like : std::array<T, U::value> {
        using std::array<T, U::value>::array;

        template <typename... ARGS>
        std_array_like(ARGS... args) noexcept(std::is_nothrow_constructible_v<T, ARGS...>)
            : std::array<T, U::value>{args...}
        {
            static_assert(AreConvertibleV<T, ARGS...>, "arguemnt error");
        }
    };
    }  // namespace Inner_
    }  // namespace Nstd
```

まずは、このコードの使用例を兼ねた単体テストを下記に示す。

```cpp
    // @@@ example/template/nstd_safe_index_ut.cpp 134

    auto sal = Nstd::Inner_::std_array_like<int, std::integral_constant<size_t, 3>>{1, 2, 3};

    static_assert(std::is_nothrow_constructible_v<decltype(sal), int>);  // エクセプション無し生成
    static_assert(std::is_same_v<int&, decltype(sal[0])>);
    static_assert(std::is_base_of_v<std::array<int, 3>, decltype(sal)>);
    ASSERT_EQ(1, sal[0]);
    ASSERT_EQ(2, sal[1]);
    ASSERT_EQ(3, sal[2]);

    using T   = Nstd::Inner_::std_array_like<std::string, std::integral_constant<size_t, 3>>;
    auto sal2 = T{"1", "2", "3"};

    static_assert(!std::is_nothrow_constructible_v<std::string, char const*>);
    static_assert(!std::is_nothrow_constructible_v<T, char const*>);  // エクセプション有り生成
    static_assert(std::is_same_v<std::string&, decltype(sal2[0])>);
    static_assert(std::is_base_of_v<std::array<std::string, 3>, decltype(sal2)>);
    ASSERT_EQ("1", sal2[0]);
    ASSERT_EQ("2", sal2[1]);
    ASSERT_EQ("3", sal2[2]);
```

これを使えば、下記のような記述が可能となる。

```cpp
    // @@@ example/template/nstd_safe_index_ut.cpp 157

    using T2   = Nstd::SafeIndex<Nstd::Inner_::std_array_like, std::string,
                               std::integral_constant<size_t, 4>>;
    auto sal_s = T2{"1", "2", "3"};

    static_assert(!std::is_nothrow_constructible_v<T2, char const*>);  // エクセプション有り生成
    static_assert(std::is_same_v<std::string&, decltype(sal_s[0])>);
    static_assert(std::is_base_of_v<std::array<std::string, 4>, decltype(sal_s)>);
    ASSERT_EQ("1", sal_s[0]);
    ASSERT_EQ("2", sal_s[1]);
    ASSERT_EQ("3", sal_s[2]);
    ASSERT_EQ("", sal_s[3]);
    ASSERT_THROW(sal_s[4], std::out_of_range);
```

このままでは使いづらいのでNstd::SafeVector、Nstd::Stringと同様にエイリアスを使えば、
下記のようになる。

```cpp
    // @@@ example/template/nstd_safe_index.h 53

    namespace Nstd {

    template <typename T, size_t N>
    using SafeArray
        = Nstd::SafeIndex<Nstd::Inner_::std_array_like, T, std::integral_constant<size_t, N>>;
    }  // namespace Nstd
```

このコードの単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_safe_index_ut.cpp 89

    auto sal_s = Nstd::SafeArray<std::string, 4>{"1", "2", "3"};

    static_assert(std::is_same_v<std::string&, decltype(sal_s[0])>);
    static_assert(std::is_base_of_v<std::array<std::string, 4>, decltype(sal_s)>);
    ASSERT_EQ("1", sal_s[0]);
    ASSERT_EQ("2", sal_s[1]);
    ASSERT_EQ("3", sal_s[2]);
    ASSERT_EQ("", sal_s[3]);
    ASSERT_THROW(sal_s[4], std::out_of_range);
```

これにより、当初の目的であったコードクローンの除去が完了した。
この効果により、下記に示したような拡張もコードクローンせずに簡単に行えるようになった。

```cpp
    // @@@ example/template/nstd_safe_index.h 62

    namespace Nstd {

    using SafeStringU16 = Nstd::SafeIndex<std::basic_string, char16_t>;
    using SafeStringU32 = Nstd::SafeIndex<std::basic_string, char32_t>;
    }  // namespace Nstd
```

```cpp
    // @@@ example/template/nstd_safe_index_ut.cpp 112

    auto u16str = Nstd::SafeStringU16{u"あいうえお"};

    static_assert(std::is_same_v<char16_t&, decltype(u16str[0])>);
    static_assert(std::is_base_of_v<std::u16string, decltype(u16str)>);
    ASSERT_EQ(5, u16str.size());
    ASSERT_EQ(u"あいうえお", u16str);
    ASSERT_THROW(u16str[5], std::out_of_range);

    auto u32str = Nstd::SafeStringU32{U"かきくけこ"};

    static_assert(std::is_same_v<char32_t&, decltype(u32str[0])>);
    static_assert(std::is_base_of_v<std::u32string, decltype(u32str)>);
    ASSERT_EQ(5, u32str.size());
    ASSERT_EQ(U"かきくけこ", u32str);
    ASSERT_THROW(u32str[5], std::out_of_range);
```

### Nstd::SafeIndexのoperator\<\<の開発 <a id="SS_3_4_3"></a>
ここでは、Nstd::SafeIndexのoperator\<\<の開発を行う。

他のoperator\<\<との間で定義が曖昧にならないようにするためには、
テンプレートテンプレートパラメータを使って以下のようにすることが考えられる。

```cpp
    // @@@ example/template/safe_index_put_to_ut.cpp 8

    template <template <class...> class C, typename... Ts>
    std::ostream& operator<<(std::ostream& os, Nstd::SafeIndex<C, Ts...> const& safe_index)
    {
        auto first = true;

        for (auto const& i : safe_index) {
            if (!std::exchange(first, false)) {
                os << ", ";
            }
            os << i;
        }

        return os;
    }
```

以下の単体テストで動作確認する。

```cpp
    // @@@ example/template/safe_index_put_to_ut.cpp 28
    {
        auto v_i = Nstd::SafeVector<int>{1, 2};

        auto oss = std::ostringstream{};
        oss << v_i;
        ASSERT_EQ("1, 2", oss.str());
    }
    {
        auto sal_s = Nstd::SafeArray<std::string, 4>{"1", "2", "3"};
        auto oss   = std::ostringstream{};
        oss << sal_s;
        ASSERT_EQ("1, 2, 3, ", oss.str());  // 4番目には何も入っていない
    }
```

ここまではうまく行っているが、以下の単体テストによりバグが発覚する。

```cpp
    // @@@ example/template/safe_index_put_to_ut.cpp 43

    {
        auto s_str = Nstd::SafeString{"hello"};
        auto oss   = std::ostringstream{};
        oss << s_str;

        // ASSERT_EQ("hello", oss.s_str());     // これがパス出来たらよいが、
        ASSERT_EQ("h, e, l, l, o", oss.str());  // 実際にはこのようになる。
    }
    {
        auto str = std::string{"hello"};  // 上記と比較のためのstd::stringでのoperator<<

        auto oss = std::ostringstream{};
        oss << str;
        ASSERT_EQ("hello", oss.str());
    }
```

この原因は、Nstd::SafeStringオブジェクトに対して、std::operator<<が使用されなかったからである。

「[メタ関数のテクニック](template_meta_programming.md#SS_3_3)」で紹介したSFINAEにより、この問題は下記のように対処できる。

```cpp
    // @@@ example/template/safe_index_put_to_ut.cpp 102

    template <template <class...> class C, typename... Ts>
    auto operator<<(std::ostream& os, Nstd::SafeIndex<C, Ts...> const& safe_index) ->
        typename std::enable_if_t<    // safe_indexがSafeString型ならば、SFINAEにより非活性化
            !std::is_same_v<Nstd::SafeIndex<C, Ts...>, Nstd::SafeString>, std::ostream&>
    {
        auto first = true;

        for (auto const& i : safe_index) {
            if (!std::exchange(first, false)) {
                os << ", ";
            }
            os << i;
        }

        return os;
    }
```

これにより先ほど問題が発生した単体テストも下記のようにパスする。

```cpp
    // @@@ example/template/safe_index_put_to_ut.cpp 138

    auto str = Nstd::SafeString{"hello"};
    auto oss = std::ostringstream{};
    oss << str;
    ASSERT_EQ("hello", oss.str());  // std::operator<<が使われる
    // ASSERT_EQ("h, e, l, l, o", oss.str());
```


### コンテナ用Nstd::operator\<\<の開発 <a id="SS_3_4_4"></a>
「[Nstd::SafeIndexのoperator\<\<の開発](template_meta_programming.md#SS_3_4_3)」で定義したNstd::operator\<\<の構造は、
範囲for文に適用できる配列やstdコンテナにも使えるため、ここではその拡張を考える。

すでに述べたように注意すべきは、

* 使い勝手の良いstd::operator\<\<(例えばchar[N]やstd::stringのoperator\<\<)はそのまま使う
* ほとんど使い物にならないstd::operator\<\<(例えば、int[N]のような配列に対するoperator\<\<(void\*))
  の代わりに、ここで拡張するNstd::operator\<\<を使う

であるため、型Tが新しいNstd::operator\<\<を使用できる条件は、

* Tの型が、以下の条件を満たす
    * T == U[N]であった場合、Uはcharではない
    * std::stringおよびその派生型ではない
* Nstd::ValueType\<T>::typeがoperator\<\<を持つ

となるだろう。この条件を診断するためのメタ関数は以下のようになる。

```cpp
    // @@@ example/template/nstd_put_to.h 16

    namespace Nstd {
    namespace Inner_ {

    template <typename T>
    constexpr bool enable_range_put_to() noexcept
    {
        if constexpr (Nstd::ValueType<T>::IsBuiltinArray) {  // Tは配列
            if constexpr (std::is_same_v<char,
                                         typename Nstd::ValueType<T>::type_direct>) {  // Tはchar配列
                return false;
            }
            else {
                return Nstd::ExistsPutToV<typename Nstd::ValueTypeT<T>>;
            }
        }
        else {  // Tは配列ではない
    #if defined(__clang__)
            if constexpr (Nstd::ExistsPutToV<T>) {  // operator<<を持つ(std::string等)
    #else                                           // g++でのワークアラウンド
            if (Nstd::ExistsPutToV<T>) {  // operator<<を持つ(std::string等)
    #endif
                return false;
            }
            else {
                if constexpr (Nstd::IsRangeV<T>) {  // 範囲for文に適用できる
                    return Nstd::ExistsPutToV<typename Nstd::ValueTypeT<T>>;
                }
                else {
                    return false;
                }
            }
        }
    }

    template <typename T>
    constexpr bool enable_range_put_to_v{enable_range_put_to<T>()};
    }  // namespace Inner_
    }  // namespace Nstd
```

ただし、このようなコードはコンパイラのバグによりコンパイルできないことがある。
実際、現在使用中の[g++](term_explanation.md#SS_5_9_1)ではこのコードはコンパイルできず、
上記コードではそのワークアラウンドを行っている。

このような場合、条件分岐に三項演算子を使うことで回避できることが多いが、
ここではg++の問題を明示するためにプリプロセッサ命令を用いた。

このような複雑なメタ関数には単体テストは必須である。

```cpp
    // @@@ example/template/test_class.h 3

    class test_class_exits_put_to {
    public:
        test_class_exits_put_to(int i = 0) noexcept : i_{i} {}
        int get() const noexcept { return i_; }

    private:
        int i_;
    };

    inline std::ostream& operator<<(std::ostream& os, test_class_exits_put_to const& p)
    {
        return os << p.get();
    }

    class test_class_not_exits_put_to {};
```

```cpp
    // @@@ example/template/nstd_put_to_ut.cpp 31

    static_assert(enable_range_put_to_v<int[3]>);                         // Nstd::operator<<
    static_assert(!enable_range_put_to_v<char[3]>);                       // std::operator<<
    static_assert(!enable_range_put_to_v<int>);                           // std::operator<<
    static_assert(enable_range_put_to_v<std::vector<int>>);               // Nstd::operator<<
    static_assert(enable_range_put_to_v<std::vector<std::vector<int>>>);  // Nstd::operator<<
    static_assert(!enable_range_put_to_v<std::string>);                   // std::operator<<
    static_assert(enable_range_put_to_v<std::vector<std::string>>);       // Nstd::operator<<

    static_assert(!enable_range_put_to_v<test_class_not_exits_put_to>);  // operator<<無し
    static_assert(!enable_range_put_to_v<test_class_exits_put_to>);  // ユーザ定義operator<<
    static_assert(
        !enable_range_put_to_v<std::vector<test_class_not_exits_put_to>>);       // operator<<無し
    static_assert(enable_range_put_to_v<std::vector<test_class_exits_put_to>>);  // Nstd::operator<<
    static_assert(
        !enable_range_put_to_v<std::list<test_class_not_exits_put_to>>);       // operator<<無し
    static_assert(enable_range_put_to_v<std::list<test_class_exits_put_to>>);  // Nstd::operator<<
```

以上によりstd::enable_ifの第1引数に渡す値(enable_range_put_to_vはconstexpr)が用意できたので、
Nstd::operator\<\<は下記のように定義できる。

```cpp
    // @@@ example/template/nstd_put_to.h 58

    namespace Nstd {
    namespace Inner_ {

    template <size_t N>
    constexpr std::string_view range_put_to_sep() noexcept
    {
        static_assert(N != 0);
        switch (N) {
        case 1:
            return ", ";
        case 2:
            return " | ";
        case 3:
        default:
            return " # ";
        }
    };
    }  // namespace Inner_

    template <typename T>
    auto operator<<(std::ostream& os, T const& t) ->
    #if defined(__clang__)
        typename std::enable_if_t<Inner_::enable_range_put_to_v<T>, std::ostream&>
    #else  // g++でのワークアラウンド
        typename std::enable_if_t<Inner_::enable_range_put_to<T>(), std::ostream&>
    #endif
    {
        auto           first = true;
        constexpr auto s     = Inner_::range_put_to_sep<ValueType<T>::Nest>();

        for (auto const& i : t) {
            if (!std::exchange(first, false)) {
                os << s;
            }
            os << i;
        }

        return os;
    }
    }  // namespace Nstd
```

値表示用のセパレータに","のみを用いるとコンテナや配列が多次元(ValueType::Nest > 2)の場合、
各次元でのデータの判別が難しくなるため、ValueType::Nestの値によってセパレータの種類を変える
range_put_to_sep<>()を用意した。
下記単体テストでわかる通り、この効果により値の構造が見やすくなっている。

まずは、配列の単体テストを示す。

```cpp
    // @@@ example/template/nstd_put_to_ut.cpp 61

    using namespace Nstd;
    {
        char str[] = "abcdef";
        auto oss   = std::ostringstream{};

        oss << str;  // std::operator<<
        ASSERT_EQ(str, oss.str());
    }
    {
        char str[2][4] = {"abc", "def"};
        auto oss       = std::ostringstream{};

        oss << str;  // Nstd::operator<<
        ASSERT_EQ("abc | def", oss.str());
    }
    {
        test_class_exits_put_to p1[3]{1, 2, 3};
        auto                    oss = std::ostringstream{};

        oss << p1;  // Nstd::operator<<
        ASSERT_EQ("1, 2, 3", oss.str());
    }
    {
        char const* str[] = {"abc", "def", "ghi"};
        auto        oss   = std::ostringstream{};

        oss << str;  // Nstd::operator<<
        ASSERT_EQ("abc, def, ghi", oss.str());
    }
    {
        int  v[2][3][2]{{{0, 1}, {2, 3}, {4, 5}}, {{6, 7}, {8, 9}, {10, 11}}};
        auto oss = std::ostringstream{};

        oss << v;
        ASSERT_EQ("0, 1 | 2, 3 | 4, 5 # 6, 7 | 8, 9 | 10, 11", oss.str());
    }
```

次に、コンテナの単体テストを示す。

```cpp
    // @@@ example/template/nstd_put_to_ut.cpp 118

    using namespace Nstd;
    {
        auto v   = std::vector<int>{1, 2, 3};
        auto oss = std::ostringstream{};

        oss << v;
        ASSERT_EQ("1, 2, 3", oss.str());
    }
    {
        auto p1  = std::list<test_class_exits_put_to>{1, 2, 3, 4};
        auto oss = std::ostringstream{};

        oss << p1;
        ASSERT_EQ("1, 2, 3, 4", oss.str());
    }
    {
        std::vector<int> v[2]{{1, 2}, {3, 4, 5}};  // std::vectorの配列
        auto             oss = std::ostringstream{};

        oss << v;
        ASSERT_EQ("1, 2 | 3, 4, 5", oss.str());
    }
```

最後に、Nstd::SafeIndexの単体テストを示す。

```cpp
    // @@@ example/template/nstd_put_to_ut.cpp 168

    {
        auto sal_s = Nstd::SafeArray<std::string, 4>{"1", "2", "3"};
        auto oss   = std::ostringstream{};

        oss << sal_s;
        ASSERT_EQ("1 | 2 | 3 | ", oss.str());
    }
    {
        auto sv
            = Nstd::SafeVector<Nstd::SafeArray<Nstd::SafeString, 2>>{{"ab", "cd"}, {"ef", "gh"}};
        auto oss = std::ostringstream{};

        oss << sv;
        ASSERT_EQ("ab | cd # ef | gh", oss.str());
    }
```

## ログ取得ライブラリの開発2 <a id="SS_3_5"></a>
ログ取得ライブラリでの問題は「Logging名前空間が依存してよい名前空間」に

```cpp
    // @@@ example/template/app_ints.h 6

    namespace App {
    using Ints_t = std::vector<int>;
    }
```

のようなコンテナに共通したoperator<<を定義することで解決する。
それは 「[コンテナ用Nstd::operator\<\<の開発](template_meta_programming.md#SS_3_4_4)」で示したコードそのものであるため、
これを使い、問題を解決したログ取得ライブラリを以下に示す。

```cpp
    // @@@ example/template/logger.h 7

    namespace Logging {
    class Logger {
    public:
        static Logger&       Inst();
        static Logger const& InstConst() { return Inst(); }

        std::string Get() const;  // ログデータの取得
        void        Clear();      // ログデータの消去

        template <typename... ARGS>  // ログの登録
        void Set(char const* filename, uint32_t line_no, ARGS const&... args)
        {
            oss_.width(32);
            oss_ << filename << ":";

            oss_.width(3);
            oss_ << line_no;

            set_inner(args...);
        }

        Logger(Logger const&)            = delete;
        Logger& operator=(Logger const&) = delete;

    private:
        void set_inner() { oss_ << std::endl; }

        template <typename HEAD, typename... TAIL>
        void set_inner(HEAD const& head, TAIL const&... tails)
        {
            using Nstd::operator<<;  // Nstd::operator<<もname lookupの対象にする

            oss_ << ":" << head;
            set_inner(tails...);
        }

        Logger() {}
        std::ostringstream oss_{};
    };
    }  // namespace Logging

    #define LOGGER_P(...) Logging::Logger::Inst().Set(__FILE__, __LINE__)
    #define LOGGER(...) Logging::Logger::Inst().Set(__FILE__, __LINE__, __VA_ARGS__)
```

問題のあったコードとの差分は、メンバ関数テンプレートset_innerの

```cpp
    // @@@ example/template/logger.h 40

    using Nstd::operator<<;  // Nstd::operator<<もname lookupの対象にする
```

のみである。実際に解決できたことを以下の単体テストで示す。

```cpp
    // @@@ example/template/logger_0_ints_ut.h 8

    auto ints = App::Ints_t{1, 2, 3};

    LOGGER("Ints", ints);
    auto line_num = __LINE__;

    auto s = Logging::Logger::InstConst().Get();

    auto exp = std::string{log_str_exp(__FILE__, line_num - 1, ":Ints:1, 2, 3\n")};
    ASSERT_EQ(exp, s);
```

また、

```cpp
    // @@@ example/template/app_ints.h 12

    namespace App {

    class X {
    public:
        X(std::string str, int num) : str_{std::move(str)}, num_{num} {}
        std::string ToString() const { return str_ + "/" + std::to_string(num_); }
        ...
    };
    }  // namespace App
```

のように定義されたクラスも、

```cpp
    // @@@ example/template/app_ints.h 28

    namespace App {

    inline std::ostream& operator<<(std::ostream& os, X const& x) { return os << x.ToString(); }
    }  // namespace App
```

のような型専用のoperator<<があれば、そのオブジェクトのみではなく、
コンテナや配列に対しても下記のようにログ取得が可能となる。

```cpp
    // @@@ example/template/logger_ut.cpp 37

    using namespace Nstd;

    auto         x  = App::X{"name", 3};
    auto         lx = std::list<App::X>{{"lx3", 3}, {"lx4", 1}};
    App::X const x3[3]{{"x0", 0}, {"x1", 1}, {"x2", 2}};

    LOGGER(1, x, x3, lx);
    auto line_num = __LINE__;

    auto s = Logging::Logger::InstConst().Get();

    auto const exp
        = log_str_exp(__FILE__, line_num - 1, ":1:name/3:x0/0, x1/1, x2/2:lx3/3, lx4/1\n");
    ASSERT_EQ(exp, s);
```

「[Nstdライブラリの開発](template_meta_programming.md#SS_3_2)」で示した依存関係も維持されており、
これでログ取得ライブラリは完成したと言って良いだろう。


## その他のテンプレートテクニック <a id="SS_3_6"></a>
ここでは、これまでの議論の対象にならなかったテンプレートのテクニックや注意点について記述する。

### ユニバーサルリファレンスとstd::forward <a id="SS_3_6_1"></a>
2個の文字列からstd::vector\<std::string>を生成する下記のような関数について考える。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 9

    std::vector<std::string> gen_vector(std::string const& s0, std::string const& s1)
    {
        auto ret = std::vector<std::string>{};

        ret.push_back(s0);
        ret.push_back(s1);

        return ret;
    }
```

これは下記のように動作する。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 25

    auto a = std::string{"a"};
    auto b = std::string{"b"};

    auto v = gen_vector(a, std::move(b));

    ASSERT_EQ((std::vector<std::string>{"a", "b"}), v);
    ASSERT_EQ("a", a);
    ASSERT_EQ("b", b);  // bはmoveされない
```

このコードは正しく動作するものの、move代入できず、パフォーマンス問題を引き起こす可能性があるため、
[ユニバーサルリファレンス](term_explanation.md#SS_5_6_1)を使って下記のように書き直した。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 41

    template <typename STR0, typename STR1>
    std::vector<std::string> gen_vector(STR0&& s0, STR1&& s1)
    {
        auto ret = std::vector<std::string>{};

        ret.push_back(s0);
        ret.push_back(s1);

        return ret;
    }
```

残念ながら、このコードは意図したようには動作せず、下記に示した通り相変わらずmove代入ができない。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 58

    auto a = std::string{"a"};
    auto b = std::string{"b"};

    auto v = gen_vector(a, std::move(b));

    ASSERT_EQ((std::vector<std::string>{"a", "b"}), v);
    ASSERT_EQ("a", a);
    ASSERT_EQ("b", b);  // bはmoveされない
```

この原因は、
「関数が受け取った[rvalue](term_explanation.md#SS_5_5_3)リファレンスは、
その関数から別の関数に受け渡される時に[lvalue](term_explanation.md#SS_5_5_2)リファレンスとして扱われる」からである。

この現象について下記の関数テンプレートを用いて解説を行う。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 71

    enum class ExpressionType { Lvalue, Rvalue };

    template <typename T>
    constexpr ExpressionType universal_ref2(T&& t)
    {
        return std::is_lvalue_reference_v<decltype(t)> ? ExpressionType::Lvalue
                                                       : ExpressionType::Rvalue;
    }

    // std::pair<>::first  : universal_refの中のtのExpressionType
    // std::pair<>::second : universal_ref2の中でtのExpressionType
    template <typename T>
    constexpr std::pair<ExpressionType, ExpressionType> universal_ref(T&& t)
    {
        return std::make_pair(
            std::is_lvalue_reference_v<decltype(t)> ? ExpressionType::Lvalue : ExpressionType::Rvalue,
            universal_ref2(t));
    }
```

下記に示した通り、universal_refとuniversal_ref2のパラメータが同じ型であるとは限らない。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 95

    auto i = 0;

    constexpr auto p = universal_ref(i);

    static_assert(universal_ref2(i) == ExpressionType::Lvalue);
    static_assert(p.first == ExpressionType::Lvalue);
    static_assert(p.second == ExpressionType::Lvalue);

    constexpr auto pm = universal_ref(std::move(i));

    static_assert(universal_ref2(std::move(i)) == ExpressionType::Rvalue);
    static_assert(pm.first == ExpressionType::Rvalue);
    static_assert(pm.second == ExpressionType::Lvalue);

    constexpr auto pm2 = universal_ref(int{});

    static_assert(universal_ref2(int{}) == ExpressionType::Rvalue);
    static_assert(pm2.first == ExpressionType::Rvalue);
    static_assert(pm2.second == ExpressionType::Lvalue);
```

この問題はstd::forwardにより対処できる。これによって改良されたコードを下記に示す。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 124

    template <typename STR0, typename STR1>
    std::vector<std::string> gen_vector(STR0&& s0, STR1&& s1)
    {
        auto ret = std::vector<std::string>{};

        ret.push_back(std::forward<STR0>(s0));
        ret.push_back(std::forward<STR1>(s1));

        return ret;
    }
```

下記単体テストが示す通り、rvalueリファレンスはmove代入され、lvalueリファレンスはcopy代入されている。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 142

    auto a = std::string{"a"};
    auto b = std::string{"b"};

    auto v = gen_vector(a, std::move(b));

    ASSERT_EQ((std::vector<std::string>{"a", "b"}), v);
    ASSERT_EQ("a", a);
    ASSERT_EQ("", b);  // bはmoveされた
```

しかし残念ながら、このコードにも改良すべき点がある。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 155

    auto a = std::string{"a"};

    auto v = gen_vector(a, "b");

    ASSERT_EQ((std::vector<std::string>{"a", "b"}), v);
    ASSERT_EQ("a", a);
```

上記の"b"のような文字列リテラルを引数にした場合、
それをstd::vector\<std::string>::push_backに渡した時に、
"b"はテンポラリオブジェクトstd::string("b")のに変換されてしまう。
std::vector\<std::string>へのオブジェクトの挿入は、文字列リテラルから行うことが出来るため、
このテンポラリオブジェクトの生成は明らかに不要な処理である。

下記は、この対策を施すとともに任意の数の引数を受け取れるように改良したコードである。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 170

    void emplace_back(std::vector<std::string>&) noexcept {}

    template <typename HEAD, typename... TAIL>
    void emplace_back(std::vector<std::string>& strs, HEAD&& head, TAIL&&... tails)
    {
        strs.emplace_back(std::forward<HEAD>(head));

        if constexpr (sizeof...(tails) != 0) {
            emplace_back(strs, std::forward<TAIL>(tails)...);
        }
    }

    template <typename... STR>
    std::vector<std::string> gen_vector(STR&&... ss)
    {
        auto ret = std::vector<std::string>{};

        emplace_back(ret, std::forward<STR>(ss)...);

        return ret;
    }
```

上記の

```cpp
    sizeof...(tails)
```

はパラメータパックの個数を受け取るための記法である。
従ってこのコードではすべてのパラメータパック変数を消費するまでリカーシブコールを続けることになる
(が、このリカーシブコールはコンパイル時に行われるため、実行時の速度低下は起こさない)。

上記の

```cpp
    std::forward<TAIL>(tails)...
```

は、それぞれのパラメータパック変数をstd::forwardに渡した戻り値を、
再びパラメータパックにするための記法である。

このコードは下記の単体テストが示すように正しく動作する
(が、残念ならがテンポラリオブジェクトが生成されていないことを単体テストで証明することはできない)。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 198

    auto a = std::string{"a"};
    auto b = std::string{"b"};

    auto v = gen_vector(a, std::move(b), "c");

    ASSERT_EQ((std::vector<std::string>{"a", "b", "c"}), v);
    ASSERT_EQ("a", a);
    ASSERT_EQ("", b);  // bはmoveされた
```

ユニバーサルリファレンスはconstにすることができないが
(T const&&はconstな[rvalue](term_explanation.md#SS_5_5_3)リファレンスである)、
ユニバーサルリファレンスが[lvalue](term_explanation.md#SS_5_5_2)リファレンスであった場合は、
constなlvalueリファレンスとして扱うべきである。

従って、下記のようなコードは書くべきではない。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 215

    template <typename STR0, typename STR1>
    std::vector<std::string> gen_vector(STR0&& s0, STR1&& s1)
    {
        auto ret = std::vector<std::string>{};

        ret.push_back(std::move(s0));
        ret.push_back(std::move(s1));

        return ret;
    }
```

もしそのようにしてしまえば、
下記単体テストが示すように非constな実引数はmoveされてしまうことになる。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 232

    auto       a = std::string{"a"};
    auto const b = std::string{"b"};

    auto v = gen_vector(a, std::move(b));

    ASSERT_EQ((std::vector<std::string>{"a", "b"}), v);
    ASSERT_EQ("", a);   // aはmoveされてしまう
    ASSERT_EQ("b", b);  // bはconstなのでmoveされない
```

任意の型Tのrvalueのみを引数に取る関数テンプレートを下記のように記述した場合、
すでに述べたように引数はユニバーサルリファレンスとなってしまうため、lvalueにもバインドしてしまう。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 248

    template <typename T>
    void f(T&& t) noexcept
    {
        ...
    }
```

このような場合、下記の記述が必要になる。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 267

    template <typename T>
    void f(T&) = delete;
```

この効果により、下記に示した通りlvalueにはバインドできなくなり、
当初の目的通り、rvalueのみを引数に取る関数テンプレートが定義できたことになる。

```cpp
    // @@@ example/template/universal_ref_ut.cpp 275

    auto s = std::string{};

    // f(s);           // f(std::string&)はdeleteされたため、コンパイルエラー
    f(std::string{});  // f(std::string&&)にはバインドできる
```

なお、ユニバーサルリファレンスは、[リファレンスcollapsing](term_explanation.md#SS_5_6_4)の一機能としても理解できる。

### ジェネリックラムダ <a id="SS_3_6_2"></a>
下記のようなクラスとoperator<<があった場合を考える。

```cpp
    // @@@ example/template/generic_lambda_ut.cpp 13

    struct XYZ {
        XYZ(int ax, int ay, int az) noexcept : x{ax}, y{ay}, z{az} {}
        int x;
        int y;
        int z;
    };

    std::ostream& operator<<(std::ostream& os, XYZ const& xyz)
    {
        return os << std::to_string(xyz.x) + "/" + std::to_string(xyz.y) + "/" + std::to_string(xyz.z);
    }
```

「[Nstd::SafeIndexの開発](template_meta_programming.md#SS_3_4_2)」や「[コンテナ用Nstd::operator\<\<の開発](template_meta_programming.md#SS_3_4_4)」
の成果物との組み合わせの単体テストは下記のように書けるだろう。

```cpp
    // @@@ example/template/generic_lambda_ut.cpp 31

    auto v   = Nstd::SafeVector<XYZ>{{9, 8, 7}, {6, 5, 4}, {3, 2, 1}, {0, 1, 2}};
    auto oss = std::ostringstream{};

    oss << v;
    ASSERT_EQ("9/8/7, 6/5/4, 3/2/1, 0/1/2", oss.str());
```

std::sortによるソートができるかどうかのテストは、C++11までは、

```cpp
    // @@@ example/template/generic_lambda_ut.cpp 41

    auto v = Nstd::SafeVector<XYZ>{{9, 8, 7}, {6, 5, 4}, {3, 2, 1}, {0, 1, 2}};

    // C++11 lambda                  型の明示が必要
    //                               ↓               ↓
    std::sort(v.begin(), v.end(), [](XYZ const& lhs, XYZ const& rhs) noexcept {
        return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
    });
    auto oss = std::ostringstream{};

    oss << v;
    ASSERT_EQ("0/1/2, 3/2/1, 6/5/4, 9/8/7", oss.str());
```

のように書くのが一般的だろう。ラムダ式の引数の型を指定しなければならないのは、
範囲for文でautoが使用出来ること等と比べると見劣りがするが、
C++14からは下記のコードで示した通り引数にautoが使えるようになった。

```cpp
    // @@@ example/template/generic_lambda_ut.cpp 57

    auto v = Nstd::SafeVector<XYZ>{{9, 8, 7}, {6, 5, 4}, {3, 2, 1}, {0, 1, 2}};

    // C++14 generic lambda          型の明示が不要
    //                               ↓                ↓
    std::sort(v.begin(), v.end(), [](auto const& lhs, auto const& rhs) noexcept {
        return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
    });
    auto oss = std::ostringstream{};

    oss << v;
    ASSERT_EQ("0/1/2, 3/2/1, 6/5/4, 9/8/7", oss.str());
```

この記法はジェネリックラムダと呼ばれる。
この機能により関数の中で関数テンプレートと同等のものが定義できるようになった。

#### ジェネリックラムダの内部構造 <a id="SS_3_6_2_1"></a>
ジェネリックラムダは下記のように使用することができる。

```cpp
    // @@@ example/template/generic_lambda_ut.cpp 73

    template <typename PUTTO>
    void f(PUTTO&& p)
    {
        p(1);       // ラムダの引数elemの型はint
        p(2.71);    // ラムダの引数elemは型はdouble
        p("hehe");  // ラムダの引数elemは型はchar [5]
    }

    TEST(Template, generic_lambda)
    {
        auto oss = std::ostringstream{};

        f([&oss](auto const& elem) { oss << elem << std::endl; });

        ASSERT_EQ("1\n2.71\nhehe\n", oss.str());
    }
```

この例で使用しているクロージャは一見、型をダイナミックに扱っているように見えるが、
下記のような「テンプレートoperator()を持つ関数型」オブジェクトとして展開されていると考えれば、
理解できる。

```cpp
    // @@@ example/template/generic_lambda_ut.cpp 92

    class Closure {
    public:
        Closure(std::ostream& os) : os_{os} {}

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
        auto oss = std::ostringstream{};

        auto closure = Closure{oss};
        f(closure);

        ASSERT_EQ("1\n2.71\nhehe\n", oss.str());
    }
```

#### std::variantとジェネリックラムダ <a id="SS_3_6_2_2"></a>
unionは、オブジェクトを全く無関係な複数の型に切り替えることができるため、
これが必要な場面では有用な機能であるが、未定義動作を誘発してしまう問題がある。
この対策としてC++17で導入されたものが、std::variantである。

まずは、std::variantの使用例を下記する。

```cpp
    // @@@ example/template/variant_ut.cpp 13

    auto v = std::variant<int, std::string, double>{};  // 3つの型を切り替える

    // std::get<N>()の戻り値型は、下記の通りで、
    // N == 0, 1, 2 は、それぞれint, std::string, doubleに対応
    static_assert(std::is_same_v<decltype(std::get<0>(v)), int&>);
    static_assert(std::is_same_v<decltype(std::get<1>(v)), std::string&>);
    static_assert(std::is_same_v<decltype(std::get<2>(v)), double&>);

    v = int{3};  // int型の3を代入

    ASSERT_EQ(v.index(), 0);                                // intを保持
    ASSERT_EQ(std::get<0>(v), 3);                           // intなので問題なくアクセス
    ASSERT_THROW(std::get<1>(v), std::bad_variant_access);  // std::stringではないのでエクセプション
    ASSERT_THROW(std::get<2>(v), std::bad_variant_access);  // doubleではないのでエクセプション

    v = std::string{"str"};  // std::stringオブジェクトを代入

    ASSERT_EQ(v.index(), 1);                                // std::stringを保持
    ASSERT_THROW(std::get<0>(v), std::bad_variant_access);  // intではないのでエクセプション
    ASSERT_EQ(std::get<1>(v), std::string{"str"});  // std::stringなので問題なくアクセス
    ASSERT_THROW(std::get<2>(v), std::bad_variant_access);  // doubleではないのでエクセプション
```

上記からわかる通り、std::variantオブジェクトは、直前に代入されたオブジェクトの型以外で、
値を読み出した場合、問題なく読み出せるが、それ以外ではエクセプションを発生させる。

このstd::variantオブジェクトの保持する型とその値を文字列として取り出すラムダ式は、
下記のように書ける。

```cpp
    // @@@ example/template/variant_ut.cpp 37

    auto oss = std::ostringstream{};

    // type_valueはvが保持する型をその値を文字列で返す
    auto type_value = [&oss](auto const& v) {  // ジェネリックラムダでなくても実装可能
        if (v.index() == 0) {
            auto a  = std::get<0>(v);
            using T = std::remove_const_t<std::remove_reference_t<decltype(a)>>;
            oss << Nstd::Type2Str<T>() << " : " << a;  // Nstd::Type2Str<T>はTの型を文字列にする
        }
        else if (v.index() == 1) {
            auto a  = std::get<1>(v);
            using T = std::remove_const_t<std::remove_reference_t<decltype(a)>>;
            oss << Nstd::Type2Str<T>() << " : " << a;  // Nstd::Type2Str<T>はTの型を文字列にする
        }
        else if (v.index() == 2) {
            auto a  = std::get<2>(v);
            using T = std::remove_const_t<std::remove_reference_t<decltype(a)>>;
            oss << Nstd::Type2Str<T>() << " : " << a;  // Nstd::Type2Str<T>はTの型を文字列にする
        }
        else {
            assert(false);  // ここには来ないはず
        }
    };

    v = 3;  // int型の3を代入
    type_value(v);
    ASSERT_EQ("int : 3", oss.str());
    oss = std::ostringstream{};  // ossのリセット

    v = std::string{"str"};  // std::stringオブジェクトを代入
    type_value(v);
    ASSERT_EQ("std::string : str", oss.str());
    oss = std::ostringstream{};  // ossのリセット

    v = 1.1;  // double型の1.1を代入
    type_value(v);
    ASSERT_EQ("double : 1.1", oss.str());
```

このラムダは、
3つの型をテンプレートパラメータとするstd::variantオブジェクト以外には適用できないため、
型の個数に制限のない方法を考える。

この実装は、

* 保持する型が何番目かを見つけるための関数テンプレート
* 関数テンプレートの引数となるジェネリックラムダ

の2つによって下記のように行うことができる。

```cpp
    // @@@ example/template/variant_ut.cpp 79

    template <typename VARIANT, typename F, size_t INDEX = 0>
    void org_visit(const F& f, const VARIANT& v)
    {
        constexpr auto n = std::variant_size_v<VARIANT>;

        if constexpr (INDEX < n) {
            if (v.index() == INDEX) {  // 保持する型が見つかった
                f(std::get<INDEX>(v));
                return;
            }
            else {  // 保持する型が見つかるまでリカーシブ
                org_visit<VARIANT, F, INDEX + 1>(f, v);
            }
        }
        else {
            assert(false);  // ここには来ないはず
        }
    }
```
```cpp
    // @@@ example/template/variant_ut.cpp 103

    auto oss = std::ostringstream{};

    // 文字列を返すためのジェネリックラムダ
    auto type_value = [&oss](auto const& a) {
        using T = std::remove_const_t<std::remove_reference_t<decltype(a)>>;
        oss << Nstd::Type2Str<T>() << " : " << a;
    };
```

単体テストは、以下のようになる。

```cpp
    // @@@ example/template/variant_ut.cpp 113
    {
        auto v = std::variant<int, std::string, double>{};  // 3つの型を切り替える

        v = 3;
        org_visit(type_value, v);
        ASSERT_EQ("int : 3", oss.str());
        oss = std::ostringstream{};  // ossのリセット

        ...
    }
    {
        auto v = std::variant<char, int, std::string, double>{};  // 4つの型を切り替える

        v = 3;
        org_visit(type_value, v);
        ASSERT_EQ("int : 3", oss.str());
        oss = std::ostringstream{};  // ossのリセット

        v = 'c';
        org_visit(type_value, v);
        ASSERT_EQ("char : c", oss.str());
        oss = std::ostringstream{};  // ossのリセット

        ...
    }
```

下記のように継承関係のない複数のクラスが同じシグネチャのメンバ関数を持つ場合、

```cpp
    // @@@ example/template/variant_ut.cpp 177

    class A {
    public:
        char f() const noexcept { return 'A'; }
    };

    class B {
    public:
        char f() const noexcept { return 'B'; }
    };

    class C {
    public:
        char f() const noexcept { return 'C'; }
    };
```

std::variant、上に示した関数テンプレート、ジェネリックラムダを使い、
下記に示したような疑似的なポリモーフィズムを実現できる。

```cpp
    // @@@ example/template/variant_ut.cpp 197

    char ret{};
    auto call_f = [&ret](auto const& a) { ret = a.f(); };

    auto v = std::variant<A, B, C>{};

    org_visit(call_f, v);  // 疑似的なポリモーフィズム
    ASSERT_EQ('A', ret);

    v = B{};
    org_visit(call_f, v);  // 疑似的なポリモーフィズム
    ASSERT_EQ('B', ret);

    v = C{};
    org_visit(call_f, v);  // 疑似的なポリモーフィズム
    ASSERT_EQ('C', ret);
```

ここで示した関数テンプレートは、デザインパターン[Visitor](design_pattern.md#SS_2_20)の例であり、
ほぼこれと同様のものがstd::visitとして定義されている。

```cpp
    // @@@ example/template/variant_ut.cpp 215

    v = A{};
    std::visit(call_f, v);  // 疑似的なポリモーフィズム
    ASSERT_EQ('A', ret);

    v = B{};
    std::visit(call_f, v);  // 疑似的なポリモーフィズム
    ASSERT_EQ('B', ret);

    v = C{};
    std::visit(call_f, v);  // 疑似的なポリモーフィズム
    ASSERT_EQ('C', ret);
```

### クラステンプレートと継承の再帰構造 <a id="SS_3_6_3"></a>
クラステンプレートと継承の再帰構造はCRTPと呼ばれる。
このコードパターンについては、
「[CRTP(curiously recurring template pattern)](design_pattern.md#SS_2_21)」で説明している。

### constexpr if文 <a id="SS_3_6_4"></a>
C++17で導入された[constexpr if文](https://cpprefjp.github.io/lang/cpp17/if_constexpr.html)とは、
文を条件付きコンパイルすることができるようにするための制御構文である。

まずは、この構文を使用しない例を示す。

```cpp
    // @@@ example/template/constexpr_if_ut.cpp 9

    // 配列のサイズ
    template <typename T>
    auto Length(T const&) -> std::enable_if_t<std::is_array_v<T>, size_t>
    {
        return std::extent_v<T>;
    }

    // コンテナのサイズ
    template <typename T>
    auto Length(T const& t) -> decltype(t.size())
    {
        return t.size();
    }

    // その他のサイズ
    size_t Length(...) { return 0; }
```
```cpp
    // @@@ example/template/constexpr_if_ut.cpp 31

    uint32_t a[5];
    auto     v = std::vector{0, 1, 2};
    struct SizeTest {
    } t;

    ASSERT_EQ(5, Length(a));
    ASSERT_EQ(3, Length(v));
    ASSERT_EQ(0, Length(t));

    // C++17で、Lengthと同様の機能の関数テンプレートがSTLに追加された
    ASSERT_EQ(std::size(a), Length(a));
    ASSERT_EQ(std::size(v), Length(v));
```

このような場合、[SFINAE](term_explanation.md#SS_5_4_7)によるオーバーロードが必須であったが、
この文を使用することで、下記のようにオーバーロードを使用せずに記述できるため、
条件分岐の可読性の向上が見込める。

```cpp
    // @@@ example/template/constexpr_if_ut.cpp 52

    struct helper {
        template <typename T>
        auto operator()(T const& t) -> decltype(t.size());
    };

    template <typename T>
    size_t Length(T const& t)
    {
        if constexpr (std::is_array_v<T>) {  // Tが配列の場合
            // Tが配列でない場合、他の条件のブロックはコンパイル対象外
            return std::extent_v<T>;
        }
        else if constexpr (std::is_invocable_v<helper, T>) {  // T::Lengthが呼び出せる場合
            // T::Lengthが呼び出せない場合、他の条件のブロックはコンパイル対象外
            return t.size();
        }
        else {  // それ以外
            // Tが配列でなく且つ、T::Lengthが呼び出ない場合、他の条件のブロックはコンパイル対象外
            return 0;
        }
    }
```

この構文は[パラメータパック](template_meta_programming.md#SS_3_1_3)の展開においても有用な場合がある。

```cpp
    // @@@ example/template/constexpr_if_ut.cpp 93

    // テンプレートパラメータで与えられた型のsizeofの値が最も大きな値を返す。
    template <typename HEAD>
    constexpr size_t MaxSizeof()
    {
        return sizeof(HEAD);
    }

    template <typename HEAD, typename T, typename... TAILS>
    constexpr size_t MaxSizeof()
    {
        return std::max(sizeof(HEAD), MaxSizeof<T, TAILS...>());
    }
```
```cpp
    // @@@ example/template/constexpr_if_ut.cpp 111

    static_assert(4 == (MaxSizeof<int8_t, int16_t, int32_t>()));
    static_assert(4 == (MaxSizeof<int32_t, int16_t, int8_t>()));
    static_assert(sizeof(std::string) == MaxSizeof<int32_t, int16_t, int8_t, std::string>());
```

C++14までの構文を使用する場合、
上記のようなオーバーロードとリカーシブコールの組み合わせが必要であったが、
constexpr ifを使用することで、やや単純に記述できる。

```cpp
    // @@@ example/template/constexpr_if_ut.cpp 123

    // テンプレートパラメータで与えられた型のsizeofの値が最も大きな値を返す。
    template <typename HEAD, typename... TAILS>
    constexpr size_t MaxSizeof()
    {
        if constexpr (sizeof...(TAILS) == 0) {  // TAILSが存在しない場合
            return sizeof(HEAD);
        }
        else {
            return std::max(sizeof(HEAD), MaxSizeof<TAILS...>());
        }
    }
```


### 意図しないname lookupの防止 <a id="SS_3_6_5"></a>
下記のようにクラスや関数テンプレートが定義されている場合を考える。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp 11

    namespace App {

    struct XY {
        int x;
        int y;
    };

    // このような関数テンプレートは適用範囲が広すぎるので定義すべきではないが、
    // 危険な例を示すためあえて定義している
    template <typename T, typename U>
    inline auto is_equal(T const& lhs, U const& rhs) noexcept
        -> decltype(lhs.x == rhs.x, lhs.y == rhs.y)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
    }  // namespace App
```

これに対しての単体テストは下記のようになる。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp 37

    auto xy0 = App::XY{0, 1};
    auto xy1 = App::XY{0, 2};
    auto xy2 = App::XY{0, 1};

    ASSERT_FALSE(is_equal(xy0, xy1));
    ASSERT_TRUE(is_equal(xy0, xy2));

    struct point {
        int x;
        int y;
    };
    auto p0 = point{0, 1};

    // 下記のような比較ができるようにするためis_equalはテンプレートで実装している
    ASSERT_TRUE(is_equal(p0, xy0));
    ASSERT_FALSE(is_equal(p0, xy1));
```

上記の抜粋である

```cpp
    // @@@ example/template/suppress_adl_ut.cpp 43

    ASSERT_FALSE(is_equal(xy0, xy1));
    ASSERT_TRUE(is_equal(xy0, xy2));
```

が名前空間Appの指定なしでコンパイルできる理由は、
[ADL](term_explanation.md#SS_5_4_5)(実引数依存探索)により、Appもis_equalの[name lookup](term_explanation.md#SS_5_4_2)の対象になるからである。
これは便利な機能であるが、その副作用として意図しないname
lookupによるバグの混入を起こしてしまうことがある。

上記の名前空間での定義が可視である状態で、下記のようなコードを書いた場合を考える。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp 63

    namespace App2 {
    struct XYZ {
        int x;
        int y;
        int z;
    };

    inline bool is_equal(XYZ const& lhs, XYZ const& rhs) noexcept
    {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }
    }  // namespace App2
```

この単体テストは、やはりADLを使い下記のように書ける。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp 84
    auto xyz0 = App2::XYZ{0, 2, 2};
    auto xyz1 = App2::XYZ{0, 1, 2};

    ASSERT_TRUE(is_equal(xyz0, xyz0));
    ASSERT_FALSE(is_equal(xyz0, xyz1));
```

これに問題はないが、下記のテストもコンパイルでき、且つテストもパスしてしまうことには問題がある。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp 93

    auto xyz0 = App2::XYZ{0, 2, 2};
    auto xyz1 = App2::XYZ{0, 1, 2};
    auto xy0  = App::XY{0, 1};

    ASSERT_FALSE(is_equal(xy0, xyz0));  // これがコンパイルできてしまう
    ASSERT_TRUE(is_equal(xy0, xyz1));   // このis_equalはAppで定義されたもの
```

このセマンティクス的に無意味な(もしくは混乱を引き起こしてしまうであろう)コードは、

* is_equalの引数の型XY、XYZはそれぞれ名前空間App、App2で定義されている
* 従って、ADLによりis_equalのname lookupには名前空間App、App2も使われる
* 引数の型XY、XYZを取り得るis_equalはAppで定義されたもののみである

というメカニズムによりコンパイルできてしまう。

こういったname lookup、特にADLの問題に対処する方法は、

* [ジェネリックすぎるテンプレートを書かない](template_meta_programming.md#SS_3_6_5_1)
* [ADLが本当に必要でない限り名前を修飾する](template_meta_programming.md#SS_3_6_5_2)
* [ADL Firewallを使う](template_meta_programming.md#SS_3_6_5_3)

のようにいくつか考えられる。これらについて以下で説明を行う。

#### ジェネリックすぎるテンプレートを書かない <a id="SS_3_6_5_1"></a>
ここでの「ジェネリックすぎるテンプレート」とは、
シンタックス的には適用範囲が広いにもかかわらず、セマンティクス的な適用範囲は限られているものを指す。
従って下記のような関数テンプレートを指す概念ではない。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp 108

    template <typename T, size_t N>
    constexpr auto array_length(T const (&)[N]) noexcept
    {
        return N;
    }
```

前記で問題を起こした関数テンプレート

```cpp
    // @@@ example/template/suppress_adl_ut.cpp 20

    // このような関数テンプレートは適用範囲が広すぎるので定義すべきではないが、
    // 危険な例を示すためあえて定義している
    template <typename T, typename U>
    inline auto is_equal(T const& lhs, U const& rhs) noexcept
        -> decltype(lhs.x == rhs.x, lhs.y == rhs.y)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
    }  // namespace App
```

が典型的な「ジェネリックすぎるテンプレート」である。
これに対する最も安全な対処は下記コードで示す通りテンプレートを使わないことである。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp 126

    namespace App {

    struct XY {
        int x;
        int y;
    };

    inline bool is_equal(XY const& lhs, XY const& rhs) noexcept
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
    }  // namespace App
```

ジェネリックなis_equalが必要であれば下記単体テストのように
[ジェネリックラムダ](template_meta_programming.md#SS_3_6_2)を使えばよい。
こうすることでその適用範囲はそれを定義した関数内に留まる。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp 153

    // 下記のpointのようなクラスが他にもいくつかあった場合、
    // このジェネリックラムダでコードの被りは回避できる
    auto is_equal = [](auto const& lhs, auto const& rhs) noexcept {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    };

    struct point {
        int x;
        int y;
    };
    auto p0 = point{0, 1};

    ASSERT_TRUE(is_equal(p0, xy0));
    ASSERT_FALSE(is_equal(p0, xy1));
```

上記で示した

* テンプレートを使わない
* 適用範囲の広いテンプレート(ジェネリック)に対してはアクセスできる箇所を局所化する

といった方法の他にも、「[コンテナ用Nstd::operator\<\<の開発](template_meta_programming.md#SS_3_4_4)」で示した

* std::enable_if等を使用してテンプレートに適用できる型を制限する

ことも考えられる。
ベストな方法は状況に大きく依存するため一概には決められない。
その状況でのもっとも単純は方法を選ぶべきだろう(が、何が単純かも一概に決めることは難しい)。


#### ADLが本当に必要でない限り名前を修飾する <a id="SS_3_6_5_2"></a>
下記のコードについて考える。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp 176

    struct A {
        int f(int i) noexcept { return i * 3; }
    };

    int f(int i) noexcept { return i * 2; }

    namespace App {

    template <typename T>
    class ExecF : public T {
    public:
        int operator()(int i) noexcept
        {
            return f(i);  // T::fの呼び出しにも見えるが、::fの呼び出し
        }

        // Tを使ったコード
        ...
    };
    }  // namespace App
```

基底クラスのメンバ関数を呼び出す場合は、T::f()、もしくは、this->f()と書く必要があるため、
下記コードで呼び出した関数fは外部関数fの呼び出しになる
([two phase name lookup](term_explanation.md#SS_5_4_3)の一回目のname lookupでfがバインドされるため)。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp 203

    auto ef = App::ExecF<A>{};

    ASSERT_EQ(4, ef(2));  // ::fの呼び出しなので、2 * 2 == 4となる
```

これだけでも十分わかりづらいが、
ExecFのテンプレートパラメータにはクラスAしか使われないことがわかったので、
下記のようにリファクタリングしたとしよう。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp 213

    struct A {
        int f(int i) noexcept { return i * 3; }
    };

    int f(int i) noexcept { return i * 2; }

    namespace App {

    class ExecF : public A {
    public:
        int operator()(int i) noexcept { return f(i); }

        // Tを使ったコード
        ...
    };
    }  // namespace App
```

すると、fのname lookupの対象が変わってしまい、元の単体テストはパスしなくなる。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp 236

    auto ef = App::ExecF{};

    // ASSERT_EQ(4, ef(2));
    ASSERT_EQ(6, ef(2));  // リファクタリングでname lookupの対象が変わり、A::fが呼ばれる
```

こういった場合に備え単体テストを実行すべきなのだが、この程度の問題はコンパイルで検出したい。
[ADL](term_explanation.md#SS_5_4_5)や[two phase name lookup](term_explanation.md#SS_5_4_3)が絡む場合ならなおさらである。

こういう意図しない[name lookup](term_explanation.md#SS_5_4_2)に備えるためには、
修飾されていない識別子を使わないこと、つまり、
識別子には、名前空間、クラス名、this->等による修飾を施すことが重要である。

ただし、「[コンテナ用Nstd::operator\<\<の開発](template_meta_programming.md#SS_3_4_4)」で示したコード等にはADLが欠かせないため、
修飾することをルール化することはできない。場合に合わせた運用が唯一の解となる。


#### ADL Firewallを使う <a id="SS_3_6_5_3"></a>
下記のコードについて考える。

```cpp
    // @@@ example/template/adl_firewall_0_ut.cpp 10

    namespace App {

    template <typename T>
    std::string ToString(std::vector<T> const& t)
    {
        auto oss = std::ostringstream{};

        using Nstd::operator<<;
        oss << t;  // Nstd::operator<<もname lookupの対象に含める

        return oss.str();
    }
    }  // namespace App

    ...

    namespace App {
    struct XY {
        XY(int ax, int ay) noexcept : x{ax}, y{ay} {}
        int x;
        int y;
    };

    std::ostream& operator<<(std::ostream& os, XY const& xyz)
    {
        return os << std::to_string(xyz.x) + "/" + std::to_string(xyz.y);
    }
    }  // namespace App
```

上記のApp::ToStringは「[コンテナ用Nstd::operator\<\<の開発](template_meta_programming.md#SS_3_4_4)」のコードを使用し、
std::vectorオブジェクトをstd::stringに変換する。

これに対しての単体テストは下記のようになる。

```cpp
    // @@@ example/template/adl_firewall_0_ut.cpp 47

    auto xys = std::vector<App::XY>{{9, 8}, {7, 6}};

    ASSERT_EQ("9/8, 7/6", App::ToString(xys));
```

これは想定通りの動作だが、上記のAppの後に下記のコードを追加するとApp::ToStringは影響を受ける。

```cpp
    // @@@ example/template/adl_firewall_1_ut.cpp 40

    // Appに下記を追加
    namespace App {
    template <typename T>
    std::ostream& operator<<(std::ostream& os, std::vector<T> const& t)
    {
        return os << "size:" << t.size();
    }
    }  // namespace App
```

これにより元の単体テストはエラーとなり、新しい単体テストは下記のようになる。

```cpp
    // @@@ example/template/adl_firewall_1_ut.cpp 56

    auto xys = std::vector<App::XY>{{9, 8}, {7, 6}};

    // App::operator<<の追加で、App::ToStringの出力が影響を受ける
    // ASSERT_EQ("9/8, 7/6", App::ToString(xys));
    ASSERT_EQ("size:2", App::ToString(xys));
```

これが意図通りなら問題ないが、
ここでは「新たに追加した関数テンプレートApp::operator<<はstd::vector\<App::XY>用ではなかった」
としよう。その場合、これは意図しないADLによるバグの混入となる。
「[ジェネリックすぎるテンプレートを書かない](template_meta_programming.md#SS_3_6_5_1)」
で述べたように追加した関数テンプレートの適用範囲が広すぎることが原因であるが、
XY型から生成されたオブジェクト(std::vector\<App::XY>も含む)によるADLのため、
Appの宣言がname lookupの対象になったことにも原因がある。

下記のコードは後者の原因を解消する。

```cpp
    // @@@ example/template/adl_firewall_2_ut.cpp 23

    // Appの中の新たな名前空間XY_Firewall_でstruct XYとoperator<<を宣言
    namespace App {
    namespace XY_Firewall_ {

    struct XY {
        XY(int ax, int ay) noexcept : x{ax}, y{ay} {}
        int x;
        int y;
    };

    std::ostream& operator<<(std::ostream& os, XY const& xyz)
    {
        return os << std::to_string(xyz.x) + "/" + std::to_string(xyz.y);
    }
    }  // namespace XY_Firewall_

    using XY = XY_Firewall_::XY;

    }  // namespace App
```

XY型オブジェクトを引数にした関数呼び出しによる[関連名前空間](term_explanation.md#SS_5_4_6)は、
極小なApp::XY_Firewall\_であるため、意図しないADLは起こりづらく、起こっても発見しやすい。
また、XY型用operator<<もApp::XY_Firewall\_で定義し、
App内でusing XYを宣言したことで、これまで通りApp::XYが使える。

このようなテクニックをADL firewallと呼ぶ。


### Nstd::Type2Strの開発 <a id="SS_3_6_6"></a>
「[Nstdライブラリの開発](template_meta_programming.md#SS_3_2)」等で行ったメタ関数の実装は、

* 入り組んだ<>や()の対応漏れ
* &や&&のつけ忘れ
* typenameやtemplateキーワードの漏れ
* メタ関数メンバー::valueや::typeの漏れ

等によるコンパイルエラーとの戦いである。
また、これをクリアしても[two phase name lookup](term_explanation.md#SS_5_4_3)や[ADL](term_explanation.md#SS_5_4_5)が次の関門になる。
これには、デバッガのステップ実行が強力な武器となるが、
型を文字列に変換する関数があればこれもまた強力な武器となる。

以下に示すNstd::Type2Strは、「[Nstdライブラリの開発](template_meta_programming.md#SS_3_2)」等で実際に使用したそのような関数である。

```cpp
    // @@@ h/nstd_type2str.h 9

    namespace Nstd {
    namespace Inner_ {

    inline std::string demangle(char const* to_demagle)
    {
        int status;

        std::unique_ptr<char, decltype(&std::free)> demangled{
            abi::__cxa_demangle(to_demagle, 0, 0, &status), &std::free};

        return demangled.get();
    }

    template <typename>  // typenameを取り出すためだけのクラステンプレート
    struct type_capture {
    };
    }  // namespace Inner_

    template <typename T>
    std::string Type2Str()
    {
        // typeid(T)とした場合、const/volatile/&の情報が捨てられるため、
        // typeid(type_capture<T>)とし、それを防ぐ。
        auto str = std::string{Inner_::demangle(typeid(Inner_::type_capture<T>).name())};

        // T   == const int ならば、
        // str == Nstd::Inner_::type_capture<int const>
        //        <----------- 27 ----------><-- x --> 下記ではxを切り出す
        constexpr auto beg = 27U;                           // 先頭の不要な文字列数
        auto name = str.substr(beg, str.size() - beg - 1);  // 最後の文字は>なので不要

        while (name.back() == ' ') {  // 無駄なスペースを消す
            auto last = --name.end();
            name.erase(last);
        }

        return name;
    }
    }  // namespace Nstd
```

typeid::name()が返す文字列リテラルは引数の型の文字列表現を持つが、
マングリングされているためヒューマンリーダブルではない。
それをデマングルするのがabi::\_\_cxa\_demangleであるが、
残念なことにこの関数は非標準であるため、
それを使っているNstd::Inner\_::demangleは[g++](term_explanation.md#SS_5_9_1)/[clang++](term_explanation.md#SS_5_9_2)
でなければコンパイルできないだろう。

それを除けば、
複雑なシンタックスを持つ型を文字列で表現できるNstd::Type2Strは、
テンプレートプログラミングおける有効なデバッグツールであると言える。

下記単体テストは、そのことを示している。

```cpp
    // @@@ example/template/nstd_type2str_ut.cpp 11

    ASSERT_EQ("int const", Nstd::Type2Str<int const>());
    ASSERT_EQ("std::string", Nstd::Type2Str<std::string>());
    ASSERT_EQ("std::vector<int, std::allocator<int> >", Nstd::Type2Str<std::vector<int>>());

    extern void f(int);
    ASSERT_EQ("void (int)", Nstd::Type2Str<decltype(f)>());  // 関数の型

    auto lamda = []() noexcept {};
    ASSERT_NE("", Nstd::Type2Str<decltype(lamda)>());  // XXX::{lambda()#1}な感じになる

    ASSERT_EQ("std::ostream& (std::ostream&, int const (&) [3])",
              Nstd::Type2Str<decltype(Nstd::operator<< <int[3]>)>());

    // std::declvalはrvalueリファレンスを返す
    ASSERT_EQ("int (&&) [3]", Nstd::Type2Str<decltype(std::declval<int[3]>())>());

    int i3[3];
    ASSERT_EQ("int [3]", Nstd::Type2Str<decltype(i3)>());
    ASSERT_EQ("int (&) [3]", Nstd::Type2Str<decltype((i3))>());  // (i3)はlvalueリファレンス

    auto& r = i3;
    ASSERT_EQ("int (&) [3]", Nstd::Type2Str<decltype(r)>());
```

### 静的な文字列オブジェクト <a id="SS_3_6_7"></a>
std::stringは文字列を扱うことにおいて、非常に有益なクラスではあるが、
コンパイル時に文字列が決定できる場合でも、動的にメモリを確保する。

この振る舞いは、

* ランタイム時にnew/deleteを行うため、処理の遅さにつながる。
* 下記のようにエクセプションオブジェクトにファイル位置を埋め込むことは、
  デバッグに便利であるが、メモリ確保失敗を通知するような場面ではこの方法は使えない。

```cpp
    // @@@ example/template/nstd_exception_ut.cpp 6

    class Exception : std::exception {
    public:
        Exception(char const* filename, uint32_t line_num, char const* msg)
            : what_str_{std::string{filename} + ":" + std::to_string(line_num) + ":" + msg}
        {
        }

        char const* what() const noexcept override { return what_str_.c_str(); }

    private:
        std::string what_str_;
    };

    int32_t div(int32_t a, int32_t b)
    {
        if (b == 0) {
            throw Exception{__FILE__, __LINE__, "divided by 0"};  // 24行目
        }

        return a / b;
    }
```
```cpp
    // @@@ example/template/nstd_exception_ut.cpp 34

    auto caught = false;
    try {
        div(1, 0);
    }
    catch (Exception const& e) {
        ASSERT_STREQ("nstd_exception_ut.cpp:24:divided by 0", e.what());
        caught = true;
    }
    ASSERT_TRUE(caught);
```

このような問題を回避するために、ここでは静的に文字列を扱うためのクラスStaticStringを開発する。

#### StaticStringのヘルパークラスの開発 <a id="SS_3_6_7_1"></a>
StaticStringオブジェクトは、char配列をメンバとして持つが、
コンパイル時に解決できる配列の初期化にはパラメータパックが利用できる。
そのパラメータパック生成クラスを下記のように定義する。

```cpp
    // @@@ example/template/nstd_seq.h 4

    // パラメータパック展開ヘルパクラス
    template <size_t... Ns>
    struct index_sequence {
    };

    // index_sequence<0, 1, 2, ...>を作るためのクラステンプレート
    // make_index_sequence<3>
    // -> make_index_sequence<2, 2>
    // -> make_index_sequence<1, 1, 2>
    // -> make_index_sequence<0, 0, 1, 2>
    // -> index_sequence<0, 1, 2>
    template <size_t N, size_t... Ns>
    struct make_index_sequence : make_index_sequence<N - 1, N - 1, Ns...> {
    };

    template <size_t... Ns>
    struct make_index_sequence<0, Ns...> : index_sequence<Ns...> {
    };

```

このクラスにより、下記のような配列メンバの初期ができるようになる。

```cpp
    // @@@ example/template/nstd_seq_ut.cpp 7

    template <size_t N>
    struct seq_test {
        template <size_t... S>
        constexpr seq_test(index_sequence<S...>) noexcept : data{S...}
        {
        }
        int const data[N];
    };
```
```cpp
    // @@@ example/template/nstd_seq_ut.cpp 24

    constexpr auto st = seq_test<3>{index_sequence<1, 2, 3>()};
    ASSERT_EQ(1, st.data[0]);
    ASSERT_EQ(2, st.data[1]);
    ASSERT_EQ(3, st.data[2]);
```

これを下記のように使うことで、メンバである文字列配列のコンパイル時初期化ができるようになる。

```cpp
    // @@@ example/template/nstd_seq_ut.cpp 33

    template <size_t N>
    class seq_test2 {
    public:
        template <size_t... S>
        constexpr seq_test2(char const (&str)[N], index_sequence<S...>) noexcept : string_{str[S]...}
        {
        }

        constexpr char const (&String() const noexcept)[N] { return string_; }

    private:
        char const string_[N];
    };
```
```cpp
    // @@@ example/template/nstd_seq_ut.cpp 52

    constexpr char const str[]{"123"};

    constexpr auto st = seq_test2<4>{str, index_sequence<0, 1, 2>()};
    ASSERT_STREQ("123", st.String());

    constexpr auto st2 = seq_test2<4>{str, make_index_sequence<sizeof(str) - 1>()};
    ASSERT_STREQ("123", st2.String());
```

上記とほぼ同様のクラステンプレートstd::index_sequence、std::make_index_sequenceが、
utilityで定義されているため、以下ではこれらを使用する。

#### StaticStringの開発 <a id="SS_3_6_7_2"></a>
StaticStringはすでに示したテクニックを使い、下記のように定義できる。

```cpp
    // @@@ example/h/nstd_static_string.h 8

    template <size_t N>
    class StaticString {
    public:
        constexpr StaticString(char const (&str)[N]) noexcept
            : StaticString{str, std::make_index_sequence<N - 1>{}}
        {
        }

        constexpr StaticString(std::initializer_list<char> args) noexcept
            : StaticString{args, std::make_index_sequence<N - 1>{}}
        {
        }

        constexpr char const (&String() const noexcept)[N] { return string_; }
        constexpr size_t Size() const noexcept { return N; }

    private:
        char const string_[N];

        template <typename T, size_t... I>
        constexpr StaticString(T& t, std::index_sequence<I...>) noexcept : string_{std::begin(t)[I]...}
        {
            static_assert(
                std::is_same_v<T, std::initializer_list<char>> || std::is_same_v<T, char const[N]>);
            static_assert(N - 1 == sizeof...(I));
        }
    };
```

文字列リテラルからStaticStringを生成する単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_static_string_ut.cpp 11

    const auto fs = StaticString{"abc"};  // C++17からのNの指定は不要

    static_assert(sizeof(4) == fs.Size());
    ASSERT_STREQ("abc", fs.String());

    // 文字列不足であるため、下記はコンパイルさせない
    // constexpr StaticString<4> fs2{"ab"};
```

また、std::initializer_list\<char>による初期化の単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_static_string_ut.cpp 23

    const auto fs = StaticString<4>{'a', 'b', 'c'};  // C++17でもNの指定は必要

    static_assert(sizeof(4) == fs.Size());
    ASSERT_STREQ("abc", fs.String());

    // 文字列不足であるため、下記はコンパイルさせない
    // constexpr StaticString<4> fs2{'a', 'b'};
```

次にこのクラスにoperator == を追加する。

```cpp
    // @@@ example/h/nstd_static_string.h 38

    namespace Inner_ {
    template <size_t N>
    constexpr bool equal_n(size_t n, StaticString<N> const& lhs, StaticString<N> const& rhs) noexcept
    {
        if (n == N) {
            return true;
        }
        else {
            return lhs.String()[n] != rhs.String()[n] ? false : equal_n(n + 1, lhs, rhs);
        }
    }
    }  // namespace Inner_

    template <size_t N1, size_t N2>
    constexpr bool operator==(StaticString<N1> const&, StaticString<N2> const&) noexcept
    {
        return false;
    }

    template <size_t N1, size_t N2>
    constexpr bool operator!=(StaticString<N1> const& lhs, StaticString<N2> const& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <size_t N>
    constexpr bool operator==(StaticString<N> const& lhs, StaticString<N> const& rhs) noexcept
    {
        return Inner_::equal_n(0, lhs, rhs);
    }

    template <size_t N>
    constexpr bool operator!=(StaticString<N> const& lhs, StaticString<N> const& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <size_t N1, size_t N2>
    constexpr bool operator==(StaticString<N1> const& lhs, char const (&rhs)[N2]) noexcept
    {
        return lhs == StaticString{rhs};
    }

    template <size_t N1, size_t N2>
    constexpr bool operator!=(StaticString<N1> const& lhs, char const (&rhs)[N2]) noexcept
    {
        return !(lhs == rhs);
    }

    template <size_t N1, size_t N2>
    constexpr bool operator==(char const (&lhs)[N1], StaticString<N2> const& rhs) noexcept
    {
        return StaticString{lhs} == rhs;
    }

    template <size_t N1, size_t N2>
    constexpr bool operator!=(char const (&lhs)[N1], StaticString<N2> const& rhs) noexcept
    {
        return !(lhs == rhs);
    }
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_static_string_ut.cpp 38

    static_assert(StaticString{"123"} == StaticString{"123"});
    static_assert(StaticString{"123"} != StaticString{"1234"});
    static_assert(StaticString{"123"} == "123");
    static_assert("123" == StaticString{"123"});
    static_assert(StaticString{"123"} != "1234");
    static_assert("1234" != StaticString{"123"});
```

[非explitなコンストラクタによる暗黙の型変換](term_explanation.md#SS_5_2_5)
を利用した文字列リテラルからStaticStringオブジェクトへの変換は、
StaticStringがテンプレートであるため機能せず、上記のように書く必要がある。

同様にoperator + を追加する。

```cpp
    // @@@ example/h/nstd_static_string.h 101

    namespace Inner_ {
    template <size_t N1, size_t... I1, size_t N2, size_t... I2>
    constexpr StaticString<N1 + N2 - 1> concat(char const (&str1)[N1], std::index_sequence<I1...>,
                                               char const (&str2)[N2],
                                               std::index_sequence<I2...>) noexcept
    {
        return {str1[I1]..., str2[I2]...};
    }
    }  // namespace Inner_

    template <size_t N1, size_t N2>
    constexpr auto operator+(StaticString<N1> const& lhs, StaticString<N2> const& rhs) noexcept
    {
        return Inner_::concat(lhs.String(), std::make_index_sequence<N1 - 1>{}, rhs.String(),
                              std::make_index_sequence<N2>{});
    }

    template <size_t N1, size_t N2>
    constexpr auto operator+(StaticString<N1> const& lhs, char const (&rhs)[N2]) noexcept
    {
        return lhs + StaticString{rhs};
    }

    template <size_t N1, size_t N2>
    constexpr auto operator+(char const (&lhs)[N1], StaticString<N2> const& rhs) noexcept
    {
        return StaticString{lhs} + rhs;
    }
```

```cpp
    // @@@ example/template/nstd_static_string_ut.cpp 51

    constexpr auto fs0 = StaticString{"1234"} + StaticString{"567"};
    static_assert(std::is_same_v<StaticString<fs0.Size()> const, decltype(fs0)>);
    static_assert("1234567" == fs0);

    constexpr auto fs1 = StaticString{"1234"} + ":";
    static_assert(std::is_same_v<StaticString<fs1.Size()> const, decltype(fs1)>);
    static_assert("1234:" == fs1);

    constexpr auto fs2 = ":" + StaticString{"567"};
    static_assert(std::is_same_v<StaticString<fs2.Size()> const, decltype(fs2)>);
    static_assert(":567" == fs2);

    constexpr auto fs3 = StaticString{"1234"} + ":" + StaticString{"567"};
    static_assert(std::is_same_v<StaticString<fs3.Size()> const, decltype(fs3)>);
    static_assert("1234:567" == fs3);
```

#### 整数をStaticStringに変換する関数の開発 <a id="SS_3_6_7_3"></a>
コンパイル時に__LINE__をStaticStringに変換できれば、
ファイル位置をStaticStringで表現できるため、
ここではその変換関数Int2StaticString\<>()の実装を行う。

行番号を10進数での文字列で表現するため、いくつかのヘルパ関数を下記のように定義する。

```cpp
    // @@@ example/h/nstd_static_string_num.h 8

    namespace Inner_ {

    // 10進数桁数を返す
    constexpr size_t num_of_digits(size_t n) noexcept { return n > 0 ? 1 + num_of_digits(n / 10) : 0; }

    // 10のn乗を返す
    constexpr uint32_t ten_to_nth_power(uint32_t n) noexcept
    {
        return n == 0 ? 1 : 10 * ten_to_nth_power(n - 1);
    }

    // 10進数の桁の若い順番に左から並べなおす(12345 -> 54321)
    constexpr uint32_t reverse_num(uint32_t num) noexcept
    {
        return num != 0 ? (num % 10) * ten_to_nth_power(num_of_digits(num) - 1) + reverse_num(num / 10)
                        : 0;
    }

    // 10進数一桁をacsii文字に変換
    constexpr char digit_to_char(uint32_t num, uint32_t n_th) noexcept
    {
        return '0' + (num % (ten_to_nth_power(n_th + 1))) / ten_to_nth_power(n_th);
    }

    // Int2StaticStringのヘルパ関数
    template <size_t N, size_t... Cs>
    constexpr StaticString<num_of_digits(N) + 1> make_static_string(std::index_sequence<Cs...>) noexcept
    {
        return {digit_to_char(reverse_num(N), Cs)...};
    }
    }  // namespace Inner_
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_static_string_num_ut.cpp 47

    constexpr auto ns
        = make_static_string<__LINE__>(std::make_index_sequence<Inner_::num_of_digits(__LINE__)>());
    auto line_num = __LINE__ - 1;

    ASSERT_EQ(std::to_string(line_num), ns.String());
```

このままでは使いづらいため、これをラッピングした関数を下記のように定義することで、
Int2StaticString\<>()が得られる。

```cpp
    // @@@ example/h/nstd_static_string_num.h 42

    template <size_t N>
    constexpr StaticString<Inner_::num_of_digits(N) + 1> Int2StaticString() noexcept
    {
        return Inner_::make_static_string<N>(std::make_index_sequence<Inner_::num_of_digits(N)>());
    }
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_static_string_num_ut.cpp 66

    constexpr auto ns       = Int2StaticString<__LINE__>();
    auto           line_num = __LINE__ - 1;

    ASSERT_EQ(std::to_string(line_num), ns.String());
```

#### ファイル位置を静的に保持したエクセプションクラスの開発 <a id="SS_3_6_7_4"></a>
「[静的な文字列オブジェクト](template_meta_programming.md#SS_3_6_7)」で見たように、
ファイル位置を動的に保持するエクセプションクラスは使い勝手が悪い。
ここでは、その問題を解決するためのExceptionクラスの実装を示す。

```cpp
    // @@@ example/h/nstd_exception.h 10

    /// @class Exception
    /// @brief StaticString<>を使ったエクセプションクラス
    ///        下記のMAKE_EXCEPTIONを使い生成
    /// @tparam E   std::exceptionから派生したエクセプションクラス
    /// @tparam N   StaticString<N>
    template <class E, size_t N>
    class Exception : public E {
    public:
        static_assert(std::is_base_of_v<std::exception, E>);

        Exception(StaticString<N> const& what_str) noexcept : what_str_{what_str} {}
        char const* what() const noexcept override { return what_str_.String(); }

    private:
        StaticString<N> const what_str_;
    };
```

StaticStringと同様に、このままでは不便であるため、下記の関数を定義する。

```cpp
    // @@@ example/h/nstd_exception.h 29

    namespace Inner_ {
    template <class E, template <size_t> class STATIC_STR, size_t N>
    auto make_exception(STATIC_STR<N> exception_str) noexcept
    {
        return Exception<E, N>{exception_str};
    }
    }  // namespace Inner_

    template <class E, size_t LINE_NUM, size_t F_N, size_t M_N>
    auto MakeException(char const (&filename)[F_N], char const (&msg)[M_N]) noexcept
    {
        return Inner_::make_exception<E>(StaticString{filename} + ":" + Int2StaticString<LINE_NUM>()
                                         + ":" + msg);
    }
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_exception_ut.cpp 89

    auto caught   = false;
    auto line_num = __LINE__ + 2;  // 2行下の行番号
    try {
        throw MakeException<std::exception, __LINE__>(__FILE__, "some error message");
    }
    catch (std::exception const& e) {
        auto oss = std::ostringstream{};
        oss << __FILE__ << ":" << line_num << ":some error message";

        ASSERT_EQ(oss.str(), e.what());
        caught = true;
    }

    ASSERT_TRUE(caught);
```

Exceptionクラスの利便性をさらに高めるため、下記の定義を行う。

```cpp
    // @@@ example/h/nstd_exception.h 48

    #define MAKE_EXCEPTION(E__, msg__) Nstd::MakeException<E__, __LINE__>(__FILE__, msg__)
```

上記は、関数型マクロの数少ない使いどころである。

単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_exception_ut.cpp 109

    uint32_t line_num_div;  // エクセプション行を指定

    int32_t div(int32_t a, int32_t b)
    {
        if (b == 0) {
            line_num_div = __LINE__ + 1;  // 次の行番号
            throw MAKE_EXCEPTION(std::exception, "divided by 0");
        }

        return a / b;
    }
```
```cpp
    // @@@ example/template/nstd_exception_ut.cpp 126

    auto caught = false;

    try {
        div(1, 0);
    }
    catch (std::exception const& e) {  // リファレンスでcatchしなければならない
        auto oss = std::ostringstream{};
        oss << __FILE__ << ":" << line_num_div << ":divided by 0";
        ASSERT_EQ(oss.str(), e.what());
        caught = true;
    }

    ASSERT_TRUE(caught);
```

### 関数型をテンプレートパラメータで使う <a id="SS_3_6_8"></a>
ここで使う「関数型」とは、

* 関数へのポインタの型
* クロージャの型、もしくはラムダ式の型
* 関数オブジェクトの型

の総称を指す。

std::unique_ptrは、

* 第1パラメータにポインタの型
* 第2パラメータにそのポインタの解放用の関数ポインタの型

を取ることができるが、通常は第2パラメータは省略される。
省略時にはstd::default_deleteが割り当てられ、そのオブジェクトによって、
第1パラメータに対応するポインタがdeleteされる。

下記コードではこの第2パラメータにstd::freeのポインタの型を与え、
それから生成されるstd::unique_ptrオブジェクトを、

* abi::\__cxa_demangleがstd::mallocで取得したchar型ポインタ
* std::freeのポインタ

で初期化することでメモリの解放を行っている。

```cpp
    // @@@ h/nstd_type2str.h 18

    std::unique_ptr<char, decltype(&std::free)> demangled{
        abi::__cxa_demangle(to_demagle, 0, 0, &status), &std::free};
```

std::unique_ptrの第2パラメータには、上記のような関数へのポインタのみではなく、
関数型を取ることができる。

そのことを順を追って示す。
まずは、std::unique_ptrの動作を確かめるためのクラスを下記のように定義する。

```cpp
    // @@@ example/template/func_type_ut.cpp 10

    // デストラクタが呼び出された時に、外部から渡されたフラグをtrueにする
    struct A {
        explicit A(bool& destructor_called) noexcept : destructor_called{destructor_called} {}
        ~A() { destructor_called = true; };

        bool& destructor_called;
    };
```

次に示すのは、第2パラメータに何も指定しないパターンである。
テスト用クラスAの動作確認ができるはずである。

```cpp
    // @@@ example/template/func_type_ut.cpp 27

    {  // 第2パラメータに何も指定しない
        auto is_called = false;
        {
            auto ua = std::unique_ptr<A>{new A{is_called}};
            ASSERT_FALSE(is_called);  // uaのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // uaのデストラクタは呼ばれた
    }
```

次に示すのは、

```cpp
    // @@@ example/template/func_type_ut.cpp 20

    void delete_func(A* a) noexcept { delete a; }
```

のポインタをstd::unique_ptrの第2パラメータに与えた例である。

```cpp
    // @@@ example/template/func_type_ut.cpp 38

    {  // 第2パラメータに関数ポインタを与える
        auto is_called = false;
        {
            auto ua = std::unique_ptr<A, void (*)(A*)>{new A{is_called}, &delete_func};
            ASSERT_FALSE(is_called);  // uaのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // uaのデストラクタは呼ばれた
    }
```

次に示すのは、std::unique_ptrの第2パラメータにラムダを与えた例である。

```cpp
    // @@@ example/template/func_type_ut.cpp 49

    {  // 第2パラメータにラムダを与える
        auto is_called = false;
        {
            auto delete_lambda = [](A* a) noexcept { delete a; };

            // ラムダ式の型はインスタンス毎に異なるため、
            // ラムダ式の型を取得するためには下記のようにdecltypeを使う必要がある
            auto ua = std::unique_ptr<A, decltype(delete_lambda)>{new A{is_called}, delete_lambda};
            ASSERT_FALSE(is_called);  // uaのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // uaのデストラクタは呼ばれた
    }
```

次に示すのは、
std::unique_ptrの第2パラメータに関数型オブジェクトの型(std::function)を与えた例である。

```cpp
    // @@@ example/template/func_type_ut.cpp 64

    {  // 第2パラメータにstd::function型オブジェクトを与える
        auto is_called = false;
        {
            auto delete_obj = std::function<void(A*)>{[](A* a) noexcept { delete a; }};
            auto ua = std::unique_ptr<A, std::function<void(A*)>&>{new A{is_called}, delete_obj};
            ASSERT_FALSE(is_called);  // uaのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // uaのデストラクタは呼ばれた
    }
```

以上で見てきたようにstd::unique_ptrの第2パラメータには、
第1パラメータのポインタを引数に取る関数型であれば指定できる。

このようなテンプレートパラメータを持つクラステンプレートの実装例を示すため、
「[RAII(scoped guard)](design_pattern.md#SS_2_9)でも示したScopedGuardの実装を下記する。

やや意外だが、このようなテンプレートパラメータに特別な記法はなく、以下のようにすれば良い。

```cpp
    // @@@ h/scoped_guard.h 4

    /// @class ScopedGuard
    /// @brief RAIIのためのクラス。
    ///        コンストラクタ引数の関数オブジェクトをデストラクタから呼び出す。
    template <typename F>
    class ScopedGuard {
    public:
        explicit ScopedGuard(F&& f) noexcept : f_{f}
        {
            // f()がill-formedにならず、その戻りがvoidでなければならない
            static_assert(std::is_invocable_r_v<void, F>, "F must be callable and return void");
        }

        ~ScopedGuard() { f_(); }
        ScopedGuard(ScopedGuard const&)            = delete;  // copyは禁止
        ScopedGuard& operator=(ScopedGuard const&) = delete;  // copyは禁止

    private:
        F f_;
    };
```

上記コードの抜粋である下記は、テンプレートパラメータである関数型を規定するものである。

```cpp
    // @@@ h/scoped_guard.h 15

    // f()がill-formedにならず、その戻りがvoidでなければならない
    static_assert(std::is_invocable_r_v<void, F>, "F must be callable and return void");
```

これがなければ、誤った型の関数型をテンプレートパラメータに指定できてしまう。

以下にこのクラステンプレートの単体テストを示す。

まずは、以下の関数と静的変数の組み合わせ

```cpp
    // @@@ example/template/func_type_ut.cpp 80

    bool is_caleded_in_static{false};
    void caleded_by_destructor() noexcept { is_caleded_in_static = true; }
```

を使った例である。

```cpp
    // @@@ example/template/func_type_ut.cpp 88

    {  // Fに関数ポインタを与える
        is_caleded_in_static = false;
        {
            auto sg = ScopedGuard{&caleded_by_destructor};
            ASSERT_FALSE(is_caleded_in_static);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_caleded_in_static);  // sgのデストラクタは呼ばれた
    }
```

次に示すのは、それぞれにラムダ式とstd::functionを使った2例である。

```cpp
    // @@@ example/template/func_type_ut.cpp 103

    {  // Fにラムダ式を与える
        auto is_called = false;
        {
            auto gs = ScopedGuard{[&is_called]() noexcept { is_called = true; }};
            ASSERT_FALSE(is_called);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // sgのデストラクタは呼ばれた
    }
    {  // Fにstd::function型オブジェクトを与える
        auto is_called = false;
        {
            auto f  = std::function<void(void)>{[&is_called]() noexcept { is_called = true; }};
            auto gs = ScopedGuard{std::move(f)};  // sgのデストラクタは呼ばれていない
            ASSERT_FALSE(is_called);              // sgのデストラクタは呼ばれた
        }
        ASSERT_TRUE(is_called);
    }
```

次に示すのは関数型オブジェクト

```cpp
    // @@@ example/template/func_type_ut.cpp 125

    struct TestFunctor {
        explicit TestFunctor(bool& is_called) : is_called_{is_called} {}
        void  operator()() noexcept { is_called_ = true; }
        bool& is_called_;
    };
```

を使った例である。

```cpp
    // @@@ example/template/func_type_ut.cpp 136

    {  // Fに関数型オブジェクトを与える
        auto is_called = false;
        auto tf        = TestFunctor{is_called};
        {
            auto sg = ScopedGuard{std::move(tf)};  // C++17以降の記法
            ASSERT_FALSE(is_called);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // sgのデストラクタは呼ばれた
    }
```

C++17からサポートされた「クラステンプレートのテンプレートパラメータ推論」が使えないC++14以前では、
下記に示すようにScopedGuardのテンプレートラメータ型を指定しなければならない煩雑さがある。

```cpp
    // @@@ example/template/func_type_ut.cpp 148
    {  // Fに関数型オブジェクトを与える
        auto is_called = false;
        auto tf        = TestFunctor{is_called};
        {
            auto sg = ScopedGuard<TestFunctor>{std::move(tf)};  // C++14以前の記法
            ASSERT_FALSE(is_called);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // sgのデストラクタは呼ばれた
    }
```

これを回避するためには下記のような関数テンプレートを用意すればよい。

```cpp
    // @@@ example/template/func_type_ut.cpp 161

    template <typename F>
    ScopedGuard<F> MakeScopedGuard(F&& f) noexcept
    {
        return ScopedGuard<F>(std::move(f));
    }
```

下記に示した単体テストから明らかな通り、関数テンプレートの型推測の機能により、
テンプレートパラメータを指定する必要がなくなる。

```cpp
    // @@@ example/template/func_type_ut.cpp 172

    {  // Fに関数ポインタを与える
        is_caleded_in_static = false;
        {
            auto sg = MakeScopedGuard(&caleded_by_destructor);
            ASSERT_FALSE(is_caleded_in_static);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_caleded_in_static);  // sgのデストラクタは呼ばれた
    }
    {  // Fにラムダ式を与える
        auto is_called = false;
        {
            auto sg = MakeScopedGuard([&is_called]() noexcept { is_called = true; });
            ASSERT_FALSE(is_called);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // sgのデストラクタは呼ばれた
    }
    {  // Fにstd::function型オブジェクトを与える
        auto is_called = false;
        {
            auto f  = std::function<void(void)>{[&is_called]() noexcept { is_called = true; }};
            auto sg = MakeScopedGuard(std::move(f));
            ASSERT_FALSE(is_called);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // sgのデストラクタは呼ばれた
    }
    {  // Fに関数型オブジェクトを与える
        auto is_called = false;
        auto tf        = TestFunctor{is_called};
        {
            auto sg = MakeScopedGuard(std::ref(tf));  // std::refが必要
            ASSERT_FALSE(is_called);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // sgのデストラクタは呼ばれた
    }
```

このような便利なテンプレートは、Nstdのようなライブラリで定義、宣言し、
ソースコード全域からアクセスできるようにするとプロジェクトの開発効率が少しだけ高まる。


## 注意点まとめ <a id="SS_3_7"></a>
本章では、テンプレートメタプログラミングのテクニックや注意点について解説したが、
本章の情報量は多く、また他の章で行ったものもあるため以下にそれらをまとめる。

* [name lookup](term_explanation.md#SS_5_4_2)には複雑なルールが適用されるため、非直感的なバインドが行われる場合がある。
  従って、テンプレートライブラリの開発には単体テストは必須である。

* 使用しているコンパイラが[two phase name lookup](term_explanation.md#SS_5_4_3)をサポートしているか否かに気を付ける。
  それがオプションである場合は、two phase name lookupを活性化させる。

* 関数型マクロはそれ以外に実装方法がない時のみに使用する
  。

* 可変長引数を持つ関数の実装には[パラメータパック](template_meta_programming.md#SS_3_1_3)を使う。

* 処理速度や関数のリターンの型に影響する場合があるため、
  パラメータパックの処理の順番に気を付ける(「[前から演算するパラメータパック](template_meta_programming.md#SS_3_1_3_2)」参照)。

* [ADL](term_explanation.md#SS_5_4_5)を利用しない場合、テンプレートで使う識別子は名前空間名やthis->等で修飾する
  (「[意図しないname lookupの防止](template_meta_programming.md#SS_3_6_5)」参照)。

* テンプレートのインターフェースではないが、実装の都合上ヘッダファイルに記述する定義は、
  "namespace Inner\_"を使用し、非公開であることを明示する。
  また、"namespace Inner\_"で宣言、定義されている宣言、定義は単体テストを除き、
  外部から参照しない(「[is_void_sfinae_fの実装](template_meta_programming.md#SS_3_3_2_3)」参照)。

* ユニバーサルリファレンスの実際の型がlvalueリファレンスであるならば、
  constなlvalueリファレンスとして扱う
  。

* ユニバーサルリファレンス引数を他の関数に渡すのであれば、std::forwardを使う
  (「[ユニバーサルリファレンス](term_explanation.md#SS_5_6_1)」、「[ユニバーサルリファレンスとstd::forward](template_meta_programming.md#SS_3_6_1)」参照)。

* 関数テンプレートとその特殊化はソースコード上なるべく近い位置で定義する
  (「[two phase name lookup](term_explanation.md#SS_5_4_3)」参照)。

* [two phase name lookup](term_explanation.md#SS_5_4_3)により意図しない副作用が発生する可能性があるため、
  STLが特殊化を想定しているstd::hash等を除き、STLの拡張は行わない。

* ユーザが定義するテンプレートは適切に定義された名前空間内で定義する
  。

* 型とその2項演算子オーバーロードは同じ名前空間で定義する
  (「[two phase name lookup](term_explanation.md#SS_5_4_3)」参照)。

* 関数テンプレートのオーバーロードと特殊化の[name lookup](term_explanation.md#SS_5_4_2)の優先度に気を付ける。
  オーバーロードのベストマッチ選択後に特殊化された関数テンプレートがname lookupの対象になるため、
  下記コードが示すように直感に反する関数が選択される場合がある。

```cpp
    // @@@ example/template/etc_ut.cpp 7

    template <typename T> constexpr int32_t f(T) noexcept { return 0; }                   // f-0
    template <typename T> constexpr int32_t f(T*) noexcept { return 1; }                  // f-1
    template <>           constexpr int32_t f<int32_t*>(int32_t*) noexcept { return 2; }  // f-2
    // f-2はf-1の特殊化のように見えるが、T == int32_t*の場合のf-0の特殊化である。
```
```cpp
    // @@@ example/template/etc_ut.cpp 18

    // 以下、f-0/f-1/f-2のテスト
    auto c   = char{0};
    auto i32 = 0;

    // 以下はおそらく直感通り
    static_assert(f(0) == 0);               // f-0が呼ばれる
    static_assert(f(&c) == 1);              // f-1が呼ばれる
    static_assert(f<int32_t*>(&i32) == 2);  // f-2が呼ばれる

    // 以下はおそらく直感に反する
    static_assert(f(nullptr) == 0);  // f-1ではなく、f-0が呼ばれる
    static_assert(f(&i32) == 1);     // f-2ではなく、f-1が呼ばれる
```

* ユニバーサルリファレンスを持つ関数テンプレートをオーバーロードしない。
  「[ユニバーサルリファレンスとstd::forward](template_meta_programming.md#SS_3_6_1)」で述べたように、
  ユニバーサルリファレンスはオーバーロードするためのものではなく、
  lvalue、rvalue両方を受け取ることができる関数テンプレートを、
  オーバーロードを使わずに実現するための記法である。

* テンプレートに関数型オブジェクトを渡す場合、リファレンスの付け忘れに気を付ける
  (「[関数型をテンプレートパラメータで使う](template_meta_programming.md#SS_3_6_8)」
  参照)。

* 意図しないテンプレートパラメータによるインスタンス化の防止や、
  コンパイルエラーを解読しやすくするために、
  適切にstatic_assert(「[exists_begin/exsits_endの実装](template_meta_programming.md#SS_3_3_5_4)」
  参照)を使う。

* ランタイム時の処理を削減する、static_assertを適切に用いる等の目的のために、
  関数テンプレートには適切にconstexprを付けて宣言する
  (「[コンテナ用Nstd::operator\<\<の開発](template_meta_programming.md#SS_3_4_4)」
  参照)。


