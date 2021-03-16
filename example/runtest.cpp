#include <parse_args.hpp>
#include <iostream>


void parse_args (int argc, char ** argv,
                 std :: string & s,
                 int32_t & i,
                 double & d,
                 float & f,
                 bool & b
                 )

{
  parser :: ArgumentParser argparse("Parse Command line arguments example");

  // Syntax : argparse.add_argument < dtype > (tag_name, short_flag, long_flag, help_message, required, default_value)

  argparse.add_argument < std :: string >("sArg", "s", "str_value",  "This is the help message for the string value set as --str_value",  false, "This is the default value");
  argparse.add_argument < int32_t       >("iArg", "i", "int_value",  "This is the help message for the int    value set as --int_value",  false, 42);
  argparse.add_argument < double        >("dArg", "d", "dbl_value",  "This is the help message for the double value set as --dbl_value",  false, 3.14);
  argparse.add_argument < float         >("fArg", "f", "flt_value",  "This is the help message for the float  value set as --flt_value",  false, 1.23f);
  argparse.add_argument < bool          >("bArg", "b", "bool_value", "This is the help message for the bool   value set as --bool_value", true);

  argparse.parse_args(argc, argv);

  argparse.get("sArg", s);
  argparse.get("iArg", i);
  argparse.get("dArg", d);
  argparse.get("fArg", f);
  argparse.get("bArg", b);

  return;
}


int main (int argc, char ** argv)
{
  std :: string mystr;
  int32_t myint;
  double mydbl;
  float myflt;
  bool mybool;

  parse_args (argc, argv, mystr, myint, mydbl, myflt, mybool);

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

  return 0;
}

