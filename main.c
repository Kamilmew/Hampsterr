#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
// поиск в глубину
void DFS(int i, int j, int iw, int ih, unsigned char* grath, int* res, int col){
    res[iw*i+j] = col;
    if((i >= 1)&&(i <=ih - 1)&&(j >= 3)&&(j <= iw + 1)){
        if((abs(grath[iw*i+j] - grath[iw*(i)+(j-2)]) <= 75)){
            if((res[iw*(i) + (j-2)] == 0)){
                DFS(i, j-2, iw, ih, grath, res, col);
            }
        }
    }
    if((i >= 3)&&(i <=ih + 1)&&(j >= 0)&&(j <= iw - 2)){
        if((abs(grath[iw*i+j] - grath[iw*(i-2)+(j+1)]) <= 75)){
            if(res[iw*(i-2) + (j+1)] == 0){
                DFS(i-2, j+1, iw, ih, grath, res, col);
            }
        }
    }
    if((i >= - 1)&&(i <= ih - 3)&&(j >= 0)&&(j <= iw - 2)){
        if((abs(grath[iw*i+j] - grath[iw*(i+2) + (j+1)]) <= 75)){
            if(res[iw*(i+2) + (j+1)] == 0){
                DFS(i+2, j+1, iw, ih, grath, res, col);
            }
        }
    }
    return;
}
//предобработка
unsigned char*  gray_to_bw( unsigned char* Image, int sizeV, int sizeH, int t_black, int t_white) {
    for (int i = 2; i < sizeH-1; i++) {
        for (int j = 2; j < sizeV-1; j++) {
            if (Image[sizeV*i+j] < t_black) Image[sizeV*i+j] = 0;
            if (Image[sizeV*i+j] > t_white) Image[sizeV*i+j] = 255;
        }
    }
}

int main(){
    int iw, ih, n, i, j, k=0,check=0;

    // Загружаем изображение, чтобы получить информацию о ширине, высоте и цветовом канале
    unsigned char *idata=stbi_load("hamster.png", &iw, &ih, &n, 0);
    if (idata==NULL){
        printf("ERROR: can't read file\n");
        return 1;
    }

    //printf("iw=%d \n",iw,);

    //Переходим от четырёхканального изображения к одноканальному
    unsigned char* MyImage=(unsigned char*)malloc(iw*ih*sizeof(unsigned char));
    for (i=0; i<ih*iw*n; i=i+n){
        MyImage[k] = 0.299*idata[i] + 0.587*idata[i + 1] + 0.114*idata[i + 2];
        k = k + 1;
    }
    int t_black = 100;
    int t_white = 155;
    gray_to_bw(MyImage, iw, ih, t_black, t_white);


    /*for(i=1;i<ih-1;i++)
    {
        for(j=1;j<iw-1;j++)
        {
            MyImage[i*iw+j]+=(int)MyImage[iw*(i-1)+j-1]*(0.0924)+(int)MyImage[iw*(i-1)+j]*(0.1192)+(int)MyImage[iw*(i-1)+j+1]*(0.0924)+(int)MyImage[iw*i+j-1]*(0.1192)\
            +(int)MyImage[iw*i+j]*(0.1538)+(int)MyImage[iw*i+j+1]*(0.1192)+(int)MyImage[iw*(i+1)+j-1]*(0.0924)+(int)MyImage[iw*(i+1)+j]*(0.1192)+(int)MyImage[iw*(i+1)+j+1]*(0.0924);
        }
    }*/

    int col_num = 0;
    int* mas=(int*)malloc((iw*ih)*sizeof(int));
    for(i = 0; i < iw*ih; i++)
        mas[i] = 0;
    for (i = 1; i <= ih - 1; i++){
        for (j = 1; j <= iw - 1; j++){
            if(mas[iw*i+j] == 0){
                col_num++;
                DFS(i,j,iw,ih,MyImage,mas,col_num);
            }
            //printf("mew");
        }
    }
    // красим
    unsigned char* odata=(unsigned char*)malloc(iw*ih*n*sizeof(unsigned char));
    k=0;
    int color;
    for (i=0; i < ih*iw*n; i = i+n){
        color = mas[k]%70+ mas[k]%12;
        odata[i] = 5*color - 10 + mas[k]%40;
        odata[i+1] = 1.5*color/2 + 140;
        odata[i+2] = 3*color + 49;
        if ( n == 4)
            odata[i+3] = 255;
        k = k + 1;
    }
    /*for (i = 0; i < iw*ih; i++) {
        odata[i*n] = 78+col[i]+0.5*col[i-1];
        odata[i*n+1] = 46+col[i];
        odata[i*n+2] = 153+col[i];
        if (n == 4) odata[i*n+3] = 255;
    }*/
    // записываем картинку
    stbi_write_png("output.png", iw, ih, n, odata, 0);
    stbi_image_free(idata);
    stbi_image_free(MyImage);
    stbi_image_free(odata);
    return 0;
}
