/*Andrew Yang
  SENG265
  9/10/17
  V008787595
*/

#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]){

	//freopen((argv[argc -1]),"w",stdout);

	int d = 0, mode = 0, tmp, tmpIndex;
	int array_len;
	FILE *in_file;
	FILE *out_file;
	
	char infile[10];
	char sortMode[10];
	char printWords[15];
	
	strcpy(infile, "--infile");
	strcpy(sortMode, "--sort");
	strcpy(printWords, "--print-words");
	
	// checks to make sure a filename is included
	if(argc < 3){
		fprintf(stderr, "You must provide a filename\n");
		exit(1);
	}
	
	// checks to find out which argument to read the filename and opens the file
	
	//open file
	for(int m = 0; m < argc; m++){
		if(strstr(argv[m], ".txt") != NULL){
			in_file = fopen(argv[m], "r");
		}
	}

	if (in_file == NULL){
        	fprintf(stderr, "unable to open %s\n", argv[2]);
                exit(1);
        }
	
	
	// checks if how we sort the output and if we need to include the words
	for(int k = 1; k < argc - 1; k++){
		if(strcmp(argv[k], infile) == 0 && mode < 1 ){
			mode = 1;
		}else if(strcmp(argv[k], sortMode) == 0){
			mode = 2;
		} 

		if(strcmp(argv[k], printWords) == 0){
			mode++;
		}
	}
	/* deals with reading the opened file and counting the lengths of each word.
	   we then output sorted by word length ascending.
	*/
	if((mode) == 1){
		//x is stored as each word. pch is for strok
		char x[35];
		char *pch;
		
		// 2d array for storage of wordlength ([][0]) and number of words of
		// that length ([][1])
        	static int partAlen[35][2];
		
		// reads each word, tokenizes that word with the added delimiters,
		// takes the length of the token and adds one to the corresponding length
        	while (fscanf(in_file, "%35s", x) == 1){
                	pch = strtok (x, " .,;:!?\n\0");
			while(pch != NULL){
				partAlen[strlen(pch)][1]++;
				pch = strtok (NULL," .,;:!?\n\0");
			}
        	}
		
		// takes the size of PartAlen in bytes and divides it by the size in bytes
		// of each int (4 bytes). Could've just used 35 here though.
		array_len = sizeof(partAlen)/sizeof(int);
		
		// prints out the output in ascending order of word length
		out_file = freopen((argv[argc -1]),"w",stdout);
		
		for(int i = 0; i< array_len; i++){
			partAlen[i][0] = i;
			if(partAlen[i][1]!= 0){
				if(i < 10){
					printf("Count[0%d] = %d;\n", i, partAlen[i][1]);
				}else{
					printf("Count[%d] = %d;\n", i, partAlen[i][1]);
				}
			}
		}
	}
	/* deals with reading the opened file and counting the lengths of each word.
	   we then use insertion sort because with small sizes it can be optimal.
	   there aren't very many words longer than 10 letters so within our normal
	   size range this is fastest. We then output in ascending order of frequency.
	*/
	else if((mode) == 2){
	
	        char x[35];
                char *pch;

                static int partBlen[35][2];

                while (fscanf(in_file, "%35s", x) == 1){
                        pch = strtok (x, " .,;:!?\n\0");
                        while(pch != NULL){
                                partBlen[strlen(pch)][1]++;
                                pch = strtok (NULL," .,;:!?\n\0");
                        }
                }

                array_len = sizeof(partBlen)/sizeof(int);
		
		// insertion sort algorithm to things is ascending order.
                for(int i = 0; i < array_len; i++){
			partBlen[i][0] = i;
			d = i;
			
			// insertion sort allows us to keep rule 1 property in the case of a tie
			while(d > 0 && partBlen[d][1] > partBlen[d-1][1]){
				tmp = partBlen[d][1];
				tmpIndex = partBlen[d][0];

				partBlen[d][1] = partBlen[d-1][1];
				partBlen[d][0] = partBlen[d-1][0];

		 		partBlen[d-1][1] = tmp;
				partBlen[d-1][0] = tmpIndex;

				d--; 
			}
                }
		out_file = freopen((argv[argc -1]),"w",stdout);;
		
		// prints out the contents of the array in ascending order if it's 
		// frequency is greater than 0.
		for(int j = 0; j < array_len; j++){
			if(partBlen[j][1] != 0){
				if(partBlen[j][0] < 10){
					printf("Count[0%d] = %d;\n", partBlen[j][0], 
					partBlen[j][1]);
				}else{
					printf("Count[%d] = %d;\n", partBlen[j][0],
					partBlen[j][1]);
				}
			}
		}
	}/*else if((mode) == 4){
		//x is stored as each word. pch is for strok
                char x[35];
                char *pch;
		char tmp[5000];

                // 2d array for storage of wordlength ([][0]) and number of words of
                // that length ([][1])
                static int partClen[35][2];

		const char *listWords[35][2];

                // reads each word, tokenizes that word with the added delimiters,
                // takes the length of the token and adds one to the corresponding length
                while (fscanf(in_file, "%35s", x) == 1){
                        pch = strtok (x, " .,;:!?\n\0");
                        while(pch != NULL){
				if(listWords[strlen(pch)][1] != 0){
					strcpy(listWords[strlen(pch)][0],listWords[strlen(pch)][1]);
					strcat(listWords[strlen(pch)][0], ',');
				}else if(partClen[strlen(pch)][1] == 0){
					strcpy(listWords[strlen(pch)][1], "(words: ");
				}
			
                                partClen[strlen(pch)][1]++;
				//tmp = listWords[strlen(pch)];
				if(strstr(listWords[strlen(pch)], pch) != NULL){
					strcat(listWords[strlen(pch)][1], '"');
					strcat(listWords[strlen(pch)][1], pch);
					strcat(listWords[strlen(pch)][1], '"'); 
				}
				
                                pch = strtok (NULL," .,;:!?\n\0");
                        }
                }

                // takes the size of PartAlen in bytes and divides it by the size in bytes
                // of each int (4 bytes). Could've just used 35 here though.
                array_len = sizeof(partClen)/sizeof(int);

                // prints out the output in ascending order of word length
                for(int i = 0; i< array_len; i++){
                        partClen[i][0] = i;
                        if(partClen[i][1]!= 0){
				if(listWords[i][0] != 0){
					strcat(listWords[i][0], " and ");
				}
				
				strcat(listWords[i][0], listWords[i][1]);
                                if(i < 10){
                                        printf("Count[0%d] = %d; %s\n", i, partClen[i][1], listWords[i][0]);
                                }else{
                                        printf("Count[%d] = %d;\n", i, partClen[i][1], listWords[i][0]);
                                }
                        }
                }
		
	}*/

	fclose(in_file);				 
}
