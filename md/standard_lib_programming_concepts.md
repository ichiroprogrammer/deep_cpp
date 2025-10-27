# 標準ライブラリとプログラミングの概念
この章では、C++標準ライブラリやそれによって導入されたプログラミングの概念等の紹介を行う。

___

__この章の構成__

<!-- index 1-4 -->

[このドキュメントの構成](---)に戻る。  

___


## ユーティリティ
### std::move
std::moveは引数を[rvalueリファレンス](---)に変換する関数テンプレートである。

|引数                 |std::moveの動作                                    |
|---------------------|---------------------------------------------------|
|非const [lvalue](---)|引数を[rvalueリファレンス](---)にキャストする      |
|const [lvalue](---)  |引数をconst [rvalueリファレンス](---)にキャストする|

この表の動作仕様を下記ののコードで示す。

```cpp
    // @@@ example/cpp_standard/utility_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/cpp_standard/utility_ut.cpp #0:1 begin -1
```

std::moveは以下の２つの概念ときわめて密接に関連しており、

* [rvalueリファレンス](---)
* [moveセマンティクス](---)

これら3つが組み合わさることで、不要なコピーを避けた高効率なリソース管理が実現される。

### std::forward
std::forwardは、下記の２つの概念を実現するための関数テンプレートである。

* [forwardingリファレンス](---)
* [perfect forwarding](---)

std::forwardを適切に使用することで、引数の値カテゴリを保持したまま転送でき、
move可能なオブジェクトの不要なコピーを避けることができる。

## type_traits
type_traitsは、型に関する情報をコンパイル時に取得・変換するためのメタ関数群で、
型特性の判定や型操作を静的に行うために用いられる。

以下に代表的なものをいくつか説明する。

- [std::integral_constant](---)
- [std::true_type](---)/[std::false_type](---)
- [std::is_same](---)
- [std::enable_if](---)
- [std::conditional](---)
- [std::is_void](---)
- [std::is_copy_assignable](---)
- [std::is_move_assignable](---)

### std::integral_constant
std::integral_constantは「テンプレートパラメータとして与えられた型とその定数から新たな型を定義する」
クラステンプレートである。

以下に簡単な使用例を示す。

```cpp
    // @@@ example/cpp_standard/type_traits_ut.cpp #0:0 begin -1
```

また、すでに示したようにstd::true_type/std::false_typeを実装するためのクラステンプレートでもある。


### std::true_type
`std::true_type`(と`std::false_type`)は真/偽を返すSTL[メタ関数](---)群の戻り型となる型エイリアスであるため、
最も使われるテンプレートの一つである。

これらは、下記で確かめられる通り、後述する[std::integral_constant](---)を使い定義されている。

```cpp
    // @@@ example/cpp_standard/type_traits_ut.cpp #1:0 begin -1
```

それぞれの型が持つvalue定数は、下記のように定義されている。

```cpp
    // @@@ example/cpp_standard/type_traits_ut.cpp #1:1 begin -1
```

これらが何の役に立つのか直ちに理解することは難しいが、
true/falseのメタ関数版と考えれば、追々理解できるだろう。

以下に簡単な使用例を示す。

```cpp
    // @@@ example/cpp_standard/type_traits_ut.cpp #1:2 begin
```

上記の単体テストは下記のようになる。

```cpp
    // @@@ example/cpp_standard/type_traits_ut.cpp #1:3 begin -1
```

IsCovertibleToIntの呼び出しをdecltypeのオペランドにすることで、
std::true_typeかstd::false_typeを受け取ることができる。

### std::false_type
[std::true_type](---)を参照せよ。

### std::is_same

すでに上記の例でも使用したが、std::is_sameは2つのテンプレートパラメータが

* 同じ型である場合、std::true_type
* 違う型である場合、std::false_type

から派生した型となる。

以下に簡単な使用例を示す。

```cpp
    // @@@ example/cpp_standard/type_traits_ut.cpp #3:0 begin -1
```

また、 C++17で導入されたstd::is_same_vは、定数テンプレートを使用し、
下記のように定義されている。

```cpp
    // @@@ example/cpp_standard/type_traits_ut.cpp #3:1 begin
```

```cpp
    // @@@ example/cpp_standard/type_traits_ut.cpp #3:2 begin -1
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
    // @@@ example/cpp_standard/type_traits_ut.cpp #3:3 begin -1
```

### std::enable_if
std::enable_ifは、bool値である第1テンプレートパラメータが

* trueである場合、型である第2テンプレートパラメータをメンバ型typeとして宣言する。
* falseである場合、メンバ型typeを持たない。

下記のコードはクラステンプレートの特殊化を用いたstd::enable_ifの実装例である。

```cpp
    // @@@ example/cpp_standard/type_traits_ut.cpp #4:0 begin
```

std::enable_ifの使用例を下記に示す。

```cpp
    // @@@ example/cpp_standard/type_traits_ut.cpp #4:1 begin -1
```

実装例から明らかなように

* std::enable_if\<true>::typeは[well-formed](---)
* std::enable_if\<false>::typeは[ill-formed](---)

となるため、下記のコードはコンパイルできない。

```cpp
    // @@@ example/cpp_standard/type_traits_ut.cpp #4:2 begin -1
```

std::enable_ifのこの特性と後述する[SFINAE](---)により、
様々な静的ディスパッチを行うことができる。


### std::conditional

std::conditionalは、bool値である第1テンプレートパラメータが

* trueである場合、第2テンプレートパラメータ
* falseである場合、第3テンプレートパラメータ

をメンバ型typeとして宣言する。

下記のコードはクラステンプレートの特殊化を用いたstd::conditionalの実装例である。

```cpp
    // @@@ example/cpp_standard/type_traits_ut.cpp #5:0 begin
```

std::conditionalの使用例を下記に示す。

```cpp
    // @@@ example/cpp_standard/type_traits_ut.cpp #5:1 begin -1
```

### std::is_void
std::is_voidはテンプレートパラメータの型が

* voidである場合、std::true_type
* voidでない場合、std::false_type

から派生した型となる。

以下に簡単な使用例を示す。

```cpp
    // @@@ example/cpp_standard/type_traits_ut.cpp #2:0 begin -1
```

### std::is_copy_assignable
std::is_copy_assignableはテンプレートパラメータの型(T)がcopy代入可能かを調べる。
Tが[CopyAssignable要件](---)を満たすためには`std::is_copy_assignable<T>`がtrueでなければならないが、
その逆が成立するとは限らない。


### std::is_move_assignable
std::is_move_assignableはテンプレートパラメータの型(T)がmove代入可能かを調べる。
Tが[MoveAssignable要件](---)を満たすためには`std::is_move_assignable<T>`がtrueでなければならないが、
その逆が成立するとは限らない。


## 並列処理

### std::thread
クラスthread は、新しい実行のスレッドの作成/待機/その他を行う機構を提供する。

```cpp
    // @@@ example/cpp_standard/thread_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/cpp_standard/thread_ut.cpp #0:1 begin -1
```

### std::mutex
mutex は、スレッド間で使用する共有リソースを排他制御するためのクラスである。 

<pre>
- lock()    :メンバ関数によってリソースのロックを取得
- unlock()  :メンバ関数でリソースのロックを解放
</pre>

```cpp
    // @@@ example/cpp_standard/thread_ut.cpp #1:0 begin
    // @@@ example/cpp_standard/thread_ut.cpp #1:1 begin
    // @@@ example/cpp_standard/thread_ut.cpp #1:2 begin
```
```cpp
    // @@@ example/cpp_standard/thread_ut.cpp #1:3 begin -1
```

lock()を呼び出した状態で、unlock()を呼び出さなかった場合、デッドロックを引き起こしてしまうため、
永久に処理が完了しないバグの元となり得るため、このような問題を避けるために、
mutexは通常、[std::lock_guard](---)と組み合わせて使われる。

```cpp

    // @@@ example/cpp_standard/thread_ut.cpp #1:1 begin -1
```

### std::atomic
atomicクラステンプレートは、型Tをアトミック操作するためのものである。
[組み込み型](---)に対する特殊化が提供されており、それぞれに特化した演算が用意されている。
[std::mutex](---)で示したような単純なコードではstd::atomicを使用して下記のように書く方が一般的である。

```cpp
    // @@@ example/cpp_standard/thread_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/cpp_standard/thread_ut.cpp #2:1 begin -1
```

## ロック所有ラッパー
ロック所有ラッパーとはミューテックスのロックおよびアンロックを管理するための以下のクラスを指す。

- [std::lock_guard](---)
- [std::unique_lock](---)
- [std::scoped_lock](---)


### std::lock_guard

std::lock_guardを使わない問題のあるコードを以下に示す。

```cpp
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #0:0 begin
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #0:1 begin
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #0:2 begin
```
```cpp
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #0:1 begin -1
```

上記で示したConflict::increment()には以下のようなリスクが存在する。

1. 関数が複雑化してエクセプションを投げる可能性がある場合、
    - エクセプションをこの関数内で捕捉し、ロック解除 (mtx_.unlock()) を行った上で再スローしなければならない。
    - ロック解除を忘れるとデッドロックにつながる。

2. 複数の return 文を持つように関数が拡張された場合、
    - すべての return の前で mtx_.unlock() を呼び出さなければならない。

これらを正しく管理するためには、重複コードが増え、関数の保守性が著しく低下する。

std::lock_guardを使用して、このような問題に対処したコードを以下に示す。

```cpp
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #1:0 begin
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #1:1 begin
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #1:2 begin
```

単体テストに変更は無いため、省略する。

オリジナルの単純な以下のincrement()と改善版を比較すると、大差ないように見えるが、

```cpp
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #0:1 begin -1
```

オリジナルのコードで指摘したすべてのリスクが、わずか一行の変更で解決されている。

```cpp
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #1:1 begin -1
```

### std::unique_lock
std::unique_lockとは、ミューテックスのロック管理を柔軟に行えるロックオブジェクトである。
std::lock_guardと異なり、ロックの手動解放や再取得が可能であり、特にcondition_variable::wait()と組み合わせて使用される。
wait()は内部でロックを一時的に解放し、通知受信後に再取得する。

下記の例では、IntQueue::push()、 IntQueue::pop_ng()、
IntQueue::pop_ok()の中で行われるIntQueue::q_へのアクセスで発生する競合を回避するためにIntQueue::mtx_を使用する。

下記のコード例では、[std::lock_guard](---)の説明で述べたようにmutex::lock()、mutex::unlock()を直接呼び出すのではなく、
std::unique_lockやstd::lock_guardによりmutexを使用する。

```cpp
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #2:0 begin
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #2:1 begin
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #2:2 begin
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #2:3 begin
```
```cpp
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #2:4 begin -1
```

一般に条件変数には、[Spurious Wakeup](---)という問題があり、std::condition_variableも同様である。

上記の抜粋である下記のコード例では[Spurious Wakeup](---)の対策が行われていないため、
意図通り動作しない可能性がある。

```cpp
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #2:1 begin -1
```

下記のIntQueue::pop_ok()は、pop_ng()にSpurious Wakeupの対策を施したものである。

```cpp
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #2:2 begin -1
```

### std::scoped_lock
std::scoped_lockとは、複数のミューテックスを同時にロックするためのロックオブジェクトである。
C++17で導入され、デッドロックを回避しながら複数のミューテックスを安全にロックできる。

複数のミューテックスを扱う際、異なるスレッドが異なる順序でロックを取得しようとすると、
デッドロックが発生する可能性がある。下記の例では、2つの銀行口座間で送金を行う際に、
両方の口座を同時にロックする必要がある。

```cpp
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #3:0 begin
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #3:1 begin
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #3:2 begin
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #3:3 begin
```
下記の例では、2つのスレッドがそれぞれ逆方向の送金を同時に行う。
transfer_ok()の代わりにtransfer_ng()を使用した場合、デッドロックが発生する可能性がある。

```cpp
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #3:4 begin -1
```

transfer_ng()がデッドロックを引き起こすシナリオは、以下のようなものである。

1. スレッド1が acc1.transfer_ng(acc2, 100) を呼び出し、acc1.mtx_ をロック
2. スレッド2が acc2.transfer_ng(acc1, 100) を呼び出し、acc2.mtx_ をロック
3. スレッド1が acc2.mtx_ のロックを試みるが、スレッド2が保持しているため待機
4. スレッド2が acc1.mtx_ のロックを試みるが、スレッド1が保持しているため待機
5. 互いに相手のロック解放を待ち続け、永遠に進まない（デッドロック）

![layout](plant_uml/mutex_deadlock.png)


下記のBankAccount::transfer_ok()は、std::scoped_lockを使用して前述したデッドロックを回避したものである。

```cpp
    // @@@ example/cpp_standard/lock_ownership_wrapper_ut.cpp #3:2 begin -1
```

## スマートポインタ
スマートポインタは、C++標準ライブラリが提供するメモリ管理クラス群を指す。
生のポインタの代わりに使用され、リソース管理を容易にし、
メモリリークや二重解放といった問題を防ぐことを目的としている。

スマートポインタは通常、所有権とスコープに基づいてメモリの解放を自動的に行う。
C++標準ライブラリでは、主に以下の3種類のスマートポインタが提供されている。

* [std::unique_ptr](---)
* [std::shared_ptr](---)
* [std::weak_ptr](---)
* [std::auto_ptr](---)

### std::unique_ptr
std::unique_ptrは、C++11で導入されたスマートポインタの一種であり、std::shared_ptrとは異なり、
[オブジェクトの排他所有](---)を表すために用いられる。所有権は一つのunique_ptrインスタンスに限定され、
他のポインタと共有することはできない。ムーブ操作によってのみ所有権を移譲でき、
スコープを抜けると自動的にリソースが解放されるため、メモリ管理の安全性と効率性が向上する。

### std::shared_ptr
std::shared_ptrは、同じくC++11で導入されたスマートポインタであり、[オブジェクトの共有所有](---)を表すために用いられる。
複数のshared_ptrインスタンスが同じリソースを参照でき、
内部の参照カウントによって最後の所有者が破棄された時点でリソースが解放される。
[std::weak_ptr](---)は、shared_ptrと連携して使用されるスマートポインタであり、オブジェクトの非所有参照を表す。
参照カウントには影響せず、循環参照を防ぐために用いられる。weak_ptrから一時的にshared_ptrを取得するにはlock()を使用する。

### std::weak_ptr
std::weak_ptrは、スマートポインタの一種である。

std::weak_ptrは参照カウントに影響を与えず、[std::shared_ptr](---)とオブジェクトを共有所有するのではなく、
その`shared_ptr`インスタンスとの関連のみを保持するのため、[オブジェクトの循環所有](---)の問題を解決できる。

[オブジェクトの循環所有](---)で示した問題のあるクラスの修正版を以下に示す
(以下の例では、Xは前のままで、Yのみ修正した)。

```cpp
    // @@@ example/cpp_standard/weak_ptr_ut.cpp #0:0 begin
    // @@@ example/cpp_standard/weak_ptr_ut.cpp #0:1 begin
    // @@@ example/cpp_standard/weak_ptr_ut.cpp #0:2 begin
```

このコードからわかるように修正版YはXオブジェクトを参照するために、
`std::shared_ptr<X>`の代わりに`std::weak_ptr<X>`を使用する。
Xオブジェクトにアクセスする必要があるときに、
下記のY::WhoIsWith()関数の内部処理のようにすることで、`std::weak_ptr<X>`オブジェクトから、
それと紐づいた`std::shared_ptr<X>`オブジェクトを生成できる。

なお、上記コードは[初期化付きif文](---)を使うことで、
生成した`std::shared_ptr<X>`オブジェクトのスコープを最小に留めている。

```cpp
    // @@@ example/cpp_standard/weak_ptr_ut.cpp #0:1 begin
```

Xと修正版Yの単体テストによりメモリーリークが修正されたことを以下に示す。

```cpp
    // @@@ example/cpp_standard/weak_ptr_ut.cpp #0:3 begin -1
```

上記コード例で見てきたように`std::weak_ptr`を使用することで:

- 循環参照によるメモリリークを防ぐことができる
- 必要に応じて`lock()`でオブジェクトにアクセスできる
- オブジェクトが既に解放されている場合は`lock()`が空の`shared_ptr`を返すため、安全に処理できる

### std::auto_ptr
`std::auto_ptr`はC++11以前に導入された初期のスマートポインタであるが、異常な[copyセマンティクス](---)を持つため、
多くの誤用を生み出し、C++11から非推奨とされ、C++17から規格から排除された。

## コンテナ
データを格納し、
効率的に操作するための汎用的なデータ構造を提供するC++標準ライブラリの下記のようなクラス群である。

* [シーケンスコンテナ(Sequence Containers)](---)
* [連想コンテナ(Associative Containers)(---)
* [無順序連想コンテナ(Unordered Associative Containers)](---)
* [コンテナアダプタ(Container Adapters)](---)
* [特殊なコンテナ](---)

### シーケンスコンテナ(Sequence Containers)
データが挿入順に保持され、順序が重要な場合に使用する。

| コンテナ                 | 説明                                                                |
|--------------------------|---------------------------------------------------------------------|
| `std::vector`            | 動的な配列で、ランダムアクセスが高速。末尾への挿入/削除が効率的     |
| `std::deque`             | 両端に効率的な挿入/削除が可能な動的配列                             |
| `std::list`              | 双方向リスト。要素の順序を維持し、中間の挿入/削除が効率的           |
| [std::forward_list](---) | 単方向リスト。軽量だが、双方向の操作はできない                      |
| `std::array`             | 固定長配列で、サイズがコンパイル時に決まる                          |
| `std::string`            | 可変長の文字列を管理するクラス(厳密には`std::basic_string`の特殊化) |

#### std::forward_list

```cpp
    // @@@ example/cpp_standard/container_ut.cpp #0:0 begin -1
```

### 連想コンテナ(Associative Containers)
データがキーに基づいて自動的にソートされ、検索が高速である。

| コンテナ           | 説明                                             |
|--------------------|--------------------------------------------------|
| `std::set`         | 要素がソートされ、重複が許されない集合           |
| `std::multiset`    | ソートされるが、重複が許される集合               |
| `std::map`         | ソートされたキーと値のペアを保持。キーは一意     |
| `std::multimap`    | ソートされたキーと値のペアを保持。キーは重複可能 |

### 無順序連想コンテナ(Unordered Associative Containers)
ハッシュテーブルを基盤としたコンテナで、順序を保証しないが高速な検索を提供する。

| コンテナ                  | 説明                                                   |
|---------------------------|--------------------------------------------------------|
| [std::unordered_set](---) | ハッシュテーブルベースの集合。重複は許されない         |
| `std::unordered_multiset` | ハッシュテーブルベースの集合。重複が許される           |
| [std::unordered_map](---) | ハッシュテーブルベースのキーと値のペア。キーは一意     |
| `std::unordered_multimap` | ハッシュテーブルベースのキーと値のペア。キーは重複可能 |
| [std::type_index](---)    | 型情報型を連想コンテナのキーとして使用するためのクラス |

#### std::unordered_set

```cpp
    // @@@ example/cpp_standard/container_ut.cpp #1:0 begin -1
```

#### std::unordered_map

```cpp
    // @@@ example/cpp_standard/container_ut.cpp #2:0 begin -1
```

#### std::type_index
std::type_indexはコンテナではないが、
型情報型を連想コンテナのキーとして使用するためのクラスであるため、この場所に掲載する。

```cpp
    // @@@ example/cpp_standard/container_ut.cpp #3:0 begin -1
```


### コンテナアダプタ(Container Adapters)
特定の操作のみを公開するためのラッパーコンテナ。

| コンテナ              | 説明                                     |
|-----------------------|------------------------------------------|
| `std::stack`          | LIFO(後入れ先出し)操作を提供するアダプタ |
| `std::queue`          | FIFO(先入れ先出し)操作を提供するアダプタ |
| `std::priority_queue` | 優先度に基づく操作を提供するアダプタ     |

### 特殊なコンテナ
上記したようなコンテナとは一線を画すが、特定の用途や目的のために設計された一種のコンテナ。

| コンテナ             | 説明                                                       |
|----------------------|------------------------------------------------------------|
| `std::span`          | 生ポインタや配列を抽象化し、安全に操作するための軽量ビュー |
| `std::bitset`        | 固定長のビット集合を管理するクラス                         |
| `std::basic_string`  | カスタム文字型をサポートする文字列コンテナ                 |

## std::optional
C++17から導入されたstd::optionalには、以下のような2つの用途がある。
以下の用途2から、
このクラスがオブジェクトのダイナミックなメモリアロケーションを行うような印象を受けるが、
そのようなことは行わない。
このクラスがオブジェクトのダイナミックな生成が必要になった場合、プレースメントnewを実行する。
ただし、std::optionalが保持する型自身がnewを実行する場合は、この限りではない。

1. 関数の任意の型の[戻り値の無効表現](---)を持たせる
2. [オブジェクトの遅延初期化](---)する(初期化処理が重く、
   条件によってはそれが無駄になる場合にこの機能を使う)

### 戻り値の無効表現
```cpp
    // @@@ example/cpp_standard/optional_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/cpp_standard/optional_ut.cpp #0:1 begin -1
```

### オブジェクトの遅延初期化
```cpp
    // @@@ example/cpp_standard/optional_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/cpp_standard/optional_ut.cpp #1:1 begin -1
```

## std::variant
std::variantは、C++17で導入された型安全なunionである。
このクラスは複数の型のうち1つの値を保持することができ、
従来のunionに伴う低レベルな操作の安全性の問題を解消するために設計された。

std::variant自身では、オブジェクトのダイナミックな生成が必要な場合でも通常のnewを実行せず、
代わりにプレースメントnewを用いる
(以下のコード例のようにstd::variantが保持する型自身がnewを実行する場合は、この限りではない)。

以下にstd::variantの典型的な使用例を示す。

```cpp
    // @@@ example/cpp_standard/variant_ut.cpp #0:0 begin -1
```

std::variantとstd::visit([Visitor](---)パターンの実装の一種)を組み合わせた場合の使用例を以下に示す。

```cpp
    // @@@ example/cpp_standard/variant_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/cpp_standard/variant_ut.cpp #1:1 begin -1
```

## オブジェクトの比較
### std::rel_ops
クラスに`operator==`と`operator<`の2つの演算子が定義されていれば、
それがメンバか否かにかかわらず、他の比較演算子 !=、<=、>、>= はこれらを基に自動的に導出できる。
std::rel_opsでは`operator==`と`operator<=` を基に他の比較演算子を機械的に生成する仕組みが提供されている。

次の例では、std::rel_opsを利用して、少ないコードで全ての比較演算子をサポートする例を示す。

```cpp
    // @@@ example/cpp_standard20/comparison_operator_ut.cpp #0:1 begin -1
```

なお、std::rel_opsはC++20から導入された[<=>演算子](---)により不要になったため、
非推奨とされた。

### std::tuppleを使用した比較演算子の実装方法
クラスのメンバが多い場合、[==演算子](---)で示したような方法は、
可読性、保守性の問題が発生する場合が多い。下記に示す方法はこの問題を幾分緩和する。

```cpp
    // @@@ example/cpp_standard20/comparison_operator_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/cpp_standard20/comparison_operator_ut.cpp #2:1 begin
```

## その他
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
    // @@@ example/cpp_standard/heap_allocation_elision_ut.cpp #0:0 begin
```

この最適化により、std::make_sharedのようにstd::shared_ptrの参照カウントを管理するメモリブロックと、
オブジェクトの実体を1つのヒープ領域に割り当てることができ、
ダイナミックメモリアロケーションが1回に抑えられるため、メモリアクセスが高速化される。



