#include <stdio.h>
#include <stdlib.h>
#include <math.h>
	// WIADOMOSC POCZATKOWA , KLUCZ POCZATKOWY.
	int w[8]={0,1,1,0,0,0,1,1};
	int kp[10]={1,1,0,1,1,1,1,1,0,1};
	// 3. a)
	int P10[10]={2,4,1,6,3,9,0,8,7,5};
	int wP10[10];
	// 3.b)
	int SL1[5]={1,2,3,4,0};
	int SL1_a[5];
	int SL1_b[5];
	int PO_SL1_a[5];
	int PO_SL1_b[5];
	// 3.c)
	int scalenie[10];
	int P10W8[8]={5,2,6,3,7,4,9,8};
	int klucz_1_rundy[8];
	int klucz_2_rundy[8];
	int SL2[5]={2,3,4,0,1};
	int K2R_a[5];// PO PRZESUNIECIU o 2 bity
	int K2R_b[5];
	int scalenie2[10];
	//-----------------------------------------//ss
	// 5.a)
	int PW[8]={1,5,2,0,3,7,4,6};
	// 4.a)
	int w_a[4];
	int w_b[4];
	int w_c[4];
	int P4W8[8]={3,0,1,2,1,2,3,0};
	int w_b_P4W8[8];
	int XOR_1[8];
	int XOR_2[8];
	int XOR1_1_a[4];
	int XOR1_1_b[4];
	int XOR_2_a[4];
	int XOR_2_b[4];
	int policz,policz2;
	int wynik_sbox1[2];
	int wynik_sbox2[2];
	int scalenie_sbox[4];
	int P4[4]={1,3,2,0};
	int P4_permutacja[4];
	int XOR2[4];
	int wynik_odejmowania[4];
	int dodawanie2[8];
	int w_PO[8];
	int w_PO_a[4];
	int w_PO_b[4];
	int wPW[8];
	int zamiana[8]={4,5,6,7,0,1,2,3};
	int po_zamianie[8];
	int PO[8]={3,0,2,4,6,1,7,5};
	int po_PO[8];
	int j=0;
	int cyfra[2]={1,2};

void szyfrowanie1(int C[],int kluczyk[])
{
int i;
	for(i=0;i<8;i++)
	{
			if(i<4)
			{
				w_a[i]=C[i];
			}
				
				else
				{
					w_b[i-4]=C[i]; // TEKST NA 2 CIAGI 4 BITOWE
					w_c[i-4]=C[i];
				}
	}
	for(i=0;i<8;i++)
	{
	w_b_P4W8[i]=w_b[P4W8[i]];
	}
	for(i=0;i<8;i++)
	{
		XOR_1[i]=(w_b_P4W8[i]+kluczyk[i])%2;
	}
	for(i=0;i<8;i++)
	{
		if(i<4)
			{
			XOR1_1_a[i]=XOR_1[i];
			}
			else
			{
			XOR1_1_b[i-4]=XOR_1[i]; // 2_4 bitowe CIAGI		
			}
		  
	}
		// LICZENIE BINARNE
for(i=0;i<4;i++)
	{
		if(XOR1_1_a[i]==1)
			{
				policz=policz+pow(2,i);
			}
		if(XOR1_1_b[i]==1)
			{
				policz2=policz2+pow(2,i);
			}	
	}

	
	// SBOX 2

if(policz2==0 || policz2==12 || policz2==11 || policz2==7 || policz2==5)	
{
	wynik_sbox2[0]=0;wynik_sbox2[1]=0;
}
else if(policz2==1 || policz2==14 || policz2==6 || policz2==15)
{
	wynik_sbox2[0]=1;wynik_sbox2[1]=1;
}
else if(policz2==8 || policz2==2 || policz2==9)
{
	wynik_sbox2[0]=1;wynik_sbox2[1]=0;
}
else
{
	wynik_sbox2[0]=0;wynik_sbox2[1]=1;
}
// SBOX 1
if(policz==0 || policz==10 || policz==5 || policz==3 || policz==13)
{
	wynik_sbox1[0]=0;wynik_sbox1[1]=1;
}
else if(policz==4 || policz==14 || policz==1)
{
	wynik_sbox1[0]=0;wynik_sbox1[1]=0;
}
else if(policz==6 || policz==12 || policz==15)
{
	wynik_sbox1[0]=1;wynik_sbox1[1]=0;
}
else
{
	wynik_sbox1[0]=1;wynik_sbox1[1]=1;	
}

// SCALENIE
	for(i=0;i<4;i++)
	{
			if(i<2)
			{
				scalenie_sbox[i]=wynik_sbox1[i];
			}		
				else
			{
				scalenie_sbox[i]=wynik_sbox2[i-2];
			}	
	}	
	// PERMUTACJA P4
	for(i=0;i<4;i++)
	{
		P4_permutacja[i]=scalenie_sbox[P4[i]];
	}

for(i=0;i<4;i++)
	{	
	wynik_odejmowania[i]=(w_a[i]+P4_permutacja[i])%2;
	}
	printf("WYNIK PO ZASZYFROWANIU KLUCZEM %i:",cyfra[j]);
	for(i=0;i<8;i++)
	{
		if(i<4)
	dodawanie2[i]=wynik_odejmowania[i];
	else
	dodawanie2[i]=w_b[i-4];
	}
for(i=0;i<8;i++)
	printf("%i",dodawanie2[i]);
	printf("\n");
	j++;
		policz=0;
	policz2=0;
}


void funkcja_kluczy(void)
{
int i;
		for(i=0;i<10;i++)
				{
				wP10[i]=kp[P10[i]];
					if(i<5)
						{
						SL1_a[i]=wP10[i];
						}		
					else
						{
						SL1_b[i-5]=wP10[i];
						}
					} // DZIA£A	
			for(i=0;i<5;i++)
			{
			PO_SL1_a[i]=SL1_a[SL1[i]];
			PO_SL1_b[i]=SL1_b[SL1[i]];
			} // DZIA£A
			//3 c)
		for(i=0;i<10;i++)
		{
			if(i<5)
			scalenie[i]=PO_SL1_a[i];
			else
			scalenie[i]=PO_SL1_b[i-5];	
		}
		for(i=0;i<8;i++)
		{
			klucz_1_rundy[i]=scalenie[P10W8[i]];
		}
			printf("TWOJ KLUCZ 1 RUNDY TO:");
			for(i=0;i<8;i++)
			printf("%i",klucz_1_rundy[i]);
			printf("\n");
		for(i=0;i<5;i++)
		{
		K2R_a[i]=PO_SL1_a[SL2[i]];
		K2R_b[i]=PO_SL1_b[SL2[i]];
		}
				for(i=0;i<10;i++)
				{
				if(i<5)
				scalenie2[i]=K2R_a[i];
				else
				scalenie2[i]=K2R_b[i-5];
				}		
				for(i=0;i<8;i++)
				{
				klucz_2_rundy[i]=scalenie2[P10W8[i]];
				}
				printf("TWOJ KLUCZ 2 RUNDY TO:");
				for(i=0;i<8;i++)
				printf("%i",klucz_2_rundy[i]);
				printf("\n");
				
					for(i=0;i<8;i++)
					wPW[i]=w[PW[i]]; // WIADOMOSC W PERMUTUJEMY PW, DZIALA.
					szyfrowanie1(wPW,klucz_1_rundy);
					for(i=0;i<8;i++)
					po_zamianie[i]=dodawanie2[zamiana[i]]; // DZIALA.
					szyfrowanie1(po_zamianie,klucz_2_rundy);
					
			}

int main() 
{
	int i;
	funkcja_kluczy();
	for(i=0;i<8;i++)
	po_PO[i]=dodawanie2[PO[i]];
	printf("ZASZYFROWANY TEKST PO PERMUTACJI ODWROTNEJ:");
	for(i=0;i<8;i++)
	printf("%i",po_PO[i]);
	printf("\n");
	system("PAUSE");
	return 0;
}
