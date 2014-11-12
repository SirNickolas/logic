/*
 * Copyright (C) 2014 Nickolay Bukreyev
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#include "lexer.h"
#include <algorithm>

using namespace std;

%%{
    machine LogicCalculatorLexer;
    access _;
    variable p _p;
    variable pe _pe;

    not = [!\-];
    and = [&*];
    or  = [|+];
    xor = '^';
    imp = '->';
    eq  = '<->' | '<=>' | '=' | '==';
    var = (alpha | '_') . (alnum | '_')*;

    main := |*
        '(' => { _result.emplace_back(TK_OPEN_BRACE); };
        ')' => { _result.emplace_back(TK_CLOSE_BRACE); };
        '0' => { _result.emplace_back(TK_LITERAL, false); };
        '1' => { _result.emplace_back(TK_LITERAL, true); };
        var => { _result.emplace_back(TK_VARIABLE, _ts, _te - _ts); };
        not => { _result.emplace_back(TK_NOT); };
        and => { _result.emplace_back(TK_AND); };
        or  => { _result.emplace_back(TK_OR); };
        xor => { _result.emplace_back(TK_XOR); };
        imp => { _result.emplace_back(TK_IMPLICATION); };
        eq  => { _result.emplace_back(TK_EQUIVALENCE); };
        space;
    *|;
}%%

%% write data;

namespace logic {
    Lexer::Lexer(): _p(nullptr) { }

    Lexer::Lexer(const char* p, size_t size): _p(p), _pe(p + size) {
        %% write init;
    }

    vector<Token> Lexer::Tokenize() {
        if (!_p)
            return { };
        _result.clear();
        auto eof = _pe;
        %% write exec;
        if (_cs != %%{ write first_final; }%%)
            throw LexicalError(_p);
        _result.emplace_back();
        return move(_result);
    }
}
