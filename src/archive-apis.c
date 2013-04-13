#include <aie_archive-apis.h>

static aie_Archive* open_recognizing(FILE* file, const char* name)
{
  aie_Archive* arc;

  for(int i = 0; i < aie_ARC_KIND_MAX; i++) {
    arc = aie_arcopen(file, name, i, NULL);
    if(arc != NULL) {
      return arc;
    }
  }

  return NULL;
}

aie_Archive* aie_arcopen(FILE* file, const char* name, aie_ArcFormatKind kind,
    const char* opt)
{
  if(kind == aie_ARC_KIND_AUTO) {
    return open_recognizing(file, name);
  }

  return aie_arcfmt(kind)->open(file, name, NULL);
}

aie_Archive* aie_arcmake(const char* target, char** pathnames,
    aie_ArcFormatKind kind, const char* opt)
{
  return aie_arcfmt(kind)->create(target, pathnames, opt);
}

int aie_arcextract(const aie_Archive* archive, const char* target,
    const char* opt)
{
  return archive->fmt->extract(archive, target, opt);
}

