#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

///*********** T Y P E D E F **************

typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader

typedef struct tagBITMAPFILEHEADER {
  WORD  bfType;
  DWORD bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

// https://docs.microsoft.com/pl-pl/previous-versions/dd183376(v=vs.85)
typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

/// helpers
int validity_check(BITMAPINFOHEADER* h2){
    if(h2->biBitCount != 24 || h2->biCompression != 0){
    	printf("\nOparation failed to be conducted:");
    	if(h2->biBitCount != 24){printf("\n- biBitCount = %d, should be 24.", h2->biBitCount);}
    	if(h2->biCompression != 0){printf("\n- biCompression = %d, should be 0.\n", h2->biCompression);}
    	return 0;}
    else{return 1;}
}

void char_converter(char letter, uint8_t* array, int initial_position){
	uint8_t temp = (uint8_t) letter;
	for( int i = 0; i<8; i++){
		array[7 - i + initial_position] = temp%2;
		temp = temp/2;
		}
}

void modify(uint8_t* curr_byte, uint8_t dig){
	if ((*curr_byte)%2 != dig%2){
		if( dig == 1){ *curr_byte += 1;}
		else{ *curr_byte -= 1;}
	}}

///*********** H E A D E R S **************

//read
BITMAPFILEHEADER read_bmp_F_h(BITMAPFILEHEADER h, FILE* fp){
    fread(&(h.bfType), 2,1,fp);
    fread(&(h.bfSize), 4,1,fp);
    fread(&(h.bfReserved1), 2,1,fp);
    fread(&(h.bfReserved2), 2,1,fp);
    fread(&(h.bfOffBits), 4,1,fp);
    return h;
    };

BITMAPINFOHEADER read_bmp_I_h( BITMAPINFOHEADER h, FILE* fp){
    fread(&(h.biSize), 4,1,fp);
    fread(&(h.biWidth), 4,1,fp);
    fread(&(h.biHeight), 4,1,fp);
    fread(&(h.biPlanes), 2,1,fp);
    fread(&(h.biBitCount), 2,1,fp);
    fread(&(h.biCompression), 4,1,fp);
    fread(&(h.biSizeImage), 4,1,fp);
    fread(&(h.biXPelsPerMeter), 4,1,fp);
    fread(&(h.biYPelsPerMeter), 4,1,fp);
    fread(&(h.biClrUsed), 4,1,fp);
    fread(&(h.biClrImportant), 4,1,fp);

    return h;};

//print

void print_fh(BITMAPFILEHEADER h){
    printf("BITMAPFILEHEADER:\n");
    printf(" bfType:          %#x (%c%c)\n", h.bfType, h.bfType % 256, h.bfType / 256);
    printf(" bfSize:          %d\n", h.bfSize);
    printf(" bfReserved1:     0x%#x\n", h.bfReserved1);
    printf(" bfReserved2:     0x%#x\n", h.bfReserved2);
    printf(" bfOffBits:       %d\n", h.bfOffBits);
    }

void print_ih( BITMAPINFOHEADER h){
    printf("BITMAPINFOHEADER:\n");
    printf(" biSize:          %d\n", h.biSize);
    printf(" biWidth:         %d\n", h.biWidth);
    printf(" biHeight:        %d\n", h.biHeight);
    printf(" biPlanes:        %d\n", h.biPlanes);
    printf(" biBitCount:      %d\n", h.biBitCount);
    printf(" biCompression:   %d\n", h.biCompression);
    printf(" biSizeImage:     %d\n", h.biSizeImage);
    printf(" biXPelsPerMeter: %d\n", h.biXPelsPerMeter);
    printf(" biYPelsPerMeter: %d\n", h.biYPelsPerMeter);
    printf(" biClrUsed:       %d\n", h.biClrUsed);
    printf(" biClrImportant:  %d\n", h.biClrImportant);
    }

///*********** H I S T O G R A M **************
void print_stats( int stats[] , int len, int siz){
    for(int i = 0; i < len; ++i){
        printf(" %d-%d:  %.2f%%\n", 16*i, i*16+15, (double)stats[i]/((double)siz*0.01));
    }
}
void histogram( FILE* fp, BITMAPFILEHEADER* h1, BITMAPINFOHEADER* h2){

    if(validity_check(h2) == 1){

    int to_be_read =((24*h2->biWidth + 31)/32)*4;
    int padding = to_be_read - (h2->biWidth)*3;
    uint8_t* array_buffer = malloc(3*h2->biWidth + padding);
    int HIST[3][16] = {0};
    uint32_t siz = (h2->biWidth*h2->biHeight);


    fseek(fp,h1->bfOffBits,SEEK_SET);
    for(int row = 0; row<h2->biHeight; row++){
        fread(array_buffer,1,to_be_read, fp);
            for(int i = 0; i<(h2->biWidth)*3; i = i+3){
            //printf("PIX [%d][%d] -> R: %d G: %d B: %d, szuf: %d %d %d currently at: %ld\n", row, col, *red, *green, *blue,*red/16,*green/16, *blue/16, ftell(fp));
            HIST[0][(array_buffer[i+2])/16]+=1;
            HIST[1][(array_buffer[i+1])/16]+=1;
            HIST[2][(array_buffer[i])/16]+=1;
        }
    }
    free(array_buffer);
    //printing
    printf("\n\nHISTOGRAM:");
    printf("\nRED:\n");
    print_stats(HIST[0] , 16, siz);
    printf("\nGREEN:\n");
    print_stats(HIST[1] , 16, siz);
    printf("\nBLUE:\n");
    print_stats(HIST[2] , 16, siz);
    }
}

///*********** G R A Y S C A L E **************

void grayscale(FILE* fp, FILE* fd, BITMAPFILEHEADER* h1, BITMAPINFOHEADER* h2){
    if(validity_check(h2) == 1){
    uint8_t* red = malloc(1);
    uint8_t* blue = malloc(1);
    uint8_t* green = malloc(1);
    int padding = ((24*h2->biWidth + 31)/32)*4 - (h2->biWidth)*3;
    uint8_t grey; uint8_t tem[1];
    uint8_t grey_buff[3];
    tem[0] = (uint8_t)0;


   fseek(fp,0,SEEK_SET);
   uint8_t* buffer = malloc(h1->bfOffBits);
   fread(buffer, h1->bfOffBits,1,fp);
   fwrite(buffer, 1, h1->bfOffBits, fd);

   fseek(fp,h1->bfOffBits,SEEK_SET);
   fseek(fd,h1->bfOffBits,SEEK_SET);
    for(int row = 0; row<h2->biHeight; row++){
        for(int col = 0; col<h2->biWidth; col++){
            fread(blue, 1,1,fp);
            fread(green, 1,1,fp);
            fread(red, 1,1,fp);
            grey = *red/3 + *green/3 + *blue/3;
            grey_buff[0] = grey; grey_buff[1] = grey; grey_buff[2] = grey;
            fwrite(grey_buff, 1, 3, fd);
            //printf("PIX [%d][%d] -> R: %d G: %d B: %d, GREY: %d %d %d currently at: %ld in fp - currently at: %ld in fd \n", row, col, *red, *green, *blue,grey,grey,grey, ftell(fp),ftell(fd));

        }
        fseek(fp,padding,SEEK_CUR);
        for(int p = 0; p<padding; p++){
                fwrite(tem, 1, 1, fd);
        }
        }
    free(red);free(blue);free(green);free(buffer);

    }
}

int encode(FILE* fp, FILE* fd, BITMAPFILEHEADER* h1, BITMAPINFOHEADER* h2, char* array, int len){
    if(validity_check(h2) == 1){
	uint8_t byte[8] = {0};
	int iter = 0;
	uint8_t* curr_byte = malloc(1);
	uint8_t* binary_text = malloc((len+1)*8);
	int padding = ((24*h2->biWidth + 31)/32)*4 - (h2->biWidth)*3;
	uint8_t tem[1]; tem[0] = (uint8_t)0;

	char_converter(len, byte, 0);
	char_converter(len, binary_text, 0);

	for(int character = 0; character < len; character ++){
		char_converter(array[character],byte,0);
		char_converter(array[character], binary_text, (character + 1)*8);
		}

	fseek(fp,0,SEEK_SET);

	uint8_t* buffer = malloc(h1->bfOffBits);
	fread(buffer, h1->bfOffBits,1,fp);
	fwrite(buffer, 1, h1->bfOffBits, fd);

	fseek(fp,h1->bfOffBits,SEEK_SET);
	fseek(fd,h1->bfOffBits,SEEK_SET);
	for(int row = 0; row<h2->biHeight; row++){
		for(int col = 0; col<h2->biWidth; col++){
            		for(int j = 0; j < 3;j++){
				fread(curr_byte, 1,1,fp);
				if(iter< (len+1)*8){modify(curr_byte, binary_text[iter]);}
				fwrite(curr_byte, 1, 1, fd);
				iter++;
                }}
        fseek(fp,padding,SEEK_CUR);

        for(int p = 0; p<padding; p++){
                fwrite(tem, 1, 1, fd);
        }}

	free(binary_text);free(buffer); free(curr_byte);
	fclose(fd);
	return 1;}
	else{
        return 0;
	}}

void decode(FILE* fd, BITMAPFILEHEADER* h1, BITMAPINFOHEADER* h2){
	fseek(fd,h1->bfOffBits,SEEK_SET);
	printf("\n");
	int padding = ((24*h2->biWidth + 31)/32)*4 - (h2->biWidth)*3;
	uint8_t* curr_byte = malloc(1); char letter = (char) 0;
	int len=-1; int temp_len = 0; int iterator = 0;
	int row = 0;
	while(row<h2->biHeight && (len ==-1 || iterator < (len*8 + 8))){
		int col = 0;
		while(col<h2->biWidth*3 &&( len ==-1 || iterator < (len*8 + 8))){
			fread(curr_byte, 1,1,fd);
			if(len == -1){
				temp_len += (*curr_byte % 2)*(1 << (7 - iterator));
				if(iterator == 7){len = temp_len;}}
			else if(iterator % 8 == 0 && iterator>8){
				printf("%c", letter);
				letter = 0;
				letter += (*curr_byte % 2)*(1 << (7 - iterator%8));}
			else{
				letter += (*curr_byte % 2)*(1 << (7 - iterator%8));
			}
			iterator++;
			col++;}

		fseek(fd,padding,SEEK_CUR);
		row++;}
		if(iterator % 8 == 0 && len != -1){printf("%c", letter);}
	printf("\n");
	free(curr_byte);

}
