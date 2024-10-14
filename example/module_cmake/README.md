# cmake_import_export

# src-tree
    project_root/
    $B('(!(!(B CMakeLists.txt  # $B%S%k%I@_Dj(B
    $B('(!(!(B main.cpp        # $B%a%$%s%U%!%$%k(B
    $B('(!(!(B math.cppm       # math$B%b%8%e!<%k(B
    $B(&(!(!(B type_traits.cppm # type_traits$B%b%8%e!<%k(B


# $B%3%s%Q%$%k(B
    # $B%b%8%e!<%k$N%3%s%Q%$%k(B
    clang++ -std=c++20 -fmodules-ts --precompile math.cppm -o math.pcm

    # .cpp$B$N%3%s%Q%$%k(B
    clang++ -std=c++20 -fmodules-ts -fmodule-file=math=math.pcm -c main.cpp -o main.o

    clang++ math.o main.o -o main
