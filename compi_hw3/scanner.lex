%{
#include "hw3_output.hpp"
#include "SymbolTable.h"
#include "parser.tab.hpp"
%}

%option noyywrap
%option yylineno
letter          ([a-zA-Z])
digit           ([0-9])
nonZeroDigit    ([1-9])
zero            ([0])

id      		([a-zA-Z0-9])
printable       ([ !#-\[\]-~ ])
escseq          ([\\ntr\"0])
whitespace      ([\n\t\r ])

%%
int  yylval = new Object(yytext); return INT;
byte yylval = new Object(yytext); return BYTE;
b    yylval = new Object(yytext); return B;
bool yylval = new Object(yytext); return BOOL;
not  {yylval = new Object(yytext); return NOT;}
or   {yylval = new Object(yytext); return OR;}
and  {yylval = new Object(yytext); return AND;}
true {yylval = new Object(yytext); return TRUE;}
false {yylval = new Object(yytext); return FALSE;}
return {yylval = new Object(yytext); return RETURN;}
continue  {yylval = new Object(yytext); return CONTINUE;}
break  {yylval = new Object(yytext); return BREAK;}
while  {yylval = new Object(yytext); return WHILE;}
else  {yylval = new Object(yytext); return ELSE;}
if   {yylval = new Object(yytext); return IF;}
;    {yylval = new Object(yytext); return SC;}
\(   {yylval = new Object(yytext); return LPAREN;}
\)   {yylval = new Object(yytext); return RPAREN;}
\{   {yylval = new Object(yytext); return LBRACE;}
\}   {yylval = new Object(yytext); return RBRACE;}
=    {yylval = new Object(yytext); return ASSIGN;}
\==|\!=     {yylval = new Object(yytext); return RELOPEQUAL;}
\<|\>|\<=|\>= {yylval = new Object(yytext); return RELOPUNEQUAL; }
\+|\-    {yylval = new Object(yytext); return PLUSMINUS;}
\*|\/    {yylval = new Object(yytext); return MULDIV;}
{letter}{id}*  {yylval = new Object(yytext); return ID;}
([1-9]+{digit}*)|0  {yylval = new Object(yytext); return NUM;}
\"([^\n\r\"\\]|\\[rnt"\\])+\"	{yylval = new Object(yytext); return STRING;}
\/\/[^\n\r]*[\r|\n|\r\n]?                        		;
{whitespace}                                                   ;
.  								{output::errorLex(yylineno); exit(0);}                                                             
