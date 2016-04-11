#include<stdio.h>
#include<stdlib.h>
#include<string.h>


char wejscie[1000]; 
int i,j;
int czestosc[1000],ilosc[1000];
float grupa[10];
int ilosc_grup;
int main(){
	
	
	FILE * plik;
	plik=fopen("laborka2.txt","r");
	if(plik==NULL){
		printf("b³¹d\n");
	}; 

	fscanf(plik,"%s",wejscie);
	printf("podaj ilosc grup\n");
	scanf("%i",&ilosc_grup);

	for(j=0;j<ilosc_grup;j++)
		grupa[j]=0;

for(i=97;i<123;i++){
	for(j=0;j<ilosc_grup;j++){
		czestosc[j]=0;
		ilosc[j]=0;
	}
	for(j=0;j<strlen(wejscie);j++){
			if(wejscie[j]==i)
				czestosc[j%ilosc_grup]++;
			
			ilosc[j%ilosc_grup]++;
		}
	for(j=0;j<ilosc_grup;j++)
		grupa[j]+=(float)czestosc[j]*(czestosc[j]-1)/((ilosc[j]-1)*ilosc[j]);
}
for(j=0;j<ilosc_grup;j++)
	printf("I_c[%i]=%2f\n",j+1
	,grupa[j]);
	
	fclose(plik);
	system("pause");
	return 0;
}