#ifndef _WBRANCHSTYLE_H_
#define _WBRANCHSTYLE_H_

class wbranchstyle
{
public:
    wchar_t      delim_start;
    wchar_t      delim_end;
    bool         output_indent;
    int          tabsize;
    wchar_t      space;
    bool         output_newline;
    std::wstring newline;

public:
    wbranchstyle()
    {
        delim_start = L'{';
        delim_end = L'}';
        output_indent = true;
        tabsize = 4;
        space = L' ';
        output_newline = true;
        newline = L"\n";
    }
};

#endif // _WBRANCHSTYLE_H_

