#ifndef ALPS_EXPRESSION_IS_SYMBOLIC_HPP
#define ALPS_EXPRESSION_IS_SYMBOLIC_HPP

#include <boost/mpl/bool.hpp>

namespace alps {
  
template <class T>
struct is_symbolic : public boost::mpl::false_ {};

}

#endif
