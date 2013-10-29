#ifndef _WBRANCH_H_
#define _WBRANCH_H_

#include <string>
#include <vector>

#include "wbranchstyle.h"

using namespace std;

class wbranch
{

protected:
    vector<wbranch*> _child;

public:
    wstring tag;
    wstring value;

    // separator for path
    wchar_t separator;

public: /* Constractor, Destractor */
    wbranch(wstring atag);
    wbranch(wstring atag, wstring avalue);
    wbranch(wstring atag, vector<wbranch*> achildren);
    wbranch(wstring atag, wbranch* achild);
    wbranch(const wchar_t* atag);
    wbranch(const wchar_t* atag, const wchar_t* avalue);
    wbranch(const wchar_t* atag, wbranch* achild);
    ~wbranch();
    void destroy();

public:

    wbranch*    operator[](int index);
    void        add(wbranch* achild);
    void        clear(void);
    int         size(void);

    wbranch*         child(wstring tag);
    vector<wbranch*> children(wstring filter);

    wbranch*    path(wstring path);
    wbranch*    path(vector<wstring> path);

public:

    wstring     to_s(void);
    wstring     to_s(wbranchstyle style);
    wstring     to_s(wbranchstyle style, int nest_max);
    static      wbranch* from_s(wstring text, wbranchstyle style);
    static int  get_max_nest(wstring text, wbranchstyle style);
    static bool isvalid(wstring text, wbranchstyle style);
    static int  get_item_string(wstring text, wbranchstyle style, int index, wstring* result);

protected:

    wstring     to_s(int& nest_current, int nest_max, wbranchstyle style);

};

#endif /* _WBRANCH_H_ */

