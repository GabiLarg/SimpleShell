#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/wait.h>
#include <dirent.h>
#include <signal.h>
#include <ctype.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include "getword.h"
#define MAXITEM 100
