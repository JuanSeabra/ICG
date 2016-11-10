#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TAM_X 70
#define TAM_Y 60
#define TRUE 1
#define FALSE 0

char FrameBuffer[TAM_X][TAM_Y];

typedef struct {
	int a,b;
} Ponto;

void clearBuffer(){
	int i, j;
	for (i = 0; i < TAM_X; i++){
		for(j = 0; j < TAM_Y; j++){
			FrameBuffer[i][j] = ' ';
		}
	}
}

void PutPixel(int x, int y, char ch){
	FrameBuffer[x][y] = ch;
}

char GetPixel(int x, int y){
	return FrameBuffer[x][y];
}

void LinhaReta(int x1, int y1, int y2, short espessura, short tracejada){
	int i;
	short status = TRUE;
	for (i = y1; i < y2; i++){
		if(tracejada){
			if(status){
				if(espessura){
					PutPixel(x1,i,'*');
					PutPixel(x1+1,i,'*');
				} else {
					PutPixel(x1,i,'*');
				}
				status = FALSE;
			} else{
				status = TRUE;
			}
		} else {
			if(espessura){
				PutPixel(x1,i,'*');
				PutPixel(x1+1,i,'*');
			} else {
				PutPixel(x1,i,'*');
			}
		}
	}
}

void LineDDA1(int x1, int y1, int x2, int y2, short espessura, short tracejada){
	float y;
	short status = FALSE;
	if (x1 == x2) {
		LinhaReta(x1, y1, y2, espessura, tracejada);
		return;
	}
	float m = (y2-y1)/(x2-x1);
	int x;
	if(espessura){
		PutPixel(x1,y1, '*');
		PutPixel(x1+1,y1,'*');
	} else
		PutPixel(x1,y1, '*');
	y = y1;
	for (x=x1+1; x<=x2; x++) {
		y = y+m;
		if(tracejada){
			if(status) {
				if(espessura){
					PutPixel(x,round(y), '*');
					PutPixel(x+1,round(y), '*');
				} else {
					PutPixel(x,round(y), '*');
				}
				status = FALSE;
			} else {
				status = TRUE;
			}
		} else {
			if(espessura){
				PutPixel(x,round(y), '*');
				PutPixel(x+1,round(y), '*');
			} else {
				PutPixel(x,round(y), '*');
			}
		}
	}
}

void swap(int *x, int *y) {
	int k = *x;
	*x = *y;
	*y = k;
}

void bresenham(int x1, int y1, int x2, int y2, short espessura, short tracejada) {
	int dx,dy,p,k;
	float m;
	short status = TRUE;
	if (x1 == x2) {
		LinhaReta(x1, y1, y2, espessura, tracejada);
		return;
	}

	dx = abs(x2-x1);
	dy = abs(y2-y1);
	m = (float) (y2-y1)/(x2-x1);

	if (fabs(m)>1) {
		swap(&x1,&y1);
		swap(&x2,&y2);
		swap(&dx,&dy);
	}

	if (x1>x2) {
		x1 = x2;
		y1 = y2;
	}

	p = 2*dy - dx;


	for (k = 0; k < abs(dx); k++) {
		if (p < 0) {
			p += 2*dy;
		}
		else {
			if (m < 0)
				y1--;
			else y1++;

			p = p + (2 * dy) - (2 * dx);
		}

		x1++;
		if(tracejada){
			if(status){
				if(fabs(m) <= 1) {
					PutPixel(x1, y1, '*');
					if(espessura) PutPixel(x1,y1+1,'*');
				}
				else {
					PutPixel(y1, x1, '*');
					if(espessura) PutPixel(y1, x1+1, '*');
				}
				status = FALSE;
			} else {
				status = TRUE;
			}
		} else {
			if(fabs(m) <= 1) {
				PutPixel(x1, y1, '*');
				if(espessura) PutPixel(x1,y1+1,'*');
			}
			else {
				PutPixel(y1, x1, '*');
				if(espessura) PutPixel(y1, x1+1, '*');
			}
		}
	}
}

void PolyLine(Ponto vert[], int tam, short espessura, short tracejada){
	int i;
	for(i = 0; i < tam-1; i++){
		bresenham(vert[i].a,vert[i].b, vert[i+1].a, vert[i+1].b,
				espessura,tracejada);
	}
}

void Polygon(Ponto vert[], int tam, short espessura, short tracejada){
	int i;
	for(i = 0; i < tam-1; i++){
		bresenham(vert[i].a,vert[i].b, vert[i+1].a, vert[i+1].b,
				espessura, tracejada);
	}
	bresenham(vert[i].a,vert[i].b, vert[0].a, vert[0].b,
			espessura, tracejada);
}

//TODO DONE
void printBuffer(){
	int i, j;
	//for(i = 0; i < TAM_Y; i++) printf(" %d",i);
	printf("\n");
	for(i = 0; i < TAM_X; i++){
		//printf("%d",i);
		for(j = 0; j < TAM_Y; j++){
			printf("%c", GetPixel(i,j));
		}
		printf("\n");
	}
}

void ddacircle(int x,int y,int r){
	float x1,x2,y1,y2,eps,sx,sy;
	int val,i;
	x1=r;
	y1=0;
	sx=x1;
	sy=y1;
	i=0;
	do{
		val=pow(2,i);
		i++;
	}while(val<r);
	eps = 1/pow(2,i-1);
	do{
		x2 = x1 + y1*eps;
		y2 = y1 - eps*x2;
		PutPixel(x+x2,y-y2,'*');
		x1=x2;
		y1=y2;
	}while((y1-sy)<eps || (sx-x1)>eps);
}

void plotPoints(int cx, int cy, int x, int y)
{
	PutPixel(cx+x, cy+y, '*');
	PutPixel(cx-x, cy+y, '*');
	PutPixel(cx+x, cy-y, '*');
	PutPixel(cx-x, cy-y, '*');
	PutPixel(cx+y, cy+x, '*');
	PutPixel(cx-y, cy+x, '*');
	PutPixel(cx+y, cy-x, '*');
	PutPixel(cx-y, cy-x, '*');
}

void bresenhamCircle(int cx,int cy,int r){
	int x = 0, y, p;
	y = r;
	p = 3 - 2 * r;

	while (x < y) {
		plotPoints(cx,cy,x,y);
		x++;
		if (p < 0) {
			p = p + 4 * x + 6;
		} else {
			y--;
			p = p + 4 * (x - y) + 10;
		}
		plotPoints(cx,cy,x,y);
	}
}

float calculaErro(Ponto vert1, Ponto vert2){
	float a, b, erro = 0.0, yReal, erroMedio;
	int i, j, npontos = 0;
	if(vert1.a == vert2.a || vert1.b == vert2.b){
		return 0.0;
	}
	a = (vert2.b - vert1.b)/(vert2.a - vert1.a);
	b = (vert2.b)-(a*vert2.a);

	for (i = 0; i < TAM_X; i++) {
		for (j = 0; j < TAM_Y; j++) {
			if (FrameBuffer[i][j] == '*') {
				yReal = a*i + b;
				erro += fabs(yReal - j);
				npontos++;
				//printf("%f\n", erro );
			}
		}
	}

	erroMedio = erro/npontos;
	return erroMedio;
}

void erroMedio(int n){
	int i;
	Ponto vert1, vert2;
	float erroRetasDDA = 0.0, erroTotalDAA, erroRetasBres = 0.0, erroTotalBres;
	srand(time(NULL));
	for (i = 0; i < n; i++){
		vert1.a = rand()%TAM_X;
		vert1.b = rand()%TAM_Y;
		//printf("1 par: (%d,%d)\n", vert1.a, vert1.b );

		vert2.a = rand()%TAM_X;
		vert2.b = rand()%TAM_Y;
		//printf("2 par: (%d,%d)\n", vert2.a, vert2.b );

		LineDDA1(vert1.a, vert1.b, vert2.a, vert2.b, FALSE, FALSE);
		erroRetasDDA += calculaErro(vert1,vert2);
		//printBuffer();
		//printf("%f\n", erroRetasDDA );
		clearBuffer();

		bresenham(vert1.a, vert1.b, vert2.a, vert2.b, FALSE, FALSE);
		erroRetasBres += calculaErro(vert1, vert2);
		//printBuffer();
		//printf("%f\n", erroRetasBres );
		clearBuffer();
	}
	erroTotalDAA = erroRetasDDA/n;
	erroTotalBres = erroRetasBres/n;

	printf("Erro medio DDA: %.5f\n", erroTotalDAA );
	printf("Erro medio Bresenham: %.5f\n", erroTotalBres );
}

int main(){
	clearBuffer();
	erroMedio(10);
}
