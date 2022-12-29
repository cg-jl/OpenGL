#include <memory>

template <typename T> class once_array {
  std::unique_ptr<T[]> m_ptr;
  size_t const m_size;

public:
  once_array(size_t const sz) : m_ptr(std::make_unique<T[]>(sz)), m_size(sz) {}

  size_t size() const noexcept { return m_size; }

  T const &operator[](size_t i) const { return m_ptr[i]; }
  T &operator[](size_t i) { return m_ptr[i]; }
};
