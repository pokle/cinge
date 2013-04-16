// LDIF -> JSON files that can be loaded using the elasticsearch _bulk api
// Well, right now it just counts the number of newlines in your file!
// Preparation I say!

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/mman.h>

const char *
boyermoore_horspool_memmem(const char* haystack, size_t hlen,
                           const char* needle,   size_t nlen);


size_t fdsize(int fd) {
	struct stat stats;
	fstat(fd, &stats);
	return stats.st_size;
}

void dump_cns(void *start, size_t len) {
    const char* search = "cn: ";
    size_t search_len = strlen(search);

    const char* end = start + len;    
    char*  curr_str = start;
    size_t curr_len = len;

    while (curr_str < end) {        
        const char *match = boyermoore_horspool_memmem(curr_str, curr_len, search, search_len);
        if (NULL == match) {
            break;
        } else {
            // Spit out the value in "key: value"
            {
                char* match_end = memchr(match, '\n', end - match);
                if (NULL == match_end) {
                    match_end = end;
                }

                char* value = match + search_len;
                size_t value_len =  match_end - value + 1; // +1 to incl. newline
                fwrite(value, value_len, 1, stdout);
            }

            // next
            curr_str = match + search_len; // to skip the match
            curr_len = end - match - search_len;
        }
    }
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
            curr_str = newline + 1; // +1 to skip the newline
            curr_len = end - newline - 1;
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
	if (ldif_len == 0) {
		puts("0");
		goto close;
	}

	void *ldif_mm = mmap(0, ldif_len, PROT_READ, MAP_FILE |  MAP_SHARED, fd, 0);
	if (MAP_FAILED == ldif_mm) {
		fprintf(stderr, "Unable to memory map %s: %s\n", ldif_fn, strerror(errno));
		exit(2);
	}

    // size_t num_newlines = count_newlines(ldif_mm, ldif_len);
    // printf("%llu\n", (unsigned long long)num_newlines);
    dump_cns(ldif_mm, ldif_len);

	munmap(ldif_mm, ldif_len);
	
	close: close(fd);
}