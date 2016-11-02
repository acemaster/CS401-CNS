#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


using namespace std;
int main(int argc,char **argv)
{
	int message_length;
	char message[512];
	int key_refer[26]={0};
	char cipher[27];
	message_length = atoi(argv[1]);
	for (int i = 0; i < message_length; ++i)
	{
		message[i]=argv[2][i];
	}
	int key_length = strlen(argv[3]);
	int count=0;
	for (int i = 0; i < key_length; ++i)
	{
		if(key_refer[argv[3][i]-97]!=-1)
			{
				cipher[count]=argv[3][i];
				key_refer[argv[3][i]-97]=-1;
				count++;
			}
	}

	for(int i=0;i<26;i++)
	{
		if(key_refer[i]!=-1)
		{
			cipher[count++]=i+97;
		}
	}
	cipher[count]='\0';
	cout<<"\n=====================================\n";
	cout<<"\tMonoAlphabet cipher\n";
	cout<<"=======================================\n";
	cout<<"New cipher: "<<cipher<<endl;
	message[message_length]='\0';
	cout<<"Message: "<<message<<endl;
	for (int i = 0; i < message_length; ++i)
	{
		// cout<<int(message[i])-65<<endl;
		// cout<<char(cipher[message[i]-97])<<endl;
		// cout<<char(((int(cipher[message[i]-97]-96)+3)%26)+96)<<endl;
		// cout<<int(message[i])-96<<endl;
		// cout<<(int(message[i])-96+3)%26<<endl;
		// cout<<char((int(message[i])-96+3)%26)<<endl;
		// cout<<"=====================\n";
		if(isalpha(message[i]))
			message[i]=toupper(char((cipher[(message[i]-97+3)%26])));
	}

	cout<<"\nEncrypted message: "<<message<<endl;

		//Decrypt
	for (int i = 0; i < message_length; ++i)
	{
		for(int j=0;j<26;j++)
			if(cipher[j] == tolower(message[i]))
				message[i]=char(((j-3)%26)+97);
	}
	cout<<"\nDecrypted message: "<<message<<endl;
	return 0;
}