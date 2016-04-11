#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char wejscie[10000],klucz[10]; 
int wyjscie[10000];
int dlugosc,i,j;
int main(){

	FILE * plik;
	plik=fopen("laborka1.txt","r");
	if(plik==NULL){
		printf("PLIK JEST PUSTY\n");
	}; 
	fscanf(plik,"%s",wejscie);
	dlugosc=strlen(wejscie);
	printf("%s",&wejscie);
	printf("Dlugosc tekstu wynosi : %i\n",dlugosc);
	printf("podaj klucz:");
	scanf("%s",&klucz);

	for(i=0, j=0 ;i<dlugosc;i++)
	{
		wyjscie[i]=(((int)wejscie[i]-97) + ((int)klucz[j]-97))%26;
		j++;
		while(j==(strlen(klucz)))
		{
			j=0;
		}
    }
	fclose(plik);
	plik=fopen("laborka2.txt","w");
	for(i=0; i<dlugosc; i++)
   {
		fprintf(plik,"%c",(wyjscie[i]+97));
   }	
	system("cls");
	printf("Tekst zaszyfrowany pomyslnie\n");
	fclose(plik);
	system("pause");
	return 0;
}