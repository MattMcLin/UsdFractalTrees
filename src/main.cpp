#include "Version.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	printf("UsdFractalTrees v%d.%d.%d\n",
		UsdFractalTrees_VERSION_MAJOR,
		UsdFractalTrees_VERSION_MINOR,
		UsdFractalTrees_VERSION_PATCH);
		
	return 0;
}
