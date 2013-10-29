#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include <locale>

#include "wbranch.h"

using namespace std;

int main(int argc, char** argv)
{
    // you can call these locale setting functions which you like.
    locale::global(locale(""));
    //setlocale(LC_CTYPE, "");

    // root object
    wbranch family(L"山田");

        // add 2 children

        wbranch child1(L"太郎", L"A型");
        family.add(&child1);

        wbranch child2(L"花子", L"B型");
        family.add(&child2);

    // style of serialization
    wbranchstyle style;
    style.tabsize = 2;
    style.output_indent = true;
    style.output_newline = true;

    wcout << L"original:" << endl << family.to_s(style).c_str()
        << endl << endl;

    // from_s() requires the same style that to_s() used.
    wbranch* n = wbranch::from_s(family.to_s(style), style);

    // you can change the style after loading.
    style.output_indent = false;
    style.output_newline = false;

    // this output string formatted by new style
    wcout << L"ressult:" << endl << n->to_s(style).c_str() << endl << endl;

    // get child by tag name
    wbranch* child = family.child(L"太郎");
    if (child != NULL)
        wcout << family.tag.c_str() << child->tag.c_str() << L"は"
            << child->value.c_str() << endl;

    // get child by path string
    wbranch filesystem(L"/");
    wbranch dirA(L"dirA");
    wbranch dirB(L"dirB");
    wbranch fileA(L"fileA");
    wbranch fileB(L"fileB");

    filesystem.add(&dirA);
    dirA.add(&dirB);
    dirB.add(&fileA);
    dirB.add(&fileB);

    fileA.value = L"foo";
    fileB.value = L"bar";

    wbranch* myfile = filesystem.path(L"dirA/dirB/fileA");
    wcout << L"fileA: " << endl;
    wcout << myfile->value.c_str() << endl;

    return 0;
}

