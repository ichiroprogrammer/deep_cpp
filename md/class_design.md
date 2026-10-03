# クラス設計
本章では、オブジェクト指向・プログラミング(OOD/OOP)において、重要な原則やガイドラインを解説する。  

___

__この章の構成__

<!-- index 1-4 -->

[インデックス](---)に戻る。  

___

## SOLID
SOLIDとは、オブジェクト指向・プログラミング(OOD/OOP)において特に重要な下記の5つの原則である。

* [単一責任の原則(SRP)](---)
* [オープン・クローズドの原則(OCP)](---)
* [リスコフの置換原則(LSP)](---)
* [インターフェース分離の原則(ISP)](---)
* [依存関係逆転の原則(DIP)](---)

[インデックス](---)に戻る。  

___

### 単一責任の原則(SRP)
単一責任の原則(SRP, Single Responsibility Principle)とは、

* 一つのクラスは、ただ一つの責任(機能)を持つようにしなければならない
* 一つのクラスは、ただ一つの理由で変更されるように作られなければならない

というクラスデザイン上の制約である。

下記クラスSentenceHolderNotSRPは、一見問題ないように見えるが、std::stringの保持と、
その出力という二つの責務を持つため、SRP違反である。

```cpp
    // @@@ example/class_design/srp_ut.cpp #0:0 begin
```

実践的にはこの程度の単純なクラスでのSRP違反が問題になることは少ないが、
下記のコメントで示す通り、単体テストの実施が困難になる。

```cpp
    // @@@ example/class_design/srp_ut.cpp #0:1 begin -1
```

クラスSentenceHolderNotSRPの二つの責務をクラスSentenceHolderSRPと、
Output()に分離したコード実装例を下記する。

```cpp
    // @@@ example/class_design/srp_ut.cpp #1:0 begin
```

下記のコードで示したように、この分離の効果で単体テストの実施が容易になった。

```cpp
    // @@@ example/class_design/srp_ut.cpp #1:1 begin -1
```

[演習-SRP](~~~)  

___

### オープン・クローズドの原則(OCP)
オープン・クローズドの原則(OCP, Open-Closed Principle)とは、

* クラスは拡張に対して開いて (open) いなければならず、
* クラスは修正に対して閉じて (closed) いなければならない

というクラスデザイン上の制約である。

まずは、アンチパターンから示す。

```cpp
    // @@@ example/class_design/ocp_ut.cpp #0:0 begin
```

Transaction Method(enum TransactionMethod)が増えた場合、
少なくとも3か所に手を入れなけばならなくなる(修正に対してclosedでない)。
従って、下記のTransactorNotOCP::Charge()や、TransactorNotOCP::Pay()は
Transaction Methodの追加、変更に対して、脆弱な構造だと言える。

次に上記ソースコードのクラス図を下記する。

クラス図が示す通り、
TransactorNotOCPは、TransactorGoogle, TransactorSuica, TransactorEdy
(Transaction Methodに対応した具体的なクラス)に強く依存する。
したがって、新たなTransactor Methodが追加されれば、
Transaction Methodを使用しているTransactorNotOCPのすべてのメンバ関数は影響を受ける。
この構造は、上位概念が下位概念に依存しているとも言えるため、
後述する「[依存関係逆転の原則(DIP)](---)」にも反している。

![クラス図例](plant_uml/ocp_ng.png)

下記は、TransactorIFを導入することによって、上例をOCPに沿うように改善したクラス図と実装である。
TransactorOCPは、TransactorIFの効果によりTransaction Methodの追加に対して全く影響を受けなくなった
(実際には、TransactorIFから派生する具象クラスの生成用Factory関数(「[Factory](---)」参照)
が必要になるため全く影響がないわけではないが、
そのような箇所はソースコード全体でただ一つにすることができるため、
Transaction Methodの追加に対して強固な構造になったと言える)。

![クラス図例](plant_uml/ocp_ok.png)

下記にこのクラス図に従ったコードを示す。

```cpp
    // @@@ example/class_design/ocp_ut.cpp #1:0 begin
```

ここでは、この原則に沿う実装方法としてポリモーフィズムを使うパターンを紹介したが、
[Pimpl](---)のようにラッピングを使用するパターンも有用である。

[演習-OCP](~~~)  

___

### リスコフの置換原則(LSP)
リスコフの置換原則(LSP, Liskov Substitution Principle)とは、

* 基底クラスを使っているX(関数もしくはクラス)に、
  基底クラスの代わりにその派生クラスを渡した場合でも、
  Xはその実際の型を知ること無しに正常動作できなければならない

というクラスデザイン上の制約であり、
この制約を守るために下記のような契約プログラミングを行うことが求められる。

* 事前条件を派生クラスで強めることはできない。
  つまり、基底クラスよりも強い事前条件を持つ派生クラスを作ってはならない。
* 事後条件を派生クラスで弱めることはできない。
  つまり、基底クラスよりも弱い事後条件を持つ派生クラスを作ってはならない。

この原則に従わない実装例を示すために、以下のようなクラスRectangleとその派生クラスSquareを定義する。

![クラス図例](plant_uml/rectangle_square.png)

```cpp
    // @@@ example/class_design/lsp.h #0:0 begin
```

Rectangleのリファレンスを受け取るSetX()とその単体テストを以下のようにすると、
Rectangleのテストでは問題は起こらないが、同じことをSquareに行うとアボートしてしまう
(下記例ではASSERT_DEATHを使用しアボートすることを確認している)。

```cpp
    // @@@ example/class_design/lsp_ut.cpp #0:0 begin
```

上記コードがアボート(assertion fail)してしまったのは  

* Rectangle::SetX()は、この実行によるy\_の値が不変であることを表明している
* この表明は、Rectangle::set_x()の事後条件となる
* Square::set_x()は、この事後条件を守らず、y\_の値を変えてしまった

が原因である。このデザイン上の問題には目をつぶり(Rectangle、Squareを修正せずに)、
しかもアボートしないSetX()の実装を考えてみよう。

SetX()は仮引数で渡されたオブジェクトの実際の型がわからなければアボートを避けることはできない。
従って、 新しいSetX()のコード実装例は以下のようになる。

```cpp
    // @@@ example/class_design/lsp_ut.cpp #1:0 begin
```

上記の新たなSetX()は、アボートはしないがきわめて醜悪且つ、
Rectangleの全派生クラスに依存した、変更に弱い関数となる。  

なお、リスコフの置換原則とは関係しないが、上記のdynamic_castを含むSetX()は、
下記のように修正することができる。

```cpp
    // @@@ example/class_design/lsp_ut.cpp #2:0 begin
```

このSetX()は、Rectangleからの派生クラスに依存していないため、良い解法に見える。
ところが実際にはオブジェクトの[オブジェクトのコピー|スライシング](---)という別の問題を引き起こす。  

例示した問題は結局のところデザインの誤りが原因であり、それを修正しない限り、
問題の回避は容易ではない。

一般に、継承関係は、[is-a](---)の関係と呼ばれる。数学の世界では「正方形 is a 長方形」であるため、
この関係を継承で表したのだが、
「Rectangle::SetX()の性質より導き出されたRectangle::set_x()の事後条件」
により、「クラスSquare is **NOT** a クラスRectangle」となり、
SquareとRectangleは継承関係ではないため問題が発生した。

継承を用いなければこのような問題は発生しないため、public継承を使用する際には、
「本当にその関係は継承で表すべきか(それが最もシンプルな方法か)？」
について熟慮する必要がある。

なお、エクセプション記述子は、関数のエクセプション仕様を強制的にLSPに従わせる仕組みであるが、
C++11から非推奨になり、C++17では規格から削除された。
その理由は、
「[非推奨だった古い例外仕様を削除](https://cpprefjp.github.io/lang/cpp17/remove_deprecated_exception_specifications.html)」
の説明の通り、これを使用し場合、OCPに違反する可能性が高いからである。
従って、原則に従うのみでなく、その他の原則とのバランスも考慮する必要がある。

[演習-LSP](~~~)  

___

### インターフェース分離の原則(ISP)
インターフェース分離の原則 (ISP, Interface Segregation Principle)とは、

* クラスは、そのクライアントが使用しないメソッドへの依存を、そのクライアントに強制するべきではない。
    * クラスのインターフェースを巨大にしない。
    * 一つのヘッダファイルに互いが密接な関係を持たない複数のクラスを定義、宣言すべきでない。
    * 一つのヘッダファイルにそのファイルのコンパイルに不要なヘッダファイルをインクルードすべきでない。

というクラスデザイン上の制約である。

まずは、ISPに従っていない例を示す。
下記のStreamReadWriterは、ClientRからはStreamReadWriter::Read()のみが、
ClientWからはStreamReadWriter::Write()のみが使用されている。  

![UML例](plant_uml/isp_ng.png)

ほとんどのStreamReadWriter使用ファイルでこのような依存関係がある場合、
このクラスは下記のようにStreamReaderとStreamWriterに分割した方が良い(依存関係が小さくなる)。

![UML例](plant_uml/isp_ok.png)

クラスの設計時に統合か分割かで悩むことは多いが、一度統合してしまえば分割は困難であり、
逆に分割されたものを統合することは容易である。このことを考慮すれば、
このような逡巡に解を与えることは簡単である。言うまでもないが、「まずは分割」が原則である。

[演習-ISP](~~~)  

___

### 依存関係逆転の原則(DIP)
依存関係逆転の原則 (DIP, Dependency Inversion Principle)とは、

* 上位レベルのモジュールは下位レベルのモジュールに依存すべきではない。
* 抽象は具象に依存すべきではない。

というクラス デザイン上の制約である。

下記ServerNG::Serverは、ClientNG::Clientに非同期サービスを提供する
(従って、ServerNG::ServerはClientNG::Clientに対して上位概念である)。

![UML例](plant_uml/dip_ng_seq.png)

非同期サービスであるServerNG::Server::RequireStringAsync()の完了は
ServerNG::ServerがClientNG::Client::Done()を呼び出すことにより通知される。  
その実装、使用例を下記に示す。

```cpp
    // @@@ example/class_design/dip_server_ng.h #0:0 begin
```

```cpp
    // @@@ example/class_design/dip_server_ng.cpp #0:0 begin
```

```cpp
    // @@@ example/class_design/dip_client_ng.h #0:0 begin
```

```cpp
    // @@@ example/class_design/dip_client_ng.cpp #0:0 begin
```

```cpp
    // @@@ example/class_design/dip_ut.cpp #0:0 begin
```

上記ソースコードから明らかなようにServerNG::ServerとClientNG::Clientは相互に依存している。
このうちの一つはサーバがクライアントに依存(上位概念が下位概念に依存)する問題のある構造となっている。

![UML例](plant_uml/dip_ng.png)

このため、クライアントのバリエーションが増えた場合、容易にServerNG::Serverのコードは肥大化する。
また、ServerNG::Serverを介して各クライアント間にも(暗黙、明示両方の)依存関係が生まれやすいため、
ServerNG::Serverのコード修正は非常に困難になることが予想される。

次にDIPに従い上記コードを改善した例を示す。

```cpp
    // @@@ example/class_design/dip_server_ok.h #0:0 begin
```

```cpp
    // @@@ example/class_design/dip_server_ok.cpp #0:0 begin
```

```cpp
    // @@@ example/class_design/dip_client_ok.h #0:0 begin
```

```cpp
    // @@@ example/class_design/dip_client_ok.cpp #0:0 begin
```

```cpp
    // @@@ example/class_design/dip_ut.cpp #0:1 begin
```

修正後のコードは、

* ServerOK::ServerはServerOK::ClientIFに依存する。
* ClientOK::ClientはServerOK::ClientIFから派生する。

このクラス図を以下に示す。

![UML例](plant_uml/dip_ok.png)

ServerNGとClientNGの双方向依存関係は、ClientOKからServerOKへの単方向依存関係へと改善され、
サーバに影響を与えることなく、クライアントの機能変更やバリエーション追加を行うことが可能となった。

[演習-DIP](~~~)  

### まとめ
以上で述べたように、SOLIDはオブジェクト指向・プログラミング(OOD/OOP)において極めて重要な原則である。
この逸脱はソースコードを劣化させ、ソフトウェアの品質低下や開発費増大に直結するため、
厳守することが求められる。

[演習-SOLIDの定義](~~~)  

___

## クラス凝集性のクライテリア
クラス構造の適・不適については、[PercentLackOfCohesion](---)によって下記テーブルのように定義することができる。

| PercentLackOfCohesion                 |  クラスの状態              |
|:-------------------------------------:|:--------------------------:|
|       `PercentLackOfCohesion  <= 40`  | 理想的な状態(適)           |
| `40 <  PercentLackOfCohesion  <  60`  | 要注意状態(場合により不適) |
| `60 <= PercentLackOfCohesion `        | 改善必須状態(不適)         |

* `PercentLackOfCohesion  <= 40`  
  クラスが非常に凝集しており、[単一責任の原則(SRP)](---)を強く遵守している状態であるため、
  通常、デザインの見直しは不要である。

* `40 < PercentLackOfCohesion  < 60`  
  クラスの凝集性がやや弱くなり始めている。
  デザイン見直しの必要な時期が迫りつつあると考えるべきだろう。
  このタイミングであればリファクタリングは低コストで完了できるだろう。

* `60 <= PercentLackOfCohesion `  
  クラス内のメンバ関数間の関連性が低く、凝集性が不十分である。
  メンバ関数が異なる責務にまたがっている可能性が高いため、
  一刻も早くデザインの見直しを行うべきだろう。

[注]  
このクライテリアはアカデミックな出典があるわけではなく、著者のプロマネとして経験則に基づくものである。

---

## copy/moveのガイドライン
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
    // @@@ example/class_design/semantics_ut.cpp #0:0 begin -2
```

しかし、下記のコード内のa、bは同じ値を持つが、
アドレスが異なるため同一のオブジェクトではないにもかかわらず、組み込みの==の値はtrueとなる。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #0:1 begin -2
```

このような場合、aとbは等価であるという。同一ならば等価であるが、等価であっても同一とは限らない。

ポインタや配列をオペランドとする場合を除き、C++における組み込みの==は、
数学の等号とは違い、等価を表していると考えられるが、
上記した3つの律を守っている。従ってオーバーロードoperator==も同じ性質を守る必要がある。

組み込みの==やオーバーロード[==演算子](---)のこのような性質をここでは「等価性のセマンティクス」と呼ぶ。

クラスAを下記のように定義し、

```cpp
    // @@@ example/class_design/semantics_ut.cpp #1:0 begin
```

そのoperator==を下記のように定義した場合、

```cpp
    // @@@ example/class_design/semantics_ut.cpp #1:1 begin
```

単体テストは下記のように書けるだろう。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #1:2 begin -2
```

これは、一応パスするが(処理系定義の動作を前提とするため、必ず動作する保証はない)、
下記のようにすると、パスしなくなる。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #1:3 begin -2
```

一般にポインタの等価性は、その値の同一性ではなく、
そのポインタが指すオブジェクトの等価性で判断されるべきであるが、
先に示したoperator==はその考慮をしていないため、このような結果になった。

次に、これを修正した例を示す。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #1:4 begin
```

ポインタをメンバに持つクラスのoperator==については、上記したような処理が必要となる。

次に示す例は、基底クラスBaseとそのoperator==である。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #2:0 begin
```

次の単体テストが示す通り、これ自体には問題がないように見える。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #2:1 begin -1
```

しかし、Baseから派生したクラスDerivedを

```cpp
    // @@@ example/class_design/semantics_ut.cpp #3:0 begin
```

のように定義すると、下記の単体テストで示す通り、等価性のセマンティクスが破壊される。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #3:1 begin -1
```

Derived用のoperator==を

```cpp
    // @@@ example/class_design/semantics_ut.cpp #4:0 begin
```

と定義しても、下記に示す通り部分的な効果しかない。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #4:1 begin -1
```

この問題は、「[型とインスタンス|RTTI](---)」使った下記のようなコードで対処できる。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #5:0 begin
```

下記に示す通り、このコードは、
[オープン・クローズドの原則(OCP)](---)にも対応した柔軟な構造を実現している。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #5:1 begin
```

前例では「両辺の型が等しいこと」が「等価であること」の必要条件となるが、
この要件が、すべてのoperator==に求められるわけではない。

次に示すのは、一見すると両辺の型が違うにもかかわらず、
等価性のセマンティクスを満たしている例である。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #6:0 begin -1
```

これは、文字列リテラルを第1引数に取るstd::stringのコンストラクタが非explicitであることによって、
文字列リテラルからstd::stringへの[暗黙の型変換](---)が起こるために成立する。

以上で見てきたように、等価性のセマンティクスを守ったoperator==の実装には多くの観点が必要になる。

---

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
    // @@@ example/class_design/semantics_ut.cpp #7:0 begin -2
```

一方で、std::auto_ptrはcopyセマンティクスを満たしていない。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #7:1 begin -2
```

この仕様は極めて不自然であり、std::auto_ptrはC++11で非推奨となり、C++17で規格から排除された。

下記の単体テストから明らかな通り、
「[等価性のセマンティクス](---)」で示した最後の例も、copyセマンティクスを満たしていない。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #7:2 begin -2
```

原因は、copy代入で[オブジェクトのコピー|スライシング](---)が起こるためである。

---

### moveセマンティクス
moveセマンティクスとは以下を満たすようなセマンティクスである(operator==が定義されていると前提)。

* パフォーマンス要件  
    move代入の実行コスト <= copy代入の実行コスト(通常はmove代入の方が高速)

* 意味的要件  
    a == b が true の時に、c = std::move(a) を実行すると、  
    * b == c が true になる（値が保存される）
    * a == c は true にならなくても良い（aはmove後に不定状態になり得る）

* リソース管理   
    必須ではないが、aがポインタ等のリソースを保有している場合、
     move代入後にはそのリソースはcに移動していることが一般的である(「[expression|rvalue](---)」参照)

* エクセプション安全性  
    [no-fail保証](---)をする(noexceptと宣言し、エクセプションをthrowしない)

moveセマンティクスは、使用後に破棄されるオブジェクト(主にrvalue)からの代入処理の実行コストを削減するために導入された。

下記のクラスのmove代入の内部処理はcopy代入が行われており、
moveセマンティクスの目的である「パフォーマンスの向上」が達成されない。

そのため、このようなmove代入は避けるべきである。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #8:0 begin
```

下記のコードのようにメンバの代入もできる限りmove代入を使うことで、
パフォーマンスの良い代入ができる。

```cpp
    // @@@ example/class_design/semantics_ut.cpp #8:1 begin
```

---

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
    // @@@ example/class_design/deep_shallow_copy_ut.cpp #0:0 begin
```

コンストラクタでポインタのようなリソースを確保し、
デストラクタでそれらを解放するようなクラスの場合、シャローコピーは良く知られた問題を起こす。

下記のShallowNGはその例である。

```cpp
    // @@@ example/class_design/deep_shallow_copy_ut.cpp #1:0 begin
```

シャローコピーにより、メンバで保持していたポインタ(ポインタが指しているオブジェクトではない)
がコピーされてしまうため、下記のコード内のコメントで示した通り、
メモリリークや2重解放を起こしてしまう。

```cpp
    // @@@ example/class_design/deep_shallow_copy_ut.cpp #1:1 begin -1
```

---

#### ディープコピー
ディープコピーとは、[シャローコピー](---)が発生させる問題を回避したコピーである。

以下に例を示す。

```cpp
    // @@@ example/class_design/deep_shallow_copy_ut.cpp #2:0 begin
```

上記クラスのDeepは、copyコンストラクタ、copy代入演算子でポインタをコピーするのではなく、
ポインタが指しているオブジェクトを複製することにより、シャローコピーの問題を防ぐ。


---

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
    // @@@ example/class_design/slice_ut.cpp #0:0 begin
```

```cpp
    // @@@ example/class_design/slice_ut.cpp #0:1 begin -1
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
    // @@@ example/class_design/slice_ut.cpp #1:0 begin -1
```

![スライシング配列](plant_uml/slicing_array.png)

---



## Rule of N
### 0の原則(Rule of Zero)
「0の原則」は、リソース管理を直接クラスで行わず、
リソース管理を専門とするクラス
(例: 標準ライブラリの[RAII(scoped guard)](---)クラス)に任せる設計ガイドラインを指す。
この法則に従うと、自身で特殊メンバ関数を定義する必要がなくなる。

```cpp
    // @@@ example/class_design/rule_of_zero_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/class_design/rule_of_zero_ut.cpp #0:1 begin -1
```

クラスがリソースを直接管理する場合、メモリリークや二重解放などのリスクを伴う。
上記のように信頼性の高いクラスに特殊メンバ関数の処理を任せることにより、
クラス自体にリソース管理の責任を持たせる必要がなくなる。

---

### 5の原則(Rule of Five)
「5の原則」は、
クラスがリソース(例: 動的メモリやファイルハンドルなど)を管理する場合、
デフォルトコンストラクタを除く[オブジェクト生成と初期化|特殊メンバ関数](---)、
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

なお、「5の原則」は、「六の原則」と呼ばれることもある。
その場合、この原則が対象とする関数は、
[オブジェクト生成と初期化|特殊メンバ関数](---)のすべてとなる。

このガイドラインに従って、コピーやムーブを実装する場合、

* [等価性のセマンティクス](---)
* [copyセマンティクス](---)
* [moveセマンティクス](---)

に従わなけならない。


