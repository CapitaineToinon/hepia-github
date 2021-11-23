
import java_cup.runtime.*;
import java.util.Vector;


%%
%class Lexer
%line
%column
%cup

%{
public void yyerror() {
      System.out.println("error line " +yyline + " column " +yycolumn
      + " " +yytext());
}
%}


nombre  = [0-9]+
comment = ( [^*] | \*+[^/*] )*

%%


"somme"  { return new Symbol(sym.SOMME); }
","      { return new Symbol(sym.VIRGULE); }
"."      { return new Symbol(sym.POINT); }
"$"      { return new Symbol(sym.FIN); }


{nombre}     { return new Symbol(sym.NOMBRE, new Integer(yytext())); }

/* -------------------------------------------------
	Caracteres non pris en compte
   ------------------------------------------------- */

[\ |\t|\n|\r|\r\n]                  {}
