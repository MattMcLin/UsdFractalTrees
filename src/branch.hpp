//
//  branch.hpp
//  UsdFractalTrees
//
//  Created by Matthew McLin on 3/16/19.
//

#ifndef branch_hpp
#define branch_hpp

#include <vector>
#include "pxr/usd/usdGeom/xform.h"

using namespace pxr;

class Branch
{
public:
    Branch();
    Branch(float length, float radius, float angle, float rotation, float dist);
    ~Branch();
    
    void addBranch(Branch* pBranch);
    std::vector<Branch*>& children() { return m_children; }
    const std::vector<Branch*>& children_const() const { return m_children; }
    
    bool isLeaf() const { return (m_children.size() == 0); }
    
    float length() const { return m_length; }
    float radius() const { return m_radius; }
    float anglefromParent() const { return m_angleFromParent; }
    float rotFromParent() const { return m_rotFromParent; }
    float distFromRoot() const { return m_distFromRoot; }
    
private:
    float m_length = 0.0f;
    float m_radius = 0.0f;
    float m_angleFromParent = 0.0f; // angle between this branch and its parent
    float m_rotFromParent = 0.0f;   // direction that this branch exits from parent
    float m_distFromRoot = 0.0f;
    
    std::vector<Branch*> m_children;
};

#endif /* branch_hpp */
