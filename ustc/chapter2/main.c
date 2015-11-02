/*
在这部分中，你将使用图转移算法手工实现一个小型的词法分析器。
* 分析器的输入：存储在文本文件中的字符序列，字符取自ASCII字符集。文件中可能包括下面几种记号：关键字if、符合C语言标准的标识符、无符号整型数字、空格符、回车符\n。
* 分析器的输出：打印出所识别的记号的种类、及记号开始行号、开始列号信息。
注意：1. 忽略空格及回车符；2. 对于标识符和数字，要输出符号的具体词法单元（见下面的示例）。
【示例】对于下面的文本文件：
ifx if iif       if  234
iff     if
你的输出应该是（注意，因为文本显示的原因，列号信息可能不一定准确）：
ID(ifx) (1, 1)
IF        (1, 4)
ID(iif)  (1, 8)
IF       (1, 13)
NUM(234) (1, 16)
ID(iff) (2, 1)
IF       (2, 8)
*/

#include <stdio.h>
#include <stdlib.h>
enum kind {
	IF,
	ID,
	NUMBER,
	END
};

struct token
{
	enum kind k;
	char *lexeme;
	int line;
	int column;
};

FILE *fp;
int current_line=1;
int current_column=0;
char name[256];
int name_length=0;

char lastChar;

 struct token nextToken();
 void print(struct token t);
int main(int argc, char const *argv[])
{
	if(argc<2) printf("Usage: ./a.out filename\n");

	fp = fopen(argv[1],"rt");
    if(fp==NULL){
        printf("\nCannot open file\n");
        exit(1);
    } 

   printf("START\n");
    struct token t = nextToken();
   	while(!feof(fp)){
   		print(t);
   		t = nextToken();
   	}
    fclose(fp);
	return 0;
}

struct token nextToken(){
	
	short gotword = 0;
	short fileEnd = 0;
	char c;
	if(feof(fp)){
		fileEnd = 1;
		gotword=1;
	} else {
		c = fgetc(fp);
		
		switch(c){
			case ' ':
			case '\r':
			case '\n':
				gotword = 1;
				break;
			default :
				name[name_length]=c;
				current_column++;
				name_length++;
		}
		
	}

	struct token t;
	if(gotword==1){
		if(name_length>0){
			if(name_length==2&&name[0]=='i'&&name[1]=='f'){
				t.k = IF;
				char lexeme[256];
				lexeme[0]='i';
				lexeme[1]='f';
				lexeme[2]=0;
				t.lexeme = lexeme;
				t.line = current_line;
				t.column = current_column-name_length;
				name_length=0;
			} else {
				t.k = ID;
				char lexeme[256];
				short isNumber = 1;
				for(int i=0;i<name_length;i++){
					lexeme[i] = name[i];
					if (name[i]<'0'||name[i]>'9')
					{
						isNumber=0;
					}
				}
				if(isNumber==1){
					t.k=NUMBER;
				}
				lexeme[name_length]=0;
				t.lexeme = lexeme;
				t.line = current_line;
				t.column = current_column-name_length;
				name_length = 0;
			}
		} else {
			t = nextToken();
			
		}
		switch(c){
			case ' ':
				current_column++;
				break;
			case '\r':
			case '\n':
				if(lastChar!='\r'&&lastChar!='\n'){
					current_line++;
					current_column=0;
				}
				break;
		}
		lastChar = c;
		return t;
	} else {
		t = nextToken();
		return t;
	}
}

 void print(struct token t){
 	switch(t.k){
 		case IF:
 			printf("%s(%d,%d)\n", "IF",t.line,t.column);
 			break;
 		case ID:
 			printf("%s(%s)(%d,%d)\n", "ID",t.lexeme,t.line,t.column);
 			break;
 		case NUMBER:
 			printf("%s(%s)(%d,%d)\n", "NUM",t.lexeme,t.line,t.column);
 			break;
 		case END:
 			printf("END\n");
 			break;
 	}
 }

