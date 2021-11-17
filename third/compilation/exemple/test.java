import java.io.*;

public class test {
	public static void main(String[] args) {
        
        System.out.println("#############################################################");
        System.out.println("#                         TITLE                             #");
        System.out.println("#############################################################");
        System.out.println("# Auteur :                                                  #");
        System.out.println("# Classe :                                                  #");
        System.out.println("#                                                           #");
        System.out.printf("# Fichier testé : %-41s #\n", args[0]);
        System.out.println("#############################################################");
        System.out.println("#                        Résultat                           #");
        System.out.println("#############################################################");
        
		try {
            
			FileReader myFile = new FileReader(args[0]);
			Lexer myTP = new Lexer(myFile);
			parser myP = new parser(myTP);
			try {myP.parse();}
			catch (Exception ignored) {
				System.out.printf("parse error");
			}
 		}
		catch(Exception ignored) {
			System.out.printf("invalid file");
        }
        
        System.out.println("#############################################################");
	}
}
