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

struct omniVar {
	enum tokenType t;
	int i;
	float r;
};

struct omniVar allVars[1024];

int getId(char textVar[])
{
	char temp[64];
	int i = 0;
	int j = 0;
	int next = 2;
	while(next > 0)
	{
		if (next == 1)
		{
			temp[j] = textVar[i];
			j = j + 1;
		}
		
		if (textVar[i] == ',')
			next = 1;
		
		if (textVar[i] == '\n')
			next = 0;
		
		i = i + 1;
	}
	
	return strtol(temp, NULL, 10)-1;
}

int main(int argc, char *argv[])
{
	/*if (argc != 2)
	{
		printf("Error, expected 1 arguments!");
		exit (EXIT_FAILURE);
	}*/

	FILE *S1;
	S1 = fopen("post_code.bin", "r"); //argv[1]
	
	fread(symbols, 1, sizeof(symbols), S1);
	fread(opts, 1, sizeof(opts), S1);
	
	printf("---variables initialization ---\n");
	int i = 0;
	while (symbols[i].id[0] != '\0')
	{
		//printf("id: %s, name: %s, type: %d\n", symbols[i].id, symbols[i].name, symbols[i].type);
		allVars[i].t = symbols[i].type;
		if(symbols[i].name[0] != 'T')
		{
			if(symbols[i].type == integer)
			{
				printf("Input integer var %s: ", symbols[i].name);
				scanf("%d", &allVars[i].i);
			}
			if(symbols[i].type == real)
			{
				printf("Input real var %s: ", symbols[i].name);
				scanf("%f", &allVars[i].r);
			}
		}
		i = i + 1;
	}
	printf("---math operations---\n");
	i = 0;
	int resId2 = 0;
	while (opts[i].opt[0] != '\0')
	{
		//printf("opt: %s, res: %s, arg1: %s, arg2: %s \n", opts[i].opt, opts[i].res, opts[i].arg1, opts[i].arg2);
		
		int resId = getId(opts[i].res);
		resId2 = resId;
		//printf("num: %d \n", resId);
		
		char * end_ptr;
		
		struct omniVar omniVar1;
		if (opts[i].arg1[0] == '<')
		{
			omniVar1 = allVars[getId(opts[i].arg1)];
		}
		else
		{
			omniVar1.t = allVars[resId].t;
			if(allVars[resId].t == integer)
				omniVar1.i = strtol(opts[i].arg1, NULL, 10);
			if(allVars[resId].t == real)
				omniVar1.r = strtof(opts[i].arg1, end_ptr);
			//printf("arg1 - i:%d, r:%f\n", omniVar1.i, omniVar1.r);
		}
		
		struct omniVar omniVar2;
		if (opts[i].arg2[0] == '<')
		{
			omniVar2 = allVars[getId(opts[i].arg2)];
		}
		else
		{
			omniVar2.t = allVars[resId].t;
			if(allVars[resId].t == integer)
				omniVar2.i = strtol(opts[i].arg2, NULL, 10);
			if(allVars[resId].t == real)
				omniVar2.r = strtof(opts[i].arg2, end_ptr);
			//printf("arg2 - i:%d, r:%f\n", omniVar2.i, omniVar2.r);
		}
		
		if (strcmp(opts[i].opt, "i2f") == 0)
		{
			allVars[resId].r = omniVar1.i;
			printf("i2f %d = %f\n", omniVar1.i, allVars[resId].r);
		}
		
		if (strcmp(opts[i].opt, "add") == 0)
		{
			if(allVars[resId].t == integer)
			{
				allVars[resId].i = omniVar1.i + omniVar2.i;
				printf("%d + %d = %d\n", omniVar1.i, omniVar2.i, allVars[resId].i);
			}
			if(allVars[resId].t == real)
			{
				allVars[resId].r = omniVar1.r + omniVar2.r;
				printf("%f + %f = %f\n", omniVar1.r, omniVar2.r, allVars[resId].r);
			}
		}
		
		if (strcmp(opts[i].opt, "sub") == 0)
		{
			if(allVars[resId].t == integer)
			{
				allVars[resId].i = omniVar1.i - omniVar2.i;	
				printf("%d - %d = %d\n", omniVar1.i, omniVar2.i, allVars[resId].i);
			}
			if(allVars[resId].t == real)
			{
				allVars[resId].r = omniVar1.r - omniVar2.r;
				printf("%f - %f = %f\n", omniVar1.r, omniVar2.r, allVars[resId].r);	
			}
		}
		
		if (strcmp(opts[i].opt, "mul") == 0)
		{
			if(allVars[resId].t == integer)
			{
				allVars[resId].i = omniVar1.i * omniVar2.i;
				printf("%d * %d = %d\n", omniVar1.i, omniVar2.i, allVars[resId].i);
			}
			if(allVars[resId].t == real)
			{
				allVars[resId].r = omniVar1.r * omniVar2.r;
				printf("%f * %f = %f\n", omniVar1.r, omniVar2.r, allVars[resId].r);
			}
		}
		
		if (strcmp(opts[i].opt, "div") == 0)
		{
			if(allVars[resId].t == integer)
			{
				allVars[resId].i = omniVar1.i / omniVar2.i;
				printf("%d / %d = %d\n", omniVar1.i, omniVar2.i, allVars[resId].i);
			}
			if(allVars[resId].t == real)
			{
				allVars[resId].r = omniVar1.r / omniVar2.r;
				printf("%f / %f = %f\n", omniVar1.r, omniVar2.r, allVars[resId].r);
			}
		}
		
		i = i + 1;
	}
	
	if(allVars[resId2].t == integer)
	{
		printf("\nresult: %d", allVars[resId2].i);
	}
	if(allVars[resId2].t == real)
	{
		printf("\nresult: %f", allVars[resId2].r);
	}
}

/*  int age;
    printf("Input age: ");
    scanf("%d", &age);*/


//post_code.bin
