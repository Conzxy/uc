#ifndef FILE_UTIL_H__
#define FILE_UTIL_H__

#include <stdio.h>
#include <assert.h>

#include <memory>
#include <utility>

namespace uc {
namespace detail {

inline void DeferFcloseCallable(FILE *fp) {
  assert(fp);
  fclose(fp);
}

} // detail

template<typename T, typename D>
struct Defer {
  Defer(T *p, D deleter)
    : ptr_(p, std::move(deleter))
  {
  }
  
  ~Defer() = default; 
 private:
  std::unique_ptr<T, D> ptr_;
};

struct DeferFclose {
  explicit DeferFclose(FILE *fp)
    : defer_(fp, &detail::DeferFcloseCallable)
  {
  }

 private:
  Defer<FILE, void(*)(FILE*)> defer_;
};

}

#endif // FILE_UTIL_H__
