#include <stdio.h>
#include <stdlib.h>

//00101100 ascii ','

int main()

{
	unsigned char buffer[1];

	int count;
	FILE *ptr;
	//struct record myRecord;
	ptr=fopen("practice_project_test_file_1.dms","rb");

	if (!ptr) { 
		printf("Unable to open file!");     
		return 1;
	}

	while (fread(&buffer,sizeof(buffer),1,ptr) != 0) {
		printf("%u ", buffer[0]);
	}

	printf("\n");
	// for (count=1; count <= 10; count++) { 
	/*
	fread(&buffer,sizeof(buffer),1,ptr);
	int i = 0;  

	while (buffer[i] != '\0') {
		printf("%u ", buffer[i]);	
		i+= 1;
	}

	for(int i = 0; i<30; i++)
		printf("%u ", buffer[i]);
	*/	 

	fclose(ptr);
	return 0;
}