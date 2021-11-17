%%
/* options */
%class Lexi
%unicode
%line
%column
%standalone

/* models */
MOT=b(c?)(d?)((ab(c?)(d?))*)

%%
/* rules */
{MOT} { System.out.print("Action de la règle X->Y for " + yytext() + "\n");}
\n         {;}
.          {;}