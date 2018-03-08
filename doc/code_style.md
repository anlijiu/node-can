# Google code style

## 命名

### 文件命名
文件名要全部小写, 以 '_' 分隔
例如 my_useful_class.cc

C++ 文件要以 .cc 结尾, 头文件以 .h 结尾. 
联函数必须放在 .h 文件中. 如果内联函数比较短, 就直接放在 .h 中.

### 类型命名
类型名称的每个单词首字母均大写, 不包含下划线
例如 MyExcitingClass, MyExcitingEnum.
类型包括 struct, class, enum , typedef, using别名

### 变量命名
变量 (包括函数参数) 和数据成员名一律小写, 单词之间用下划线连接. 
类的成员变量以下划线结尾, 但结构体不用
如: 
1. 普通变量:       a_local_variable
2. struct成员变量: a_struct_data_memberx
3. class成员变量:  a_class_data_member_

### 常量命名
声明为 constexpr 或 const 的变量, 或在程序运行期间其值始终保持不变的, 命名时以 “k” 开头, 大小写混合. 
例如: const int kDaysInAWeek = 7;

### 函数命名
常规函数使用大小写混合, 函数名的每个单词首字母大写 (即 “驼峰变量名”) 没有下划线. 
例如: 写作 StartRpc() 而非 StartRPC().

### 命名空间命名
命名空间以小写字母命名. 最高级命名空间的名字取决于项目名称. 
建议使用更独特的项目标识符 (websearch::index, websearch::index_util) 而非常见的极易发生冲突的名称 (比如 websearch::util).

### 枚举命名
枚举以常量为标准 
2009 年 1 月之前, 我们一直建议采用 宏 的方式命名枚举值. 由于枚举值和宏之间的命名冲突, 
直接导致了很多问题. 由此, 这里改为优先选择常量风格的命名方式. 
例如: kErrorOutOfMemory
```cpp
enum UrlTableErrors {
  kOK = 0,
  kErrorOutOfMemory,
  kErrorMalformedInput,
};
```

### 宏命名
通常不应该 使用宏. 如果不得不用,  像这样命名: MY_MACRO_THAT_SCARES_SMALL_CHILDREN.
使用宏时要非常谨慎, 尽量以内联函数, 枚举和常量代替之.

译者（acgtyrant）笔记
感觉 Google 的命名约定很高明, 比如写了简单的类 QueryResult, 
接着又可以直接定义一个变量 query_result, 区分度很好; 
再次, 类内变量以下划线结尾, 那么就可以直接传入同名的形参, 
比如 TextQuery::TextQuery(std::string word) : word_(word) {} . 
其中 word_ 自然是类内私有成员.


## 特性

###　引用参数
所有按引用传递的参数必须加上 const.
函数参数列表中, 所有引用参数都必须是 const:
输入参数是值参或 const 引用, 输出参数为指针. 输入参数可以是 const 指针, 但决不能是非 const 的引用参数，除非用于交换，比如 swap().

### 右值引用
只在定义移动构造函数与移动赋值操作时使用右值引用. 不要使用 std::forward.

右值引用是一种只能绑定到临时对象的引用的一种, 其语法与传统的引用语法相似. 例如, void f(string&& s); 声明了一个其参数是一个字符串的右值引用的函数.
只在定义移动构造函数与移动赋值操作时使用右值引用. 不要使用 std::forward.
右值引用是一种只能绑定到临时对象的引用的一种, 其语法与传统的引用语法相似. 例如, void f(string&& s); 声明了一个其参数是一个字符串的右值引用的函数.
tring>, 则 auto v2(std::move(v1)) 将很可能不再进行大量的数据复制而只是简单地进行指针操作, 在某些情况下这将带来大幅度的性能提升.

右值引用使得编写通用的函数封装来转发其参数到另外一个函数成为可能, 无论其参数是否是临时对象都能正常工作.

右值引用能实现可移动但不可拷贝的类型, 这一特性对那些在拷贝方面没有实际需求, 但有时又需要将它们作为函数参数传递或塞入容器的类型很有用.

要高效率地使用某些标准库类型, 例如 std::unique_ptr, std::move 是必需的.
你可能会使用 std::move 来表示将值从一个对象移动而不是复制到另一个对象.

### 函数重载
若要用好函数重载，最好能让读者一看调用点（call site）就胸有成竹，不用花心思猜测调用的重载函数到底是哪一种。该规则适用于构造函数。
你可以编写一个参数类型为 const string& 的函数, 然后用另一个参数类型为 const char* 的函数重载它:
```cpp
class MyClass {
public:
  void Analyze(const string &text);
  void Analyze(const char *text, size_t textlen);
};
```
如果您打算重载一个函数, 可以试试改在函数名里加上参数信息。
例如，用 AppendString() 和 AppendInt() 等， 而不是一口气重载多个 Append().

### 缺省参数
我们不允许使用缺省函数参数，少数极端情况除外。尽可能改用函数重载。

其一，位于 .cc 文件里的静态函数或匿名空间函数，毕竟都只能在局部文件里调用该函数了。

其二，可以在构造函数里用缺省参数，毕竟不可能取得它们的地址。

其三，可以用来模拟变长数组。
```cpp
// 通过空 AlphaNum 以支持四个形参
string StrCat(
  const AlphaNum &a,
  const AlphaNum &b = gEmptyAlphaNum,
  const AlphaNum &c = gEmptyAlphaNum,
  const AlphaNum &d = gEmptyAlphaNum
  );
```

### 变长数组和 alloca()
我们不允许使用变长数组和 alloca().
改用更安全的分配器（allocator），就像 std::vector 或 std::unique_ptr<T[]>.

### 友元

我们允许合理的使用友元类及友元函数.
通常友元应该定义在同一文件内, 经常用到友元的一个地方是将 FooBuilder 声明为 Foo 的友元, 以便 FooBuilder 正确构造 Foo 的内部状态.
某些情况下, 相对于将类成员声明为 public, 使用友元是更好的选择, 尤其是如果你只允许另一个类访问该类的私有成员时. 

### 异常
我们不使用 C++ 异常!!
因为方便

### 运行时类型识别
我们禁止使用 RTTI.
RTTI 允许程序员在运行时识别 C++ 类对象的类型. 它通过使用 typeid 或者 dynamic_cast 完成.
在运行时判断类型通常意味着设计问题. 如果你需要在运行期间确定一个对象的类型, 这通常说明你需要考虑重新设计你的类.

### 类型转换

使用 C++ 的类型转换, 如 static_cast<>(). 不要使用 int y = (int)x 或 int y = int(x) 等转换方式;
用 static_cast 替代 C 风格的值转换, 或某个类指针需要明确的向上转换为父类指针时.
用 const_cast 去掉 const 限定符.
用 reinterpret_cast 指针类型和整型或其它指针之间进行不安全的相互转换. 仅在你对所做一切了然于心时使用.

### 流
只在记录日志时使用流.

### 前置自增和自减
对于迭代器和其他模板对象使用前缀形式 (++i) 的自增, 自减运算符.

### constexpr
在 C++11 里，用 constexpr 来定义真正的常量，或实现常量初始化。

### 整形
C++ 内建整型中, 仅使用 int. 如果程序中需要不同大小的变量, 可以使用 <stdint.h> 中长度精确的整型, 如 int16_t.
如果您的变量可能不小于 2^31 (2GiB), 就用 64 位变量比如 int64_t. 此外要留意，哪怕您的值并不会超出 int 所能够
表示的范围，在计算过程中也可能会溢出。所以拿不准时，干脆用更大的类型。

<stdint.h> 定义了 int16_t, uint32_t, int64_t 等整型, 在需要确保整型大小时可以使用它们代替 short, unsigned long long 等. 
在 C 整型中, 只使用 int. 在合适的情况下, 推荐使用标准类型如 size_t 和 ptrdiff_t.

### nullptr 和 NULL
整数用 0, 实数用 0.0, 指针用 nullptr 或 NULL, 字符 (串) 用 '\0'.

整数用 0, 实数用 0.0, 这一点是毫无争议的.

对于指针 (地址值), 到底是用 0, NULL 还是 nullptr. C++11 项目用 nullptr; C++03 项目则用 NULL, 毕竟它看起来像指针。实际上，一些 C++ 编译器对 NULL 的定义比较特殊，可以输出有用的警告，特别是 sizeof(NULL) 就和 sizeof(0) 不一样。

字符 (串) 用 '\0', 不仅类型正确而且可读性好.

### sizeof
尽可能用 sizeof(varname) 代替 sizeof(type).

使用 sizeof(varname) 是因为当代码中变量类型改变时会自动更新. 您或许会用 sizeof(type) 处理不涉及任何变量的代码，比如处理来自外部或内部的数据格式，这时用变量就不合适了。

```cpp
Struct data;
Struct data; 
memset(&data, 0, sizeof(data));
```

Warning
``` cpp
memset(&data, 0, sizeof(Struct));
if (raw_size < sizeof(int)) {
    LOG(ERROR) << "compressed record not big enough for count: " << raw_size;
    return false;
}
```

### auto 类型
用 auto 绕过烦琐的类型名，只要可读性好就继续用，别用在局部变量之外的地方。
例如涉及模板或命名空间的时候
```cpp
sparse_hash_map<string, int>::iterator iter = m.find(val);
//变为
auto iter = m.find(val);
```
注意: auto 只能用在局部变量里用。别用在文件作用域变量，命名空间作用域变量和类数据成员里。永远别列表初始化 auto 变量。

### 列表初始化
```cpp
struct Point { int x; int y; };
Point p = {1, 2};

// Vector 接收了一个初始化列表。
vector<string> v{"foo", "bar"};

// 不考虑细节上的微妙差别，大致上相同。
// 您可以任选其一。
vector<string> v = {"foo", "bar"};

// 可以配合 new 一起用。
auto p = new vector<string>{"foo", "bar"};

// map 接收了一些 pair, 列表初始化大显神威。
map<int, string> m = {{1, "one"}, {2, "2"}};

// 初始化列表也可以用在返回类型上的隐式转换。
vector<int> test_function() { return {1, 2, 3}; }

// 初始化列表可迭代。
for (int i : {-1, -2, -3}) {}

// 在函数调用里用列表初始化。
void TestFunction2(vector<int> v) {}
TestFunction2({1, 2, 3});

//构造函数参数 和赋值运算符
class MyType {
public:
  // std::initializer_list 专门接收 init 列表。
  // 得以值传递。
  MyType(std::initializer_list<int> init_list) {
    for (int i : init_list) append(i);
  }
  MyType& operator=(std::initializer_list<int> init_list) {
    clear();
    for (int i : init_list) append(i);
  }
};
MyType m{2, 3, 5, 7};

//常量列表初始化
double d{1.23};
// MyOtherType 没有 std::initializer_list 构造函数，
// 直接上接收常规类型的构造函数。
class MyOtherType {
  public:
  explicit MyOtherType(string);
  MyOtherType(int, string);
};
MyOtherType m = {1, "b"};
// 不过如果构造函数是显式的（explict），您就不能用 `= {}` 了。
MyOtherType m{"b"};

```

### Lambda 表达式
适当使用 lambda 表达式。别用默认 lambda 捕获，所有捕获都要显式写出来。
禁用默认捕获，捕获都要显式写出来。
例如，不要用 
```cpp
[=](int x) {return x + n;}
```
应该写成
```cpp
[n](int x) {return x + n;}
```
才对，这样读者也好一眼看出 n 是被捕获的值。

### 模板编程
不要使用复杂的模板编程
模板编程最好只用在少量的基础组件, 基础数据结构上, 

## 类

不要在构造函数中调用虚函数,这类调用是不会重定向到子类的虚函数实现
如果你的类型需要, 就让它们支持拷贝 / 移动. 否则, 就把隐式产生的拷贝和移动函数禁用.
```cpp
// MyClass is neither copyable nor movable.
MyClass(const MyClass&) = delete;
MyClass& operator=(const MyClass&) = delete;
```
### 结构体 VS. 类
仅当只有数据成员时使用 struct, 其它一概使用 class.

### 多重继承
真正需要用到多重实现继承的情况少之又少. 只在以下情况我们才允许多重继承: 最多只有一个基类是非抽象类; 其它基类都是以 Interface 为后缀的 纯接口类.

当一个类满足以下要求时, 称之为纯接口:

1. 只有纯虚函数 (“=0”) 和静态函数 (除了下文提到的析构函数).
2. 没有非静态数据成员.
3. 没有定义任何构造函数. 如果有, 也不能带有参数, 并且必须为 protected.
4. 如果它是一个子类, 也只能从满足上述条件并以 Interface 为后缀的类继承.

### 运算符重载
除少数特定环境外, 不要重载运算符. 也不要创建用户定义字面量.

##  作用域
### 命名空间
像之前的几个例子中一样，在命名空间的最后注释出命名空间的名字。

用命名空间把文件包含, gflags 的声明/定义, 以及类的前置声明以外的整个源文件封装起来, 以区别于其它命名空间:
```cpp
// .h 文件
namespace mynamespace {

// 所有声明都置于命名空间中
// 注意不要使用缩进
class MyClass {
public:
  ...
  void Foo();
};

} // namespace mynamespace
```
```cpp
// .cc 文件
namespace mynamespace {

// 函数定义都置于命名空间中
void MyClass::Foo() {
  ...
}

} // namespace mynamespace
```
不应该使用 using 指示 引入整个命名空间的标识符号。
```cpp
//禁止 —— 污染命名空间
using namnespace stl;
```

### 匿名命名空间和静态变量
在 .cc 文件中定义一个不需要被外部引用的变量时，可以将它们放在匿名命名空间或声明为 static 。但是不要在 .h 文件中这么做。
所有置于匿名命名空间的声明都具有内部链接性，函数和变量可以经由声明为 static 拥有内部链接性，这意味着你在这个文件中声明的这些标识符都不能在另一个文件中被访问。即使两个文件声明了完全一样名字的标识符，它们所指向的实体实际上是完全不同的。
```cpp
//匿名命名空间
namespace {
...
}  // namespace
```
### 非成员函数、静态成员函数和全局函数
使用静态成员函数或命名空间内的非成员函数, 尽量不要用裸的全局函数. 将一系列函数直接置于命名空间中，不要用类的静态方法模拟出命名空间的效果，类的静态方法应当和类的实例或静态数据紧密相关.











