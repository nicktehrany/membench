#ifndef PARSER_H
#define PARSER_H

// Default values
struct Arguments
{
    int runtime = 60;
    const char *path = "file";
    int fsize = 2097152;
    int buflen = 4096;
    int mode = 0;     //0=read 1=write 2=randread 3=randwrite
    int map_anon = 0; // Mapping without backing of file on file system
    int engine = 0;   // Default mmap engine
};

class Parser
{
public:
    Parser(Arguments &, int, char **);

private:
    void display_help();
    void parse_cmd_line(Arguments &, char *[], int);
    int parse_file(Arguments &, char *, char *[]);
    void set_runtime(char *, Arguments &);
    void set_filesize(char *, Arguments &);
    void set_buflen(char *, Arguments &);
    void set_path(char *, Arguments &);
    void set_mode(char *, Arguments &);
    void set_engine(char *, Arguments &);
};

#endif