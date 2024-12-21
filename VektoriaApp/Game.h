#pragma once
//-----------------------------------------------------------------------
// CGame
// 
// Erklärung : Eine weitestgehend leere Klasse, 
//             sinnfällig in eine Windows-Applikation eingebettet,
//             um ein Spiel bzw. eine 3D-Simulation zu erzeugen
// Autor     : Prof. Dr. Tobias Breiner
// Ort       : Pfungstadt
// Zeit      : seit Aug. 2011 (mit seitdem ständigen Verbesserungs-Updates)
// Copyright : Tobias Breiner  
// Disclaimer: Nutzung auf eigene Gefahr, keine Gewähr, no warranty!
//------------------------------------------------------------------------


#ifdef _WIN64
#ifdef _DEBUG
#pragma comment (lib, "Vektoria_Debug64.lib")
#pragma comment (lib, "VektoriaMath_Debug64.lib")
#else
#pragma comment (lib, "Vektoria_Release64.lib")
#pragma comment (lib, "VektoriaMath_Release64.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment (lib, "Vektoria_Debug.lib")
#pragma comment (lib, "VektoriaMath_Debug.lib")
#else
#pragma comment (lib, "Vektoria_Release.lib")
#pragma comment (lib, "VektoriaMath_Release.lib")
#endif
#endif


#include "Vektoria\Root.h"
#define ANZAHLASTEROIDEN 300
using namespace Vektoria;

class Rockets; 

class CGame
{
public:
	// Wird vor Begin einmal aufgerufen (Konstruktor):
	CGame(void);																				

	// Wird nach Ende einmal aufgerufen (Destruktor):
	~CGame(void);																				

	// Wird zu Begin einmal aufgerufen:
	void Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash * psplash);	

	// Wird während der Laufzeit bei jedem Bildaufbau aufgerufen:
	void Tick(float fTime, float fTimeDelta);						

	// Wird am Ende einmal aufgerufen:
	void Fini();																				

	// Wird immer dann aufgerufen, wenn der Benutzer die Fenstergröße verändert hat:
	void WindowReSize(int iNewWidth, int iNewHeight);											

	// Holt das minimale Zeitdelta zur eventuellen Frame-Rate-Beschränkung:
	float GetTimeDeltaMin();																	

	// Holt die Versionsnummer:
	float GetVersion();

	void GenRockets();
	void DoRocketTick();
	




private:
	
	 
	CRoot m_zr; 
	CFrame m_zF;
	CViewport m_zV;
	COverlay m_zO;
	CImage m_zI;
	CScene m_zS;
	CPlacement m_zPCam;
	CCamera m_zC;
	CPlacement m_zPSphareEarth;
	CLightParallel m_zL;
	CGeoSphere m_zgSphereEarth;
	CMaterial m_zMEarth;

	CPlacement m_zPlacementSky;
	CGeoSphere m_zgSphereSky;
	CMaterial m_zMSky;

	CPlacement  m_zPlaccementemms;
	CPlacement m_zPlacementMoon;
	CGeoSphere m_zgSphereMoon;
	CMaterial m_zMMoon;

	CPlacement m_zPlacementSun;
	CGeoSphere m_zgSphereSun;
	CMaterial m_zMSun;

	CPlacement m_zPlacementDeath;
	CGeoSphere m_zgShpereDeath;
	CMaterial m_zDeath;

	CDeviceKeyboard m_zdKey;
	CDeviceCursor m_zdCurs;
	CGeos m_gclickable;
	
	Rockets * prArray[11];

	COverlay m_oStart;

	CPlacement m_azpAsteroid[ANZAHLASTEROIDEN];
	CGeoQuad m_zgAsteroid;
	CMaterial m_zmAsteroid;
	CHVector m_avAsteroid[ANZAHLASTEROIDEN];

	COverlay m_oLclick;
	COverlay m_oRclick;
	COverlay m_oSingle;
	COverlay m_oMulti;
	COverlays m_oButtons;
	bool m_bGamemode=false;
	CAudio m_zMusic;

	// Importer für Obj
	CFileWavefront m_fileObj;
	CGeoTriangleTable* m_triangleTable = nullptr;
	CPlacement m_zpSpaceship;
	CMaterial m_zmSpaceship;

	// Das Blaupausen-Bullet: 
	CPlacement m_zPBullet;
	CGeoCylinder m_zgBullet;
	CMaterial m_zBullet;
	CPlacements m_zPBullets;
	CHVector m_avBulletLastPos[30];
	CAudio m_azaGun[30];

};


