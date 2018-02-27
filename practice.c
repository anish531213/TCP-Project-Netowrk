#include <stdio.h>
#include <stdlib.h>

//00101100 ascii ','

int main()

{
	unsigned char buffer[30];

	int count;
	FILE *ptr;
	//struct record myRecord;
	ptr=fopen("practice_project_test_file_1.dms","rb");

	if (!ptr) { 
		printf("Unable to open file!");     
		return 1;
	}

	// for (count=1; count <= 10; count++) { 
	fread(&buffer,sizeof(buffer),1,ptr);   
	for(int i = 0; i<30; i++)
		printf("%u ", buffer[i]);	 

	fclose(ptr);
	return 0;
}