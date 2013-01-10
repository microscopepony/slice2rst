// **********************************************************************
//
// Copyright (c) 2003-2006 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <vector>

#include <IceUtil/Options.h>
#include <Slice/Preprocessor.h>
#include <Slice/Parser.h>

#include "RstGen.hpp"

using namespace std;
using namespace Slice;

void
usage(const char* n)
{
    cerr << "Usage: " << n << " [options] slice-file\n";
    cerr <<
        "Options:\n"
        "-h, --help       Show this message.\n"
        "-Idir            Put DIR in the include file search path.\n";
}

int
main(int argc, char* argv[])
{
    vector<string> cppArgs;
    //bool preprocess;
    //string include;
    //string output;
    //string dllExport;
    //bool impl;
    //bool depend;
    //bool debug;
    //bool ice;
    //bool checksum;
    //bool stream;
    //bool caseSensitive;

    string inputFilename;

#ifdef false
    po::variables_map vm;

    //try
    //{

    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("input-file", po::value< vector<string> >(), "input file")
        ;


    po::options_description allowed("Allowed options");
    allowed.add_options()
        ("help", "produce help message")
        ("output", po::value<std::string>(), "output file name")
        ;

    po::options_description cmdline("Command line options");
    cmdline.add(allowed).add(hidden);

    po::positional_options_description p;
    p.add("input-file", -1);

    po::store(po::command_line_parser(argc, argv).
              options(cmdline).positional(p).run(), vm);

    po::notify(vm);

    if (vm.count("help"))
    {
        cout << allowed << "\n";
        return 1;
    }

    if ( vm.count("input-file"))
    {
        vector<string> sv = vm["input-file"].as<vector<string> >();
        //for (vector<string>::iterator i=sv.begin(); i!=sv.end(); i++)
        //{
        //cout << *i << "\n";
        //}

        inputFilename = sv[0];
    }

    if (vm.count("output"))
    {
        cout << "Output filename is " << vm["output"].as<std::string>() << "\n";
    }

    //}
    //catch(...)
    //{
    //cout << "CRAP!\n";
    //}


    cout << "Input file is \"" << inputFilename << "\"\n";

#endif

    IceUtilInternal::Options opts;
    opts.addOpt("h", "help");
    opts.addOpt("I", "", IceUtilInternal::Options::NeedArg, "",
                IceUtilInternal::Options::Repeat);


    vector<string> args;
    try
    {
        args = opts.parse(argc, argv);
    }
    catch(const IceUtilInternal::BadOptException& e)
    {
        cerr << argv[0] << ": " << e.reason << endl;
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    if(opts.isSet("h") || opts.isSet("help"))
    {
        usage(argv[0]);
        return EXIT_SUCCESS;
    }

    vector<string> includePaths;
    includePaths = opts.argVec("I");
    for (vector<string>::const_iterator i=includePaths.begin(); i!=includePaths.end(); ++i)
    {
        cppArgs.push_back("-I" + Preprocessor::normalizeIncludePath(*i));
    }

    if(args.empty())
    {
        cerr << argv[0] << ": no input file" << endl;
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    int status = EXIT_SUCCESS;

    vector<string>::const_iterator i = args.begin();
    std::string filename = *i;
#if ICE_INT_VERSION / 100 < 304
    Preprocessor* icecpp = new Preprocessor(argv[0], *i, cppArgs);
#else
    PreprocessorPtr icecpp = Preprocessor::create(argv[0], *i, cppArgs);
#endif

    FILE* cppHandle = icecpp->preprocess(true);

    if (cppHandle == 0)
    {
        return EXIT_FAILURE;
    }

//#if ICE_INT_VERSION / 100 < 304
    bool ignRedefs = false;
    bool all = false;
    bool allowIcePrefix = false;
    bool caseSensitive = true;
    //StringList defaultGlobalMetadata;
    UnitPtr u = Unit::createUnit(ignRedefs, all, allowIcePrefix, caseSensitive);
//#else
//      UnitPtr u = Unit::createUnit(false, false, false);
//#endif

    int parseStatus = u->parse(*i, cppHandle, false);

    if ( parseStatus == EXIT_FAILURE )
    {
        cerr << "Parsing failed";
        exit(0);
    }

    if(!icecpp->close())
    {
        u->destroy();
        return EXIT_FAILURE;
    }


    RstGen* myRstGen = new RstGen;
    u->visit(myRstGen, true);

    u->destroy();

    return status;
}
