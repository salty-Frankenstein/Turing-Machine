# Turing Machine
A Multi-tape Turing Machine simulator, course project for **Formal Language and Automata**, NJU, 2021fall.

## Features
### Frontend
- Applicative parser combinator
  - implemented `Applicative` and `Alternative`: `pure`, `<*>`, (`<$>`), `*>`, `<*`, `<|>`.
  - basic combinators: `safisty`, `many`, `many1`, `sepBy` and more.
- Preprocessor
  - Normalization
- Error handling 
  - Preprocessing errors
  - Parsing error messages

### Backend
- Static checking for well-formedness of the IR
  - Normalization for the rules of the transfer function
- Full support for pattern-matching and wildcard
- Full implementation of both normal mode and verbose mode execution
- Error handling

### Shell
- Argument parsing
- Error handling
