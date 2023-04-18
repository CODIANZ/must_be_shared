# must_be_shared

`must_be_shared` is a class that forces classes derived from `std::enable_shared_from_this` to be managed by `std::shared_ptr`.

## usage

1. Replace `std::enable_shared_from_this` with `must_be_shared`.
1. Place the constructor in `public`.
1. Add `const magic_t& _` to the first argument of the constructor.
1. Add `must_be_shared<XX>(_)` in constructor initializer.

It requires that the constructor be placed `public`, but it is not possible to create it locally, as there is no way to create a `magic_t` as `public`.


## sample

```cpp
#include "must_be_shared.h"

class FOO : public must_be_shared<FOO> {
private:
  int foo_value_;
public:
  FOO(const magic_t& _, int foo_value):
    must_be_shared<FOO>(_),
    foo_value_(foo_value)
    {}
  void foo() {
    auto instance = shared_from_this();
    std::cout << "foo -> " << instance->foo_value_ << std::endl;
  }
};

class BAR : public FOO {
private:
  int bar_value_;
public:
  BAR(const magic_t& _, int bar_value = 100, int foo_value = 200):
    FOO(_, foo_value),
    bar_value_(bar_value)
     {}
  void bar() {
    auto instance = shared_from_this_as<BAR>();
    std::cout << "bar -> " << instance->bar_value_ << std::endl;
  }
};

int main() {
  // compile error
  // auto foo = FOO(FOO::magic_t{FOO::_magic_seed{}}, 1);

  // basic
  auto foo = FOO::create(1);
  foo->foo();

  // derived class
  auto bar = BAR::create<BAR>(10, 20);
  bar->foo();
  bar->bar();

  // You can use default arguments
  auto bar_default = BAR::create<BAR>();
  bar_default->foo();
  bar_default->bar();

  // If you want to manage with a base class
  auto bar_as_foo = BAR::create<BAR, FOO>(10, 20);
  bar_as_foo->foo();
  // bar_as_foo->bar(); // <-- compile error
  bar_as_foo->shared_from_this_as<BAR>()->bar();
}

// foo -> 1
// foo -> 20
// bar -> 10
// foo -> 200
// bar -> 100
// foo -> 20
// bar -> 10
```