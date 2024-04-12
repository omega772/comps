#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char int2FloatStr[64] = "Int2Float";

enum tokenType {
	null,
    integer,
    real,
    var,
    floatVar,
    bracket1,
    bracket2,
    action,
    Int2Float
};

//char Bracket[2] = {'(', ')'};

/*const char* BracketStr[] = {
	"открывающая скобка",
	"закрывающая скобка",
};*/

char Bracket1[1] = {'('};

char Bracket2[1] = {')'};

char TypeBracket1[1] = {'['};

char TypeBracket2[1] = {']'};

char Actions[4] = {'+', '-', '*', '/'};
int ActionWeight[4] = {1, 1, 2, 2};
char ActionText[4][4] = {
	{'a','d','d','\0'},
	{'s','u','b','\0'},
	{'m','u','l','\0'},
	{'d','i','v','\0'}
};

const char* ActionsStr[] = {
	"сложение",
	"вычитание",
	"умножение",
	"деление",
};

char SpecialVar[1] = {'_'}; 

char Letters[52] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

char Points[1] = {'.'};

char Ignorable[4] = {' ', '	', '\n', EOF};

bool containedIn(char ch, char Arr[], int arrSize)
{
	for(int i=0; i<arrSize; i++)
	{
		if (ch == Arr[i])	
			return true;
	}
	return false;
}

bool isTypebracket1(char ch)
{
	return containedIn(ch, TypeBracket1, 1);
}

bool isTypebracket2(char ch)
{
	return containedIn(ch, TypeBracket2, 1);
}

bool isbracket1(char ch)
{
	return containedIn(ch, Bracket1, 1);
}

bool isbracket2(char ch)
{
	return containedIn(ch, Bracket2, 1);
}

bool isaction(char ch)
{
	return containedIn(ch, Actions, 4);
}

bool isvar(char ch)
{
	return (containedIn(ch, Letters, 52) || containedIn(ch, SpecialVar, 1) || isdigit(ch));
}

bool isreal(char ch)
{
	return (containedIn(ch, Points, 1) || isdigit(ch));
}

bool isignorable(char ch)
{
	return containedIn(ch, Ignorable, 4);
}

int strLen(char text[], int maxLen)
{
	int textLen = maxLen;
	for(int i=0; i<textLen; i++)
	{
		if (text[i] == '\0')
			textLen = i;
	}
	return textLen;
}

bool isZero(char text[64])
{
	int textLen = strLen(text, 64);
	for(int i=0; i<=textLen; i++)
	{
		if ((text[i] != '0') && (text[i] != '.') && (text[i] != '\0'))
			return false;
	}
	return true;
}

/*char* translatBracket(char ch)
{
	for(int i=0; i<2; i++)
	{
		if (ch == Bracket[i])
		{
			return BracketStr[i];
		}
	}
}*/

char* translatActions(char ch)
{
	for(int i=0; i<4; i++)
	{
		if (ch == Actions[i])
		{
			return ActionsStr[i];
		}
	}
}

int pointF(char *str, int strLen)
{
	for(int i=0; i<strLen; i++)
	{
		if (str[i] == '\0')
		{
			return -1;
		}
		for(int j=0; j<1; j++)
		{
			if (str[i] == Points[j])
			{
				return i;
			}
		}
	}
	return -1;
}

void drop_left(char *s, int n)
{
   char* s2 = s + n;
   while ( *s2 )
   {
      *s = *s2;
      ++s;
      ++s2;
   }
   *s = '\0';
}

int isIntType(enum tokenType type)
{
	if (type == integer)
		return true;
	if (type == var)
		return true;
	return false;
}

int isRealType(enum tokenType type)
{
	if (type == real)
		return true;
	if (type == floatVar)
		return true;
	if (type == Int2Float)
		return true;
	return false;
}

void printErrorCharacter(char ch, int charPos)
{
	printf("Lexical error! Invalid character \"%c\" at position %d\n", ch, charPos);
}

// возвращает tokenType к которому следует отнести символ, если встречается символ
//обозначающий неправильное изначальное определение токена, то меняется изначальное определние.
enum tokenType TSymbolAnalysis(char ch, int charPos, enum tokenType *curTT, int *typeBracketCounter) 
{
	if (isTypebracket1(ch))
	{
		(*typeBracketCounter) = (*typeBracketCounter) + 1;
		return *curTT;
	}
	
	if (isTypebracket2(ch))
	{
		(*typeBracketCounter) = (*typeBracketCounter) - 1;
		return *curTT;
	}
	
	if ((*typeBracketCounter) > 0)
		return *curTT;
	
	if (isbracket1(ch))
		return bracket1;
	
	if (isbracket2(ch))
		return bracket2;
		
	if (isaction(ch))
		return action;
	
	if (*curTT != var)
	{
		if (*curTT != real)
			if (isdigit(ch))
				return integer;
		
		if (isreal(ch))
		{
			if (*curTT == integer)
				*curTT = real;
			return real;
		}
	}
	
	if (isvar(ch))
	{
		if ((*curTT == integer) || (*curTT == real))
			*curTT = var;
		return var;
	}
	
	printErrorCharacter(ch, charPos);
	return null;	
}

int varCount = 0;
char Vars[64][64];
enum tokenType VarsType[64];

void addVar(char var[64], enum tokenType varType)
{
	strncpy(Vars[varCount], var, 64);
	VarsType[varCount] = varType;
	varCount = varCount + 1;
}

int FVar(char *var)
{
	for(int i=0; i<varCount; i++)
	{
		if (strncmp(Vars[i], var, 1) == 0)
		{
			return i;
		}
			
	}
	return -1;
}

struct tokenStract {
    enum tokenType type;
    char token[64];
    int tokenPos;
};

struct tokenStract allToken[1024];
int allTokenCount = 0;

enum tokenType tokenTypeHandler(char token[64], char tokenTypeInfo[8], enum tokenType curTT)
{
	switch (curTT)
	{
		case var:
			printf("");
			int varId = FVar(token);
			if (varId >= 0)
				return VarsType[varId];
			
			if ((strcmp(tokenTypeInfo, "f") == 0) || (strcmp(tokenTypeInfo, "F") == 0))
				return floatVar;
				
			if ((strcmp(tokenTypeInfo, "i") == 0) || (strcmp(tokenTypeInfo, "I") == 0))
				return var;
	}
	return curTT;
}

void tokenHandler(enum tokenType fCurTT, char token[64], char tokenTypeInfo[8], int tokenPos, FILE *Ftokens, FILE *Fsymbols, int hasOut)
{
	enum tokenType curTT = tokenTypeHandler(token, tokenTypeInfo, fCurTT);
	
	switch (curTT)
	{
		case null:
			break;
		case integer:
			if (hasOut == 1)
				fprintf(Ftokens, "<%s> - константа целого типа\n", token);
			//printf("<%s> - константа целого типа\n", token);
			break;
		case real:
			if (hasOut == 1)
				fprintf(Ftokens, "<%s> - константа вещественного типа\n", token);

			char tokenClon1[64] = "";
			strncpy(tokenClon1, token, 64);
			int Ppos1 = pointF(tokenClon1, 64);
			int ofset1 = Ppos1+1;
			drop_left(tokenClon1, Ppos1+1);
			Ppos1 = pointF(tokenClon1, 64);
			while (Ppos1 >= 0)
			{
				ofset1 = ofset1 + Ppos1+1;
				printf("Lexical error! constant is set incorrectly \"%s\" at position %d \n", token, tokenPos+ofset1);
				drop_left(tokenClon1, Ppos1+1);
				Ppos1 = pointF(tokenClon1, 64);
			} 
			
			//printf("<%s> - константа вещественного типа\n", token);
			break;
		case var:
			if (FVar(token) < 0)
			{
				addVar(token, curTT);
				if (hasOut == 1)
					fprintf(Fsymbols, "%d - %s[целый]\n", FVar(token)+1, token);
			}
			
			if (hasOut == 1)
				fprintf(Ftokens, "<%s,%d> - идентификатор с именем %s целого типа\n", token, FVar(token)+1, token);
			
			if (isdigit(token[0]))
				printf("Lexical error! identifier \"%s\" cannot start with digits at position %d\n", token, tokenPos);
				
			char tokenClon[64] = "";
			strncpy(tokenClon, token, 64);
			int Ppos = pointF(tokenClon, 64);
			int ofset = 0;
			while (Ppos >= 0)
			{
				ofset = ofset + Ppos+1;
				printf("Lexical error! identifier \"%s\" must not contain dots at position %d \n", token, tokenPos+ofset);
				drop_left(tokenClon, Ppos+1);
				Ppos = pointF(tokenClon, 64);
			} 
			
			//printf("<%s,%d> - идентификатор с именем %s\n", token, varCount, token);
			break;
		case floatVar:
			if (FVar(token) < 0)
			{
				addVar(token, curTT);
				if (hasOut == 1)
					fprintf(Fsymbols, "%d - %s[вещественный]\n", FVar(token)+1, token);
			}
			
			if (hasOut == 1)
				fprintf(Ftokens, "<%s,%d> - идентификатор с именем %s вещественного типа\n", token, FVar(token)+1, token);
			
			if (isdigit(token[0]))
				printf("Lexical error! identifier \"%s\" cannot start with digits at position %d\n", token, tokenPos);
				
			char tokenClon2[64] = "";
			strncpy(tokenClon2, token, 64);
			int Ppos2 = pointF(tokenClon2, 64);
			int ofset2 = 0;
			while (Ppos2 >= 0)
			{
				ofset2 = ofset2 + Ppos2+1;
				printf("Lexical error! identifier \"%s\" must not contain dots at position %d \n", token, tokenPos+ofset2);
				drop_left(tokenClon2, Ppos2+1);
				Ppos2 = pointF(tokenClon2, 64);
			} 
			
			//printf("<%s,%d> - идентификатор с именем %s\n", token, varCount, token);
			break;
		case bracket1:
			if (hasOut == 1)
				fprintf(Ftokens, "<%s> - открывающая скобка\n", token);
			//printf("<%s> - %s\n", token, translatBracket(token[0]));
			break;
		case bracket2:
			if (hasOut == 1)
				fprintf(Ftokens, "<%s> - закрывающая скобка\n", token);
			//printf("<%s> - %s\n", token, translatBracket(token[0]));
			break;
		case action:
			if (hasOut == 1)
				fprintf(Ftokens, "<%s> - операция %s\n", token, translatActions(token[0]));
			//printf("<%s> - операция %s\n", token, translatActions(token[0]));
			break;
	}
	strncpy(allToken[allTokenCount].token, token, 64);
	allToken[allTokenCount].type = curTT;
	allToken[allTokenCount].tokenPos = tokenPos;
	//printf("allToken[%d] = %s / %d \n",allTokenCount, allToken[allTokenCount].token, allToken[allTokenCount].type);
	allTokenCount = allTokenCount + 1;
}

int lexicalAnalysis(char f1Name[], char f2Name[], char f3Name[], int hasOut)
{
	FILE *S1, *S2, *S3;
	char ch;
	system("chcp 1251");
	system("cls");
	S1 = fopen(f1Name, "r");
	S2 = fopen(f2Name, "w");
	S3 = fopen(f3Name, "w");
	
	enum tokenType curTT = null;
	enum tokenType AnalysisTT = null;
	
	int charPos = 0;
	int tokenLen = 0;
	char token[64] = "";
	
	int typeBracketCounter = 0;
	char tokenTypeInfo[8] = "";
	int tokenTypeInfoLen = 0;
	
	do {
		ch = fgetc(S1);
		if (!(isignorable(ch)))
		{
			AnalysisTT = TSymbolAnalysis(ch, charPos, &curTT, &typeBracketCounter);
			if (curTT != null)
				if ((curTT != AnalysisTT) || (curTT == bracket1) || (curTT == bracket2) || (curTT == action))
				{
					tokenHandler(curTT, token, tokenTypeInfo, charPos-tokenLen, S2, S3, hasOut);
					tokenLen = 0;
					tokenTypeInfoLen = 0;
					memset(token,0,64);
					memset(tokenTypeInfo,0,8);
				}
			
			if (typeBracketCounter > 0)
			{
				if (!(isTypebracket1(ch)))
				{
					tokenTypeInfo[tokenTypeInfoLen] = ch;
					tokenTypeInfoLen = tokenTypeInfoLen+1;
				}
			}
			else
			{
				if (!(isTypebracket2(ch)))
				{
					token[tokenLen] = ch;
					tokenLen = tokenLen + 1;
				}
			}
			
			curTT = AnalysisTT;
		}
		
		charPos = charPos + 1;
	} while (ch != EOF);
	
	tokenHandler(curTT, token, tokenTypeInfo, charPos-tokenLen, S2, S3, hasOut);
	
	fclose(S1);
	fclose(S2);
	fclose(S3);
	return 0;
}

struct syntaxTreePart {
	struct tokenStract field;
	int bracketCount;
	struct syntaxTreePart *left;
	struct syntaxTreePart *right;
	struct syntaxTreePart *up;
};

struct syntaxTreePart *newSyntTreePart(struct tokenStract field, int unclosedBracketCount)
{
	//printf("new tree part: %s\n", field.token);
	struct syntaxTreePart *tree;
	tree = (struct syntaxTreePart*) malloc(sizeof(struct syntaxTreePart));
	(*tree).field = field; 
	(*tree).left = NULL;
	(*tree).right = NULL;
	(*tree).up = NULL;
	(*tree).bracketCount = unclosedBracketCount;
	//printf("adress: %d\n", tree);
	return tree;
}

struct syntaxTreePart *SyntaxTree;

int getActionWeight(char action)
{
	for(int i=0; i<4; i++)
	{
		if (action == Actions[i])	
			return ActionWeight[i];
	}
	return 0;
}

char* getActionText(char action)
{
	for(int i=0; i<4; i++)
	{
		if (action == Actions[i])	
			return ActionText[i];
	}
	return " ";
}

struct syntaxTreePart *syntTreePartToFloat(struct syntaxTreePart *lastSyntTreePart)
{
	struct tokenStract int2FloatField;
	int2FloatField.type = Int2Float;
	strcpy(int2FloatField.token, int2FloatStr);
	int2FloatField.tokenPos = (*lastSyntTreePart).field.tokenPos;
	
	struct syntaxTreePart *tree;
	tree = newSyntTreePart(int2FloatField, (*lastSyntTreePart).bracketCount);
	(*tree).left = lastSyntTreePart;
	
	(*lastSyntTreePart).up = tree;
	
	return tree;
}

// если дейсвие в элементе дерева A приорететнее чем действие в элементе B то вернёт 1, если наоборот то -1, а если ранвы то 0.
int comparTreeActions(struct syntaxTreePart *treePartA, struct syntaxTreePart *treePartB)
{
	if ((*treePartA).bracketCount > (*treePartB).bracketCount)
	{
		return 1;
	}
	else
	{
		if ((*treePartA).bracketCount < (*treePartB).bracketCount)
		{
			return -1;
		}
		else
		{
			int temp = getActionWeight((*treePartA).field.token[0]) - getActionWeight((*treePartB).field.token[0]);
			if (temp > 0)
				return 1;
			if (temp < 0)
				return -1;
			return 0;
		}
	}
}

int addChildInTree(struct syntaxTreePart *lastTreePart, struct syntaxTreePart *newTreePart)
{
	if ((*lastTreePart).left == NULL)
	{
		(*lastTreePart).left = newTreePart;
	}
	else
	{
		(*lastTreePart).right = newTreePart;
	}
	(*newTreePart).up = lastTreePart;
	return 0;
}

int addIntoTreeInstead(struct syntaxTreePart *lastTreePart, struct syntaxTreePart *newTreePart)
{
	(*newTreePart).left = (*lastTreePart).left;
	(*newTreePart).right = (*lastTreePart).right;
	(*newTreePart).up = (*lastTreePart).up;
	if ((*lastTreePart).left != NULL)
	{
		(*(*lastTreePart).left).up = newTreePart;
	}
	if ((*lastTreePart).right != NULL)
	{
		(*(*lastTreePart).right).up = newTreePart;
	}
	if ((*lastTreePart).up != NULL)
	{
		if ((*(*lastTreePart).up).left == lastTreePart)
			(*(*lastTreePart).up).left = newTreePart;
		if ((*(*lastTreePart).up).right == lastTreePart)
			(*(*lastTreePart).up).right = newTreePart;
	}
}

void parsing()
{	
	struct tokenStract previousField;
	previousField.type = null;
	
	int unclosedBracketCount;
	int unclosedBracketPos[1024];
	
	struct tokenStract tokenNull;
	tokenNull.type = null;
	tokenNull.tokenPos = 0;
	
	SyntaxTree = newSyntTreePart(tokenNull, 0);
	
	struct syntaxTreePart *lastTreePart = SyntaxTree;
	
	for(int i=0; i<allTokenCount; i++)
	{
		switch(allToken[i].type)
		{
			case null:
				break;
			case integer:
				if ((previousField.type != action) && (previousField.type != bracket1) && (previousField.type != null))
					printf("Syntax error! An operator was expected after <%s> at position %d.\n", previousField.token, previousField.tokenPos);
				addChildInTree(lastTreePart, newSyntTreePart(allToken[i], unclosedBracketCount));
				break;
			case real:
				if ((previousField.type != action) && (previousField.type != bracket1) && (previousField.type != null))
					printf("Syntax error! An operator was expected after <%s> at position %d.\n", previousField.token, previousField.tokenPos);
				addChildInTree(lastTreePart, newSyntTreePart(allToken[i], unclosedBracketCount));
				break;
			case var:
				if ((previousField.type != action) && (previousField.type != bracket1) && (previousField.type != null))
					printf("Syntax error! An operator was expected after <%s> at position %d.\n", previousField.token, previousField.tokenPos);
				addChildInTree(lastTreePart, newSyntTreePart(allToken[i], unclosedBracketCount));
				break;
			case floatVar:
				if ((previousField.type != action) && (previousField.type != bracket1) && (previousField.type != null))
					printf("Syntax error! An operator was expected after <%s> at position %d.\n", previousField.token, previousField.tokenPos);
				addChildInTree(lastTreePart, newSyntTreePart(allToken[i], unclosedBracketCount));
				break;
			case bracket1:
				unclosedBracketPos[unclosedBracketCount] = allToken[i].tokenPos;
				unclosedBracketCount = unclosedBracketCount + 1;
				if ((previousField.type != action) && (previousField.type != bracket1) && (previousField.type != null))
					printf("Syntax error! An operator was expected after <%s> at position %d.\n", previousField.token, previousField.tokenPos);
				break;
			case bracket2:
				unclosedBracketCount = unclosedBracketCount - 1;
				if ((previousField.type == action))
					printf("Syntax error! The <%s> operation at position %d has no operand.\n", allToken[i].token, allToken[i].tokenPos);
				break;
			case action:
				if ((previousField.type == action) || (previousField.type == bracket1))
					printf("Syntax error! The <%s> operation at position %d has no operand.\n", allToken[i].token, allToken[i].tokenPos);
				struct syntaxTreePart *newTreePart = newSyntTreePart(allToken[i], unclosedBracketCount);
				switch((*lastTreePart).field.type)
				{
					case null:
						addIntoTreeInstead(lastTreePart, newTreePart);
						lastTreePart = newTreePart;
						SyntaxTree = lastTreePart;
						break;
					case action:
						printf(""); //это очень важный printf! его нельзя удалять и комментировать иначе будет ошибка т.к. внутри обявлееного case нельзя сразу же обявить переменную из-за особенности работы лэйблов
						int compar = comparTreeActions(lastTreePart, newTreePart);
						int isUpNull = 0;
						while (compar >= 0)
						{
							if ((*lastTreePart).up != NULL)
							{
								lastTreePart = (*lastTreePart).up;
								compar = comparTreeActions(lastTreePart, newTreePart);
							}
							else
							{
								compar = -1;
								isUpNull = 1;
								SyntaxTree = newTreePart;
								(*newTreePart).left = lastTreePart;
								lastTreePart = newTreePart;
							}
						}
						if (isUpNull == 0)
						{
							(*newTreePart).up = lastTreePart;
							if ((*lastTreePart).right != NULL)
								(*newTreePart).left = (*lastTreePart).right;
							(*lastTreePart).right = newTreePart;
							lastTreePart = newTreePart;
						}
						break;
				}
				break;
		}
		//отладочка такая вот, да XD
		/*printf("last tree part (%d) \"%s\"", lastTreePart, (*lastTreePart).field.token);
		if ((*lastTreePart).left != NULL)
			printf("| left %s ", (*(*lastTreePart).left).field.token);
		if ((*lastTreePart).right != NULL)
			printf("| right %s ", (*(*lastTreePart).right).field.token);
		printf("\n");*/
		
		previousField = allToken[i];
	}
	
	if (previousField.type == action)
		printf("Syntax error! An operator was expected after <%s> at position %d.\n", previousField.token, previousField.tokenPos);
	for(int i=0; i<unclosedBracketCount; i++)
		printf("Syntax error! The opening bracket at position %d does not have a closing bracket.\n", unclosedBracketPos[i]);
}   

enum tokenType defineMainType(struct syntaxTreePart *treePart)
{
	if ((*treePart).field.type == action)
	{
		if ((*treePart).field.token[0] == '/')
		{	
			if (isZero((*(*treePart).right).field.token))
				printf("Semantic error! division by zero is encountered at %d\n", (*(*treePart).right).field.tokenPos);
		}
	
		enum tokenType leftArg = defineMainType((*treePart).left);
		enum tokenType rightArg = defineMainType((*treePart).right);
		if (leftArg == rightArg)
			return leftArg;
		if ((leftArg == integer) && (rightArg == real))
		{
			struct syntaxTreePart *newPartToFloat = syntTreePartToFloat((*treePart).left);
			(*(*treePart).left).up = newPartToFloat;
			(*treePart).left = newPartToFloat;
			return real;
		}
		if ((leftArg == real) && (rightArg == integer))
		{
			struct syntaxTreePart *newPartToFloat = syntTreePartToFloat((*treePart).right);
			(*(*treePart).right).up = newPartToFloat;
			(*treePart).right = newPartToFloat;
			return real;
		}
	}
	if (isIntType((*treePart).field.type))
		return integer;
	if (isRealType((*treePart).field.type))
		return real;
}

void typeConversion()
{
	defineMainType(SyntaxTree);
}

int printInPos(char syntaxTreeText[1024][1024], int strNum, int offset, char text[64])
{
	int textLen = strLen(text, 64);
	for(int i=0; i<=offset; i++)
		if (syntaxTreeText[strNum][i] == '\0')
			syntaxTreeText[strNum][i] = ' ';
	for(int i=0; i<=textLen; i++)
		syntaxTreeText[strNum][offset+i] = text[i];
	//printf("y = %d x = %d | %s\n", strNum, offset, text);
	return textLen;
}

int printTokenInPos(char syntaxTreeText[1024][1024], int strNum, int offset, struct tokenStract token)
{
	char tokenText[64];
	int tokenLen = strLen(token.token, 60);
	tokenText[0] = '<';
	for(int i=0; i<tokenLen; i++)
		tokenText[1+i] = token.token[i];
	tokenText[tokenLen+1] = '>';
	tokenText[tokenLen+2] = '\0';
	return printInPos(syntaxTreeText, strNum, offset, tokenText);
}

void printSyntaxTreePart(char syntaxTreeText[1024][1024], int strNum, int *offset, struct syntaxTreePart *treePart)
{
	int curOffset = strLen(syntaxTreeText[strNum], 1024);
	for (int i=curOffset; i<(*offset); i++)
		syntaxTreeText[strNum][i] = '-';
	
	if ((*treePart).left != NULL)
		printInPos(syntaxTreeText, strNum+2, (*offset), " |");
	if (((*treePart).right != NULL) || ((*treePart).left != NULL))
		printInPos(syntaxTreeText, strNum+1, (*offset), " |");
	
	(*offset) = (*offset) + printTokenInPos(syntaxTreeText, strNum, (*offset), (*treePart).field);
	
	if ((*treePart).left != NULL)
		printSyntaxTreePart(syntaxTreeText, strNum+2, offset, (*treePart).left);

	if ((*treePart).right != NULL)
		printSyntaxTreePart(syntaxTreeText, strNum+1, offset, (*treePart).right);

}

void printSyntaxTree(fileName)
{
	char syntaxTreeText[1024][1024];
	for(int i=0; i<1024; i++)
		memset(syntaxTreeText[i], '\0', sizeof syntaxTreeText[i]);
	int offset = 0;
	
	FILE *S1;
	/*system("chcp 1251");
	system("cls");*/
	S1 = fopen(fileName, "w");
	
	offset = offset + printTokenInPos(syntaxTreeText, 0, 0, (*SyntaxTree).field);
	printInPos(syntaxTreeText, 1, 0, " |");
	printInPos(syntaxTreeText, 2, 0, " |");
	
	printSyntaxTreePart(syntaxTreeText, 2, &offset, (*SyntaxTree).left);
	printSyntaxTreePart(syntaxTreeText, 1, &offset, (*SyntaxTree).right);

	for(int i=0; i<1024; i++)
		fprintf(S1, "%s\n", syntaxTreeText[i]);
	
	fclose(S1);
}

/*
int varCount = 0;
char Vars[64][64];
enum tokenType VarsType[64];
*/

struct tokenStract opt2Helper(struct syntaxTreePart *treePart)
{
	if ((*treePart).field.type == Int2Float)
	{
		struct tokenStract left = opt2Helper((*treePart).left);
		if (left.type == integer)
		{
			int len = strLen(left.token, 62);
			left.token[len] = '.';
			left.token[len+1] = '0';
			left.type = real;
			(*treePart).field = left;
		}
	}
	if ((*treePart).field.type == action)
	{
		opt2Helper((*treePart).left);
		opt2Helper((*treePart).right);
	}
	return (*treePart).field;
}

struct tokenStract opt1Helper(struct syntaxTreePart *treePart)
{
	if ((*treePart).field.type == action)
	{
		struct tokenStract left = opt1Helper((*treePart).left);
		struct tokenStract right = opt1Helper((*treePart).right);
		
		if ((left.type == var) || (left.type == floatVar) || (right.type == var) || (right.type == floatVar))
		{
			if ((*(*treePart).left).field.type != action && (*(*treePart).left).field.type != Int2Float)
				(*(*treePart).left).field = left;
			if ((*(*treePart).right).field.type != action && (*(*treePart).right).field.type != Int2Float)
				(*(*treePart).right).field = right;
			if ((left.type == var) || (left.type == floatVar))
				return left;
			else
				return right;
		}
		else
		{
			if (left.type == integer)
			{
				long ress = 0;
				switch ((*treePart).field.token[0])
				{
					case '+':
						ress = strtol(left.token, NULL, 10) + strtol(right.token, NULL, 10);
						break;
					case '-':
						ress = strtol(left.token, NULL, 10) - strtol(right.token, NULL, 10);
						break;
					case '*':
						ress = strtol(left.token, NULL, 10) * strtol(right.token, NULL, 10);
						break;
					case '/':
						ress = strtol(left.token, NULL, 10) / strtol(right.token, NULL, 10);
						break;
				}
				sprintf((*treePart).field.token, "%ld", ress);
				(*treePart).field.type = integer;
				return (*treePart).field;
			}
			if (left.type == real)
			{
				char* nstrL = left.token;
				char* nstrR = right.token;
				double ress = 0;
				switch ((*treePart).field.token[0])
				{
					case '+':
						ress = strtod (left.token, &nstrL) + strtod (right.token, &nstrR);
						break;
					case '-':
						ress = strtod (left.token, &nstrL) - strtod (right.token, &nstrR);
						break;
					case '*':
						ress = strtod (left.token, &nstrL) * strtod (right.token, &nstrR);
						break;
					case '/':
						ress = strtod (left.token, &nstrL) / strtod (right.token, &nstrR);
						break;
				}
				sprintf((*treePart).field.token, "%f", ress);
				(*treePart).field.type = real;
				return (*treePart).field;
			}
		}
	}
	if ((*treePart).field.type == Int2Float)
	{	
		struct tokenStract left = opt1Helper((*treePart).left);
		if (left.type == integer)
		{
			int len = strLen(left.token, 62);
			left.token[len] = '.';
			left.token[len+1] = '0';
			left.type = real;
		}
		return left;
	}
	if (((*treePart).field.type == integer) || ((*treePart).field.type == real) || ((*treePart).field.type == var) || ((*treePart).field.type == floatVar))
	{
		return (*treePart).field;
	}
}

struct tokenStract opt3Helper(struct syntaxTreePart *treePart, bool isLeft)
{
	if ((*treePart).field.type == action)
	{
		struct tokenStract left = opt3Helper((*treePart).left, true);
		struct tokenStract right = opt3Helper((*treePart).right, false);
		
		char* nstrL = left.token;
		char* nstrR = right.token;
		
		switch ((*treePart).field.token[0])
		{
			case '+':
				if (isZero(left.token))
				{
					if (isLeft)
						(*(*treePart).up).left = (*treePart).right;
					else
						(*(*treePart).up).right = (*treePart).right;
					(*(*treePart).right).up = (*treePart).up;
				}
				if (isZero(right.token))
				{
					if (isLeft)
						(*(*treePart).up).left = (*treePart).left;
					else
						(*(*treePart).up).right = (*treePart).left;
					(*(*treePart).left).up = (*treePart).up;
				}
				break;
			case '-':
				if (isZero(right.token))
				{
					if (isLeft)
						(*(*treePart).up).left = (*treePart).left;
					else
						(*(*treePart).up).right = (*treePart).left;
					(*(*treePart).left).up = (*treePart).up;
				}
				break;
			case '*':
				if (strtod(left.token, &nstrL) == 1.0)
				{
					if (isLeft)
						(*(*treePart).up).left = (*treePart).right;
					else
						(*(*treePart).up).right = (*treePart).right;
					(*(*treePart).right).up = (*treePart).up;
				}
				if (strtod(right.token, &nstrL) == 1.0)
				{
					if (isLeft)
						(*(*treePart).up).left = (*treePart).left;
					else
						(*(*treePart).up).right = (*treePart).left;
					(*(*treePart).left).up = (*treePart).up;
				}
				if (isZero(left.token))
				{
					if (isLeft)
						(*(*treePart).up).left = (*treePart).left;
					else
						(*(*treePart).up).right = (*treePart).left;
					(*(*treePart).left).up = (*treePart).up;
				}
				if (isZero(right.token))
				{
					if (isLeft)
						(*(*treePart).up).left = (*treePart).right;
					else
						(*(*treePart).up).right = (*treePart).right;
					(*(*treePart).right).up = (*treePart).up;
				}
				break;
			case '/':
				if (strtod(right.token, &nstrL) == 1.0)
				{
					if (isLeft)
						(*(*treePart).up).left = (*treePart).left;
					else
						(*(*treePart).up).right = (*treePart).left;
					(*(*treePart).left).up = (*treePart).up;
				}
				break;
		}
	}
	if ((*treePart).field.type == Int2Float)
		opt3Helper((*treePart).left, true);
	return (*treePart).field;
}

void opt1()
{
	opt1Helper(SyntaxTree);
}

void opt2()
{
	opt2Helper(SyntaxTree);
}

void opt3()
{
	opt3Helper(SyntaxTree, true);
}

struct symbol {
	char id[64];
	char name[64];
    enum tokenType type;
};

int symbolsNum = 0;
struct symbol symbols[1024];

struct portableCodeEl {
	char opt[4];
	char res[64];
	char arg1[64];
	char arg2[64];
};

int optsNum = 0;
struct portableCodeEl opts[1024];

char* genActionVar(
	struct syntaxTreePart *treePart,
	FILE *S1,
	int *resVarCount,
	char resVars[1024][64],
	enum tokenType resVarsType[1024],
	bool isMod3)
{	
	if (((*treePart).field.type == integer) || ((*treePart).field.type == real))
		return (*treePart).field.token;
	
	char out[64];
	
	if (((*treePart).field.type == var) || ((*treePart).field.type == floatVar))
		sprintf(out, "<id,%d>", FVar((*treePart).field.token)+1);

	if ((*treePart).field.type == Int2Float)
	{
		char left[64]; 
		strcpy(left, genActionVar((*treePart).left, S1, resVarCount, resVars, resVarsType, isMod3));
		
		sprintf(out, "<id,%d>", (*resVarCount)+1);
		strcpy(resVars[(*resVarCount)], out);
		resVarsType[(*resVarCount)] = real;
		(*resVarCount) = (*resVarCount)+1;
		
		if (isMod3)
		{
			struct portableCodeEl temp;
			strcpy(temp.opt, "i2f");
			strcpy(temp.res, out);
			strcpy(temp.arg1, left);
			strcpy(temp.arg2, "");
			opts[optsNum] = temp;
			optsNum = optsNum + 1;
		}
		else
			fprintf(S1, "i2f %s %s\n", out, left);
	}

	if ((*treePart).field.type == action)
	{
		char left[64]; 
		strcpy(left, genActionVar((*treePart).left, S1, resVarCount, resVars, resVarsType, isMod3));

		char right[64];
		strcpy(right, genActionVar((*treePart).right, S1, resVarCount, resVars, resVarsType, isMod3));
		
		sprintf(out, "<id,%d>", (*resVarCount)+1);
		strcpy(resVars[(*resVarCount)], out);
		resVarsType[(*resVarCount)] = defineMainType(treePart);
		(*resVarCount) = (*resVarCount)+1;
		
		if (isMod3)
		{
			struct portableCodeEl temp;
			strcpy(temp.opt, getActionText((*treePart).field.token[0]));
			strcpy(temp.res, out);
			strcpy(temp.arg1, left);
			strcpy(temp.arg2, right);
			opts[optsNum] = temp;
			optsNum = optsNum + 1;
		}
		else
			fprintf(S1, "%s %s %s %s\n", getActionText((*treePart).field.token[0]), out, left, right);
	}
	
	return out;
}

int actionVarOptAddVar(
	char out[64],
	int *resVarCount,
	char resVars[1024][64],
	enum tokenType resVarsType[1024],
	enum tokenType baseType,
	int sysVarsIntFree[1024],
	int sysVarsFloatFree[1024],
	int *sysVarIntFree,
	int *sysVarFloatFree)
{
	int varId = 0;
	
	if (baseType == integer)
	{
		if ((*sysVarIntFree) > 0)
		{
			varId = sysVarsIntFree[(*sysVarIntFree)-1];
			(*sysVarIntFree) = (*sysVarIntFree) - 1;
			sprintf(out, "<id,%d>", varId+1);
			return varId;
		}
	}
	if (baseType == real)
	{
		if ((*sysVarFloatFree) > 0)
		{
			varId = sysVarsFloatFree[(*sysVarFloatFree)-1];
			(*sysVarFloatFree) = (*sysVarFloatFree) - 1;
			sprintf(out, "<id,%d>", varId+1);
			return varId;
		}
	}
	sprintf(out, "<id,%d>", (*resVarCount)+1);
	strcpy(resVars[(*resVarCount)], out);
	resVarsType[(*resVarCount)] = baseType;
	(*resVarCount) = (*resVarCount)+1;
	return ((*resVarCount)-1);
}

char* genActionVarOpt(
	struct syntaxTreePart *treePart,
	FILE *S1,
	int *resVarCount,
	char resVars[1024][64],
	enum tokenType resVarsType[1024],
	int sysVarsIntFree[1024],
	int sysVarsFloatFree[1024],
	int *sysVarIntFree,
	int *sysVarFloatFree,
	int *retId,
	bool isMod3)
{
	int leftId = -1;
	int rightId = -1;
	
	if (((*treePart).field.type == integer) || ((*treePart).field.type == real))
		return (*treePart).field.token;
	
	char out[64];
	
	if (((*treePart).field.type == var) || ((*treePart).field.type == floatVar))
	{
		//(*retId) = FVar((*treePart).field.token)
		sprintf(out, "<id,%d>", FVar((*treePart).field.token)+1);
	}

	if ((*treePart).field.type == Int2Float)
	{
		char left[64]; 
		//strcpy(left, genActionVar((*treePart).left, S1, resVarCount, resVars, resVarsType));
		
		strcpy(
			left,
			genActionVarOpt((*treePart).left, S1, resVarCount, resVars, resVarsType,
				sysVarsIntFree, sysVarsFloatFree, sysVarIntFree, sysVarFloatFree, &leftId, isMod3)
		);
		
		(*retId) = actionVarOptAddVar(out, resVarCount, resVars, resVarsType, real,
			sysVarsIntFree, sysVarsFloatFree, sysVarIntFree, sysVarFloatFree);
		
		if (isMod3)
		{	
			struct portableCodeEl temp;
			strcpy(temp.opt, "i2f");
			strcpy(temp.res, out);
			strcpy(temp.arg1, left);
			strcpy(temp.arg2, "");
			opts[optsNum] = temp;
			optsNum = optsNum + 1;
		}
		else
			fprintf(S1, "i2f %s %s\n", out, left);
		
		if (leftId >= 0)
		{
			enum tokenType leftType = defineMainType((*treePart).left);
			if (leftType == integer)
			{
				sysVarsIntFree[(*sysVarIntFree)] = leftId;
				(*sysVarIntFree) = (*sysVarIntFree) + 1;
			}
			if (leftType == real)
			{
				sysVarsFloatFree[(*sysVarFloatFree)] = leftId;
				(*sysVarFloatFree) = (*sysVarFloatFree) + 1;
			}
		}
	}

	if ((*treePart).field.type == action)
	{
		char left[64]; 
		//strcpy(left, genActionVar((*treePart).left, S1, resVarCount, resVars, resVarsType));
		
		strcpy(
			left,
			genActionVarOpt((*treePart).left, S1, resVarCount, resVars, resVarsType,
				sysVarsIntFree, sysVarsFloatFree, sysVarIntFree, sysVarFloatFree, &leftId, isMod3)
		);
		
		char right[64];
		//strcpy(right, genActionVar((*treePart).right, S1, resVarCount, resVars, resVarsType));
		
		strcpy(
			right,
			genActionVarOpt((*treePart).right, S1, resVarCount, resVars, resVarsType,
				sysVarsIntFree, sysVarsFloatFree, sysVarIntFree, sysVarFloatFree, &rightId, isMod3)
		);
		
		(*retId) = actionVarOptAddVar(out, resVarCount, resVars, resVarsType, defineMainType(treePart),
			sysVarsIntFree, sysVarsFloatFree, sysVarIntFree, sysVarFloatFree);
		
		if (isMod3)
		{
			struct portableCodeEl temp;
			strcpy(temp.opt, getActionText((*treePart).field.token[0]));
			strcpy(temp.res, out);
			strcpy(temp.arg1, left);
			strcpy(temp.arg2, right);
			opts[optsNum] = temp;
			optsNum = optsNum + 1;
		}
		else
			fprintf(S1, "%s %s %s %s\n", getActionText((*treePart).field.token[0]), out, left, right);
		
		if (leftId >= 0)
		{
			enum tokenType leftType = defineMainType((*treePart).left);
			if (leftType == integer)
			{
				sysVarsIntFree[(*sysVarIntFree)] = leftId;
				(*sysVarIntFree) = (*sysVarIntFree) + 1;
			}
			if (leftType == real)
			{
				sysVarsFloatFree[(*sysVarFloatFree)] = leftId;
				(*sysVarFloatFree) = (*sysVarFloatFree) + 1;
			}
		}
		if (rightId >= 0)
		{
			enum tokenType rightType = defineMainType((*treePart).right);
			if (rightType == integer)
			{
				sysVarsIntFree[(*sysVarIntFree)] = rightId;
				(*sysVarIntFree) = (*sysVarIntFree) + 1;
			}
			if (rightType == real)
			{
				sysVarsFloatFree[(*sysVarFloatFree)] = rightId;
				(*sysVarFloatFree) = (*sysVarFloatFree) + 1;
			}
		}
	}
	
	return out;
}

void gen1(bool isOpt, bool isMod3)
{
	FILE *S1, *S2;
	if (!isMod3)
	{
		S1 = fopen("portable_code.txt", "w");
		S2 = fopen("symbols.txt", "w");
	}
	else
	{
		memset(symbols, '\0', 1024);
		memset(opts, '\0', 1024);
		S1 = fopen("post_code.bin", "w");
	}
	
	int resVarCount = varCount;
	char resVars[1024][64];
	enum tokenType resVarsType[1024];
	
	for(int i=0; i<varCount; i++)
	{
		sprintf(resVars[i], "<id,%d>", i+1);
		if (VarsType[i] == var)
			resVarsType[i] = integer;
		if (VarsType[i] == floatVar)
			resVarsType[i] = real;
	}
	
	if (isOpt)
	{
		int sysVarsIntFree[1024];
		int sysVarsFloatFree[1024];
		int sysVarIntFree = 0;
		int sysVarFloatFree = 0;
		int temp = -1;
		genActionVarOpt(SyntaxTree, S1, &resVarCount, resVars, resVarsType,
			sysVarsIntFree, sysVarsFloatFree, &sysVarIntFree, &sysVarFloatFree, &temp, isMod3);
	}
	else
		genActionVar(SyntaxTree, S1, &resVarCount, resVars, resVarsType, isMod3);
	
	char cureVarType[64];
	for(int i=0; i<resVarCount; i++)
	{
		if (resVarsType[i] == integer)
			strcpy(cureVarType, "integer");
		if (resVarsType[i] == real)
			strcpy(cureVarType, "float");
		
		if (i < varCount)
		{
			if (isMod3)
			{
				struct symbol temp;
				strcpy(temp.id, resVars[i]);
				sprintf(temp.name, "%s", Vars[i]);
				temp.type = resVarsType[i];
				symbols[symbolsNum] = temp;
				symbolsNum = symbolsNum + 1;
			}
			else
				fprintf(S2, "%s - %s, %s\n", resVars[i], Vars[i], cureVarType);
		}
		else
		{
			if (isMod3)
			{
				struct symbol temp;
				strcpy(temp.id, resVars[i]);
				sprintf(temp.name, "T%d", i+1-varCount);
				temp.type = resVarsType[i];
				symbols[symbolsNum] = temp;
				symbolsNum = symbolsNum + 1;
			}
			else
				fprintf(S2, "%s - T%d, %s\n", resVars[i], i+1-varCount, cureVarType);
		}
	}
	
	if (isMod3)
	{	
		for (int i=0; i<symbolsNum; i++)
		{
			printf("id: %s, name: %s, type: %d\n", symbols[i].id, symbols[i].name, symbols[i].type);
		}
		fwrite(symbols, 1, sizeof(symbols), S1);
		
		for (int i=0; i<optsNum; i++)
		{
			printf("opt: %s, res: %s, arg1: %s, arg2: %s \n", opts[i].opt, opts[i].res, opts[i].arg1, opts[i].arg2);
		}
		fwrite(opts, 1, sizeof(opts), S1);
	}
	
	fclose(S1);
	fclose(S2);
}

void gen2Helper(struct syntaxTreePart *treePart, struct tokenStract nums[1024], struct tokenStract acts[1024], int *numsLen, int *actsLen)
{
	if ((*treePart).field.type == action)
	{
		gen2Helper((*treePart).left, nums, acts, numsLen, actsLen);
		gen2Helper((*treePart).right, nums, acts, numsLen, actsLen);
		acts[(*actsLen)] = (*treePart).field;
		(*actsLen) = (*actsLen)+1;
	}
	if ((*treePart).field.type == Int2Float)
	{
		gen2Helper((*treePart).left, nums, acts, numsLen, actsLen);
		acts[(*actsLen)] = (*treePart).field;
		(*actsLen) = (*actsLen)+1;
	}
	if (((*treePart).field.type == integer) || ((*treePart).field.type == real) || ((*treePart).field.type == var) || ((*treePart).field.type == floatVar))
	{
		nums[(*numsLen)] = (*treePart).field;
		(*numsLen) = (*numsLen)+1;
	}
}

void gen2()
{
	FILE *S1, *S2;
	S1 = fopen("postfix.txt", "w");
	S2 = fopen("symbols.txt", "w");
	
	struct tokenStract nums[1024];
	int numsLen = 0;
	struct tokenStract acts[1024];
	int actsLen = 0;
	
	gen2Helper(SyntaxTree, nums, acts, &numsLen, &actsLen);
	
	for (int i=0; i<numsLen; i++)
	{
		if ((nums[i].type == var) || (nums[i].type == floatVar))
		{
			fprintf(S1, "<id,%d>", FVar(nums[i].token)+1);
		}
		if ((nums[i].type == integer) || (nums[i].type == real))
		{
			fprintf(S1, "<%s>", nums[i].token);
		}
	}
	
	for (int i=0; i<actsLen; i++)
	{
		if (acts[i].type == action)
		{
			fprintf(S1, "<%s>", acts[i].token);
		}
		if (acts[i].type == Int2Float)
		{
			fprintf(S1, "<i2f>");
		}
	}
	
	char cureVarType[64];
	for(int i=0; i<varCount; i++)
	{
		if (VarsType[i] == var)
			strcpy(cureVarType, "integer");
		if (VarsType[i] == floatVar)
			strcpy(cureVarType, "float");
			
		fprintf(S2, "<id,%d> - %s, %s\n", i+1, Vars[i], cureVarType);
	}
	
	fclose(S1);
	fclose(S2);
}

int main(int argc, char *argv[])
{
	int spArgs = 0;
	bool isOpt = false;
	if (argc != 3)
	{
		if (strcmp(argv[2], "OPT") || strcmp(argv[2], "opt"))
		{
			if (argc != 4)
			{
				printf("Error, expected 3 arguments!");
				exit (EXIT_FAILURE);
			}
			spArgs = spArgs+1;
			isOpt = true;
		}
		else
		{
			printf("Error, expected 2 arguments!");
			exit (EXIT_FAILURE);
		}
	}
	if (strcmp(argv[1], "LEX") || strcmp(argv[1], "lex"))
		lexicalAnalysis(argv[2+spArgs], "tokens.txt", "symbols.txt", 1);
	else
		lexicalAnalysis(argv[2+spArgs], "tokens.txt", "symbols.txt", 0);
	parsing();
	if (strcmp(argv[1], "SYN") || strcmp(argv[1], "syn"))
	{
		printSyntaxTree("syntax_tree.txt");
	}
	typeConversion();
	if (strcmp(argv[1], "SEM") || strcmp(argv[1], "sem"))
	{
		printSyntaxTree("syntax_tree_mod.txt");
	}
	if (isOpt)
	{
		opt3();
		opt2();
		opt1();
	}
	if (strcmp(argv[1], "GEN1") || strcmp(argv[1], "gen1"))
	{
		gen1(isOpt, false);
	}
	if (strcmp(argv[1], "GEN2") || strcmp(argv[1], "gen2"))
	{
		if (argv[1+spArgs][3] == '2')
			gen2();
	}
	if (strcmp(argv[1], "GEN3") || strcmp(argv[1], "gen3"))
	{
		if (argv[1+spArgs][3] == '3')
			gen1(isOpt, true);
	}
}

//opt, gen3, S1.txt
