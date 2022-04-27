import java.io.FileReader;
import java_cup.runtime.*;

public class App {
	public static void main(String[] args) throws Exception {
		if (args.length < 1) {
			throw new Exception("args empty");
		}

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

		FileReader myFile;
		Lexer myTP;
		parser myP;

		try {
			myFile = new FileReader(args[0]);
		} catch (Exception e) {
			System.out.println("[test.java] new FileReader failed");
			throw e;
		}

		try {
			myTP = new Lexer(myFile);
		} catch (Exception e) {
			System.out.println("[test.java] new Lexer failed");
			throw e;
		}

		try {
			myP = new parser(myTP);
		} catch (Exception e) {
			System.out.println("[test.java] new parser failed");
			throw e;
		}

		try {
			myP.parse();
		} catch (Exception e) {
			System.out.println("[test.java] myP.parse() failed");
			throw e;
		}

		System.out.println("#############################################################");
	}
}
