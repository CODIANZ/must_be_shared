#if !defined(__h_must_be_shared__)
#define __h_must_be_shared__

#include <memory>

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
    return std::make_shared<T>(_magic{_magic_seed{}}, std::forward<ARGS>(args)...);
  }

  template <typename Derived, typename OutType = Derived, typename ...ARGS>
  static auto create(ARGS&&...args) {
    return std::static_pointer_cast<OutType>(
      std::make_shared<Derived>(_magic{_magic_seed{}}, std::forward<ARGS>(args)...)
    );
  }

  template <typename X> auto shared_from_this_as() noexcept {
    return std::dynamic_pointer_cast<X>(must_be_shared<T>::shared_from_this());
  }
};

#endif /* __h_must_be_shared__ */