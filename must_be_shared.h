#if !defined(__h_must_be_shared__)
#define __h_must_be_shared__

#include <memory>

/**
 * 
 * Usage:
 * 
 * class FOO : public must_be_shared<FOO> {
 * private:
 *   int foo_value_;
 * public:
 *   FOO(int foo_value, const magic_t& _):
 *     must_be_shared<FOO>(_),
 *     foo_value_(foo_value)
 *     {}
 *   void foo() {
 *     auto instance = shared_from_this();
 *     std::cout << instance->foo_value_ << std::endl;
 *   }
 * };
 * 
 * 
 * class BAR : public FOO {
 * private:
 *   int bar_value_;
 * public:
 *   BAR(int bar_value, int foo_value, const FOO::magic_t& _):
 *     FOO(foo_value, _),
 *     bar_value_(bar_value)
 *      {}
 *   void bar() {
 *     auto instance = shared_from_this_as<BAR>(); // `shared_from_this`
 *     std::cout << instance->bar_value_ << std::endl;
 *   }
 * };
 * 
 * 
 * int main() {
 *   auto bar = BAR::create<BAR>(10, 20);
 *  bar->foo();
 *  bar->bar();
 *
 *  auto foo = BAR::create<BAR, FOO>(10, 20); // If you want to manage with a base class
 *  foo->foo();
 *  // foo->bar(); // <-- compile error
 *  foo->shared_from_this_as<BAR>()->bar();
 * }
 * 
 **/

template <typename T> class must_be_shared :
  public std::enable_shared_from_this<T>
{
private:
  struct _magic_seed {};
  struct _magic { _magic(const _magic_seed&) noexcept {} };

protected:
  using magic_t = typename must_be_shared<T>::_magic;
  must_be_shared(const magic_t&) noexcept {}

public:
  virtual ~must_be_shared() = default;

  template <typename ...ARGS>
  static auto create(ARGS&&...args) {
    return std::make_shared<T>(std::forward<ARGS>(args)..., _magic{_magic_seed{}});
  }

  template <typename Derived, typename OutType = Derived, typename ...ARGS>
  static auto create(ARGS&&...args) {
    return std::static_pointer_cast<OutType>(
      std::make_shared<Derived>(std::forward<ARGS>(args)..., _magic{_magic_seed{}})
    );
  }

  template <typename X> auto shared_from_this_as() noexcept {
    return std::dynamic_pointer_cast<X>(must_be_shared<T>::shared_from_this());
  }
};

#endif /* __h_must_be_shared__ */