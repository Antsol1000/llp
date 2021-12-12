#include "header.h"


int main(int argc, char **argv) {
    BITMAPFILEHEADER h1;
    BITMAPINFOHEADER h2;

    BITMAPFILEHEADER* h1p = &h1;
    BITMAPINFOHEADER* h2p = &h2;

    FILE * fp; FILE * fd;

    fp = fopen(argv[1], "rb");

    	/// reading headers
    h1 = read_bmp_F_h(h1, fp);
    h2 = read_bmp_I_h(h2, fp);
        ///displaying headers
    print_fh(h1);
    print_ih(h2);
        ///histpgram
    if(argc == 2){histogram(fp, h1p, h2p);fclose(fp);}
    	//grayscale
    if(argc == 3){
        fd = fopen(argv[argc - 1], "w+b");
        grayscale(fp, fd, h1p, h2p);
        fclose(fd);
        fclose(fp);}
	//steganografia
    if(argc == 4){
        char opinion; int result;
        fd = fopen(argv[argc - 2], "w+b");
        result = encode(fp, fd, h1p, h2p,argv[argc - 1], strlen(argv[argc - 1]));
        fclose(fp);
        if(result == 1){
        printf ("\nDecode steganography? [Y/n] ");
            scanf ("%c",&opinion);
            if(opinion == 'Y'){
                fd = fopen(argv[argc - 2], "rb");
                decode(fd, h1p, h2p);
                fclose(fd);
                }
            else{printf("That's fine, Goodbye!\n\n");
                printf("▓▓▓▓\n▒▒▒▓▓\n");
                printf("▒▒▒▒▒▓\n▒▒▒▒▒▒▓\n");
                printf("▒▒▒▒▒▒▓\n");
                printf("▒▒▒▒▒▒▒▓\n▒▒▒▒▒▒▒▓▓▓\n▒▓▓▓▓▓▓░░░▓\n▒▓░░░░▓░░░░▓\n");
                printf("▓░░░░░░▓░▓░▓\n▓░░░░░░▓░░░▓\n▓░░▓░░░▓▓▓▓\n▒▓░░░░▓▒▒▒▒▓\n");
                printf("▒▒▓▓▓▓▒▒▒▒▒▓\n▒▒▒▒▒▒▒▒▓▓▓▓\n▒▒▒▒▒▓▓▓▒▒▒▒▓\n▒▒▒▒▓▒▒▒▒▒▒▒▒▓\n");
                printf("▒▒▒▓▒▒▒▒▒▒▒▒▒▓\n▒▒▓▒▒▒▒▒▒▒▒▒▒▒▓\n▒▓▒▓▒▒▒▒▒▒▒▒▒▓\n");
                printf("▒▓▒▓▓▓▓▓▓▓▓▓▓\n▒▓▒▒▒▒▒▒▒▓\n▒▒▓▒▒▒▒▒▓\n");}}
         else{fclose(fd);}}



return 0;

}

