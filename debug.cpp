#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include <locale>

#include "wbranch.h"

int main(int argc, char** argv)
{
    std::locale::global(std::locale(""));
    //setlocale(LC_CTYPE, "");

    wbranch a(L"太郎", L"Aaaa");
    wbranch b(L"花子",L"Bbba");

    wbranch c(L"classA", &a);
    c.add(&b);

    std::wstring text = L"test{foo{hoge}bar{fuga{baz{piyo}}}}";

    wbranchstyle style;
    style.output_indent = true;
    style.output_newline = true;

    std::wcout << L"org:" << std::endl
        << c.to_s(style).c_str() << std::endl;

    wbranch* n = wbranch::from_s(c.to_s(style), style);

    style.output_indent = false;
    style.output_newline = false;

    //wprintf(L"converted:\n%s\n", n->to_s(style).c_str());

    std::wcout << L"res:" << std::endl
        << n->to_s(style).c_str() << std::endl;

    return 0;
}

