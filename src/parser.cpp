#include "parser.h"
#include <iostream>
#include <string.h>

// TODO CHECK IF READ FROM FILE OR CMD LINE
// file size needs to be multiple of block size for alignment and limited to 1GB
Parser::Parser(Arguments &args, int argc, char **argv)
{
    parse_cmd_line(args, argc, argv);
}

void Parser::parse_cmd_line(Arguments &args, int argc, char **argv)
{
    char *tokens[argc - 1];
    for (int i = 0; i < argc - 1; i++)
    {
        if (strncmp(argv[i + 1], "-h", 2) == 0)
            display_help();

        tokens[i] = argv[i + 1];
    }

    for (char *token : tokens)
    {
        if (strncmp(token, "-runtime=", 9) == 0)
            set_runtime(token, args);
        else if (strncmp(token, "-filesize=", 10) == 0)
            set_filesize(token, args);
        else if (strncmp(token, "-copysize=", 10) == 0)
            set_buflen(token, args);
        else if (strncmp(token, "-dir=", 5) == 0)
            set_path(token, args);
        else if (strncmp(token, "-mode=", 6) == 0)
            set_mode(token, args);
        else if (strncmp(token, "-engine=", 8) == 0)
            set_engine(token, args);
        else
        {
            std::cout << "Unknow command " << token;
            exit(1);
        }
    }

    if (args.fsize % args.buflen != 0)
    {
        perror("Not aligned sizes");
        exit(1);
    }
}

void Parser::display_help()
{
    std::cout << "Possible commands are:" << std::endl;
    std::cout << "-runtime=\tSet runtime seconds (Default 60)" << std::endl;
    std::cout << "-filesize=\tSet file size (Default 2 MiB)" << std::endl;
    std::cout << "-copysize=\tSet copy size for memcpy (Default 4KiB)" << std::endl;
    std::cout << "-dir=\t\tPath to file (/dev/null or /dev/zero for MAP_ANONYMOUS)" << std::endl;
    std::cout << "-mode=\t\tPossible modes are:\n\tread\n\twrite\n\trandread\n\trandwrite (Default read)" << std::endl;
    std::cout << "-engine=\tPossible engines are mmap and pmem (Default mmap)" << std::endl;
    exit(0);
}

void Parser::set_runtime(char *token, Arguments &args)
{
    std::string temp = token;
    temp.erase(0, 9);
    try
    {
        args.runtime = stoi(temp);
    }
    catch (std::invalid_argument &e)
    {
        perror("Invalid runtime");
        exit(1);
    }
}

void Parser::set_filesize(char *token, Arguments &args)
{
    std::string temp = token;
    temp.erase(0, 10);
    try
    {
        args.fsize = stoi(temp);
    }
    catch (std::invalid_argument &e)
    {
        perror("Invalid file size");
        exit(1);
    }
}

void Parser::set_buflen(char *token, Arguments &args)
{
    std::string temp = token;
    temp.erase(0, 10);
    try
    {
        args.buflen = stoi(temp);
    }
    catch (std::invalid_argument &e)
    {
        perror("Invalid copy size");
        exit(1);
    }
}

void Parser::set_path(char *token, Arguments &args)
{
    std::string temp = token;
    temp.erase(0, 5);
    char *dir = new char[temp.length() + 1];
    strcpy(dir, temp.c_str());
    if (strcmp(dir, "/dev/null") == 0 || strcmp(dir, "/dev/zero") == 0)
        args.map_anon = 1;
    args.path = dir;
}

void Parser::set_mode(char *token, Arguments &args)
{
    std::string temp = token;
    temp.erase(0, 6);
    char *mode = new char[temp.length() + 1];
    strcpy(mode, temp.c_str());
    if (strncmp(mode, "read", 4) == 0)
        args.mode = 0;
    else if (strncmp(mode, "write", 5) == 0)
        args.mode = 1;
    else if (strncmp(mode, "randread", 8) == 0)
        args.mode = 2;
    else if (strncmp(mode, "randwrite", 9) == 0)
        args.mode = 3;
    else
        args.mode = 4;
}

void Parser::set_engine(char *token, Arguments &args)
{
    std::string temp = token;
    temp.erase(0, 8);
    char *mode = new char[temp.length() + 1];
    strcpy(mode, temp.c_str());
    if (strncmp(mode, "mmap", 4) == 0)
        args.engine = 0;
    else if (strncmp(mode, "pmem", 4) == 0)
        args.engine = 1;
}