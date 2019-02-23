#include "Version.h"
#include <stdio.h>
#include <string>

#include "pxr/usd/usd/stage.h"
#include "pxr/usd/usdGeom/xform.h"
#include "pxr/usd/usdGeom/cylinder.h"
#include "pxr/base/vt/array.h"
#include "pxr/base/arch/systemInfo.h"
#include "pxr/base/plug/registry.h"

using namespace pxr;

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

void createTrunk(pxr::UsdStageRefPtr stage) {
    UsdGeomCylinder trunk = UsdGeomCylinder::Define(stage, pxr::SdfPath("/tree/trunk"));
    double radius = 0.5;
    double height = 5.0;
    trunk.CreateRadiusAttr(VtValue(radius));
    trunk.CreateHeightAttr(VtValue(height));
    trunk.CreateExtentAttr(VtValue(VtArray<GfVec3f>{GfVec3f(-radius, -radius, -height*0.5),GfVec3f(radius, radius, height*0.5)}));
}

void generateTree(pxr::UsdStageRefPtr stage) {
    UsdGeomXform::Define(stage, SdfPath("/tree"));
    createTrunk(stage);
}

int main(int argc, char* argv[])
{
	printf("UsdFractalTrees v%d.%d.%d\n",
		UsdFractalTrees_VERSION_MAJOR,
		UsdFractalTrees_VERSION_MINOR,
		UsdFractalTrees_VERSION_PATCH);

    const std::string outFileName("tree.usda");
    UsdStageRefPtr stage = UsdStage::CreateNew(outFileName);
    if (stage) {
        generateTree(stage);
        stage->GetRootLayer()->Save();
        printf("Result written to %s\n", outFileName.c_str());
    } else {
        printf("Failed to create stage.\n");
    }
		
	return 0;
}
