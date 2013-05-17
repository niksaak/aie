# AIE
*Artificial (In the name of love) Extractifier*

### What is it
AIE is the project that aims to write library and accompanying tools for
working with various kinds of game resources like archives, images, and
miscellaneous data for purposes of translation and modding. It is being written
in C, using [AE - VN Tools][ae-github] by [dsp2003][wks] as a reference for
unpacking algorithms.

### Version
Current version: **0.03+0.095i**

Versions in AIE have purely decorative and psychological meaning, and
the author himself (i.e. me) prefers to refer to specific versions by their
commit hashsums.
But complex numbers are cute, and for that reason AIE version was decided to be
a complex number with its imaginary part representing "readiness" of core
library, and its real part representing basically how much useful it can be at
the moment e.g. how many game archive formats it supports.

### Milestones

**Real 0.1** - at least 9 formatters implemented for reading, extracting
and packing.<br />
**Imag 0.1** - library has more or less stable minimal external API providing
basic means for frontenfs to retrieve information about formatters and use them,
and internal API with basic abstractions allowing formatters to be as much
system-independent as possible.

[ae-github]: https://github.com/dsp2003/ae "AE GitHub repo"
[wks]: http://wks.arai-kibou.ru/ "WinKiller studio"

