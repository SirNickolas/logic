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

#include "parser.h"
#include <vector>
#include "../make_unique.h"

using namespace std;

namespace {
    using namespace logic;

    struct StackFrame {
        unique_ptr<Expression> lhs4, lhs3, lhs2, lhs1;
        int notCounter = 0;
        auto (*binary)(unique_ptr<Expression>&&, unique_ptr<Expression>&&) -> unique_ptr<BinaryOp>;
    };

    template <class T>
    unique_ptr<BinaryOp> CreateBinary(unique_ptr<Expression>&& a, unique_ptr<Expression>&& b) {
        return make_unique<T>(move(a), move(b));
    }
}

%%{
    machine LogicCalculatorParser;
    alphtype unsigned char;
    import "token_id.h";
    access _;
    variable p _p;
    getkey _p->id;

    prepush {
        _stack.emplace_back();
        frames.emplace_back();
        f = &frames.back();
    }

    postpop {
        _stack.pop_back();
        frames.pop_back();
        f = &frames.back();
    }

    action createExpression {
        fcall main;
    }

    action tryRet {
        if (!_top)
            throw SyntaxError(hint);
        frames[frames.size() - 2].lhs4 = move(f->lhs1);
        fret;
    }

    action break {
        fbreak;
    }

    action createLiteral {
        f->lhs4 = make_unique<Const>(fpc->value);
    }

    action createVariable {
        f->lhs4 = make_unique<Variable>(string(fpc->identifier, fpc->size));
    }

    action incNotCounter {
        f->notCounter++;
    }

    action setLhs4 {
        while (f->notCounter) {
            f->lhs4 = make_unique<Not>(move(f->lhs4));
            f->notCounter--;
        }
    }

    action setAnd { f->binary = CreateBinary<And>; }
    action setOr  { f->binary = CreateBinary<Or>; }
    action setXor { f->binary = CreateBinary<Xor>; }
    action setImp { f->binary = CreateBinary<Implication>; }
    action setEq  { f->binary = CreateBinary<Equivalence>; }

    action first3 {
        f->lhs3 = move(f->lhs4);
    }

    # action append3 {
        # f->lhs3 = f->binary(move(f->lhs3), move(f->lhs4));
    # }

    action appendAnd {
        f->lhs3 = make_unique<And>(move(f->lhs3), move(f->lhs4));
    }

    action first2 {
        f->lhs2 = move(f->lhs3);
    }

    action append2 {
        f->lhs2 = f->binary(move(f->lhs2), move(f->lhs3));
    }

    action first1 {
        f->lhs1 = move(f->lhs2);
    }

    action append1 {
        f->lhs1 = f->binary(move(f->lhs1), move(f->lhs2));
    }

    prio5 =
        TK_LITERAL @createLiteral |
        TK_VARIABLE @createVariable |
        TK_OPEN_BRACE @createExpression;

    prio4 =
        TK_NOT* $incNotCounter prio5 %setLhs4;

    prio3 = prio4 %first3 (
        # TK_AND @setAnd prio4 %append3
        TK_AND prio4 %appendAnd
    )*;

    prio2 = prio3 %first2 (
        (
            TK_OR @setOr |
            TK_XOR @setXor
        ) prio3 %append2
    )*;

    prio1 = prio2 %first1 (
        (
            TK_IMPLICATION @setImp |
            TK_EQUIVALENCE @setEq
        ) prio2 %append1
    )*;

    main :=
        prio1 (TK_CLOSE_BRACE @tryRet | TK_EOF @break);
}%%

%% write data;

namespace logic {
    Parser::Parser(): _p(nullptr) { }

    Parser::Parser(const Token* p): _p(p) {
        %% write init;
    }

    auto Parser::Parse(const char hint[ ]) -> unique_ptr<Expression> {
        if (!_p)
            return nullptr;
        vector<int> _stack;
        vector<StackFrame> frames(1);
        auto f = &frames[0];
        %% write exec noend;
        if (_cs != %%{ write first_final; }%% || _top)
            throw SyntaxError(hint);
        return move(f->lhs1);
    }
}
