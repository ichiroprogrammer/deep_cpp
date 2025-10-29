# C++慣用語句
この章では、C++慣用言句ついて解説を行う。

___

__この章の構成__

<!-- index 1-4 -->

[このドキュメントの構成](---)に戻る。  

___

## オブジェクト指向
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
    // @@@ example/cpp_idioms/class_relation_ut.cpp #0:0 begin
```

bird::flyのオーバーライド関数(penguin::fly)について、[リスコフの置換原則(LSP)](---)に反した例を下記する。

```cpp
    // @@@ example/cpp_idioms/class_relation_ut.cpp #0:1 begin
    // @@@ example/cpp_idioms/class_relation_ut.cpp #0:2 begin -1
```

birdからpenguinへの派生がリスコフ置換の原則に反してしまった原因は以下のように考えることができる。

* bird::flyの事前条件penguin::flyが強めた
* bird::flyの事後条件をpenguin::flyが弱めた

penguinとbirdの関係はis-aの関係ではあるが、
上記コードの問題によって不適切なis-aの関係と言わざるを得ない。

上記の例では鳥全般と鳥の種類のis-a関係をpublic継承を使用して表した(一部不適切であるもの)。
さらにis-aの誤った適用例を示す。
自身が飼っている九官鳥に"キューちゃん"と名付けることはよくあることである。
キューちゃんという名前の九官鳥は一種の九官鳥であることは間違いのないことであるが、
このis-aの関係を表すためにpublic継承を使用するのは、is-aの関係の誤用になることが多い。
実際のコード例を以下に示す。この場合、型とインスタンスの概念の混乱が原因だと思われる。

```cpp
    // @@@ example/cpp_idioms/class_relation_ut.cpp #1:0 begin
```

この誤用を改めた例を以下に示す。

```cpp
    // @@@ example/cpp_idioms/class_relation_ut.cpp #2:0 begin
    // @@@ example/cpp_idioms/class_relation_ut.cpp #2:1 begin -1
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
    // @@@ example/cpp_idioms/class_relation_ut.cpp #3:0 begin
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
* CC(クライアントのクライアント): Cのメンバを使用するクラス

コード量の観点から考えた場合、手段1が最も優れていることが多い。
依存関係の複雑さから考えた場合、CはSに強く依存する。
場合によっては、この依存はCCからSへの依存間にも影響をあたえる。
従って、手段3が依存関係を単純にしやすい。
手段1は[is-a](---)に見え、以下に示すような問題も考慮する必要があるため、
可読性、保守性を劣化させる可能性がある。

```cpp
    // @@@ example/cpp_idioms/class_relation_ut.cpp #6:0 begin
    // @@@ example/cpp_idioms/class_relation_ut.cpp #6:1 begin -1
```

以上述べたように問題の多い手段1であるが、実践的には有用なパターンであり、
[CRTP(curiously recurring template pattern)](https://ja.wikibooks.org/wiki/More_C%2B%2B_Idioms/%E5%A5%87%E5%A6%99%E3%81%AB%E5%86%8D%E5%B8%B0%E3%81%97%E3%81%9F%E3%83%86%E3%83%B3%E3%83%97%E3%83%AC%E3%83%BC%E3%83%88%E3%83%91%E3%82%BF%E3%83%BC%E3%83%B3(Curiously_Recurring_Template_Pattern))
の実現手段でもあるため、一概にコーディング規約などで排除することもできない。


#### public継承によるis-implemented-in-terms-of
public継承によるis-implemented-in-terms-ofの実装例を以下に示す。

```cpp
    // @@@ example/cpp_idioms/class_relation_ut.cpp #7:0 begin
    // @@@ example/cpp_idioms/class_relation_ut.cpp #7:1 begin -1
```

すでに述べたようにこの方法は、
[private継承によるis-implemented-in-terms-of](---)や、
[コンポジションによる(has-a)is-implemented-in-terms-of](---)
と比べコードがシンプルになる。 

#### private継承によるis-implemented-in-terms-of
private継承によるis-implemented-in-terms-ofの実装例を以下に示す。

```cpp
    // @@@ example/cpp_idioms/class_relation_ut.cpp #4:0 begin
    // @@@ example/cpp_idioms/class_relation_ut.cpp #4:1 begin -1
```

この方法は、[public継承によるis-implemented-in-terms-of](---)が持つデストラクタ問題は発生せす、
[is-a](---)と誤解してしまう問題も発生しない。


#### コンポジションによる(has-a)is-implemented-in-terms-of
コンポジションによる(has-a)is-implemented-in-terms-ofの実装例を示す。

```cpp
    // @@@ example/cpp_idioms/class_relation_ut.cpp #5:0 begin
    // @@@ example/cpp_idioms/class_relation_ut.cpp #5:1 begin -1
```

この方は実装を利用するクラストの依存関係を他の2つに比べるとシンプルにできるが、
逆に実装例から昭なとおり、コード量が増えてしまう。

## オブジェクトの所有権
### オブジェクトの排他所有
オブジェクトの排他所有や、それを容易に実現するための
[std::unique_ptr](https://cpprefjp.github.io/reference/memory/unique_ptr.html)
の仕様を説明するために、下記のようにクラスA、Xを定義する。

```cpp
    // @@@ example/cpp_idioms/unique_ptr_ownership_ut.cpp #0:0 begin
```

下記に示した上記クラスの単体テストにより、
オブジェクトの所有権やその移動、
std::unique_ptr、std::move()、[expression|rvalue](---)の関係を解説する。

```cpp
    // @@@ example/cpp_idioms/unique_ptr_ownership_ut.cpp #0:1 begin -1
```

```cpp
    // @@@ example/cpp_idioms/unique_ptr_ownership_ut.cpp #0:2 begin -1
```

![所有権1](plant_uml/unique_ownership_1.png)

```cpp
    // @@@ example/cpp_idioms/unique_ptr_ownership_ut.cpp #0:3 begin -1
```

![所有権2](plant_uml/unique_ownership_2.png)

```cpp
    // @@@ example/cpp_idioms/unique_ptr_ownership_ut.cpp #0:4 begin -1
```
![所有権3](plant_uml/unique_ownership_3.png)

```cpp
    // @@@ example/cpp_idioms/unique_ptr_ownership_ut.cpp #0:5 begin -1
```

![所有権4](plant_uml/unique_ownership_4.png)

```cpp
    // @@@ example/cpp_idioms/unique_ptr_ownership_ut.cpp #0:6 begin -1
```

![所有権5](plant_uml/unique_ownership_5.png)

```cpp
    // @@@ example/cpp_idioms/unique_ptr_ownership_ut.cpp #0:7 begin -1
```

![所有権6](plant_uml/unique_ownership_6.png)


また、以下に見るようにstd::unique_ptrはcopy生成やcopy代入を許可しない。

```cpp
    // @@@ example/cpp_idioms/unique_ptr_ownership_ut.cpp #1:0 begin -1
```

以上で示したstd::unique_ptrの仕様の要点をまとめると、以下のようになる。

* std::unique_ptrはダイナミックに生成されたオブジェクトを保持する。
* ダイナミックに生成されたオブジェクトを保持するstd::unique_ptrがスコープアウトすると、
  保持中のオブジェクトは自動的にdeleteされる。
* 保持中のオブジェクトを他のstd::unique_ptrにmoveすることはできるが、
  copyすることはできない。このため、下記に示すような不正な方法以外で、
  複数のstd::unique_ptrが1つのオブジェクトを共有することはできない。

```cpp
    // @@@ example/cpp_idioms/unique_ptr_ownership_ut.cpp #2:0 begin -1
```

こういった機能によりstd::unique_ptrはオブジェクトの排他所有を実現している。

### オブジェクトの共有所有
オブジェクトの共有所有や、それを容易に実現するための
[std::shared_ptr](https://cpprefjp.github.io/reference/memory/shared_ptr.html)
の仕様を説明するために、下記のようにクラスA、Xを定義する。

```cpp
    // @@@ example/cpp_idioms/shared_ptr_ownership_ut.cpp #0:0 begin
```

下記に示した上記クラスの単体テストにより、
オブジェクトの所有権やその移動、共有、
std::shared_ptr、std::move()、[expression|rvalue](---)の関係を解説する。

```cpp
    // @@@ example/cpp_idioms/shared_ptr_ownership_ut.cpp #0:1 begin -1
```

```cpp
    // @@@ example/cpp_idioms/shared_ptr_ownership_ut.cpp #0:2 begin -1
```

![所有権1](plant_uml/shared_ownership_1.png)


```cpp
    // @@@ example/cpp_idioms/shared_ptr_ownership_ut.cpp #0:3 begin -1
```

![所有権2](plant_uml/shared_ownership_2.png)

```cpp
    // @@@ example/cpp_idioms/shared_ptr_ownership_ut.cpp #0:4 begin -1
```

![所有権3](plant_uml/shared_ownership_3.png)

```cpp
    // @@@ example/cpp_idioms/shared_ptr_ownership_ut.cpp #0:5 begin -1
```

![所有権4](plant_uml/shared_ownership_4.png)

```cpp
    // @@@ example/cpp_idioms/shared_ptr_ownership_ut.cpp #0:6 begin -1
```

![所有権5](plant_uml/shared_ownership_5.png)


```cpp
    // @@@ example/cpp_idioms/shared_ptr_ownership_ut.cpp #0:7 begin -1
```

![所有権6](plant_uml/shared_ownership_6.png)


```cpp
    // @@@ example/cpp_idioms/shared_ptr_ownership_ut.cpp #0:8 begin -1
```

![所有権7](plant_uml/shared_ownership_7.png)

以上で示したstd::shared_ptrの仕様の要点をまとめると、以下のようになる。

* std::shared_ptrはダイナミックに生成されたオブジェクトを保持する。
* ダイナミックに生成されたオブジェクトを保持するstd::shared_ptrがスコープアウトすると、
  共有所有カウントはデクリメントされ、その値が0ならば保持しているオブジェクトはdeleteされる。
* std::shared_ptrを他のstd::shared_ptrに、
    * moveすることことで、保持中のオブジェクトの所有権を移動できる。
    * copyすることことで、保持中のオブジェクトの所有権を共有できる。
* 下記のようなコードはstd::shared_ptrの仕様が想定する[セマンティクス](---)に沿っておらず、
  [未定義動作](---)に繋がる。

```cpp
    // @@@ example/cpp_idioms/shared_ptr_ownership_ut.cpp #1:0 begin -1
```

こういった機能によりstd::shared_ptrはオブジェクトの共有所有を実現している。


### オブジェクトの循環所有
[std::shared_ptr](https://cpprefjp.github.io/reference/memory/shared_ptr.html)の使い方を誤ると、
以下のコード例が示すようにメモリーリークが発生する。

なお、この節の題名である「オブジェクトの循環所有」という用語は、
この前後の節がダイナミックに確保されたオブジェクトの所有の概念についての解説しているため、
この用語を選択したが、文脈によっては、「オブジェクトの循環参照」といった方がふさわしい場合もある。

---

まずは、**メモリリークが発生しない**`std::shared_ptr`の正しい使用例を示す。

```cpp
    // @@@ example/cpp_idioms/shared_ptr_cycle_ut.cpp #0:0 begin
```

上記のクラスの使用例を示す。下記をステップ1とする。

```cpp
    // @@@ example/cpp_idioms/shared_ptr_cycle_ut.cpp #1:0 begin -1
```

![shread_ptrメモリーリーク](plant_uml/shared_each_1.png)


上記の続きを以下に示し、ステップ2とする。

```cpp
    // @@@ example/cpp_idioms/shared_ptr_cycle_ut.cpp #1:1 begin -1
```

![shread_ptrメモリーリーク](plant_uml/shared_each_2.png)


上記の続きを以下に示し、ステップ3とする。

```cpp
    // @@@ example/cpp_idioms/shared_ptr_cycle_ut.cpp #1:2 begin -1
```

![shread_ptrメモリーリーク](plant_uml/shared_each_3.png)


上記の続きを以下に示し、ステップ4とする。

```cpp
    // @@@ example/cpp_idioms/shared_ptr_cycle_ut.cpp #1:3 begin -1
```

![shread_ptrメモリーリーク](plant_uml/shared_each_4.png)


このような動作により、`std::make_shared<>`で生成されたX、Yオブジェクトは解放される。

---

次は**メモリリークが発生する**`std::shared_ptr`の誤用を示す。まずはクラスの定義から。

```cpp
    // @@@ example/cpp_idioms/shared_ptr_cycle_ut.cpp #2:0 begin
```

上記のクラスの動作を以下に示したコードで示す。

```cpp
    // @@@ example/cpp_idioms/shared_ptr_cycle_ut.cpp #2:1 begin -1
```

x0のライフタイムに差を作るために新しいスコープを導入し、そのスコープ内で、y0を生成し、
`X::Register`、`Y::Register`を用いて、循環を作ってしまう例(メモリーリークを起こすバグ)を示す。

```cpp
    // @@@ example/cpp_idioms/shared_ptr_cycle_ut.cpp #2:2 begin -1
```

![shread_ptrメモリーリーク](plant_uml/shared_cyclic.png)

下記のコードでは、y0がスコープアウトするが、そのタイミングでは、x0はまだ健在であるため、
Yオブジェクトの参照カウントは1になる(x0::y_が存在するため0にならない)。

```cpp
    // @@@ example/cpp_idioms/shared_ptr_cycle_ut.cpp #2:3 begin -1
```

![shread_ptrメモリーリーク](plant_uml/shared_cyclic_2.png)

ここでの状態をまとめると、

- y0がもともと持っていたXオブジェクトは健在(このオブジェクトはx0が持っているものでもあるため、use_countは2のまま)
- x0が宣言されたスコープが残っているため、当然ながらx0は健在
- x0はYオブジェクトを持ったままではあるが、y0がスコープアウトしたため、Yオブジェクトのuse_countは1に減った

  
次のコードでは、x0がスコープアウトし、y0がもともと持っていたXオブジェクトは健在であるため、
Xオブジェクトの参照カウントも1になる。このため、x0、y0がスコープアウトした状態でも、
X、Yオブジェクトの参照カウントは0にならず、従ってこれらのオブジェクトは解放されない
(shared_ptrは参照カウントが1->0に変化するタイミングで保持するオブジェクトを解放する)。

```cpp
    // @@@ example/cpp_idioms/shared_ptr_cycle_ut.cpp #2:4 begin -1
```

![shread_ptrメモリーリーク](plant_uml/shared_cyclic_3.png)

X、Yオブジェクトへの[ハンドル](---)を完全に失った状態であり、X、Yオブジェクトを解放する手段はない。

## copy/moveと等価性のセマンティクス
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
    // @@@ example/cpp_idioms/semantics_ut.cpp #0:0 begin -2
```

しかし、下記のコード内のa、bは同じ値を持つが、
アドレスが異なるため同一のオブジェクトではないにもかかわらず、組み込みの==の値はtrueとなる。

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #0:1 begin -2
```

このような場合、aとbは等価であるという。同一ならば等価であるが、等価であっても同一とは限らない。

ポインタや配列をオペランドとする場合を除き、C++における組み込みの==は、
数学の等号とは違い、等価を表していると考えられるが、
上記した3つの律を守っている。従ってオーバーロードoperator==も同じ性質を守る必要がある。

組み込みの==やオーバーロード[==演算子](---)のこのような性質をここでは「等価性のセマンティクス」と呼ぶ。

クラスAを下記のように定義し、

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #1:0 begin
```

そのoperator==を下記のように定義した場合、

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #1:1 begin
```

単体テストは下記のように書けるだろう。

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #1:2 begin -2
```

これは、一応パスするが(処理系定義の動作を前提とするため、必ず動作する保証はない)、
下記のようにすると、パスしなくなる。

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #1:3 begin -2
```

一般にポインタの等価性は、その値の同一性ではなく、
そのポインタが指すオブジェクトの等価性で判断されるべきであるが、
先に示したoperator==はその考慮をしていないため、このような結果になった。

次に、これを修正した例を示す。

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #1:4 begin
```

ポインタをメンバに持つクラスのoperator==については、上記したような処理が必要となる。

次に示す例は、基底クラスBaseとそのoperator==である。

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #2:0 begin
```

次の単体テストが示す通り、これ自体には問題がないように見える。

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #2:1 begin -1
```

しかし、Baseから派生したクラスDerivedを

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #3:0 begin
```

のように定義すると、下記の単体テストで示す通り、等価性のセマンティクスが破壊される。

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #3:1 begin -1
```

Derived用のoperator==を

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #4:0 begin
```

と定義しても、下記に示す通り部分的な効果しかない。

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #4:1 begin -1
```

この問題は、「[型とインスタンス|RTTI](---)」使った下記のようなコードで対処できる。

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #5:0 begin
```

下記に示す通り、このコードは、
[オープン・クローズドの原則(OCP)](---)にも対応した柔軟な構造を実現している。

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #5:1 begin
```

前例では「両辺の型が等しいこと」が「等価であること」の必要条件となるが、
この要件が、すべてのoperator==に求められるわけではない。

次に示すのは、一見すると両辺の型が違うにもかかわらず、
等価性のセマンティクスを満たしている例である。

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #6:0 begin -1
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
    // @@@ example/cpp_idioms/semantics_ut.cpp #7:0 begin -2
```

一方で、std::auto_ptrはcopyセマンティクスを満たしていない。

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #7:1 begin -2
```

この仕様は極めて不自然であり、std::auto_ptrはC++11で非推奨となり、C++17で規格から排除された。

下記の単体テストから明らかな通り、
「[等価性のセマンティクス](---)」で示した最後の例も、copyセマンティクスを満たしていない。

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #7:2 begin -2
```

原因は、copy代入で[オブジェクトのコピー|スライシング](---)が起こるためである。


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
    // @@@ example/cpp_idioms/semantics_ut.cpp #8:0 begin
```

下記のコードのようにメンバの代入もできる限りmove代入を使うことで、
パフォーマンスの良い代入ができる。

```cpp
    // @@@ example/cpp_idioms/semantics_ut.cpp #8:1 begin
```

### MoveAssignable要件
MoveAssignable要件は、C++において型がmove代入をサポートするために満たすべき条件を指す。
move代入はリソースを効率的に転送する操作であり、以下の条件を満たす必要がある。

1. リソースの移動  
   move代入では、リソース(動的メモリ等)が代入元から代入先へ効率的に転送される。

2. 有効だが未定義の状態  
   move代入後、代入元のオブジェクトは有効ではあるが未定義の状態となる。
   未定義の状態とは、破棄や再代入が可能である状態を指し、それ以外の操作は保証されない。

3. 自己代入の安全性  
   同一のオブジェクトをmove代入する場合でも、未定義動作やリソースリークを引き起こしてはならない。

4. 効率性  
   move代入は通常、copy代入よりも効率的であることが求められる。
   これは、リソースの複製を避けることで達成される(「[moveセマンティクス](---)」参照)。

5. デフォルト実装  
   move代入演算子が明示的に定義されていない場合でも、
   クラスが一定の条件(例: move不可能なメンバが存在しないこと)を満たしていれば、
   コンパイラがデフォルトの実装(「[オブジェクト生成と初期化|特殊メンバ関数](---)」参照)を生成する。

### CopyAssignable要件
CopyAssignable要件は、C++において型がcopy代入をサポートするために満たすべき条件を指す。

1. 動作が定義されていること  
   代入操作は未定義動作を引き起こしてはならない。自己代入（同じオブジェクトを代入する場合）においても正しく動作し、
   リソースリークを引き起こさないことが求められる。

2. 値の保持  
   代入後、代入先のオブジェクトの値は代入元のオブジェクトの値と一致していなければならない。

3. 正しいセマンティクス  
   copy代入によって代入元のオブジェクトが変更されてはならない(「[copyセマンティクス](---)」参照)。
   代入先のオブジェクトが保持していたリソース(例: メモリ)は適切に解放される必要がある。

4. デフォルト実装  
   copy代入演算子が明示的に定義されていない場合でも、
   クラスが一定の条件(例: copy不可能なメンバが存在しないこと)を満たしていれば、
   コンパイラがデフォルトの実装(「[オブジェクト生成と初期化|特殊メンバ関数](---)」参照)を生成する。

## classガイドライン
### ゼロの原則(Rule of Zero)
「ゼロの原則」は、リソース管理を直接クラスで行わず、
リソース管理を専門とするクラス
(例: 標準ライブラリの[RAII(scoped guard)](---)クラス)に任せる設計ガイドラインを指す。
この法則に従うと、自身で特殊メンバ関数を定義する必要がななくなる。

```cpp
    // @@@ example/cpp_idioms/rule_of_zero_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/cpp_idioms/rule_of_zero_ut.cpp #0:1 begin -1
```

クラスがリソースを直接管理する場合、メモリリークや二重解放などのリスクを伴う。
上記のように信頼性の高いクラスに特殊メンバ関数の処理を任せることにより、
クラス自体にリソース管理の責任を持たせる必要がなくなる。

### 五の原則(Rule of Five)
「五の原則」は、
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

なお、「五の原則」は、「六の原則」と呼ばれることもある。
その場合、この原則が対象とする関数は、
[オブジェクト生成と初期化|特殊メンバ関数](---)のすべてとなる。

このガイドラインに従って、コピーやムーブを実装する場合、

* [等価性のセマンティクス](---)
* [copyセマンティクス](---)
* [moveセマンティクス](---)

に従わなけならない。


## コーディングスタイル
### AAAスタイル
このドキュメントでのAAAとは、単体テストのパターンarrange-act-assertではなく、
almost always autoを指し、
AAAスタイルとは、「可能な場合、型を左辺に明示して変数を宣言する代わりに、autoを使用する」
というコーディングスタイルである。
この用語は、Andrei Alexandrescuによって造られ、Herb Sutterによって広く推奨されている。

特定の型を明示して使用する必要がない場合、下記のように書く。

```cpp
    // @@@ example/cpp_idioms/aaa.cpp #0:0 begin -1
```

特定の型を明示して使用する必要がある場合、下記のように書く。

```cpp
    // @@@ example/cpp_idioms/aaa.cpp #1:0 begin -1
```

関数の戻り値を受け取る変数を宣言する場合、下記のように書く。

```cpp
    // @@@ example/cpp_idioms/aaa.cpp #2:0 begin -1
```

ただし、関数の戻り値型が容易に推測しがたい下記のような場合、
型を明示しないAAAスタイルは使うべきではない。

```cpp
    // @@@ example/cpp_idioms/aaa.cpp #2:1 begin -1
```

インライン関数や関数テンプレートの宣言は、下記のように書く。

```cpp
    // @@@ example/cpp_idioms/aaa.cpp #3:0 begin
```

ただし、インライン関数や関数テンプレートが複雑な下記のような場合、
AAAスタイルは出来る限り避けるべきである。

```cpp
    // @@@ example/cpp_idioms/aaa.cpp #3:1 begin
```

このスタイルには下記のような狙いがある。

* コードの安全性の向上  
  autoで宣言された変数は未初期化にすることができないため、未初期化変数によるバグを防げる。
  また、下記のように縮小型変換(下記では、unsignedからsignedの変換)を防ぐこともできる。

```cpp
    // @@@ example/cpp_idioms/aaa.cpp #4:0 begin -1
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
    // @@@ example/cpp_idioms/aaa.cpp #5:0 begin -1
```
 
### east-const
east-constとは、`const`修飾子を修飾する型要素の右側(east＝右)に置くコーディングスタイルのこと。
つまり「`const`はどの対象を修飾するか」を明確にするため、被修飾対象の直後に const を書くのが特徴である。

このスタイルは、C言語由来の「`const`を左に置く」スタイル([west-const](---))に比べ、
テンプレート展開や型推論の際に一貫性があり、C++コミュニティではしばしば論理的・直感的と評価されている。

```cpp
    // @@@ example/cpp_idioms/east_west_const.cpp #0:0 begin -1
```

このスタイルは 「east constスタイル」 または 「右側const」と呼ばれ、
typeid のデマングル結果や Itanium C++ ABI でもこの形式が採用されている。

### west-const
west-constとは、`const`修飾子を型の左側(west＝左)に置くコーディングスタイルのこと。
C言語からの伝統的な表記法であり、多くの標準ライブラリや教科書でも依然としてこの書き方が用いられている。

可読性は慣れに依存するが、`const`の位置が一貫しないケース(`T* const`など)では理解しづらくなることもある。

```cpp
    // @@@ example/cpp_idioms/east_west_const.cpp #0:1 begin -1
```

このスタイルは「west constスタイル」または「左側const」と呼ばれ、
C言語文化圏での可読性・慣習を重視する場合に採用されることが多い。

## オブジェクトのコピー
### シャローコピー
シャローコピー(浅いコピー)とは、暗黙的、
もしくは=defaultによってコンパイラが生成するようなcopyコンストラクタ、
copy代入演算子が行うコピーであり、[ディープコピー](---)と対比的に使われる概念である。

以下のクラスShallowOKには、コンパイラが生成するcopyコンストラクタ、
copy代入演算子と同等なものを定義したが、これは問題のないシャローコピーである
(が、正しく自動生成される関数を実装すると、メンバ変数が増えた際にバグを生み出すことがあるため、
実践的にはこのようなことはすべきではない)。

```cpp
    // @@@ example/cpp_idioms/deep_shallow_copy_ut.cpp #0:0 begin
```

コンストラクタでポインタのようなリソースを確保し、
デストラクタでそれらを解放するようなクラスの場合、シャローコピーは良く知られた問題を起こす。

下記のShallowNGはその例である。

```cpp
    // @@@ example/cpp_idioms/deep_shallow_copy_ut.cpp #1:0 begin
```

シャローコピーにより、メンバで保持していたポインタ(ポインタが指しているオブジェクトではない)
がコピーされてしまうため、下記のコード内のコメントで示した通り、
メモリリークや2重解放を起こしてしまう。

```cpp
    // @@@ example/cpp_idioms/deep_shallow_copy_ut.cpp #1:1 begin -1
```

### ディープコピー
ディープコピーとは、[シャローコピー](---)が発生させる問題を回避したコピーである。

以下に例を示す。

```cpp
    // @@@ example/cpp_idioms/deep_shallow_copy_ut.cpp #2:0 begin
```

上記クラスのDeepは、copyコンストラクタ、copy代入演算子でポインタをコピーするのではなく、
ポインタが指しているオブジェクトを複製することにより、シャローコピーの問題を防ぐ。


### スライシング
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
    // @@@ example/cpp_idioms/slice_ut.cpp #0:0 begin
```

```cpp
    // @@@ example/cpp_idioms/slice_ut.cpp #0:1 begin -1
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
    // @@@ example/cpp_idioms/slice_ut.cpp #1:0 begin -1
```

![スライシング配列](plant_uml/slicing_array.png)


## C++注意点
### オーバーライドとオーバーロードの違い
下記例では、Base::g()がオーバーロードで、Derived::f()がオーバーライドである
(Derived::g()はオーバーロードでもオーバーライドでもない(「[name-hiding](---)」参照))。


```cpp
    // @@@ example/cpp_idioms/override_overload_ut.cpp #0:0 begin
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
    // @@@ example/cpp_idioms/override_overload_ut.cpp #0:1 begin -1
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

### danglingリファレンス
Dangling リファレンスとは、破棄後のオブジェクトを指しているリファレンスを指す。
このようなリファレンスにアクセスすると、[未定義動作](---)に繋がるに繋がる。

```cpp
    // @@@ example/cpp_idioms/dangling_ut.cpp #0:0 begin

    // @@@ example/cpp_idioms/dangling_ut.cpp #0:1 begin -1
```

### danglingポインタ
danglingポインタとは、[danglingリファレンス](---)と同じような状態になったポインタを指す。


### Most Vexing Parse
Most Vexing Parse(最も困惑させる構文解析)とは、C++の文法に関連する問題で、
Scott Meyersが彼の著書"Effective STL"の中でこの現象に名前をつけたことに由来する。

この問題はC++の文法が関数の宣言と変数の定義とを曖昧に扱うことによって生じる。
特にオブジェクトの初期化の文脈で発生し、意図に反して、その行は関数宣言になってしまう。

```cpp
    // @@@ example/cpp_idioms/most_vexing_parse_ut.cpp #0:0 begin

    // @@@ example/cpp_idioms/most_vexing_parse_ut.cpp #0:1 begin -1
```

[特殊メンバ関数|初期化子リストコンストラクタ](---)の呼び出しでオブジェクトの初期化を行うことで、
このような問題を回避できる。

## ソフトウェア一般
### ハンドル
CやC++の文脈でのハンドルとは、ポインタかリファレンスを指す。

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
    // @@@ example/cpp_idioms/lack_of_cohesion_ut.cpp #0:0 begin
```

良く設計されたクラスは、下記のようにメンバが結合しあっているため凝集度が高い
(ただし、「[Immutable](---)」の観点からは、QuadraticEquation::Set()がない方が良い)。
言い換えれば、凝集度を落とさずにクラスを分割することは難しい。
なお、上記の```LCOM == 9```なっているクラスを凝集性を高く、修正した例を以下に示す。

```cpp
    // @@@ example/cpp_idioms/lack_of_cohesion_ut.cpp #0:1 begin
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
    // @@@ example/cpp_idioms/spurious_wakeup_ut.cpp #0:0 begin
```

std::condition_variable::wait()の第2引数を下記のようにすることでこの現象を回避できる。

```cpp
    // @@@ example/cpp_idioms/spurious_wakeup_ut.cpp #1:0 begin
```

### Static Initialization Order Fiasco(静的初期化順序問題)
静的初期化順序問題とは、
グローバルや名前空間スコープの静的オブジェクトの初期化順序が翻訳単位間で未定義であることに起因する不具合である。
あるオブジェクトAが初期化時に別のオブジェクトBに依存していても、Bがまだ初期化されていない場合、
Aの初期化は未定義の状態となり、不正アクセスやクラッシュを引き起こす可能性がある。

原因は、C++標準が同じ翻訳単位内の静的オブジェクトの初期化順序は保証するが、
異なる翻訳単位間の順序は保証しないことにある。さらに、動的初期化を必要とするオブジェクトでは、
初期化順序の依存関係が問題を起こす。

C++20からこの問題の対策として、[constinit](---)が導入された。

### 副作用
プログラミングにおいて、式の評価による作用には、
主たる作用とそれ以外の
[副作用](https://ja.wikipedia.org/wiki/%E5%89%AF%E4%BD%9C%E7%94%A8_(%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%A0))
(side effect)とがある。
式は、評価値を得ること(関数では「引数を受け取り値を返す」と表現する)が主たる作用とされ、
それ以外のコンピュータの論理的状態(ローカル環境以外の状態変数の値)を変化させる作用を副作用という。
副作用の例としては、グローバル変数や静的ローカル変数の変更、
ファイルの読み書き等のI/O実行、等がある。


### Itanium C++ ABI
ItaniumC++ABIとは、C++コンパイラ間でバイナリ互換性を確保するための規約である。
関数呼び出し規約、クラスレイアウト、仮想関数テーブル、例外処理、
名前修飾(マングリング)などC++のオブジェクト表現と呼び出し方法に関する標準ルールを定めている。

もともとはIntelItanium(IA-64)プロセッサ向けに策定されたが、
[g++](---)や[clang++](---)はx86/x86-64やARM64など多くのプラットフォームでもItaniumC++ABI準拠の規約を採用している。
そのため異なるコンパイラ間でもオブジェクトファイルやライブラリのリンクが可能である。
また、typeid(...).name()をデマングルした場合、
constがeast-const形式(T const)で表示されるのもこのABIの規約によるものである。

| ABI               | 主な対象プラットフォーム  | マングリング規則    | クラスレイアウト  | 例外処理    |
| ----------------- | ------------------------- | ------------------- | ----------------- | ----------- |
| **ItaniumC++ABI** | IA-64, x86, x86-64, ARM64 | east-const形式      | Itanium規則       | Itanium規則 |
| **MSVC C++ABI**   | Windows x86/x64           | 独自形式            | 独自規則          | 独自規則    |
| **ARM C++ABI**    | AArch32, AArch64          | 基本的にItanium準拠だが例外あり         | ARM規則     |


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

## 非ソフトウェア用語
### セマンティクス
シンタックスとは構文論のことであり、セマンティクスとは意味論のことである。
セマンティクス、シンタックスの違いをはっきりと際立たせる以下の有名な例文により、
セマンティクスの意味を直感的に理解することができる。

```
    Colorless green ideas sleep furiously(直訳:無色の緑の考えが猛烈に眠る)
```

この文は文法的には正しい(シンタックス的に成立している)が、意味的には不自然で理解不能である
(セマンティクス的には破綻している)。ノーム・チョムスキーによって提示されたこの例文は、
構文が正しくても意味が成立しないことがあるという事実を示しており、構文と意味の違いを鮮やかに浮かび上がらせる。


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



