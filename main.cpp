#include <algorithm>
#include <bitset>
#include <iostream>
#include <map>
#include <string>
#include "gray/for_each.h"
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

    vector<vector<bool>> table(
        1LL << deps.size(),
        vector<bool>(deps.size() + subsets.size())
    );
    map<string, bool> context;
    for (const string& var: deps)
        context.emplace(var, 0);
    bitset<MAX_VARIABLES> mask;
    gray::ForEach(deps.size(),
        [&](int bit) {
            if (bit != -1) {
                mask.flip(bit);
                context.find(deps[bit])->second ^= true;
            }
            auto pos = mask.to_ulong();
            size_t i;
            for (i = 0; i < deps.size(); i++)
                table[pos][i] = mask[deps.size() - i - 1];
            for (auto e: subsets)
                table[pos][i++] = e->Evaluate(context);
        }
    );

    for_each(deps.rbegin(), deps.rend(),
        [ ](const string& var) { cout << var << '\t'; }
    );
    for (size_t i = 1; i <= subsets.size(); i++)
        cout << 'F' << i << '\t';
    cout << endl;
    for (const auto& row: table) {
        for (auto b: row)
            cout << b << '\t';
        cout << endl;
    }
}
