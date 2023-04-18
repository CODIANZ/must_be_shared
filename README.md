# must_be_shared

`must_be_shared` is a class that forces classes derived from `std::enable_shared_from_this` to be managed by `std::shared_ptr`.


## sample

```cpp
class FOO : public must_be_shared<FOO> {
private:
  int foo_value_;
public:
  FOO(int foo_value, const magic_t& _):
    must_be_shared<FOO>(_),
    foo_value_(foo_value)
    {}
  void foo() {
    auto instance = shared_from_this();
    std::cout << instance->foo_value_ << std::endl;
  }
};

class BAR : public FOO {
private:
  int bar_value_;
public:
  BAR(int bar_value, int foo_value, const FOO::magic_t& _):
    FOO(foo_value, _),
    bar_value_(bar_value)
     {}
  void bar() {
    // compiler cannot detect that it was generated with `BAR`.
    auto instance = shared_from_this_as<BAR>();
    std::cout << instance->bar_value_ << std::endl;
  }
};

int main() {
  // basic
  auto foo = FOO::create(1);
  foo->foo();

  // derived class
  auto bar = BAR::create<BAR>(10, 20);
  bar->foo();
  bar->bar();

  // If you want to manage with a base class
  auto bar_as_foo = BAR::create<BAR, FOO>(10, 20);
  bar_as_foo->foo();
  // bar_as_foo->bar(); // <-- compile error
  bar_as_foo->shared_from_this_as<BAR>()->bar();
}
```