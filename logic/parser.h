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
#include "expression.h"
#include "token.h"

namespace logic {
    class SyntaxError: public std::logic_error {
    public:
        using std::logic_error::logic_error;
    };

    class Parser {
    public:
        Parser();
        explicit Parser(const Token[ ]);
        auto Parse(const char hint[ ] = "") -> std::unique_ptr<Expression>;

    private:
        int _cs;
        int _top;
        const Token* _p;
        const Token* _ts;
        const Token* _te;
    };
}
