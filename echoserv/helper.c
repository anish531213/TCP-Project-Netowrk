/*

  HELPER.C
  ========
  (c) Paul Griffiths, 1999
  Email: mail@paulgriffiths.net

  Implementation of sockets helper functions.

  Many of these functions are adapted from, inspired by, or 
  otherwise shamelessly plagiarised from "Unix Network 
  Programming", W Richard Stevens (Prentice Hall).

*/

#include "helper.h"
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



void convertFirstTypes(FILE* fp, unsigned char type, int amount, int numbers[]) {

	// Printing type
	if (type == 1) {
		//printf("Type0\n");
		fprintf(fp, "%c", type);
	} else {
		//printf("Type1\n");
		fprintf(fp, "%c", type+1);
	}
	

	char snum[3];
	char s;

	int k;
	
	// print amount to file in aciia
	for (k=2; k>=0; k--) {
		// Making amount as 3 byte ascii
		s = amount/pow(10, k)+'0';
		fprintf(fp, "%c", s);
	}

	// directly printing to a file as string
	for (k=0; k<amount-1; k++) {
		//sprintf(nums[i], "%d", numbers[i]);
		fprintf(fp, "%d,", numbers[k]);
	}
	// Printing end number without comma
	fprintf(fp, "%d", numbers[amount-1]);
	

	
}

void convertSecondTypes(FILE* fp, unsigned char type, int amount, int numbers[]) {	
	// Wrting type character
	if (type == 0) {
		//printf("Type1\n");
		fprintf(fp, "%c", type);
	} else {
		//printf("Type0\n");
		fprintf(fp, "%c", type-1);
	}

	// Using fwrite to write integer byte
	fwrite(&amount, 1, 1, fp);

	// Introducing flip_num array to address byte flip issue
	int flip_num[amount];

	for (int k=0; k<amount; k++) {
		//fprintf(fp, "%X", numbers[i]);
		flip_num[k] = (numbers[k]>>8) | (numbers[k]<<8);
		fwrite(&flip_num[k] , 2, 1, fp);
	}
	// Logic to convert type 0 to type 1
	//fprintf(fp, "\n");
}


/*  Convert function to convert types  */

int convert(char* file_name, char* char_type, int* data_array, int count, int start) {

	FILE *fp;
	//int myInt = 5;
	fp = fopen(file_name, "wb");

	int main_type;

	main_type = atoi(char_type);

	int i=start;
	int amount;
	// Default no type is 5
	int type = 5;

	while (i < count) {

		// if no type, assigns type 0 or 1
		if (type == 5) {
			type = data_array[i];
			i += 1;

		// if type = 0
		} else if (type == 0) {
			printf("Type 0 ");

			amount = data_array[i];
			printf("Amount: %d, ", amount);
			// number array for storing numbers
			int numbers[amount];
			i += 1;
			for (int j=0; j<amount; j++) {
				// Concatinating two single byte data
				int result = (data_array[i] << 8) | data_array[i+1];
				printf("Number %d: %d, ", j, result);
				numbers[j] = result;
				i += 2;
			}

			printf("\n");
			
			// Converting the ints to the respective type
			if (main_type == 3 || main_type == 1) {
				convertFirstTypes(fp, type, amount, numbers);
			} else {
				convertSecondTypes(fp, type, amount, numbers);
			}

			//convertFirstTypes(fp, type, amount, numbers);
			type = 5;

		// if type == 1
		} else if (type == 1) {
			printf("\nType 1 ");
			//char amount[3];

			amount = 100*(data_array[i]-48) + 10*(data_array[i+1]-48) + (data_array[i+2]-48);
			printf("Amount: %d, ", amount);
			// number arary for storing numbers
			int numbers[amount];
			i += 3;
			for (int j=0; j<amount; j++) {
				int result = 0;
				while (data_array[i] != 0 && data_array[i] != 1  && data_array[i] != '\0') {
					if (data_array[i] == 44) {
						i += 1;
						break;
					}
					result = result*10 + (data_array[i]-48);
					i += 1;
				} 
				printf("Number %d: %d, ", j, result);
				numbers[j] = result;
			}

			printf("\n");

			if (main_type == 3 || main_type == 2) {
				convertSecondTypes(fp, type, amount, numbers);
			} else {
				convertFirstTypes(fp, type, amount, numbers);
			}
			//convertSecondTypes(fp, type, amount, numbers);

			type = 5;
			//return 0;
		}


	}

	fclose(fp);

	return 1;

}


