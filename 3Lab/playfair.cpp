#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


using namespace std;

int findletter(char find,char argv[5][5],int *findrow,int *findcol)
{
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			if(argv[i][j] == toupper(find))
				{
					*(findrow)=i;
					*(findcol)=j;
				}
		}
	}
}

int main(int argc,char **argv)
{
	int message_length;
	char message[512];
	int key_refer[26]={0};
	char cipher[5][5];
	message_length = atoi(argv[1]);
	for (int i = 0; i < message_length; ++i)
	{
		message[i]=argv[2][i];
		if(argv[2][i]=='j')
			message[i]='i';
	}
	int key_length = strlen(argv[3]);
	int countr=0;
	int countc=0;
	for (int i = 0; i < key_length; ++i)
	{
		if(key_refer[argv[3][i]-97]!=-1)
			{
				cipher[countr][countc]=toupper(argv[3][i]);
				key_refer[argv[3][i]-97]=-1;
				countc++;
				if(countc == 5)
				{
					countc=0;
					countr++;
				}
			}
	}

	for(int i=0;i<26;i++)
	{
		if(key_refer[i]!=-1 && i!=9)
		{
			cipher[countr][countc++]=i+65;
			if(countc == 5)
			{
				countc=0;
				countr++;
			}
		}
	}
	cout<<"\n=====================================\n";
	cout<<"\tPlayfaircipher\n";
	cout<<"=======================================\n";
	cout<<"New cipher: "<<endl;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			cout<<"\t"<<cipher[i][j];
		}
		cout<<endl;
	}
	message[message_length]='\0';
	cout<<"Message: "<<message<<endl;
	char letter1;
	char letter2;
	int findrow1,findcol1,findrow2,findcol2;
	char encmessage[200];
	int count=0;
	for (int i = 0; i < message_length; i=i+2)
	{
		letter1=message[i];
		letter2=message[i+1];
		if(letter1 == letter2)
		{
			letter2='x';
			i=i-1;
		}
		findletter(letter1,cipher,&findrow1,&findcol1);
		findletter(letter2,cipher,&findrow2,&findcol2);
		cout<<"findrow1 : "<<findrow1<<endl;
		cout<<"findrow2 : "<<findrow2<<endl;
		cout<<"findcol1 : "<<findcol1<<endl;
		cout<<"findcol2 : "<<findcol2<<endl;
		if(findrow1 == findrow2)
		{
			letter1 = cipher[findrow1][(findcol1+1)%5];
			letter2 = cipher[findrow1][(findcol2+1)%5];
		}
		else if(findcol1 == findcol2)
		{
			letter1 = cipher[(findrow1+1)%5][findcol2];
			letter2 = cipher[(findrow2+1)%5][findcol1];
		}
		else
		{
			letter1 = cipher[findrow1][findcol2];
			letter2 = cipher[findrow2][findcol1];
		}
		cout<<letter1<<" : "<<letter2<<endl;
		encmessage[count++]=letter1;
		encmessage[count++]=letter2;
	}

	cout<<"\nEncrypted message: "<<encmessage<<endl;

		//Decrypt
	for (int i = 0; i < message_length; ++i)
	{

	}
	cout<<"\nDecrypted message: "<<message<<endl;
	return 0;
}