#include "option_parser.h"

#include <iostream>
#include <sstream>
#include <string>
#include <complex>
#include <algorithm>


class Output {
public:
  Output(const std::string& d) : delim(d), first(true) {}
  void operator() (const std::string& s) {
    if (first)
      first = false;
    else
      std::cout << delim;
    std::cout << s;
  }
  ~Output() { std::cout << std::endl; }
  const std::string& delim;
  bool first;
};

class MyCallback : public optparse::Callback {
public:
  MyCallback() : counter(0) {}
  void operator() (const optparse::Option& option, const std::string& opt, const std::string& val, const optparse::OptionParser& parser) {
    counter++;
    std::cout << "--- MyCallback --- " << counter << ". time called" << std::endl;
    std::cout << "--- MyCallback --- option.action(): " << option.action() << std::endl;
    std::cout << "--- MyCallback --- opt: " << opt << std::endl;
    std::cout << "--- MyCallback --- val: " << val << std::endl;
    std::cout << "--- MyCallback --- parser.usage(): " << parser.usage() << std::endl;
    std::cout << std::endl;
  }
  int counter;
};

int main(int argc, char *argv[])
{
#ifndef DISABLE_USAGE
  const std::string usage = "usage: %prog [OPTION]... DIR [FILE]...";
#else
  const std::string usage = SUPPRESS_USAGE;
#endif
  const std::string version = "%prog 1.0\nCopyright (C) 2010 Johannes Weißl\n"
    "License GPLv3+: GNU GPL version 3 or later "
    "<http://gnu.org/licenses/gpl.html>.\n"
    "This is free software: you are free to change and redistribute it.\n"
    "There is NO WARRANTY, to the extent permitted by law.";
  const std::string desc = "Lorem ipsum dolor sit amet, consectetur adipisicing"
    " elit, sed do eiusmod tempor incididunt ut labore et dolore magna"
    " aliqua.\nUt enim ad minim veniam, quis nostrud exercitation ullamco"
    " laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor"
    " in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla"
    " pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa"
    " qui officia deserunt mollit anim id est laborum.";
  const std::string epilog = "Sed ut perspiciatis unde omnis iste natus error sit"
    " voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque"
    " ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae"
    " dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit"
    " aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos"
    " qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui"
    " dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia"
    " non numquam eius modi tempora incidunt ut labore et dolore magnam"
    " aliquam quaerat voluptatem.";

  OptionParser parser = OptionParser()
    .usage(usage)
    .version(version)
    .description(desc)
    .epilog(epilog)
#ifdef DISABLE_INTERSPERSED_ARGS
    .disable_interspersed_args()
#endif
  ;

  parser.set_defaults("verbosity", "50");
  parser.set_defaults("no_clear", "0");

  // test all actions
  parser.add_option("--clear") .action("store_false") .dest("no_clear") .help("clear (default)");
  parser.add_option("--no-clear") .action("store_true") .help("not clear");
  parser.add_option("--string")
    .help("This is a really long text... very long indeed! It must be wrapped on normal terminals.");
  parser.add_option("-x", "--clause", "--sentence") .metavar("SENTENCE") .set_default("I'm a sentence")
    .help("This is a really long text... very long indeed! It must be wrapped on normal terminals. "
          "Also it should appear not on the same line as the option.");
  parser.add_option("-k") .action("count") .help("how many times?");
  parser.add_option("-v", "--verbose") .action("store_const") .set_const("100") .dest("verbosity") .help("be verbose!");
  parser.add_option("-s", "--silent") .action("store_const") .set_const("0") .dest("verbosity") .help("be silent!");
  parser.add_option("-n", "--number") .type("int") .set_default("1") .metavar("NUM") .help("number of files (default: %default)");
  parser.add_option("-H") .action("help") .help("alternative help");
  parser.add_option("-V") .action("version") .help("alternative version");
  parser.add_option("-i", "--int") .action("store") .type("int") .set_default(3) .help("default: %default");
  parser.add_option("-f", "--float") .action("store") .type("float") .set_default(5.3) .help("default: %default");
  parser.add_option("-c", "--complex") .action("store") .type("complex");
  char const* const choices[] = { "foo", "bar", "baz" };
  parser.add_option("-C", "--choices") .choices(&choices[0], &choices[3]);
  parser.add_option("-m", "--more") .action("append");
  parser.add_option("--more-milk") .action("append_const") .set_const("milk");
  parser.add_option("--hidden") .help(SUPPRESS_HELP);

  MyCallback mc;
  parser.add_option("-K", "--callback") .action("callback") .callback(mc) .help("callback test");

  OptionGroup group = OptionGroup("Dangerous Options",
      "Caution: use these options at your own risk. "
      "It is believed that some of them bite.");
  group.add_option("-g") .action("store_true") .help("Group option.") .set_default("0");
  parser.add_option_group(group);

  Values& options = parser.parse_args(argc, argv);
  vector<std::string> args = parser.args();

  std::cout << "clear: " << (options.get("no_clear") ? "false" : "true") << std::endl;
  std::cout << "string: " << options["string"] << std::endl;
  std::cout << "clause: " << options["clause"] << std::endl;
  std::cout << "k: " << options["k"] << std::endl;
  std::cout << "verbosity: " << options["verbosity"] << std::endl;
  std::cout << "number: " << (int) options.get("number") << std::endl;
  std::cout << "int: " << (int) options.get("int") << std::endl;
  std::cout << "float: " << (float) options.get("float") << std::endl;
  complex<double> c = 0;
  if (options.is_set("complex")) {
    std::stringstream ss;
    ss << options["complex"];
    ss >> c;
  }
  std::cout << "complex: " << c << std::endl;
  std::cout << "choices: " << (const char*) options.get("choices") << std::endl;
  std::cout << "more: ";
  for_each(options.all("more").begin(), options.all("more").end(), Output(", "));
  std::cout << "more_milk: ";
  {
    Output out(", ");
    for (Values::iterator it = options.all("more_milk").begin(); it != options.all("more_milk").end(); ++it)
      out(*it);
  }
  std::cout << "hidden: " << options["hidden"] << std::endl;
  std::cout << "group: " << (options.get("g") ? "true" : "false") << std::endl;

  std::cout << std::endl << "leftover arguments: " << std::endl;
  for (vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
    std::cout << "arg: " << *it << std::endl;
  }

  return 0;
}
