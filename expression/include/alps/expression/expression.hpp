#ifndef ALPS_EXPRESSION_HPP
#define ALPS_EXPRESSION_HPP

#include <boost/call_traits.hpp>
#include <alps/numeric/is_zero.hpp>
#include "expression_fwd.hpp"
#include "term.hpp"

namespace alps {
namespace expression {

template<class T>
class expression : public evaluatable<T> {
public:
  typedef T value_type;
  typedef term<T> term_type;
  typedef typename std::vector<term<T> >::const_iterator term_iterator;

  expression() {}
  expression(const std::string& str) { parse(str); }
  expression(std::istream& in) { parse(in); }
  expression(typename boost::call_traits<value_type>::param_type val)
    : terms_(1,term<T>(val)) {}
#ifndef BOOST_NO_SFINAE
  template<class U>
  expression(U val, typename boost::enable_if<boost::is_arithmetic<U> >::type* = 0) : terms_(1,term<T>(value_type(val))) {}
#else
  expression(int val) : terms_(1,term<T>(value_type(val))) {}
#endif
  expression(const evaluatable<T>& e) : terms_(1,term<T>(e)) {}
  expression(const term<T>& e) : terms_(1,e) {}
  virtual ~expression() {}

  value_type value(const evaluator<T>& = evaluator<T>(), bool=false) const;
  // value_type value(const Parameters& p) const {
  //   return value(ParameterEvaluator<T>(p));
  // }

  bool can_evaluate(const evaluator<T>& = evaluator<T>(), bool=false) const;
  // bool can_evaluate(const Parameters& p) const
  // {
  //   return can_evaluate(ParameterEvaluator<T>(p));
  // }
  void partial_evaluate(const evaluator<T>& =evaluator<T>(), bool=false);
  // void partial_evaluate(const Parameters& p) {
  //   partial_evaluate(ParameterEvaluator<T>(p));
  // }

  void sort();
  void output(std::ostream& os) const;

  evaluatable<T>* clone() const { return new expression<T>(*this); }

  std::pair<term_iterator,term_iterator> terms() const
  {
    return std::make_pair(terms_.begin(),terms_.end());
  }

  void flatten(); // multiply out all blocks

  boost::shared_ptr<expression> flatten_one_expression();

  const expression& operator+=(const term<T>& term)
  {
    terms_.push_back(term);
    partial_evaluate(evaluator<T>());
    return *this;
  }

  const expression& operator-=(term<T> term)
  {
    term.negate();
    terms_.push_back(term);
    partial_evaluate(evaluator<T>());
    return *this;
  }

  const expression& operator+=(const expression& e)
  {
    std::copy(e.terms_.begin(),e.terms_.end(),std::back_inserter(terms_));
    partial_evaluate(evaluator<T>());
    return *this;
  }

  const expression& operator-=(expression const& e)
  {
    return operator+=(-e);
  }

  const expression& operator*=(const expression<T>& e)
  {
    term<T> newt(factor<T>(block<T>(*this)));
    newt *= factor<T>(block<T>(e));
    terms_.clear();
    newt.remove_superfluous_parentheses();
    terms_.push_back(newt);
    partial_evaluate(evaluator<T>());
    return *this;
  }
  
  
  void simplify();
  void remove_superfluous_parentheses();

  bool has_no_term()    const { return terms_.empty(); }
  bool is_single_term() const { return terms_.size() == 1; }
  term<T> get_term() const;
  term<T> zeroth_term() const { return terms_[0]; }
  bool depends_on(const std::string&) const;

  expression<T> expression_dependent_on(const std::string&) const;
  expression<T> expression_dependent_only_on(const std::string&) const;

  void parse(const std::string& str);
  bool parse(std::istream& is);

  expression operator-() const { expression e(*this); e.negate(); return e;}
  const expression& negate() 
  {
    for (typename std::vector<term<T> >::iterator it=terms_.begin();it!=terms_.end();++it)
      it->negate();
    return *this;
  } 
private:
  std::vector<term<T> > terms_;
};

//
// implementation of expression<T>
//

template<class T>
bool expression<T>::depends_on(const std::string& s) const {
  for(term_iterator it=terms().first; it!=terms().second; ++it)
    if (it->depends_on(s))
      return true;
  return false;
}

template<class T>
void expression<T>::remove_superfluous_parentheses()
{
  for (typename std::vector<term<T> >::iterator it=terms_.begin();
       it!=terms_.end(); ++it)
    it->remove_superfluous_parentheses();
}

template<class T>
void expression<T>::simplify()
{
  partial_evaluate(evaluator<T>());
  for (typename std::vector<term<T> >::iterator it=terms_.begin();
       it!=terms_.end(); ++it)
    it->simplify();
  sort();
  partial_evaluate(evaluator<T>());
}

template<class T>
term<T> expression<T>::get_term() const
{
  if (!is_single_term())
    boost::throw_exception(std::logic_error("Called get_term() for multi-term expression"));
  return terms_[0];
}

template<class T>
expression<T> expression<T>::expression_dependent_on(const std::string& str) const
{
  expression<T> e;
  for (typename std::vector<term<T> >::const_iterator it=terms_.begin();
       it!=terms_.end(); ++it)
    if (it->depends_on(str))
      e += (*it);
  return e;
} 

template<class T>
expression<T> expression<T>::expression_dependent_only_on(const std::string& str) const
{
  expression<T> e;
  for (typename std::vector<term<T> >::const_iterator it=terms_.begin();
       it!=terms_.end(); ++it)
    if (it->depends_only_on(str))
      e += (*it);
  return e;
}

template<class T>
void expression<T>::parse(const std::string& str)
{
  std::istringstream in(str);
  if (!parse(in))
    boost::throw_exception(std::runtime_error("Did not parse to end of string '" + str + "'"));
}

template<class T>
bool expression<T>::parse(std::istream& is)
{
  terms_.clear();
  bool negate=false;
  char c;
  is >> c;
  if (is.eof())
    return true;
  if (c=='-')
    negate=true;
  else if (c=='+')
    negate=false;
  else
    is.putback(c);
  terms_.push_back(term<T>(is,negate));
  while(true) {
    if(!(is >> c))
      return true;
    if (is.eof())
      return true;
    if (c=='-')
      negate=true;
    else if (c=='+')
      negate=false;
    else {
      is.putback(c);
      return false;
    }
    terms_.push_back(term<T>(is,negate));
  }
}

template <class T>
void expression<T>::sort()
{
  partial_evaluate(evaluator<T>());
  std::sort(terms_.begin(),terms_.end(),term_less<T>());

  typename std::vector<term<T> >::iterator prev,it;
  prev=terms_.begin();
  if (prev==terms_.end())
    return;
  it=prev;
  ++it;
  bool added=false;
  std::pair<T,term<T> > prev_term=prev->split();
  while (it !=terms_.end()) {
    std::pair<T,term<T> > current_term=it->split();
    
    if (prev_term.second==current_term.second) {
      prev_term.first += current_term.first;
      terms_.erase(it);
      added=true;
      *prev=term<T>(prev_term);
      it = prev;
      ++it;
    }
    else {
      if (added && numeric::is_zero(prev_term.first))
        terms_.erase(prev);
      else {
        prev=it;
        ++it;
      }
      prev_term=current_term;
    }
    added=false;
  }
  if (added && numeric::is_zero(prev_term.first))
    terms_.erase(prev);
}

template<class T>
typename expression<T>::value_type expression<T>::value(const evaluator<T>& p, bool isarg) const
{
  if (terms_.size()==0)
    return value_type(0.);
  value_type val=terms_[0].value(p);
  for (unsigned int i=1;i<terms_.size();++i)
    val += terms_[i].value(p,isarg);
  return val;
}

template<class T>
bool expression<T>::can_evaluate(const evaluator<T>& p, bool isarg) const
{
  if (terms_.size()==0)
    return true;
  bool can=true;

  for (unsigned int i=0;i<terms_.size();++i)
    can = can && terms_[i].can_evaluate(p,isarg);
  return can;
}

template<class T>
void expression<T>::partial_evaluate(const evaluator<T>& p, bool isarg)
{
  if (can_evaluate(p,isarg))
    (*this) = expression<T>(value(p,isarg));
  else {
    value_type val(0);
    for (unsigned int i=0; i<terms_.size(); ++i) {
      if (terms_[i].can_evaluate(p,isarg)) {
        val += terms_[i].value(p,isarg);
        terms_.erase(terms_.begin()+i);
        --i;
      } else {
        terms_[i].partial_evaluate(p,isarg);
      }
    }
    if (val != value_type(0.)) terms_.insert(terms_.begin(), term<T>(val));
  }
}

template<class T>
void expression<T>::output(std::ostream& os) const
{
  if (terms_.size()==0)
    os <<"0";
  else {
    terms_[0].output(os);
    for (unsigned int i=1;i<terms_.size();++i) {
      if(!terms_[i].is_negative())
        os << " + ";
      terms_[i].output(os);
    }
  }
}

template<class T>
void expression<T>::flatten()
{
  unsigned int i=0;
  while (i<terms_.size()) {
    boost::shared_ptr<term<T> > term = terms_[i].flatten_one_term();
    if (term)
      terms_.insert(terms_.begin()+i,*term);
    else
      ++i;
  }
}

template<class T>
boost::shared_ptr<expression<T> > expression<T>::flatten_one_expression()
{
  flatten();
  if (terms_.size()>1) {
    boost::shared_ptr<expression<T> > tm(new expression<T>());
    tm->terms_.push_back(terms_[0]);
    terms_.erase(terms_.begin());
    return tm;
  }
  else
    return boost::shared_ptr<expression<T> >();
}

} // end namespace expression
} // end namespace alps

#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE
namespace alps {
namespace expression {
#endif

template<class T>
inline alps::expression::expression<T> operator+(const alps::expression::expression<T>& ex1, const alps::expression::expression<T>& ex2)
{
  alps::expression::expression<T> ex(ex1);
  ex += ex2;
  return ex;
}

template<class T>
inline std::istream& operator>>(std::istream& is, alps::expression::expression<T>& e)
{
  std::string s;
  is >> s;
  e.parse(s);
  return is;
}

template<class T>
inline bool operator==(const alps::expression::expression<T>& ex1, const alps::expression::expression<T>& ex2)
{
  return (boost::lexical_cast<std::string>(ex1) ==
          boost::lexical_cast<std::string>(ex2));
}

template<class T>
inline bool operator==(const alps::expression::expression<T>& ex, const std::string& s)
{
  return boost::lexical_cast<std::string>(ex) == s;
}

template<class T>
inline bool operator==(const std::string& s, const alps::expression::expression<T>& ex)
{
  return ex == s;
}

template<class T>
inline bool operator!=(const alps::expression::expression<T>& ex1, const alps::expression::expression<T>& ex2)
{
  return (boost::lexical_cast<std::string>(ex1) !=
          boost::lexical_cast<std::string>(ex2));
}

template<class T>
inline bool operator!=(const alps::expression::expression<T>& ex, const std::string& s)
{
  return boost::lexical_cast<std::string>(ex) != s;
}

template<class T>
inline bool operator!=(const std::string& s, const alps::expression::expression<T>& ex)
{
  return ex != s;
}

template<class T>
inline bool operator<(const alps::expression::expression<T>& ex1, const alps::expression::expression<T>& ex2)
{
  return (boost::lexical_cast<std::string>(ex1) <
          boost::lexical_cast<std::string>(ex2));
}

template<class T>
inline bool operator<(const alps::expression::expression<T>& ex, const std::string& s)
{
  return boost::lexical_cast<std::string>(ex) < s;
}

template<class T>
inline bool operator<(const std::string& s, const alps::expression::expression<T>& ex)
{
  return s < boost::lexical_cast<std::string>(ex);
}

#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE
} // end namespace expression
} // end namespace alps
#endif

#endif // ! ALPS_EXPRESSION_HPP
