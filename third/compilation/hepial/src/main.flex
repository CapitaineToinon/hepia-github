%%
/* options */
%class Lexer
%unicode
%line
%column
%standalone
%cup

%{
public void yyerror() {
  System.out.println("[jflex] error line " + yyline + " column " + yycolumn + " " + yytext());
}

public void syntax_error(java_cup.runtime.Symbol s){
  System.out.println("[jflex] compiler has detected a syntax error at line " + s.left 
    + " column " + s.right);
}
%}

/* models */
number = [0-9]+

%%
/* rules */
{number} {
  System.out.println("[jflex] Found the number " + yytext());
  return new java_cup.runtime.Symbol(sym.NUMBER, Integer.parseInt(yytext()));
}
"+" { 
  System.out.println("[jflex] Found a plus sign");
  return new java_cup.runtime.Symbol(sym.PLUS_SIGN);
}
"-" { 
  System.out.println("[jflex] Found a minus sign");
  return new java_cup.runtime.Symbol(sym.MINUS_SIGN);
}
"*" { 
  System.out.println("[jflex] Found a mupltiply sign");
  return new java_cup.runtime.Symbol(sym.MULTIPLY_SIGN);
}
"/" { 
  System.out.println("[jflex] Found a divide sign");
  return new java_cup.runtime.Symbol(sym.DIVIDE_SIGN);
}

/* ignored */
[\ |\t|\n|\r|\r\n] {}
