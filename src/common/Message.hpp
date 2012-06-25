#ifndef AEIRC_MESSAGE_HPP_INCLUDED
#define AEIRC_MESSAGE_HPP_INCLUDED

#include <string>
#include <boost/regex.hpp>

namespace aeirc
{
  namespace Message
  {
    struct Prefix
    {
      std::string user;
      std::string host;
    };
    
    bool ParsePrefix(const std::string& str, Prefix& p);
  }
}

#endif // AEIRC_MESSAGE_HPP_INCLUDED
