//
//  branch.cpp
//  UsdFractalTrees
//
//  Created by Matthew McLin on 3/16/19.
//

#include "branch.hpp"

Branch::Branch()
{
}

Branch::Branch(float length, float radius, float angle, float rotation, float dist)
:
    m_length(length),
    m_radius(radius),
    m_angleFromParent(angle),
    m_rotFromParent(rotation),
    m_distFromRoot(dist)
{
}

Branch::~Branch()
{
    for (auto* branch : m_children)
    {
        delete branch;
    }
    m_children.clear();
}

void Branch::addBranch(Branch* pBranch)
{
    m_children.push_back(pBranch);
}
