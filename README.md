# MyLang Compiler & VM

A minimal “MyLang” compiler plus a tiny stack‐based VM.

## Language Features

- Integer literals (e.g. `42`)
- Variables (assignment with `=`, e.g. `x = 5;`)
- Binary arithmetic: `+`, `-`, `*`, `/` (integer division)
- `print(<expr>);`
- Statements must end in `;`
- Program is a sequence of statements. Variables default to `0` if used before assignment.

## Building

You need a C++17‐capable compiler (e.g. `g++` or `clang++`) and `make`.

```bash
# From the top‐level MyLang/ directory:
make
