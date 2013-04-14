# AIE VN-TOOLS
*Artificial (In the name of love) Extractifier*

There shall be rewrite of dsp2003's [*AE - VN Tools*][vn-tools] archive
manipulator in C.

Current version: **0.03+0.09i** <br />
Working on: *figuring out external interface for library users,
cleanups*

[vn-tools]: http://wks.arai-kibou.ru/ae.php?p=dl "AE"

# AIE VN-TOOLS
*Artificial (In the name of love) Extractifier*

This is the try to rewrite the rewrite of dsp2003's [*AE - VN TOOLS*][vn-tools]
archives-and-other-thingys manipulator in C.

Current version: **0.03+0.09i**

TODO for this branch:

+ make functions that work with structures accept compound literals instead of
taking one argument per structure field;
+ reduce the use of malloc() and related functions to necessary minimum, i.e.
use heap only for data of undeterminable size whilst giving user to deside on
allocation method for other structures;
+ implement simple errno-like error-handling mechanism though accepting custom
error strings;
+ create base for future multilanguage support;
