#include <aie_archive-format.h>

const aie_ArcFormat* aie_arcfmt(aie_ArcFormatKind kind)
{
  for(size_t i = 0; i <= (aie_ARC_FORMAT_KIND_MAX); i++) {
    if(aie_arcformats[i]->id == kind)
      return aie_arcformats[i];
  }

  return aie_arcfmt(aie_ARC_UNSUPPORTED);
}

const char* aie_arcfmt_name(const aie_ArcFormat* format)
{
  return format->name;
}

const char* aie_arcfmt_subformats(const aie_ArcFormat* format)
{
  if(format->subformat_names[0] == 0)
    return NULL;
  return format->subformat_names;
}

const char* aie_arcfmt_extensions(const aie_ArcFormat* format)
{
  static char extensions[512] = {0};

  strncat(extensions, format->arc_ext, sizeof extensions - 1);
  sprintf(extensions, " ");
  strncat(extensions, format->meta_ext, 
          sizeof extensions - strlen(extensions));

  return extensions;
}

aie_ArcFormatStatus aie_arcfmt_status(const aie_ArcFormat* format)
{
  return format->status;
}

const char* aie_arcfmt_statusstr(const aie_ArcFormat* format)
{
  aie_ArcFormatStatus status = aie_arcfmt_status(format);
  static char ret[512] = {0};
  char* str = NULL;

  switch(status & 0x7FFFFFFF)
  {
    case aie_NORMAL:
      str = "Normal";
      break;
    case aie_UNSAFE:
      str = "Unsafe";
      break;
    case aie_EXPERIMENTAL:
      str = "Experimental";
      break;
    case aie_HACK:
      str = "Hack";
      break;
    case aie_WRITEONLY:
      str = "Write only";
      break;
    case aie_READONLY:
      str = "Read only";
      break;
  }

  snprintf(ret, sizeof ret, str);
  if(status & aie_PLACEHOLDER)
    snprintf(ret, sizeof ret - strlen(ret) - 1, ", Placeholder");

  return ret;
}

size_t aie_arcfmt_namelen(const aie_ArcFormat* format)
{
  return format->filename_len;
}

uint32_t aie_arcfmt_ver(const aie_ArcFormat* format)
{
  return format->drv_version;
}


