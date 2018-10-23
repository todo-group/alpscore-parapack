#ifndef ALPS_EXPRESSION_FUNCTION_HPP
#define ALPS_EXPRESSION_FUNCTION_HPP

#include <alps/expression/expression_fwd.hpp>

namespace alps {
namespace expression {


template<class T>
class function : public evaluatable<T> {
public:
  typedef T value_type;

  function(std::istream&, const std::string&);
  function(const std::string& n, const expression<T>& e) : name_(n), args_(1,e) {}
  function(const std::string& n, const std::vector<expression<T> >& e) : name_(n), args_(e) {}
  value_type value(const evaluator<T>& =evaluator<T>(), bool=false) const;
  bool can_evaluate(const evaluator<T>& =evaluator<T>(), bool=false) const;
  void output(std::ostream&) const;
  evaluatable<T>* clone() const { return new function<T>(*this); }
  boost::shared_ptr<evaluatable<T> > flatten_one();
  evaluatable<T>* partial_evaluate_replace(const evaluator<T>& =evaluator<T>(), bool=false);
  bool depends_on(const std::string& s) const;
private:
 std::string name_;
 std::vector<expression<T> > args_;
};

//
// implementation of function<T>
//

template<class T>
function<T>::function(std::istream& in,const std::string& name)
  :  name_(name), args_()
{
  char c;
  in >> c;
  if (c!=')') {
    in.putback(c);
    do {
      args_.push_back(expression<T>(in));
      in >> c;
    } while (c==',');
    if (c!=')')
      boost::throw_exception(std::runtime_error(std::string("received ") + c + " instead of ) at end of function argument list"));
  }
}

template<class T>
bool function<T>::depends_on(const std::string& s) const {
  if (name_==s) return true;
  for (typename std::vector<expression<T> >::const_iterator it=args_.begin();it != args_.end();++it)
    if (it->depends_on(s))
      return true;
  return false;
}

template<class T>
boost::shared_ptr<evaluatable<T> > function<T>::flatten_one()
{
  for (typename std::vector<expression<T> >::iterator it=args_.begin();it != args_.end();++it)
    it->flatten();
  return boost::shared_ptr<expression<T> >();
}

template<class T>
evaluatable<T>* function<T>::partial_evaluate_replace(const evaluator<T>& p, bool isarg)
{
  p.partial_evaluate_expressions(args_,true);
  return new block<T>(p.partial_evaluate_function(name_,args_,isarg));
}

template<class T>
typename function<T>::value_type function<T>::value(const evaluator<T>& p, bool isarg) const
{
  value_type val=p.evaluate_function(name_,args_,isarg);
  return val;
}

template<class T>
bool function<T>::can_evaluate(const evaluator<T>& p, bool isarg) const
{
  return p.can_evaluate_function(name_,args_,isarg);
}

template<class T>
void function<T>::output(std::ostream& os) const
{
  os << name_ << "(";
  for (std::size_t i = 0; i < args_.size(); ++i) {
    os << args_[i];
    if (i != args_.size() - 1) os << ", ";
  }
  os << ")";
}

} // end namespace expression
} // end namespace alps

#endif // ! ALPS_EXPRESSION_IMPL_H
