/* Andrew Yang
   SENG265
   10/22/17
   V00878595
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Nameval Nameval;
static void *emalloc(size_t n);
Nameval *newitem(int name, char *words);
Nameval *addfront(Nameval *listp, Nameval *newp);
void apply(Nameval *listp, void(*fn)(Nameval*, void*), void *arg);
void freeall(Nameval *listp);
Nameval *lookup(Nameval *listp, int name);
Nameval *initialize(char *buffer);
Nameval *insertion_sort_1(Nameval *head);
Nameval *insertion_sort_2(Nameval *head);
void infile_mode(Nameval *head, int print_flag);
void sort_mode(Nameval *head, int print_flag);
void print_data(Nameval *head);
void print_words_data(Nameval *head);
Nameval *create_list_words(Nameval *node);
Nameval *insertion_sort_words(Nameval *head);
char *str_to_lower(char *str);

int main(int argc, char *argv[]){
	// in_file is the file we open
    FILE *in_file;
    
    // buffer to hold all our strings
	char *buffer;
    
    // our flags to determine which modes to enter
	int infile_flag = 0;
	int sort_flag = 0;
	int print_flag = 0;
	int file_length;
    
    // checks if we give an argument that is too short to contain
    // a filename.
	if(argc < 3){
		fprintf(stderr, "You must provide a filename\n");
		exit(1);
	}
    
    // checks for the ".txt" extension and opens the argv[element]
    // where the filename exists
	for(int m = 0; m< argc ; m++){
		if(strstr(argv[m],".txt") != NULL){
			in_file = fopen(argv[m], "r");
		}
	}
    
    // error message for if the file doesn't open anything
	if (in_file == NULL){
		fprintf(stderr, "unable to open %s\n", argv[2]);
		exit(1);
	}
    
    // sets our flags according to the commands we give the program
	for(int k = 1; k<argc-1; k++){
		if(strcmp(argv[k], "--file") == 0){
			infile_flag = 1;
		}
		if(strcmp(argv[k], "--sort") == 0){
			sort_flag = 1;
		}
		if(strcmp(argv[k], "--print-words") == 0){
			print_flag = 1;
		}
	}
	
    // this block checks the length of the buffer space we need to
    // allocate
	fseek(in_file, 0L, SEEK_END);
	file_length = ftell(in_file);
	rewind(in_file);
    
    // allocates the buffer space and sets the null terminator 
	buffer = (char*)emalloc((file_length + 1) * sizeof(*buffer));
	fread(buffer, file_length, 1, in_file);
	buffer[file_length] = '\0';
    
    // close the file because we're done with it
	fclose(in_file);
	
    // logic handling different method calls depending on the flags 
    // we set.
	if(sort_flag == 1 && print_flag == 0){
		sort_mode(initialize(buffer), 0);
	}else if(sort_flag == 1 && print_flag == 1){
		sort_mode(initialize(buffer), 1);
	}else if(sort_flag  == 0 && print_flag == 0){
		infile_mode(initialize(buffer), 0);
	}else if(sort_flag == 0 && print_flag == 1){
		infile_mode(initialize(buffer), 1);
	} 	

	
    // return the space allocated to buffer
	free(buffer);
}

// our "node" for the linked list
struct Nameval{
	int name;
	char *words;
	int frequency;
	Nameval *next;
}; // Nameval;

// emalloc is malloc but automatically checks if allocation works or
// not
static void *emalloc(size_t n){
	void *p;
	
	p = malloc(n);
	if(p == NULL){
		fprintf(stderr, "malloc of %zu bytes failed", n);
		exit(1);
	}
	return p;
}

// initialize is a method that takes our char* buffer and creates a
// linked list of nodes with name of the size, concatenates the words
// onto the node->words string if there are no repititions
Nameval *initialize(char *buffer){
	char *token;
	Nameval *mode_one = NULL;
    
    // splits our buffer into tokens determined by the delimiters
	token = strtok(buffer, " .,;:!?()\n\0");
	while(token){
		if(lookup(mode_one, strlen(token)) != NULL){
			Nameval *tmp_node = lookup(mode_one, strlen(token));
            
            // checks if the token is already in our node->words string
            // if not, concatenate it plus a space.
			if(strstr(tmp_node->words, token) == NULL){
				size_t len1 = strlen(tmp_node->words), 
					len2 = strlen(token);
                        	char *tmp_cat = (char *)emalloc(len1 + len2 + 2);

				// creates the new string of word length
				*(tmp_cat + (len1+ len2 + 1)) = '\0';
				memcpy(tmp_cat, tmp_node->words, len1);
				memset(tmp_cat+len1, ' ', 1);
				memcpy(tmp_cat+len1+1, token, len2);
				
				//printf("%s\n", tmp_cat);
				tmp_node->words = strdup(tmp_cat);

				free(tmp_cat);
			}
			tmp_node->frequency = (tmp_node->frequency + 1);
		}else{
			Nameval *newnode = newitem(strlen(token), token);
			mode_one = addfront(mode_one, newnode);
		}
		token = strtok(NULL," .,;:|?()\n\0"); 		
	}
	return mode_one;
}

// infile_mode is a method that sorts our linked list by word length
// ascending and then determines if we need to print out the words
// using the print_flag variable.
void infile_mode(Nameval *head, int print_flag){
	//printf("infile_mode entered...\n");
	// sort the values by word length
	Nameval *infile_mode = insertion_sort_1(head);
	
	if(print_flag == 0){
		// print out in order the values
		print_data(infile_mode);
	}else if(print_flag == 1){
		print_words_data(infile_mode);
	}
    
    // frees all our nodes on the linked list
	freeall(infile_mode);			 
}

// sort_mode is a method that sorts our linked list by frequency
// ascending and then determines if we need to print out the words
// using the print_flag variable.
void sort_mode(Nameval *head, int print_flag){
	printf("sort_mode entered...\n");
	// sort the nodes by frequency
	Nameval *sort_mode = insertion_sort_2(head);
	
	if(print_flag == 0){
		//print out in order the values
		print_data(sort_mode);
	}else if(print_flag == 1){
		printf("print_words_data calling...");
		print_words_data(sort_mode);	
	}
    
    // frees all the nodes on the linked list
	freeall(sort_mode); 
}	

// this method is called if we only need to print the word length and
// frequency. takes a parameter and points to it so we know where to
// start printing.
void print_data(Nameval *head){
	Nameval *x;
	x = head;
	//int i = 0;
	
	//printf("printing...\n");
	while(x != NULL){
		printf("Count[%02d]=%02d;\n", x->name, x->frequency);
		x = x->next;
		//free(x->words);
		//x->words = NULL;
	}
}

// this method is called if we need to print the words and has logic
// that creates the correct output for our print line.
void print_words_data(Nameval *head){
	Nameval *x, *y, *e;
	int count;
	x = head;
	
	//printf("print_words_data entered...\n");

	while(x != NULL){
		count = 1;
		y = insertion_sort_words(create_list_words(x));
		printf("Count[%02d]=%02d; (words:", x->name, x->frequency);
		while(y != NULL){
			if((y->next == NULL) && (count != 1)){
				printf("and \"%s\")\n", y->words);
			}else if((y->next == NULL) && (count == 1)){
				printf(" \"%s\")\n", y->words);
			}else if(y->next != NULL){
				e = y->next;
				if(e->next == NULL){
					printf(" \"%s\" ", y->words);	
				}else{
					printf(" \"%s\",", y->words);
				}
			}
			y = y->next;
			count++;
		}
		x = x->next;
	} 			
}			

// checks our tokens and turns them into lower case words
char *str_to_lower(char *str){
	unsigned char *mystr = (unsigned char*)emalloc(strlen(str));
	mystr = (unsigned char *)str;

	while(*mystr){
		*mystr = tolower(*mystr);
		mystr++;
	}
	return str;
}

// constructor for our node class
Nameval *newitem(int name, char *string){
	Nameval *newp;

	newp = (Nameval *)emalloc(sizeof(Nameval));
	newp->name = name;
	newp->words = strdup(string);
	newp->frequency = 1;
	newp->next = NULL;
	return newp;
}

// adds the node to the front of the list
Nameval *addfront(Nameval *listp, Nameval *newp){
	newp->next = listp;
	return newp;
}

Nameval *create_list_words(Nameval *node){
	//printf("create_list_words...\n");
	char *token;
	Nameval *ind_word;
	int id = 0;

	token = strtok(node->words, " \0");
	while(token){
		token = str_to_lower(token);
		Nameval *newnode = newitem(id, token);
		ind_word = addfront(ind_word,newnode);
		token = strtok(NULL, " \0");
		id++;
		//printf("%d", id);
	}
	
	return ind_word;		
}

// this method sorts the word linked list for part c. sorts in ascending order
Nameval *insertion_sort_words(Nameval *head){
	Nameval *x, *y, *e;

	x = head;
	head = NULL;

	while(x != NULL){
		e = x;
		x = x->next;
		if(head != NULL){
			if(strcmp(e->words, head->words) > 0){
				y = head;
				while((y->next != NULL) && (strcmp(e->words, y->next->words) > 0)){
					y = y->next;
				}

				e->next = y->next;
				y->next = e;
			}else{
				e->next = head;
				head = e;
			}
		}else{
			e->next = NULL;
			head = e;
		}
	}
	return head;			 			
}

// this method sorts our linked list by word length in ascending order
Nameval *insertion_sort_1(Nameval *head){
	Nameval *x, *y, *e;

	x = head;
	head = NULL;

	while(x != NULL){
		e = x;
		x = x->next;
		if(head != NULL){
			if(e->name > head->name){
				y = head;
				while((y->next != NULL) && (e->name > y->next->name)){
					y = y->next;
				}

				e->next = y->next;
				y->next = e;
			}else{
				e->next = head;
				head = e;
			}
		}else{
			e->next = NULL;
			head = e;
		}
	}
	return head;
}

// this method sorts our linked list by frequency in ascending order
Nameval *insertion_sort_2(Nameval *head){
	Nameval *x, *y, *e;

	x = head;
	head = NULL;
		
	while(x != NULL){
		e = x;
		x = x->next;
		if(head != NULL){
			if(e->frequency < head->frequency){
				y = head;
				while((y->next != NULL) && (e->frequency < y->next->frequency)){
					y = y->next;
				}

				e->next = y->next;
				y->next = e;
			}else{
				e->next = head;
				head = e;
			}
		}else{
			e->next = NULL;
			head = e;
		}
	}
	return head;
}		

// this method looks in the list for a node with the node->name = name
// returns the address if it finds the location, returns NULL if it doens't exist in the list
Nameval *lookup(Nameval *listp, int name){
	for( ; listp != NULL; listp = listp->next){
		if(name == listp->name){
			return listp;
		}
	}
	return NULL;
}

// applies a function to all nodes of the linked list.
void apply(Nameval *listp, void (*fn)(Nameval*, void*), void *arg){
	for( ; listp != NULL; listp = listp->next){
		(*fn)(listp, arg); /* call the function */
	}
}

// iterates through the nodes of the linked list and frees the memory allocated
void freeall(Nameval *listp){
	Nameval *next;

	for( ; listp != NULL; listp = next){
		next = listp->next;
		free(listp->words);
        // listp-> name and other variables are freed elsewhere
		free(listp);
        
	}
}


	
