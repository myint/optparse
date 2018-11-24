#include "optparse.h"

#include <iostream>

int main(int argc, char *argv[])
{
    const std::string version =
        "%prog 1.0\nCopyright (C) 2017\n"
        "This is free software: you are free to change and redistribute it.\n"
        "There is NO WARRANTY, to the extent permitted by law.";

    optparse::OptionParserExcept parser = optparse::OptionParserExcept().version(version)
          .description("Description")
          .epilog("Epilog");

    parser.add_option("--string")
        .help("This is a really long text... very long indeed! "
              "It must be wrapped on normal terminals.");

    try
    {
        const optparse::Values &options = parser.parse_args(argc, argv);
        const std::vector<std::string> args = parser.args();

        std::cout << "string: " << options["string"] << std::endl;

        std::cout << std::endl << "leftover arguments: " << std::endl;
        for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
        {
            std::cout << "arg: " << *it << std::endl;
        }
    }
    catch (int ret_code)
    {
        std::cout << "OptionParser has thrown " << ret_code << std::endl;
        return ret_code;
    }

    return 0;
}
