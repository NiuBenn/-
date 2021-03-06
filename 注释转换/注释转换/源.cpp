#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<assert.h>
#include<windows.h>
enum State
{
	NORMAL,//普通状态
	FOUND_SLASH,//找到斜杠
	C_COMMENT,//C风格
	CPP_COMMENT,//C++风格
	FOUND_ASTERISK//
};


void CommentConvert(FILE *pIn, FILE *pOut)
{
	char ch,nextCh;
	State state = NORMAL;
	while (1)
	{
		ch = fgetc(pIn);
		if (ch == EOF)
		{
			break;
		}
		switch (state)
		{
		case NORMAL:
			if (ch == '/') 
				state = FOUND_SLASH;
			else
			{
				state = NORMAL;
				fputc(ch, pOut);
			}
			break;
		case FOUND_SLASH:
			if (ch == '*') 
			{
				fprintf(pOut, "//");
				state = C_COMMENT;
			}
			else if (ch == '/') 
			{	
				fprintf(pOut, "//");
				state = CPP_COMMENT;
			}
			else 
			{ 
				fputc('/', pOut);
				fputc(ch, pOut);
				state = NORMAL;
			}
			break;
		case C_COMMENT:
			if (ch == '*')  
				state = FOUND_ASTERISK;
			else {
				fputc(ch, pOut);
				if (ch == '\n') 
					fprintf(pOut, "//");
				state = C_COMMENT;
			}
			break;
		case CPP_COMMENT:
			if (ch == '\n') 
			{
				state = NORMAL;
				fputc(ch, pOut);
			}
			else
			{
				state = CPP_COMMENT;
				fputc(ch, pOut);
			}
			break;
		case FOUND_ASTERISK:
			if (ch == '*') 
			{
				fputc('*', pOut);
				state = FOUND_ASTERISK;
			}
			else if (ch == '/') 
			{
				nextCh = fgetc(pIn);
				if (nextCh != '\n') 
					fputc('\n', pOut);
				ungetc(nextCh, pIn);
				state = NORMAL;
			}
			else 
			{
				fputc('*', pOut);
				fputc(ch, pOut);
				state = C_COMMENT;
			}
			break;
		}

		const char *messages[] = 
		{
			"普通",
			"找到斜杠了",
			"C 风格注释",
			"C++ 风格注释",
			"找到星号了"
		};
		printf("当前字符: %c, 当前状态是: %s\n", ch, messages[state]);
		Sleep(100);
	}
}


int main()
{
	const char *INPUT = "input.c";
	const char *OUTPUT = "output.c";
	FILE *pIn = fopen(INPUT, "r");
	assert(pIn != NULL);
	FILE *pOut = fopen(OUTPUT, "w");
	assert(pIn);
	CommentConvert(pIn, pOut);
	fclose(pOut);
	fclose(pIn);
}








