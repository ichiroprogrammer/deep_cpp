# C++標準
この章では、C++標準の専門用語ついて解説を行う。

___

__この章の構成__

<!-- index 1-4 -->

[このドキュメントの構成](---)に戻る。  

___

## 型システムと算術の基礎

### 基本型
基本型(fundamental types)は、C++の標準で定義されている型で、
特別なキーワードを使用して直接宣言できる型の総称である。
[組み込み型](---)とも呼ばれることもある。

基本型は以下のに示した型によって構成される。

* [型システムと算術の基礎|算術型](---)
* [型システムと算術の基礎|汎整数型](---)
* [型システムと算術の基礎|浮動小数点型](---)
* void
* 上記した型のポインタ型

注:  
リファレンスは基本型に含まれない。

### 組み込み型
組み込み型(built-in types)は[型システムと算術の基礎|基本型](---)(fundamental types)の別称。

### 算術型
算術型とは下記の型の総称である。

* [型システムと算術の基礎|汎整数型](---)(bool, char, int, unsigned int, long long等)
* [型システムと算術の基礎|浮動小数点型](---)(float、double、long double)

算術型のサイズは下記のように規定されている。

* 1 == sizeof(bool) == sizeof(char)
* sizeof(char) <= sizeof(short) <= sizeof(int) <= sizeof(long) <= sizeof(long long)
* 4 <= sizeof(long)
* 8 <= sizeof(long long)
* 4 == sizeof(float)
* 8 == sizeof(double) <= sizeof(long double)

### 汎整数型
汎整数型とは下記の型の総称である。

* 論理型(bool)
* 文字型(char、wchar_t等)
* [型システムと算術の基礎|整数型](---)(int、unsigned int、long等)

### 整数型
整数型とは下記の型の総称である。

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

### 算術変換
C++における算術変換とは、算術演算の1つのオペランドが他のオペランドと同じ型でない場合、
1つのオペランドを他のオペランドと同じ型に変換するプロセスのことを指す。

算術変換は、[汎整数型昇格](---)と通常算術変換に分けられる。

```cpp
    // @@@ example/term_explanation/integral_promotion_ut.cpp #0:0 begin -1
```

[オブジェクト生成と初期化|一様初期化](---)を使用することで、
変数定義時の算術変換による意図しない値の変換([縮小型変換](---))を防ぐことができる。

```cpp
    // @@@ example/term_explanation/integral_promotion_ut.cpp #0:1 begin -2
```

以下に示すように、算術変換の結果は直感に反することがあるため、注意が必要である。

```cpp
    // @@@ example/term_explanation/integral_promotion_ut.cpp #0:2 begin -2
```

### 汎整数型昇格
bool、char、signed char、unsigned char、short、unsigned short型の変数が、
算術のオペランドとして使用される場合、

* その変数の型の取り得る値全てがintで表現できるのならば、int型に変換される。
* そうでなければ、その変数はunsigned int型に変換される。

この変換を汎整数型昇格(integral promotion)と呼ぶ。

従って、sizof(short) < sizeof(int)である処理系では、
bool、char、signed char、unsigned char、short、unsigned short型の変数は、
下記のようにintに変換される。

```cpp
    // @@@ example/term_explanation/integral_promotion_ut.cpp #1:0 begin -1
```

### 汎整数型拡張
汎整数型拡張とは[汎整数型昇格](---)と同じ概念を指す。

### 浮動小数点型昇格
浮動小数点型昇格とは、float型とdouble型の演算で、
float型オブジェクトがdoulbe型に変換されることを指す。

```cpp
    // @@@ example/term_explanation/integral_promotion_ut.cpp #2:0 begin -1
```

### デフォルト引数昇格
デフォルト引数昇格(Default Argument Promotions)とは、可変長引数`(...)`や、
プロトタイプを持たない関数に[型システムと算術の基礎|算術型](---)引数を渡す際に適用される昇格ルールの総称である。

デフォルト引数昇格には以下が含まれる。

- [汎整数型昇格](---)
- [浮動小数点型昇格](---)

### 縮小型変換
縮小型変換(Narrowing Conversion) とは、あるデータ型から別のデータ型に変換する際に、
変換先の型が元の型の表現範囲を完全にカバーしていない場合に発生する変換を指す。
主に[型システムと算術の基礎|整数型](---)や[型システムと算術の基礎|浮動小数点型](---)などの値を小さな範囲の型に変換する際に起こる。

```cpp
    // @@@ example/term_explanation/etc_ut.cpp #1:0 begin -1
```

[オブジェクト生成と初期化|リスト初期化](---)を使うことで、このような変換によるバグの発生を防ぐことができる。


### 浮動小数点型
浮動小数点型は以下の型の総称である。

* `float`
* `double`
* `long double`

浮動小数点の仕様は、IEEE 754標準に準拠している。
この標準は、浮動小数点演算の表現方法、精度、丸め方法、および例外処理を規定しており、
広く使用されている。

#### 浮動小数点型のダイナミックレンジ

| 型                          | 正の最小値                    | 正の最大値                    |
|:----------------------------|:------------------------------|:------------------------------|
| `float`                     | 1.175494351 e-38              | 3.402823466 e+38              |
| `double`                    | 2.2250738585072014 e-308      | 1.7976931348623158 e+308      |
| `long double`               | 3.36210314311209350626 e-4932 | 1.18973149535723176502 e+4932 |
| `int32_t`                   | -2,147,483,648                | 2,147,483,647                 |
| `int64_t`                   | -9,223,372,036,854,775,808    | 9,223,372,036,854,775,807     |

ここで`long double`の最小値と最大値は、システムやコンパイラに依存して異なる場合がある点に留意する。

#### 浮動小数点の誤差
浮動小数点変数の10進数の表現が2進数では循環小数となる場合があり、
正確に表現できないことがある。これにより、計算結果がわずかに異なる値を返す場合がある。
浮動小数点誤差は、特に計算の繰り返しや桁数の多い計算で顕著になる。

以下のコードにより誤差が容易に発生することを示す。

```cpp
    // @@@ example/term_explanation/float_ut.cpp #0:0 begin -1
```

#### イプシロン
イプシロン(epsilon)とは、ある浮動小数点数に対して「1」を加えた時に、
異なる値として識別できる最小の差分を指す。
つまり、イプシロンは浮動小数点数の精度を示す尺度である。

任意の浮動小数点変数a, bがあり、`|a - b| <= epsilon`であった場合、
浮動小数点の仕組みではa、bの差が無いものと考えて、aとbが同値であると考えることが一般的である。

イプシロンを使用した浮動小数点変数の同値判定のコード例を以下に示す。

```cpp
    // @@@ example/term_explanation/float_ut.cpp #1:0 begin -1
```

#### 浮動小数点の演算エラー
浮動小数点の演算は以下のようなエラーを生じることがある。

| エラーの種類   | 説明                                                                           | 例                           |
|:---------------|:-------------------------------------------------------------------------------|:-----------------------------|
| 丸め誤差       | 有限桁数による四捨五入の誤差が発生し、正確な値とわずかに異なる場合がある。     | `0.1 + 0.2 != 0.3`           |
| 桁落ち         | 非常に小さい数と大きい数の加算時に、小さい数が無視され、精度が低下する。       | `1e20 + 1 - 1e20 == 0`       |
| 累積誤差       | 繰り返し演算で小さな誤差が積み重なり、最終的に大きなズレが生じることがある。   | ループ内での浮動小数点の加算 |
| ゼロ除算       | 0での除算により計算が定義されず、例外が発生または±無限大が返される。           | `1.0 / 0.0`                  |
| オーバーフロー | 型が表現可能な最大値を超えると無限大（`inf`）として扱われる。                  | `std::pow(10.0, 308)`        |
| アンダーフロー | 型の最小値より小さい数値は0または非常に小さな値として表現され、精度が失われる。| `std::pow(10.0, -308)`       |
| NaN            | 実数では表現できない。                                                         | `std::sqrt(-1)`              |

浮動小数点の演算エラーの検出コード例を以下に示す。

```cpp
    // @@@ example/term_explanation/float_ut.cpp #2:0 begin -1
```

なお、上記のコードで使用した`std::fetestexcept`は一般にスレッドセーフである。
`std::fetestexcept`がスレッドセーフでない処理系では、浮動小数演算エラーの検出は、
実質的には不可能になってしまうため、
浮動小数演算を複数コンテキストで行うソフトウェアの開発する場合、
処理系の選択に注意が必要である。

## 列挙型とバイト表現
### enum
C++03までのenumは定数を分かりやすい名前で定義するための記法である。
このドキュメントでは、[スコープドenum](---)に対して、C++03までのenumを非スコープドenum、
通常のenum、あるいは単にenumと呼ぶことがある。
C++03までのenumには、以下のような問題があった。

* スコープの制限: 名前付きスコープ内に定義するためには、クラスのメンバとして定義しなければならない。
* 型安全性: enumの値は整数型と暗黙の変換が行われてしまう。
* 名前空間の汚染: グローバルスコープに定義されたenumは、名前空間を汚染する。

```cpp
    // @@@ example/term_explanation/enum_ut.cpp #0:0 begin -1
```

### enum class
enum classは通常の[列挙型とバイト表現|enum](---)の問題を解決するためにC++11から導入された。

```cpp
    // @@@ example/term_explanation/enum_ut.cpp #1:0 begin -1
```

```cpp
    // @@@ example/term_explanation/enum_ut.cpp #1:1 begin -1
```

### スコープドenum
[enum class](---)はスコープドenum(scoped enum)と呼ばれることがある。


### underlying type
underlying typeとは、enumやenum classの[汎整数型](---)を指定できるようにするために、
C++11で導入されたシンタックスである。enumのサイズをユーザが定義できるため、
特定のバイナリプロトコルとの互換性が必要な場合や、特定のハードウェアと連携する際に特に有効である。

```cpp
    // @@@ example/term_explanation/enum_ut.cpp #2:0 begin
```

C++17までは、型安全の観点から、初期化においては、以下のコードコメントのような仕様であったが、
C++17から導入された[std::byte](---)の利便性のため、
underlying typeを指定したenumやenum class変数のunderlying typeインスタンスによる初期化が認められるようになった。

```cpp
    // @@@ example/term_explanation/enum_ut.cpp #2:1 begin -1
```

上記コードにもあるが、underlying typeインスタンスによる初期化を行う場合は、
意図しない縮小型変換によるバグの発生を防ぐためにも、
[オブジェクト生成と初期化|一様初期化](---)を使用するべきだろう。

一部の例外を除くとunderlying typeを指定しないenumやenum classはコンパイル時にサイズが確定できないため、
前方宣言できないが、underlying typeを指定したenum、enum classは前方宣言することができる。

```cpp
    // @@@ example/term_explanation/enum_ut.cpp #3:0 begin
```

### std::byte
C++17で導入されたstd::byte型は、バイト単位のデータ操作に使用され、
[型システムと算術の基礎|整数型](---)としての意味を持たないため、型安全性を確保する。
uint8_t型と似ているが、uint8_t型の演算による[汎整数型昇格](---)を発生させないため、
可読性、保守性の向上が見込める。

```cpp
    // @@@ example/term_explanation/enum_ut.cpp #4:0 begin -1
```

### using enum
名前空間のように、

```cpp
    using enum EnumType;
```

もしくは

```cpp
    using EnumType::enumerator
```

とすることで、スコープによる修飾を省略するための記法である。

```cpp
    // @@@ example/term_explanation/enum_ut.cpp #5:0 begin
```
```cpp
    // @@@ example/term_explanation/enum_ut.cpp #5:1 begin
```

```cpp
    // @@@ example/term_explanation/enum_ut.cpp #6:0 begin
```
```cpp
    // @@@ example/term_explanation/enum_ut.cpp #6:1 begin -1
```

この記法は、簡潔に記述できるものの、一方では過度な使用は、
C++03までのenumが持っていた問題を再発生させてしまうため、
ブロックスコープ以外での使用に関しては控え目に使用するべきだろう。

## 型とインスタンス
### トリビアル型
トリビアル型とは、

* 全ての[型とインスタンス|特殊メンバ関数](---)がデフォルトである。
* バーチャル関数や仮想継承を持たない。
* 基底クラスがある場合、基底クラスもトリビアルである。

である。その結果、トリビアル型とは、[トリビアルに破壊可能な型](---)となる。

「型Tがトリビアルであること」と「以下の行がコンパイルできること」は等価である。

```cpp
    static_assert(std::is_trivial_v<T>);
```

下記のコードはその使用例である。

```cpp
    // @@@ example/term_explanation/trivial_ut.cpp #0:3 begin -2
```

### トリビアルに破壊可能な型
「トリビアルに破壊可能な型(Trivially Destructible)」とは、以下の条件を満たす型を指す。

* デストラクタがユーザー定義されていない
  (つまりコンパイラが生成したデフォルトのデストラクタを使用している)。
* 型に含まれるすべてのメンバ変数や基底クラスも「トリビアルに破壊可能」である。

```cpp
    // @@@ example/term_explanation/trivial_ut.cpp #1:0 begin -2
```

### 標準レイアウト型
「型Tが標準レイアウトであること」と「以下の行がコンパイルできること」は等価である。

```cpp
    static_assert(std::is_standard_layout_v<T>);
```

下記のコードはその使用例である。

```cpp
    // @@@ example/term_explanation/trivial_ut.cpp #0:2 begin -2
```

### 集成体
型Tが集成体であるための条件を以下に示す。

* 型Tが`class`、`struct`、`union`であった場合、以下の条件を満たせばTは集成体である。
    * 以下に示したユーザ定義による特殊関数が存在しない。
        * ユーザー定義のコンストラクタ(デフォルトコンストラクタ、コピーコンストラクタ、ムーブコンストラクタ)
        * デストラクタ
        * コピー代入演算子
        * ムーブ代入演算子

    * すべての非静的メンバがpublicであるか、それらに外部からアクセスできる。
    * 仮想関数や仮想基底クラスを持たないこと
    * 仮想関数が定義されておらず、仮想基底クラス（仮想継承）を使用していない。
    * 基底クラスを持たない。

* 集成体の配列や、組み込み型の配列は集成体である。

### POD
PODとは、 Plain Old Dataの略語であり、
「型TがPODであること」と「以下の行がコンパイルできること」は等価である。

```cpp
    static_assert(std::is_pod_v<T>);  // is_podはC++20から非推奨
```

「型が[トリビアル型](---)且つ[標準レイアウト型](---)であること」と
「型が[POD](---)であること」は等価であるため、C++20では、
[PODという用語は非推奨](https://cpprefjp.github.io/lang/cpp20/deprecate_pod.html)となった。
従って、std::is_pod_vは以下のように置き換えられるべきである。

```cpp
    // @@@ example/term_explanation/trivial_ut.cpp #0:0 begin
```

下記のコードは置き換えられたstd::is_pod_vの使用例である。

```cpp
    // @@@ example/term_explanation/trivial_ut.cpp #0:1 begin -2
```

上記からわかる通り、POD型とは概ね、C言語と互換性のある型を指すと思って良い。


### 不完全型
不完全型とは、型のサイズや構造が不明な型を指す。
以下のis_completeで示したテンプレート定数で、不完全型か否かを判定できる。

```cpp
    // @@@ example/term_explanation/incomplete_type_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/incomplete_type_ut.cpp #1:0 begin -2
```
```cpp
    // @@@ example/term_explanation/incomplete_type_ut.cpp #1:1 begin -2
```

### 完全型
[不完全型](---)ではない型を指す。

### ポリモーフィックなクラス
ポリモーフィックなクラスとは仮想関数を持つクラスや、
ポリモーフィックなクラスから派生したクラスを指す。
なお、純粋仮想関数を持つクラスは、
仮想クラスと呼ばれれる(「[インターフェースクラス](---)」参照)。
ポリモーフィックなクラスと、
非ポリモーフィックなクラスは[型とインスタンス|RTTI](---)との組み合わせで動作の違いが顕著となる。

非ポリモーフィックなクラスは非静的なメンバ変数が定義された順にメモリ上に配置されたレイアウトを持つ
(CPUアーキテクチャに依存したパディング領域が変数間に挿入されることもある)。
このようなクラスは[POD](---)
(C++20では、[PODという用語は非推奨](https://cpprefjp.github.io/lang/cpp20/deprecate_pod.html)
となり、[トリビアル型](---)と[標準レイアウト型](---)に用語が分割された)とも呼ばれ、
C言語の構造体のレイアウトと互換性を持つことが一般的である。

ポリモーフィックなクラスは、
仮想関数呼び出しを行う(「[オーバーライドとオーバーロードの違い](---)」参照)
ためのメモリレイアウトが必要になる。
それを示すために、まずは下記のようにクラスX、Y、Zを定義する。

```cpp
    // @@@ example/term_explanation/class_layout_ut.cpp #0:0 begin
```

通常のC++コンパイラが作り出すX、Y、Zの概念的なメモリレイアウトは下記のようになる。

下図中のvtbl(virtual table または virtual function table)とは、
仮想関数ポインタを保持するための構造体であり、仮想関数呼び出しを解決するための仕組みである。

![layout](plant_uml/class_layout.png)

各クラスがvtblへのポインタを保持するため、このドキュメントで使用している[g++](---)では、
sizeof(X)は8ではなく16、sizeof(Y)は16ではなく24、sizeof(Z)は24ではなく32となる。

g++の場合、以下のオプションを使用し、クラスのメモリレイアウトをファイルに出力することができる。

```cpp
    // @@@ example/term_explanation/Makefile #0:0 begin
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
    // @@@ example/term_explanation/class_layout_ut.cpp #0:1 begin -1
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

### RTTI
RTTI(Run-time Type Information)とは、プログラム実行中のオブジェクトの型を導出するための機能であり、
具体的には下記の3つの要素を指す。

* [dynamic_cast](---)
* [typeid](---)
* [std::type_info](---)


#### dynamic_cast
dynamic_castは、実行時の型チェックと安全なダウンキャストを行うためのキャスト演算子であるため、
[ポリモーフィックなクラス](---)とは密接な関係を持つ。


下記のような[ポリモーフィックなクラス](---)に対しては、

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #0:0 begin
```

dynamic_castは下記のように振舞う。

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #0:1 begin -1
```


一方で、下記のような非[ポリモーフィックなクラス](---)に対しては、

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #1:0 begin
```

dynamic_castは下記のように振舞う。

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #1:1 begin -1
```

#### typeid
typeidは[型とインスタンス|RTTI](---)オブジェクトの型情報
([std::type_info](---))を実行時に取得するための演算子である。
dynamic_castとは違い、
typeidのオペランドは[ポリモーフィックなクラス](---)のインスタンスでなくても良い。
以下の例では[基本型](---)に対するtypeidが返す[std::type_info](---)の振る舞いを表す。

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #0:2 begin -1
```

下記のような[ポリモーフィックなクラス](---)のインスタンスに関して、

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #0:0 begin -1
```

typeidが返す[std::type_info](---)オブジェクトは下記のように振舞う。

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #0:3 begin -1
```

一方で、下記のような非[ポリモーフィックなクラス](---)に対しては、

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #1:0 begin
```

typeidが返す[std::type_info](---)オブジェクトは下記のように振舞う。

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #1:2 begin -1
```

従って、このような場合のtypeidは静的な型(表層の型)に対しての情報を返すため、
コンパイル時にのみ評価され、ランタイム時に評価されない。

[ポリモーフィックなクラス](---)のオブジェクトをオペランドとするtypeidの実行は、
そのオペランドの実際のオブジェクトの型を取得することはすでに示した。
このような場合、オペランド式は実行時に評価される。以下のコードはそのことを表している。

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #0:4 begin -1
```


一方、非[ポリモーフィックなクラス](---)のオブジェクトをオペランドとするtypeidのオペランド式は、
コンパイル時に処理されるため、その式は実行されない。以下のコードはそのことを表している。

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #1:4 begin -1
```

#### std::type_info
type_infoクラスは、[typeid](----)演算子によって返される、型の情報が格納された型である。

std::type_infoはコンパイラの実装で定義された型名を含んでいる。
以下のコードで示したように`std::type_info::name()`によりその型名を取り出すことができる。

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #2:0 begin -1
```

`std::type_info::name()`が返すCスタイルの文字列リテラルを、
「人間が認知できる元の型名に戻す関数」を通常のコンパイラは独自に提供する。
このドキュメントのコードのコンパイルに使用している[g++](---)/[clang++](---)では、
そのような関数は、`abi::__cxa_demangle`である。

`std::type_info::name()`と`abi::__cxa_demangle`を利用して、
オブジェクトの[被修飾型](---)名をstd::stringオブジェクトとして取り出す関数とその使用例を以下に示す。

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #2:1 begin
```
```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #2:2 begin -1
```

### Run-time Type Information
「[型とインスタンス|RTTI](---)」を参照せよ。


### インターフェースクラス
インターフェースクラスとは、純粋仮想関数のみを持つ抽象クラスのことを指す。
インターフェースクラスは、クラスの実装を提供することなく、
クラスのインターフェースを定義するために使用される。
インターフェースクラスは、クラスの仕様を定義するために使用されるため、
多くの場合、抽象基底クラスとして使用される。

```cpp
    // @@@ example/term_explanation/interface_class.cpp #0:0 begin
```

### constインスタンス
constインスタンスは、ランタイムまたはコンパイル時に初期化され、
その後、状態が不変であるインスタンスである。
必ずしも以下に示すようにconstインスタンスがコンパイル時に値が定まっているわけではない。
[constexprインスタンス](---)はconstインスタンスである。
C++03までのコンパイラに、
最適化の一環で`static const`インスタンスを[constexprインスタンス](---)と扱うものもあった。


```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #0:0 begin -1
```

### リテラル型
constexpr導入後のC++11の標準では、下記の条件を満たすクラスは、

* constexprコンストラクタを持つ
* すべてのメンバ変数がリテラル型である
* 仮想関数や仮想基底クラスを持たない

constexpr定数もしくはconstexprインスタンスをコンストラクタに渡すことにより、
[constexprインスタンス](---)を生成できる。

このようなクラスは慣習的にリテラル型(literal type)と呼ばれる。

以下にリテラル型を例示する。

```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #3:0 begin
```
```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #3:1 begin -1
```

## 定数式とコンパイル時評価

### constexpr
constexprはC++11で導入されたキーワードで、
関数や変数をコンパイル時に評価可能にする。
これにより、定数計算がコンパイル時に行われ、
実行時のパフォーマンスが向上し、コンパイル時にエラーを検出できることがある。

### constexpr定数
C++11以前で定数を定義する方法は、

* マクロ定数
* [enum](---)
* static const(定数となるか否かは、コンパイラの実装依存に依存する)

の方法があったが、それぞれの方法には下記のような問題がある。

* マクロにはスコープが無く、`#undef`できてしまう。
* enumには整数の定義に限られる。
* static constに関しては、コンパイラの実装依存に依存する。

こういった問題を解決できるのがconstexpr定数である。constexpr定数とは下記のような定数を指す。

```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #1:1 begin -1
```

constexpr定数がif文のオカレンスになる場合、[constexpr if文](---)することで、
[ill-formed](---)を使用した場合分けが可能になる。


### constexpr関数
関数に`constexpr`をつけて宣言することで定数を定義することができる。
constexpr関数の呼び出し式の値がコンパイル時に確定する場合、
その値はconstexpr定数となるため、関数呼び出しが発生しないため、実行効率が向上する。
一方で、constexpr関数の呼び出し式の値が、コンパイル時に確定しない場合、
通常の関数呼び出しと同じになる。

```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #2:1 begin -1
```

C++11の規約では、constexpr関数の制約は厳しく、
for/if文や条件分岐のような処理を含むことができなかったため、
下記のコード例で示した通り、条件演算子とリカーシブコールをうことが多かった。

```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #5:0 begin
```
このため、可読性、保守性があったため、C++14で制約が緩和され、
さらにC++17では for/if文などの一般的な制御構文も使えるようになった。

```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #5:1 begin
```

### コア定数式
コア定数式(core constant expression)とは以下の条件を満たす式である。

1. 以下のいずれかに該当する式であること  
   - リテラル
   - constexpr変数への参照
   - 定数式で初期化された参照
   - constexprサブオブジェクトへの参照
   - constexpr関数呼び出し
   - sizeof演算子の適用結果
   - typeid演算子の適用結果(式の値が[ポリモーフィックなクラス](---)である場合を除く)

2. 以下のすべてを満たすこと:  
   - 浮動小数点の比較演算を含まない
   - インクリメント/デクリメント演算を含まない
   - 代入演算を含まない
   - 動的メモリ割り当てを含まない
   - 仮想関数の呼び出しを含まない
   - 未定義動作を引き起こさない
   - エクセプションを投げない
   - アドレス取得演算子の使用が定数式の評価に限定される

3. その式の評価において:  
   - すべてのサブ式も定数式である
   - 使用されるすべての変数は定数式で初期化されている
   - 呼び出されるすべての関数はconstexpr関数である

このドキュメントでは慣用的に[constexpr定数](---)と呼んでいる概念が、コア定数式である。

### constexprインスタンス
[constexpr定数](---)を引数にして、[リテラル型](---)のconstexprコンストラクタを呼び出せば、
constexprインスタンスを生成できる。このリテラル型を使用して下記のように[ユーザー定義リテラル](---)
を定義することで、constexprインスタンスをより簡易に使用することができるようになる。

```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #4:0 begin
```
```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #4:1 begin -1
```

### consteval
constevalはC++20 から導入されたキーワードであり、
常にコンパイル時に評価されることを保証する関数を定義するために使用される。
このキーワードを使用すると、引数や関数内の処理がコンパイル時に確定できなければ、
コンパイルエラーが発生する。constexprと異なり、ランタイム評価が許されないため、
パフォーマンスの最適化やコンパイル時のエラー検出に特化した関数を作成する際に便利である。

```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #6:0 begin
```
```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #6:1 begin -1
```

### constinit
constinitはC++20から導入されたキーワードであり、コンパイル時における変数の初期化を強制する。

### constexprラムダ
constexprラムダはC++17から導入された機能であり、以下の条件を満たした[言語拡張機能|ラムダ式](---)である。

* 引数やラムダ式内の処理がコンパイル時に評価可能である必要がある。
  すべての処理はconstexpr関数のようにコンパイル時に確定する必要があり、
  動的な処理やランタイムでしか決定できないものは含めることができない。

* ラムダ内で使用される関数や式もconstexprでなければならない。
  たとえば、関数の呼び出しや算術演算は、コンパイル時に評価可能なものであることが求められる。

* ラムダキャプチャはconstexprに適合している必要がある。
  キャプチャする変数もコンパイル時に確定できるものに限られる。
  動的な変数をキャプチャすると、コンパイルエラーとなる。

* エクセプション処理 (try/catch/throw) が禁止されている。
  constexprラムダでは、エクセプション処理を含むことはできない。

* 動的メモリの割り当て(new/delete) が禁止されている。
  これらの操作はコンパイル時には行えないため、constexprラムダでは使用できない。

```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #7:0 begin -1
```
```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #7:1 begin -1
```

## オブジェクト生成と初期化
### リスト初期化
リスト初期化とは、C++11で導入された`{}`を使ったオブジェクトの初期化構文を指す。
以下にコード例を示す。

```cpp
    // @@@ example/term_explanation/uniform_initialization_ut.cpp #0:0 begin -1
```

変数による一様初期化が縮小型変換を起こす場合や、
リテラルによる一様初期化がその値を変更する場合、コンパイルエラーとなるため、
この機能を積極的に使用することで、縮小型変換による初期化のバグを未然に防ぐことができる。

```cpp
    // @@@ example/term_explanation/uniform_initialization_ut.cpp #0:1 begin -1
```

### 一様初期化
一様初期化(Uniform Initialization)は 、
[オブジェクト生成と初期化|リスト初期化](---)による初期化方法がC++における初期化を統一的に扱えるように設計された概念を指さす。

### 初期化子リストコンストラクタ
初期化子リストコンストラクタ([オブジェクト生成と初期化|リスト初期化](---)用のコンストラクタ)とは、
{}による[オブジェクト生成と初期化|リスト初期化](---)をサポートするためのコンストラクタである。
下記コードでは、 E::E(std::initializer_list\<uint32_t>)が初期化子リストコンストラクタである。

```cpp
    // @@@ example/term_explanation/constructor_ut.cpp #0:0 begin
```

デフォルトコンストラクタと初期化子リストコンストラクタが、
それぞれに定義されているクラスの初期化時に空の初期化子リストが渡された場合、
デフォルトコンストラクタが呼び出される。

初期化子リストコンストラクタと、
「その初期化子リストの要素型と同じ型の仮引数のみを受け取るコンストラクタ
(上記コードのE::E(uint32_t, uint32_t))」
の両方を持つクラスの初期化時にどちらでも呼び出せる初期化子リストが渡された場合({}を使った呼び出し)、
初期化子コンストラクタが呼び出される。

### 継承コンストラクタ
継承コンストラクタとは、基底クラスで定義したコンストラクタ群を、
派生クラスのインターフェースとしても使用できるようにするための機能である。
下記コードのように、継承コンストラクタは派生クラス内でusingを用いて宣言される。

```cpp
    // @@@ example/term_explanation/constructor_ut.cpp #1:0 begin
```

### 委譲コンストラクタ
委譲コンストラクタとは、コンストラクタから同じクラスの他のコンストラクタに処理を委譲する機能である。
以下のコード中では、委譲コンストラクタを使い、
A::A(uint32_t)の処理をA::A(std::string const&)へ委譲している。

```cpp
    // @@@ example/term_explanation/constructor_ut.cpp #2:0 begin
```

### NSDMI
NSDMIとは、non-static data member initializerの略語であり、
下記のような非静的なメンバ変数の初期化子を指す。

```cpp
    // @@@ example/term_explanation/nsdmi.cpp #0:0 begin
```

### 初期化子リストでの初期化
「非静的メンバ変数をコンストラクタの本体よりも前に初期化する」言語機能である。
メンバ変数は宣言された順序で初期化されるため、
初期化子リストでの順序は、実際の初期化の順序とは関係がない。

この機能を使うことで、メンバ変数の初期化処理が簡素に記述できる。
constメンバ変数は、初期化子リストでの初期化か[NSDMI](---)でしか初期化できない。

```cpp
    // @@@ example/term_explanation/nsdmi.cpp #0:1 begin
```

### コンストラクタ内での非静的なメンバ変数の初期値の代入
この方法は単なる代入でありメンバ変数の初期化ではない。

[NSDMI](---)、
[初期化子リストでの初期化](---)で初期化できない変数を未初期化でない状態にするための唯一の方法である。

```cpp
    // @@@ example/term_explanation/nsdmi.cpp #0:2 begin
```

## 値カテゴリとリファレンス
ここでは、expression(式)の値カテゴリや、それに付随した機能についての解説を行う。

### expression

[expression](https://ja.cppreference.com/w/cpp/language/expressions)(式)とは、
「演算子とそのオペランドの並び」である(オペランドのみの記述も式である)。
演算子とは以下のようなものである。

* 四則演算、代入(a = b、a += b ...)、インクリメント、比較、論理式
* 明示的キャストや型変換
* メンバアクセス(a.b、a->b、a[x]、 \*a、&a ...)
* 関数呼び出し演算子(f(...))、sizeof、decltype等


expressionは、

* [expression|lvalue](---)
* [expression|rvalue](---)
* [expression|xvalue](---)
* [expression|glvalue](---)
* [expression|prvalue](---)

に分類される。
![expression分類](plant_uml/rvalue.png)


expressionは、[expression|lvalue](---)か[expression|rvalue](---)である。


#### lvalue
lvalueとは、

* 名前を持つオブジェクト(識別子で参照可能)や関数を指す式
* 代入式の左辺になり得る式であるため、左辺値と呼ばれることがある。
* constなlvalueは代入式の左辺にはなり得ないが、lvalueである。
* [expression|rvalue](---)でない[expression](---)がlvalueである。

`T const&`は代入式の左辺になりは得ないがlvalueである。[rvalueリファレンス](---)もlvalueである。

#### rvalue
rvalueとは、

* テンポラリな値を表す式(代入式の右辺値として使われることが多い)
* [expression|xvalue](---)か[expression|prvalue](---)である。
* [expression|lvalue](---)でない[expression](---)がrvalueである。

[rvalueリファレンス](---)(`T&&`型の変数)はlvalueである。
一方、その初期化に使われる式(例えばstd::move(x))は[xvalue](---)である。


#### xvalue
xvalueとは以下のようなものである。

* 戻り値の型がT&&(Tは任意の型)である関数の呼び出し式(std::move(x))
* オブジェクトへのT&&へのキャスト式(static_cast<char&&>(x))
* aを配列のxvalueとした場合のa[N]や、cをクラス型のrvalueとした場合のc.m(mはaの非staticメンバ)等

#### prvalue
prvalueとは、オブジェクトやビットフィールドを初期化する、
もしくはオペランドの値を計算する式であり、以下のようなものである。

* 文字列リテラルを除くリテラル
* 戻り値の型が非リファレンスである関数呼び出し式、
  または前置++と前置--を除くオーバーロードされた演算子式(`path.string()`、`str1 + str2`、`it++` ...)
* 組み込み型インスタンスaの`a++`、`a--`(`++a`や`--a`はlvalue)
* 組み込み型インスタンスa、bに対する
  `a + b`、 `a % b`、 `a & b`、 `a && b`、 `a || b`、 `!a`、 `a < b`、 `a == b`等
* prvalue(もしくはrvalue)は、
    * アドレス演算子(&)のオペランドになれない。
    * 非constな[lvalueリファレンス](---)ではバインドできないが、
      constな[lvalueリファレンス](---)や[rvalueリファレンス](---)でバインドできる。
  

つまり、prvalueとはいわゆるテンポラリオブジェクトのことである(下記の`std::string{}`で作られるようなオブジェクト)。
多くの場合、prvalueはテンポラリオブジェクトを生成するが、
C++17以降は[RVO(Return Value Optimization)](---)により、
テンポラリオブジェクトを生成せず、直接、初期化に使われる場合もある。  
また、正確にはprvalueと呼ぶべき場面でも単にrvalueと呼ばれることがある。
このドキュメントでも、そうなっていることもある。

```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #0:0 begin -1
```

#### glvalue
glvalueは、

* [expression|lvalue](---)か[expression|xvalue](---)である。
* "generalized lvalue"の略称

オブジェクトや関数を参照する式を総称してglvalueと呼ぶ。
これにより、式が「場所を指す」か「一時的な値を表す」かを大きく分類できる。


### decltypeとexpression
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
    // @@@ example/term_explanation/decltype_expression_ut.cpp #0:0 begin -1
```

## リファレンス

リファレンス(参照)とは、以下のいずれか、もしくはすべてを指すが、
単にリファレンスと呼ぶ場合、lvalueリファレンスを指すことが多い。

* [lvalueリファレンス](---)
* [rvalueリファレンス](---)
* [forwardingリファレンス](---)


これらの概念と関わり強い、[リファレンスcollapsing](---)についても併せて解説を行う。

### lvalueリファレンス
lvalueリファレンスとは、

* C++98(もしくは03)から導入されたシンタックスであり、任意の型Tに対して`T&`という形式で宣言される。
* 既存のオブジェクトに対する別名(エイリアス)であり、宣言時に必ず初期化が必要で、
  一度初期化後は別のオブジェクトを参照することはできない。
* [rvalueリファレンス](---)導入前のC++では、すべてのリファレンスはlvalueリファレンスであったため、
  lvalueリファレンスを単にリファレンスと呼んでいた。
* オブジェクトaのエイリアスとして、
   リファレンスa_refが宣言されることを「a_refはaをバインドする」という。
* 以下のコード例で示すように、
    * 非const lvalueリファレンスは[expression|rvalue](---)をバインドできないが、
    * const lvalueリファレンスは[expression|rvalue](---)をバインドできる。

```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #1:0 begin -1
```

このようなリファレンスのバインドの可否はオーバーロードにも影響を与える。


```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #1:1 begin
```

```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #1:2 begin -1
```

### rvalueリファレンス
rvalueリファレンスは、

* C++11で導入されたシンタックスであり、任意の型Tに対して、`T&&`で宣言される。
* 「テンポラリオブジェクト([expression|rvalue](---))」をバインドできるリファレンス。
* C++11の[moveセマンティクス](---)と[perfect forwarding](---)を実現するために導入された。
* **注意1** 型が`T&&`である変数の値カテゴリは[lvalue](---)である。
* **注意2** 型が`T&&`である変数は、`T&`でバインドできる。

```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #2:0 begin -1
```

このようなリファレンスのバインドの可否はオーバーロードにも影響を与える。


```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #2:1 begin
```

```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #2:2 begin -1
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #2:3 begin -1
```

上記コードの最後の部分の抜粋である以下のコードについては、少々解説が必要だろう。

```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #2:3 begin -1
```

ref_refの型は`int &&`であるが、ref_refの値カテゴリは[expression|rvalue](---)ではなく、[lvalue](---)である。
そのため、`f(ref_ref)`はlvalueリファレンスを引数とするf-1が選択される。

rvalueリファレンス型の仮引数（`T&&`）を持つ関数は、ムーブコンストラクタやムーブ代入演算子など頻繁に使用される。
しかし、関数内では仮引数は名前を持つため、常にlvalueとして扱われる。
この動作を理解することは、
[moveセマンティクス](---)や[perfect forwarding](---)を正しく実装/使用するために極めて重要である。

```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #2:4 begin
```
```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #2:5 begin -1
```
---

C++11でrvalueの概念の整理やrvalueリファレンス、
std::move()の導入が行われた目的はプログラム実行速度の向上である。
以下のパターンの代入式の処理がどのように違うのかを見ることでrvalueやstd::moveの効果について説明する。

* [lvalueからの代入](---)
* [rvalueからの代入](---)
* [std::move(lvalue)からの代入](---)



#### lvalueからの代入
下記コードにより「[expression|lvalue](---)からの代入」を説明する。

```.cpp
    // @@@ example/term_explanation/rvalue_move_ut.cpp #0:0 begin -1
```

* 行１、２  
  str0、str1がそれぞれ初期化される
  ("hehe"を保持するバッファが生成され、それをstr1オブジェクトが所有する)。

* 行３  
  str1が所有している文字列バッファと同じ内容を持つ新しいバッファが確保され、
  その内容がコピーされstr0がそれを所有する。従って、"hehe"を保持するバッファが2つできる。この代入をcopy代入と呼ぶ。

![lvalueからの代入](plant_uml/rvalue_from_lvalue.png)


#### rvalueからの代入
下記コードにより「[expression|rvalue](---)からの代入」を説明する。

```.cpp
    // @@@ example/term_explanation/rvalue_move_ut.cpp #0:1 begin -1
```

* 行１  
  str0が「std::string()により作られたテンポラリオブジェクト」により初期化される。

* 行２の右辺  
 「"hehe"を保持するをstd::stringテンポラリオブジェクトが生成される。

* 行２の左辺  
  この例の場合、std::stringがmoveコンストラクタ／move代入演算子を提供しているため、
  下記図のようなバッファの所有が移し替えられるだけである(この代入をmove代入と呼ぶ)。

* 行３  
  テンポラリオブジェクトが解体されるが、heheバッファはstr0の所有になったためdeleteする必要がなく、
  実際には何もしない。move代入によって、文字列バッファの生成と破棄の回数がそれぞれ1回少なくなったため、
  実行速度は向上する(通常、new/deleteの処理コストは高い)。

![rvalueサポート](plant_uml/rvalue_from_rvalue.png)

#### std::move(lvalue)からの代入
下記コードにより「std::move(lvalue)からの代入」を説明する。

```.cpp
    // @@@ example/term_explanation/rvalue_move_ut.cpp #0:2 begin -1
```

* 行１  
  「[lvalueからの代入](---)」の行１と同じである。

* 行２  
  「[lvalueからの代入](---)」の行２と同じである。

* 行３  
  std::moveは単にrvalueリファレンスへのキャストを行うだけであり、ランタイム時の処理コストは発生しない。
  この例の場合、std::stringがmoveコンストラクタ／move代入演算子を提供しているため、
  下記図のようなバッファの所有が移し替えられるだけである(この代入もmove代入と呼ぶ)。
  この動作は「[rvalueからの代入](---)の行２の左辺」と同じであり、同様に速度が向上するが、その副作用として、
  str1への代入以外のアクセスは[未規定動作](---)であるため、避けるべきである
  (多くの実装では、str1.size() == 0となることが多いがこの動作は約束されない)。

![std::move(lvalue)からの代入](plant_uml/rvalue_from_move.png)


### forwardingリファレンス
関数テンプレートの型パラメータTに対して`T&&`として宣言された仮引数、
または型推論を伴うauto&&として宣言された変数を、forwardingリファレンスと呼ぶ
(この概念はC++14から存在し、慣用的にユニバーサルリファレンスと呼ばれていたが、
C++17から正式にforwardingリファレンスと命名された)。
forwardingリファレンスは一見rvalueリファレンスのように見えるが、
下記に示す通り、lvalueにもrvalueにもバインドできる
([リファレンスcollapsing](---)により、このようなバインドが可能になる)。

```cpp
    // @@@ example/term_explanation/universal_ref_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/universal_ref_ut.cpp #0:1 begin -1
```

下記のコードは[ジェネリックラムダ](---)の引数をforwardingリファレンスにした例である。

```cpp
    // @@@ example/term_explanation/universal_ref_ut.cpp #0:2 begin -1
```

通常、forwardingリファレンスはstd::forwardと組み合わせて使用される。


### ユニバーサルリファレンス
ユニバーサルリファレンスとは、「[forwardingリファレンス](---)」の通称、もしくは旧称である。

### perfect forwarding
perfect forwarding(完全転送)とは、引数の[expression|rvalue](---)性や
[expression|lvalue](---)性を損失することなく、
その引数を別の関数に転送する技術のことを指す。
通常は、[forwardingリファレンス](---)である関数の仮引数をstd::forwardを用いて、
他の関数に渡すことで実現される。

perfect forwardingの使用例を以下に示す。

```cpp
    // @@@ example/term_explanation/perfect_forwarding_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/perfect_forwarding_ut.cpp #0:1 begin -1
```

### リファレンスcollapsing
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
    // @@@ example/term_explanation/ref_collapsing_ut.cpp #0:0 begin -1
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
    // @@@ example/term_explanation/ref_collapsing_ut.cpp #0:1 begin
```

下記のコードにより、テンプレートパラメータに対するこの変換則を確かめることができる。

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp #0:2 begin -1
```

この機能がないC++03では、

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp #0:3 begin
```

ようなクラステンプレートに下記コードのようにリファレンス型を渡すとコンパイルエラーとなる。

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp #0:5 begin -1
```

この問題を回避するためには下記のようなテンプレートの特殊化が必要になる。

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp #0:4 begin
```

上記したようなクラステンプレートでのメンバエイリアスの宣言は、
[テンプレートメタプログラミング](https://ja.wikipedia.org/wiki/%E3%83%86%E3%83%B3%E3%83%97%E3%83%AC%E3%83%BC%E3%83%88%E3%83%A1%E3%82%BF%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0)
で頻繁に使用されるため、
このようなテンプレートの特殊化を不要にするリファレンスcollapsingは、
有用な機能拡張であると言える。

### danglingリファレンス
Dangling リファレンスとは、破棄後のオブジェクトを指しているリファレンスを指す。
このようなリファレンスにアクセスすると、[未定義動作](---)に繋がるに繋がる。

```cpp
    // @@@ example/term_explanation/dangling_ut.cpp #0:0 begin

    // @@@ example/term_explanation/dangling_ut.cpp #0:1 begin -1
```

### danglingポインタ
danglingポインタとは、[danglingリファレンス](---)と同じような状態になったポインタを指す。

### リファレンス修飾
[rvalue修飾](---)と[lvalue修飾](---)とを併せて、リファレンス修飾と呼ぶ。

#### rvalue修飾
下記GetString0()のような関数が返すオブジェクトの内部メンバに対する[ハンドル](---)は、
オブジェクトのライフタイム終了後にもアクセスすることができるため、
そのハンドルを通じて、
ライフタイム終了後のオブジェクトのメンバオブジェクトにもアクセスできてしまう。

ライフタイム終了後のオブジェクトにアクセスすることは未定義動作であり、
特にそのオブジェクトがrvalueであった場合、さらにその危険性は高まる。

こういったコードに対処するためのシンタックスが、lvalue修飾、rvalue修飾である。

下記GetString1()、GetString3()、GetString4()のようにメンバ関数をlvalue修飾やrvalue修飾することで、
rvalueの内部ハンドルを返さないようにすることが可能となり、上記の危険性を緩和することができる。

```cpp
    // @@@ example/term_explanation/ref_qualifiers_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/ref_qualifiers_ut.cpp #0:1 begin -1
```

#### lvalue修飾
[rvalue修飾](---)を参照せよ。


## 構文と制御構造          

### 属性構文
C++14から導入されたの属性構文は、[[属性名]]の形式で記述され、
特定のコード要素に対する追加情報やコンパイラへの指示を与えるためのものである。

|属性                 |C++Ver|効果                                               |
|---------------------|------|---------------------------------------------------|
|[[noreturn]]         |C++11 |関数が決して返らないことを示す                     |
|[[deprecated]]       |C++14 |関数や変数が非推奨であることを示しめす             |
|[[maybe_unused]]     |C++17 |変数や関数が未使用である警告の抑止                 |
|[[nodiscard]]        |C++17 |戻り値が無視されると警告                           |
|[[fallthrough]]      |C++14 |switch文のfallthroughの警告抑止                    |
|[[no_unique_address]]|C++20 |クラスや構造体のメンバに対して、メモリの最適化促進 |

```cpp
    // @@@ example/term_explanation/attr_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/attr_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/attr_ut.cpp #2:0 begin

    // @@@ example/term_explanation/attr_ut.cpp #2:1 begin -1
```
```cpp
    // @@@ example/term_explanation/attr_ut.cpp #3:0 begin -1
```

### 関数tryブロック
関数tryブロックとはtry-catchを本体とした下記のような関数のブロックを指す。

```cpp
    // @@@ example/term_explanation/func_try_block.cpp #0:0 begin
```

### 範囲for文
範囲for文は、

```cpp
    for ( for-range-declaration : for-range-initializer ) statement
```

このような形式で表され、C++17までは下記のような疑似コードに展開される。

```cpp
    {
        auto && __range = for-range-initializer;
        for ( auto __begin = begin-expr, __end = end-expr; __begin != __end; ++__begin ) {
        for-range-declaration = *__begin;
        statement
      }
    }
```

単純な範囲for文の使用例は下記の通りである。

```cpp
    // @@@ example/term_explanation/range_for_ut.cpp #0:0 begin -2
```

上記のコードは下記のように展開される。

```cpp
    // @@@ example/term_explanation/range_for_ut.cpp #0:1 begin -2
```


C++17以前は、上記のコードのコメントにある通り、`__begin`と`__end`が同一の型である前提であった。
C++17以降は、この規制が緩和されたため、以下のように展開されることになった。

```cpp
    {
        auto && __range = for-range-initializer;
        auto __begin = begin-expr;
        auto __end = end-expr;     // C++17までは、__begin と __endは同一である前提
        for ( ; __begin != __end; ++__begin ) {
            for-range-declaration = *__begin;
            statement
        }
    }
```

この規制緩和により、以下のようなコードが範囲for文で記述できるようになった。
下記のコードはこの緩和ルールの応用例である。

```cpp
    // @@@ example/term_explanation/range_for_ut.cpp #2:0 begin -2
```
上記のコードは下記のように展開される。

```cpp
    // @@@ example/term_explanation/range_for_ut.cpp #2:1 begin -2
```

### 構造化束縛
構造化束縛はC++17 から導入されたもので、std::tuppleやstd::pair、std::arrayなど、
構造体のメンバーを個別の変数に分解して簡潔に扱うことをできるようにするための機能である。

```cpp
    // @@@ example/term_explanation/structured_binding_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/structured_binding_ut.cpp #0:1 begin
```
```cpp
    // @@@ example/term_explanation/structured_binding_ut.cpp #0:2 begin
```
```cpp
    // @@@ example/term_explanation/structured_binding_ut.cpp #0:3 begin
```

### 初期化付きif/switch文
C++17で、if文とswitc文に初期化を行う構文が導入された。
これにより、変数をそのスコープ内で初期化し、その変数を条件式の評価に使用できる。
初期化された変数は、if文やswitch文のスコープ内でのみ有効であり、他のスコープには影響を与えない。

この構文は、従来のfor文で使用されていた初期化ステートメントを、if/switch文に拡張したものである。
この類似性が理解しやすいように、本節では、 敢えて以下のコード例で同じ関数、同じクラスを使用し、
対比できるようにした。

- [初期化付きfor文(従来のfor文)](---)
- [初期化付きwhile文(従来のwhile文)](---)
- [初期化付きif文](---)
- [初期化付きswitch文](---)


#### 初期化付きfor文(従来のfor文)
下記の疑似コードは従来のfor文の構造を表す。

```cpp
    for (init-statement; condition; post-statement) {
        // ループ処理
    }
```
上記のと同様の実際のfor文のコードを以下に示す。

```cpp
    // @@@ example/term_explanation/if_switch_init_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/if_switch_init_ut.cpp #1:0 begin -1
```

#### 初期化付きwhile文(従来のwhile文)
下記の疑似コードこの節で説明しようとしているwhile文の構造を表す(従来からのwhile文)。

```cpp
    while (type-specifier-seq declarator) {
        // 条件がtrueの場合の処理
    }
```

[初期化付きif文](---)/[初期化付きswitch文](---)はC++17から導入されたシンタックスであるが、
それと同様のシンタックスはwhileには存在しないが、
以下のコード例のように従来の記法は広く知られているため、念とため紹介する。

```cpp
    // @@@ example/term_explanation/if_switch_init_ut.cpp #2:0 begin -1
```

#### 初期化付きif文
下記の疑似コードこの節で説明しようとしているif文の構造を表す。

```cpp
    if (init-statement; condition) {
        // 条件がtrueの場合の処理
    }
```

上記と同様の構造を持つ実際のif文のコードを以下に示す。

```cpp
    // @@@ example/term_explanation/if_switch_init_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/if_switch_init_ut.cpp #3:0 begin -1
```

クラスの独自の[<=>演算子](---)を定義する場合、下記のように使用することができる。

```cpp
    // @@@ example/term_explanation/if_switch_init_ut.cpp #4:0 begin
```

#### 初期化付きswitch文
下記の疑似コードはこの節で説明しようとしているswitch文の構造を表す。

```cpp
    switch (init-statement; condition) {
        case value1:
            // 条件が value1 の場合の処理
            break;
        case value2:
            // 条件が value2 の場合の処理
            break;
        // その他のケース
    }

```
上記と同様の構造を持つ実際のswitch文のコードを以下に示す。

```cpp
    // @@@ example/term_explanation/if_switch_init_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/if_switch_init_ut.cpp #5:0 begin -1
```

## 言語拡張機能
### コルーチン
コルーチンはC++20から導入された機能であり、以下の新しいキーワードによりサポートされる。

* [co_await](---)
* [co_return](---)
* [co_yield](---)

#### co_await
co_awaitはコルーチンの非同期操作の一時停止と再開に使用される。
co_waitとco_returnを使用したコードを以下に示す。

```cpp
    // @@@ example/term_explanation_cpp20/co_await_ut.cpp #0:0 begin
```
以下単体テストコードによりに上記コルーチンの動作を示す。

```cpp
    // @@@ example/term_explanation_cpp20/co_await_ut.cpp #0:1 begin -1
```

上記のコルーチンと同じ機能を持つクラスのco_await/co_returnを使わない実装を以下に示す。

```cpp
    // @@@ example/term_explanation_cpp20/co_await_ut.cpp #1:0 begin
```

このクラスは当然ながら、前記のコルーチンの単体テストコードとほぼ同じになる。

```cpp
    // @@@ example/term_explanation_cpp20/co_await_ut.cpp #1:1 begin -1
```

C++20から導入されたco_await、co_return、TaskとC++17以前の機能のみを使用したコードの対比によって、
コルーチンのサポート機能により実装が容易になることが理解できるだろう。

#### co_return
co_returnはコルーチンの終了時に値を返すために使用される。
co_returnは通常[co_await](---)と同時に使われることが多い。

#### co_yield
co_yieldはコルーチンから値を返しつつ、
次の再開ポイントまで処理を中断する。これはジェネレーターの実装に便利である。

```cpp
    // @@@ example/term_explanation_cpp20/co_yield_ut.cpp #0:0 begin
```
このテストを以下に示す。

```cpp
    // @@@ example/term_explanation_cpp20/co_yield_ut.cpp #0:1 begin -1
```

[co_await](---)、co_returnの例でみたように、
co_yieldを使用したコルーチンと同じ機能を持つクラスのco_yieldを使わない実装を以下に示す。

```cpp
    // @@@ example/term_explanation_cpp20/co_yield_ut.cpp #1:0 begin
```

このクラスは当然ながら、前記のコルーチンの単体テストコードとほぼ同じになる。

```cpp
    // @@@ example/term_explanation_cpp20/co_yield_ut.cpp #1:1 begin -1
```

C++20から導入されたco_await、co_return、TaskとC++17以前の機能のみを使用したコードの対比によって、
コルーチンのサポート機能により実装が容易になることが理解できるだろう。

### モジュール
モジュールはC++20から導入された機能であり、以下の新しいキーワードによりサポートされる。

* module: モジュールを宣言する。独立した構造を持ち、名前の衝突を防ぐ。
* export: モジュール外部に公開する関数やクラスを指定する。公開しない要素はモジュール内に限定される。
* import: 他のモジュールをインポートして利用できる。従来の#includeと異なり、依存関係を最適化し、ビルド時間を短縮する。

以下にこれらのキーワードのコード例を示す。

まずは、同時に使われることが多い`module`と`export`の使用例を示す。

```cpp
    // @@@ example/module_cmake/type_traits.cppm #0:0 begin
```

最後に`import`の使用例を示す。

```cpp
    // @@@ example/module_cmake/main.cpp #0:0 begin
```

これらにより、モジュールは依存関係の管理、名前空間の分離、ビルド時間の短縮を実現し、
大規模プロジェクトでの保守性向上に貢献する。
が、一方ではC++のモジュールに対応してるビルドツールを使用することが望ましい。

### ラムダ式
ラムダ式に関する言葉の定義と例を示す。

* ラムダ式とは、その場で関数オブジェクトを定義する式。
* クロージャ(オブジェクト)とは、ラムダ式から生成された関数オブジェクト。
* クロージャ型とは、クロージャオブジェクトの型。
* キャプチャとは、ラムダ式外部の変数をラムダ式内にコピーかリファレンスとして定義する機能。
* ラムダ式からキャプチャできるのは、ラムダ式から可視である自動変数と仮引数(thisを含む)。
* [constexprラムダ](---)とはクロージャ型の[constexprインスタンス](---)。
* [ジェネリックラムダ](---)とは、C++11のラムダ式を拡張して、
  パラメータにautoを使用(型推測)できるようにした機能。

```cpp
    // @@@ example/term_explanation/lambda.cpp #0:0 begin -1
```

#### クロージャ
「[言語拡張機能|ラムダ式](---)」を参照せよ。

#### クロージャ型
「[言語拡張機能|ラムダ式](---)」を参照せよ。

#### 一時的ラムダ
一時的ラムダ(transient lambda)とは下記のような使い方をするラムダ式指す慣用用語である。

複雑な初期化を必要とするconstオブジェクトの生成をするような場合に有用なテクニックである。

```cpp
    // @@@ example/term_explanation/transient_lambda_ut.cpp #0:0 begin -1
```

#### transient lambda
「[ラムダ式|一時的ラムダ](---)」を参照せよ。


### 指示付き初期化
指示付き初期化(designated initialization)とは、C++20から導入されたシンタックスであり、
構造体やクラスのメンバを明示的に指定して初期化できるようにする機能である。
この構文により、コードの可読性と安全性が向上し、初期化漏れや順序の誤りを防ぐことができる。

まずは、この機能を有効に使えるクラス例を以下に示す。

```cpp
    // @@@ example/term_explanation_cpp20/designated_init_ut.cpp #0:0 begin 
```
```cpp
    // @@@ example/term_explanation_cpp20/designated_init_ut.cpp #0:1 begin -1
```

下記に示すように、[Polymorphic Memory Resource(pmr)](---)のpool_resourceの初期化には、
この機能を使うと可読性の改善が期待できる。

```cpp
    // @@@ example/term_explanation_cpp20/designated_init_ut.cpp #1:0 begin -2
```

指示付き初期化を使わない以下のコード例と上記を比べれば可読性の改善に議論の余地はないだろう。

```cpp
    // @@@ example/term_explanation_cpp20/designated_init_ut.cpp #1:1 begin -2
```


---

| **第9章**  | 
言語拡張機能                    
モジュール／ラムダ式／クロージャ型／コルーチン（co_await／co_yield／co_return）                                                                          | 「言語機能」節                                  |

| **第10章** | テンプレートと型推論                | SFINAE／コンセプト／CTAD／パラメータパック／畳み込み式／constexpr if／decltype(auto)／autoパラメータ                                                        | 「template強化機能」「型推論」節                     |
| **第11章** | 名前探索と継承構造                 | name lookup／ADL／two-phase lookup／hidden friend／using宣言／仮想継承／ドミナンス                                                             | 「name lookupと名前空間」節                      |
| **第12章** | 例外安全と仕様                   | noexcept／no-fail／強い保証／基本保証／例外安全保証の定義                                                                                          | 「エクセプション安全性の保証」節                         |
| **第13章** | 標準ライブラリとユーティリティ           | std::move／std::forward／type_traits群（is_same, enable_if等）／スマートポインタ／optional／variant／thread／mutex／atomic／lock_guard／scoped_lock | 「プログラミング概念と標準ライブラリ」節                     |
| **第14章** | 言語仕様の定義要素                 | ill-formed／well-formed／未定義動作／未規定動作／ODR／alignas／alignof／addressof／評価順序                                                         | 「C++その他」節（RVOなど実装依存は注記扱い）                |
| **付録A**  | 歴史的項目と実装依存                | RVO／SSO／heap allocation elision／std::rel_ops／Most Vexing Parse／トライグラフ／g++／clang++                                             | 「C++その他」「C++コンパイラ」節から抽出                  |
| **付録B**  | 用語定義・関連概念                 | 副作用（定義のみ）／フリースタンディング環境（規格での定義のみ）                                                                                              | 「ソフトウェア一般」節から抜粋                          |



| **第7章**  | 比較と等価性                    | ==演算子／<=>／三方比較／spaceship operator                                                                                             | 「比較演算子」「==演算子」節                          |




