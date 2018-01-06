#include "optparse.h"

#include <algorithm>
#include <complex>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>


class MyCallback : public optparse::Callback
{
public:
    MyCallback() : counter(0) {}
    void operator()(const optparse::Option &option, const std::string &opt, const std::string &val, const optparse::OptionParser &parser)
    {
        counter++;
    }
    int counter;
};


int custom_main(int argc, char *argv[])
{
    const std::string usage = "usage: %prog [OPTION]... DIR [FILE]...";

    const std::string version =
        "%prog 1.0\nCopyright (C) 2010 Johannes Weißl\n"
        "License GPLv3+: GNU GPL version 3 or later "
        "<http://gnu.org/licenses/gpl.html>.\n"
        "This is free software: you are free to change and redistribute it.\n"
        "There is NO WARRANTY, to the extent permitted by law.";
    const std::string desc =
        "Lorem ipsum dolor sit amet, consectetur adipisicing"
        " elit, sed do eiusmod tempor incididunt ut labore et dolore magna"
        " aliqua.\nUt enim ad minim veniam, quis nostrud exercitation ullamco"
        " laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor"
        " in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla"
        " pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa"
        " qui officia deserunt mollit anim id est laborum.";
    const std::string epilog =
        "Sed ut perspiciatis unde omnis iste natus error sit"
        " voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque"
        " ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae"
        " dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit"
        " aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos"
        " qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui"
        " dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia"
        " non numquam eius modi tempora incidunt ut labore et dolore magnam"
        " aliquam quaerat voluptatem.";

    optparse::OptionParser parser = optparse::OptionParser()
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
    parser.add_option("--clear").action("store_false").dest("no_clear").help("clear (default)");
    parser.add_option("--no-clear").action("store_true").help("not clear");
    parser.add_option("--string")
        .help("This is a really long text... very long indeed! It must be wrapped on normal terminals.");
    parser.add_option("-x", "--clause", "--sentence").metavar("SENTENCE").set_default("I'm a sentence")
        .help("This is a really long text... very long indeed! It must be wrapped on normal terminals. "
              "Also it should appear not on the same line as the option.");
    parser.add_option("-k").action("count").help("how many times?");
    parser.add_option("-v", "--verbose").action("store_const").set_const("100").dest("verbosity").help("be verbose!");
    parser.add_option("-s", "--silent").action("store_const").set_const("0").dest("verbosity").help("be silent!");
    parser.add_option("-n", "--number").type("int").set_default("1").metavar("NUM").help("number of files (default: %default)");
    parser.add_option("-H").action("help").help("alternative help");
    parser.add_option("-V").action("version").help("alternative version");
    parser.add_option("-i", "--int").action("store").type("int").set_default(3).help("default: %default");
    parser.add_option("-f", "--float").action("store").type("float").set_default(5.3).help("default: %default");
    parser.add_option("-c", "--complex").action("store").type("complex");
    char const *const choices[] = {"foo", "bar", "baz"};
    parser.add_option("-C", "--choices").choices(&choices[0], &choices[3]);
    parser.add_option("-m", "--more").action("append");
    parser.add_option("--more-milk").action("append_const").set_const("milk");
    parser.add_option("--hidden").suppress_help();

    MyCallback mc;
    parser.add_option("-K", "--callback").action("callback").callback(mc).help("callback test");

    optparse::OptionGroup group = optparse::OptionGroup(
        "Dangerous Options",
        "Caution: use these options at your own risk. "
        "It is believed that some of them bite.");
    group.add_option("-g").action("store_true").help("Group option.").set_default("0");
    parser.add_option_group(group);

    try
    {
        parser.parse_args(argc, argv);
        parser.args();
    }
    catch (const std::invalid_argument &exception)
    {
        //std::cerr << exception.what() << "\n";
    }

    return 0;
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *const data,
                                      const size_t size)
{
    // Split single array to simulate command line arguments.
    const size_t max_argument_size = 100;
    const size_t num_flags = size / max_argument_size;
    const int argc = 1 + num_flags;

    const size_t max_flags = 100;
    char *argv[max_flags];
    for (size_t i = 0; i < max_flags; ++i)
    {
        argv[i] = new char[max_argument_size];
        memset(argv[i], 0, max_argument_size);
    }

    const char *program_name = "program_name";
    strcpy(argv[0], program_name);

    for (size_t flag_index = 0; flag_index < num_flags; ++flag_index)
    {
        const size_t argv_index = 1 + flag_index;

        strncpy(
            argv[argv_index],
            reinterpret_cast<const char *>(
                &data[flag_index * max_argument_size]),
            max_argument_size);

        argv[argv_index][max_argument_size - 1] = '\0';
        //std::cerr << "argv[" << argv_index << "]: " << argv[argv_index] << "\n";
    }

    custom_main(argc, argv);

    for (size_t i = 0; i < max_flags; ++i)
    {
        delete [] argv[i];
    }

    return 0;
}
