# MyBash — A Unix Shell Implementation in C

A POSIX-like shell built from scratch in C, supporting pipelines, I/O redirection, background execution, and built-in commands. Developed as a systems programming project for an Operating Systems course.

## Features

- **Pipelines** — chain commands with `|`, connecting stdout/stdin across processes
- **I/O redirection** — `<` and `>` for input/output redirection per command
- **Background execution** — `&` to run pipelines without blocking the shell
- **Built-in commands** — `cd`, `exit`, `help`, handled internally without forking
- **Process management** — fork/exec/wait orchestration with zombie process cleanup
- **Custom lexer/parser** — tokenizes and builds the pipeline structure from raw input

## Architecture

| File | Responsibility |
|---|---|
| `mybash.c` | Entry point, REPL loop, prompt rendering |
| `parsing.c` / `parser.h` | Tokenizes user input and builds the pipeline structure |
| `command.c` | Core data structures: `scommand` (simple command) and `pipeline` |
| `execute.c` | Process orchestration: forking, piping, redirection, waiting |
| `builtin.c` | Internal command implementations (`cd`, `exit`, `help`) |
| `strextra.c` | String utility helpers used across modules |

The lexer/parser core is precompiled per architecture (`objects-x86_64/`, `objects-i386/`) and linked at build time.

## Build & Run

Requires `gcc` and `glib-2.0`.

```bash
make          # builds the ./mybash binary
./mybash      # starts the shell
```

## Testing

The project includes a test suite covering parsing, pipelines, and command execution, with syscalls mocked to test process logic in isolation, plus memory-leak checks via Valgrind.

```bash
make test       # run the test suite
make memtest    # run tests under Valgrind to catch leaks
```

## About This Project

This was a **team project for the Operating Systems course at FaMAF (Facultad de Matemática, Astronomía, Física y Computación), Universidad Nacional de Córdoba (UNC)**, built by 4 students over several work branches with cross-review between members. Each module had a primary author and, in most cases, a collaborator who helped debug and refine the implementation:

| Module | Author(s) | Collaborator(s) |
|---|---|---|
| `command` | Lissandro Bosque, Franco Galassi, Máximo Ortega, Joaquín Pairetti | — |
| `parsing` | Máximo Ortega, Joaquín Pairetti | — |
| `execute` | Lissandro Bosque | Franco Galassi |
| `builtin` | Franco Galassi | Joaquín Pairetti |
| Documentation | Joaquín Pairetti | — |
| Demo video | Franco Galassi | — |

**Team:** Joaquín Pairetti · Máximo Ortega · Franco Galassi · Lissandro Bosque

AI assistance was used responsibly during development — mainly to clarify OS theory (syscalls, processes), debug memory/pointer issues, and understand `glib` APIs — rather than to generate the implementation outright.
