#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	FILE * src=stdin;		//we save the input file in src, might change later from keyboard to file
	FILE * dst=stdout;		//we save the output file in dst, might change later from screen to file
	int encflag=0,i;		//flag for encoding
	int hexaNum=0;			//hexaNum- the number after +/-e_
	int changed=0;			//changed- flag for changed input mode
	int debflag=0;			//flag of debug		     
	int caps=0;			//CAPITAL counter
    	int count=0;			//number of letters left to print/ignore 
    	int isfirst=0;			//flag for first char
	int debugchar;			//the char to print
	int baseNumber=0;		//the ascii code to reduce from the char(48 for 1-9, 55 for A-F)
	char ch='.';			//IMPORTANT: the current char we read from user/file is saved here	
    	char firstc='.';		//the first char we recieve (for +e_ encryption)
	int finish=0;
	for(i=1; i<argc; i++){
		if(strncmp(argv[i],"-D",2)==0){		// we search for the debug flag (-D) 
			printf("-D\n");
			debflag=1;			// debug mode - on
		}
		else if(strncmp(argv[i],"-i",2)==0){	// we search for the input flag (-i___)
			src=fopen(argv[i]+2,"r");
			changed=1;			// external input mode - on
		}
		else if (strncmp(argv[i],"-o",2)==0){	//we search for the output flag (-o___)
			dst=fopen(argv[i]+2,"w");
		}
		else if (argv[i][1]=='e'){		// we search for the encryption flag (-e_/+e_)
			encflag=1;			// encryption mode - on

			if (argv[i][2]>='1' && argv[i][2]<='9') baseNumber=48;//substruction
			else if(argv[i][2]>='A' && argv[i][2]<='F') baseNumber=55;//substruction
			if (argv[i][0]=='-') hexaNum-=argv[i][2]-baseNumber;
			if (argv[i][0]=='+') hexaNum+=argv[i][2]-baseNumber;
 			count=hexaNum;
		}
		else{
			printf("invalid parameter - %s\n",argv[i]);
			return 1;
		}
	}
	
	while(1){
        	ch=fgetc(src);		//here we read from src char by char
        	if(isfirst==0){		//if this is first letter (for e__)
            		isfirst=1;	//we change the flag from 0 to 1
            		firstc=ch;	//we keep the first char
        	}
		if (ch==10 && debflag!=0){	//we got \n in debug mode
			fprintf(stderr, "the number of letters: %d \n", caps);
			printf("\n");
			caps=0;
			continue;
		}
		if(ch==10 & encflag==1 & finish==0 & hexaNum<0) fputs("-NONE-\n",dst);	//we check if we need to print NONE or something was printed
        	
		if (ch==EOF) {							//we got EOF (^D)
			if (changed!=0) fputc('\n',dst);					
			break ;
		}
		debugchar=ch;							//previous char

		if (encflag==0 && ch>='A' && ch<='Z'){				//we change from capital to normal
			ch+=32;							//we add 1 more changed char
			caps++;
		}
		if (debflag!=0) fprintf(stderr, "%d\t%d\n", ch,debugchar);	//if we are in dubug mode - we print as needed

		if (hexaNum<0 && count<0 && ch!=32) count++;			//we ignore chars until count=0
		else if(hexaNum<0 && count<0 && ch==32){}			//we ignore spacebar if we didnt finish reading all the needed chars
        	else if(hexaNum<0 && count==0) {fputc(ch,dst); finish=1;}	//we print AND change the flag so we know not to print NOTHING
        	else if(hexaNum>0 && ch==10 && count>0){for(int i=0;i<count;i++)fputc(firstc,dst);}	//if we got \n in encode mode - we print the first letter "count" times
        	else fputc(ch,dst);						//none of the above
    }
	return(0);
}
