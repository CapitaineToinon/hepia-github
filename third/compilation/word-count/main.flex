import java.util.HashMap;
import java.util.HashSet; 
import java.util.Map;

%%
/* options */
%class Lexi
%unicode
%line
%column
%standalone

%{
Integer NB_LINE = 1;
HashMap<String, Integer> words = new HashMap<String, Integer>();
HashMap<String, HashSet<Integer>> lines = new HashMap<String, HashSet<Integer>>();

public void yyerror() {
  System.out.println("error line " +yyline + " column " +yycolumn+ " " +yytext());
}
%}


%{eof
  for (Map.Entry<String, Integer> word : words.entrySet()) {
    HashSet<Integer> wordLines = lines.get(word.getKey());
    System.out.printf("%s %d %s\n", word.getKey(), word.getValue(), wordLines.toString());
  }
%eof}

/* models */
MOT=[a-z|']+

%%
/* rules */
{MOT}      { 
  String word = yytext();
  // add word count
  Integer count = words.getOrDefault(word, 0);
  words.put(word, count + 1);

  // save line number
  HashSet<Integer> wordLines = lines.getOrDefault(word, new HashSet<Integer>());
  wordLines.add(NB_LINE);
  lines.put(word, wordLines);
}
 
\n         { 
  NB_LINE++;
}
.          {;}
