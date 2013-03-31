#include <stdio.h>
#include <string.h>

#include <aie_archive-kinds.h>
#include <aie_archive-format.h>

const aie_ArcFormat* aie_arcfmt(aie_ArcFormatKind kind)
{
  for(size_t i = 0; i <= (aie_ARC_FORMAT_KIND_MAX); i++) {
    if(aie_arcformats[i]->id == kind)
      return aie_arcformats[i];
  }

  return NULL;
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

  strncat(extensions, format->ext, sizeof extensions - 1);

  return extensions;
}

aie_ArcFormatFeatures aie_arcfmt_features(const aie_ArcFormat* format)
{
  return format->features;
}

const char* aie_arcfmt_featurestr(const aie_ArcFormat* format)
{
  aie_ArcFormatFeatures features = aie_arcfmt_features(format);
  static char ret[512] = {0};
  int pos = 0;

  if(features == aie_FMTOK) {
    sprintf(ret, "OK");
  } else {
    if(features & aie_FMTOpen)
      pos = snprintf(&ret[pos], 512, "Open | ");
    if(features & aie_FMTExtract)
      pos = snprintf(&ret[pos], 512 - strlen(ret), "Extract | ");
    if(features & aie_FMTCreate)
      pos = snprintf(&ret[pos], 512 - strlen(ret), "Create | ");
    if(features & aie_FMTUnsafe)
      pos = snprintf(&ret[pos], 512 - strlen(ret), "Unsafe | ");
    if(features & aie_FMTHack)
      pos = snprintf(&ret[pos], 512 - strlen(ret), "Hack | ");
    if(features & aie_FMTPlaceholder)
      pos = snprintf(&ret[pos], 512 - strlen(ret), "Placeholder | ");
    if(features)
      ret[strlen(ret) - 3] = '\0';
  }

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

