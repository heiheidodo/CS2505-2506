int fg = 1;
char first[30];
char second[30];
char third[30];

file = fopen("/proc/meminfo", "r");

if (file != NULL)
{
	strcpy(body, strcat(body, "{"));
	
	while (!feof(file))
	{
		memset(second, '\0', sizeof(second));
		memset(third, '\0', sizeof(third));
		if (strcmp(first, "kB") == 0 || fg){
			memset(first,'\0', sizeof(first));
			fscanf(file, "%s", first);
			fg = 0;
		}
		
		fscanf(file, "%s", second);
		
		int xdx = 0;
		for( xdx = 0; xdx < 30; xdx++)
		{
			if (first[xdx] == ':') break;
			third[xdx] = first[xdx];
		}
	
		strcpy(body, strcat(body, "\""));
		strcpy(body, strcat(body, third));
		strcpy(body, strcat(body, "\": "));
	
		int jdx = 0;
		for (jdx =0; jdx < 30; jdx++){
			if (second[jdx] == '\0') break;
		}
	
		strcpy(body, strcat(body, "\""));
		strcpy(body, strcat(body, second));
		strcpy(body, strcat(body, "\""));
	
		fscanf(file, "%s", first);
		
		/* check end of the file */
		if (!feof(file))
		{
			strcat(body, ", ");
		}
	}

	strcpy(body, strcat(body, "}"));

}
else
{
    printf("File not found");
}
