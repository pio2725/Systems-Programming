#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<time.h>
#include<netdb.h>
#include<unistd.h>

typedef unsigned int dns_rr_ttl;
typedef unsigned short dns_rr_type;
typedef unsigned short dns_rr_class;
typedef unsigned short dns_rdata_len;
typedef unsigned short dns_rr_count;
typedef unsigned short dns_query_id;
typedef unsigned short dns_flags;

typedef struct {
	char *name;
	dns_rr_type type;
	dns_rr_class class;
	dns_rr_ttl ttl;
	dns_rdata_len rdata_len;
	unsigned char *rdata;
} dns_rr;

struct dns_answer_entry;
struct dns_answer_entry {
	char *value;
	struct dns_answer_entry *next;
};
typedef struct dns_answer_entry dns_answer_entry;

void free_answer_entries(dns_answer_entry *ans) {
	dns_answer_entry *next;
	while (ans != NULL) {
		next = ans->next;
		free(ans->value);
		free(ans);
		ans = next;
	}
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

void canonicalize_name(char *name) {
	/*
	 * Canonicalize name in place.  Change all upper-case characters to
	 * lower case and remove the trailing dot if there is any.  If the name
	 * passed is a single dot, "." (representing the root zone), then it
	 * should stay the same.
	 *
	 * INPUT:  name: the domain name that should be canonicalized in place
	 */
	
	int namelen, i;

	// leave the root zone alone
	if (strcmp(name, ".") == 0) {
		return;
	}

	namelen = strlen(name);
	// remove the trailing dot, if any
	if (name[namelen - 1] == '.') {
		name[namelen - 1] = '\0';
	}

	// make all upper-case letters lower case
	for (i = 0; i < namelen; i++) {
		if (name[i] >= 'A' && name[i] <= 'Z') {
			name[i] += 32;
		}
	}
}

int name_ascii_to_wire(char *name, unsigned char *wire) {
	/* 
	 * Convert a DNS name from string representation (dot-separated labels)
	 * to DNS wire format, using the provided byte array (wire).  Return
	 * the number of bytes used by the name in wire format.
	 *
	 * INPUT:  name: the string containing the domain name
	 * INPUT:  wire: a pointer to the array of bytes where the
	 *              wire-formatted name should be constructed
	 * OUTPUT: the length of the wire-formatted name.
	 */
}

char *name_ascii_from_wire(unsigned char *wire, int *indexp) {
	/* 
	 * Extract the wire-formatted DNS name at the offset specified by
	 * *indexp in the array of bytes provided (wire) and return its string
	 * representation (dot-separated labels) in a char array allocated for
	 * that purpose.  Update the value pointed to by indexp to the next
	 * value beyond the name.
	 *
	 * INPUT:  wire: a pointer to an array of bytes
	 * INPUT:  indexp, a pointer to the index in the wire where the
	 *              wire-formatted name begins
	 * OUTPUT: a string containing the string representation of the name,
	 *              allocated on the heap.
	 */
}

dns_rr rr_from_wire(unsigned char *wire, int *indexp, int query_only) {
	/* 
	 * Extract the wire-formatted resource record at the offset specified by
	 * *indexp in the array of bytes provided (wire) and return a 
	 * dns_rr (struct) populated with its contents. Update the value
	 * pointed to by indexp to the next value beyond the resource record.
	 *
	 * INPUT:  wire: a pointer to an array of bytes
	 * INPUT:  indexp: a pointer to the index in the wire where the
	 *              wire-formatted resource record begins
	 * INPUT:  query_only: a boolean value (1 or 0) which indicates whether
	 *              we are extracting a full resource record or only a
	 *              query (i.e., in the question section of the DNS
	 *              message).  In the case of the latter, the ttl,
	 *              rdata_len, and rdata are skipped.
	 * OUTPUT: the resource record (struct)
	 */
}


int rr_to_wire(dns_rr rr, unsigned char *wire, int query_only) {
	/* 
	 * Convert a DNS resource record struct to DNS wire format, using the
	 * provided byte array (wire).  Return the number of bytes used by the
	 * name in wire format.
	 *
	 * INPUT:  rr: the dns_rr struct containing the rr record
	 * INPUT:  wire: a pointer to the array of bytes where the
	 *             wire-formatted resource record should be constructed
	 * INPUT:  query_only: a boolean value (1 or 0) which indicates whether
	 *              we are constructing a full resource record or only a
	 *              query (i.e., in the question section of the DNS
	 *              message).  In the case of the latter, the ttl,
	 *              rdata_len, and rdata are skipped.
	 * OUTPUT: the length of the wire-formatted resource record.
	 *
	 */
}



unsigned short create_dns_query(char *qname, dns_rr_type qtype, unsigned char *wire) {
	/* 
	 * Create a wire-formatted DNS (query) message using the provided byte
	 * array (wire).  Create the header and question sections, including
	 * the qname and qtype.
	 *
	 * INPUT:  qname: the string containing the name to be queried
	 * INPUT:  qtype: the integer representation of type of the query (type A == 1)
	 * INPUT:  wire: the pointer to the array of bytes where the DNS wire
	 *               message should be constructed
	 * OUTPUT: the length of the DNS wire message
	 */

	typedef struct {
		unsigned short randomId;
		unsigned short flags;
		unsigned short numQuestion;
		unsigned short answer_rr;
		unsigned short authority_rr; // always 00
		unsigned short additional_rr; // always 00
	} dns_query_header;

	dns_query_header* header = malloc(sizeof(dns_query_header));

	srandom(time(0));
	unsigned short randomNum = random();

	header->randomId = randomNum;
	header->flags = 0x001;
	header->numQuestion = 0x0100;
	header->answer_rr = 0x0000;
	header->additional_rr = 0x0000;
	header->authority_rr = 0x0000;

	// Copying header
	memcpy(wire, (unsigned char*) header, sizeof(dns_query_header));

	unsigned char* question = malloc(sizeof(unsigned char) * (strlen(qname) + 2));
	const char s[2] = ".";
	char* token = strtok(qname, s);
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

	
	memcpy(wire + 12, question, strlen(question));
	int question_length = strlen(question) + 1;

	int offset = 12 + question_length;
	typedef struct {
		unsigned short type;
		unsigned short class;
	} dns_type_class;

	dns_type_class* type_and_class = malloc(sizeof(dns_type_class));
	type_and_class->type = 0x0100;
	type_and_class->class = 0x0100;

	memcpy(wire + offset, (unsigned char*) type_and_class, sizeof(dns_type_class));

	//print_bytes(wire, offset + 4);
	free(header);
	free(question);
	free(type_and_class);
	return offset + 4;
}

dns_answer_entry *get_answer_address(char *qname, dns_rr_type qtype, unsigned char *wire, int message_length, int response_length) {
	/* 
	 * Extract the IPv4 address from the answer section, following any
	 * aliases that might be found, and return the string representation of
	 * the IP address.  If no address is found, then return NULL.
	 *
	 * INPUT:  qname: the string containing the name that was queried
	 * INPUT:  qtype: the integer representation of type of the query (type A == 1)
	 * INPUT:  wire: the pointer to the array of bytes representing the DNS wire message
	 * OUTPUT: a linked list of dns_answer_entrys the value member of each
	 * reflecting either the name or IP address.  If
	 */

	// Answer Decoding


	// qname has sending message
	// wire has response
	typedef struct {
		unsigned short matchingId;
		unsigned short flags;
		unsigned short totalQuestions;
		unsigned short totalAnswerRecords;
		unsigned short totalAuthority;
		unsigned short totalAdditional;
	} dns_answer_query;

	dns_answer_query* answer_query = malloc(sizeof(dns_answer_query));
	// Copying the first 12 bytes
	memcpy(answer_query, wire, 12);

	
	//print_bytes(answer_query,12);
	
	//printf("%02x%02x\n", answer_query->totalAnswerRecords & 255, (answer_query->totalAnswerRecords/256)&255);
	typedef struct {
		unsigned short owner_name;
		unsigned short type;
		unsigned short class;
		unsigned short ttl1;
		unsigned short ttl2;
		unsigned short data_length;
	} answer_data;

	// numRR has # of resource record
	unsigned short numRR = ntohs(answer_query->totalAnswerRecords);

	//unsigned char temp[100];
	answer_data* temp_answer = malloc(sizeof(answer_data));
	memcpy(temp_answer, wire + message_length, 12);
	int numType = ntohs(temp_answer->type);

	if (numType == 5) {
		// Has an Alias

		int first_data_bytes = ntohs(temp_answer->data_length);
		char* temp_address = malloc(sizeof(unsigned char) * first_data_bytes);
		memcpy(temp_address, wire + message_length + 12, first_data_bytes);

		//print_bytes(temp_address, first_data_bytes);

		unsigned char alias_name[first_data_bytes];
		memcpy(alias_name, temp_address, first_data_bytes);

		int numBytes = alias_name[0]; // has 6
		char new_name[100];
		
		strncpy(new_name, alias_name + 1, numBytes);
		new_name[numBytes] = '.';
		new_name[numBytes+1] = '\0';
		// new name has mg-web
		//printf("first data bytes = %d\n", first_data_bytes);
		//printf("numbytes = %d   %s\n", numBytes, new_name);
		free(temp_answer);
		free(temp_address);
		free(answer_query);
		

		int offset = alias_name[first_data_bytes - 1]; // has 16

		unsigned char copying_name[50]; 
		memcpy(copying_name, wire + offset, message_length - offset);
		//print_bytes(copying_name, message_length - offset);
		//int bytes_to_read = copying_name[0];
		unsigned char to_concat[30];
		int count = 0;
		for (int i = 0; i < (message_length - offset); i++) {
			if (copying_name[i] == 0x00) break;
			
			int bytes_to_read = copying_name[i];
			strncpy(to_concat + count, copying_name + (i + 1), bytes_to_read);

			
			count += bytes_to_read;
			to_concat[count] = '.';
			++count;
			i += bytes_to_read;
		}
		to_concat[count-1] = '\0';
		//printf("to_concat %s\n", to_concat);
		strcat(new_name, to_concat);
		//print_bytes(new_name, 30);
		//printf("%s\n", new_name);

		// new name is the head's value

		char resource_record[16];
		int index = message_length;
		
		//printf("%d \n", message_length);
		//memcpy(resource_record, wire + message_length, 16);
		//print_bytes(resource_record, 16);
		
		// Allocating space for the head
		//dns_answer_entry* head = (struct dns_answer_entry *)malloc(sizeof(struct dns_answer_entry));
		
		
		dns_answer_entry* head = (struct dns_answer_entry *)malloc(sizeof(struct dns_answer_entry));
		//head->value = new_name;
		head->value = (char*)malloc(sizeof(char)*sizeof(new_name));
		strcpy(head->value, new_name);

		dns_answer_entry* current_ptr = head;

		for (int i = 0; i < numRR - 1; i++) {

			// new ptr = current;
			// new ptr malloc
			//printf("curr_ptr val = %s\n", current_ptr->value);
			dns_answer_entry* new_entry = (struct dns_answer_entry *)malloc(sizeof(struct dns_answer_entry));
			//if (i == 0) {
				current_ptr->next = new_entry;
			//}
			
			//new_entry = current_ptr;
			current_ptr = new_entry;

			// Copying 16 bytes into the array
			memcpy(resource_record, wire + index + 12 + first_data_bytes, 16);

			//printf("resource record is\n");
			//print_bytes(resource_record, 16);

			//answer_data* data_answer = malloc(sizeof(answer_data));
			//memcpy(data_answer, wire + index, 12); // except the actual address
			//int data_bytes = ntohs(data_answer->data_length);
			char* address = malloc(sizeof(unsigned char) * 4); // memory allocated for the address
			char str[INET_ADDRSTRLEN];
			memcpy(address, wire + index + 24 + first_data_bytes, 4); // copying the address
			inet_ntop(AF_INET, address, str, INET_ADDRSTRLEN);

			//print_bytes(address, 4);
			free(address);



			//current_ptr = (struct dns_answer_entry *)malloc(sizeof(struct dns_answer_entry));
			//new_entry->value = (char*)malloc(sizeof(char) * sizeof(str));
			current_ptr->value = (char*)malloc(sizeof(char) * sizeof(str));
			//strcpy(new_entry->value, str);
			strcpy(current_ptr->value, str);
			//printf("address : %s\n", new_entry->value);
			
			//if (i == 0) {
			//	head = new_entry;
			//}
			if (i == (numRR - 2)) {
				//new_entry->next = NULL;
				current_ptr->next = NULL;
				return head;
			}

			
			//@@@@@TEMP@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			index += 16;
		}


	}
	else {
			//char* data_info = malloc(sizeof(unsigned char) * 12);
		if (numRR == 1) {
			// single answer
			answer_data* data_answer = malloc(sizeof(answer_data));
			memcpy(data_answer, wire + message_length, 12);

			free(data_answer);

			int data_bytes = ntohs(data_answer->data_length);

			// address has the IP Address bytes
			char* address = malloc(sizeof(unsigned char) * data_bytes);
			
			// Store IP address string into str
			char str[INET_ADDRSTRLEN];
			// the rest is for address
			memcpy(address, wire + message_length + 12, response_length - (message_length + 12));
			// Converting
			inet_ntop(AF_INET, address, str, INET_ADDRSTRLEN);

			free(address);

			dns_answer_entry* answer = (struct dns_answer_entry *)malloc(sizeof(struct dns_answer_entry));
			answer->value = (char*)malloc(sizeof(char) * sizeof(str));
			strcpy(answer->value, str);
			answer->next = NULL;
			return answer;
		}
		else if (numRR == 0) {
			// no answer
			dns_answer_entry* answer = NULL;
			return answer;
		}
		else if (numRR > 1) {

			char resource_record[16];
			int index = message_length;
			
			//printf("%d \n", message_length);
			//memcpy(resource_record, wire + message_length, 16);
			//print_bytes(resource_record, 16);
			
			// Allocating space for the head
			//dns_answer_entry* head = (struct dns_answer_entry *)malloc(sizeof(struct dns_answer_entry));
			
			
			dns_answer_entry* head;
			dns_answer_entry* current_ptr;

			for (int i = 0; i < numRR; i++) {

				// new ptr = current;
				// new ptr malloc
				dns_answer_entry* new_entry = (struct dns_answer_entry *)malloc(sizeof(struct dns_answer_entry));
				if (i != 0) {
					current_ptr->next = new_entry;
				}
				
				//new_entry = current_ptr;
				current_ptr = new_entry;

				// Copying 16 bytes into the array
				memcpy(resource_record, wire + index, 16);

				answer_data* data_answer = malloc(sizeof(answer_data));
				memcpy(data_answer, wire + index, 12); // except the actual address
				int data_bytes = ntohs(data_answer->data_length);
				char* address = malloc(sizeof(unsigned char) * data_bytes); // memory allocated for the address
				char str[INET_ADDRSTRLEN];
				memcpy(address, wire + index + 12, 4); // copying the address
				inet_ntop(AF_INET, address, str, INET_ADDRSTRLEN);

				free(address);

				//current_ptr = (struct dns_answer_entry *)malloc(sizeof(struct dns_answer_entry));
				new_entry->value = (char*)malloc(sizeof(char) * sizeof(str));
				strcpy(new_entry->value, str);
				//printf("address : %s\n", new_entry->value);
				
				if (i == 0) {
					head = new_entry;
				}
				if (i == (numRR - 1)) {
					new_entry->next = NULL;
					return head;
				}

				
				//@@@@@TEMP@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
				index += 16;
			}


		}
	}

	
	


	
	
	
}

int send_recv_message(unsigned char *request, int requestlen, unsigned char *response, char *server, unsigned short port) {
	/* 
	 * Send a message (request) over UDP to a server (server) and port
	 * (port) and wait for a response, which is placed in another byte
	 * array (response).  Create a socket, "connect()" it to the
	 * appropriate destination, and then use send() and recv();
	 *
	 * INPUT:  request: a pointer to an array of bytes that should be sent
	 * INPUT:  requestlen: the length of request, in bytes.
	 * INPUT:  response: a pointer to an array of bytes in which the
	 *             response should be received
	 * OUTPUT: the size (bytes) of the response received
	 */

	// Setting client section

	int sockfd;
	struct sockaddr_in servaddr;

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr(server);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		printf("connect error\n");
	}

	send(sockfd, request, requestlen, 0);

	char buffer[512];

	int received_bytes = recv(sockfd, buffer, sizeof(buffer), 0);

	//printf("%d\n", received_bytes);

	memcpy(response, buffer, sizeof(buffer));

	//print_bytes(buffer, received_bytes);

	return received_bytes;

}

dns_answer_entry *resolve(char *qname, char *server, char *port) {

	unsigned char* message = malloc(sizeof(char) * (16 + strlen(qname)));
	unsigned char* response = malloc(sizeof(char) * 512);
	dns_rr_type typeClass = 0x0001;

	// Create DNS query message
	unsigned short message_length = create_dns_query(qname, typeClass, message);

	unsigned short portNum = 53;
	if (strcmp(port, "") != 0) {
		portNum = (unsigned short) strtoul(port, NULL, 0);
	}
	//printf("port num = %d\n", portNum);

	int response_bytes = send_recv_message(message, message_length, response, server, portNum);	
	//print_bytes(message, message_length);
	//print_bytes(response, response_bytes);
	dns_answer_entry *result;
	result = get_answer_address(message, typeClass, response, message_length, response_bytes);


	//printf("ending.. value is answer: %s\n", result->value);
	free(message);
	free(response);
	return result;
}

int main(int argc, char *argv[]) {
	char *port;
	dns_answer_entry *ans_list, *ans;
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <domain name> <server> [ <port> ]\n", argv[0]);
		exit(1);
	}
	if (argc > 3) {
		port = argv[3];
	} else {
		port = "53";
	}
	ans = ans_list = resolve(argv[1], argv[2], port);
	while (ans != NULL) {
		printf("%s\n", ans->value);
		ans = ans->next;
	}
	if (ans_list != NULL) {
		free_answer_entries(ans_list);
	}
}
