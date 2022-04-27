SRC=src
BIN=bin
LIB=lib

JAVA=java
JAVAC=javac
JFLEX=jflex
JAVACUP=$(LIB)/java-cup-11a.jar
CLASSPATH=$(JAVACUP):.

FILE_FLEX=$(SRC)/main.flex
FILE_CUP=$(SRC)/main.cup
FILE_JAVA_NAME=Lexer
FILE_TEST_PRG_NAME=test

TEST_CLASS=test

FILE=test.txt
ifdef TESTFILE
	FILE=$(TESTFILE)
endif

all: $(FILE) sym.class parser.class $(FILE_JAVA_NAME).class

$(FILE_JAVA_NAME).java : $(FILE_FLEX)
	$(JFLEX) $(FILE_FLEX)

sym.java parser.java: $(FILE_CUP)
	$(JAVA) -jar $(JAVACUP) -destdir $(SRC) $(FILE_CUP)

%.class : %.java
	cd $(BIN) && $(JAVAC) -d . -classpath ../$(CLASSPATH) ../$(SRC)/$<

clean :
	rm -f $(BIN)/* $(SRC)/parser.java $(SRC)/sym.java $(SRC)/$(FILE_JAVA_NAME).java $(SRC)/$(FILE_JAVA_NAME).java~
