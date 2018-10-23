#ifndef ALPS_EXPRESSION_NUMBER_HPP
#define ALPS_EXPRESSION_NUMBER_HPP

#include <alps/expression/expression_fwd.hpp>
#include <alps/expression/evaluate_helper.hpp>
#include <alps/numeric/real.hpp>
#include <boost/call_traits.hpp>

namespace alps {
namespace expression {

template<class T>
class number : public evaluatable<T> {
public:
  typedef T value_type;
  typedef typename alps::numeric::real_type<T>::type real_type;

  number(typename boost::call_traits<value_type>::param_type x) : val_(x) {}
  value_type value(const evaluator<T>& =evaluator<T>(), bool=false) const;
  bool can_evaluate(const evaluator<T>& =evaluator<T>(), bool=false) const { return true; }
  void output(std::ostream&) const;
  evaluatable<T>* clone() const { return new number<T>(*this); }
private:
  value_type val_;
};

template<class T>
typename number<T>::value_type number<T>::value(const evaluator<T>&, bool) const
{
  return val_;
}

template<class T>
void number<T>::output(std::ostream& os) const
{
  if (evaluate_helper<T>::imag(val_) == 0)
    os << evaluate_helper<T>::real(val_);
  else
    os << val_;
}

} // end namespace expression
} // end namespace alps

#endif // ! ALPS_EXPRESSION_IMPL_H
