#include "Rockets.h"
#include "chrono"
#include "random"



auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
std::mt19937 rng(seed);
std::uniform_int_distribution<int> dist(1.6F, 7.F);




Rockets::Rockets()
{
	m_RBody.Init(0.03F, 0.03F, 0.25F, &m_RMaterial, 20, true, true);
	m_RPlacement.AddGeo(&m_RBody);
	distance[0] = 0.f;
	distance[1] = 0.f;

}

Rockets::Rockets(CPlacement* parentplac)
{
	
	m_RBody.Init(0.03F, 0.03F, 0.25F, &m_RMaterial, 20, true, true);
	parentplac->AddPlacement(&m_RPlacement);
	m_RPlacement.AddGeo(&m_RBody);
	distance[0] = dist(rng);
	distance[1] = dist(rng);
	m_RMaterial.MakeTextureDiffuse("textures\\red_image.jpg");
		
}
