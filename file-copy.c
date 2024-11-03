#include <stdio.h>
#include <stdlib.h>

int main() {
	char src[50], dst[50];
	printf("Enter source file name: ");
	scanf("%s", src);
	printf("Enter destiny file name: ");
	scanf("%s", dst);

	FILE *fSrc, *fDst;
	fSrc = fopen(src, "r");
	if(fSrc == NULL) {
		printf("Source file does not exists\n");
		exit(1);
	}

	fDst = fopen(dst, "w");

	char buff[50];
	while(fgets(buff, sizeof(buff), fSrc)) {
		fputs(buff, fDst);
	}

	fclose(fSrc);
	fclose(fDst);
	return 0;
}
