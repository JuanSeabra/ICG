#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAM_X 80
#define TAM_Y 80
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


void LineDDA(int x1, int y1, int x2, int y2, short espessura, short tracejada){

	float xi, yi, x, y;
	int i, s, dx, dy;
	short status = TRUE;

	dx = x2-x1;
	dy = y2-y1;

	if(abs(dx)>abs(dy)) {
		s = abs(dx);
	}
	else s = abs(dy);

	xi = dx/ (float) s;
	yi = dy/ (float) s;

	x = x1;
	y = y1;

	PutPixel(x1,y1, '*');

	for (i=0; i< s; i++) {
		x+=xi;
		y+=yi;
		if(tracejada){
			if(status){
				PutPixel(round(x),round(y), '*');
				if (espessura) {
					if (x1 == x2) {
						PutPixel(round(x)+1,round(y), '*');
					} else {
						PutPixel(round(x),round(y)+1, '*');
					}
				}
				status = FALSE;
			} else { 
				status = TRUE;
			}
		} else {
			PutPixel(round(x),round(y), '*');
			if (espessura) {
				if (x1 == x2) {
					PutPixel(round(x)+1,round(y), '*');
				} else {
					PutPixel(round(x),round(y)+1, '*');
				}
			}

		}
	}
}

/*void bresenham1(int x1, int y1, int x2, int y2){        
  int slope;
  int dx, dy, d, x, y;
  if (x1 > x2){
  bresenham1(x2, y2, x1, y1);
  return;
  }        
  dx = x2 - x1;
  dy = y2 - y1;

  if (dy < 0){            
  slope = -1;
  dy = -dy;
  } else{            
  slope = 1;
  }
  d = 2 * dy - dx;
  y = y1;       
  for (x = x1; x <= x2; x++){
  PutPixel(x, y, '*');
  if (d <= 0){
  d += 2 * dy;
  } else{
  d += 2 * dy - 2 * dx;
  y += slope;
  }
  }
  }*/

void swap(int *x, int *y) {
	int k = *x;
	*x = *y;
	*y = k;
}

void bresenham(int x1, int y1, int x2, int y2, short espessura, short tracejada) {
	int dx,dy,p,k;
	float m;
	short status = TRUE;

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
		if(vert[i].a == vert[i+1].a)
			LineDDA(vert[i].a,vert[i].b, vert[i+1].a, vert[i+1].b,
					espessura, tracejada);
		else 
			bresenham(vert[i].a,vert[i].b, vert[i+1].a, vert[i+1].b,
					espessura,tracejada);
	}
}

void Polygon(Ponto vert[], int tam, short espessura, short tracejada){
	int i;
	for(i = 0; i < tam-1; i++){
		if(vert[i].a == vert[i+1].a)
			LineDDA(vert[i].a,vert[i].b, vert[i+1].a, vert[i+1].b,
					espessura, tracejada);
		else 
			bresenham(vert[i].a,vert[i].b, vert[i+1].a, vert[i+1].b, 
					espessura, tracejada);
	}
	if(vert[i].a == vert[0].a)
		LineDDA(vert[i].a,vert[i].b, vert[0].a, vert[0].b,
				espessura, tracejada);
	else 
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

int main(){
	Ponto vert[4];
	vert[0].a = 10; vert[0].b = 30;
	vert[1].a = 30; vert[1].b = 10;
	vert[2].a = 50; vert[2].b = 10;
	vert[4].a = 30; vert[4].b = 50;
	vert[3].a = 50; vert[3].b = 50;
	clearBuffer();
	//Polygon(vert,5,TRUE,FALSE);
	bresenhamCircle(50,50,10);
	printBuffer();
	/*clearBuffer();
	  printf("tentando separado\n\n");
	  bresenham1(10,0,10,30);
	  printBuffer();
	  printf("tentando com DDA\n\n");
	  LineDDA(10,0,10,30);
	  printBuffer();*/
}
