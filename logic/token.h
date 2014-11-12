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

#include "token_id.h"

namespace logic {
    struct Token {
        TokenId id;
        union {
            bool value;
            struct {
                const char* identifier;
                size_t size;
            };
        };

        explicit Token(TokenId id = TK_EOF): id(id) { }
        Token(TokenId id, bool value): id(id), value(value) { }
        Token(TokenId id, const char* identifier, size_t size):
            id(id), identifier(identifier), size(size) { }
    };
}
