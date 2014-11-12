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

#pragma once

#include <stdexcept>
#include <vector>
#include "token.h"

namespace logic {
    class LexicalError: public std::logic_error {
    public:
        using std::logic_error::logic_error;
    };

    class Lexer {
    public:
        Lexer();
        Lexer(const char*, size_t);
        auto Tokenize() -> std::vector<Token>;

    private:
        std::vector<Token> _result;

        int _cs;
        int _act;
        const char* _p;
        const char* _pe;
        const char* _ts;
        const char* _te;
    };
}
