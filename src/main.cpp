#include "Version.h"
#include <stdio.h>
#include <string>
#include <random>

#include "pxr/usd/usd/stage.h"
#include "pxr/usd/usd/prim.h"
#include "pxr/usd/usdGeom/xform.h"
#include "pxr/usd/usdGeom/cylinder.h"
#include "pxr/usd/usdGeom/sphere.h"
#include "pxr/base/vt/array.h"
#include "pxr/base/arch/systemInfo.h"
#include "pxr/base/plug/registry.h"

#include "branch.hpp"

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

float randomFloat(float min, float max)
{
    static std::mt19937 rng;
    static bool initialized = false;
    if (!initialized)
    {
        rng.seed(std::random_device()());
        initialized = true;
    }
    
    // consider size of mantissa when generating integer distribution to be converted to float
    std::uniform_int_distribution<> dist(0,2^24);
    float r = (float)dist(rng) / (float)(2^24) * (max - min) + min;
    return r;
}

void growTree(Branch* rootBranch, int numIterations, int branchesPerIter, float spread, float lean)
{
    if (numIterations > 0)
    {
        for (int i=0; i<branchesPerIter; ++i)
        {
            
            float newLength = rootBranch->length() * randomFloat(0.5f, 0.95f);
            float newRadius = rootBranch->radius() * randomFloat(0.4f, 0.6f);
            
            // rotation here refers to which side/direction the new growth sprouts from
            float newRot = randomFloat(0.0f, 360.0f);
            
            // spread = angle that new branches spread out relative to parent
            float newSpread = randomFloat(spread*0.8f, spread*1.2f);
            
            // decide how far down the parent branch the new growth should sprout from
            float dist = (i == 0) ? 1.0f : randomFloat(rootBranch->length() * 0.5, rootBranch->length());
            
            Branch* branch = new Branch(newLength, newRadius, newSpread, newRot, dist);
            rootBranch->addBranch(branch);
            growTree(branch, numIterations - 1, branchesPerIter, spread, lean);
        }
    }
}

void convertTreeToUsd(const Branch* branch, pxr::UsdStageRefPtr stage, const pxr::SdfPath &rootPath)
{
    if (branch->isLeaf()) {
        SdfPath xform_path = rootPath.AppendChild(TfToken("leafXform"));
        SdfPath path = xform_path.AppendChild(TfToken("leaf"));
        auto leafXform = UsdGeomXform::Define(stage, xform_path);
        auto trans = leafXform.AddTranslateOp();
        float offset = (branch->anglefromParent() < 10.0f) ? 0.05 : 0.15;
        trans.Set(VtValue(GfVec3d{0.0, branch->length() * offset, 0.0}));
        auto scale = leafXform.AddScaleOp();
        scale.Set(VtValue(GfVec3f{3.0f, 1.0f, 3.0f}));
        
        UsdGeomSphere leaf = UsdGeomSphere::Define(stage, path);
        double radius = branch->radius() * 2.0;
        leaf.CreateRadiusAttr(VtValue(radius));
        leaf.CreateExtentAttr(VtValue(VtArray<GfVec3f>{GfVec3f(-radius, -radius, -radius),
                                                       GfVec3f(radius, radius, radius)}));
        auto leafColor = leaf.GetDisplayColorAttr();
        leafColor.Set(VtValue(VtArray<GfVec3f>{GfVec3f{0.3f, 0.95f, 0.1f}}));
    } else {
        // Construct a branch.
        // Some extra hoops to jump through to rotate the cylinder for our
        // branch without affecting the children/attachments.
        SdfPath branch_path = rootPath.AppendChild(TfToken("branch"));
        SdfPath xform_path = branch_path.AppendChild(TfToken("branchXform"));
        SdfPath cyl_path = xform_path.AppendChild(TfToken("cylinder"));
        UsdPrim branchDef = stage->DefinePrim(branch_path);
        auto branchXform = UsdGeomXform::Define(stage, xform_path);
        auto trans = branchXform.AddTranslateOp();
        trans.Set(VtValue(GfVec3d{0.0, branch->length() * 0.5, 0.0}));
        auto rotX = branchXform.AddRotateXOp();
        rotX.Set(90.0f);
        
        UsdGeomCylinder trunk = UsdGeomCylinder::Define(stage, cyl_path);
        double radius = branch->radius();
        double length = branch->length();
        trunk.CreateRadiusAttr(VtValue(radius));
        trunk.CreateHeightAttr(VtValue(length));
        trunk.CreateExtentAttr(VtValue(VtArray<GfVec3f>{GfVec3f(-radius, -radius, -length*0.5),GfVec3f(radius, radius, length*0.5)}));
        auto trunkColor = trunk.GetDisplayColorAttr();
        trunkColor.Set(VtValue(VtArray<GfVec3f>{GfVec3f{0.78f, 0.64f, 0.37f}}));
        
        int i = 0;
        for (const auto* twig : branch->children_const())
        {
            std::string attachStr("attach");
            attachStr.append(std::to_string(i));
            SdfPath childPath = branch_path.AppendChild(TfToken(attachStr));
            auto childXform = UsdGeomXform::Define(stage, childPath);
            auto trans = childXform.AddTranslateOp();
            trans.Set(VtValue(GfVec3d{0.0, twig->distFromRoot(), 0.0}));
            auto rotY = childXform.AddRotateYOp();
            rotY.Set(twig->rotFromParent());
            auto rotZ = childXform.AddRotateZOp();
            rotZ.Set(twig->anglefromParent());
            
            //auto lean = childXform.AddRotateZOp();
            //lean.Set(twig->lean());
            convertTreeToUsd(twig, stage, childPath);
            i++;
        }
    }
}

void generateTree(pxr::UsdStageRefPtr stage)
{
    const SdfPath treeRootPath("/tree");
    auto rootXform = UsdGeomXform::Define(stage, treeRootPath);
    
    const int numIter = 4;
    const int branchesPerIter = 5;
    const float height = 3.0f; // trunk length
    const float radius = 0.5f;
    const float spread = 30.0f; // all angles in degrees
    const float rotation = 0.0f;
    const float lean = 5.0f;    // think of this as effect of wind blowing from a consistent direction
    const float dist = height;
    
    Branch tree(height, radius, spread, rotation, dist);
    growTree(&tree, numIter, branchesPerIter, spread, lean);
    convertTreeToUsd(&tree, stage, treeRootPath);
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
