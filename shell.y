// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// shell.y                                                                   //
// This file contains grammar rules for the shell commands.                  //
//                                                                           //
// ------------------------------------------------------------------------- //

%token  <string_val> FIZ
%token	<string_val> WORD
%token  <string_val> SUBSHELL
%token  LF
%token  GT
%token  LT
%token  AMP
%token  GTGT
%token  PIPE
%token  GTAMP
%token  GTGTAMP
%token 	NOTOKEN

%union	{ char   *string_val; }

%{
  #include "main.hpp"
%}

%%

  goal:
    commands;

  commands:
    command | commands command;

  command:
    partials redirects background LF { parser -> execute();           } |
    error LF                         { yyerrok;                       } |
    LF                               { parser -> newline();           } ;

  // ----------------------------------------------------------------------- //
  // Partial command list                                                    //
  //                                                                         //
  //   cmd [arg]* [ | cmd [arg]* ]*                                          //
  //                                                                         //
  // ----------------------------------------------------------------------- //
  partials:
    partial | partial PIPE partials;

  // ----------------------------------------------------------------------- //
  // Partial command                                                         //
  //                                                                         //
  //   cmd [arg]*                                                            //
  //                                                                         //
  // ----------------------------------------------------------------------- //
  partial:
    cmd_word arguments               { parser -> partial_end();       } ;

  cmd_word:
    WORD                             { parser -> partial_make($1);    } ;

  // ----------------------------------------------------------------------- //
  // Argument list                                                           //
  //                                                                         //
  //   [arg]*                                                                //
  //                                                                         //
  // ----------------------------------------------------------------------- //
  arguments: argument arguments |;
  argument:
    WORD                             { parser -> partial_arg($1);     } |
    SUBSHELL                         { parser -> subshell_arg($1);    } |
    FIZ                              { parser -> fiz_arg($1);         } ;

  // ----------------------------------------------------------------------- //
  // IO Redirects                                                            //
  //                                                                         //
  //   [                                                                     //
  //     [>  filename]                                                       //
  //     [<  filename]                                                       //
  //     [>& filename]                                                       //
  //     [>> filename]                                                       //
  //     [>>& filename]                                                      //
  //   ]*                                                                    //
  //                                                                         //
  // ----------------------------------------------------------------------- //
  redirects: redirect redirects |;
  redirect:
    GT WORD                           { parser -> out_file($2, 0, 0); } |
    GTGT WORD                         { parser -> out_file($2, 0, 1); } |
    GTAMP WORD                        { parser -> out_file($2, 1, 0); } |
    GTGTAMP WORD                      { parser -> out_file($2, 1, 1); } |
    LT WORD                           { parser -> in_file ($2);       } ;

  // ----------------------------------------------------------------------- //
  // Background flag                                                         //
  //                                                                         //
  //   [&]                                                                   //
  //                                                                         //
  // ----------------------------------------------------------------------- //
  background:
    AMP                              { parser -> background();        } |
                                       /* DELIBERATELY LEFT EMPTY */    ;
%%
