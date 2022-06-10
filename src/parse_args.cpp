#include <parse_args.hpp>


namespace parser
{

int32_t ArgumentParser :: PRINT_HELP            = 101;
int32_t ArgumentParser :: ERROR_PARSER          = 102;
int32_t ArgumentParser :: ERROR_PARSER_INPUTS   = 103;
int32_t ArgumentParser :: ERROR_PARSER_REQUIRED = 104;
int32_t ArgumentParser :: ERROR_PARSER_UNKNOWN  = 105;
int32_t ArgumentParser :: ERROR_PARSER_INVARG   = 106;
int32_t ArgumentParser :: ERROR_PARSER_OUTRANGE = 107;
int32_t ArgumentParser :: ERROR_PARSER_BOOL     = 108;
int32_t ArgumentParser :: ERROR_PARSER_CHAR     = 109;
int32_t ArgumentParser :: ERROR_PRIVATE_FLAG    = 110;


// Argument Class

ArgumentParser :: argument :: argument (std :: string && name,
  std :: string && short_flag, std :: string && long_flag,
  std :: string && help,
  const bool & required,
  const bool & is_flag,
  std :: string && defualt_value,
  std :: string && data_type)
    : name (std :: move(name)),
      short_flag (std :: move(short_flag)), long_flag (std :: move(long_flag)),
      help (std :: move(help)),
      defualt_value (std :: move(defualt_value)),
      data_type (std :: move(data_type)),
      required (required),
      is_flag (is_flag)
{
}


// ArgumentParser Class

ArgumentParser :: ArgumentParser ( std :: string && description ) : description (std :: move(description))
{
}

void ArgumentParser :: parse_args (const int32_t & argc, char ** argv)
{
  this->program = std :: string(argv[0]);

  // This line is valid only if there are not vectors in command line
  //if ( static_cast < int32_t >(this->args.size()) < ((argc - 1) >> 1) )
  //  this->error_parsing_inputs_arg();

  // check if help is required
  for (int32_t i = 0; i < argc; ++i)
  {
    if ( std :: string (argv[i]) == "--help" )
      this->print_help(0);
  }

  for (auto & arg : this->args)
  {
    bool found = false;

    for (int32_t i = 0; i < argc; ++i)
    {
      if ( std :: string (argv[i]) == "-" + arg.short_flag ||
           std :: string (argv[i]) == "--" + arg.long_flag
         )
      {
        if (arg.is_flag)
        {
          found = true;
          arg.values.emplace_back("1");
        }
        else
        {
          arg.values.emplace_back(argv[i + 1]);

          for (int32_t j = i + 2; j < argc; ++j)
          {
            if ( argv[j][0] != '-' ) arg.values.emplace_back(argv[j]);
            else break;
          }

          found = true;
          break;
        }
      }
    }

    if ( !found )
    {
      if (arg.required) this->error_parsing_required_arg(arg.name);
      else              arg.values.emplace_back(std :: move(arg.defualt_value));
    }
  }
}


void ArgumentParser :: print_help (const int32_t & error_index)
{
  std :: cerr << "Usage: " << this->program << " ";

  for (auto & arg : this->args)
    std :: cerr << ((arg.required) ? "-" + arg.short_flag + " <" + arg.data_type + ">"
                                   : "[-" + arg.long_flag + " <" + arg.data_type + "> ]") << " ";

  std :: cerr << std :: endl << std :: endl;

  std :: cerr << this->description << std :: endl << std :: endl;

  std :: cerr << "optional arguments:" << std :: endl;

  for(auto & arg : this->args)
    std :: cerr << "\t" << std :: left << std :: setw(6)  << "-" + arg.short_flag + ","
                        << std :: left << std :: setw(12) <<"--" + arg.long_flag
                        << "\t\t" + arg.help
                        << std :: endl;

  std :: exit(error_index);
}



void ArgumentParser :: error_parsing_type (const std :: string & data_type)
{
  std :: cerr << "Error parsing! Wrong type variable: given, '"
              << data_type
              << "'; possible type are 'int', 'float', 'double', 'string', 'char', 'bool'"
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER);
}

void ArgumentParser :: error_parsing_inputs_arg ()
{
  std :: cerr << "Error parsing! Not enough inputs in command line."
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER_INPUTS);
}

void ArgumentParser :: error_parsing_required_arg (const std :: string & name)
{
  std :: cerr << "Error parsing! Input not found. Variable "
              << name << " is required but it's not found in command line."
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER_REQUIRED);
}

void ArgumentParser :: error_parsing_unknown_arg (const std :: string & name)
{
  std :: cerr << "Error parsing! Variable "
              << name
              << " unknown by parser."
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER_UNKNOWN);
}

void ArgumentParser :: error_parsing_invalid_arg (const std :: string & name, const std :: string & value)
{
  std :: cerr << "Error parsing! Invalid argument for variable "
              << name
              << ". Given: "
              << value
              << " ."
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER_INVARG);
}

void ArgumentParser :: error_parsing_out_of_range_arg (const std :: string & name, const std :: string & value)
{
  std :: cerr << "Error parsing! Out of range argument for variable "
              << name
              << ". Given: "
              << value
              << " ."
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER_OUTRANGE);
}

void ArgumentParser :: error_parsing_bool (const std :: string & name, const std :: string & value)
{
  std :: cerr << "Error parsing! Invalid argument for bool variable "
              << name
              << ". Given: "
              << value
              << " ."
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER_BOOL);
}

void ArgumentParser :: error_parsing_char (const std :: string & name, const std :: string & value)
{
  std :: cerr << "Error parsing! Invalid argument for char variable "
              << name
              << ". Given: "
              << value
              << " ."
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER_CHAR);
}

void ArgumentParser :: error_private_flag (const std :: string & flag)
{
  std :: cerr << "Error parsing! Invalid flag provided. The given flag "
              << flag
              << " is reserved for special usage by the libray. Please change it with a different value"
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PRIVATE_FLAG);
}


void ArgumentParser :: add_flag (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help)
{
  this->args.emplace_back(argument(static_cast < std :: string && >(name),
    static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag),
    static_cast < std :: string && >(help),
    false, // require
    true, // is_flag
    static_cast < std :: string && >("0"),
    static_cast < std :: string && >("bool")));
}

// export the specialized members

template void ArgumentParser :: add_argument < int >(std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, int default_value);
template void ArgumentParser :: add_argument < float >(std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, float default_value);
template void ArgumentParser :: add_argument < double >(std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, double default_value);
template void ArgumentParser :: add_argument < std :: string >(std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, std :: string default_value);
template void ArgumentParser :: add_argument < bool >(std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, bool default_value);


template void ArgumentParser :: get < int >(const std :: string & name, int & values);
template void ArgumentParser :: get < float >(const std :: string & name, float & values);
template void ArgumentParser :: get < double >(const std :: string & name, double & values);
template void ArgumentParser :: get < std :: string >(const std :: string & name, std :: string & values);
template void ArgumentParser :: get < bool >(const std :: string & name, bool & values);


} // end namespace parser
