#ifndef ALPS_EXPRESSION_NUMBER_HPP
#define ALPS_EXPRESSION_NUMBER_HPP

#include <alps/expression/expression_fwd.hpp>
#include <alps/expression/evaluate_helper.hpp>
#include <alps/numeric/real.hpp>
#include <boost/call_traits.hpp>

namespace alps {
namespace expression {

template<class T>
class Number : public Evaluatable<T> {
public:
  typedef T value_type;
  typedef typename alps::numeric::real_type<T>::type real_type;

  Number(typename boost::call_traits<value_type>::param_type x) : val_(x) {}
  value_type value(const Evaluator<T>& =Evaluator<T>(), bool=false) const;
  bool can_evaluate(const Evaluator<T>& =Evaluator<T>(), bool=false) const { return true; }
  void output(std::ostream&) const;
  Evaluatable<T>* clone() const { return new Number<T>(*this); }
private:
  value_type val_;
};

template<class T>
typename Number<T>::value_type Number<T>::value(const Evaluator<T>&, bool) const
{
  return val_;
}

template<class T>
void Number<T>::output(std::ostream& os) const
{
  os << "Number[";
  if (evaluate_helper<T>::imag(val_) == 0)
    os << evaluate_helper<T>::real(val_);
  else
    os << val_;
  os << "]";
}

} // end namespace expression
} // end namespace alps

#endif // ! ALPS_EXPRESSION_IMPL_H
