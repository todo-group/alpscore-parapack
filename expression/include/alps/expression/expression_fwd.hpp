#ifndef ALPS_EXPRESSION_EXPRESSION_FWD_HPP
#define ALPS_EXPRESSION_EXPRESSION_FWD_HPP

#include <complex>
#include <boost/mpl/bool.hpp>
#include "is_symbolic.hpp"

namespace alps {
namespace expression {

template<class T = std::complex<double> > class Expression;
template<class T = std::complex<double> > class Term;
template<class T = std::complex<double> > class Factor;
template<class T = std::complex<double> > class Evaluator;
template<class T = std::complex<double> > class ParameterEvaluator;
template <class T> class Block;
template <class T> class Function;
template <class T> class Number;
template <class T> class Symbol;

}
  
template <class T>
struct is_symbolic<expression::Expression<T> > : public boost::mpl::true_ {};

template <class T>
struct is_symbolic<expression::Term<T> > : public boost::mpl::true_ {};

template <class T>
struct is_symbolic<expression::Factor<T> > : public boost::mpl::true_ {};

template <class T>
struct is_symbolic<expression::Block<T> > : public boost::mpl::true_ {};

template <class T>
struct is_symbolic<expression::Function<T> > : public boost::mpl::true_ {};

template <class T>
struct is_symbolic<expression::Symbol<T> > : public boost::mpl::true_ {};

}

#endif
