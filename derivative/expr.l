%{
#include "expr.tab.h"
#include <stdio.h>
%}

%%
[0-9]+              { printf("TOKEN: NUMBER(%s)\n", yytext); yylval.num = atoi(yytext); return NUMBER; }
"x"                 { printf("TOKEN: VARIABLE\n"); return VARIABLE; }
"sin"               { printf("TOKEN: SIN\n");  return SIN; }
"tan"		    {  printf("TOKEN: TAN\n"); return TAN; }
"cos"               { printf("TOKEN: COS\n"); return COS; }
"exp"               { printf("TOKEN: EXP\n"); return EXP; }
"ln"               { printf("TOKEN: LN\n"); return LN; }
"+"                 { printf("TOKEN: +\n"); return '+'; }
"-"                 { printf("TOKEN: -\n"); return '-'; }
"*"                 { printf("TOKEN: *\n"); return '*'; }
"/"                 { printf("TOKEN: /\n"); return '/'; }
"("                 { printf("TOKEN: (\n"); return '('; }
")"                 { printf("TOKEN: )\n"); return ')'; }
[ \t\n\r]+          { printf("TOKEN: WHITESPACE\n"); /* skip whitespace */ }
.                   { printf("TOKEN: OTHER(%c)\n", yytext[0]); return yytext[0]; }
%%
