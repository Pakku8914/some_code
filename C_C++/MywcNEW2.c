#include <stdio.h>
#include <stdlib.h>


int Myisalph(unsigned char cha)
{
	return cha > 0x20;  
//	return (cha > 0x20) ? 1 : 0 ;
}


int Mywc(char inchar)
{
	static int state = 0x00;
	int wc = 0x0;
	
	switch (state) {
		case 0x00 :
			switch (Myisalph(inchar)) {
				case 0 :
		    		break;
		    	default :
		    		wc++;
		    		state = 0x01;
		    		break;
		    }
			break;
		case 0x01 :
		    switch (Myisalph(inchar)) {
		    	case 0 :
		    		state = 0x00;
		    		break;
		    	default :
		    		break;
		    	}
			break;
	}
	
	return wc;
}


int main(int argc, char **argv)
{
    FILE *fp;
    char c;
    int wc = 0;

	if (argc != 2) {
		printf("Usage: ./a file_name\n");
		exit(0);
	}
	
    if ((fp = fopen(*(argv + 1), "r")) == NULL) {
		printf("Can,t open %s\n", *(argv + 1));
        exit(1);
    }
    
    printf("file name = %s\n", *(argv + 1));
    
    while ((c = fgetc(fp)) != EOF) {    
    	printf("%c(0x%x),  ", c, c);
	   	wc += Mywc(c);  	
    }
    
    printf("\n\nword count = %d\n\n", wc);

    fclose(fp);
    

    return 0;
}
