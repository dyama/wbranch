#ifndef _BRANCH_H_
#define _BRANCH_H_

#include <string>
#include <vector>

#include "branchstyle.h"


class branch
{

protected:

    std::vector<branch*> _child;

public:

    std::string tag;
    std::string value;

public: /* Constractor, Destractor */

    branch(std::string atag)
    {
        tag = std::string(atag);
        value = std::string("");
        return;
    }

    branch(std::string atag, std::string avalue)
    {
        tag = std::string(atag);
        value = std::string(avalue);
        return;
    }

    branch(std::string atag, std::vector<branch*> achildren)
    {
        tag = std::string(atag);
        _child = std::vector<branch*>(achildren);
        return;
    }

    branch(std::string atag, branch* achild)
    {
        tag = std::string(atag);
        _child.push_back(achild);
        return;
    }

    branch(const char* atag)
    {
        tag = std::string(atag);
        value = std::string("");
        return;
    }

    branch(const char* atag, const char* avalue)
    {
        tag = std::string(atag);
        value = std::string(avalue);
        return;
    }

    branch(const char* atag, branch* achild)
    {
        tag = std::string(atag);
        _child.push_back(achild);
        return;
    }

    ~branch()
    {
        // if (_child != NULL)
        //     delete(_child);
        return;
    }

    /**
     * \brief 子孫まで追って殺す
     */
    void destroy()
    {
        return;
    }

public:

    branch* operator[](int index)
    {
        if (index > 0 || index < _child.size())
            return _child.at(index);
        else
            return NULL;
    }

    void add(branch* achild)
    {
        _child.push_back(achild);
        return;
    }

    void clear(void)
    {
        _child.clear();
    }

    int size(void)
    {
        return _child.size();
    }

public:

    std::string to_s(void)
    {
        int nest = 0;
        return to_s(nest, -1, branchstyle());
    }

    std::string to_s(branchstyle style)
    {
        int nest = 0;
        return to_s(nest, -1, style);
    }

    std::string to_s(branchstyle style, int nest_max)
    {
        int nest = 0;
        return to_s(nest, nest_max, style);
    }

    static branch* from_s(std::string text, branchstyle style)
    {

        if (!isvalid(text, style))
            return NULL;

        branch* res = NULL;
        std::string buf;
        int nextpos = get_item_string(text, style, 0, &buf);

        if (nextpos < 0)
            return NULL;

        if (!buf.empty()) {
            int max_nest = get_max_nest(buf, style);

            // タグ取得
            std::string _tag = std::string(buf);
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

            res = new branch(_tag);

            if (max_nest == 1) {
                // ここで終わりなので、値を抽出
                std::string _value = std::string(buf);
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

                    branch* mychild = NULL;

                    if (mypos <= 0)
                        break;

                    std::string mybuf;
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
    static int get_max_nest(std::string text, branchstyle style)
    {
        int nest = 0;
        for (int i=0; i < text.size(); i++) {
            char c = text.at(i);
            if (c == style.delim_start) {
                nest++;
            }
        }
        return nest;
    }

    /**
     * \brief 項目のネスト状況が正しいか調べる
     */
    static bool isvalid(std::string text, branchstyle style)
    {
        int nest = 0;

        for (int i=0; i < text.size(); i++) {
            char c = text.at(i);
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
    static int get_item_string(std::string text, branchstyle style, int index, std::string* result)
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
            char c = text.at(i);
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

protected:

    std::string to_s(int& nest_current, int nest_max, branchstyle style)
    {
        if (nest_max > 0 && nest_current > nest_max)
            return "";

        std::string res("");
        std::string ind("");

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
            res += (style.output_newline ? style.newline : "");
            std::vector<branch*>::iterator it = _child.begin();
            for (; it != _child.end(); it++) {
                res += (*it)->to_s(++nest_current, nest_max, style)
                    + (style.output_newline ? style.newline : "");
                --nest_current;
            }
            res += ind + style.delim_end;
        }

        return res;
    }

};

#endif /* _BRANCH_H_ */

