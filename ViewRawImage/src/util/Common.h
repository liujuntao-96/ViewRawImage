#pragma once

#define ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))

#define ALIGN(x, a) (((x)+(a)-1)&~((a)-1))