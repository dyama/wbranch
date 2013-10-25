#ifndef _WBRANCH_H_
#define _WBRANCH_H_

#include <string>
#include <vector>

#include "wbranchstyle.h"

class wbranch
{

protected:
    std::vector<wbranch*> _child;

public:
    std::wstring tag;
    std::wstring value;

public: /* Constractor, Destractor */
    wbranch(std::wstring atag);
    wbranch(std::wstring atag, std::wstring avalue);
    wbranch(std::wstring atag, std::vector<wbranch*> achildren);
    wbranch(std::wstring atag, wbranch* achild);
    wbranch(const wchar_t* atag);
    wbranch(const wchar_t* atag, const wchar_t* avalue);
    wbranch(const wchar_t* atag, wbranch* achild);
    ~wbranch();
    void destroy();

public:
    wbranch* operator[](int index);
    void add(wbranch* achild);
    void clear(void);
    int size(void);

public:

    std::wstring to_s(void);
    std::wstring to_s(wbranchstyle style);
    std::wstring to_s(wbranchstyle style, int nest_max);
    static wbranch* from_s(std::wstring text, wbranchstyle style);
    static int get_max_nest(std::wstring text, wbranchstyle style);
    static bool isvalid(std::wstring text, wbranchstyle style);
    static int get_item_string(std::wstring text, wbranchstyle style, int index, std::wstring* result);

protected:

    std::wstring to_s(int& nest_current, int nest_max, wbranchstyle style);

};

#endif /* _WBRANCH_H_ */

