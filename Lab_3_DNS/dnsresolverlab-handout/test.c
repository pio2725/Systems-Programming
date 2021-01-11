#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

void print_bytes(unsigned char *bytes, int byteslen);

int main() {

    
	typedef struct {
		unsigned short randomId;
		unsigned short flags;
		unsigned short numQuestion;
		unsigned short answer_rr;
		unsigned short authority_rr; // always 00
		unsigned short additional_rr; // always 00
	} dns_query_header;

	dns_query_header *header = malloc(sizeof(dns_query_header));
    
	srandom(time(0));
	unsigned short randomNum = random();
	header->randomId = randomNum;
	header->flags = 0x001;
	header->numQuestion = 0x0100;
	header->answer_rr = 0x0000;
	header->additional_rr = 0x0000;
	header->authority_rr = 0x0000;


    unsigned char msg[29];
    memcpy(msg, (unsigned char*)header, sizeof(dns_query_header));

    
    
    
    char* qname = "www.example.com";
    char name[sizeof(qname)];
    strcpy(name, qname);

    char* question = (char*)malloc((strlen(qname)+2) * sizeof(char));
    
    char* asdf = malloc(sizeof(char)*30);
    
    printf("size of question = %ld\n", sizeof(asdf));



    const char s[2] = ".";
    
    char* token = strtok(name, s);

    int index = 0;
    while (token != NULL) {

        int length = strlen(token);

        question[index] = length;
        ++index;

        for (int i = 0; i < length; i++) {
            question[index] = token[i];
            ++index;
        }
        token = strtok(NULL, s);
    }    
    question[index+1] = 0x00;

    
    //print_bytes(question, strlen(qname) + 2);
    //memcpy(msg+12, question, sizeof(question));

    //print_bytes(msg, 12);
    

    print_bytes(msg, 12);


    return 0;
}

void print_bytes(unsigned char *bytes, int byteslen) {
	int i, j, byteslen_adjusted;
	unsigned char c;

	if (byteslen % 8) {
		byteslen_adjusted = ((byteslen / 8) + 1) * 8;
	} else {
		byteslen_adjusted = byteslen;
	}
	for (i = 0; i < byteslen_adjusted + 1; i++) {
		if (!(i % 8)) {
			if (i > 0) {
				for (j = i - 8; j < i; j++) {
					if (j >= byteslen_adjusted) {
						printf("  ");
					} else if (j >= byteslen) {
						printf("  ");
					} else if (bytes[j] >= '!' && bytes[j] <= '~') {
						printf(" %c", bytes[j]);
					} else {
						printf(" .");
					}
				}
			}
			if (i < byteslen_adjusted) {
				printf("\n%02X: ", i);
			}
		} else if (!(i % 4)) {
			printf(" ");
		}
		if (i >= byteslen_adjusted) {
			continue;
		} else if (i >= byteslen) {
			printf("   ");
		} else {
			printf("%02X ", bytes[i]);
		}
	}
	printf("\n");
}