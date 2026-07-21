%{
#include "expr.tab.h"
#include <stdio.h>
%}

%%
[0-9]+              { yylval.num = atoi(yytext); return NUMBER; }
"x"                 { return VARIABLE; }
"sin"               { return SIN; }
"tan"		    { return TAN; }
"cos"               { return COS; }
"exp"               { return EXP; }
"ln"               { return LN; }
"log"               { return LOG; }
"+"                 { return '+'; }
"-"                 { return '-'; }
"*"                 { return '*'; }
"/"                 { return '/'; }
"^"                 { return POW; }
"("                 { return '('; }
")"                 { return ')'; }
[ \t\n\r]+          { /* skip whitespace */ }
.                   { return yytext[0]; }
%%
