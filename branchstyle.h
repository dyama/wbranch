#ifndef _BRANCHSTYLE_H_
#define _BRANCHSTYLE_H_

class branchstyle
{
public:
    char        delim_start;
    char        delim_end;
    bool        output_indent;
    int         tabsize;
    char        space;
    bool        output_newline;
    std::string newline;

public:
    branchstyle()
    {
        delim_start = '{';
        delim_end = '}';
        output_indent = true;
        tabsize = 4;
        space = ' ';
        output_newline = true;
        newline = "\n";
    }
};

#endif // _BRANCHSTYLE_H_

