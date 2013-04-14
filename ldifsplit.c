// LDIF -> JSON files that can be loaded using the elasticsearch _bulk api

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/mman.h>

size_t fdsize(int fd) {
	struct stat stats;
	fstat(fd, &stats);
	return stats.st_size;
}

size_t count_newlines(void *start, size_t len) {
    size_t count = 0;

    void  *end = start + len;    
    void  *curr_str = start;
    size_t curr_len = len;

    while (curr_str < end) {        
        void *newline = memchr(curr_str, '\n', curr_len);
        if (NULL == newline) {
            fputs("Done!\n", stderr);
            break;
        } else {
            count++;
            curr_str = newline;
            curr_len = end - newline;
        }
    }

    return count;
}


int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        fprintf(stderr, "usage: %s file.ldif\n", argv[0]);
        return 1;
    }

    char *ldif_fn = argv[1];
	int fd = open(ldif_fn, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Unable to open %s: %s\n", ldif_fn, strerror(errno));
		exit(1);
	}

	size_t ldif_len = fdsize(fd);
	fprintf(stderr, "File size is %llu\n", (unsigned long long)ldif_len);

	void *ldif_mm = mmap(0, ldif_len, PROT_READ, MAP_FILE | MAP_NOCACHE | MAP_SHARED, fd, 0);
	if (MAP_FAILED == ldif_mm) {
		fprintf(stderr, "Unable to memory map %s: %s\n", ldif_fn, strerror(errno));
		exit(2);
	}

    size_t num_newlines = count_newlines(ldif_mm, ldif_len);
    printf("%llu\n", (unsigned long long)num_newlines);

	munmap(ldif_mm, ldif_len);
	close(fd);
}