#include <stdio.h>
#include <string.h>

#include <aie_archive.h>
#include <aie_archive-kinds.h>

#include "test_util.h"

void arcfmt_select(void)
{
  const aie_ArcFormat* fmt = aie_arcfmt(aie_ARC_KID_ENGINE_LINK);

  ASSERT(fmt->id == aie_ARC_KID_ENGINE_LINK);
}

void archive_open(void)
{
  const aie_ArcFormat* fmt = aie_arcfmt(aie_ARC_KID_ENGINE_LINK);
  aie_Archive* archive =
    fmt->open(fopen("script.rus", "r"), "script.rus", NULL);

  ASSERT(archive != NULL);
}

void archive_files_nonNULL(void)
{
  const aie_ArcFormat* fmt = aie_arcfmt(aie_ARC_KID_ENGINE_LINK);
  aie_Archive* archive =
    fmt->open(fopen("script.rus", "r"), "srcipt.rus", NULL);

  ASSERT(archive->files != NULL);
}

void strtoks_delim_at_end(void)
{
  char str[] = "delimiting space at the end of the string ";
  const char* toks[9] = {NULL};
  int tokc = 0;

  tokc = aie_strtoks(str, " ", toks, 9);

  ASSERT(!strcmp(toks[tokc - 1], "string "));
}

void strtoks_many_delim(void)
{
  char str[] = "many   delimiters";
  const char* toks[9] = {NULL};

  aie_strtoks(str, " ", toks, 9);

  ASSERT(!strcmp(toks[1], "delimiters"));
}

void tokcpy_sane(void)
{
  char src[] = "The source";
  char dst[9] = {0};
  char chk[9] = {0};

  strncpy(chk, src, 3);
  aie_tokcpy(dst, src, ' ', 9);

  ASSERT(!strcmp(dst, chk));
}

int main(int argc, char** argv)
{
  TEST(arcfmt_select);
  TEST(archive_open);
  TEST(archive_files_nonNULL);
  TEST(strtoks_delim_at_end);
  TEST(strtoks_many_delim);
  TEST(tokcpy_sane);
  test_summary();


  if(failedc == 0) {
    const aie_ArcFormat* fmt = aie_arcfmt(aie_ARC_KID_ENGINE_LINK);
    FILE* file = fopen("script.rus", "r");
    aie_Archive* arc = fmt->open(file, "script.rus", NULL);

    puts("\nNow real action - printing archive contents...");
    printf("Formatter features: [%s]\n", aie_arcfmt_featurestr(fmt));
    printf("Contents of %s:\n", arc->files->name);

    for(int i = 0; i < arc->table->unitc; i++) {
      const aie_ArcUnit* aru = aie_arctable_get(arc->table, i);
      printf("%s %ziB\n", aru->name, aie_arcsegment_sizesum(aru->segments));
    }

    puts("Done?");
  }

  return 0;
}

