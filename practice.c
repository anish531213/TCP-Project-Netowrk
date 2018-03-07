#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//00101100 ascii ','
void convertFirstTypes(FILE* fp, unsigned char type, int amount, int numbers[]) {

	// Printing type
	fprintf(fp, "%c", type+1);

	char snum[3];
	char s;
	
	// print amount to file in aciia
	for (int i=2; i>=0; i--) {
		// Making amount as 3 byte ascii
		s = amount/pow(10, i)+'0';
		fprintf(fp, "%c", s);
	}

	// directly printing to a file as string
	for (int i=0; i<amount-1; i++) {
		//sprintf(nums[i], "%d", numbers[i]);
		fprintf(fp, "%d,", numbers[i]);
	}
	// Printing end number without comma
	fprintf(fp, "%d", numbers[amount-1]);
	

	
}

void convertSecondTypes(FILE* fp, unsigned char type, int amount, int numbers[]) {	
	// Wrting type character
	fprintf(fp, "%c", type-1);

	// Using fwrite to write integer byte
	fwrite(&amount, 1, 1, fp);

	// Introducing flip_num array to address byte flip issue
	int flip_num[amount];

	for (int i=0; i<amount; i++) {
		//fprintf(fp, "%X", numbers[i]);
		flip_num[i] = (numbers[i]>>8) | (numbers[i]<<8);
		fwrite(&flip_num[i] , 2, 1, fp);
	}
	// Logic to convert type 0 to type 1
	//fprintf(fp, "\n");
}



int main()

{

	unsigned char buffer[1];

	int data_array[500];
	int count;

	FILE *fp;
	//int myInt = 5;
	fp = fopen("Output.dms", "wb");

	unsigned char type;
	FILE *ptr;
	// struct record myRecord;
	ptr=fopen("practice_project_test_file_1.dms","rb");

	if (!ptr) { 
		printf("Unable to open file!");     
		return 1;
	}

	// keeping count of the bytes
	count = 0;
	// Reading the buffer 1 byte by 1 byte and saving into array
	while (fread(&buffer,sizeof(buffer),1,ptr) != 0) {
		// Saving the file buffer into data array
		data_array[count] = buffer[0];
		count += 1;
		//printf("%u ", buffer[0]);
	}

	//printf("\n");

	int i=0;
	int amount;
	// Default no type is 5
	type = 5;

	while (i < count) {

		// if no type, assigns type 0 or 1
		if (type == 5) {
			type = data_array[i];
			i += 1;

		// if type = 0
		} else if (type == 0) {
			printf("\nType 0 ");

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


			convertFirstTypes(fp, type, amount, numbers);

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
				while (data_array[i] != 0 && data_array[i] != 1) {
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


			convertSecondTypes(fp, type, amount, numbers);

			type = 5;
			//return 0;
		}


	}

	fclose(fp);
	fclose(ptr);
	return 0;
}