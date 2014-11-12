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

#include "subset_visitor.h"
#include <functional>
#include <sstream>
#include "expression.h"

using namespace std;

namespace logic {
    void SubsetVisitor::Visit(const Expression* e) {
        ostringstream ss;
        e->ToString(ss);
        size_t h = hash<string>()(ss.str());
        auto lb = _hashes.lower_bound(h);
        if (lb == _hashes.end() || _hashes.key_comp()(h, *lb)) {
            _result.push_back(e);
            _hashes.insert(lb, h);
        }
    }

    auto SubsetVisitor::GetResult() -> vector<const Expression*> {
        return move(_result);
    }
}
