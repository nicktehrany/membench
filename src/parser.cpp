#include "parser.h"
#include <iostream>
#include <string.h>
#include <fstream>

// file size needs to be multiple of block size for alignment and limited to 1GB
Parser::Parser(Arguments &args, int argc, char **argv)
{

    char *tokens[100]; // If reading from file won;t know how many tokens, 100 max

    if (argc > 100)
    {
        perror("Too many commands");
        exit(1);
    }
    for (int i = 0; i < argc - 1; i++)
    {
        if (strncmp(argv[i + 1], "-h", 2) == 0)
            display_help();
        else if (strncmp(argv[i + 1], "-file=", 5) == 0)
        {
            argc = parse_file(args, argv[i + 1], tokens);
            break;
        }
        tokens[i] = argv[i + 1];
    }
    parse_cmd_line(args, tokens, argc - 1);
}

void Parser::parse_cmd_line(Arguments &args, char *tokens[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (strncmp(tokens[i], "-runtime=", 9) == 0)
            set_runtime(tokens[i], args);
        else if (strncmp(tokens[i], "-filesize=", 10) == 0)
            set_filesize(tokens[i], args);
        else if (strncmp(tokens[i], "-copysize=", 10) == 0)
            set_buflen(tokens[i], args);
        else if (strncmp(tokens[i], "-dir=", 5) == 0)
            set_path(tokens[i], args);
        else if (strncmp(tokens[i], "-mode=", 6) == 0)
            set_mode(tokens[i], args);
        else if (strncmp(tokens[i], "-engine=", 8) == 0)
            set_engine(tokens[i], args);
        else
        {
            std::cout << "Unknow command " << tokens[i];
            exit(1);
        }
    }

    if (args.fsize % args.buflen != 0)
    {
        perror("Not aligned sizes");
        exit(1);
    }
}

int Parser::parse_file(Arguments &args, char *token, char *tokens[])
{
    std::string temp = token;
    temp.erase(0, 6);

    std::ifstream infile;
    infile.open(temp);

    if (infile.fail())
    {
        perror("Invalid file");
        exit(1);
    }

    std::string cmd;
    int counter = 0;
    while (infile >> cmd)
    {
        if (counter > 100)
        {
            perror("Too many commands");
            exit(1);
        }
        char *x = new char[cmd.length() + 1];
        strcpy(x, cmd.c_str());
        tokens[counter] = x;
        counter++;
    }

    return counter + 1; // Account for program call in cmd line
}
void Parser::display_help()
{
    std::cout << "Possible commands are:" << std::endl;
    std::cout << "-file=\tProvide an input file with commands" << std::endl;
    std::cout << "-runtime=\tSet runtime seconds (Default 60)" << std::endl;
    std::cout << "-filesize=\tSet file size (Default 2 MiB)" << std::endl;
    std::cout << "-copysize=\tSet copy size for memcpy (Default 4KiB)" << std::endl;
    std::cout << "-dir=\t\tPath to file (/dev/null or /dev/zero for MAP_ANONYMOUS)" << std::endl;
    std::cout << "-mode=\t\tPossible modes are: read write randread randwrite (Default read)" << std::endl;
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
        args.fsize = stol(temp);
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
        args.buflen = stol(temp);
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