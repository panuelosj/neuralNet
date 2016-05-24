#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "net.h"

float** readCSV(const char filename[], const int lengthRow, const int lengthCol) {
	float** arrayWrite = allocateArray2D(lengthRow, lengthCol);

	char line[MAX_LINE];
	int i, j;
	float temp;
	
	// open input file, read only
	FILE *stream = fopen(filename, "r");

	if (stream == NULL)	
		errorCase(ERR_FILE_OPEN);

	for (i = 0; i < lengthRow; i++) {
		// grab next line of stream
		fgets(line, MAX_LINE, stream);

		// convert to float, everything before the comma delimiter
		temp = atof(strtok(line, ",\n"));
		for (j = 0; j < (lengthCol - 1); j++) {
			arrayWrite[i][j] = temp;		//save float in matrix
			temp = atof(strtok(NULL, ",\n"));	//keep reading from last position
		}
		arrayWrite[i][j] = temp;		//save last line
	}
	
	// cleanup
	fclose(stream);
	printf("%s read successful!\n", filename);
	return arrayWrite;
}
int writeCSV(const char filename[], float** arrayIn, const int lengthRow, const int lengthCol) {
	char line[MAX_LINE];
	int i, j;
	float temp;

	FILE *stream = fopen(filename, "w");

	for (i = 0; i < lengthRow; i++) {
		for (j = 0; j < lengthCol; j++) {
			if (arrayIn[i][j] > 0)		// check if input makes sense
				fprintf(stream, "%f", arrayIn[i][j]);
			fprintf(stream, ",");
		}
		fprintf(stream, "\n");
	}
	fclose(stream);
	return 1;
}
