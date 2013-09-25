#pragma once

/* #undef ENTITYX_HAVE_BOOST_SHARED_PTR */
/* #undef ENTITYX_HAVE_STD_SHARED_PTR */
/* #undef ENTITYX_USE_STD_SHARED_PTR */
#define ENTITYX_MAX_COMPONENTS 64
/* #undef ENTITYX_HAVE_BOOST_PYTHON */
/* #undef ENTITYX_INSTALLED_PYTHON_PACKAGE_DIR */
/* #undef ENTITYX_NEED_GET_POINTER_SHARED_PTR_SPECIALIZATION */

#include <cstdint>
#include "config.h"

namespace entityx {

static const uint64_t MAX_COMPONENTS = ENTITYX_MAX_COMPONENTS;

}  // namespace entityx


// Which shared_ptr implementation should we use?
#if 1 | (ENTITYX_HAVE_STD_SHARED_PTR && ENTITYX_USE_STD_SHARED_PTR)

#include <memory>

namespace entityx {

template <typename T, typename U>
std::shared_ptr<U> static_pointer_cast(const std::shared_ptr<T> &ptr) {
  return std::static_pointer_cast<U>(ptr);
}

}  // namespace entityx

#elif ENTITYX_HAVE_BOOST_SHARED_PTR

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>


namespace entityx {

template <typename T>
using ptr = boost::shared_ptr<T>;
template <typename T>
using weak_ptr = boost::weak_ptr<T>;
template <typename T, typename U>
ptr<U> static_pointer_cast(const ptr<T> &ptr) {
  return boost::static_pointer_cast<U>(ptr);
}
template <typename T>
using enable_shared_from_this = boost::enable_shared_from_this<T>;

}  // namespace entityx

#else

#pragma message (  "Don't have a boost or std shared_ptr implementation to use" )

#endif


namespace entityx {

template <typename T>
bool operator == (const std::weak_ptr<T> &a, const std::weak_ptr<T> &b) {
  return a.lock() == b.lock();
}

template <typename T>
int use_count(const std::shared_ptr<T> &ptr) {
  return ptr.use_count();
}

}  // namespace entityx
