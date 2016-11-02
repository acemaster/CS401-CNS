/*
Ceasar Cipher 
================
q = p+3 % 26
================
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


using namespace std;
int main(int argc,char **argv)
{
	int message_length;
	char message[512];
	message_length = atoi(argv[1]);
	for (int i = 0; i < message_length; ++i)
	{
		message[i]=argv[2][i];
	}
	cout<<"\n=====================================\n";
	cout<<"\tCeasar's cipher\n";
	cout<<"=======================================\n";
	message[message_length]='\0';
	cout<<"Message: "<<message<<endl;
	//Encrypt
	for (int i = 0; i < message_length; ++i)
	{
		// cout<<int(message[i])-96<<endl;
		// cout<<(int(message[i])-96+3)%26<<endl;
		// cout<<char((int(message[i])-96+3)%26)<<endl;
		// cout<<"=====================\n";
		if(isalpha(message[i]))
			message[i]=toupper(char(((int(message[i])-96+3)%26)+96));
	}
	cout<<"\nEncrypted message: "<<message<<endl;

	//Decrypt
	for (int i = 0; i < message_length; ++i)
	{
		// cout<<int(message[i])-96<<endl;
		// cout<<(int(message[i])-96+3)%26<<endl;
		// cout<<char((int(message[i])-96+3)%26)<<endl;
		// cout<<"=====================\n";
		if(isalpha(message[i]))
			message[i]=tolower(char(((int(message[i])-64-3)%26)+64));
	}
	cout<<"\nDecrypted message: "<<message<<endl;
	return 0;
}