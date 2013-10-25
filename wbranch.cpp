#include "wbranch.h"

wbranch::wbranch(std::wstring atag)
{
    tag = std::wstring(atag);
    value = std::wstring(L"");
    return;
}

wbranch::wbranch(std::wstring atag, std::wstring avalue)
{
    tag = std::wstring(atag);
    value = std::wstring(avalue);
    return;
}

wbranch::wbranch(std::wstring atag, std::vector<wbranch*> achildren)
{
    tag = std::wstring(atag);
    _child = std::vector<wbranch*>(achildren);
    return;
}

wbranch::wbranch(std::wstring atag, wbranch* achild)
{
    tag = std::wstring(atag);
    _child.push_back(achild);
    return;
}

wbranch::wbranch(const wchar_t* atag)
{
    tag = std::wstring(atag);
    value = std::wstring(L"");
    return;
}

wbranch::wbranch(const wchar_t* atag, const wchar_t* avalue)
{
    tag = std::wstring(atag);
    value = std::wstring(avalue);
    return;
}

wbranch::wbranch(const wchar_t* atag, wbranch* achild)
{
    tag = std::wstring(atag);
    _child.push_back(achild);
    return;
}

wbranch::~wbranch()
{
    // if (_child != NULL)
    //     delete(_child);
    return;
}

/**
 * \brief 子孫まで追って殺す
 */
void wbranch::destroy()
{
    return;
}

wbranch* wbranch::operator[](int index)
{
    if (index > 0 || index < _child.size())
        return _child.at(index);
    else
        return NULL;
}

void wbranch::add(wbranch* achild)
{
    _child.push_back(achild);
    return;
}

void wbranch::clear(void)
{
    _child.clear();
}

int wbranch::size(void)
{
    return _child.size();
}

wbranch* wbranch::child(std::wstring tag)
{
    if (!_child.size())
        return NULL;

    std::vector<wbranch*>::iterator it = _child.begin();
    for (; it != _child.end(); it++) {
        if (tag == (*it)->tag)
            return *it;
    }

    return NULL;
}

std::wstring wbranch::to_s(void)
{
    int nest = 0;
    return to_s(nest, -1, wbranchstyle());
}

std::wstring wbranch::to_s(wbranchstyle style)
{
    int nest = 0;
    return to_s(nest, -1, style);
}

std::wstring wbranch::to_s(wbranchstyle style, int nest_max)
{
    int nest = 0;
    return to_s(nest, nest_max, style);
}

wbranch* wbranch::from_s(std::wstring text, wbranchstyle style)
{
    if (!isvalid(text, style))
        return NULL;

    wbranch* res = NULL;
    std::wstring buf;
    int nextpos = get_item_string(text, style, 0, &buf);

    if (nextpos < 0)
        return NULL;

    if (!buf.empty()) {
        int max_nest = get_max_nest(buf, style);

        // タグ取得
        std::wstring _tag = std::wstring(buf);
        int spos = buf.find_first_of(style.delim_start, 0);
        _tag.erase(spos);
        if (style.output_newline) {
            for (int i = 0; i<_tag.size(); i++) {
                if (_tag.at(i) == '\n' || _tag.at(i) == '\r') {
                    _tag.erase(0, 1);
                    i--;
                }
                else
                    break;
            }
        }
        if (style.output_indent) {
            for (int i = 0; i<_tag.size(); i++) {
                if (_tag.at(i) == style.space) {
                    _tag.erase(0, 1);
                    i--;
                }
                else
                    break;
            }
        }

        res = new wbranch(_tag);

        if (max_nest == 1) {
            // ここで終わりなので、値を抽出
            std::wstring _value = std::wstring(buf);
            _value.erase(0, spos+1);
            int epos = _value.find_last_of(style.delim_end);
            _value.erase(epos);
            // 戻り値として設定
            res->value = _value;
        }
        else if (max_nest > 1) {
            // 子達に突入

            // 最初の子要素の頭位置
            int mypos = text.find_first_of(style.delim_start, 0);

            for (; mypos < text.size(); ) { // 子毎

                wbranch* mychild = NULL;

                if (mypos <= 0)
                    break;

                std::wstring mybuf;
                int mysize = get_item_string(text, style, mypos + 1, &mybuf);

                if (mysize >= 0 && !mybuf.empty()) {
                    mychild = from_s(mybuf, style);
                    if (mychild)
                        res->add(mychild);
                }
                else {
                    break;
                }

                mypos += mysize;
            }
        }
        else {
            // 0
            // 要素としておかしい
            return NULL;
        }
    }

    return res;
}

/**
 * \brief 最大ネスト数を調べる(0=ブラケットなし, 1=ブラケット1ペア)
 */
int wbranch::get_max_nest(std::wstring text, wbranchstyle style)
{
    int nest = 0;
    for (int i=0; i < text.size(); i++) {
        wchar_t c = text.at(i);
        if (c == style.delim_start) {
            nest++;
        }
    }
    return nest;
}

/**
 * \brief 項目のネスト状況が正しいか調べる
 */
bool wbranch::isvalid(std::wstring text, wbranchstyle style)
{
    int nest = 0;

    for (int i=0; i < text.size(); i++) {
        wchar_t c = text.at(i);
        if (c == style.delim_start)
            nest++;
        else if (c == style.delim_end)
            nest--;

        if (nest < 0)
            return false;
    }

    return nest == 0;
}

/**
 * \brief 指定された文字列から一番外側を覆う要素を構成する文字列を取得する
 */
int wbranch::get_item_string(std::wstring text, wbranchstyle style, int index, std::wstring* result)
{
    if (index >= text.size()) {
        result = NULL;
        return -1; // out of range
    }

    if (index)
        text.erase(0, index);

    int i = text.find_first_of(style.delim_start, 0);

    if (i < 0)
        return -1;

    int nest = 0;
    for (; i < text.size(); i++) {
        wchar_t c = text.at(i);
        if (c == style.delim_start)
            nest++;
        else if (c == style.delim_end)
            nest--;
        if (nest <= 0) {
            i++;
            break;
        }
    }

    if (i < text.size())
        text.erase(i);

    *result = text;

    return i;
}

std::wstring wbranch::to_s(int& nest_current, int nest_max, wbranchstyle style)
{
    if (nest_max > 0 && nest_current > nest_max)
        return L"";

    std::wstring res(L"");
    std::wstring ind(L"");

    if (style.output_indent) {
        for (int i=0; i<nest_current; i++) {
            for (int j=0; j<style.tabsize; j++) {
                ind += style.space;
            }
        }
    }
    res += ind + tag + style.delim_start;

    if (!_child.size()) { // this instance is a terminal point
        res += value + style.delim_end;
    }
    else {
        res += (style.output_newline ? style.newline : L"");
        std::vector<wbranch*>::iterator it = _child.begin();
        for (; it != _child.end(); it++) {
            res += (*it)->to_s(++nest_current, nest_max, style)
                + (style.output_newline ? style.newline : L"");
            --nest_current;
        }
        res += ind + style.delim_end;
    }

    return res;
}

