```
       |
     \|/|/      ______     __  __     ______     __  __
   \|\\|//|/   /\  == \   /\ \/\ \   /\  ___\   /\ \_\ \
    \|\|/|/    \ \  __<   \ \ \_\ \  \ \___  \  \ \  __ \  
     \\|//      \ \_____\  \ \_____\  \/\_____\  \ \_\ \_\
 _\|/__|_\|/__   \/_____/   \/_____/   \/_____/   \/_/\/_/
```

# BUSH
*Blatantly Useless Shell: yet another imitation of CSH.*

My take on writing the shell in C++ using Flex and Bison, done as a part of CS252 Operating Systems course at Purdue University.

Even though this shell used some scaffolding files provided by instructors, I took liberty to clean up the code that I considered messy. I also implemented the shell using the modular structure that I personally prefer. In addition to that, the entire test suit provided by instructors has been revamped for better user experience.

**TL;DR** This shell was written in a way that may be significantly different from what instructors expected.


## Table of Contents

## Building
You can build the shell by using:
```
$ make
```
There are also `debug` and `release` targets, that make the executable print (or not to print) debug messages. You can fine tune the level of debugging detail printed by specifying `DEBUG` flag like this:
```
$ make DEBUG=4
```
You can run the test suit by using the following (which will force a release rebuild):
```
$ make test
```
 
**NOTE: BUSH requires [LibFIZ](https://github.com/jluchiji/fiz) to build correctly. LibFIZ is included as a git submodule, which has to be downloaded and built, therefore the building process above requires an Internet connection.**

## Inline FIZ Integration
BUSH supports inline [FIZ](https://github.com/jluchiji/fiz) scripts using a syntax like this:
```
bush> echo [[ (inc 1) ; This is a comment ]]
2
```
FIZ integration is powered by the newest version of the FIZ project that generates the static library version of FIZ with a minimalistic public API. BUSH repository includes a `fiz-src` submodule that builds into `libfiz.a`.

FIZ supported by the BUSH is a superset of the official syntax, with the following differences:

 - Unlike the official version, does not limit the number of function arguments and user-defined functions.
 - Supports environment variables via the `$` prefix.

### Inline FIZ Features

#### Environment Variables
Environment variables visible to the BUSH are also visible to its inline interpreter, which are accessible by prefixing variable names with a single `$`.
```
bush> setenv VAR 17
bush> echo [[ (inc $VAR) ; results in VAR + 1 ]]
18
```
Environment variables can be used anywhere a FIZ expression is used. Only environment variables with integer values are supported: attempting to use an environment variable with a string value in FIZ will result in error.
```
bush> setenv VAR string
bush> echo [[ (inc $VAR) ]]
-bush: fiz: Environment variable value not supported.
```
Attempting to reference an environment that does not exist will result in the same error as using an undefined variable.
```
bush> unsetenv VAR
bush> echo [[ (inc $VAR) ]]
-bush: fiz: Variable is not defined.
```

#### Multiline FIZ Scripts
Multiline FIZ scripts are supported. For example, you can do something like this:
```
bush> echo [[
import math.f

; x + y
(define (add x y)
  (ifz y x (add
             (inc x)
             (dec y)
           )
  )
)

; outputs 5
(add 2 3)
]]
5
```

#### Multiple FIZ Outputs
If your FIZ script has multiple outputs, then each one of them will be inserted as a separate argument.
```
bush> echo [[ (inc 1) (inc 2) ]]
2 3
```

#### FIZ Errors and `(halt)`
Any error in the FIZ script will print an error message and block the shell command from executing.
Invocation of `(halt)` in FIZ is considered an error, which will also block the shell command from executing.
```
bush> echo [[ (halt) ]]
-bush: fiz: Halted.
bush> echo [[ ))) ]]
-bush: fiz: Syntax error.
```


## Features

### Part 1
 - [x] Lexical Analysis
 - [x] Grammar Rules

### Part 2
 - [x] Simple Command Redirection
 - [x] File Redirection
 - [x] Pipes

### Part 3
 - [x] Ctrl-C
 - [x] Subshell
 - [x] `setenv`
 - [x] `printenv`
 - [x] `unsetenv`
 - [x] `cd [ dir ]`
 - [x] Allow Quotes
 - [x] File Globbing
 - [x] Tilde Expansion
 - [x] Zombie Processes
 - [x] Escape Characters
 - [x] Prompt only in TTY
 - [x] Environment Variable Expansion
 - [x] Link LibTTY (Linux Only)

### Part 4
 - [x] [FIZ](https://github.com/jluchiji/fiz) Integration

## License
### The MIT License

Copyright (c) 2015 Denis Luchkin-Zhou

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
