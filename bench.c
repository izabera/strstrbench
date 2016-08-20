#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <sys/time.h>

char *bsd_strstr(const char *, const char *);
char *uclibc_strstr(const char *, const char *);
char *diet_strstr(const char *, const char *);
char *musl_strstr(const char *, const char *);
char *my_strstr(const char *, const char *);
char *my2_strstr(const char *, const char *);
char *fast_strstr(const char *, const char *);
char *plan9_strstr(const char *, const char *);

void stupid_gcc_stop_optimizing_this_away_for_fucks_sake(const char *);

char *noop(const char *a, const char *b) {
  // just to get the file in cache before the actual tests
  return (char *)a + strlen(a) + strlen(b);
}

int main() {
  struct { char *word, *filename; } tests[] = {
    { "smoothness"           , "tests/texts/anna-karenina.txt"       }, // line 10003
    { "reglez"               , "tests/texts/artamene.txt"            }, // line 10010
    { "wears"                , "tests/texts/brothers-karamazov.txt"  }, // line 10001
    { "hark"                 , "tests/texts/complete-shakespear.txt" }, // line 10006
    { "Pride"                , "tests/texts/iliad.txt"               }, // line 10002
    { "Haggerston"           , "tests/texts/pride-and-prejudice.txt" }, // line 10006
    { "abiding"              , "tests/texts/republic.txt"            }, // line 10016
    { "disorder"             , "tests/texts/war-and-peace.txt"       }, // line 10012
    { "ACAGCGACATACCGATATGC" , "tests/dna/100k"                      },
    { "GCGTCCTGCTGGGTCAAACG" , "tests/dna/1M"                        },
    { "GCAACGAACCCCAGGAGCTC" , "tests/dna/10M"                       },
    { "aaaaaaaaaaaaaaaaaaaa" , "tests/repetitions/20-1"              },
  };

  struct { char *name; char *(*func)(const char *, const char *); } strstrs[] = {
    { "noop"   ,           noop },
    { "glibc"  ,         strstr },
    { "plan9"  ,   plan9_strstr },
    { "musl"   ,    musl_strstr },
    { "uclibc" ,  uclibc_strstr },
    { "bsd"    ,     bsd_strstr },
    { "diet"   ,    diet_strstr },
    { "mine"   ,      my_strstr },
    { "mine2"  ,     my2_strstr },
    { "fast"   ,    fast_strstr },
  };

  for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
    int fd = open(tests[i].filename, O_RDONLY);
    struct stat st;
    fstat(fd, &st);
    char *map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    struct timeval t1, t2;

    printf("-----\nword: %s\nfilename: %s\n-----\n", tests[i].word, tests[i].filename);

    for (size_t j = 0; j < sizeof(strstrs)/sizeof(strstrs[0]); j++) {
      gettimeofday(&t1, NULL);
      for (int q = 0; q < 100; q++) 
        stupid_gcc_stop_optimizing_this_away_for_fucks_sake(strstrs[j].func(map, tests[i].word));
      gettimeofday(&t2, NULL);
      printf("%-10s %11.6f\n", strstrs[j].name,
          (double) (t2.tv_sec  - t1.tv_sec) +
          (double) (t2.tv_usec - t1.tv_usec) / 1000000);
    }

    munmap(map, st.st_size);
  }

  return 0;
}

