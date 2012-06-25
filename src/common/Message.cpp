#include <boost/regex.hpp>
#include "Message.hpp"

namespace aeirc
{
  namespace Message
  {
    bool ParsePrefix(const std::string& str, Prefix& p)
    {
      bool ret = false;
      try
      {
        boost::match_results<std::string::const_iterator> what;
        boost::regex expression("(!(\\w+))?(@(\\w+))?", boost::regex::perl);
        std::string::const_iterator start = str.begin(), end = str.end(); 
        ret = boost::regex_search(start, end, what, expression);
        
        // what[0] contains the whole string
        // what[1] contains the username with ! prefix
        // what[2] contains just the username
        // what[3] contains the hostname with ! prefix
        // what[4] contains just the hostname
        if (what.size() >= 5 && what[1].matched)
        {
          p.user = std::string(what[2].first, what[2].second);
        }
        if (what.size() >= 5 && what[2].matched)
        {
          p.host = std::string(what[4].first, what[4].second);
        }
      }
      catch (std::exception& e)
      {
        return false;
      }
      
      return ret;
    }
  }
}
