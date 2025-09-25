# 用語解説

この章では、このドキュメントで使用する用語の解説をする。

___

__この章の構成__

<!-- index 1-4 -->

[このドキュメントの構成](---)に戻る。  

___

## 組み込み型とインスタンス

### 基本型
基本型(fundamental types)は、C++の標準で定義されている型で、
特別なキーワードを使用して直接宣言できる型の総称である。
[組み込み型](---)とも呼ばれることもある。

基本型は以下のに示した型によって構成される。

* [用語解説|組み込み型とインスタンス|算術型](---)
* [用語解説|組み込み型とインスタンス|汎整数型](---)
* [用語解説|組み込み型とインスタンス|浮動小数点型](---)
* void
* 上記した型のポインタ型

注:  
リファレンスは基本型に含まれない。

### 組み込み型
組み込み型(built-in types)は[基本型](---)(fundamental types)の別称。

### 算術型
算術型とは下記の型の総称である。

* [用語解説|組み込み型とインスタンス|汎整数型](---)(bool, char, int, unsigned int, long long等)
* [用語解説|組み込み型とインスタンス|浮動小数点型](---)(float、double、long double)

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
* [用語解説|組み込み型とインスタンス|整数型](---)(int、unsigned int、long等)

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

[一様初期化](---)を使用することで、
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
プロトタイプを持たない関数に[組み込み型とインスタンス|算術型](---)引数を渡す際に適用される昇格ルールの総称である。

デフォルト引数昇格には以下が含まれる。

- [汎整数型昇格](---)
- [浮動小数点型昇格](---)

### 縮小型変換
縮小型変換(Narrowing Conversion) とは、あるデータ型から別のデータ型に変換する際に、
変換先の型が元の型の表現範囲を完全にカバーしていない場合に発生する変換を指す。
主に[用語解説|組み込み型とインスタンス|整数型](---)や[用語解説|組み込み型とインスタンス|浮動小数点型](---)などの値を小さな範囲の型に変換する際に起こる。

```cpp
    // @@@ example/term_explanation/etc_ut.cpp #1:0 begin -1
```

[リスト初期化](---)を使うことで、このような変換によるバグの発生を防ぐことができる。


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

## enum
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
enum classは通常の[用語解説|enum](---)の問題を解決するためにC++11から導入された。

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
[一様初期化](---)を使用するべきだろう。

一部の例外を除くとunderlying typeを指定しないenumやenum classはコンパイル時にサイズが確定できないため、
前方宣言できないが、underlying typeを指定したenum、enum classは前方宣言することができる。

```cpp
    // @@@ example/term_explanation/enum_ut.cpp #3:0 begin
```

### std::byte
C++17で導入されたstd::byte型は、バイト単位のデータ操作に使用され、
[用語解説|組み込み型とインスタンス|整数型](---)としての意味を持たないため、型安全性を確保する。
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
### 特殊メンバ関数
特殊メンバ関数とは下記の関数を指す。

* デフォルトコンストラクタ
* copyコンストラクタ
* copy代入演算子
* moveコンストラクタ
* move代入演算子
* デストラクタ

以下のメンバ関数は特殊関数ではないが、C++20から特殊関数と同様に`=default`とすることで自動生成される。

* [==演算子](---)  
  クラス内のすべてのメンバが==をサポートしている場合、`= default`とすることで自動生成される。
* [<=>演算子](---)  
  すべてのメンバが[<=>演算子](---)での比較可能である場合、`= default`とすることで自動生成される。 

ユーザがこれらを一切定義しない場合、または一部のみを定義する場合、
コンパイラは、下記のテーブル等で示すルールに従い、特殊関数メンバの宣言、定義の状態をを定める。

左1列目がユーザによる各関数の宣言を表し、2列目以降はユーザ宣言の影響による各関数の宣言の状態を表す。  
下記表において、

* 「`= default`」とは、「コンパイラによってその関数が`= default`と宣言された」状態であることを表す。
* 「~~= default~~」とは、`= default`と同じであるが、バグが発生しやすいので推奨されない。
* 「宣言無し」とは、「コンパイラによってその関数が`= default`と宣言された状態ではない」ことを表す。
    * 「moveコンストラクタが`= default`と宣言された状態ではない」且つ
      「copyコンストラクタが宣言されている」場合、
      rvalueを使用したオブジェクトの初期化には、
      moveコンストラクタの代わりにcopyコンストラクタが使われる。
    * 「move代入演算子が`= default`と宣言された状態ではない」且つ
      「copy代入演算子が宣言されている」場合、
      rvalueを使用したオブジェクトの代入には、
      move代入演算子の代わりにcopy代入演算子が使われる。
* 「= delete」とは「コンパイラによってその関数が= deleteと宣言された」状態であることを表す。

|  user-defined  |default ctor|   dtor  |  copy ctor  | copy assign |move ctor|move assign|   `==`   |   `<=>`  |
|:--------------:|:----------:|:-------:|:-----------:|:-----------:|:-------:|:---------:|:--------:|:--------:|
|   undeclared   |  = default |= default|  = default  |  = default  |= default| = default |undeclared|undeclared|
|non-default ctor| undeclared |= default|  = default  |  = default  |= default| = default |undeclared|undeclared|
|  default ctor  |      -     |= default|  = default  |  = default  |= default| = default |undeclared|undeclared|
|      dtor      |  = default |    -    |~~= default~~|~~= default~~|= default| = default |undeclared|undeclared|
|    copy ctor   |  = default |= default|      -      |~~= default~~|= default| = default |undeclared|undeclared|
|   copy assign  |  = default |= default|~~= default~~|      -      |= default| = default |undeclared|undeclared|
|    move ctor   |  = default |= default|   = delete  |   = delete  |    -    | = default |undeclared|undeclared|
|   move assign  |  = default |= default|   = delete  |   = delete  |= default|     -     |undeclared|undeclared|
|      `==`      |      -     |    -    |      -      |      -      |    -    |     -     |     -    |undeclared|
|      `<=>`     |      -     |    -    |      -      |      -      |    -    |     -     |undeclared|     -    |


**テーブル注**  

* C++14以前と、C++17以降での仕様の差は以下のようになる。
    * C++14以前では、コピーコンストラクタやコピー代入演算子をユーザ定義すると、
      ムーブコンストラクタ／ムーブ代入演算子は自動生成されず` = delete`となる。
    * C++17以降では、コピー系をユーザ定義していても、ムーブ系は自動生成される(` = default`と同等になる)ことがある。
      コンパイラは「コピー系の存在」だけではムーブ系を削除しない。
      ただし、ムーブ不可能なメンバや基底がある場合は、結果的に` = delete`になる。
    * C++17以降では、` = default`された特殊メンバ関数は明示的に`noexcept`推定され、ムーブセマンティクスの活用がしやすくなる。
    * C++20以降では、比較演算子(`==, <=>`)も`= default`によって自動生成可能だが、特殊メンバ関数とは分類が異なるが、
      上記テーブルでは同じように扱う。
* ctor: コンストラクタを指す。
* dtor: デストラクタを指す。
* assign: 代入演算子（assignment）を指す。
* user-defined: この列の関数がユーザによって定義されていることを指す。
  従って、non-default ctorは、デフォルトコンストラクタでないコンストラクタが定義されている行を指す。
* undeclared: 特定の特殊メンバ関数がユーザによって宣言されていないことを指し、
  コンパイラによる自動生成もされていないことを指す。
* 「~~= default~~」とは、`= default`と同様に自動生成されるが、
  場合によっては不適切な挙動を引き起こす可能性があるため、推奨されない。


上記表より、下記のようなことがわかる。

* ユーザが上記6メンバ関数を一切宣言しない場合、それらはコンパイラにより暗黙に宣言、定義される。
* ユーザがcopyコンストラクタを宣言した場合、デフォルトコンストラクタは暗黙に宣言、定義されない。
* moveコンストラクタ、move代入演算子は、
  以下のいずれもが明示的に宣言されていない場合にのみ暗黙に宣言、定義される。
    * copyコンストラクタ
    * copy代入演算子(operator =)
    * moveコンストラクタ
    * move代入演算子
    * デストラクタ

* ユーザがmoveコンストラクタまたはmove代入演算子を宣言した場合、
  copyコンストラクタ、copy代入演算子は`= delete`される。


これらの特殊メンバ関数に対しての設計のガイドラインには、以下のようなものがある。

* [ゼロの原則(Rule of Zero)](---)
* [五の原則(Rule of Five)](---)

この2つの原則(ガイドライン)の使い分けに関しては、

* リソース管理を外部([RAII(scoped guard)](---)クラス)に任せられる場合: ゼロの法則を採用し、特殊メンバ関数を明示的に定義しない。
* リソースをクラス内で直接管理する場合: 五の法則を採用し、すべての特殊メンバ関数を適切に定義する。

とすることで安全で保守性性の高いコードを設計できる。

#### ゼロの原則(Rule of Zero)
「ゼロの原則」は、リソース管理を直接クラスで行わず、
リソース管理を専門とするクラス
(例: 標準ライブラリの[RAII(scoped guard)](---)クラス)に任せる設計ガイドラインを指す。
この法則に従うと、自身で特殊メンバ関数を定義する必要がななくなる。

```cpp
    // @@@ example/term_explanation/rule_of_zero_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/rule_of_zero_ut.cpp #0:1 begin -1
```

クラスがリソースを直接管理する場合、メモリリークや二重解放などのリスクを伴う。
上記のように信頼性の高いクラスに特殊メンバ関数の処理を任せることにより、
クラス自体にリソース管理の責任を持たせる必要がなくなる。

#### 五の原則(Rule of Five)
「五の原則」は、
クラスがリソース(例: 動的メモリやファイルハンドルなど)を管理する場合、
デフォルトコンストラクタを除く[用語解説|型とインスタンス|特殊メンバ関数](---)、
つまり以下の5つの関数をすべて適切に定義する必要があるという設計ガイドラインを指す。

* デストラクタ
* コピーコンストラクタ
* コピー代入演算子
* ムーブコンストラクタ
* ムーブ代入演算子

特殊メンバ関数の挙動を正しく定義しないと、
リソースの不適切な管理(例: メモリリーク、リソースの二重解放)を招く可能性がある。
自動生成されるメンバ関数では、
複雑なリソース管理の要件を満たせないことがある(「[シャローコピー](---)」参照)。

なお、「五の原則」は、「六の原則」と呼ばれることもある。
その場合、この原則が対象とする関数は、
[用語解説|型とインスタンス|特殊メンバ関数](---)のすべてとなる。

このガイドラインに従って、コピーやムーブを実装する場合、

* [等価性のセマンティクス](---)
* [copyセマンティクス](---)
* [moveセマンティクス](---)

に従わなけならない。


### トリビアル型
トリビアル型とは、

* 全ての[用語解説|型とインスタンス|特殊メンバ関数](---)がデフォルトである。
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

### オーバーライドとオーバーロードの違い
下記例では、Base::g()がオーバーロードで、Derived::f()がオーバーライドである
(Derived::g()はオーバーロードでもオーバーライドでもない(「[name-hiding](---)」参照))。


```cpp
    // @@@ example/term_explanation/override_overload_ut.cpp #0:0 begin
```

下記図の通り、

* BaseのインスタンスはBase用のvtblへのポインタを内部に持ち、
  そのvtblでBase::f()のアドレスを保持する。
* DerivedのインスタンスはDerived用のvtblへのポインタを内部に持ち、
  そのvtblでDerived::f()のアドレスを保持する。
* Base::g()、Base::g(int)、
  Derived::g()のアドレスはBaseやDerivedのインスタンスから辿ることはできない。

![vtbl](plant_uml/vtbl.png)

vtblとは仮想関数テーブルとも呼ばれる、仮想関数ポインタを保持するための上記のようなテーブルである
(「[ポリモーフィックなクラス](---)」参照)。

Base::f()、Derived::f()の呼び出し選択は、オブジェクトの表層の型ではなく、実際の型により決定される。
Base::g()、Derived::g()の呼び出し選択は、オブジェクトの表層の型により決定される。

```cpp
    // @@@ example/term_explanation/override_overload_ut.cpp #0:1 begin -1
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

## constexpr
constexprはC++11で導入されたキーワードで、
関数や変数をコンパイル時に評価可能にする。
これにより、定数計算がコンパイル時に行われ、
実行時のパフォーマンスが向上し、コンパイル時にエラーを検出できることがある。

### constexpr定数
C++11以前で定数を定義する方法は、

* マクロ定数
* [用語解説|enum](---)
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
   - 例外を投げない
   - アドレス取得演算子の使用が定数式の評価に限定される

3. その式の評価において:  
   - すべてのサブ式も定数式である
   - 使用されるすべての変数は定数式で初期化されている
   - 呼び出されるすべての関数はconstexpr関数である

このドキュメントでは慣用的に[constexpr定数](---)と呼んでいる概念が、コア定数式である。

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

### constexprラムダ
constexprラムダはC++17から導入された機能であり、以下の条件を満たした[言語機能|ラムダ式](---)である。

* 引数やラムダ式内の処理がコンパイル時に評価可能である必要がある。
  すべての処理はconstexpr関数のようにコンパイル時に確定する必要があり、
  動的な処理やランタイムでしか決定できないものは含めることができない。

* ラムダ内で使用される関数や式もconstexprでなければならない。
  たとえば、関数の呼び出しや算術演算は、コンパイル時に評価可能なものであることが求められる。

* ラムダキャプチャはconstexprに適合している必要がある。
  キャプチャする変数もコンパイル時に確定できるものに限られる。
  動的な変数をキャプチャすると、コンパイルエラーとなる。

* 例外処理 (try/catch/throw) が禁止されている。
  constexprラムダでは、例外処理を含むことはできない。

* 動的メモリの割り当て(new/delete) が禁止されている。
  これらの操作はコンパイル時には行えないため、constexprラムダでは使用できない。

```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #7:0 begin -1
```
```cpp
    // @@@ example/term_explanation/const_xxx_ut.cpp #7:1 begin -1
```

## オブジェクトと生成
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
[リスト初期化](---)による初期化方法がC++における初期化を統一的に扱えるように設計された概念を指さす。

### 初期化子リストコンストラクタ
初期化子リストコンストラクタ([リスト初期化](---)用のコンストラクタ)とは、
{}による[リスト初期化](---)をサポートするためのコンストラクタである。
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

### 非静的なメンバ変数の初期化
非静的なメンバ変数の初期化には下記の3つの方法がある。

* [NSDMI](---)
* [初期化子リストでの初期化](---)
* [コンストラクタ内での非静的なメンバ変数の初期値の代入](---)

同一変数に対して、
「[NSDMI](---)」と「[初期化子リストでの初期化](---)」
が行われた場合、その変数に対するNSDMIは行われない。

#### NSDMI
NSDMIとは、non-static data member initializerの略語であり、
下記のような非静的なメンバ変数の初期化子を指す。

```cpp
    // @@@ example/term_explanation/nsdmi.cpp #0:0 begin
```

#### 初期化子リストでの初期化
「非静的メンバ変数をコンストラクタの本体よりも前に初期化する」言語機能である。
メンバ変数は宣言された順序で初期化されるため、
初期化子リストでの順序は、実際の初期化の順序とは関係がない。

この機能を使うことで、メンバ変数の初期化処理が簡素に記述できる。
constメンバ変数は、初期化子リストでの初期化か[NSDMI](---)でしか初期化できない。

```cpp
    // @@@ example/term_explanation/nsdmi.cpp #0:1 begin
```

#### コンストラクタ内での非静的なメンバ変数の初期値の代入
この方法は単なる代入でありメンバ変数の初期化ではない。

[NSDMI](---)、
[初期化子リストでの初期化](---)で初期化できない変数を未初期化でない状態にするための唯一の方法である。

```cpp
    // @@@ example/term_explanation/nsdmi.cpp #0:2 begin
```

### オブジェクトの所有権
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
    // @@@ example/term_explanation/ambiguous_ownership_ut.cpp #0:0 begin -1
```

こういった問題に対処するためのプログラミングパターンを以下の
「[オブジェクトの排他所有](---)」と「[オブジェクトの共有所有](---)」で解説する。

#### オブジェクトの排他所有
オブジェクトの排他所有や、それを容易に実現するための
[std::unique_ptr](https://cpprefjp.github.io/reference/memory/unique_ptr.html)
の仕様をを説明するために、下記のようにクラスA、Xを定義する。

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:0 begin
```

下記に示した上記クラスの単体テストにより、
オブジェクトの所有権やその移動、
std::unique_ptr、std::move()、[expressionと値カテゴリ|rvalue](---)の関係を解説する。

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:1 begin -1
```

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:2 begin -1
```

![所有権1](plant_uml/unique_ownership_1.png)

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:3 begin -1
```

![所有権2](plant_uml/unique_ownership_2.png)

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:4 begin -1
```
![所有権3](plant_uml/unique_ownership_3.png)

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:5 begin -1
```

![所有権4](plant_uml/unique_ownership_4.png)

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:6 begin -1
```

![所有権5](plant_uml/unique_ownership_5.png)

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:7 begin -1
```

![所有権6](plant_uml/unique_ownership_6.png)


また、以下に見るようにstd::unique_ptrはcopy生成やcopy代入を許可しない。

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #1:0 begin -1
```

以上で示したstd::unique_ptrの仕様の要点をまとめると、以下のようになる。

* std::unique_ptrはダイナミックに生成されたオブジェクトを保持する。
* ダイナミックに生成されたオブジェクトを保持するstd::unique_ptrがスコープアウトすると、
  保持中のオブジェクトは自動的にdeleteされる。
* 保持中のオブジェクトを他のstd::unique_ptrにmoveすることはできるが、
  copyすることはできない。このため、下記に示すような不正な方法以外で、
  複数のstd::unique_ptrが1つのオブジェクトを共有することはできない。

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #2:0 begin -1
```

こういった機能によりstd::unique_ptrはオブジェクトの排他所有を実現している。

#### オブジェクトの共有所有
オブジェクトの共有所有や、それを容易に実現するための
[std::shared_ptr](https://cpprefjp.github.io/reference/memory/shared_ptr.html)
の仕様をを説明するために、下記のようにクラスA、Xを定義する。

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:0 begin
```

下記に示した上記クラスの単体テストにより、
オブジェクトの所有権やその移動、共有、
std::shared_ptr、std::move()、[expressionと値カテゴリ|rvalue](---)の関係を解説する。

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:1 begin -1
```

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:2 begin -1
```

![所有権1](plant_uml/shared_ownership_1.png)


```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:3 begin -1
```

![所有権2](plant_uml/shared_ownership_2.png)

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:4 begin -1
```

![所有権3](plant_uml/shared_ownership_3.png)

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:5 begin -1
```

![所有権4](plant_uml/shared_ownership_4.png)

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:6 begin -1
```

![所有権5](plant_uml/shared_ownership_5.png)


```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:7 begin -1
```

![所有権6](plant_uml/shared_ownership_6.png)


```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:8 begin -1
```

![所有権7](plant_uml/shared_ownership_7.png)

以上で示したstd::shared_ptrの仕様の要点をまとめると、以下のようになる。

* std::shared_ptrはダイナミックに生成されたオブジェクトを保持する。
* ダイナミックに生成されたオブジェクトを保持するstd::shared_ptrがスコープアウトすると、
  共有所有カウントはデクリメントされ、その値が0ならば保持しているオブジェクトはdeleteされる。
* std::shared_ptrを他のstd::shared_ptrに、
    * moveすることことで、保持中のオブジェクトの所有権を移動できる。
    * copyすることことで、保持中のオブジェクトの所有権を共有できる。
* 下記のようなコードはstd::shared_ptrの仕様が想定するセマンティクスに沿っておらず、
  [未定義動作](---)に繋がる。

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #1:0 begin -1
```

こういった機能によりstd::shared_ptrはオブジェクトの共有所有を実現している。


#### オブジェクトの循環所有
[std::unique_ptr](https://cpprefjp.github.io/reference/memory/unique_ptr.html)の使い方を誤ると、
以下のコード例が示すようにメモリーリークが発生する。

なお、この節の題名である「オブジェクトの循環所有」という用語は、
この前後の節がダイナミックに確保されたオブジェクトの所有の概念についての解説しているため、
この用語を選択したが、文脈によっては、「オブジェクトの循環参照」といった方がふさわしい場合もある。

まずは、リークが発生しないstd::unique_ptrの正しい使用例を示す。

```cpp
    // @@@ example/term_explanation/weak_ptr_ut.cpp #0:0 begin
```

上記のクラスの使用例を示す。下記をステップ1とする。

```cpp
    // @@@ example/term_explanation/weak_ptr_ut.cpp #1:0 begin -1
```

![shread_ptrメモリーリーク](plant_uml/shared_each_1.png)


上記の続きを以下に示し、ステップ2とする。

```cpp
    // @@@ example/term_explanation/weak_ptr_ut.cpp #1:1 begin -1
```

![shread_ptrメモリーリーク](plant_uml/shared_each_2.png)


上記の続きを以下に示し、ステップ3とする。

```cpp
    // @@@ example/term_explanation/weak_ptr_ut.cpp #1:2 begin -1
```

![shread_ptrメモリーリーク](plant_uml/shared_each_3.png)


上記の続きを以下に示し、ステップ4とする。

```cpp
    // @@@ example/term_explanation/weak_ptr_ut.cpp #1:3 begin -1
```

![shread_ptrメモリーリーク](plant_uml/shared_each_4.png)


このような動作により、`std::make_shared<>`で生成されたX、Yオブジェクトは解放される。

次にshare_ptrを使用し、循環する参照を作ったためにオブジェクトが解放されないコード例を示す。
まずはクラスの定義から。

```cpp
    // @@@ example/term_explanation/weak_ptr_ut.cpp #2:0 begin
```

上記のクラスの動作を以下に示したコードで示す。

```cpp
    // @@@ example/term_explanation/weak_ptr_ut.cpp #2:1 begin -1
```

x0のライフタイムに差を作るために新しいスコープを導入し、そのスコープ内で、y0を生成し、
X::Register`、`Y::Register`を用いて、循環を作ってしまう例(メモリーリークを起こすバグ)を示す。

```cpp
    // @@@ example/term_explanation/weak_ptr_ut.cpp #2:2 begin -1
```

![shread_ptrメモリーリーク](plant_uml/shared_cyclic.png)

下記のコードでは、y0がスコープアウトするが、そのタイミングでは、x0はまだ健在であるため、
Yオブジェクトの参照カウントは1になる(x0::y_が存在するため0にならない)。

```cpp
    // @@@ example/term_explanation/weak_ptr_ut.cpp #2:3 begin -1
```

![shread_ptrメモリーリーク](plant_uml/shared_cyclic_2.png)

次に、x0がスコープアウトし、そのタイミングではY::x_が健在であるため、
Xオブジェクトの参照カウントも1になる。このため、x0、y0がスコープアウトした状態でも、
X、Yオブジェクトの参照カウントは0にならず、従ってこれらのオブジェクトは解放されない
(shared_ptrは参照カウントが0->1に変化するタイミングで保持するオブジェクトを解放する)。

```cpp
    // @@@ example/term_explanation/weak_ptr_ut.cpp #2:4 begin -1
```

![shread_ptrメモリーリーク](plant_uml/shared_cyclic_3.png)

X、Yオブジェクトへのハンドルを完全に失った状態であり、X、Yオブジェクトを解放する手段はない。

#### std::weak_ptr
std::weak_ptrは、[スマートポインタ](---)の一種である。

std::weak_ptrは参照カウントに影響を与えず、共有所有ではなく参照のみを保持するのため、
[オブジェクトの循環所有](---)の問題を解決できる。

[オブジェクトの循環所有](---)で示した問題のあるクラスの修正版を以下に示す
(以下の例では、Xは前のままで、Yのみ修正した)。

```cpp
    // @@@ example/term_explanation/weak_ptr_ut.cpp #3:0 begin
    // @@@ example/term_explanation/weak_ptr_ut.cpp #3:1 begin
    // @@@ example/term_explanation/weak_ptr_ut.cpp #3:2 begin
```

このコードからわかるように修正版YはXオブジェクトを参照するために、
`std::weak_ptr<X>`を使用する。
`std::weak_ptr<X>`にアクセスする必要があるときに、
下記のY::DoSomething()関数の内部処理のようにすることで、
`std::weak_ptr<X>`オブジェクトから、
それと紐づいた`std::shared_ptr<X>`オブジェクトを生成できる。

なお、上記コードは[初期化付きif文](---)を使うことで、
生成した`std::shared_ptr<X>`オブジェクトのスコープを最小に留めている。

```cpp
    // @@@ example/term_explanation/weak_ptr_ut.cpp #3:1 begin
```

Xと修正版Yの単体テストによりメモリーリークが修正されたことを以下に示す。

```cpp
    // @@@ example/term_explanation/weak_ptr_ut.cpp #3:3 begin -1
```


### オブジェクトのライフタイム
オブジェクトは、以下のような種類のライフタイムを持つ。

* 静的に生成されたオブジェクトのライフタイム
* thread_localに生成されたオブジェクトのライフタイム
* newで生成されたオブジェクトのライフタイム
* スタック上に生成されたオブジェクトのライフタイム
* prvalue(「[expressionと値カテゴリ|rvalue](---)」参照)のライフタイム

なお、リファレンスの初期化をrvalueで行った場合、
そのrvalueはリファレンスがスコープを抜けるまで存続し続ける。

rvalueをバインドするリファレンスが存在しない状態で、
そのrvalueがメンバ変数へのリファレンスを返す関数を呼び出し、
そのリファレンスをバインドするリファレンス変数を初期化した場合、
リファレンスが指すオブジェクトはすでにライフタイムを終了している。
このような状態のリファレンスを[danglingリファレンス](---)と呼ぶ。
同様に、このような状態のポインタを[danglingポインタ](---)と呼ぶ。

### オブジェクトのコピー
#### シャローコピー
シャローコピー(浅いコピー)とは、暗黙的、
もしくは=defaultによってコンパイラが生成するようなcopyコンストラクタ、
copy代入演算子が行うコピーであり、[ディープコピー](---)と対比的に使われる概念である。

以下のクラスShallowOKには、コンパイラが生成するcopyコンストラクタ、
copy代入演算子と同等なものを定義したが、これは問題のないシャローコピーである
(が、正しく自動生成される関数を実装すると、メンバ変数が増えた際にバグを生み出すことがあるため、
実践的にはこのようなことはすべきではない)。

```cpp
    // @@@ example/term_explanation/deep_shallow_copy_ut.cpp #0:0 begin
```

コンストラクタでポインタのようなリソースを確保し、
デストラクタでそれらを解放するようなクラスの場合、シャローコピーは良く知られた問題を起こす。

下記のShallowNGはその例である。

```cpp
    // @@@ example/term_explanation/deep_shallow_copy_ut.cpp #1:0 begin
```

シャローコピーにより、メンバで保持していたポインタ(ポインタが指しているオブジェクトではない)
がコピーされてしまうため、下記のコード内のコメントで示した通り、
メモリリークや2重解放を起こしてしまう。

```cpp
    // @@@ example/term_explanation/deep_shallow_copy_ut.cpp #1:1 begin -1
```

#### ディープコピー
ディープコピーとは、[シャローコピー](---)が発生させる問題を回避したコピーである。

以下に例を示す。

```cpp
    // @@@ example/term_explanation/deep_shallow_copy_ut.cpp #2:0 begin
```

上記クラスのDeepは、copyコンストラクタ、copy代入演算子でポインタをコピーするのではなく、
ポインタが指しているオブジェクトを複製することにより、シャローコピーの問題を防ぐ。


#### スライシング
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
    // @@@ example/term_explanation/slice_ut.cpp #0:0 begin
```

```cpp
    // @@@ example/term_explanation/slice_ut.cpp #0:1 begin -1
```

copy代入演算子(=)によりコピーが行われた場合、=の両辺のオブジェクトは等価になるべきだが
(copy代入演算子をオーバーロードした場合も、そうなるように定義すべきである)、
スライシングが起こった場合、そうならないことが問題である(「[等価性のセマンティクス](---)」参照)。

下記にこの現象の発生メカニズムについて解説する。

1. 上記クラスBase、Derivedのメモリ上のレイアウトは下記のようになる。

![スライシングクラス](plant_uml/slicing_class.png)

2. 上記インスタンスd0、d1、d2、d2_refのメモリ上のレイアウトは下記のようになる。

![スライシング初期化](plant_uml/slicing_init.png)

3. d2 = d0をした場合の状態は下記のようになる。

![スライシング問題なし](plant_uml/slicing_normal.png)

4. 上記の状態でd2_ref = d1をした場合の状態は下記のようになる。

![スライシング問題](plant_uml/slicing_slicing.png)

d2.name1\_の値が元のままであるが(これがスライシングである)、その理由は下記の疑似コードが示す通り、
「d2_refの表層型がクラスBaseであるためd1もクラスBase(正確にはBase型へのリファレンス)へ変換された後、
d2_refが指しているオブジェクト(d2)へコピーされた」からである。

```cpp
    d2_ref.Base::operator=(d1);   // Base::operator=(Base const&)が呼び出される
                                  // 関数Base::operator=(Base const&)の中では、
                                  // d1の型はBase型のリファレンスとなる
```

次に示すのは、
「オブジェクトの配列をその基底クラスへのポインタに代入し、
そのポインタを配列のように使用した場合に発生する」スライシングと類似の現象である。

```cpp
    // @@@ example/term_explanation/slice_ut.cpp #1:0 begin
```

![スライシング配列](plant_uml/slicing_array.png)


## リテラル
プログラムに具体的な値を与えるための基本的な即値を指す。
例えば、1, 2, 1.0, true/false, nullptr, "literal string"など。

### 生文字列リテラル
下記の例にあるように正規表現をそのまま文字列リテラルとして表現するために、
C++11から導入された導入されたリテラル。

```cpp
    // @@@ example/term_explanation/literal_ut.cpp #0:0 begin
```

### 2進数リテラル
C++14以降では、0bまたは 0B をプレフィックスとして使うことで、2進数リテラルを表現できる。

```cpp
    // @@@ example/term_explanation/literal_ut.cpp #1:0 begin -1
```

### 数値リテラル
C++14では区切り文字'を使用し、数値リテラルを記述できるようになった。

```cpp
    // @@@ example/term_explanation/literal_ut.cpp #2:0 begin -1
```

### ワイド文字列
ワイド文字列リテラルを保持する型は下記のように定義された。

* char16_t: UTF-16エンコーディングのコード単位を扱う型。 u"..." というリテラルでUTF-16文字列を表す。
* char32_t: UTF-32エンコーディングのコード単位を扱う型。 U"..." というリテラルでUTF-32文字列を表す。
* char8_t: UTF-8エンコーディングのコード単位を扱う型。 u8"..." というリテラルでUTF-8文字列を表す。

```cpp
    // @@@ example/term_explanation/literal_ut.cpp #3:0 begin
```

### 16進浮動小数点数リテラル
16進浮動小数点数リテラルは、
C++17から導入された浮動小数点数を16進数で表現する方法である。
特に、ハードウェアや低レベルのプログラミングで、
浮動小数点数の内部表現を直接扱う際に便利である

```
    一般的な形式:
        0x[数字].[数字]p[指数]
        0x: 16進数を表すプレフィックス
        [数字]: 16進数の数字 (0-9, a-f, A-F)
        .: 小数点
        p: 指数部を表す
        [指数]: 10進数の指数

    例:
        0x1.2p3は下記に解説する

    リテラルの構成:
        0x: 16進数の開始を示す。
        1.2: 仮数部を表す。この部分は16進数。
        p3: 指数部を表す。この場合、2の3乗を意味すため、つまり8。

        1.2(16進数) =  1 + 2 / 16 = 1.125(10進数)
        1.125 * 8 = 9.0
```

```cpp
    // @@@ example/term_explanation/literal_ut.cpp #4:0 begin -1
```

### ユーザー定義リテラル
[ユーザ定義リテラル演算子](---)により定義されたリテラルを指す。

#### ユーザ定義リテラル演算子
ユーザ定義リテラル演算子とは以下のようなものである。

```cpp
    // @@@ example/term_explanation/user_defined_literal_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/user_defined_literal_ut.cpp #0:1 begin -1
```

#### std::string型リテラル
"xxx"sとすることで、std::string型のリテラルを作ることができる。

```cpp
    // @@@ example/term_explanation/user_defined_literal_ut.cpp #1:0 begin -1
```

#### std::chronoのリテラル
std::chronoのリテラルは以下のコードのように使用できる。

```cpp
    // @@@ example/term_explanation/literal_ut.cpp #5:0 begin
```

#### std::complexリテラル
std::complexリテラル以下のコードのように使用できる。

```cpp
    // @@@ example/term_explanation/literal_ut.cpp #6:0 begin -1
```

### ==演算子
クラスの==演算子の実装方法には、
[メンバ==演算子](---)、[非メンバ==演算子](---)の2つの方法がある。

#### メンバ==演算子
メンバ==演算子には、[非メンバ==演算子](---)に比べ、下記のようなメリットがある。

* メンバ変数へのアクセスが容易であるため、より実装が単純になりやすい。
* メンバ変数へのアクセスが容易であるため、パフォーマンスが向上する。
* インライン化し易い。

```cpp
    // @@@ example/term_explanation_cpp20/comparison_operator_ut.cpp #0:0 begin
```

すべてのメンバ変数に==演算子が定義されている場合、
C++20以降より、`=default`により==演算子を自動生成させることができるようになった。

```cpp
    // @@@ example/term_explanation_cpp20/comparison_operator_ut.cpp #5:0 begin
```


#### 非メンバ==演算子
非メンバ==演算子には、[メンバ==演算子](---)に比べ、下記のようなメリットがある。

* クラスをよりコンパクトに記述できるが、その副作用として、
  アクセッサやfriend宣言が必要になることがある。

```cpp
    // @@@ example/term_explanation_cpp20/comparison_operator_ut.cpp #1:0 begin
```

* [暗黙の型変換](---)を利用した以下に示すようなシンプルな記述ができる場合がある。

```cpp
    // @@@ example/term_explanation_cpp20/comparison_operator_ut.cpp #1:1 begin -2
```

すべてのメンバ変数に==演算子が定義されている場合、
C++20以降より、`=default`により==演算子を自動生成させることができるようになった。

```cpp
    // @@@ example/term_explanation_cpp20/comparison_operator_ut.cpp #6:0 begin
```

### 比較演算子
比較演算子とは、[==演算子](--)の他に、!=、 <=、>、>= <、>を指す。
C++20から導入された[<=>演算子](---)の定義により、すべてが定義される。

#### std::rel_ops
クラスに`operator==`と`operator<`の2つの演算子が定義されていれば、
それがメンバか否かにかかわらず、他の比較演算子 !=、<=、>、>= はこれらを基に自動的に導出できる。
std::rel_opsでは`operator==`と`operator<=` を基に他の比較演算子を機械的に生成する仕組みが提供されている。

次の例では、std::rel_opsを利用して、少ないコードで全ての比較演算子をサポートする例を示す。

```cpp
    // @@@ example/term_explanation_cpp20/comparison_operator_ut.cpp #0:1 begin -1
```

なお、std::rel_opsはC++20から導入された[<=>演算子](---)により不要になったため、
非推奨とされた。

#### std::tuppleを使用した比較演算子の実装方法
クラスのメンバが多い場合、[==演算子](---)で示したような方法は、
可読性、保守性の問題が発生する場合が多い。下記に示す方法はこの問題を幾分緩和する。

```cpp
    // @@@ example/term_explanation_cpp20/comparison_operator_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/term_explanation_cpp20/comparison_operator_ut.cpp #2:1 begin
```


#### <=>演算子
「[std::tuppleを使用した比較演算子の実装方法](---)」
で示した定型のコードはコンパイラが自動生成するのがC++規格のセオリーである。
このためC++20から導入されたのが<=>演算子`<=>`である。

```cpp
    // @@@ example/term_explanation_cpp20/comparison_operator_ut.cpp #3:0 begin
```
```cpp
    // @@@ example/term_explanation_cpp20/comparison_operator_ut.cpp #3:1 begin -1
```

定型の比較演算子では不十分である場合、<=>演算子を実装する必要が出てくる。
そのような場合に備えて、上記の自動生成コードの内容を敢えて実装して、以下に示す。

```cpp
    // @@@ example/term_explanation_cpp20/comparison_operator_ut.cpp #4:0 begin
```

#### 三方比較演算子
三方比較演算子とは[<=>演算子](---)を指す。

#### spaceship operator
spaceship operatorとは[<=>演算子](---)を指す。
この名前は`<=>`が宇宙船に見えることに由来としている。

## 構文
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

## 言語機能
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
* [constexpr|constexprラムダ](---)とはクロージャ型の[constexprインスタンス](---)。
* [ジェネリックラムダ](---)とは、C++11のラムダ式を拡張して、
  パラメータにautoを使用(型推測)できるようにした機能。

```cpp
    // @@@ example/term_explanation/lambda.cpp #0:0 begin -1
```

#### クロージャ
「[言語機能|ラムダ式](---)」を参照せよ。

#### クロージャ型
「[言語機能|ラムダ式](---)」を参照せよ。

#### 一時的ラムダ
一時的ラムダ(transient lambda)とは下記のような使い方をするラムダ式指す慣用用語である。

複雑な初期化を必要とするconstオブジェクトの生成をするような場合に有用なテクニックである。

```cpp
    // @@@ example/term_explanation/transient_lambda_ut.cpp #0:0 begin -1
```

#### transient lambda
「[言語機能|ラムダ式|一時的ラムダ](---)」を参照せよ。


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

## プログラミング概念と標準ライブラリ
### スマートポインタ
スマートポインタは、C++標準ライブラリが提供するメモリ管理クラス群を指す。
生のポインタの代わりに使用され、リソース管理を容易にし、
メモリリークや二重解放といった問題を防ぐことを目的としている。

スマートポインタは通常、所有権とスコープに基づいてメモリの解放を自動的に行う。
C++標準ライブラリでは、主に以下の3種類のスマートポインタが提供されている。

* **`std::unique_ptr`** 
   はダイナミックにアロケートされた[オブジェクトの排他所有](---)を表すために用いられる。  
* **`std::shared_ptr`** 
   はダイナミックにアロケート[オブジェクトの共有所有](---)を表現、管理するために用いられる。   
* **[std::weak_ptr](---)**
   は`std::shared_ptr`と組み合わせて使用される補助的なスマートポインタである。
   参照カウントに影響を与えず、[オブジェクトの循環所有](---)よるメモリリークを防ぐために用いられる。
   std::weak_ptr`はリソースへの弱い参照を保持し、リソースの有効性を確認する際に使用される。  
* `std::auto_ptr`はC++11以前に導入された初期のスマートポインタであるが、
   異常な[copyセマンティクス](---)を持つため、多くの誤用を生み出し、
   C++11から非推奨とされ、C++17から規格から排除された。

### コンテナ
データを格納し、
効率的に操作するための汎用的なデータ構造を提供するC++標準ライブラリの下記のようなクラス群である。

* [シーケンスコンテナ(Sequence Containers)](---)
* [連想コンテナ(Associative Containers)(---)
* [無順序連想コンテナ(Unordered Associative Containers)](---)
* [コンテナアダプタ(Container Adapters)](---)
* [特殊なコンテナ](---)

#### シーケンスコンテナ(Sequence Containers)
データが挿入順に保持され、順序が重要な場合に使用する。

| コンテナ                 | 説明                                                                |
|--------------------------|---------------------------------------------------------------------|
| `std::vector`            | 動的な配列で、ランダムアクセスが高速。末尾への挿入/削除が効率的     |
| `std::deque`             | 両端に効率的な挿入/削除が可能な動的配列                             |
| `std::list`              | 双方向リスト。要素の順序を維持し、中間の挿入/削除が効率的           |
| [std::forward_list](---) | 単方向リスト。軽量だが、双方向の操作はできない                      |
| `std::array`             | 固定長配列で、サイズがコンパイル時に決まる                          |
| `std::string`            | 可変長の文字列を管理するクラス(厳密には`std::basic_string`の特殊化) |

##### std::forward_list

```cpp
    // @@@ example/term_explanation/cotainer_ut.cpp #0:0 begin -1
```

#### 連想コンテナ(Associative Containers)
データがキーに基づいて自動的にソートされ、検索が高速である。

| コンテナ           | 説明                                             |
|--------------------|--------------------------------------------------|
| `std::set`         | 要素がソートされ、重複が許されない集合           |
| `std::multiset`    | ソートされるが、重複が許される集合               |
| `std::map`         | ソートされたキーと値のペアを保持。キーは一意     |
| `std::multimap`    | ソートされたキーと値のペアを保持。キーは重複可能 |

#### 無順序連想コンテナ(Unordered Associative Containers)
ハッシュテーブルを基盤としたコンテナで、順序を保証しないが高速な検索を提供する。

| コンテナ                  | 説明                                                   |
|---------------------------|--------------------------------------------------------|
| [std::unordered_set](---) | ハッシュテーブルベースの集合。重複は許されない         |
| `std::unordered_multiset` | ハッシュテーブルベースの集合。重複が許される           |
| [std::unordered_map](---) | ハッシュテーブルベースのキーと値のペア。キーは一意     |
| `std::unordered_multimap` | ハッシュテーブルベースのキーと値のペア。キーは重複可能 |
| [std::type_index](---)    | 型情報型を連想コンテナのキーとして使用するためのクラス |

##### std::unordered_set

```cpp
    // @@@ example/term_explanation/cotainer_ut.cpp #1:0 begin -1
```

##### std::unordered_map

```cpp
    // @@@ example/term_explanation/cotainer_ut.cpp #2:0 begin -1
```

##### std::type_index
std::type_indexはコンテナではないが、
型情報型を連想コンテナのキーとして使用するためのクラスであるため、この場所に掲載する。

```cpp
    // @@@ example/term_explanation/cotainer_ut.cpp #3:0 begin -1
```


#### コンテナアダプタ(Container Adapters)
特定の操作のみを公開するためのラッパーコンテナ。

| コンテナ              | 説明                                     |
|-----------------------|------------------------------------------|
| `std::stack`          | LIFO(後入れ先出し)操作を提供するアダプタ |
| `std::queue`          | FIFO(先入れ先出し)操作を提供するアダプタ |
| `std::priority_queue` | 優先度に基づく操作を提供するアダプタ     |

#### 特殊なコンテナ
上記したようなコンテナとは一線を画すが、特定の用途や目的のために設計された一種のコンテナ。

| コンテナ             | 説明                                                       |
|----------------------|------------------------------------------------------------|
| `std::span`          | 生ポインタや配列を抽象化し、安全に操作するための軽量ビュー |
| `std::bitset`        | 固定長のビット集合を管理するクラス                         |
| `std::basic_string`  | カスタム文字型をサポートする文字列コンテナ                 |

### std::optional
C++17から導入されたstd::optionalには、以下のような2つの用途がある。
以下の用途2から、
このクラスがオブジェクトのダイナミックなメモリアロケーションを行うような印象を受けるが、
そのようなことは行わない。
このクラスがオブジェクトのダイナミックな生成が必要になった場合、プレースメントnewを実行する。
ただし、std::optionalが保持する型自身がnewを実行する場合は、この限りではない。

1. 関数の任意の型の[戻り値の無効表現](---)を持たせる
2. [オブジェクトの遅延初期化](---)する(初期化処理が重く、
   条件によってはそれが無駄になる場合にこの機能を使う)

#### 戻り値の無効表現
```cpp
    // @@@ example/term_explanation/optional_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/optional_ut.cpp #0:1 begin -1
```

#### オブジェクトの遅延初期化
```cpp
    // @@@ example/term_explanation/optional_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/optional_ut.cpp #1:1 begin -1
```

### std::variant
std::variantは、C++17で導入された型安全なunionである。
このクラスは複数の型のうち1つの値を保持することができ、
従来のunionに伴う低レベルな操作の安全性の問題を解消するために設計された。

std::variant自身では、オブジェクトのダイナミックな生成が必要な場合でも通常のnewを実行せず、
代わりにプレースメントnewを用いる
(以下のコード例のようにstd::variantが保持する型自身がnewを実行する場合は、この限りではない)。

以下にstd::variantの典型的な使用例を示す。

```cpp
    // @@@ example/term_explanation/variant_ut.cpp #0:0 begin -1
```

std::variantとstd::visit([Visitor](---)パターンの実装の一種)を組み合わせた場合の使用例を以下に示す。

```cpp
    // @@@ example/term_explanation/variant_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/variant_ut.cpp #1:1 begin -1
```

## name lookupと名前空間
ここではname lookupとそれに影響を与える名前空間について解説する。

### ルックアップ
このドキュメントでのルックアップとは[name lookup](---)を指す。

### name lookup
[name lookup](https://en.cppreference.com/w/cpp/language/lookup)
とはソースコードで名前が検出された時に、その名前をその宣言と関連付けることである。
以下、name lookupの例を上げる。

下記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp #0:0 begin
```

以下のコードでの関数呼び出しf()のname lookupは、


```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp #1:0 begin -1
```

1. NS_LUをその前方で宣言された名前空間と関連付けする
2. f()呼び出しをその前方の名前空間NS_LUで宣言された関数fと関連付ける

という手順で行われる。

下記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp #0:1 begin
```

以下のコードでの関数呼び出しg()のname lookupは、


```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp #1:1 begin -1
```

1. NS_LUをその前方で宣言された名前空間と関連付けする
2. 名前空間NS_LU内で宣言された複数のgを見つける
3. g()呼び出しを、
   すでに見つけたgの中からベストマッチしたg(T const (&)[N])と関連付ける

という手順で行われる。

下記記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp #2:0 begin
```

以下のコードでの関数呼び出しToString()のname lookupは、

```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp #2:1 begin -1
```

1. ToString()呼び出しの引数xの型Xが名前空間NS_LUで定義されているため、
   ToStringを探索する名前空間にNS_LUを組み入れる(「[関連名前空間](---)」参照)
2. ToString()呼び出しより前方で宣言されたグローバル名前空間とNS_LUの中から、
   複数のToStringの定義を見つける
3. ToString()呼び出しを、
   すでに見つけたToStringの中からベストマッチしたNS_LU::ToStringと関連付ける

という手順で行われる。


### two phase name lookup
[two phase name lookup](https://en.cppreference.com/w/cpp/language/two-phase_lookup)
とはテンプレートをインスタンス化するときに使用される、下記のような2段階でのname lookupである。

1. テンプレート定義内でname lookupを行う(通常のname lookupと同じ)。
   この時、テンプレートパラメータに依存した名前
   ([dependent_name](https://en.cppreference.com/w/cpp/language/dependent_name))は
   name lookupの対象外となる(name lookupの対象が確定しないため)。
2. 1の後、テンプレートパラメータを展開した関数内で、
   [関連名前空間](---)の宣言も含めたname lookupを行う。

以下の議論では、

* 上記1のname lookupを1st name lookup
* 上記2のname lookupを2nd name lookup

と呼ぶことにする。

下記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #0:0 begin
```

以下のコードでのTypeNameのインスタンス化に伴うname lookupは、

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #0:1 begin -1
```

1. TypeName()呼び出しの引数xの型Xが名前空間NS_TPLUで宣言されているため、
   NS_TPLUをTypeNameを探索する[関連名前空間](---)にする。
2. TypeName()呼び出しより前方で宣言されたグローバル名前空間とNS_TPLUの中からTypeNameを見つける。
3. TypeNameは関数テンプレートであるためtwo phase lookupが以下のように行われる。
    1. TypeName内でのHeader(int{})の呼び出しは、1st name lookupにより、
       Header(long)の宣言と関連付けられる。
       Header(int)はHeader(long)よりもマッチ率が高い、
       TypeNameの定義より後方で宣言されているため、name lookupの対象外となる。
    2. TypeName内でのToType(t)の呼び出しに対しては、2nd name lookupが行われる。
       このためTypeName定義より前方で宣言されたグローバル名前空間と、
       tの型がNS_TPLU::Xであるため[関連名前空間](---)となったNS_TPLUがname lookupの対象となるが、
       グローバル名前空間内のToTypeは、
       NS_TPLU内でTypeNameより前に宣言されたtemplate<> ToTypeによって[name-hiding](---)が起こり、
       TypeNameからは非可視となるためname lookupの対象から外れる。
       このため、ToType(t)の呼び出しは、NS_TPLU::ToType(X const&)の宣言と関連付けられる。

という手順で行われる。

上と同じ定義、宣言がある場合の以下のコードでのTypeNameのインスタンス化に伴うname lookupは、

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #0:2 begin -1
```

1. NS_TPLUを名前空間と関連付けする
   (引数の型がintなのでNS_TPLUは[関連名前空間](---)とならず、NS_TPLUを明示する必要がある)。
2. TypeName()呼び出しより前方で宣言されたNS_TPLUの中からTypeNameを見つける。
3. TypeNameは関数テンプレートであるためtwo phase lookupが以下のように行われる。
    1. TypeName内でのHeader(int{})の呼び出しは、1st name lookupにより、
       前例と同じ理由で、Header(long)の宣言と関連付けられる。
    2. TypeName内でのToType(t)の呼び出しに対しては、2nd name lookupが行われる。
       tの型がintであるためNS_TPLUは[関連名前空間](---)とならず、通常のname lookupと同様に
       ToType(t)の呼び出し前方のグローバル名前空間とNS_TPLUがname lookupの対象になるが、
       グローバル名前空間内のToTypeは、
       NS_TPLU内でTypeNameより前に宣言されたtemplate<> ToTypeによって[name-hiding](---)が起こり、
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
サンプルコードを[g++](---)と[clang++](---)でコンパイルしている)。

以下に、two phase lookupにまつわるさらに驚くべきコード例を紹介する。
上と同じ定義、宣言がある場合の以下のコードの動作を考える。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #0:3 begin -1
```

NS_TPLU::TypeName(int{})のintをlongにしただけなので、この単体テストはパスしないが、
この単体テストコードの後(実際にはこのファイルのコンパイル単位の中のNS_TPLU内で、
且つtemplate<> ToTypeの宣言の後方であればどこでもよい)
に以下のコードを追加するとパスしてしまう。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #0:4 begin
```

この理由は、関数テンプレート内での2nd name lookupで選択された名前が関数テンプレートであった場合、
その特殊化の検索範囲はコンパイル単位内になることがあるからである
([template_specialization](https://en.cppreference.com/w/cpp/language/template_specialization)
によるとこの動作は未定義のようだが、
[g++](---)/[clang++](---)両方ともこのコードを警告なしでコンパイルする)。

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
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #1:1 begin
```

これは先に示したNS_TPLU::Xの例と極めて似ている。本質的な違いは、
TypeNameやToTypeがグローバル名前空間で宣言されていることのみである。
だが、下記の単体テストで示す通り、
TypeName内でのname lookupで関数オーバーライドToType(NS_TPLU2::Y const&)が選択されないのである。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #1:2 begin -1
```

ここまでの現象を正確に理解するには、
「two phase lookupの対象となる宣言」を下記のように、より厳密に認識する必要がある。

* TypeNameの中で行われる1st name lookupの対象となる宣言は下記の積集合である。
    * TypeNameと同じ名前空間内かグローバル名前空間内の宣言
    * TypeName定義位置より前方の宣言

* TypeNameの中で行われる2nd name lookupの対象となる宣言は下記の和集合である。
    * 1st name lookupで使われた宣言
    * TypeName呼び出しより前方にある[関連名前空間](---)内の宣言

この認識に基づくNS_TPLU2::Yに対するグローバルなTypeName内でのtwo phase name lookupは、

1. TypeName内に1st name lookupの対象がないため何もしない。
2. TypeName内の2nd name lookupに使用される[関連名前空間](---)NS_TPLU2は、
   ToType(NS_TPLU2::Y const&)の宣言を含まないため、この宣言は2nd name lookupの対象とならない。
   その結果、ToType(t)の呼び出しは関数テンプレートToType(T const&)と関連付けられる。

という手順で行われる。

以上が、TypeNameからToType(NS_TPLU2::Y const&)が使われない理由である。

ここまでで示したようにtwo phase name lookupは理解しがたく、
理解したとしてもその使いこなしはさらに難しい。

次のコードは、この難解さに翻弄されるのが現場のプログラマのみではないことを示す。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #2:0 begin
```

上記の宣言、定義があった場合、operator+の単体テストは以下のようになる。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #2:1 begin -2
```

このテストは当然パスするが、次はどうだろう？

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #2:2 begin -2
```

これまでのtwo phase name lookupの説明では、
operator+(NS_TPLU2::Y const& y, int i)はTypeNum内でのname lookupの対象にはならないため、
このテストはエラーとならなければならないが、[g++](---)ではパスしてしまう。
2nd name lookupのロジックにバグがあるようである。

有難いことに、[clang++](---)では仕様通りこのテストはエラーとなり、
当然ながら以下のテストはパスする(つまり、g++ではエラーする)。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #2:3 begin -2
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


### 実引数依存探索
実引数依存探索とは、argument-dependent lookupの和訳語であり、
通常はその略語である[ADL](---)と呼ばれる。

### ADL
ADLとは、関数の実引数の型が宣言されている名前空間(これを[関連名前空間](---)と呼ぶ)内の宣言が、
その関数の[name lookup](---)の対象になることである。

下記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/name_lookup_adl_ut.cpp #0:0 begin
```

以下のコードでのToStringの呼び出しに対するのname lookupは、

```cpp
    // @@@ example/term_explanation/name_lookup_adl_ut.cpp #0:1 begin -1
```

* ToStringの呼び出しより前方で行われているグローバル名前空間内の宣言
* ToStringの呼び出しより前方で行われているNS_ADL内の宣言

の両方を対象として行われる。
NS_ADL内の宣言がToStringの呼び出しに対するのname lookupの対象になる理由は、
ToStringの呼び出しに使われている実引数aの型AがNS_ADLで宣言されているからである。
すでに述べたようにこれをADLと呼び、この場合のNS_ADLを[関連名前空間](---)と呼ぶ。

ADLは思わぬname lookupによるバグを誘発することもあるが、
下記コードを見れば明らかなように、また、
多くのプログラマはそれと気づかずに使っていることからもわかる通り、
コードをより自然に、より簡潔に記述するための重要な機能となっている。

```cpp
    // @@@ example/term_explanation/name_lookup_adl_ut.cpp #0:2 begin -1
```

### 関連名前空間
関連名前空間(associated namespace)とは、
[ADL](---)(実引数依存探索)によってname lookupの対象になった宣言を含む名前空間のことである。


### 修飾付き関数呼び出し
修飾付き関数呼び出し(Qualified Call)は、
C++で関数やメンバ関数を明示的にスコープやクラス名で修飾して呼び出す方法である。
名前の曖昧性を回避し、特定の関数やクラスメンバを明確に選択する際に利用される。
これにより、意図しない[name lookup](---)を回避することができるため、可読性と安全性が向上する。
一方で、[ADL](---)が働かなくなるため、フレキシブルな[name lookup](---)ができなくなる。

```cpp
    // @@@ example/term_explanation/etc_ut.cpp #2:0 begin -1
```

### hidden-friend関数
hidden-friend関数(隠れたフレンド関数、あるいは単にhidden-friend)とは、

* クラスの内部で定義された、
* 名前空間スコープでの通常の[name lookup](---)できず、[ADL](---)のみでname lookupできる

friend関数のことを指す。このような性質から、non-namespace-visible friend関数と呼ばれることもある。

これにより、意図的に外部からのアクセスを制限し、
必要な場合にのみ利用されることを保証する設計が可能となる。

hidden-friend関数(隠れたフレンド関数)の目的は、

* カプセル化の強化：
  クラスの内部実装を外部から隠しつつ、特定の操作だけを許可する。
* 名前空間汚染の防止：
  関数が名前空間スコープに現れないため、他の名前と衝突しにくい。
* 最適化：
  コンパイラによる最適化を妨げることなく、特定の機能を提供する。

```cpp
    // @@@ example/term_explanation/hidden_friend_ut.cpp #0:0 begin 
```
```cpp
    // @@@ example/term_explanation/hidden_friend_ut.cpp #0:1 begin -1
```


### name-hiding
name-hidingとは
「前方の識別子が、その後方に同一の名前をもつ識別子があるために、
[name lookup](---)の対象外になる」現象一般をを指す通称である
([namespace](https://en.cppreference.com/w/cpp/language/namespace)参照)。

まずは、クラスとその派生クラスでのname-hidingの例を示す。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #0:0 begin
```

上記の関数fは一見オーバーロードに見えるが、そうではない。下記のコードで示したように、
Base::f()には、修飾しない形式でのDerivedクラス経由のアクセスはできない。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #0:1 begin
```

これは前述したように、
Base::fがその後方にあるDerived::f(int)によりname-hidingされたために起こる現象である
(name lookupによる探索には識別子が使われるため、シグネチャの違いはname-hidingに影響しない)。

下記のように[using宣言](---)を使用することで、
修飾しない形式でのDerivedクラス経由のBase::f()へのアクセスが可能となる。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/name_hiding.cpp #1:1 begin -1
```

下記コードは、名前空間でも似たような現象が起こることを示している。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #2:0 begin
```

この問題に対しては、下記のようにf(int)の定義位置を後方に移動することで回避できる。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #2:1 begin
```

また、先述のクラスでの方法と同様にusing宣言を使い、下記のようにすることもできる。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #2:2 begin
```

当然ながら、下記のようにf()の呼び出しを::で修飾することもできる。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #2:3 begin
```

修飾の副作用として「[two phase name lookup](---)」の例で示したような
[ADL](---)を利用した高度な静的ディスパッチが使用できなくなるが、
通常のソフトウェア開発では、ADLが必要な場面は限られているため、
デフォルトでは名前空間を使用して修飾を行うことにするのが、
無用の混乱をさけるための安全な記法であると言えるだろう。

次に、そういった混乱を引き起こすであろうコードを示す。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #3:0 begin
```

NS_B_Inner::g()内のf(int)の呼び出しはコンパイルできるが、
name-hidingが原因で、NS_B_Inner::h()内のf(int)の呼び出しはコンパイルできず、
名前空間で修飾することが必要になる。
一方で、ADLの効果で名前空間での修飾をしていないf(S_in_B)の呼び出しはコンパイルできる。

全チームメンバがこういったname lookupを正しく扱えると確信できないのであれば、
前述の通り、デフォルトでは名前空間を使用して修飾を行うのが良いだろう。

### ダイヤモンド継承
ダイヤモンド継承(Diamond Inheritance)とは、以下のような構造のクラス継承を指す。

* 基底クラス(Base)が一つ存在し、その基底クラスから二つのクラス(Derived_0、Derived_1)が派生する。
* Derived_0とDerived_1からさらに一つのクラス(DerivedDerived)が派生する。
  したがって、DerivedDerivedはBaseの孫クラスとなる。

この継承は、多重継承の一形態であり、クラス図で表すと下記のようになるため、
ダイヤモンド継承と呼ばれる。

![diamond inheritance](plant_uml/diamond_inheritance.png)

ダイヤモンド継承は、
[仮想継承](---)(virtual inheritance)を使ったものと、使わないものに分類できる。

[仮想継承](---)を使わないダイヤモンド継承のコードを以下に示す。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp #0:1 begin -1
```

これからわかるように、DerivedDerivedインスタンスの中に2つのBaseインスタンスが存在する。
この状態をオブジェクト図で表すと下記のようになる。

![diamond inheritance](plant_uml/diamond_inheritance_obj.png)

下記コードは、それが原因で名前解決が曖昧になりコンパイルできない。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp #0:2 begin -1
```

この問題に対処するには、クラス名による修飾が必要になるが、
Baseインスタンスが2つ存在するため、下記に示すようなわかりづらいバグの温床となる。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp #0:3 begin -1
```

次に示すのは、[仮想継承](---)を使用したダイヤモンド継承の例である。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp #1:1 begin -1
```

仮想継承の効果で、DerivedDerivedインスタンスの中に存在するBaseインスタンスは1つになるため、
上で示した仮想継承を使わないダイヤモンド継承での問題は解消される
(が、[仮想継承](---)による別の問題が発生する)。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp #1:2 begin -1
```

この状態をオブジェクト図で表すと下記のようになる。

![diamond inheritance](plant_uml/diamond_inheritance_virtual_obj.png)

### 仮想継承
下記に示した継承方法を仮想継承、仮想継承の基底クラスを仮想基底クラスと呼ぶ。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #0:0 begin
```

仮想継承は、[ダイヤモンド継承](---)の基底クラスのインスタンスを、
その継承ヒエラルキーの中で1つのみにするための言語機能である。

仮想継承の独特の動作を示すため、
上記コードに加え、仮想継承クラス、通常の継承クラス、
それぞれを通常の継承したクラスを下記のように定義する。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #0:1 begin
```

この場合、継承ヒエラルキーに仮想継承を含むクラスと、含まないクラスでは、
以下に示したような違いが発生する。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #0:2 begin -1
```

この動作は、下記の仕様に起因している
(引数なしで呼び出せる基底クラスのコンストラクタがない場合はコンパイルエラー)。

__「仮想継承クラスを継承したクラスが、仮想継承クラスの基底クラスのコンストラクタを明示的に呼び出さない場合、
引数なしで呼び出せる基底クラスのコンストラクタが呼ばれる」__  

以下では、これを「仮想継承のコンストラクタ呼び出し」仕様と呼ぶことにする。

仮想継承クラスが、基底クラスのコンストラクタを呼び出したとしても、この仕様が優先されるため、
上記コードのような動作となる。

これを通常の継承クラスと同様な動作にするには、下記のようにしなければならない。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #0:3 begin
```
```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #0:4 begin -1
```
「仮想継承のコンストラクタ呼び出し」仕様は、
[ダイヤモンド継承](---)での基底クラスのコンストラクタ呼び出しを一度にするために存在する。

もし、この機能がなければ、下記のコードでの基底クラスのコンストラクタ呼び出しは2度になるため、
デバッグ困難なバグが発生してしまうことは容易に想像できるだろう。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #1:1 begin -1
```

基底クラスのコンストラクタ呼び出しは、下記のコードのようにした場合でも、
単体テストが示すように、一番最初に行われる。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #2:1 begin -1
```

このため、基底クラスのコンストラクタ呼び出しは下記のような順番で行うべきである。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #3:0 begin
```

### 仮想基底
仮想基底(クラス)とは、[仮想継承](---)の基底クラス指す。

### ドミナンス
[ドミナンス(Dominance、支配性)](https://en.wikipedia.org/wiki/Dominance_(C%2B%2B))とは、
「探索対称の名前が継承の中にも存在するような場合の[name lookup](---)の仕様の一部」
を指す慣用句である。

以下に

* [ダイヤモンド継承を含まない場合](---)
* [ダイヤモンド継承かつそれが仮想継承でない場合](---)
* [ダイヤモンド継承かつそれが仮想継承である場合](---)

のドミナンスについてのコードを例示する。

この例で示したように、[ダイヤモンド継承](---)を通常の継承で行うか、
[仮想継承](---)で行うかでは結果が全く異なるため、注意が必要である。

#### ダイヤモンド継承を含まない場合

```cpp
    // @@@ example/term_explanation/dominance_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/dominance_ut.cpp #0:1 begin -1
```

この[name lookup](---)については、[name-hiding](---)で説明した通りである。

#### ダイヤモンド継承かつそれが仮想継承でない場合

```cpp
    // @@@ example/term_explanation/dominance_ut.cpp #1:0 begin
```

上記コードはコードブロック内のコメントのようなメッセージが原因でコンパイルできない。

Derived_0のドミナンスにより、DerivedDerived::gはDerived_0::fを呼び出すように見えるが、
もう一つの継承元であるDerived_1が導入したDerived_1::f(実際には、Derived_1::Base::f)があるため、
Derived_1によるドミナンスも働き、その結果として、呼び出しが曖昧(ambiguous)になることで、
このような結果となる。

#### ダイヤモンド継承かつそれが仮想継承である場合

```cpp
    // @@@ example/term_explanation/dominance_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/term_explanation/dominance_ut.cpp #2:1 begin -1
```

これまでと同様にDerived_0のドミナンスによりBase::fは[name-hiding](---)されることになる。
この時、Derived_0、Derived_1がBaseから[仮想継承](---)した効果により、
この継承ヒエラルキーの中でBaseは１つのみ存在することになるため、
Derived_1により導入されたBase::fも併せて[name-hiding](---)される。
結果として、曖昧性は排除され、コンパイルエラーにはならず、このような結果となる。

### using宣言
using宣言とは、"using XXX::func"のような記述である。
この記述が行われたスコープでは、この記述後の行から名前空間XXXでの修飾をすることなく、
funcが使用できる。

```cpp
    // @@@ example/term_explanation/namespace_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/namespace_ut.cpp #0:1 begin
```

### usingディレクティブ
usingディレクティブとは、"using namespace XXX"のような記述である。
この記述が行われたスコープでは、下記例のように、この記述後から名前空間XXXでの修飾をすることなく、
XXXの識別子が使用できる。

```cpp
    // @@@ example/term_explanation/namespace_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/namespace_ut.cpp #0:2 begin
```

より多くの識別子が名前空間の修飾無しで使えるようになる点において、
[using宣言](---)よりも危険であり、また、
下記のように[name-hiding](---)された識別子の導入には効果がない。

```cpp
    // @@@ example/term_explanation/namespace_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/namespace_ut.cpp #0:3 begin
```

従って、usingディレクティブの使用は避けるべきである。

## template強化機能
### SFINAE
[SFINAE](https://cpprefjp.github.io/lang/cpp11/sfinae_expressions.html)
(Substitution Failure Is Not An Errorの略称、スフィネェと読む)とは、
「テンプレートのパラメータ置き換えに失敗した([ill-formed](---)になった)際に、
即時にコンパイルエラーとはせず、置き換えに失敗したテンプレートを
[name lookup](---)の候補から除外する」
という言語機能である。

### コンセプト
C++17までのテンプレートには以下のような問題があった。

* [SFINAE](---)による制約が複雑  
  テンプレートの制約を行うために、
  std::enable_ifやの仕組みを使う必要があり、コードが非常に複雑で難読になりがちだった。
* エラーメッセージが不明瞭  
  テンプレートのパラメータが不適切な型だった場合に、
  コンパイルエラーのメッセージが非常にわかりにくく、問題の原因を特定するのが困難だった。
* テンプレートの適用範囲が不明確  
  テンプレートの使用可能な型の範囲がドキュメントやコメントでしか表現されず、
  明確な制約がコードに反映されていなかったため、コードの意図が伝わりづらい。
* 部分特殊化やオーバーロードによる冗長性  
  特定の型に対するテンプレートの処理を制限するために、
  部分特殊化やテンプレートオーバーロードを行うことが多く、コードが冗長になりがちだった。

C++20から導入された「コンセプト(concepts)」は、
テンプレートパラメータを制約する機能である。
この機能を使用することで、以下のようなプログラミングでのメリットが得られる。

* テンプレートの制約を明確に定義できる  
  コンセプトを使うことで、テンプレートパラメータが満たすべき条件を宣言的に記述できるため、
  コードの意図が明確にできる。
* コンパイルエラーがわかりやすくなる  
  コンセプトを使用すると、テンプレートの適用範囲外の型に対して、
  より具体的でわかりやすいエラーメッセージが表示される。
* コードの可読性が向上する  
  コンセプトを利用することで、
  テンプレート関数やクラスのインターフェースが明確になり、可読性が向上する。

```cpp
    // @@@ example/term_explanation/concept_ut.cpp #0:0 begin

    // @@@ example/term_explanation/concept_ut.cpp #0:1 begin -1
```

```cpp
    // @@@ example/term_explanation/concept_ut.cpp #1:0 begin

    // @@@ example/term_explanation/concept_ut.cpp #1:1 begin -1
```

以下はテンプレートパラメータの制約にstatic_assertを使用した例である。

```cpp
    // @@@ example/term_explanation/concept_ut.cpp #2:0 begin
```

以上の関数テンプレートをコンセプトを使用して改善した例である。

```cpp
    // @@@ example/term_explanation/concept_ut.cpp #3:0 begin
```

フレキシブルに制約を記述するためにrequiresを使用したコード例を下記する。

```cpp
    // @@@ example/term_explanation/concept_ut.cpp #4:0 begin
```

### 畳み込み式
畳み式(fold expression)とは、C++17から導入された新機能であり、
可変引数テンプレートのパラメータパックに対して二項演算を累積的に行うためのものである。

畳み込み式のシンタックスの使用は下記のようなものである。
```
( pack op ... )          // (1) 単項右畳み込み
( ... op pack )          // (2) 単項左畳み込み
( pack op ... op init )  // (3) 二項右畳み込み
( init op ... op pack )  // (4) 二項左畳み込み
```

1. 単項右畳み込み
```cpp
    // @@@ example/term_explanation/flold_expression_ut.cpp #0:0 begin 
```
2. 単項左畳み込み
```cpp
    // @@@ example/term_explanation/flold_expression_ut.cpp #0:1 begin 
```
3. 二項右畳み込み
```cpp
    // @@@ example/term_explanation/flold_expression_ut.cpp #0:2 begin 
```
4. 二項左畳み込み
```cpp
    // @@@ example/term_explanation/flold_expression_ut.cpp #0:3 begin 
```

上記したような単純な例では、畳み込み式の効果はわかりづらいため、
もっと複雑なで読解が困難な再帰構造を持ったコードを以下に示す。

```cpp
    // @@@ example/term_explanation/flold_expression_ut.cpp #1:0 begin 
```
```cpp
    // @@@ example/term_explanation/flold_expression_ut.cpp #1:1 begin
```

畳み込み式を使うことで、この問題をある程度緩和したコードを下記する。

```cpp
    // @@@ example/term_explanation/flold_expression_ut.cpp #2:0 begin 
```
```cpp
    // @@@ example/term_explanation/flold_expression_ut.cpp #2:1 begin
```

### ジェネリックラムダ
ジェネリックラムダとは、C++11のラムダ式のパラメータの型にautoを指定できるようにした機能で、
C++14で導入された。

この機能により関数の中で関数テンプレートと同等のものが定義できるようになった。

ジェネリックラムダで定義されたクロージャは、通常のラムダと同様にオブジェクトであるため、
下記のように使用することもできる便利な記法である。

```cpp
    // @@@ example/term_explanation/generic_lambda_ut.cpp #0:0 begin
```

なお、上記のジェネリックラムダは下記クラスのインスタンスの動きと同じである。

```cpp
    // @@@ example/term_explanation/generic_lambda_ut.cpp #0:1 begin
```

### クラステンプレートのテンプレート引数の型推論
C++17から、
「コンストラクタに渡される値によって、クラステンプレートのテンプレート引数を推論する」
機能が導入された。

この機能がないC++14までは以下のように記述する必要があった。

```cpp
    // @@@ example/term_explanation/template_ut.cpp #0:0 begin -1
```

これに対して、この機能により、以下のようにシンプルに記述できるようになった。

```cpp
    // @@@ example/term_explanation/template_ut.cpp #0:1 begin -1
```

### テンプレートの型推論ガイド
テンプレートの型推論ガイド([CTAD(Class Template Argument Deduction)](---))は、
C++17で導入された機能である。この機能により、
クラステンプレートのインスタンス化時にテンプレート引数を明示的に指定せず、
引数から自動的に型を推論できるようになる。型推論ガイドを使用することで、
コードの可読性と簡潔性が向上する。

型推論ガイドがない場合、[クラステンプレートのテンプレート引数の型推論](---)は限定的であり、
明示的にテンプレート引数を指定する必要がある場合が多い。
一方、型推論ガイドを使用することで、
コンストラクタの引数からテンプレート引数を自動的に決定することが可能になる。

```cpp
    // @@@ example/term_explanation/deduction_guide_ut.cpp #0:0 begin
```
上記のクラステンプレートは、ガイドがない場合、
以下に示すように型推論によりテンプレート引数を決定することができない。

```cpp
    // @@@ example/term_explanation/deduction_guide_ut.cpp #0:1 begin -1
```

以上に示したクラステンプレートに以下の型推論ガイドを追加することにより、
テンプレート引数を型推論できるようになる。

```cpp
    // @@@ example/term_explanation/deduction_guide_ut.cpp #0:2 begin 
```
```cpp
    // @@@ example/term_explanation/deduction_guide_ut.cpp #0:3 begin -1
```

### CTAD(Class Template Argument Deduction)
CTAD(Class Template Argument Deduction)とは、[テンプレートの型推論ガイド](---)のことである。

### 変数テンプレート
変数テンプレートとは、下記のコード示したような機能である。

```cpp
    // @@@ example/term_explanation/template_ut.cpp #1:0 begin
```

なお、変数テンプレートはconstexprと定義されるが、
「定数テンプレート」ではなく変数テンプレートである。


### エイリアステンプレート
エイリアステンプレート(alias templates)とはC++11から導入され、
下記のコード例で示したようにテンプレートによって型の別名を定義する機能である。

```cpp
    // @@@ example/term_explanation/template_ut.cpp #2:0 begin
```

### constexpr if文
C++17で導入された[constexpr if文](https://cpprefjp.github.io/lang/cpp17/if_constexpr.html)とは、
文を条件付きコンパイルすることができるようにするための制御構文である。

まずは、この構文を使用しない例を示す。

```cpp
    // @@@ example/term_explanation/constexpr_if_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/constexpr_if_ut.cpp #0:1 begin -1
```

このような場合、[SFINAE](---)によるオーバーロードが必須であったが、
この文を使用することで、下記のようにオーバーロードを使用せずに記述できるため、
条件分岐の可読性の向上が見込める。

```cpp
    // @@@ example/term_explanation/constexpr_if_ut.cpp #0:2 begin
```

この構文は[パラメータパック](---)の展開においても有用な場合がある。

```cpp
    // @@@ example/term_explanation/constexpr_if_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/constexpr_if_ut.cpp #1:1 begin -1
```

C++14までの構文を使用する場合、
上記のようなオーバーロードとリカーシブコールの組み合わせが必要であったが、
constexpr ifを使用することで、やや単純に記述できる。

```cpp
    // @@@ example/term_explanation/constexpr_if_ut.cpp #1:2 begin
```

### autoパラメータによる関数テンプレートの簡易定義
この機能は、C++20から導入された。
下記のコードで示すように簡易的に関数テンプレートを定義するための機能である。

```cpp
    // @@@ example/term_explanation/decltype_ut.cpp #4:0 begin
```
```cpp
    // @@@ example/term_explanation/decltype_ut.cpp #4:1 begin -1
```


## 型推論
### AAAスタイル
このドキュメントでのAAAとは、単体テストのパターンarrange-act-assertではなく、
almost always autoを指し、
AAAスタイルとは、「可能な場合、型を左辺に明示して変数を宣言する代わりに、autoを使用する」
というコーディングスタイルである。
この用語は、Andrei Alexandrescuによって造られ、Herb Sutterによって広く推奨されている。

特定の型を明示して使用する必要がない場合、下記のように書く。

```cpp
    // @@@ example/term_explanation/aaa.cpp #0:0 begin -1
```

特定の型を明示して使用する必要がある場合、下記のように書く。

```cpp
    // @@@ example/term_explanation/aaa.cpp #1:0 begin -1
```

関数の戻り値を受け取る変数を宣言する場合、下記のように書く。

```cpp
    // @@@ example/term_explanation/aaa.cpp #2:0 begin -1
```

ただし、関数の戻り値型が容易に推測しがたい下記のような場合、
型を明示しないAAAスタイルは使うべきではない。

```cpp
    // @@@ example/term_explanation/aaa.cpp #2:1 begin -1
```

インライン関数や関数テンプレートの宣言は、下記のように書く。

```cpp
    // @@@ example/term_explanation/aaa.cpp #3:0 begin
```

ただし、インライン関数や関数テンプレートが複雑な下記のような場合、
AAAスタイルは出来る限り避けるべきである。

```cpp
    // @@@ example/term_explanation/aaa.cpp #3:1 begin
```

このスタイルには下記のような狙いがある。

* コードの安全性の向上  
  autoで宣言された変数は未初期化にすることができないため、未初期化変数によるバグを防げる。
  また、下記のように縮小型変換(下記では、unsignedからsignedの変換)を防ぐこともできる。

```cpp
    // @@@ example/term_explanation/aaa.cpp #4:0 begin -1
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
    // @@@ example/term_explanation/aaa.cpp #5:0 begin -1
```

### decltype
decltypeはオペランドに[expression](---)を取り、その型を算出する機能である。
下記のコードにあるようなautoの機能との微妙な差に気を付ける必要がある。

```cpp
    // @@@ example/term_explanation/decltype_ut.cpp #0:0 begin -1
```

decltypeは、テンプレートプログラミングに多用されるが、
クロージャ型(「[言語機能|ラムダ式](---)」参照)
のような記述不可能な型をオブジェクトから算出できるため、
下記例のような場合にも有用である。

```cpp
    // @@@ example/term_explanation/decltype_ut.cpp #0:1 begin
```

### decltype(auto)
decltype(auto)はC++14から導入されたdecltypeの類似機能である。

auto、decltype、decltype(auto)では、以下に示す通りリファレンスの扱いが異なることに注意する必要がある。

```cpp
    // @@@ example/term_explanation/decltype_ut.cpp #0:2 begin -1
```

### CTAD（Class Template Argument Deduction）

### 戻り値型を後置する関数宣言
関数の戻り値型後置構文は戻り値型をプレースホルダ(auto)にして、
実際の型を->で示して型推論させるシンタックスを指す。実際には関数テンプレートで使用されることが多い。
コード例を以下に示す。

```cpp
    // @@@ example/term_explanation/decltype_ut.cpp #1:0 begin
```

この構文をC++11から導入された理由は以下のコードを見れば明らかだろう。

```cpp
    // @@@ example/term_explanation/decltype_ut.cpp #1:1 begin
```

### 関数の戻り値型auto
C++14から導入された機能で、関数の戻り値の型をautoキーワードで宣言することで、
コンパイラがreturn文から自動的に型を推論してくれる機能である。
これにより、複雑な型の戻り値を持つ関数でも、より簡潔に記述できるようになる
(「[autoパラメータによる関数テンプレートの簡易定義](---)」を参照)。

```cpp
    // @@@ example/term_explanation/decltype_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/term_explanation/decltype_ut.cpp #2:1 begin -1
```

### 後置戻り値型auto
C++14から導入された[関数の戻り値型auto](---)と似た、
関数の戻り値の型を関数本体の後に-> autoと書くことでができる機能である。
autoプレースホルダーとし、そのプレースホルダーを修飾することで、戻り値型の推論を補助できる。

```cpp
    // @@@ example/term_explanation/decltype_ut.cpp #3:0 begin
```
```cpp
    // @@@ example/term_explanation/decltype_ut.cpp #3:1 begin -1
```


## explicit
explicitは、コンストラクタに対して付与することで、
コンストラクタによる暗黙の型変換を禁止するためのキーワードである。
暗黙の型変換とは、ある型の値を別の型の値に自動的に変換する言語機能を指す。
explicitキーワードを付けることで、意図しない型変換を防ぎ、コードの堅牢性を高めることがでできる。

この節で説明するexplicitの機能は下記のような項目に渡って説明を行う。

- [暗黙の型変換](---)
- [暗黙の型変換抑止](---)
- [explicit type operator()](---)
- [explicit(COND)](---)

### 暗黙の型変換
この節で扱う暗黙の型変換とは、
以下に示したような「非explicitなコンストラクタを持つクラス」による暗黙の型変換を指し、
[汎整数型昇格](---)や[算術変換](---)等を指さない。

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp #0:0 begin
```

上記のクラスPersonを使用して、下記のようなコードをコンパイルできるようにする機能である。

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp #0:1 begin
```

この記法は下記コードの短縮形であり、コードの見た目をシンプルに保つ効果がある。

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp #0:2 begin
```

この記法は下記のようにstd::string等のSTLでも多用され、その効果は十分に発揮されているものの、

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp #0:3 begin -2
```

以下のようなコードがコンパイルできてしまうため、わかりづらいバグの元にもなる。

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp #0:4 begin -2
```

下記のようにコンストラクタにexplicitを付けて宣言することにより、この問題を防ぐことができる。

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp #0:5 begin
```

std::stringは暗黙の型変換を許して良く、(多くの場合)Personには暗黙の型変換をしない方が良い理由は、

* std::stringの役割は文字列の管理と演算のみであるため、
  std::stringを文字列リテラルと等価なもののように扱っても違和感がない
* Personは、明らかに文字列リテラルと等価なものではない

といったセマンティクス的観点(「[シンタックス、セマンティクス](---)」参照)によるものである。

クラスPersonと同様に、
ほとんどのユーザ定義クラスには非explicitなコンストラクタによる暗黙の型変換は必要ない。

### 暗黙の型変換抑止
explicit宣言されていないコンストラクタを持つクラスは、
下記のコードのように[暗黙の型変換](---)が起こる。

```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #0:1 begin -1
```

暗黙の型変換はわかりづらいバグを生み出してしまうことがあるため、
下記のように適切にexplicitを使うことで、このような変換を抑止することができる。

```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #1:1 begin -1
```

C++03までは、[一様初期化](---)がサポートされていなかったため、
explicitは単一引数のコンストラクタに使用されることが一般的であった。

C++11からサポートされた[一様初期化](---)を下記のように使用することで、
暗黙の型変換を使用できる。

```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #2:1 begin -1
```

以下に示す通り、コンストラクタの引数の数によらず、
C++11からは暗黙の型変換を抑止したい型のコンストラクタにはexplicit宣言することが一般的となっている。

```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #3:0 begin
```
```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #3:1 begin -1
```

### explicit type operator()
型変換演算子のオーバーロードの戻り値をさらに別の型に変換すると、
きわめてわかりづらいバグを生み出してしまうことがあるため、
この機能を使用すると型変換演算子のオーバーロードの型変換の抑止することができる。

```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #4:0 begin
```
```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #4:1 begin -1
```

以下に示すようにexplicitを使うことで、このような暗黙の型変換を抑止できる。

```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #5:0 begin
```
```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #5:1 begin -1
```

### explicit(COND)
C++20から導入されたexplicit(COND)は、
コンストラクタや変換演算子に対して、
特定の条件下で暗黙の型変換を許可または禁止する機能である。
CONDには、型特性や定数式などの任意のconstexprな条件式を指定できる。
以下にこのシンタックスの単純な使用例を示す。

```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #6:0 begin
```
```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #6:1 begin -1
```

テンプレートのパラメータの型による暗黙の型変換の可否をコントロールする例を以下に示す。

```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #7:0 begin
```
```cpp
    // @@@ example/term_explanation/explicit_ut.cpp #7:1 begin -1
```

こういった工夫により、コードの過度な柔軟性を適度に保つことができ、
可読性の向上につながる。


## expressionと値カテゴリ
ここでは、expression(式)の値カテゴリや、それに付随した機能についての解説を行う。

### expression

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

![expression分類](plant_uml/rvalue.png)

ざっくりと言えば、lvalueとは代入式の左辺になり得る式、rvalueとは代入式の左辺にはなり得ない式である。
T const&は左辺になり得ないが、lvalueである。rvalueリファレンス(T&&)もlvalueであるため、
rvalueであることとrvalueリファレンスであることとは全く異なる。

xvalueとは、多くの場合、「std::move()の呼び出し式のことである」と考えても差し支えない。

prvalueとは、いわゆるテンポラリオブジェクトのことであるため
(下記のstd::string()で作られるようなオブジェクト)、名前はない。
また、アドレス演算子(&)のオペランドになれない。

```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #0:0 begin
```

C++11でrvalueの概念の整理やstd::move()の導入が行われた目的はプログラム実行速度の向上である。

* lvalueからの代入
* rvalueからの代入
* std::move(lvalue)からの代入

の処理がどのように違うのかを見ることでrvalueの効果について説明する。

1. 下記コードにより「lvalueからの代入」を説明する。

    ```.cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #0:1 begin -1
    ```

    * ステップ1。
      str0、str1がそれぞれ初期化される
      ("hehe"を保持するバッファが生成され、それをstr1オブジェクトが所有する)。

    * ステップ2。
      st1が所有している文字列バッファと等価のバッファが作られ
      (文字列バッファ用のメモリをnewし、文字列を代入)、
      str0がそれを所有する。従って、"hehe"を保持するバッファが2つできる。
      この代入をcopy代入と呼ぶ。

![lvalueからの代入](plant_uml/rvalue_from_lvalue.png)


2. 下記コードにより「rvalueからの代入」を説明する。

    ```.cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #0:2 begin -1
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

![rvalueサポート](plant_uml/rvalue_from_rvalue.png)

3. 下記コードにより「std::move(lvalue)からの代入」を説明する。

    ```.cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #0:3 begin -1
    ```

    * ステップ1。「lvalueからの代入」のステップ1と同じである。

    * ステップ2。
      std::move()の効果により(実際にはrvalueリファレンスへのキャストが行われるだけなので、
      実行時速度に影響はない)、"hehe"を保持する文字列バッファをもう1つ作る代わりに、
      str1が所有している文字列バッファをstr0の所有にする。
      この代入もmove代入と呼ぶ。
      この動作は「rvalueからの代入」と同じであり、同様に速度が向上するが、その副作用として、
      str1.size() == 0となる。

![std::move(lvalue)からの代入](plant_uml/rvalue_from_move.png)


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
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #0:4 begin
```

### lvalue
「[expression](---)」を参照せよ。

### rvalue
「[expression](---)」を参照せよ。

#### xvalue
「[expression](---)」を参照せよ。

#### prvalue
「[expression](---)」を参照せよ。

### rvalue修飾
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
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #1:1 begin -1
```

### lvalue修飾
[rvalue修飾](---)を参照せよ。


### リファレンス修飾
[rvalue修飾](---)と[lvalue修飾](---)とを併せて、リファレンス修飾と呼ぶ。


## リファレンス
ここでは、C++11から導入された

* [ユニバーサルリファレンス](---)
* [リファレンスcollapsing](---)

について解説する。

### ユニバーサルリファレンス
関数テンプレートの型パラメータや型推論autoに&&をつけて宣言された変数を、
ユニバーサルリファレンスと呼ぶ(C++17から「forwardingリファレンス」という正式名称が与えられた)。
ユニバーサルリファレンスは一見rvalueリファレンスのように見えるが、
下記に示す通り、lvalueにもrvalueにもバインドできる。

```cpp
    // @@@ example/term_explanation/universal_ref_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/universal_ref_ut.cpp #0:1 begin -1
```

下記のコードは[ジェネリックラムダ](---)の引数をユニバーサルリファレンスにした例である。

```cpp
    // @@@ example/term_explanation/universal_ref_ut.cpp #0:2 begin -1
```

通常、ユニバーサルリファレンスはstd::forwardと組み合わせて使用される。


### forwardingリファレンス
「[ユニバーサルリファレンス](---)」を参照せよ。

### perfect forwarding
perfect forwarding とは、引数の[expressionと値カテゴリ|rvalue](---)性や
[expressionと値カテゴリ|lvalue](---)性を損失することなく、
その引数を別の関数に転送する技術のことを指す。
通常は、[ユニバーサルリファレンス](---)である関数の仮引数をstd::forwardを用いて、
他の関数に渡すことで実現される。

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

## エクセプション安全性の保証
関数のエクセプション発生時の安全性の保証には以下の3つのレベルが規定されている。

* [no-fail保証](---)
* [強い安全性の保証](---)
* [基本的な安全性の保証](---)

### no-fail保証
「no-fail保証」を満たす関数はエクセプションをthrowしない。
no-failを保証する関数は、
[noexcept](---)を使用してエクセプションを発生させないことを明示できる。

標準テンプレートクラスのパラメータとして使用するクラスのメンバ関数には、
正確にnoexceptの宣言をしないと、
テンプレートクラスのメンバ関数によってはパフォーマンスを起こしてしまう可能性がある。

### 強い安全性の保証
「強い保証」を満たす関数は、この関数がエクセプションによりスコープから外れた場合でも、
この関数が呼ばれなかった状態と同じ(プログラムカウンタ以外の状態は同じ)であることを保証する。
従って、この関数呼び出しは成功したか、完全な無効だったかのどちらかになる。

### 基本的な安全性の保証
「基本的な安全性の保証」を満たす関数は、この関数がエクセプションによりスコープから外れた場合でも、
メモリ等のリソースリークは起こさず、
オブジェクトは(変更されたかもしれないが)引き続き使えることを保証する。

### noexcept
C++11で導入されたnoexceptキーワードには、以下の2つの意味がある。

* C++03までのthrowキーワードによる例外仕様の代替。
  関数がどの例外を送出する可能性があるかを列挙するのではなく、
  例外を送出する可能性があるかないかのみを指定する。

* sizeofと同じような形式で使用されるのような演算子としてのnoexceptは、
  noexcept(expression)の形式使用され、
  expressionがエクセプションを送出しないと宣言されている場合(noexceptと宣言された関数の呼び出し)、
  noexcept(expression)は静的にtrueとなる。

以下に上記のコード例を示す。

```cpp
    // @@@ example/term_explanation/noexcept_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/noexcept_ut.cpp #0:1 begin -1
```

演算子としてのnoexceptはテンプレートで頻繁に使用されるため、以下にそのような例を示す。

```cpp
    // @@@ example/term_explanation/noexcept_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/noexcept_ut.cpp #1:1 begin -1
```

### exception-unfriendly
以下のような関数  

* 初期化に関連する関数やコンストラクタ
    * 静的または thread_local な変数を初期化する関数やコンストラクタ
* 特殊メンバ関数
    * すべてのデストラクタ
    * すべての例外オブジェクトのコピーコンストラクタ
    * すべてのムーブコンストラクタ
    * すべてのムーブ代入演算子
* 特定の名前を持つ関数
    * "swap" という名前のすべての関数
* C言語との互換性を持つ関数
    * Cとのリンケージを持つすべての関数

の呼び出しでエクセプションがthrowされると、[未定義動作](---)や[未規定動作](---)が発生するため、
exception-unfriendly(エクセプションに不向き)であるとされる。
従って上記の関数は暗黙的または明示的に`noexcept`であることが求められる。

## シンタックス、セマンティクス
直訳すれば、シンタックスとは構文論のことであり、セマンティクスとは意味論のことである。
この二つの概念の違いをはっきりと際立たせる有名な文を例示する。

```
    Colorless green ideas sleep furiously(直訳:無色の緑の考えが猛烈に眠る)
```

この文は構文的には正しい(シンタックスは問題ない)が、
意味不明である(セマンティクスは誤り)。

C++プログラミングにおいては、コンパイルできることがシンタックス的な正しさであり、例えば

* クラスや関数がその名前から想起できる責務を持っている
    * 「[単一責任の原則(SRP)](---)」を満たしている
    * [Accessor](---)を実装する関数の名前は、GetXxxやSetXxxになっている
    * コンテナクラスのメンバ関数beginやendは、
      そのクラスが保持する値集合の先頭や最後尾の次を指すイテレータを返す
    * 等

* 「[等価性のセマンティクス](---)」を守ってる
* 「[copyセマンティクス](---)」を守ってる
* 「[moveセマンティクス](---)」を守っている

等がセマンティクス的な正しさである。

セマンティクス的に正しいソースコードは読みやすく、保守性、拡張性に優れている。

### 等価性のセマンティクス
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
    // @@@ example/term_explanation/semantics_ut.cpp #0:0 begin -2
```

しかし、下記のコード内のa、bは同じ値を持つが、
アドレスが異なるため同一のオブジェクトではないにもかかわらず、組み込みの==の値はtrueとなる。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #0:1 begin -2
```

このような場合、aとbは等価であるという。同一ならば等価であるが、等価であっても同一とは限らない。

ポインタや配列をオペランドとする場合を除き、C++における組み込みの==は、
数学の等号とは違い、等価を表していると考えられるが、
上記した3つの律を守っている。従ってオーバーロードoperator==も同じ性質を守る必要がある。

組み込みの==やオーバーロード[==演算子](---)のこのような性質をここでは「等価性のセマンティクス」と呼ぶ。

クラスAを下記のように定義し、

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #1:0 begin
```

そのoperator==を下記のように定義した場合、

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #1:1 begin
```

単体テストは下記のように書けるだろう。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #1:2 begin -2
```

これは、一応パスするが(処理系定義の動作を前提とするため、必ず動作する保証はない)、
下記のようにすると、パスしなくなる。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #1:3 begin -2
```

一般にポインタの等価性は、その値の同一性ではなく、
そのポインタが指すオブジェクトの等価性で判断されるべきであるが、
先に示したoperator==はその考慮をしていないため、このような結果になった。

次に、これを修正した例を示す。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #1:4 begin
```

ポインタをメンバに持つクラスのoperator==については、上記したような処理が必要となる。

次に示す例は、基底クラスBaseとそのoperator==である。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #2:0 begin
```

次の単体テストが示す通り、これ自体には問題がないように見える。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #2:1 begin -1
```

しかし、Baseから派生したクラスDerivedを

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #3:0 begin
```

のように定義すると、下記の単体テストで示す通り、等価性のセマンティクスが破壊される。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #3:1 begin -1
```

Derived用のoperator==を

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #4:0 begin
```

と定義しても、下記に示す通り部分的な効果しかない。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #4:1 begin -1
```

この問題は、「[型とインスタンス|RTTI](---)」使った下記のようなコードで対処できる。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #5:0 begin
```

下記に示す通り、このコードは、
[オープン・クローズドの原則(OCP)](---)にも対応した柔軟な構造を実現している。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #5:1 begin
```

前例では「両辺の型が等しいこと」が「等価であること」の必要条件となるが、
この要件が、すべてのoperator==に求められるわけではない。

次に示すのは、一見すると両辺の型が違うにもかかわらず、
等価性のセマンティクスを満たしている例である。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #6:0 begin -1
```

これは、文字列リテラルを第1引数に取るstd::stringのコンストラクタが非explicitであることによって、
文字列リテラルからstd::stringへの[暗黙の型変換](---)が起こるために成立する。

以上で見てきたように、等価性のセマンティクスを守ったoperator==の実装には多くの観点が必要になる。

### copyセマンティクス
copyセマンティクスとは以下を満たすようなセマンティクスである。

* a = bが行われた後に、aとbが等価である。
* a = bが行われた前後でbの値が変わっていない。

従って、これらのオブジェクトに対して[等価性のセマンティクス](---)
を満たすoperator==が定義されている場合、
以下を満たすようなセマンティクスであると言い換えることができる。

* a = bが行われた後に、a == bがtrueになる。
* b == b_preがtrueの時に、a = bが行われた後でもb == b_preがtrueとなる。

下記に示す通り、std::stringはcopyセマンティクスを満たしている。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #7:0 begin -2
```

一方で、std::auto_ptrはcopyセマンティクスを満たしていない。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #7:1 begin -2
```

この仕様は極めて不自然であり、std::auto_ptrはC++11で非推奨となり、C++17で規格から排除された。

下記の単体テストから明らかな通り、
「[等価性のセマンティクス](---)」で示した最後の例も、copyセマンティクスを満たしていない。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #7:2 begin -2
```

原因は、copy代入で[オブジェクトのコピー|スライシング](---)が起こるためである。


### moveセマンティクス
moveセマンティクスとは以下を満たすようなセマンティクスである(operator==が定義されていると前提)。

* copy代入の実行コスト >= move代入の実行コスト
* a == bがtrueの時に、c = std::move(a)が行われた場合、
    * b == cがtrueになる。
    * a == cはtrueにならなくても良い(aはmove代入により破壊されるかもしれない)。

  必須ではないが、「aがポインタ等のリソースを保有している場合、move代入後には、
  そのリソースはcに移動している」ことが一般的である(「[expressionと値カテゴリ|rvalue](---)」参照)。

* [no-fail保証](---)をする(noexceptと宣言し、エクセプションをthrowしない)。

moveセマンティクスはcopy代入後に使用されなくなるオブジェクト(主にrvalue)
からのcopy代入の実行コストを下げるために導入されたため、
下記のようなコードは推奨されない。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #8:0 begin
```

下記のコードのようにメンバの代入もできる限りmove代入を使うことで、
パフォーマンスの良い代入ができる。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #8:1 begin
```

### MoveAssignable要件
MoveAssignable要件は、C++において型がムーブ代入をサポートするために満たすべき条件を指す。
ムーブ代入はリソースを効率的に転送する操作であり、以下の条件を満たす必要がある。

1. リソースの移動  
   ムーブ代入では、リソース(動的メモリ等)が代入元から代入先へ効率的に転送される。

2. 有効だが未定義の状態  
   ムーブ代入後、代入元のオブジェクトは有効ではあるが未定義の状態となる。
   未定義の状態とは、破棄や再代入が可能である状態を指し、それ以外の操作は保証されない。

3. 自己代入の安全性  
   同一のオブジェクトをムーブ代入する場合でも、未定義動作やリソースリークを引き起こしてはならない。

4. 効率性  
   ムーブ代入は通常、コピー代入よりも効率的であることが求められる。
   これは、リソースの複製を避けることで達成される(「[moveセマンティクス](---)」参照)。

5. デフォルト実装  
   ムーブ代入演算子が明示的に定義されていない場合でも、
   クラスが一定の条件(例: ムーブ不可能なメンバが存在しないこと)を満たしていれば、
   コンパイラがデフォルトの実装(「[用語解説|型とインスタンス|特殊メンバ関数](---)」参照)を生成する。

### CopyAssignable要件
CopyAssignable要件は、C++において型がコピー代入をサポートするために満たすべき条件を指す。

1. 動作が定義されていること  
   代入操作は未定義動作を引き起こしてはならない。自己代入（同じオブジェクトを代入する場合）においても正しく動作し、リソースリークを引き起こさないことが求められる。

2. 値の保持  
   代入後、代入先のオブジェクトの値は代入元のオブジェクトの値と一致していなければならない。

3. 正しいセマンティクス  
   コピー代入によって代入元のオブジェクトが変更されてはならない(「[copyセマンティクス](---)」参照)。
   代入先のオブジェクトが保持していたリソース(例: メモリ)は適切に解放される必要がある。

4. デフォルト実装  
   コピー代入演算子が明示的に定義されていない場合でも、
   クラスが一定の条件(例: コピー不可能なメンバが存在しないこと)を満たしていれば、
   コンパイラがデフォルトの実装(「[用語解説|型とインスタンス|特殊メンバ関数](---)」参照)を生成する。

## C++その他
### 型特性キーワード
アライメントとは、
データが効率的にアクセスされるために特定のメモリアドレス境界に配置される規則である。
C++03までの規約では、アライメントのコントロールは実装依存した#pragmaなどで行っていた。

[alignas](---)、
[alignof](---)によりコンパイラの標準的な方法でアライメントのコントロールできるようになった。

#### alignof
C++11で導入されたキーワードで、型のアライメント要求を取得するために使用する。

```cpp
    // @@@ example/term_explanation/aliging_ut.cpp #0:0 begin -1
```

#### alignas
C++11で導入されたキーワードで、メモリのアライメントを指定するために使用する。

```cpp
    // @@@ example/term_explanation/aliging_ut.cpp #1:0 begin -1
```

#### addressof
addressofは、オブジェクトの「実際の」
アドレスを取得するために使用されるC++標準ライブラリのユーティリティ関数である。
通常、オブジェクトのアドレスを取得するには&演算子を使うが、
operator& がオーバーロードされている場合には、
&演算子ではオブジェクトのメモリ上の実際のアドレスを取得できない場合があり得る。
そのような場合にstd::addressofすることにより、
オーバーロードを無視して元のアドレスを確実に取得できる。

```cpp
    // @@@ example/term_explanation/aliging_ut.cpp #2:0 begin -1
```
```cpp
    // @@@ example/term_explanation/aliging_ut.cpp #2:1 begin -1
```

### 演算子のオペランドの評価順位

C++17で、演算子のオペランドに対する評価順序が明確に規定された。
それに対し、C++14までは、演算子のオペランド部分式の評価順序は[未規定動作](---)であった。
以下の表で示す演算子に関しては、オペランドaがオペランドbよりも先に評価される。

| 演算子               |説明                                                                   |
|:---------------------|:----------------------------------------------------------------------|
| a.b                  |メンバアクセス演算子                                                   |
| a->b                 |ポインタメンバアクセス演算子                                           |
| a->\*b               |メンバポインタアクセス演算子                                           |
| a(b1,  b2, b3)       |関数呼び出し、引数リストの評価順序は規定外)                            |
| b @= a               |代入演算子 = や複合代入演算子。@は+,-,/,&,\|など                       |
| a[b]                 |配列アクセス                                                           |
| a << b               |ビットシフト左演算子                                                   |
| a >> b               |ビットシフト右演算子                                                   |

C++11以前では、以下のコードの評価順序は未規定であったが、上記の通り定義された。

```cpp
    // @@@ example/term_explanation/etc_ut.cpp #0:0 begin -1
```

関数呼び出しにおける引数の式の評価順序は、上記の例a(b1, b2, b3)での評価順序は、
不定順で序列化される。これは、b1, b2, b3 が特定の順序で評価される保証はなく、
例えば b3, b2, b1 の順に評価されたり、
b2, b3, b1 で評価される可能性があることを意味する。
一方で一度評価が開始された場合、部分式間でインターリーブ（交差実行されることはない。
つまり、b1 の評価が完全に終わる前に b2 や b3 の評価が開始されることはない。

条件演算子式`condition ? expr1 : expr2`については、
最初の部分であるconditionがまず評価される。
conditionの評価結果に基づき、expr1または expr2 のどちらかが選択され、選択された側だけが評価される。  

```cpp
    // @@@ example/term_explanation/etc_ut.cpp #0:1 begin -1
```

なお、単項演算子のオペランドは1つであるため、優先順位の定義は不要である。

### 実引数/仮引数
引数(もしくは実引数、argument)、仮引数(parameter)とは下記のように定義される。

```cpp
    // @@@ example/term_explanation/argument.cpp #0:0 begin
```

### 単純代入
代入は下記のように分類される。

* 単純代入(=)
* 複合代入(+=，++ 等)

### ill-formed
[標準規格と処理系](https://cpprefjp.github.io/implementation-compliance.html)に詳しい解説があるが、

* [well-formed](---)(適格)とはプログラムが全ての構文規則・診断対象の意味規則・
  単一定義規則を満たすことである。
* ill-formed(不適格)とはプログラムが適格でないことである。

プログラムがwell-formedになった場合、そのプログラムはコンパイルできる。
プログラムがill-formedになった場合、通常はコンパイルエラーになるが、
対象がテンプレートの場合、事情は少々異なり、[SFINAE](---)によりコンパイルできることもある。

### well-formed
「[ill-formed](---)」を参照せよ。

### 未定義動作
未定義動作(Undefined Behavior)とは、
C++標準が特定の操作や状況に対して一切の制約を設けないケースである。
未定義動作が発生すると、プログラムの実行結果が予測できなくなり、
何が起こるかはコンパイラや環境によって異なる。
未定義動作を含むコードは、クラッシュやセキュリティの問題を引き起こす可能性がある。

```cpp
    // @@@ example/term_explanation/undefined_ut.cpp #0:0 begin -1
```

### 未規定動作
未規定動作(Unspecified Behavior)とは、C++標準がある操作の動作を完全には決めておらず、
複数の許容可能な選択肢がある場合でのコードの動作を指す。
未規定動作は、実装ごとに異なる可能性があり、標準は少なくとも「何らかの合理的な結果」を保証する。
つまり、動作が特定の範囲で予測可能だが、正確な挙動が処理系の実装に依存することになる。

```cpp
    // @@@ example/term_explanation/undefined_ut.cpp #1:0 begin -1
```

### 未定義動作と未規定動作
| 種類            |定義                                                               | 例                               | 結果                           |
|-----------------|-------------------------------------------------------------------|----------------------------------|--------------------------------|
|[未定義動作](---)|C++標準が全く保証しない動作                                        | ゼロ除算、配列範囲外アクセス     | 予測不能(クラッシュなど)       |
|[未規定動作](---)|C++標準が動作を定めていないが、いくつかの選択肢が許容されている動作| `int8_t` に収まらない値のキャスト| 実装依存(異なるが合理的な動作) |


### 被修飾型
被修飾型(unqualified type)とは、変数の宣言において付加される修飾子(const、
volatile など)やポインタやリファレンスなどの間接指定子を除いた素の型を指す。

修飾子(const、volatile)に注視しい場合、cv-被修飾型(cv-unqualified type)という場合もある。

例えば: 

|定義         |被修飾型|
|-------------|:------:|
|const A& a   |A       |
|volatile B& b|B       |
|const T* C   |C       |
|const D d    |D       |

見た目が類似する[修飾付き関数呼び出し](---)とは無関係である。

### one-definition rule
「[ODR](---)」を参照せよ。

### ODR
ODRとは、One Definition Ruleの略語であり、下記のようなことを定めている。

* どの翻訳単位でも、テンプレート、型、関数、またはオブジェクトは、複数の定義を持つことができない。
* プログラム全体で、オブジェクトまたは非インライン関数は複数の定義を持つことはできない。
* 型、テンプレート、外部インライン関数等、いくつかのものは複数の翻訳単位で定義することができる。

より詳しい内容がが知りたい場合は、
[https://en.cppreference.com/w/cpp/language/definition](https://en.cppreference.com/w/cpp/language/definition)
が参考になる。

### RVO(Return Value Optimization)
関数の戻り値がオブジェクトである場合、
戻り値オブジェクトは、その関数の呼び出し元のオブジェクトにコピーされた後、すぐに破棄される。
この「オブジェクトをコピーして、その後すぐにそのオブジェクトを破棄する」動作は、
「関数の戻り値オブジェクトをそのままその関数の呼び出し元で使用する」ことで効率的になる。
RVOとはこのような最適化を指す。

なお、このような最適化は、
[C++17から規格化](https://cpprefjp.github.io/lang/cpp17/guaranteed_copy_elision.html)された。


### SSO(Small String Optimization)
一般にstd::stringで文字列を保持する場合、newしたメモリが使用される。
64ビット環境であれば、newしたメモリのアドレスを保持する領域は8バイトになる。
std::stringで保持する文字列が終端の'\0'も含め8バイト以下である場合、
アドレスを保持する領域をその文字列の格納に使用すれば、newする必要がない(当然deleteも不要)。
こうすることで、短い文字列を保持するstd::stringオブジェクトは効率的に動作できる。

SOOとはこのような最適化を指す。

### heap allocation elision
C++11までの仕様では、new式によるダイナミックメモリアロケーションはコードに書かれた通りに、
実行されなければならず、ひとまとめにしたり省略したりすることはできなかった。
つまり、ヒープ割り当てに対する最適化は認められなかった。
ダイナミックメモリアロケーションの最適化のため、この制限は緩和され、
new/deleteの呼び出しをまとめたり省略したりすることができるようになった。

```cpp
    // @@@ example/term_explanation/heap_allocation_elision_ut.cpp #0:0 begin
```

この最適化により、std::make_sharedのようにstd::shared_ptrの参照カウントを管理するメモリブロックと、
オブジェクトの実体を1つのヒープ領域に割り当てることができ、
ダイナミックメモリアロケーションが1回に抑えられるため、メモリアクセスが高速化される。


### Most Vexing Parse
Most Vexing Parse(最も困惑させる構文解析)とは、C++の文法に関連する問題で、
Scott Meyersが彼の著書"Effective STL"の中でこの現象に名前をつけたことに由来する。

この問題はC++の文法が関数の宣言と変数の定義とを曖昧に扱うことによって生じる。
特にオブジェクトの初期化の文脈で発生し、意図に反して、その行は関数宣言になってしまう。

```cpp
    // @@@ example/term_explanation/most_vexing_parse_ut.cpp #0:0 begin

    // @@@ example/term_explanation/most_vexing_parse_ut.cpp #0:1 begin -1
```

[オブジェクトと生成|初期化子リストコンストラクタ](---)の呼び出しでオブジェクトの初期化を行うことで、
このような問題を回避できる。


### トライグラフ
トライグラフとは、2つの疑問符とその後に続く1文字によって表される、下記の文字列である。

```
    ??=  ??/  ??'  ??(  ??)  ??!  ??<  ??>  ??-
```

## C++コンパイラ
本ドキュメントで使用するg++/clang++のバージョンは以下のとおりである。

### g++
```
    g++ (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0
    Copyright (C) 2021 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

### clang++
```
    Ubuntu clang version 14.0.0-1ubuntu1
    Target: x86_64-pc-linux-gnu
    Thread model: posix
    InstalledDir: /usr/bin
```

## ソフトウェア一般
### フリースタンディング環境
[フリースタンディング環境](https://ja.wikipedia.org/wiki/%E3%83%95%E3%83%AA%E3%83%BC%E3%82%B9%E3%82%BF%E3%83%B3%E3%83%87%E3%82%A3%E3%83%B3%E3%82%B0%E7%92%B0%E5%A2%83)
とは、組み込みソフトウェアやOSのように、その実行にOSの補助を受けられないソフトウエアを指す。

### サイクロマティック複雑度
[サイクロマティック複雑度](https://ja.wikipedia.org/wiki/%E5%BE%AA%E7%92%B0%E7%9A%84%E8%A4%87%E9%9B%91%E5%BA%A6)
とは関数の複雑さを表すメトリクスである。
このメトリクスの解釈は諸説あるものの、概ね以下のテーブルのようなものである。

|サイクロマティック複雑度(CC)|複雑さの状態                              |
|:--------------------------:|:-----------------------------------------|
|           CC <= 10         |非常に良い構造                            |
|      11 < CC <  30         |やや複雑                                  |
|      31 < CC <  50         |構造的なリスクあり                        |
|      51 < CC               |テスト不可能、デグレードリスクが非常に高い|

### 凝集度
[凝集度](https://ja.wikipedia.org/wiki/%E5%87%9D%E9%9B%86%E5%BA%A6)
とはクラス設計の妥当性を表す尺度の一種であり、
「[凝集度の欠如](---)(LCOM)」というメトリクスで計測される。

* [凝集度の欠如](---)メトリクスの値が1に近ければ凝集度は低く、この値が0に近ければ凝集度は高い。
* メンバ変数やメンバ関数が多くなれば、凝集度は低くなりやすい。
* 凝集度は、クラスのメンバがどれだけ一貫した責任を持つかを示す。
* 「[単一責任の原則(SRP)](---)」を守ると凝集度は高くなりやすい。
* 「[Accessor](---)」を多用すれば、振る舞いが分散しがちになるため、通常、凝集度は低くなる。
   従って、下記のようなクラスは凝集度が低い。言い換えれば、凝集度を下げることなく、
   より小さいクラスに分割できる。
   なお、以下のクラスでは、```LCOM == 9```となっており、凝集性が欠如していることがわかる。

```cpp
    // @@@ example/term_explanation/lack_of_cohesion_ut.cpp #0:0 begin
```

良く設計されたクラスは、下記のようにメンバが結合しあっているため凝集度が高い
(ただし、「[Immutable](---)」の観点からは、QuadraticEquation::Set()がない方が良い)。
言い換えれば、凝集度を落とさずにクラスを分割することは難しい。
なお、上記の```LCOM == 9```なっているクラスを凝集性を高く、修正した例を以下に示す。

```cpp
    // @@@ example/term_explanation/lack_of_cohesion_ut.cpp #0:1 begin
```

#### 凝集度の欠如
[ソフトウェア一般|凝集度](---)の欠如(Lack of Cohesion in Methods/LCOM)とは、
クラス設計の妥当性を表す尺度の一種であり、`0 ～ 1`の値で表すメトリクスである。

LCOMの値が大きい(1か1に近い値)場合、「クラス内のメソッドが互いに関連性を持たず、
それぞれが独立した責務やデータに依存するため、クラス全体の統一性が欠けている」ことを表す。

クラスデザイン見直しの基準値としてLCOMを活用する場合、
[LCOMの評価基準](---)に具体的な推奨値を示す。

#### LCOMの評価基準
クラスデザイン良し悪しの基準値としてLCOMを活用する場合の推奨値を以下に示す。

| 凝集度の欠如(LCOM)  | クラスの状態 |
|:-------------------:|:------------:|
|       `LCOM <= 0.4` | 理想的な状態 |
|`0.4 <  LCOM <  0.6` | 要注意状態   |
|`0.6 <= LCOM`        | 改善必須状態 |


* `LCOM <= 0.4`  
  クラスが非常に凝集しており、[単一責任の原則(SRP)](---)を強く遵守している状態であるため、
  通常、デザインの見直しは不要である。

* `0.4 < LCOM < 0.6`  
  クラスの凝集度がやや弱くなり始めている。
  デザイン見直しの必要な時期が迫りつつあると考えるべきだろう。
  このタイミングであればリファクタリングは低コストで完了できるだろう。

* `0.6 <= LCOM`  
  クラス内のメソッド間の関連性が低く、凝集度が不十分である。
  メソッドが異なる責務にまたがっている可能性が高いため、
  一刻も早くデザインの見直しを行うべきだろう。


### Spurious Wakeup
[Spurious Wakeup](https://en.wikipedia.org/wiki/Spurious_wakeup)とは、
条件変数に対する通知待ちの状態であるスレッドが、その通知がされていないにもかかわらず、
起き上がってしまう現象のことを指す。

下記のようなstd::condition_variableの使用で起こり得る。

```cpp
    // @@@ example/term_explanation/spurious_wakeup_ut.cpp #0:0 begin
```

std::condition_variable::wait()の第2引数を下記のようにすることでこの現象を回避できる。

```cpp
    // @@@ example/term_explanation/spurious_wakeup_ut.cpp #1:0 begin
```

### 副作用
プログラミングにおいて、式の評価による作用には、
主たる作用とそれ以外の
[副作用](https://ja.wikipedia.org/wiki/%E5%89%AF%E4%BD%9C%E7%94%A8_(%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%A0))
(side effect)とがある。
式は、評価値を得ること(関数では「引数を受け取り値を返す」と表現する)が主たる作用とされ、
それ以外のコンピュータの論理的状態(ローカル環境以外の状態変数の値)を変化させる作用を副作用という。
副作用の例としては、グローバル変数や静的ローカル変数の変更、
ファイルの読み書き等のI/O実行、等がある。


### is-a
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
  この原則を簡単に説明すると、
  「派生クラスのオブジェクトは、
  いつでもその基底クラスのオブジェクトと置き換えても、
  プログラムの動作に悪影響を与えずに問題が発生してはならない」という設計の制約である。

 「is-a」の関係とは「一種の～」と言い換えることができることが多い.
ペンギンや九官鳥 は一種の鳥であるため、この関係を使用したコード例を次に示す。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp #0:0 begin
```

bird::flyのオーバーライド(penguin::fly)について、 リスコフの置換原則に反した例を下記する。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp #0:1 begin
    // @@@ example/term_explanation/class_relation_ut.cpp #0:2 begin -1
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
    // @@@ example/term_explanation/class_relation_ut.cpp #1:0 begin
```

この誤用を改めた例を以下に示す。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp #2:0 begin
    // @@@ example/term_explanation/class_relation_ut.cpp #2:1 begin -1
```

修正されたKyukancho はstd::string インスタンスをメンバ変数として持ち、
kyukanchoとstd::stringの関係を[has-a](---)の関係と呼ぶ。


### has-a
「has-a」の関係は、
あるクラスのインスタンスが別のクラスのインスタンスを構成要素として含む関係を指す。
つまり、あるクラスのオブジェクトが別のクラスのオブジェクトを保持している関係である。

例えば、CarクラスとEngineクラスがあるとする。CarクラスはEngineクラスのインスタンスを含むので、
CarはEngineを「has-a」の関係にあると言える。
通常、has-aの関係はクラス内でメンバ変数またはメンバオブジェクトとして実装される。
Carクラスの例ではCarクラスにはEngine型のメンバ変数が存在する。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp #3:0 begin
```

### is-implemented-in-terms-of
「is-implemented-in-terms-of」の関係は、
オブジェクト指向プログラミング（OOP）において、
あるクラスが別のクラスの機能を内部的に利用して実装されていることを示す概念である。
これは、あるクラスが他のクラスのインターフェースやメソッドを用いて、
自身の機能を提供する場合に使われる。
[has-a](---)の関係は、is-implemented-in-terms-of の関係の一種である。

is-implemented-in-terms-ofは下記の手段1-3に示した方法がある。

*手段1.* [public継承によるis-implemented-in-terms-of](---)  
*手段2.* [private継承によるis-implemented-in-terms-of](---)  
*手段3.* [コンポジションによる(has-a)is-implemented-in-terms-of](---)  

手段1-3にはそれぞれ、長所、短所があるため、必要に応じて手段を選択する必要がある。
以下の議論を単純にするため、下記のようにクラスS、C、CCを定める。

* S(サーバー): 実装を提供するクラス
* C(クライアント): Sの実装を利用するクラス
* CC(クライアントのクライアント): Cのメンバをを使用するクラス

コード量の観点から考えた場合、手段1が最も優れていることが多い。
依存関係の複雑さから考えた場合、CはSに強く依存する。
場合によっては、この依存はCCからSへの依存間にも影響をあたえる。
従って、手段3が依存関係を単純にしやすい。
手段1は[is-a](---)に見え、以下に示すような問題も考慮する必要があるため、
可読性、保守性を劣化させる可能性がある。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp #6:0 begin
    // @@@ example/term_explanation/class_relation_ut.cpp #6:1 begin -1
```

以上述べたように問題の多い手段1であるが、実践的には有用なパターンであり、
[CRTP(curiously recurring template pattern)](https://ja.wikibooks.org/wiki/More_C%2B%2B_Idioms/%E5%A5%87%E5%A6%99%E3%81%AB%E5%86%8D%E5%B8%B0%E3%81%97%E3%81%9F%E3%83%86%E3%83%B3%E3%83%97%E3%83%AC%E3%83%BC%E3%83%88%E3%83%91%E3%82%BF%E3%83%BC%E3%83%B3(Curiously_Recurring_Template_Pattern))
の実現手段でもあるため、一概にコーディング規約などで排除することもできない。


#### public継承によるis-implemented-in-terms-of
public継承によるis-implemented-in-terms-ofの実装例を以下に示す。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp #7:0 begin
    // @@@ example/term_explanation/class_relation_ut.cpp #7:1 begin -1
```

すでに述べたようにこの方法は、
[private継承によるis-implemented-in-terms-of](---)や、
[コンポジションによる(has-a)is-implemented-in-terms-of](---)
と比べコードがシンプルになる。 

#### private継承によるis-implemented-in-terms-of
private継承によるis-implemented-in-terms-ofの実装例を以下に示す。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp #4:0 begin
    // @@@ example/term_explanation/class_relation_ut.cpp #4:1 begin -1
```

この方法は、[public継承によるis-implemented-in-terms-of](---)が持つデストラクタ問題は発生せす、
[is-a](---)と誤解してしまう問題も発生しない。


#### コンポジションによる(has-a)is-implemented-in-terms-of
コンポジションによる(has-a)is-implemented-in-terms-ofの実装例を示す。

```cpp
    // @@@ example/term_explanation/class_relation_ut.cpp #5:0 begin
    // @@@ example/term_explanation/class_relation_ut.cpp #5:1 begin -1
```

この方は実装を利用するクラストの依存関係を他の2つに比べるとシンプルにできるが、
逆に実装例から昭なとおり、コード量が増えてしまう。


## 非ソフトウェア用語
### 割れ窓理論
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


### 車輪の再発明
[車輪の再発明](https://ja.wikipedia.org/wiki/%E8%BB%8A%E8%BC%AA%E3%81%AE%E5%86%8D%E7%99%BA%E6%98%8E)
とは、広く受け入れられ確立されている技術や解決法を（知らずに、または意図的に無視して）
再び一から作ること」を指すための慣用句である。
ソフトウェア開発では、STLのような優れたライブラリを使わずに、
それと同様なライブラリを自分たちで実装するような非効率な様を指すことが多い。


