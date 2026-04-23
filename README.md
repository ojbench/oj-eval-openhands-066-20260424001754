# Problem 066 - Conless 的 any_ptr

**ACMOJ Problem ID**: 2090

## Table of Contents

- [Problem 066 - Conless 的 any_ptr](#problem-066-conless-的-any_ptr)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
    - [Background](#background)
  - [Assignment Description](#assignment-description)
    - [Grade Composition](#grade-composition)
  - [Assignment Requirements](#assignment-requirements)
    - [Input Format](#input-format)
    - [Output Format](#output-format)
    - [Samples](#samples)
    - [Data Constraints](#data-constraints)
  - [Per-Testcase Resource Limits](#per-testcase-resource-limits)
  - [Test Data](#test-data)
  - [Submission Requirements](#submission-requirements)
    - [OJ Git Repository Compilation Process](#oj-git-repository-compilation-process)
    - [Git Configuration Requirements](#git-configuration-requirements)
    - [Submission Guidelines](#submission-guidelines)
    - [Evaluation Notes](#evaluation-notes)
    - [Academic Integrity](#academic-integrity)

## Introduction

### Background

Implement type-erased smart pointer (any_ptr) supporting any type, with reference counting and garbage collection.

## Assignment Description

### Grade Composition

| Grading Component | Percentage |
| :--: | :--: |
| Pass **2090. Conless 的 any_ptr** | 80% |
| Code Review | 20% |

Here are several points that need clarification:

- In the Code Review, we will **strictly examine your code style and repository organization structure, etc.**

- This assignment provides some sample data for testing, stored in the `/workspace/data/066/data_test/` directory. Note that these are not the test cases on the Online Judge. Passing all local test cases does not guarantee that you will pass the OJ tests.

- Besides the provided sample data, we also encourage you to design your own test data based on your program logic to assist debugging.

## Assignment Requirements

### Problem Description

本题的题面和下发数据可以在 [attachment]any_ptr.zip[/attachment] / [JBox](https://jbox.sjtu.edu.cn/l/u1V9Fq) 下载。<a href="https://wwqd.lanzoul.com/iQy5d1ikpcli">备用下载地址</a>。

和你们一样，正在当程设助教的 ~~anonymous~~ 大一也辛苦地学习了一年 C++，培养了扎实的代码能力。进入大二，由于课程安排，她在平时作业中更多使用的语言变成了 Python；她这才发现 C++ 写起来有多么的麻烦：静态类型、内存管理……

Conless 看她这么痛苦，只好接下了帮她“简化”C++的任务，说是简化，其实也就是写个可以存放任意类型的智能指针 any_ptr 类。这样一来，以下操作就是有可能的了：

```cpp
any_ptr a = make_any_ptr(1);
cout << a.unwrap<int>() << endl; // 1
a = make_any_ptr("Hello, World!");
cout << a.unwrap<string>() << endl; // Hello, World!
```

除此以外，该类型还可以根据引用计数自动释放空间（类似 `std::shared_ptr` 类型），实现垃圾回收。但是 Conless 急着去踢球，就把这个任务踢给你们了。

下面是代码模版，你只需要实现 any_ptr 类的各成员函数。

```cpp
#ifndef SRC_HPP
#define SRC_HPP

#include <stdexcept>
#include <initializer_list>
// 以上是你所需要的头文件，如果你想使用其它头文件，请询问助教
// 禁止使用 std::shared_ptr 与 std::any

namespace sjtu {

class any_ptr {
 public:
  /**
   * @brief 默认构造函数，行为应与创建空指针类似
   * 
   */
  any_ptr() {}

  /**
   * @brief 拷贝构造函数，要求拷贝的层次为浅拷贝，即该对象与被拷贝对象的内容指向同一块内存
   * @note 若将指针作为参数传入，则指针的生命周期由该对象管理
   * @example
   *  any_ptr a = make_any_ptr(1);
   *  any_ptr b = a;
   *  a.unwrap<int> = 2;
   *  std::cout << b << std::endl; // 2
   * @param other
   */
  any_ptr(const any_ptr &other) {}
  template <class T> any_ptr(T *ptr) {}

  /**
   * @brief 析构函数，注意若一块内存被多个对象共享，那么只有最后一个析构的对象需要释放内存
   * @example
   *  any_ptr a = make_any_ptr(1);
   *  {
   *    any_ptr b = a;
   *  }
   *  std::cout << a << std::endl; // valid
   * @example
   *  int *p = new int(1);
   *  {
   *    any_ptr a = p;
   *  }
   *  std::cout << *p << std::endl; // invalid
   * 
   */
  ~any_ptr() {}

  /**
   * @brief 拷贝赋值运算符，要求拷贝的层次为浅拷贝，即该对象与被拷贝对象的内容指向同一块内存
   * @note 应与指针拷贝赋值运算符的语义相近
   * @param other
   * @return any_ptr&
   */
  any_ptr &operator=(const any_ptr &other) {}
  template <class T> any_ptr &operator=(T *ptr) {}

  /**
   * @brief 获取该对象指向的值的引用
   * @note 若该对象指向的值不是 T 类型，则抛出异常 std::bad_cast
   * @example
   *  any_ptr a = make_any_ptr(1);
   *  std::cout << a.unwrap<int>() << std::endl; // 1
   * @tparam T
   * @return T&
   */
  template <class T> T &unwrap() {}
  // 某一个 any_ptr 类对象可能为 const，请你补充 unwrap 函数
};

/**
 * @brief 由指定类型的值构造一个 any_ptr 对象
 * @example
 *  any_ptr a = make_any_ptr(1);
 *  any_ptr v = make_any_ptr<std::vector<int>>(1, 2, 3);
 *  any_ptr m = make_any_ptr<std::map<int, int>>({{1, 2}, {3, 4}});
 * @tparam T
 * @param t
 * @return any_ptr
 */
template <class T> any_ptr make_any_ptr(const T &t) { return any_ptr(new T(t)); }
// 某些 any_ptr 类对象可能由不定长参数或初始化列表构造，请你参照上方的 example 补充 make_any_ptr 函数，我们会有一个特殊的测试点测试你的程序是否完成要求

}  // namespace sjtu

#endif
```

### Input Format

见下发文件中的 [example1.cpp](https://jbox.sjtu.edu.cn/l/u1V9Fq) 和 [example2.cpp](https://jbox.sjtu.edu.cn/l/u1V9Fq)

### Output Format

见下发文件中的注释

### Samples

No sample data provided for this problem.

### Data Constraints

<table border="1" cellpadding="1" cellspacing="1">
	<thead>
		<tr>
			<th scope="col" style="text-align:center">测试点</th>
			<th scope="col" style="text-align:center">考察内容</th>
			<th scope="col" style="text-align:center">数据类型</th>
			<th scope="col" style="text-align:center">分值</th>
		</tr>
	</thead>
	<tbody>
		<tr>
      <td style="text-align:center"> 1 </td>
      <td style="text-align:center"> any_ptr 的基础使用 </td>
      <td rowspan="2" style="text-align:center"> 内建简单类型 (int, double, std::string) </td>
			<td style="text-align:center"> 20% </td>
    </tr>
		<tr>
			<td style="text-align:center"> 2 </td>
			<td rowspan="1" style="text-align:center"> any_ptr 的析构正确性 </td>
			<td style="text-align:center"> 20% </td>
		</tr>
		<tr>
			<td style="text-align:center"> 3 </td>
			<td rowspan="1" style="text-align:center"> any_ptr 的特殊使用</td>
			<td style="text-align:center"> 内建复杂类型 (std::vector, std::map, std::set) </td>
			<td style="text-align:center"> 20% </td>
		</tr>
		<tr>
			<td style="text-align:center"> 4 </td>
			<td rowspan="2"style="text-align:center"> 所有内容 </td>
			<td rowspan="2" style="text-align:center"> 自定义类型 </td>
			<td rowspan="2" style="text-align:center"> 40% </td>
		</tr>
		<tr>
			<td style="text-align:center"> 5 </td>
		</tr>
  </tbody>
</table>

## Per-Testcase Resource Limits

- **Time Limit (per test case)**: 1000 ms
- **Memory Limit (per test case)**: 512 MiB
- **Disk Usage**: No disk usage is permitted.

## Test Data

The test data for this problem is located at `/workspace/data/066/data_test/`.

Attachments and starter files for this problem are located at `/workspace/data/066/`.

## Submission Requirements

### OJ Git Repository Compilation Process

For Git compilation, we will first clone the repository using a command similar to:
```bash
git clone <repo_url> . --depth 1 --recurse-submodules --shallow-submodules --no-local
```

Then we check if there is a `CMakeLists.txt` file. If it exists, we run (if not, a warning message will be displayed):
```bash
cmake .
```

Finally, we check if there is any of `GNUmakefile`/`makefile`/`Makefile` (if cmake was run previously, this will be the generated Makefile). If it exists, we run (if not, a warning message will be displayed):
```bash
make
```

After this process is complete, we will use the `code` file in the project root directory as the compilation result.

A `CMakeLists.txt` file is provided in the project. You can use or modify it as needed. The local environment has gcc-13 and g++-13 available.

### Git Configuration Requirements

**IMPORTANT**: You must create a `.gitignore` file in your project root directory to avoid OJ evaluation conflicts.

The `.gitignore` file should include at least the following entries:

```gitignore
CMakeFiles/
CMakeCache.txt
```

### Submission Guidelines

- The submitted code must be able to compile successfully through the above compilation process
- The compiled executable file name must be `code`
- The program needs to be able to read data from standard input and write results to standard output
- Please ensure the code runs correctly within the given time and space limits
- **You must use C++ or C language** to implement this assignment

### Evaluation Notes

- The evaluation system will test your program using the provided test data
- The program output must exactly match the expected output (including format)
- Exceeding time or memory limits will be judged as the corresponding error type
- Please pay attention to the overall time performance of your code and the time complexity of each part of your algorithm.

### Academic Integrity

If any violations are found during evaluation or code review (including but not limited to using unconventional methods to pass test cases), your final score may be significantly reduced or become **0 points**.
