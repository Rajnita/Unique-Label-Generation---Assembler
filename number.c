#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void GETLINE(); /*Getline function */
void PROCESSLINE(); /*Process line function*/
void DEFINE();/*Define Function*/
void EXPAND();/*Expand the opcode function*/
FILE *output; /*Output file */
FILE *input; /*Input file containing Macro and Main program */
char label[20],opcode[10],operand[25];
char line[20];
int namcount=0, defcount=0;int n1=0;
int EXPANDING;
int curr;

struct NAMTAB
{
 char name[10];
 int start,end;
}mynametab[15];

struct DEFTAB
{
char macroline[25];
}mydefinetab[25];

struct ARGTAB
{
char arg[3][9];
}myargumenttab;

 /*Main function*/

int main()
{
	 EXPANDING=0;
	 printf("\n C PROGRAM OF MACRO OPCODE ASSEMBLER\n\n");
	 input =fopen("input.txt","r");                         
	 output=fopen("output.txt","w");
	 GETLINE();
	 while(strcmp(opcode,"END")!=0)
	 { 
		PROCESSLINE();
		GETLINE();
	 }
	 fprintf(output,"%s",line);
	 printf("\nExpanded output file has been successfully created.\n");
	 system("pause");
	 //getch();
	 return 1;
}

/* GETLINE FUNCTION */

void GETLINE()

{   

char word1[10],word2[10],word3[10],buff[10];

int count=0,i,j=0;

if(EXPANDING)

strcpy(line,mydefinetab[curr++].macroline);

else

fgets(line,20,input);

opcode[0]='\0';label[0]='\0';operand[0]='\0';word1[0]='\0';word2[0]='\0';word3[0]='\0';

 

for(i=0;line[i]!='\0';i++)

{

  if(line[i]!=' ')

    buff[j++]=line[i];

  else

  {

    buff[j]='\0';

    strcpy(word3,word2);

    strcpy(word2,word1);

    strcpy(word1,buff);

    j=0;

    count++;

    }

}

buff[j-1]='\0';

strcpy(word3,word2);

strcpy(word2,word1);

strcpy(word1,buff);

 

switch(count)

 {

  case 0:strcpy(opcode,word1);break;

  case 1:{strcpy(opcode,word2);strcpy(operand,word1);}break;

  case 2:{strcpy(label,word3);strcpy(opcode,word2);strcpy(operand,word1);}break;

  }

}

 

/* PROCESSLINE Function */

void PROCESSLINE()

{

  int i;

  for(i=0;i<namcount;i++)

  if(!strcmp(opcode,mynametab[i].name))

  {
	n1++;
    EXPAND();

    return;

    }

  if(!strcmp(opcode,"MACRO"))

    DEFINE();

  else

    fprintf(output,"%s",line);

}

 

/* Define Function */  

void DEFINE()

{

  int LEVEL,i=0,j=0,k=0;

  char param[5][9];

  char s[3];

  strcpy(s,"123");

  strcpy(mynametab[namcount].name,label);

  mynametab[namcount].start=defcount;//start of macro

  strcpy(mydefinetab[defcount].macroline,line);//copy to deftab each line

  while(operand[i]!='\0')

  {

    if(operand[i]!=',')

      param[j][k++]=operand[i];

    else

      {

        param[j++][k]='\0';

        k=0;

        }

       i++;

   }

   param[j][k]='\0';

   LEVEL=1;

   while(LEVEL>0)

   {

     GETLINE();

     if(operand[0]!='\0')

     {

       for(i=0;i<3;i++)

       {

         if(!strcmp(operand,param[i]))

           {

            operand[0]='?';

            operand[1]=s[i];

            operand[2]='\0';

            }

        }

      }

     if(!strcmp(opcode,"MACRO"))

       LEVEL++;

     else if(!strcmp(opcode,"MEND"))

       LEVEL--;

       strcpy(mydefinetab[defcount].macroline,label);// add label from input to the define tab
	strcat(mydefinetab[defcount].macroline," ");
       strcat(mydefinetab[defcount].macroline,opcode);// add opcode from input to the define tab
     if(operand[0]!='\0')//if operand exists in the line replace by ?positional parameter

     {

      strcat(mydefinetab[defcount].macroline," ");

      strcat(mydefinetab[defcount].macroline,operand);

      strcat(mydefinetab[defcount].macroline,"\n");
     }
     strcat(mydefinetab[defcount++].macroline,"\n");
  }
mynametab[namcount++].end=defcount;//update nametab with end of deftab
}

 

/*Expand Function */

void EXPAND()

{

  int i,end=0,j=0,k=0;

  EXPANDING=1;

  int arg=0;

  fprintf(output,"//%s",line);

  for(i=0;i<namcount;i++)//running till we find the name of macro to be expanded

  {

    if(!strcmp(opcode,mynametab[i].name))

    {

      curr=mynametab[i].start;

      end=mynametab[i].end;

      while(operand[i]!='\0')

      {

        if(operand[i]!=',')

          myargumenttab.arg[j][k++]=operand[i];

        else

        {

          myargumenttab.arg[j++][k]='\n';

          k=0;

         }

        i++;

       }                             

       myargumenttab.arg[j][k]='\n';               

     }

  }

  while(curr<(end-1))

  {

    GETLINE();

    if(operand[0]=='?')
	strcpy(operand,myargumenttab.arg[operand[1]-'0'-1]);
	
			int n=n1;
    if(operand[0]=='$'||label[0]=='$')
	{
		char num[20];char newstr[25];   	
		// n++;
		newstr[0]='$';
		newstr[1]='\0';
		sprintf(num,"%d",n);
		strcat(newstr,num);
		//getting the nameoflabel
		int r=1;int c=0;
		if(operand[0]=='$')
		{	
			char getoper[20]="";
			while(label[r]!='\0')
			{
				getoper[c]=operand[r];
				c++;r++;
			}
			getoper[c]='\0';
			
			strcat(newstr,getoper);
			//strcat(newstr,"LOOP\n");
			strcpy(operand,newstr);
			//printf("%s",operand);
		}
		else if(label[0]=='$')
		{
			char getlab[20]="";
			while(label[r]!='\0')
			{
				getlab[c]=label[r];
				c++;r++;
			}
			getlab[c]='\0';
			strcat(newstr,getlab);
			//strcat(newstr,"LOOP");
			strcpy(label,newstr);
			//printf("%s lab",label);		
		}

		//n++;
		
	
	
	}
    fprintf(output,"%s %s %s",label,opcode,operand);

   }

  EXPANDING=0;

}



