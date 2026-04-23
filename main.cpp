

#include "src.hpp"

#include <iostream>
#include <map>
#include <vector>

int main() {
  sjtu::any_ptr a = sjtu::make_any_ptr(1);
  sjtu::any_ptr v = sjtu::make_any_ptr(std::vector<int>{1, 2, 3});
  sjtu::any_ptr m = sjtu::make_any_ptr(std::map<int, int>{{1, 2}, {3, 4}});
  std::cout << a.unwrap<int>() << std::endl;  // 1
  std::cout << v.unwrap<std::vector<int>>().size() << std::endl;  // 3
  std::cout << m.unwrap<std::map<int, int>>().size() << std::endl;  // 2
}


