#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int curr = 0;
char str[100];
void T();
void Tprime();
void S();

void invalid()
{
printf("-----------------ERROR!----------------\n");
exit(0);
}
void valid()
{
printf("----------------SUCCESS!---------------\n");
exit(0);
}
void T()
{	S();
	Tprime();
}
void Tprime()
{	if(str[curr] == ',')
	  {
		curr++;
		S();
		Tprime();
	  }
	  else {if(str[curr] == '$')
		valid();
	  	else return;  
		}
}
void S()
{	if(str[curr] == 'a' || str[curr]=='>')
	{curr++;
	return;
	}
	else if(str[curr]=='(')
		{curr++;
		T();
		if(str[curr]==')')
			valid();
		else invalid();
		}
	else invalid();
}
int main()
	{printf("Enter a $ terminated string: ");
	scanf("%s", str);
	S();
	if(str[curr] == '$')
	valid();
	else
	invalid();
return 0;
}
