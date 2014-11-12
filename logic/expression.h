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

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include "visitor.h"

namespace logic {
    /*interface*/ class Expression {
    public:
        virtual ~Expression() = default;
        virtual bool Evaluate(const std::map<std::string, bool>&) const = 0;
        virtual void Traverse(Visitor*) const = 0;
        virtual short GetPriority() const = 0;
        virtual bool IsLeftAssociative() const = 0;
        virtual auto Clone() const -> std::unique_ptr<Expression> = 0;
        virtual void ToString(std::ostream&) const = 0;
    };

    class Const: public Expression {
    public:
        /*implicit*/ Const(bool = false);
        bool GetValue() const;
        bool Evaluate(const std::map<std::string, bool>&) const;
        void Traverse(Visitor*) const;
        short GetPriority() const;
        bool IsLeftAssociative() const;
        auto Clone() const -> std::unique_ptr<Expression>;
        void ToString(std::ostream&) const;

    private:
        bool _value;
    };

    class Variable: public Expression {
    public:
        Variable() = default;
        explicit Variable(const std::string&);
        explicit Variable(std::string&&);
        std::string GetName() const;
        bool Evaluate(const std::map<std::string, bool>&) const;
        void Traverse(Visitor*) const;
        short GetPriority() const;
        bool IsLeftAssociative() const;
        auto Clone() const -> std::unique_ptr<Expression>;
        void ToString(std::ostream&) const;

    private:
        std::string _name;
    };

    /*abstract*/ class ReprCachable: public Expression {
    public:
        void ToString(std::ostream&) const;

    protected:
        virtual void _ToString(std::ostream&) const = 0;

    private:
        mutable std::string _repr;
    };

    /*abstract*/ class Operator: public ReprCachable {
    protected:
        virtual const char* _GetSign() const = 0;
    };

    /*abstract*/ class UnaryOp: public Operator {
    public:
        explicit UnaryOp(std::unique_ptr<Expression>&& = nullptr);
        void Traverse(Visitor*) const;
        bool IsLeftAssociative() const;

    protected:
        std::unique_ptr<Expression> _x;

        void _ToString(std::ostream&) const;
        virtual bool _IsPrefix() const = 0;
    };

    class Not: public UnaryOp {
    public:
        using UnaryOp::UnaryOp;
        bool Evaluate(const std::map<std::string, bool>&) const;
        short GetPriority() const;
        auto Clone() const -> std::unique_ptr<Expression>;

    protected:
        const char* _GetSign() const;
        bool _IsPrefix() const;
    };

    /*abstract*/ class BinaryOp: public Operator {
    public:
        BinaryOp() = default;
        BinaryOp(std::unique_ptr<Expression>&&, std::unique_ptr<Expression>&&);
        void Traverse(Visitor*) const;

    protected:
        std::unique_ptr<Expression> _a, _b;

        void _ToString(std::ostream&) const;
    };

    /*abstract*/ class LeftAssociativeBinaryOp: public BinaryOp {
    public:
        using BinaryOp::BinaryOp;
        bool IsLeftAssociative() const;
    };

    class And: public LeftAssociativeBinaryOp {
    public:
        using LeftAssociativeBinaryOp::LeftAssociativeBinaryOp;
        bool Evaluate(const std::map<std::string, bool>&) const;
        short GetPriority() const;
        auto Clone() const -> std::unique_ptr<Expression>;

    protected:
        const char* _GetSign() const;
    };

    class Or: public LeftAssociativeBinaryOp {
    public:
        using LeftAssociativeBinaryOp::LeftAssociativeBinaryOp;
        bool Evaluate(const std::map<std::string, bool>&) const;
        short GetPriority() const;
        auto Clone() const -> std::unique_ptr<Expression>;

    protected:
        const char* _GetSign() const;
    };

    class Xor: public LeftAssociativeBinaryOp {
    public:
        using LeftAssociativeBinaryOp::LeftAssociativeBinaryOp;
        bool Evaluate(const std::map<std::string, bool>&) const;
        short GetPriority() const;
        auto Clone() const -> std::unique_ptr<Expression>;

    protected:
        const char* _GetSign() const;
    };

    class Implication: public LeftAssociativeBinaryOp {
    public:
        using LeftAssociativeBinaryOp::LeftAssociativeBinaryOp;
        bool Evaluate(const std::map<std::string, bool>&) const;
        short GetPriority() const;
        auto Clone() const -> std::unique_ptr<Expression>;

    protected:
        const char* _GetSign() const;
    };

    class Equivalence: public LeftAssociativeBinaryOp {
    public:
        using LeftAssociativeBinaryOp::LeftAssociativeBinaryOp;
        bool Evaluate(const std::map<std::string, bool>&) const;
        short GetPriority() const;
        auto Clone() const -> std::unique_ptr<Expression>;

    protected:
        const char* _GetSign() const;
    };
}
