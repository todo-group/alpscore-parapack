#ifndef ALPS_EXPRESSION_PARSER_HPP
#define ALPS_EXPRESSION_PARSER_HPP

// #include <alps/cctype.h>
// #include <alps/parser/xmlattributes.h>
#include <map>
#include <string>

namespace alps {

namespace detail {

// \brief returns true if the character is a XML tag name
inline bool is_identifier_char(char c) 
{ 
  return std::isalnum(c) || c=='_' || c==':' || c=='#';
}

} // end namespace detail

// /// \brief a struct to store the contents of an XML tag
// struct XMLTag
// {
//   /// the name of the tag
//   std::string name;
//   /// the attributes
//   XMLAttributes attributes;
// /*/// \brief the type of tag
//   ///
//   /// The meaning of the values, and what is stored in the \c name member can be seen in the following table
//   /// \htmlonly
//   /// <TABLE BORDER=2>
//   /// <TR><TD><B>type</B></TD><TD><B>name</B></TD><TD><B>example</B></TD></TR>
//   /// <TR><TD><TT>OPENING</TT></TD><TD><TT>TAG</TT></TD><TD><TT>&lt;TAG&gt;</TT></TD></TR>
//   /// <TR><TD><TT>CLOSING</TT></TD><TD><TT>/TAG</TT></TD><TD><TT>&lt;/TAG&gt;</TT></TD></TR>
//   /// <TR><TD><TT>SINGLE</TT></TD><TD><TT>TAG</TT></TD><TD><TT>&lt;TAG/&gt;</TT></TD></TR>
//   /// <TR><TD><TT>COMMENT</TT></TD><TD><TT>!</TT></TD><TD><TT>&lt;!-- comment --!/&gt;</TT></TD></TR>
//   /// <TR><TD><TT>PROCESSING</TT></TD><TD><TT>?</TT></TD><TD><TT>&lt;? processing instruction ?/&gt;</TT></TD></TR>
//   /// </TABLE>
//   /// \endhtmlonly
//   /// \latexonly
//   /// \begin{tabulate}[|c|c|c|]
//   /// \hline
//   /// type & name & example \\ %
//   /// \hline
//   /// {\tt OPENING}    & {\tt TAG}  & {\tt <TAG>} \\  %
//   /// {\tt CLOSING}    & {\tt /TAG} & {\tt </TAG>} \\  %
//   /// {\tt SINGLE}     & {\tt TAG}  & {\tt <TAG/>} \\  %
//   /// {\tt COMMENT}    & {\tt !}    & {\tt <!-- comment --!/>} \\ %
//   /// {\tt PROCESSING} & {\tt !}    & {\tt <? processing instruction ?/>} \\ %
//   /// \hline
//   /// \end{tabulate}  
//   /// \endlatexonly
// */enum {OPENING, CLOSING, SINGLE, COMMENT, PROCESSING} type;
//   /// returns true if the tag is a comment
//   bool is_comment() { return type==COMMENT;}
//   /// returns true if the tag is a processing instruction
//   bool is_processing() { return type==PROCESSING;}
//   /// returns true if the tag is an opening or closing tag of an XML element
//   bool is_element() { return !is_comment() && !is_processing();}
// };


/// reads an XML tag or attribute name from a \c std::istream
std::string parse_identifier(std::istream& in);

/// \brief reads an ALPS parameter name from a \c std::istream
/// 
/// valid characters, in addition to those in an XML identifier are \c ', 
/// and additionally any arbitrary sequence of characters (including whitespace) surrounded by \c [ ... \ ] 
/// characters, such as in \c MEASURE[Staggered \c Magnetization^2] .
inline std::string parse_parameter_name(std::istream& in) {
  char c;
  in >> c;
  std::string name;
  while (in && !in.eof() && (detail::is_identifier_char(c) || c=='\''  || c=='[')) {
    name+=c;
        if (c=='[') 
          do {
            c=in.get();
                name+=c;
          } while (c!=']');
    c=in.get();
  }
  if (!in.eof())
    in.putback(c);
  return name;          
}

/// \brief reads until the next occurence of the character \a end or until the end of the stream is reached. 
///
/// \param in the stream to be read
/// \param end the character until which should be read
/// \return  string containing the characters read, excluding leading and trailing whitespace 
/// and excluding the terminating character \a end.
std::string read_until(std::istream& in, char end);

/// \brief checks that the next character read from the stream.
/// \param in the stream to be read
/// \param c the character that should be read
/// \param err the error message to be used if the next character is not \a c.
/// \throw \c std::runtime_error( \a err \c ) if the next character is not \a c
///  reads the next character (slipping white space) and checks if it is the same
///  as the character passed as argument \a c and throws a \c std::runtime_error otherwise.
inline void check_character(std::istream& in, char test, const std::string& err) {
  char c;
  in >> c;
  if (c!=test)
    boost::throw_exception(std::runtime_error(err));
}

/// \brief parses an XML tag
/// \param in the stream to be read
/// \param skip_comments if true, the function skips any comments or processing instructions while parsing
/// \return an \c XMLTag structure containing information about the tag
// XMLTag parse_tag(std::istream& in, bool skip_comments = true);

/// reads the contents of an element, until the first < character found
std::string parse_content(std::istream& in);

/// \brief skips an XML element
/// \param in the stream to be read
/// \param tag the opening tag of the element to be skipped
/// the function reads until it finds the closing tag correesponding to the \a tag passed as argument.
// void skip_element(std::istream& in,const XMLTag& tag);

/// \brief checks whether the next tag in the XML file has the given name
/// \param in the stream to be read
/// \param name the name of the expected tag
/// \throw \c std::runtime_error if the next tag read from the stream \a in does not have the name given by the argument \a name.
void check_tag(std::istream& in, const std::string& name);

} // end namespace alps

#endif // ALPS_PARSER_PARSER_H
