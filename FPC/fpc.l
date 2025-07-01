%{
#include<math.h>
#include<ctype.h>

#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = read( fileno(yyin), (char *) buf, max_size )) < 0 ) \
            YY_FATAL_ERROR( "read() in flex scanner failed" ); \
        else \
          { \
           int i; \
           for(i=0;i<strlen(buf) ; i++ ) \
               buf[i]= toupper(buf[i]); \
          }

%}

UNDERSCORE  "_"
LETTRE [a-zA-Z]
CHIFFRE [0-9]
IDENTIFICATEUR ({LETTRE}|{UNDERSCORE})({LETTRE}|{CHIFFRE}|{UNDERSCORE})*
CONST_ENTIERE {CHIFFRE}+
CONST_REELLE {CHIFFRE}*(\.{CHIFFRE}*?([Ee][+\-]?{CHIFFRE}+)?)? 

%%
"{"[^"}"|\n]*"}"|\n       { int i;
                            fprintf(stderr,"\r\rCompiling Line...%d",nbligne); 
                            for(i=0;i<strlen(yytext);i++) 
                                if(yytext[i]=='\n') nbligne++;
                          }
" "+                      { }
"OR"                      { yylval.type=OR; return(OR); }
"AND"                     { yylval.type=AND; return(AND); }
"NOT"                     { yylval.type=NOT; return(NOT); }
":="                      { yylval.type=AFFECTATION; return(AFFECTATION); }
"<"                       { yylval.type=INFERIEUR; return(INFERIEUR); }
">"                       { yylval.type=SUPERIEUR; return(SUPERIEUR); }
"<="                      { yylval.type=INFERIEUR_EGAL; return(INFERIEUR_EGAL); }
">="                      { yylval.type=SUPERIEUR_EGAL; return(SUPERIEUR_EGAL); }
"="                       { yylval.type=EGALITE; return(EGALITE); }
"<>"                      { yylval.type=DIFFERENCE; return(DIFFERENCE); }
"+"                       { yylval.type=PLUS; return(PLUS); }
"-"                       { yylval.type=MOINS; return(MOINS); }
"*"                       { yylval.type=MULTIPLIER; return(MULTIPLIER); }
"/"                       { yylval.type=DIVISER; return(DIVISER); }
"%"                       { yylval.type=MODULO; return(MODULO); }
"INT"                     { yylval.type=TYPE_ENTIER; return(TYPE_ENTIER); }
"REAL"                    { yylval.type=TYPE_REAL; return(TYPE_REAL); }
"BOOL"                    { yylval.type=TYPE_BOOL; return(TYPE_BOOL); }
"BEGIN"                   { yylval.type=PBEGIN; return(PBEGIN); }
"END"                     { yylval.type=PEND; return(PEND);   }
"("                       { yylval.type=PARENTHESE_OUVERTE; return(PARENTHESE_OUVERTE); }
")"                       { yylval.type=PARENTHESE_FERMEE; return(PARENTHESE_FERMEE);  }
";"                       { yylval.type=FIN_INSTRUCTION; return(FIN_INSTRUCTION);   }
":"                       { yylval.type=DOUBLE_POINT; return(DOUBLE_POINT);   } 
","                       { yylval.type=VIRGULE; return(VIRGULE);        }
"."                       { yylval.type=POINT; return(POINT); }
"IF"                      { yylval.type=IF; return(IF);  }
"THEN"                    { yylval.type=THEN; return(THEN); }
"ELSE"                    { yylval.type=ELSE; return(ELSE); }
"GOTO"                    { yylval.type=GOTO; return(GOTO); }
"WHILE"                   { yylval.type=WHILE; return(WHILE); }
"PROGRAM"                 { yylval.type=PROGRAM; return(PROGRAM); }
"FUNCTION"                { yylval.type=FUNCTION; return(FUNCTION); }
"PROCEDURE"               { yylval.type=PROCEDURE; return(PROCEDURE); }
"VAR"                     { yylval.type=VAR; return(VAR);   }
"RETURN"                  { yylval.type=RETURN; return(RETURN); }
"WRITE"                   { yylval.type=VAR; return(WRITE); }
"WRITELN"                 { yylval.type=WRITELN; return(WRITELN); } 
"COS"                     { yylval.type=COS; return(COS); }
"SIN"                     { yylval.type=SIN; return(SIN); }
"LN"                      { yylval.type=LN; return(LN); }
"EXP"                     { yylval.type=EXP; return(EXP); }
"SQRT"                    { yylval.type=SQRT; return(SQRT); }
"ABS"                     { yylval.type=ABS; return(ABS); }
{CONST_ENTIERE}           {
                            strcpy(yylval.nom,yytext);
                            yylval.type = CONSTANTE_ENTIERE;
                            yylval.eval = atoi(yytext);
                            return(CONSTANTE);
                          }
{CONST_REELLE}            { 
                            strcpy(yylval.nom,yytext);  
                            yylval.type = CONSTANTE_REELLE;
                            yylval.fval = atof(yytext);
                            return(CONSTANTE);
                          }
{IDENTIFICATEUR}          { strcpy(yylval.nom,yytext);
                            yylval.type = IDENTIFICATEUR;
                            return(IDENTIFICATEUR); 
                          } 
%%
#undef yyunput
#undef yy_push_state
#undef yy_pop_state
#undef yy_top_state



