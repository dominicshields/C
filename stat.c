/*
 * stat.c - stat() a file and print the result.
 * This code shows what kind of informations you can find about files.
 * compile with: gcc -o stat -c stat.c
 * or simply: make stat
 * 
 * It can be useful to use when doing queries by time, as in
$ touch foobar;
$ ./stat foobar
foobar:
dev     = 4
ino     = 1048910
mode    = \00000100644 -rw-r--r-- ---
nlink   = 1
uid     = 0
gid     = 0
size    = 0
blksize = 65536
atime   = 1071664991    Wed Dec 17 13:43:11 2003
mtime   = 1071664986    Wed Dec 17 13:43:06 2003
ctime   = 1071664986    Wed Dec 17 13:43:06 2003
crtime  = 1071664986    Wed Dec 17 13:43:06 2003
$ query 'last_modified>=1071664991'
 */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>

#define STAT lstat
//#define STAT stat

static char perms_buff[30];

const char *get_perms(mode_t mode)
{
	char ftype = '?';
	if (S_ISREG(mode)) ftype = '-';
	if (S_ISLNK(mode)) ftype = 'l';
	if (S_ISDIR(mode)) ftype = 'd';
	if (S_ISBLK(mode)) ftype = 'b';
	if (S_ISCHR(mode)) ftype = 'c';
	if (S_ISFIFO(mode)) ftype = '|';
	if (S_ISINDEX(mode)) ftype = 'i';
	sprintf(perms_buff, "%c%c%c%c%c%c%c%c%c%c %c%c%c", 
	ftype,
	mode & S_IRUSR ? 'r' : '-',
	mode & S_IWUSR ? 'w' : '-',
	mode & S_IXUSR ? 'x' : '-',
	mode & S_IRGRP ? 'r' : '-',
	mode & S_IWGRP ? 'w' : '-',
	mode & S_IXGRP ? 'x' : '-',
	mode & S_IROTH ? 'r' : '-',
	mode & S_IWOTH ? 'w' : '-',
	mode & S_IXOTH ? 'x' : '-',
	mode & S_ISUID ? 'U' : '-',
	mode & S_ISGID ? 'G' : '-',
	mode & S_ISVTX ? 'S' : '-');
	return (const char *)perms_buff;
}

int mystat(char *file, int fd)
{
	int err;
	struct stat st;
	if (file != NULL)
		err = STAT(file, &st);
	else
		err = fstat(fd, &st);
	if (err < 0) {
		perror(file);
		return -1;
	}
	if (file != NULL)
		printf("%s:\n", file);
	else
		printf("%d:\n", fd);
	printf("dev     = %d\n", st.st_dev);
	printf("ino     = %d\n", st.st_ino);
	printf("mode    = \\0%010lo %s\n", st.st_mode, get_perms(st.st_mode));
	printf("nlink   = %d\n", st.st_nlink);
	printf("uid     = %d\n", st.st_uid);
	printf("gid     = %d\n", st.st_gid);
	printf("size    = %lld\n", st.st_size);
	printf("blksize = %ld\n", st.st_blksize);
	printf("atime   = %ld \t%s", st.st_atime, ctime(&st.st_atime));
	printf("mtime   = %ld \t%s", st.st_mtime, ctime(&st.st_mtime));
	printf("ctime   = %ld \t%s", st.st_ctime, ctime(&st.st_ctime));
	printf("crtime  = %ld \t%s", st.st_crtime, ctime(&st.st_crtime));
	return 0;
}

int usage(char *name)
{
	printf("usage:\n");
	printf("%s file1 [file2 [file...]]\n", name);
	return 1;
}

int main(int argc, char **argv)
{
	int i;
	if (argc < 2)
		return usage(argv[0]);
	for (i = 1; i < argc; i++)
		mystat(argv[i], 0);
	return 0;
}
