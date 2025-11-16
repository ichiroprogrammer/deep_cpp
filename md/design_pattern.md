# デザインパターン

ソースコードを劣化させるアンチパターンには、以下のようなものがある。

* 大きすぎる関数、クラス、ファイル等のソフトウェア構成物
* 複雑怪奇な依存関係
* コードクローン

こういった問題は、良質なコードを書くという意識を持ったプログラマの継続的な努力と、
ソフトウェア工学の知識によって回避可能である。
本章ではその知識の一翼を担うイディオム、デザインパターンを解説、例示する。

なお、ここに挙げるイディオム、デザインパターンは「適切な場所に適用される場合、
ソースコードをよりシンプルに記述できる」というメリットがある一方で、
「不適切な場所に適用される場合、ソースコードの複雑度を不要に上げてしまう」という負の一面を持つ。
また、この章で取り扱うパターンを覚えたてのプログラマは、自分のスキルが向上したという一種の高揚感や顕示欲から、
それらをむやみに多用してしまう状態に陥ることがある。

___

__この章の構成__

[イディオム](---)  

- [ガード節(Early Return)](---)
- [RAII(scoped guard)](---)
- [Copy-And-Swap](---)
- [CRTP(curiously recurring template pattern)](---)
- [Accessor](---)
- [Immutable](---)
- [NVI(non virtual interface)](---)

[実装パターン](---)

- [Pimpl](---)
- [lightweight Pimpl](---)
- [BitmaskType](---)
- [Future](---)
- [Null Object](---)
- [Cでのクラス表現](---)

[オブジェクト生成系デザインパターン](---)

- [Singleton](---)
- [Named Constructor](---)
- [Clone(仮想コンストラクタ)](---)
- [Factory](---)

[オブジェクトの動作/協調に関するデザインパターン](---)

- [Templateメソッド](---)
- [Strategy](---)
- [State](---)
- [Observer](---)
- [Visitor](---)

[アーキテクチャパターン(システムレベルの構造)](---)

- [DI(dependency injection)](---)
- [MVC](---)


[インデックス](---)に戻る。  

___

## オブジェクト生成系デザインパターン
### Singleton
このパターンにより、特定のクラスのインスタンスをシステム全体で唯一にすることができる。
これにより、グローバルオブジェクトを規律正しく使用しやすくなる。

以下は、Singletonの実装例である。

```cpp
    // @@@ example/design_pattern/singleton_ut.cpp #0:0 begin
```

このパターンを使用する場合、以下に注意する。  

* Singletonはデザインパターンの中でも、特にパターン猿病を発生しやすい。
  Singletonは「ほとんどグローバル変数である」ことを理解した上で、控えめに使用する。
* Singletonを定義する場合、以下の二つを定義する。
    * インスタンスを返すstaticメンバ関数Inst()
    * constインスタンスを返すstaticメンバ関数InstConst()
* InstConst()は、Inst()が返すオブジェクトと同じオブジェクトを返すようにする。
* Singletonには、可能な限りInstConst()経由でアクセスする。

Singletonオブジェクトの初期化(最初のコンストラクタ呼び出し)は、
C++03以前はスレッドセーフでなかったため、「 Double Checked Lockingを使って競合を避ける」か、
「他のスレッドを起動する前にメインスレッドから各SingletonのInstConst()を呼び出す」
ことが必要であった。
C++11から上記例のようなSingletonオブジェクトのコンストラクタ呼び出しはスレッドセーフとなったため、
このような黒魔術が不要になった。

なお、Inst()のような関数を複数定義する場合、そのパターンはNamed Constructor
(「[Named Constructor](---)」参照)と呼ばれる。

[演習-Singleton](~~~)  

### Named Constructor
Named Connstructorは、[Singleton](---)のようなオブジェクトを複数、生成するためのパターンである。

```cpp
    // @@@ example/design_pattern/enum_operator.h #2:0 begin
```

上記例のHuman()、Bat()、Whale()は、人、コウモリ、クジラに対応するクラスMammalsオブジェクトを返す。

次に示したのは「[Factory](---)」の例にこのパターンを適応したコードである。

```cpp
    // @@@ example/design_pattern/template_method.h #0:1 begin
    // @@@ example/design_pattern/template_method.h #0:2 begin
    // @@@ example/design_pattern/template_method.h #0:4 begin
```

```cpp
    // @@@ example/design_pattern/template_method.cpp #2:0 begin
```

これまでにXxxDataFormatterIFオブジェクトを取得するパターンを以下のように3つ示した。

1. Factory関数によってstd::unique_ptr\<XxxDataFormatterIF>オブジェクトを返す。
2. Factory関数によってstaticなXxxDataFormatterIFオブジェクトを返す。
3. Named ConstructorによってstaticなXxxDataFormatterIFオブジェクトを返す。

最も汎用的な方法はパターン1であるが、
上記例のようにオブジェクトが状態を持たない場合、これは過剰な方法であり、
パターン3が最適であるように思える。このような考察からわかるように、
(単にnewする場合も含めて)オブジェクトの取得にどのような方法を用いるかは、
クラスの性質に依存する。

[演習-Named Constructor](~~~)  

### Clone(仮想コンストラクタ)
オブジェクトコピーによる[オブジェクトのコピー|スライシング](---)を回避するためのイデオムである。

下記は、オブジェクトコピーによるスライシングを起こしてしまう例である。

```cpp
    // @@@ example/design_pattern/clone_ut.cpp #0:0 begin
```

下記は、上記にcloneイデオムを適用した例である。

```cpp
    // @@@ example/design_pattern/clone_ut.cpp #0:1 begin
```

B1::Clone()やそのオーバーライドであるD1::Clone()を使うことで、
スライシングを起こすことなくオブジェクトのコピーを行うことができるようになった。

[演習-Clone](~~~)  

### Factory
Factoryは、専用関数(Factory関数)にオブジェクト生成をさせるためのパターンである。
オブジェクトを生成するクラスや関数をそのオブジェクトの生成方法に依存させたくない場合や、
オブジェクトの生成に統一されたルールを適用したい場合等に用いられる。
DI(「[DI(dependency injection)](---)」参照)と組み合わせて使われることが多い。

「[Templateメソッド](---)」の例にFactoryを適用したソースコードを下記する。

下記のXxxDataFormatterFactory関数により、

* XxxDataFormatterIFオブジェクトはstd::unique_ptrで保持されることを強制できる
* XxxDataFormatterIFから派生したクラスはtemplate_method.cppの無名名前空間で宣言できるため、
  これらのクラスは他のクラスから直接依存されることがない

といった効果がある。

```cpp
    // @@@ example/design_pattern/template_method.h #1:0 begin
```

```cpp
    // @@@ example/design_pattern/template_method.cpp #1:0 begin
```

以下に上記クラスの単体テストを示す。

```cpp
    // @@@ example/design_pattern/template_method_factory_ut.cpp #0:0 begin
```

一般にFactory関数はヒープを使用してオブジェクトを生成する場合が多いため、
それを例示する目的でXxxDataFormatterFactoryもヒープを使用している。

この例ではその必要はないため、ヒープを使用しないFactory関数の例を下記する。

```cpp
    // @@@ example/design_pattern/template_method.cpp #1:1 begin
```

次に示すのは、このパターンを使用して、プリプロセッサ命令を排除するリファクタリングの例である。

まずは、出荷仕分け向けのプリプロセッサ命令をロジックの内部に記述している問題のあるコードを示す。
このようなオールドスタイルなコードは様々な開発阻害要因になるため、避けるべきである。

```cpp
    // in shipping.h
    // @@@ example/design_pattern/factory_shipping.h #0:0 begin 0 -1
```
```cpp
    // in shipping_japan.h
    // @@@ example/design_pattern/factory_shipping_lib.cpp #0:0 begin 0 -1
```
```cpp
    // in xxx.cpp 仕分けに依存した処理
    // @@@ example/design_pattern/factory_shipping_lib.cpp #1:0 begin 0 -1
```

このコードは、
関数テンプレートの特殊化を利用したFactoryを以下のように定義することで改善することができる。

```cpp
    // in shipping.h
    // @@@ example/design_pattern/factory_shipping.h #1:0 begin 0 -1
    // @@@ example/design_pattern/factory_shipping.h #1:1 begin 0 -1
```
```cpp
    // in shipping_japan.cpp
    // ファクトリーの効果で、ShippingOp_Japanは外部への公開が不要
    // @@@ example/design_pattern/factory_shipping_lib.cpp #0:0 begin 0 -1
    // @@@ example/design_pattern/factory_shipping_lib.cpp #2:0 begin 0 -1
```
```cpp
    // in xxx.cpp 仕分けに依存した処理
    // @@@ example/design_pattern/factory_shipping_ut.cpp #1:1 begin -1 -1
```

もしくは、
関数オーバーロードを利用したFactoryを以下のように定義することで改善することもできる。

```cpp
    // in shipping.h
    // @@@ example/design_pattern/factory_shipping.h #1:0 begin 0 -1
    // @@@ example/design_pattern/factory_shipping.h #2:0 begin 0 -1
```
```cpp
    // in shipping_japan.cpp
    // ファクトリーの効果で、ShippingOp_Japanは外部への公開が不要
    // @@@ example/design_pattern/factory_shipping_lib.cpp #0:0 begin 0 -1
    // @@@ example/design_pattern/factory_shipping_lib.cpp #3:0 begin 0 -1
```
```cpp
    // in xxx.cpp 仕分けに依存した処理
    // @@@ example/design_pattern/factory_shipping_ut.cpp #1:2 begin -1 -1
```

[演習-Factory](~~~)  

## オブジェクトの動作/協調に関するデザインパターン

### Templateメソッド
Templateメソッドは、雛形の形式(書式等)を定めるメンバ関数(templateメソッド)と、
それを埋めるための振る舞いやデータを定めるメンバ関数を分離するときに用いるパターンである。

以下に実装例を示す。

```cpp
    // @@@ example/design_pattern/template_method.h #0:0 begin
    // @@@ example/design_pattern/template_method.h #0:1 begin
    // @@@ example/design_pattern/template_method.h #0:3 begin
    // @@@ example/design_pattern/template_method.h #0:4 begin
```

上記XxxDataFormatterIFでは、以下のようなメンバ関数を宣言、定義している。

|メンバ関数  |                      | 振る舞い                                                 |
|:-----------|:---------------------|:---------------------------------------------------------|
| header()   | private pure-virtual | ヘッダをstd::stringオブジェクトとして生成                |
| footer()   | private pure-virtual | フッタをstd::stringオブジェクトとして生成                |
| body()     | private pure-virtual | XxxDataからボディをstd::stringオブジェクトとして生成     |
| ToString() | public  normal       | header(),body(),footer()の出力を組み合わせた全体像を生成 |

この構造により、XxxDataFormatterIFは、

* 全体の書式を定義している。
* 各行の生成をXxxDataFormatterIFから派生した具象クラスに委譲している。

下記XxxDataFormatterXml、XxxDataFormatterCsv、XxxDataFormatterTableでは、
header()、body()、footer()をオーバーライドすることで、それぞれの機能を実現している。

```cpp
    // @@@ example/design_pattern/template_method.cpp #0:0 begin
```

以下の単体テストで、これらのクラスの振る舞いを示す。

```cpp
    // @@@ example/design_pattern/template_method_ut.cpp #0:0 begin
```

上記で示した実装例は、public継承による動的ポリモーフィズムを使用したため、
XxxDataFormatterXml、XxxDataFormatterCsv、XxxDataFormatterTableのインスタンスやそのポインタは、
XxxDataFormatterIFのリファレンスやポインタとして表現できる。
この性質は、[Factory](---)や[Named Constructor](---)の実装には不可欠であるが、
逆にこのようなポリモーフィズムが不要な場合、このよう柔軟性も不要である。

そういった場合、private継承を用いるか、
テンプレートを用いた静的ポリモーフィズムを用いることでこの柔軟性を排除できる。

下記のコードはそのような実装例である。

```cpp
    // @@@ example/design_pattern/template_method_ut.cpp #1:0 begin
```

上記の単体テストは下記のようになる。

```cpp
    // @@@ example/design_pattern/template_method_ut.cpp #1:1 begin
```

[演習-Templateメソッド](~~~)  

### Strategy
関数f(args)の振る舞いが、

* 全体の制御
* 部分的な振る舞い(何らかの条件を探す等)

に分けられるような場合、関数fを

* 「全体の制御」を行う関数g
* 「部分的な振る舞い」を規定するStrategyオブジェクト(関数へのポインタ、関数オブジェクト、ラムダ式)

に分割し、下記のように、Strategyオブジェクトをgの引数として外部から渡せるようにしたパターンである
(std::sort()のようなパターン)。

```cpp
    g(args, Strategyオブジェクト)
```

Strategyオブジェクトにいろいろなバリエーションがある場合、このパターンを使うと良い。
なお、このパターンの対象はクラスになる場合もある。

「ディレクトリをリカーシブに追跡し、引数で指定された属性にマッチしたファイルの一覧を返す関数」
を開発することを要求されたとする。

まずは、拡張性のない実装例を示す。

```cpp
    // @@@ example/design_pattern/find_files_old_style.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/find_files_old_style.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/find_files_ut.cpp #0:0 begin
```

この関数は、見つかったファイルが「引数で指定された属性」にマッチするかどうかを検査する。
検査は、「引数で指定された属性」に対するswitch文によって行われる。
これにより、この関数は「引数で指定された属性」の変更に強く影響を受ける。

下記は、この関数にStrategyパターンを適用したものである。

```cpp
    // @@@ example/design_pattern/find_files_strategy.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/find_files_strategy.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/find_files_ut.cpp #0:1 begin
```

捜査対象のファイル属性の指定をfind_files_recursively()の外に出しため、
その属性の追加に対して「[オープン・クローズドの原則(OCP)](---)」に対応した構造となった。

なお、上記find_files_recursivelyの第2パラメータをテンプレートパラメータとすることで、

```cpp
    // @@@ example/design_pattern/find_files_strategy.h #1:0 begin
```

のように書くこともできる。

次に示すのは、このパターンを使用して、プリプロセッサ命令を排除するリファクタリングの例である。

まずは、出荷仕分け向けのプリプロセッサ命令をロジックの内部に記述している問題のあるコードを示す。
このようなオールドスタイルなコードは様々な開発阻害要因になるため、避けるべきである。

```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #0:1 begin -1
```

このコードは、Strategyを使用し以下のようにすることで、改善することができる。

```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #1:1 begin
```
```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #1:2 begin -1
```

あるいは、[DI(dependency injection)](---)と組み合わせて、下記のような改善も有用である。

```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #2:1 begin
```
```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #2:2 begin -1
```

[演習-Strategy](~~~)  

### State
Stateは、オブジェクトの状態と、それに伴う振る舞いを分離して記述するためのパターンである。
これにより状態の追加、削減、変更に伴う修正範囲が限定される
(「[オープン・クローズドの原則(OCP)](---)」参照)。
またオブジェクトのインターフェース変更(パブリックメンバ関数の変更)に関しても、修正箇所が明確になる。

![ステートマシン図](plant_uml/state_machine.png)

上記ステートマシン図の「オールドスタイルによる実装」と、「stateパターンによる実装」、
それぞれを例示する。

まずは、下記にオールドスタイルな実装例を示す。
この実装では、状態を静的なenum変数thread_old_style_stateで管理するため、
ThreadOldStyleStateStr()、ThreadOldStyleRun()、ThreadOldStyleAbort()、ThreadOldStyleSuspend()
には、thread_old_style_stateに対する同型のswitch文が入ることになる(下記例では一部省略)。
これは醜悪で、バグを起こしやすい構造である。
ただし、要求される状態遷移がこの例程度であり、状態ごとに決められた振る舞いの数が少なければ、
この構造でも問題ないともいえる。

```cpp
    // @@@ example/design_pattern/state_machine_old.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/state_machine_old.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/state_machine_ut.cpp #0:0 begin
```

下記は、上記例へstateパターンを適用した例である。
まずは、stateパターンを形成するクラスの関係をクラス図で示す。

![クラス図](plant_uml/state_machine_class.png)

次に上記クラス図の実装例を示す。

```cpp
    // @@@ example/design_pattern/state_machine_new.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/state_machine_new.h #0:1 begin
```

```cpp
    // @@@ example/design_pattern/state_machine_new.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/state_machine_ut.cpp #1:0 begin
```

オールドスタイルな構造に比べると一見複雑に見えるが同型のswitch構造がないため、
状態の増減や振る舞いの変更等への対応が容易である。
一方で、前述したとおり、この例程度の要求であれば、
シンプルさという意味においてオールドスタイルのソースコードの方が優れているともいえる。
従って、オールドスタイルとstateパターンの選択は、
その要求の複雑さと安定度によって決定されるべきものである。

なお、C++でのstateパターンの実装には、下記に示すようなメンバ関数を使う方法もある。
多くのクラスを作る必要はないが、
各状態での状態管理変数を別の状態のものと分けて管理することができないため、
複雑な状態管理が必要な場合には使えないが、単純な状態管理で十分な場合には便利なパターンである。

```cpp
    // @@@ example/design_pattern/state_machine_new.h #1:0 begin
```

```cpp
    // @@@ example/design_pattern/state_machine_new.cpp #1:0 begin
```

```cpp
    // @@@ example/design_pattern/state_machine_ut.cpp #2:0 begin
```

[演習-State](~~~)  

### Observer
Observerは、クラスSubjectと複数のクラスObserverN(N = 0, 1, 2 ...)があり、
この関係が下記の条件を満たさなければならない場合に使用されるパターンである。

* ObserverNオブジェクトはSubjectオブジェクトが変更された際、その変更通知を受け取る。
* SubjectはObserverNへ依存してはならない。

GUIアプリケーションを[MVC](---)で実装する場合のModelがSubjectであり、
ViewがObserverNである。

まずは、このパターンを使用しない実装例を示す。

```cpp
    // @@@ example/design_pattern/observer_ng.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/observer_ng.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/subject_ng.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/subject_ng.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/observer_ut.cpp #0:0 begin -1
```

上記実装例のクラス図を下記する。
これを見ればわかるように、クラスSubjectNGとクラスObserverNG_Nは相互依存しており、機能追加、
修正が難しいだけではなく、この図の通りにパッケージを分割した場合
(パッケージがライブラリとなると前提）、リンクすら難しくなる。

![observer クラス図](plant_uml/observer_class_ng.png)

このようなクラス間の依存関係は下記のようにファイル間の依存関係に反映される。
このような相互依存は、差分ビルドの長時間化等の問題も引き起こす。

![ファイルの依存関係](plant_uml/observer_file_ng.png)

次に、上記にObserverパターンを適用した実装例
(Subjectを抽象クラスにすることもあるが、下記例ではSubjectを具象クラスにしている)を示す。

```cpp
    // @@@ example/design_pattern/observer_ok.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/observer_ok.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/subject_ok.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/subject_ok.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/observer_ut.cpp #0:1 begin -1
```

上記実装例のクラス図を下記する。
Observerパターンを使用しない例と比べると、
クラスSubjectOKとクラスObserverOK_Nとの相互依存が消えたことがわかる。

![observer クラス図](plant_uml/observer_class_ok.png)

最後に、上記のファイルの依存関係を示す。
ファイル(パッケージ)の依存関係においてもSubjectOKはObserverOKに依存していないことがわかる
(MVCに置き換えると、ModelはViewに依存していない状態であるといえる)。

![ファイルの依存関係](plant_uml/observer_file_ok.png)

[演習-Observer](~~~)  

### Visitor
このパターンは、クラス構造とそれに関連するアルゴリズムを分離するためのものである。

最初に
「クラス構造とそれに関連するアルゴリズムは分離できているが、
それ以前にオブジェクト指向の原則に反している」
例を示す。

```cpp
    // @@@ example/design_pattern/visitor.cpp #0:0 begin
```

下記クラス図からもわかる通り、ポリモーフィズムに反したこのような構造は複雑な依存関係を作り出す。
このアンチパターンにより同型の条件文が2度出てきてしまうため、
Printerのアルゴリズム関数が増えれば、この繰り返しはそれに比例して増える。
またFileEntityの派生が増えれば、それら条件文はすべて影響を受ける。
このようなソースコードは、このようにして等比級数的に複雑化する。

![クラス図](plant_uml/visitor_ng1.png)

これをポリモーフィズムの導入で解決した例を示す。

```cpp
    // @@@ example/design_pattern/visitor.cpp #1:0 begin
```

上記例では、PrinterのアルゴリズムをFileEntityの各派生クラスのメンバ関数で実装することで、
Printerの各関数は単純化された。

![クラス図](plant_uml/visitor_ng2.png)

これはポリモーフィズムによるリファクタリングの良い例と言えるが、
SRP(「[単一責任の原則(SRP)](---)」)に反するため、
Printerの関数が増えるたびにPrintPathname1、
PrintPathname2のようなFileEntityのインターフェースが増えてしまう。

このようなインターフェースの肥大化に対処するパターンがVisitorである。

上記例にVisitorを適用してリファクタリングした例を示す。

```cpp
    // @@@ example/design_pattern/visitor.h #0:0 begin
    // @@@ example/design_pattern/visitor.h #0:1 begin
    // @@@ example/design_pattern/visitor.h #0:2 begin
```

```cpp
    // @@@ example/design_pattern/visitor.cpp #2:0 begin
```

上記クラスの関係は下記のようになる。

![クラス図](plant_uml/visitor_ok.png)

このリファクタリングには、

* FileEntityのインターフェースを小さくできる
* FileEntityVisitorから派生できるアルゴリズムについては、
  FileEntityのインターフェースに影響を与えずに追加できる
  (「[オープン・クローズドの原則(OCP)](---)」参照)

という利点がある。
一方で、この程度の複雑さの(単純な)例では、Visitorの適用によって以前よりも構造が複雑になり、
改悪してしまった可能性があるため、デザインパターンを使用する場合には注意が必要である。

なお、上記の抜粋である下記コード

```cpp
    // @@@ example/design_pattern/visitor.h #0:1 begin -1 1
```

はコードクローンだが、thisの型が違うため、
各Acceptが呼び出すFileEntityVisitor::Visit()も異り、単純に統一することはできない。
これを改めるためには、「[CRTP(curiously recurring template pattern)](---)」が必要になる。

このパターンを用いて、前記したFileEntityの3つの派生クラスが持つコードクローンを解消した例を以下に示す。

```cpp
    // @@@ example/design_pattern/visitor_crtp.h #0:0 begin
```

---

次に示すソースコードはVisitorとは関係がないが、
FileEntityVisitorから派生するクラスを下記クラス図が示すように改善することで、
単体テストが容易になる例である(「[DI(dependency injection)](---)」参照)。

![クラス図](plant_uml/visitor_ut.png)

```cpp
    // @@@ example/design_pattern/visitor.h #1:0 begin
```

```cpp
    // @@@ example/design_pattern/visitor.cpp #3:0 begin
```

```cpp
    // @@@ example/design_pattern/visitor_ut.cpp #0:0 begin
```

[演習-Visitor](~~~)  

### Proxy
Proxyとは代理人という意味で、
本物のクラスに代わり代理クラス(Proxy)が処理を受け取る
(実際は、処理自体は本物クラスに委譲されることもある)パターンである。

以下の順番で例を示すことで、Proxyパターンの説明を行う。

1. 内部構造を外部公開しているサーバ クラス
2. そのサーバをラッピングして、使いやすくしたサーバ クラス(Facadeパターン)
3. サーバをラップしたクラスのProxyクラス

まずは、内部構造を外部公開しているの醜悪なサーバの実装例である。

```cpp
    // @@@ example/design_pattern/bare_server.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/bare_server.cpp #0:0 begin
```

下記は、上記BareServerを使用するクライアントの実装例である。通信がpipe()によって行われ、
その中身がPacket{}であること等、不要な依存関係をbare_client()に強いていることがわかる。
このような構造は、機能追加、保守作業を非効率、困難にするアンチパターンである。

```cpp
    // @@@ example/design_pattern/proxy_ut.cpp #0:0 begin
```

次に、このむき出しの構造をラッピングする例を示す(このようなラッピングをFacadeパターンと呼ぶ)。

```cpp
    // @@@ example/design_pattern/bare_server_wrapper.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/bare_server_wrapper.cpp #0:0 begin
```

下記は、上記BareServerWrapperのクライアントの実装例である。
BareServerWrapperがむき出しの通信をラップしたことで、bare_wrapper_client()は、
bare_client()に比べてシンプルになったことがわかる。

```cpp
    // @@@ example/design_pattern/proxy_ut.cpp #0:1 begin
```

次の例は、BareServerとBareServerWrapperを統合し、
さらに全体をシンプルにリファクタリングしたWrappedServerである。
Packet{}やpipe等の通信の詳細がwrapped_server.cppの無名名前空間に閉じ込められ、
クラスの隠蔽性が強化されたことで、より機能追加、保守が容易になった。

```cpp
    // @@@ example/design_pattern/wrapped_server.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/wrapped_server.cpp #0:0 begin
```

WrappedServerの使用例を下記する。当然ながらbare_wrapper_client()とほぼ同様になる。

```cpp
    // @@@ example/design_pattern/proxy_ut.cpp #0:2 begin
```

WrappedServerが提供する機能はスレッド間通信を含むため処理コストが高い。
その対策として、サーバから送られてきた文字列をキャッシュするクラス(Proxyパターン)の導入により、
そのコストを削減する例を下記する。

```cpp
    // @@@ example/design_pattern/wrapped_server_proxy.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/wrapped_server_proxy.cpp #0:0 begin
```

下記図のようにWrappedServerProxyはWrappedServerからのパブリック継承であるため、
WrappedServerのクライアントは、そのままWrappedServerProxyのクライアントとして利用できる。

![クラス図](plant_uml/proxy.png)

なお、正確には下記のようなクラス構造をProxyパターンと呼ぶことが多いが、
ここでは単純さを優先した。

![クラス図](plant_uml/proxy_general.png)

[演習-Proxy](~~~)  


## アーキテクチャパターン(システムレベルの構造)

### DI(dependency injection)
メンバ関数内でクラスDependedのオブジェクトを直接、生成する
(もしくは[Singleton](---)オブジェクトや静的オブジェクト(std::coutやstd::cin等)に直接アクセスする)
クラスNotDIがあるとする。
この場合、クラスNotDIはクラスDependedのインスタンスに依存してしまう。
このような依存関係はクラスNotDIの可用性とテスト容易性を下げる。
これは、「仮にクラスDependedがデータベースをラップするクラスだった場合、
クラスNotDIの単体テストにデータベースが必要になる」ことからも容易に理解できる。

```cpp
    // @@@ example/design_pattern/di_ut.cpp #0:0 begin
```

下記は上記NotDIにDIパターンを適用した例である。
この場合、クラスDIは、クラスDependedの型にのみ依存する。

```cpp
    // @@@ example/design_pattern/di_ut.cpp #0:1 begin
```

下記は、クラスNotDIとクラスDIがそれぞれのDoSomething()を呼び出すまでのシーケンス図である。

![シーケンス図](plant_uml/di.png)

このパターンの効果により、
DIオブジェクトにはDependedかその派生クラスのオブジェクトを渡すことができるようになった。
これによりクラスDIは拡張性に対して柔軟になっただけでなく、テスト容易性も向上した。

次に示すのは、このパターンを使用して問題のある単体テストを修正した例である。

まずは、問題があるクラスとその単体テストを下記する。

```cpp
    // in device_io.h
    // @@@ example/design_pattern/di_ut.cpp #1:0 begin 0 -1
```
```cpp
    // in widget.h
    
    #include "device_io.h"
    // @@@ example/design_pattern/di_ut.cpp #1:1 begin 0 -1
```
```cpp
    // in widget_ut.cpp
    // @@@ example/design_pattern/di_ut.cpp #1:2 begin -1 -1
```

当然であるが、この単体テストは、UNIT_TESTマクロを定義している場合のWidgetの評価であり、
UNIT_TESTを定義しない実際のコードの評価にはならない。

以下では、DIを用い、この問題を回避する。

```cpp
    // in device_io.h
    // @@@ example/design_pattern/di_ut.cpp #2:0 begin 0 -1
```
```cpp
    // in widget.h
    // @@@ example/design_pattern/di_ut.cpp #2:1 begin 0 -1
```
```cpp
    // in widget_ut.cpp
    // @@@ example/design_pattern/di_ut.cpp #2:2 begin 0 -1
```
```cpp
    // 上記DeviceIO_Mockと同様に、in widget_ut.cpp
    // @@@ example/design_pattern/di_ut.cpp #2:3 begin -1 -1
```

この例では、単体テストのためだけに仮想関数を導入しているため、多少やりすぎの感がある。
そのような場合、下記のようにテンプレートを用いればよい。

```cpp
    // in device_io.h
    // @@@ example/design_pattern/di_ut.cpp #3:0 begin 0 -1
```
```cpp
    // in widget.h
    // @@@ example/design_pattern/di_ut.cpp #3:1 begin 0 -1
```
```cpp
    // in widget_ut.cpp
    // @@@ example/design_pattern/di_ut.cpp #3:2 begin 0 -1
```
```cpp
    // 上記DeviceIO_Mockと同様に、in widget_ut.cpp
    // @@@ example/design_pattern/di_ut.cpp #3:3 begin -1 -1
```

以上からわかるように、
ここで紹介したDIは単体テストを容易にするクラス設計のためにも非常に有用なパターンである。

[演習-DI](~~~)  


### MVC

MVCはデザインパターンと言うよりもアーキテクチャパターンである。
一般にGUIアプリケーションのアーキテクチャに使用されるが、
外部からの非同期要求を処理するアプリケーションのアーキテクチャにも相性が良い。

MVCのそれぞれのアルファベットの意味は、下記テーブルの通りである。

|   | MVC            | 主な役割                        |
|:-:|:---------------|:--------------------------------|
| M | Model          | ビジネスロジックの処理          |
| V | View           | UIへの出力                      |
| C | Controller     | 入力をModelへ送信               |

下記はMVCの概念モデルである(矢印は制御の流れであって、依存関係ではない)。

![MVC](plant_uml/mvc.png)

制御の流れは、

1. ユーザの入力に応じてControllerのメソッドが呼び出される。
2. Controllerのメソッドは、ユーザの入力に応じた引数とともにModelのメソッドを呼び出す。
3. Modelは、それに対応するビジネスロジック等の処理を(通常、非同期に)行い、
自分自身の状態を変える(変わらないこともある)。
4. Modelの状態変化は、そのModelのオブザーバーとして登録されているViewに通知される。
5. Viewは関連するデータをModelから取得し、それを出力(UIに表示)する。

ViewはModelの[Observer](---)であるため、ModelはViewへ依存しない。
多々あるMVC派生パターンすべてで、そのような依存関係は存在しない
(具体的なパターンの選択はプロジェクトで使用するGUIフレームワークに強く依存する)。

そのようにする理由は下記の通りで、極めて重要な規則である。

* GUIのテストは目で見る必要がある(ことが多い)ため、Viewに自動単体テストを実施することは困難である。
  一方、ViewがModelに依存しないのであれば、Modelは自動単体テストをすることが可能である。
* 通常、Viewの仕様は不安定で、Modelの仕様は安定しているため、Modelのソースコード変更は
  Viewのそれよりもかなり少ない。
  しかし、ModelがViewに依存してしまうと、Viewに影響されModelのソースコード変更も多くなる。

[演習-デザインパターン選択1](~~~)  
[演習-デザインパターン選択2](~~~)  
[演習-デザインパターン選択3](~~~)  




