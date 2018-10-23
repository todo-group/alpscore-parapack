#ifndef ALPS_EXPRESSION_EXPRESSION_FWD_HPP
#define ALPS_EXPRESSION_EXPRESSION_FWD_HPP

#include <complex>
#include <boost/mpl/bool.hpp>
#include "is_symbolic.hpp"

namespace alps {
namespace expression {

template<class T = std::complex<double> > class expression;
template<class T = std::complex<double> > class term;
template<class T = std::complex<double> > class factor;
template<class T = std::complex<double> > class evaluator;
template<class T = std::complex<double> > class ParameterEvaluator;
template <class T> class block;
template <class T> class function;
template <class T> class number;
template <class T> class symbol;

}
  
template <class T>
struct is_symbolic<expression::expression<T> > : public boost::mpl::true_ {};

template <class T>
struct is_symbolic<expression::term<T> > : public boost::mpl::true_ {};

template <class T>
struct is_symbolic<expression::factor<T> > : public boost::mpl::true_ {};

template <class T>
struct is_symbolic<expression::block<T> > : public boost::mpl::true_ {};

template <class T>
struct is_symbolic<expression::function<T> > : public boost::mpl::true_ {};

template <class T>
struct is_symbolic<expression::symbol<T> > : public boost::mpl::true_ {};

}

#endif
