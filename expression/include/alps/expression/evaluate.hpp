#ifndef ALPS_EXPRESSION_EVALUATE_HPP
#define ALPS_EXPRESSION_EVALUATE_HPP

#include "expression_fwd.hpp"
#include "expression.hpp"
#include "evaluator.hpp"
#include "evaluate_helper.hpp"

namespace alps {

template<class T>
inline bool can_evaluate(const expression::evaluatable<T>& ex, const expression::evaluator<T>& ev, bool isarg=false)
{
  return ex.can_evaluate(ev,isarg);
}

template<class T>
inline bool can_evaluate(const std::string& v, const expression::evaluator<T>& p, bool isarg=false)
{
  return expression::expression<T>(v).can_evaluate(p,isarg);
}

// inline bool can_evaluate(const std::string& v, const Parameters& p=Parameters())
// {
//   return can_evaluate(v, expression::ParameterEvaluator<>(p));
// }

// template<class U>
// inline bool can_evaluate(const std::string& v, const Parameters& p, const U&)
// {
//   return can_evaluate(v, expression::ParameterEvaluator<U>(p));
// }

// inline bool can_evaluate(const StringValue& v, const Parameters& p=Parameters())
// {
//   return can_evaluate(static_cast<std::string>(v), p);
// }

// template<class U>
// inline bool can_evaluate(const StringValue& v, const Parameters& p, const U&)
// {
//   return can_evaluate(static_cast<std::string>(v), p, U());
// }

template<class U, class T>
inline U evaluate(const expression::expression<T>& ex, const expression::evaluator<T>& ev = expression::evaluator<T>(), bool isarg=false)
{
  return expression::evaluate_helper<U>::value(ex, ev, isarg);
}

template<class U, class T>
inline U evaluate(const expression::term<T>& ex, const expression::evaluator<T>& ev = expression::evaluator<T>(), bool isarg=false)
{
  return expression::evaluate_helper<U>::value(ex, ev,isarg);
}

template<class U, class T>
inline U evaluate(const char* v, const expression::evaluator<T>& ev, bool isarg=false)
{
  return expression::evaluate_helper<U>::value(expression::expression<T>(std::string(v)), ev,isarg);
}

template<class U, class T>
inline U evaluate(const std::string& v, const expression::evaluator<T>& ev, bool isarg=false)
{
  return expression::evaluate_helper<U>::value(expression::expression<T>(v), ev,isarg);
}

// template<class U, class T>
// inline U evaluate(const StringValue& v, const expression::evaluator<T>& ev, bool isarg=false)
// {
//   return evaluate<U>(static_cast<std::string>(v), ev,isarg);
// }

template<class U>
inline U evaluate(const char* v)
{
  return evaluate<U, U>(v, expression::evaluator<
    typename expression::evaluate_helper<U>::value_type>());
}
inline double evaluate(const char* v) {
  return evaluate<double, double>(v, expression::evaluator<
    expression::evaluate_helper<double>::value_type>());
}

template<class U>
inline U evaluate(const std::string& v)
{
  return evaluate<U, U>(v, expression::evaluator<
    typename expression::evaluate_helper<U>::value_type>());
}
inline double evaluate(const std::string& v) {
  return evaluate<double, double>(v, expression::evaluator<
    expression::evaluate_helper<double>::value_type>());
}

// template<class U>
// inline U evaluate(const StringValue& v)
// {
//   return evaluate<U, U>(v, expression::evaluator<
//     typename expression::evaluate_helper<U>::value_type>());
// }
// inline double evaluate(const StringValue& v) {
//   return evaluate<double, double>(v, expression::evaluator<
//     expression::evaluate_helper<double>::value_type>());
// }

// template<class U>
// inline U evaluate(const char* v, const Parameters& p)
// {
//   return evaluate<U, typename expression::evaluate_helper<U>::value_type>(v,
//     expression::Parameterevaluator<
//     typename expression::evaluate_helper<U>::value_type>(p));
// }
// inline double evaluate(const char* v, const Parameters& p)
// {
//   return evaluate<double, expression::evaluate_helper<double>::value_type>(v,
//     expression::ParameterEvaluator<
//     expression::evaluate_helper<double>::value_type>(p));
// }

// template<class U>
// inline U evaluate(const std::string& v, const Parameters& p)
// {
//   return evaluate<U,typename expression::evaluate_helper<U>::value_type>(v,
//     expression::ParameterEvaluator<
//     typename expression::evaluate_helper<U>::value_type>(p));
// }
// inline double evaluate(const std::string& v, const Parameters& p)
// {
//   return evaluate<double, expression::evaluate_helper<double>::value_type>(v,
//     expression::ParameterEvaluator<
//     expression::evaluate_helper<double>::value_type>(p));
// }

// template<class U>
// inline U evaluate(const StringValue& v, const Parameters& p)
// {
//   return evaluate<U, typename expression::evaluate_helper<U>::value_type>(v,
//     expression::ParameterEvaluator<
//     typename expression::evaluate_helper<U>::value_type>(p));
// }
// inline double evaluate(const StringValue& v, const Parameters& p)
// {
//   return evaluate<double, expression::evaluate_helper<double>::value_type>(v,
//     expression::ParameterEvaluator<
//     expression::evaluate_helper<double>::value_type>(p));
// }

template<class T>
void simplify(T) {}

template<class T>
void simplify(expression::expression<T>& x) { x.simplify();}


// StringValue simplify_value(StringValue const& val, Parameters const& parms, bool eval_random=false);

// bool same_values(StringValue const& x, StringValue const& y, double eps=1e-15);

} // end namespace alps

#endif // ! ALPS_EXPRESSION_H
