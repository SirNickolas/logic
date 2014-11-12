To get the parsing tree (a `logic::Expression` object) you are to do as following:

```cpp
auto tokens = logic::Lexer(data, size).Tokenize();
auto expression = logic::Parser(tokens.data()).Parse(/*data*/);
```

Optional parameter `data` in `logic::Parser::Parse` is used for debugging purposes only.

`logic::Expression` objects support the following methods:

- ```cpp
  bool Evaluate(const map<string, bool>&) const;
  ```

  Evaluates the expression with the given context.

- ```cpp
  void ToString(ostream&) const;
  ```

  Sends the string representation to the output stream.

- ```cpp
  unique_ptr<logic::Expression> Clone() const;
  ```

  Returns a deep copy of an object.

- ```cpp
  void Traverse(logic::Visitor*) const;
  ```

  Traverses the parsing tree using the Visitor design pattern.

There are two built-in visitors:

- `logic::DependencyVisitor`

  Has a method `GetResult() -> set<string>` that returns names of all variables used.
- `logic::SubsetVisitor`

  Has a method `GetResult() -> vector<const logic::Expression*>` that returns all the subexpressions
  (without repetitions).

For more information see file `main.cpp`.
