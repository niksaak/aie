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

void archive_open_unexistent(void)
{
  const aie_ArcFormat* fmt = aie_arcfmt(aie_ARC_KID_ENGINE_LINK);
  aie_Archive* archive = fmt->open("fdsfgds");

  ASSERT(archive == NULL);
}

void archive_open(void)
{
  const aie_ArcFormat* fmt = aie_arcfmt(aie_ARC_KID_ENGINE_LINK);
  aie_Archive* archive = fmt->open("script.rus");

  ASSERT(archive != NULL);
}

void archive_kill(void)
{
  const aie_ArcFormat* fmt = aie_arcfmt(aie_ARC_KID_ENGINE_LINK);
  aie_Archive* archive = fmt->open("script.rus");

  ASSERT(!aie_kmarchive(archive));
}

void archive_files_nonNULL(void)
{
  const aie_ArcFormat* fmt = aie_arcfmt(aie_ARC_KID_ENGINE_LINK);
  aie_Archive* archive = fmt->open("script.rus");

  ASSERT(archive->files != NULL);
}

void archive_filename(void)
{
  const aie_ArcFormat* fmt = aie_arcfmt(aie_ARC_KID_ENGINE_LINK);
  aie_Archive* archive = fmt->open("script.rus");

  ASSERT(!strcmp(archive->files->name, "script.rus"));
}

int main(int argc, char** argv)
{
  TEST(arcfmt_select);
  TEST(archive_open_unexistent);
  TEST(archive_open);
  TEST(archive_kill);
  TEST(archive_files_nonNULL);
  TEST(archive_filename);
  test_summary();

  puts("\nNow real action - printing archive contents...");
  {
    const aie_ArcFormat* fmt = aie_arcfmt(aie_ARC_KID_ENGINE_LINK);
    aie_Archive* arc = fmt->open("script.rus");

    printf("Contents of %s:\n", arc->files->name);
    for(int i = 0; i < arc->table->unitc; i++) {
      puts(arc->table->unitv[i].name);
    }
  }
  puts("Done?");

  return 0;
}

