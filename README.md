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


My take on writing the shell in C++, done as a part of CS252 Operating Systems
course at Purdue University.

Even though this shell used some scaffolding files provided by instructors,
I took liberty to clean up the code that I considered messy. I also implemented
the shell using the modular structure that I personally prefer. In addition to
that, the entire test suit provided by instructors has been revamped for better
user experience.

**TL;DR** This shell was written in a way that is significantly different from
what instructors expected.

## Building
You can build the shell by using:
```
$ make
```
There are also `debug` and `release` targets, that make the executable print
(or not to print) debug messages. You can fine tune the level of debugging
detail printed by specifying `DEBUG` flag like this:
```
$ make DEBUG=4
```
You can run the test suit by using the following (which will force a
release rebuild):
```
$ make test
```

## Inline FIZ Integration
BUSH supports inline [FIZ](https://github.com/jluchiji/fiz) scripts using a syntax like this:
```
bush> echo [[ (inc 1) ; This is a comment ]]
```

Multiline FIZ scripts are supported. For example, you can do something like this:
```
bush> echo [[

; x + y
(define (add x y)
  (ifz y
       x
      (add
        (inc x)
        (dec y)
      )
  )
)

; outputs 5
(add 2 3)

]]
```

If your FIZ script has multiple outputs, then each one of them will be inserted as a separate argument.
```
# This line will print "2 3"
bush> echo [[ (inc 1) (inc 2) ]]
```

Invocation of FIZ halt usually results in the exit code of 1, which is considered an error. Therefore
BUSH will refuse to execute you command if

**NOTE:** Depending on your FIZ interpreter, some functionality may not be available.

### Supply your own FIZ interpreter
 If you have your own FIZ interpreter that you wish to use, you can set the `FIZ` environment
 variable to the path of your FIZ interpreter. You can also supply a path relative to the directory
 where BUSH executable is located. Example:
```
 # Assuming that your FIZ interpreter is /bin/fiz
bush> setenv FIZ /bin/fiz
bush> echo [[ (dec 2) ]]
```

### Use my FIZ interpreter
 If you want to use my [FIZ interpreter](https://github.com/jluchiji/fiz), then you need to
 get the `fiz-src` submodule of this repository. You will need to do the following in the root
 directory of the BUSH repository:
```
$ git submodule init
$ git submodule update
```
After that, you can set up FIZ integration by simply:
```
$ make fiz
```
Which will build the FIZ interpreter and copy it to the BUSH repository root.

## Features

### Part 1
 - [x] Lex
 - [x] Yacc

### Part 2
 - [x] Simple Commands
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

 - [ ] Link LibTTY (Linux Only)

### Part 4
 - [ ] [FIZ](https://github.com/jluchiji/fiz) Integration

## License
### The MIT License

Copyright (c) 2015 Denis Luchkin-Zhou

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
