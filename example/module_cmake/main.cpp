// @@@ sample begin 0:0

import math;        // モジュールのインポート
import type_traits; // モジュールのインポート

#include <iostream>  // これまで同様のインクルード。stdのモジュール化はC++23から

void f(int a, int b)
{
    std::cout << "Add: " << math::add(a, b) << std::endl;
    std::cout << "Multiply: " << math::multiply(a, b) << std::endl;

    std::cout << std::boolalpha << type_traits::is_void<void>() << std::endl;
    std::cout << std::boolalpha << type_traits::is_void_v<decltype(a)> << std::endl;
}
// @@@ sample end

int main()
{
    f(3, 5);
    return 0;
}
