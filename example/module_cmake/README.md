# cmake_import_export

# src-tree
    project_root/
    ├── CMakeLists.txt  # ビルド設定
    ├── main.cpp        # メインファイル
    ├── math.cppm       # mathモジュール
    └── type_traits.cppm # type_traitsモジュール


# コンパイル
    # モジュールのコンパイル
    clang++ -std=c++20 -fmodules-ts --precompile math.cppm -o math.pcm

    # .cppのコンパイル
    clang++ -std=c++20 -fmodules-ts -fmodule-file=math=math.pcm -c main.cpp -o main.o

    clang++ math.o main.o -o main
