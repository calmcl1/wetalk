/* 
 * File:   wetalk.cpp
 * Author: callum
 *
 * Created on 25 June 2014, 14:06
 */

#include <iostream>
#include <stdio.h>
extern "C" {
#include <gstreamer-1.0/gst/gst.h>
}
#include <jack/jack.h>
#include "include/ezOptionParser.hpp"
#include "wetalk.h"


using namespace std;


int parse_options(ez::ezOptionParser opt, int argc, const char* argv[]) {
    opt.overview = "weTalk is a FOSS audio communications server, designed to \
be used with off-the-shelf hardware.";
    opt.syntax = "wetalk [OPTIONS]";
    opt.example = "wetalk [-t test_name] [-d]\n\n";
    opt.footer = "weTalk Copyright (C) 2014\n\n";

    opt.add(
            "", // Default value
            0, // Required value?
            0, // Number of args expected?
            0, // Delimiter if using multiple args
            "Display usage instructions", // Help
            "-h", // Flag
            "--help" // Flag
            );
    
        opt.add(
            "", // Default value
            0, // Required value?
            -1, // Number of args expected?
            ',', // Delimiter if using multiple args
            "Run integration tests", // Help
            "-t", // Flag
            "--test" // Flag
            );


    opt.parse(argc, argv);

    if (opt.isSet("-h")) {
        string usage;
        opt.getUsage(usage);
        cout << usage;

        return -1;
    }
    
    /*if(opt.isSet("'t")){
        vector<string> tests_to_run;
        opt.get("-t")->getStrings(tests_to_run);
        for (int i=0; i<tests_to_run.size();++i){
            if (tests_to_run[i] == "jack"){
                jack_test();
            }
        }
        return -1;
    }*/
}

int main(int argc, const char *argv[]) {

    ez::ezOptionParser opt;
    int ret = parse_options(opt, argc, argv);
    if (ret == -1) {
        return 0;
    };


    char ** argv_nonconst = const_cast<char**> (argv);

    //gst_init(&argc, &argv_nonconst);
 

    return 0;
}

