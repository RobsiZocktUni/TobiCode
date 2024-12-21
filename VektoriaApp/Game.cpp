#include "Game.h"
#include <iostream>
#include "Rockets.h"
#include <chrono>
#include <random>


auto main_seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
std::mt19937 gen(main_seed);
std::uniform_int_distribution<int> distribution(1, 2);

CGame::CGame(void)
{
}

CGame::~CGame(void)
{
}

void CGame::Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash * psplash)
{
	//---------------------------------
	// Initialisiere die Knotenobjekte:
	//---------------------------------



	// Zuallererst muss die Root mit dem Splash-Screen initialisiert werden, 
	// damit die Engine freigeschaltet wird:
	m_zr.Init(psplash, false, false, true);
	m_zC.Init(QUARTERPI);
	m_zCamL.Init(QUARTERPI);
	m_zCamR.Init(QUARTERPI);
	m_zF.Init(hwnd, procOS);
	m_zF.AddDeviceKeyboard(&m_zdKey);
	m_zF.AddDeviceCursor(&m_zdCurs);
	m_zV.InitFull(&m_zC);
	m_zVL.InitHalfLeft(&m_zCamL);
	m_zVR.InitHalfRight(&m_zCamR);
	m_zVR.SwitchOff();
	m_zVL.SwitchOff();
	m_zL.Init(CHVector(-1.0f, -1.0f, -1.0f), CColor(1.0f, 1.0f, 1.0f));
	m_zgSphereEarth.Init(20.F, &m_zMEarth, 50, 50);
	m_zgSphereSky.Init(1000000.0F, &m_zMSky, 50, 50);
	m_zgSphereMoon.Init(8.f, &m_zMMoon, 50, 50);
	m_zgSphereSun.Init(65.f, &m_zMSun, 50, 50);
	m_zgShpereDeath.Init(7.F, &m_zDeath,50,50);
	m_zO.InitFull(&m_zI);
	m_zgSphereSky.Flip();
	// Hier kommen alle weiteren Initialisierungen hinein: 
	m_zr.AddFrame(&m_zF);
	m_zF.AddViewport(&m_zV);
	m_zF.AddViewport(&m_zVR);
	m_zF.AddViewport(&m_zVL);
	m_zr.AddScene(&m_zS);
	m_zS.AddPlacement(&m_zPCam);
	m_zS.AddPlacement(&m_zPCam2);
	m_zS.AddPlacement(&m_zPlacementSun);
	m_zPlacementSun.AddPlacement(&m_zPlaccementemms);
	m_zPlaccementemms.AddPlacement(&m_zPSphareEarth);
	m_zS.AddPlacement(&m_zPlacementSky);
	m_zS.AddLightParallel(&m_zL);
	m_zPCam.AddCamera(&m_zC);
	m_zPCam.AddCamera(&m_zCamL);
	m_zPCam2.AddCamera(&m_zCamR);
	m_zPSphareEarth.AddGeo(&m_zgSphereEarth);
	m_zPlacementSky.AddGeo(&m_zgSphereSky);
	m_zMEarth.MakeTextureDiffuse("textures\\earth_image.jpg");
	m_zMEarth.MakeTextureGlow("textures\\earth_glow.jpg");
	m_zMEarth.MakeTextureBump("textures\\earth_bump.jpg");
	m_zMEarth.MakeTextureSpecular("textures\\earth_refection.jpg");
	m_zMSky.MakeTextureSky("textures\\starmap_2020_4k_print.jpg");
	m_zPCam.TranslateZ(100.0f);
	m_zPCam.SetTranslationSensitivity(100);
	
	m_zPlaccementemms.AddPlacement(&m_zPlacementMoon);
	m_zPlacementMoon.AddGeo(&m_zgSphereMoon);
	m_zMMoon.MakeTextureDiffuse("textures\\moon_image.jpg");
	m_zPlacementSun.AddGeo(&m_zgSphereSun);

	m_zPlaccementemms.AddPlacement(&m_zPlacementDeath);
	m_zPlacementDeath.AddGeo(&m_zgShpereDeath);
	std::cout << "pre gen" << std::endl;	
	CGame::GenRockets();
	std::cout << "post gen" << std::endl;
	m_gclickable.Add(&m_zgShpereDeath);


	//Asteroidengürtel um die Sonne:
	m_zmAsteroid.MakeTextureBillboard("textures\\Asteroid.png");
	m_zmAsteroid.SetChromaKeyingOn();
	m_zgAsteroid.Init(1.f, &m_zmAsteroid);

	for (int i = 0; i < ANZAHLASTEROIDEN; i++) //Tobis Code
	{
		m_zPlacementSun.AddPlacement(&m_azpAsteroid[i]);
		m_azpAsteroid[i].SetBillboard(eBillboardType_Parallel);
		m_azpAsteroid[i].AddGeo(&m_zgAsteroid);

		// Befülle für jeden Stein einen Zufallsvektor: 

		float fRadius;
		float fHeight;
		float fa = rand() % 10000 / 10000.0f * TWOPI;
		if (rand() % 2)
			fRadius = 0.6f + 1.0f / (rand() % 100 + 1);
		else
			fRadius = 0.6f - 1.0f / (rand() % 100 + 1);

		if (rand() % 2)
			fHeight = +1.0f / (rand() % 100 + 1);
		else
			fHeight = -1.0f / (rand() % 100 + 1);
		fRadius *= 200.0f;

		m_avAsteroid[i] = CHVector(
			// X,Y und Z-Tupel werden für die Anfangstranslation hergenommen: 
			// Es wird eine modifizierte Kreisfunktion verwendet: 
			sinf(fa) * fRadius,
			fHeight,
			cosf(fa) * fRadius,
			// Der W-Tupel wird für die Geschwindigkeit
			// missbraucht, je weiter der Stein von der Erde entfernt ist, 
			// desto langsamer soll er sich drehen: 
			0.1f / fRadius);

		// Variiere die Steine noch bezüglich Winkel und Größe: 
		m_azpAsteroid[i].SetBillboardAngle((rand() % 3600) / 1000.0f);
		m_azpAsteroid[i].SetBillboardScaling(3.0f + (rand() % 1000) / 1000.0f);
	}
	m_zMusic.Init("textures//AtoS_rereMASTER.wav");
	m_zS.AddAudio(&m_zMusic);
	m_zMusic.Loop();
	m_zMusic.SetVolume(0.85f);
	//m_zMusic.SetFrequency(44100.f);

	m_zS.SetPhysicsSpace();

	m_oLclick.Init("textures\\green_image.jpg",C2dRect(0.1f,0.1f));
	m_oRclick.Init("textures\\black_image.jpg", C2dRect(0.1f, 0.1f));
	m_oSingle.Init("textures\\red_image.jpg",C2dRect(0.2f,0.1f,0.75f,0.8f));
	m_oMulti.Init("textures\\blue_image.jpg", C2dRect(0.2f,0.1f,0.75f,0.65f));

	// Importiere Geometrie
	m_triangleTable = m_fileObj.LoadGeoTriangleTable("textures//prometheus.obj");
	m_zmSpaceship.MakeTextureDiffuse("textures//StartScreen.jpg");
	m_triangleTable->SetMaterial(&m_zmSpaceship);
	m_zPCam.AddPlacement(&m_zpSpaceship);
	m_zpSpaceship.AddGeo(m_triangleTable);
	
	m_zpSpaceship.TranslateZDelta(-20);
	m_zpSpaceship.TranslateYDelta(-2.5f);

	m_zBullet.LoadPreset("MetalRustyFlaking");
	m_zgBullet.SetAxis(eAxisZ);
	m_zgBullet.Init(0.5, 1.0f, &m_zBullet);
	m_zPBullet.AddGeo(&m_zgBullet);
	m_zPBullet.SetPhysics(2.0f, 0.1f, 0.0f, 2000000.0f, true);
	m_zPBullet.SwitchOff();
	m_zS.AddPlacement(&m_zPBullet);

	m_zPBullets.RingMake(30, m_zPBullet);
	m_zS.AddPlacements(m_zPBullets);

	for (int i = 0; i < 30; i++) {
		m_zS.AddAudio(&m_azaGun[i]);
		m_azaGun[i].Init("textures//Gun.wav");
	}


	//deathstar bullets
	m_zgDeathBullet.SetAxis(eAxisZ);
	m_zgDeathBullet.Init(0.5, 1.0f, &m_zBullet);
	m_zPDeathBullet.AddGeo(&m_zgDeathBullet);
	m_zPDeathBullet.SetPhysics(2.0f, 0.1f, 0.0f, 2000000.0f, true);
	m_zPDeathBullet.SwitchOff();
	m_zS.AddPlacement(&m_zPDeathBullet);

	m_zPDeathBullets.RingMake(30, m_zPDeathBullet);
	m_zS.AddPlacements(m_zPDeathBullets);

	for (int i = 0; i < 30; i++) {
		m_zS.AddAudio(&m_azaDeathGun[i]);
		m_azaDeathGun[i].Init("textures//Gun.wav");
	}



	m_oLclick.SetLayer(0.f);
	m_oRclick.SetLayer(0.f);
	m_oSingle.SetLayer(0.6f);
	m_oMulti.SetLayer(0.6f);
	m_zV.AddOverlay(&m_oStart);
	m_zV.AddOverlay(&m_oSingle);
	m_zV.AddOverlay(&m_oMulti);
	m_zV.AddOverlay(&m_oLclick);
	m_zV.AddOverlay(&m_oRclick);
	
	m_oStart.InitFull("textures\\start.jpg");
	m_oStart.SetLayer(0.7f);

	m_oButtons.Add(&m_oSingle);
	m_oButtons.Add(&m_oMulti);
}

void CGame::Tick(float fTime, float fTimeDelta)
{
	//-------------------------------------------
	// Veränderungen während der Simulationszeit:
	//-------------------------------------------
	
	for (int i = 0; i < 30; i++) {
		if (m_zPBullets.m_applacement[i]->IsOn())
			m_avBulletLastPos[i] = m_zPBullets.m_applacement[i]->GetPosGlobal();
	}

	if (m_zdKey.KeyDown(DIK_SPACE)) {
		if (m_zPBullets.RingIsNotFull()) {
			CPlacement* pzp = m_zPBullets.RingInc();
			pzp->SetMat(m_zPCam.GetMatGlobal());
			CHVector vDirect(m_zPCam.GetDirectionGlobal());
			vDirect.Normal();
			pzp->SetPhysicsVelocity(vDirect * 80.f);
			m_azaGun[m_zPBullets.m_uRingStart].Start();
		}
	}
	CPlacement* pzPOldestBullet = m_zPBullets.RingAskLast();
	if (pzPOldestBullet) {
		if ((pzPOldestBullet->GetPosGlobal() - m_zPCam.GetPosGlobal()).Length() > 200.0f)
			m_zPBullets.RingDec();
	}



	float fx, fy;
	m_zdCurs.GetFractional(fx, fy, true);
	if (m_zdCurs.ButtonPressed(0))
	{
		m_oLclick.SwitchOff();
		m_oRclick.SwitchOn();
		m_oRclick.SetPos(fx - 0.1f, fy + 0.f);
	}
	else
	{
		m_oLclick.SwitchOn();
		m_oRclick.SwitchOff();
		m_oLclick.SetPos(fx - 0.1f, fy + 0.f);
	}

	if (m_zdCurs.ButtonDownLeft()) {
		if (m_zdCurs.PickOverlayPreselected(m_oButtons) == &m_oSingle) {
			m_oSingle.SwitchOff();
			m_oMulti.SwitchOff();
			m_oStart.SwitchOff();
			m_zV.SwitchOn();
			m_zVL.SwitchOff();
			m_zVR.SwitchOff();
			m_bGamemode = false;
			bgameplay = true;
		}
		else if (m_zdCurs.PickOverlayPreselected(m_oButtons) == &m_oMulti) {
			m_oSingle.SwitchOff();
			m_oMulti.SwitchOff();
			m_oStart.SwitchOff();
			
			m_zVL.SwitchOn();
			m_zVR.SwitchOn();
			m_zV.SwitchOff();
			m_bGamemode = true;
			bgameplay = true;
		}
	}

	// Hier kommen die Veränderungen pro Renderschritt hinein: 
	m_zdKey.PlaceWASD(m_zPCam, fTimeDelta, false);
	
	m_zPSphareEarth.RotateY(fTime);
	
	m_zPSphareEarth.TranslateXDelta(UM_DEG2RAD(24));
	m_zPSphareEarth.RotateZDelta(UM_DEG2RAD(24));
	m_zPlacementMoon.TranslateX(5.f);
	m_zPlaccementemms.TranslateX(180.f);
	m_zPlacementMoon.RotateYDelta(fTime * 0.5);
	m_zPlaccementemms.RotateYDelta(fTime);

	m_zPlacementDeath.TranslateX(26.f);
	m_zPlacementDeath.RotateYDelta(fTime);
	if(bgameplay)
	{
		if (m_zPDeathBullets.RingIsNotFull()) {
			CPlacement* pzp = m_zPDeathBullets.RingInc();
			pzp->SetMat(m_zPlacementDeath.GetMatGlobal());
			
			CHVector vDeathaim = m_zpSpaceship.GetPosGlobal() - m_zPlacementDeath.GetPosGlobal();
			if(m_bGamemode)
				if(distribution(gen)==2)
					CHVector vDeathaim = m_zpSpaceship.GetPosGlobal() - m_zPlacementDeath.GetPosGlobal(); //TODO change to second spaceship
			vDeathaim.Normal();
			pzp->SetPhysicsVelocity(vDeathaim * 70.f);
			m_azaGun[m_zPBullets.m_uRingStart].Start();
		}
	}
	CPlacement* pzPDeathOldestBullet = m_zPDeathBullets.RingAskLast();
	if (pzPDeathOldestBullet) {
		if ((pzPDeathOldestBullet->GetPosGlobal() - m_zPCam.GetPosGlobal()).Length() > 300.0f)
			m_zPDeathBullets.RingDec();
	}



	CGame::DoRocketTick();
	
	if (m_zdCurs.ButtonDownLeft()) {
		if (m_zdCurs.PickGeoPreselected(m_gclickable) == &m_zgShpereDeath)
			m_zDeath.MakeTextureDiffuse("textures\\red_image.jpg");
	}



	// Animiere den Ring: 
	for (int i = 0; i < ANZAHLASTEROIDEN; i++)
	{
		m_azpAsteroid[i].Translate(m_avAsteroid[i]);
		m_azpAsteroid[i].RotateYDelta(m_avAsteroid[i].w * fTime);
		m_azpAsteroid[i].RotateZDelta(UM_DEG2RAD(24));
	}

	// Traversiere am Schluss den Szenengraf und rendere:
	m_zr.Tick(fTimeDelta);
	
}




void CGame::Fini()
{
	//-------------------------------
	// Finalisiere die Knotenobjekte:
	//-------------------------------

	// Hier die Finalisierung Deiner Vektoria-Objekte einfügen:

	// Auch die Root sollte finalisiert werden:   
	m_zr.Fini();
}

void CGame::WindowReSize(int iNewWidth, int iNewHeight)
{
	// Windows ReSize wird immer automatisch aufgerufen, wenn die Fenstergröße verändert wurde.
	// Hier kannst Du dann die Auflösung des Viewports neu einstellen:
	// z.B. mittels: m_zf.ReSize(iNewWidth, iNewHeight);
}

float CGame::GetTimeDeltaMin()
{
	return m_zr.GetTimeDeltaMin();
}

float CGame::GetVersion()
{
	return m_zr.GetVersion();
}


void CGame::DoRocketTick()
{
	for (int i = 0; i < 11; i++) {
		prArray[i]->m_RPlacement.TranslateY(prArray[i]->distance[0]);
		prArray[i]->m_RPlacement.TranslateX(prArray[i]->distance[1]);    
	}
}




void CGame::GenRockets() {


	for (int i = 0; i < 11; i++) {


		prArray[i] =new  Rockets(&m_zPlaccementemms);


		/*Rocket tmp = {};
		tmp.distance[0] = dist(rng);
		tmp.distance[1] = dist(rng);
		m_zPlaccementemms.AddPlacement(&tmp.m_RPlac);
		tmp.m_RPlac.AddGeo(&tmp.m_RGeo);
		tmp.m_RGeo.Init(0.03F, 0.03F, 0.25F, &tmp.m_RMat, 20, true, true);
		tmp.m_RMat.MakeTextureDiffuse("textures\\red_image.jpg");
		rArray[i] = tmp;*/

	}
}
