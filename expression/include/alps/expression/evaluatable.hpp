/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_EXPRESSION_EVALUATABLE_HPP
#define ALPS_EXPRESSION_EVALUATABLE_HPP

#include <boost/shared_ptr.hpp>
#include "expression_fwd.hpp"

namespace alps {
namespace expression {

template<class T>
class Evaluatable {
public:
  typedef T value_type;

  Evaluatable() {}
  virtual ~Evaluatable() {}
  virtual value_type value(const Evaluator<T>& =Evaluator<T>(), bool=false) const = 0;
  virtual bool can_evaluate(const Evaluator<T>& =Evaluator<T>(), bool=false) const = 0;
  virtual void output(std::ostream&) const = 0;
  virtual Evaluatable* clone() const = 0;
  virtual boost::shared_ptr<Evaluatable> flatten_one() { return boost::shared_ptr<Evaluatable>(); }
  virtual Evaluatable* partial_evaluate_replace(const Evaluator<T>& =Evaluator<T>(),bool=false) { return this; }
  virtual bool is_single_term() const { return false; }
  virtual Term<T> term() const;
  virtual bool depends_on(const std::string&) const { return false; }
};

template<class T>
inline Term<T> Evaluatable<T>::term() const { return Term<T>(); }

template<class T>
inline std::ostream& operator<<(std::ostream& os, const alps::expression::Evaluatable<T>& e) {
  e.output(os);
  return os;
}

} // end namespace expression
} // end namespace alps

#endif
