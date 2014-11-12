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

#include "expression.h"
#include <sstream>
#include "exception.h"
#include "../make_unique.h"

using namespace std;

namespace logic {
    Const::Const(bool value): _value(value) { }

    bool Const::GetValue() const {
        return _value;
    }

    bool Const::Evaluate(const map<string, bool>&) const {
        return _value;
    }

    void Const::Traverse(Visitor* visitor) const {
        visitor->Visit(this);
    }

    short Const::GetPriority() const {
        return 5;
    }

    bool Const::IsLeftAssociative() const {
        return true;
    }

    auto Const::Clone() const -> unique_ptr<Expression> {
        return make_unique<Const>(_value);
    }

    void Const::ToString(ostream& os) const {
        os << char(_value + '0');
    }

    Variable::Variable(const string& name): _name(name) { }

    Variable::Variable(string&& name): _name(move(name)) { }

    string Variable::GetName() const {
        return _name;
    }

    bool Variable::Evaluate(const map<string, bool>& context) const {
        auto it = context.find(_name);
        if (it == end(context))
            throw UndeclaredVariableError(_name);
        return it->second;
    }

    void Variable::Traverse(Visitor* visitor) const {
        visitor->Visit(this);
    }

    short Variable::GetPriority() const {
        return 5;
    }

    bool Variable::IsLeftAssociative() const {
        return true;
    }

    auto Variable::Clone() const -> unique_ptr<Expression> {
        return make_unique<Variable>(_name);
    }

    void Variable::ToString(ostream& os) const {
        os << _name;
    }

    void ReprCachable::ToString(ostream& os) const {
        if (_repr.empty()) {
            ostringstream ss;
            _ToString(ss);
            _repr = ss.str();
        }
        os << _repr;
    }

    UnaryOp::UnaryOp(unique_ptr<Expression>&& x): _x(move(x)) { }

    void UnaryOp::Traverse(Visitor* visitor) const {
        _x->Traverse(visitor);
        visitor->Visit(this);
    }

    bool UnaryOp::IsLeftAssociative() const {
        return !_IsPrefix();
    }

    void UnaryOp::_ToString(ostream& os) const {
        bool prefix = _IsPrefix();
        auto prio = GetPriority(), xPrio = _x->GetPriority();
        bool needsBraces = prio > xPrio || (prio == xPrio && prefix == _x->IsLeftAssociative());
        if (prefix)
            os << _GetSign();
        if (needsBraces) { os << '('; }
        _x->ToString(os);
        if (needsBraces) { os << ')'; }
        if (!prefix)
            os << _GetSign();
    }

    bool Not::Evaluate(const map<string, bool>& context) const {
        return !_x->Evaluate(context);
    }

    short Not::GetPriority() const {
        return 4;
    }

    const char* Not::_GetSign() const {
        return "!";
    }

    bool Not::_IsPrefix() const {
        return true;
    }

    auto Not::Clone() const -> unique_ptr<Expression> {
        return make_unique<Not>(_x->Clone());
    }

    BinaryOp::BinaryOp(unique_ptr<Expression>&& a, unique_ptr<Expression>&& b):
        _a(move(a)), _b(move(b)) { }

    void BinaryOp::Traverse(Visitor* visitor) const {
        _a->Traverse(visitor);
        _b->Traverse(visitor);
        visitor->Visit(this);
    }

    void BinaryOp::_ToString(ostream& os) const {
        auto prio = GetPriority(), aPrio = _a->GetPriority(), bPrio = _b->GetPriority();
        bool needsBraces = prio > aPrio || (prio == aPrio && !IsLeftAssociative());
        if (needsBraces) { os << '('; }
        _a->ToString(os);
        if (needsBraces) { os << ')'; }
        os << _GetSign();
        needsBraces = prio > bPrio || (prio == bPrio && _b->IsLeftAssociative());
        if (needsBraces) { os << '('; }
        _b->ToString(os);
        if (needsBraces) { os << ')'; }
    }

    bool LeftAssociativeBinaryOp::IsLeftAssociative() const {
        return true;
    }

    bool And::Evaluate(const map<string, bool>& context) const {
        return _a->Evaluate(context) && _b->Evaluate(context);
    }

    short And::GetPriority() const {
        return 3;
    }

    const char* And::_GetSign() const {
        return " & ";
    }

    auto And::Clone() const -> unique_ptr<Expression> {
        return make_unique<And>(_a->Clone(), _b->Clone());
    }

    bool Or::Evaluate(const map<string, bool>& context) const {
        return _a->Evaluate(context) || _b->Evaluate(context);
    }

    short Or::GetPriority() const {
        return 2;
    }

    const char* Or::_GetSign() const {
        return " | ";
    }

    auto Or::Clone() const -> unique_ptr<Expression> {
        return make_unique<Or>(_a->Clone(), _b->Clone());
    }

    bool Xor::Evaluate(const map<string, bool>& context) const {
        return _a->Evaluate(context) ^ _b->Evaluate(context);
    }

    short Xor::GetPriority() const {
        return 2;
    }

    const char* Xor::_GetSign() const {
        return " ^ ";
    }

    auto Xor::Clone() const -> unique_ptr<Expression> {
        return make_unique<Xor>(_a->Clone(), _b->Clone());
    }

    bool Implication::Evaluate(const map<string, bool>& context) const {
        return !_a->Evaluate(context) || _b->Evaluate(context);
    }

    short Implication::GetPriority() const {
        return 1;
    }

    const char* Implication::_GetSign() const {
        return " -> ";
    }

    auto Implication::Clone() const -> unique_ptr<Expression> {
        return make_unique<Implication>(_a->Clone(), _b->Clone());
    }

    bool Equivalence::Evaluate(const map<string, bool>& context) const {
        return _a->Evaluate(context) == _b->Evaluate(context);
    }

    short Equivalence::GetPriority() const {
        return 1;
    }

    const char* Equivalence::_GetSign() const {
        return " <-> ";
    }

    auto Equivalence::Clone() const -> unique_ptr<Expression> {
        return make_unique<Equivalence>(_a->Clone(), _b->Clone());
    }
}
