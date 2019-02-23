#include "Version.h"
#include <stdio.h>
#include <string>

#include "pxr/usd/usd/stage.h"
#include "pxr/base/arch/systemInfo.h"
#include "pxr/base/plug/registry.h"

class InitUSDPluginPath {
public:
    InitUSDPluginPath() {
        std::string dir = pxr::TfGetPathName(pxr::ArchGetExecutablePath());
        std::string pluginDir = pxr::TfStringCatPaths(dir, "/usd/");
        pxr::PlugRegistry::GetInstance().RegisterPlugins(pluginDir);
    }
};
// constructor runs once to set USD plugin path (must be done early)
static InitUSDPluginPath getUSDPlugInPathSet;

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
