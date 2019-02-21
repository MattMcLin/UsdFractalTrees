#include "Version.h"
#include <stdio.h>

#include "pxr/usd/usd/stage.h"

int main(int argc, char* argv[])
{
	printf("UsdFractalTrees v%d.%d.%d\n",
		UsdFractalTrees_VERSION_MAJOR,
		UsdFractalTrees_VERSION_MINOR,
		UsdFractalTrees_VERSION_PATCH);

    const std::string outFileName("test.usda");
    pxr::UsdStageRefPtr stage = pxr::UsdStage::CreateNew(outFileName);
    if (stage) {
        stage->GetRootLayer()->Save();
        printf("Success!\n");
    } else {
        printf("Failed to create stage.\n");
    }
		
	return 0;
}
