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

#include <cstdint>

namespace logic {
    enum TokenId: uint8_t {
        TK_EOF = 0,
        TK_OPEN_BRACE = 1,
        TK_CLOSE_BRACE = 2,
        TK_LITERAL = 3,
        TK_VARIABLE = 4,
        TK_NOT = 5,
        TK_AND = 6,
        TK_OR = 7,
        TK_XOR = 8,
        TK_IMPLICATION = 9,
        TK_EQUIVALENCE = 10,
    };
}
