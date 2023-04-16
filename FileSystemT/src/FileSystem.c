#include <stdlib.h>
#include <stdio.h>
#include "FSConfig.h"

int main(void) {
	configFS = config_create("../FileSystem/filesystem.config");
}
