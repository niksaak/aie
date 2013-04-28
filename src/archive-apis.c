#include <aie_archive-apis.h>
#include <aie_error.h>

#define ASSERT_ENOWAY(fun, datum, ret) \
  do { \
    if(fun == NULL) { \
      AIE_WARNING(aie_ENOWAY, datum); \
      return ret; \
    } \
  } while(0)

static aie_Archive open_recognizing(aie_ArcFile file)
{ // TODO: check formatters with matching fileextensions first
  AIE_ERESET();

  for(int i = 0; i < aie_ARC_KIND_COUNT; i++) {
    aie_Archive arc = aie_arcopen(file, i, NULL);

    if(arc.fmt != NULL)
      return arc;
    if(aie_geterror().level >= aie_ELError)
      return aie_ARCNIL;
  }

  AIE_ERROR(aie_EFORMAT, file.name);

  return aie_ARCNIL;
}

aie_Archive aie_arcopen(aie_ArcFile file, aie_ArcFormatKind kind,
    const char* opt)
{
  AIE_ERESET();

  aie_ArcFormat fmt;

  if(kind == aie_ARC_KIND_UNKNOWN) {
    return open_recognizing(file);
  }
  fmt = aie_arcfmt(kind);
  if(fmt.id == aie_ARC_KIND_UNKNOWN)
    return aie_ARCNIL;
  ASSERT_ENOWAY(fmt.open, fmt.name, aie_ARCNIL);

  return (*fmt.open)(file, opt);
}

/*
aie_Archive aie_arccreate(aie_ArcFile file, char** pathnames,
    aie_ArcFormatKind kind, char* opt)
{
  aie_ArcFormat fmt = aie_arcfmt(kind);

  if(fmt.id == aie_ARC_KIND_UNKNOWN) {
    return aie_ARCNIL;
  }
  ASSERT_ENOWAY(fmt.create, fmt.name, aie_ARCNIL);

  return (*fmt.create)(file, pathnames, kind, opt);
}
*/

int aie_arcextract(const aie_Archive* archive, const char* target,
    const char* opt)
{
  AIE_ERESET();

  AIE_ASSERT(archive != NULL, -1);
  AIE_ASSERT(archive->fmt != NULL, -1);

  ASSERT_ENOWAY(archive->fmt->extract, archive->fmt->name, -1);

  return (*archive->fmt->extract)(archive, target, opt);
}

int aie_arc_uextract(const aie_ArcUnit* unit, const aie_ArcFormat* fmt,
    const char* target, const char* opt)
{
  AIE_ERESET();

  if(fmt == NULL) {
    AIE_PANIC(aie_ENURUPO, "fmt");
    return -1;
  }
  ASSERT_ENOWAY(fmt->uextract, fmt->name, -1);

  return (*fmt->uextract)(unit, target, opt);
}

size_t aie_arc_umextract(const aie_ArcUnit* unit, const aie_ArcFormat* fmt,
    char* buf, size_t offset, size_t size, const char* opt)
{
  AIE_ERESET();

  if(fmt == NULL) {
    AIE_PANIC(aie_ENURUPO, "fmt");
    return -1;
  }
  if(buf == NULL) {
    AIE_PANIC(aie_ENURUPO, "buf");
    return -1;
  }
  ASSERT_ENOWAY(fmt->uextract, fmt->name, -1);

  return (*fmt->umextract)(unit, buf, offset, size, opt);
}

