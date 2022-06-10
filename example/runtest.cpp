#include <parse_args.hpp>
#include <iostream>


void parse_args (int argc, char ** argv,
                 std :: string & s,
                 int32_t & i,
                 double & d,
                 float & f,
                 bool & b,
                 bool & flag,
                 std :: vector < int32_t > & integers
                 )

{
  parser :: ArgumentParser argparse("Parse Command line arguments example");

  // Syntax : argparse.add_argument < dtype > (tag_name, short_flag, long_flag, help_message, required, default_value)

  argparse.add_argument < std :: string >("sArg", "s", "str_value",  "This is the help message for the string value set as --str_value",  false, "This is the default value");
  argparse.add_argument < int32_t       >("iArg", "i", "int_value",  "This is the help message for the int    value set as --int_value",  false, 42);
  argparse.add_argument < double        >("dArg", "d", "dbl_value",  "This is the help message for the double value set as --dbl_value",  false, 3.14);
  argparse.add_argument < float         >("fArg", "f", "flt_value",  "This is the help message for the float  value set as --flt_value",  false, 1.23f);
  argparse.add_argument < bool          >("bArg", "b", "bool_value", "This is the help message for the bool   value set as --bool_value", true);

  argparse.add_flag ("eArg", "e", "flag", "This is the help message for the flag set as --flag");

  argparse.add_argument < int32_t > ("vArg", "v", "vector", "This is the help message for the vector set as --vector", false, 1);

  argparse.parse_args(argc, argv);

  argparse.get("sArg", s);
  argparse.get("iArg", i);
  argparse.get("dArg", d);
  argparse.get("fArg", f);
  argparse.get("bArg", b);
  argparse.get("eArg", flag);
  argparse.get("vArg", integers);

  return;
}


template < typename T >
std :: ostream & operator << (std :: ostream & os, const std :: vector < T > & x)
{
  for ( std :: size_t i = 0; i < x.size() - 1; ++i )
    os << x[i] << ", ";

  os << x[x.size() - 1];

  return os;
}


int main (int argc, char ** argv)
{
  std :: string mystr;
  int32_t myint;
  double mydbl;
  float myflt;
  bool mybool;
  bool myflag;
  std :: vector < int32_t > integers;

  parse_args (argc, argv, mystr, myint, mydbl, myflt, mybool, myflag, integers);

  if (myflag)
    std :: cout << "Flag ENABLED" << std :: endl;
  else
    std :: cout << "Flag DISABLED" << std :: endl;

  if (mybool)
  {
    std :: cout << "With the boolean ENABLED my variables are:" << std :: endl;
    std :: cout << "- String : "  << mystr << std :: endl;
    std :: cout << "- Integer : " << myint << std :: endl;
    std :: cout << "- Double : "  << mydbl << std :: endl;
    std :: cout << "- Float : "   << myflt << std :: endl;
  }
  else
  {
    std :: cout << "With the boolean DISABLED my variables are:" << std :: endl;
    std :: cout << "- String : "  << mystr << std :: endl;
    std :: cout << "- Integer : " << myint << std :: endl;
    std :: cout << "- Double : "  << mydbl << std :: endl;
    std :: cout << "- Float : "   << myflt << std :: endl;
  }

  std :: cout << "The integers vector contains: " << integers << std :: endl;

  return 0;
}

