#pragma once
#include "Vektoria\Root.h"

using namespace Vektoria; 
class Rockets
{
public:
	Rockets();
	Rockets(CPlacement *parentplac);
	CPlacement m_RPlacement;
	CGeoCylinder m_RBody;
	CMaterial m_RMaterial;
	float distance[2];
};

