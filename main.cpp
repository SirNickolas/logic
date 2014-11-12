#include <algorithm>
#include <bitset>
#include <iostream>
#include <map>
#include <string>
#include "logic/lexer.h"
#include "logic/parser.h"
#include "logic/dependency_visitor.h"
#include "logic/subset_visitor.h"

using namespace std;

const short MAX_VARIABLES = 31;

class OpCountVisitor: public logic::Visitor {
public:
    void Visit(const logic::Expression* e) {
        if (dynamic_cast<const logic::Operator*>(e))
            _count++;
    }

    size_t GetCount() const { return _count; }

private:
    size_t _count = 0;
};

int main() {
    string s;
    getline(cin, s);

    unique_ptr<logic::Expression> expr;
    try {
        auto tokens = logic::Lexer(s.c_str(), s.length()).Tokenize();
        expr = logic::Parser(tokens.data()).Parse(s.c_str());
    }
    catch (logic::LexicalError&) {
        cerr << "Lexical error\n";
        return 1;
    }
    catch (logic::SyntaxError&) {
        cerr << "Syntax error\n";
        return 1;
    }

    logic::DependencyVisitor dVisitor;
    expr->Traverse(&dVisitor);
    auto dependencySet = dVisitor.GetResult();
    if (dependencySet.size() > MAX_VARIABLES) {
        cerr << "Too many variables!\n";
        return 1;
    }
    vector<string> deps(dependencySet.rbegin(), dependencySet.rend());

    logic::SubsetVisitor sVisitor;
    expr->Traverse(&sVisitor);
    auto subsets = sVisitor.GetResult();
    subsets.erase(remove_if(begin(subsets), end(subsets),
        [ ](const logic::Expression* e) {
            return !dynamic_cast<const logic::Operator*>(e);
        }
    ), end(subsets));

    OpCountVisitor cVisitor;
    expr->Traverse(&cVisitor);
    cout << endl << cVisitor.GetCount() << " operations\n";

    for (size_t i = 0; i < subsets.size(); i++) {
        cout << 'F' << i + 1 << " = ";
        subsets[i]->ToString(cout);
        cout << endl;
    }
    cout << endl;

    //Generating table with Gray code.
    vector<vector<bool>> table(
        1LL << deps.size(),
        vector<bool>(deps.size() + subsets.size())
    );
    map<string, bool> context;
    for (const string& var: deps)
        context.emplace(var, 0);
    size_t j = deps.size();
    for (auto e: subsets)
        table[0x0][j++] = e->Evaluate(context);
    bitset<MAX_VARIABLES> mask;
    for (size_t i = 0x1; i < 1LL << deps.size(); i++) {
        size_t x = i, pos = 0;
        while (!(x & 0x1)) {
            x >>= 1;
            pos++;
        }
        mask.flip(pos);
        context.find(deps[pos])->second ^= true;
        pos = mask.to_ulong();
        for (j = 0; j < deps.size(); j++)
            table[pos][j] = mask[deps.size() - j - 1];
        for (auto e: subsets)
            table[pos][j++] = e->Evaluate(context);
    }

    for_each(deps.rbegin(), deps.rend(),
        [ ](const string& var) { cout << var << '\t'; }
    );
    for (j = 1; j <= subsets.size(); j++)
        cout << 'F' << j << '\t';
    cout << endl;
    for (const auto& row: table) {
        for (auto b: row)
            cout << b << '\t';
        cout << endl;
    }
}
