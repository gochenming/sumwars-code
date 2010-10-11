#include "application.h"

#include "tooltipmanager.h"
#include "itemwindow.h"
#include "templateloader.h"
#include "music.h"
#include "debugpanel.h"
#include "guidebugtab.h"
#include "luascripttab.h"
#include "textfileeditwindow.h"

#include "OgreConfigFile.h"

#ifdef __APPLE__
#include <physfs.h>
// Return the path to where files should be stored on mac
Ogre::String userPath()
{
    Ogre::String path = PHYSFS_getUserDir();
    path.append("Library/Application Support/Sumwars");
    return path;
}
#endif

Application::Application(char *argv)
{
	// Anwendung initialisieren
	bool ret = false;
	try
	{
		ret = init(argv);
	}
	catch (std::exception &e)
	{
		ERRORMSG("Error message: %s",e.what());
	}

	if (ret == false)
	{
		// ERRORMSG("Initialisation failed, stop");
		m_shutdown = true;
	}

}

bool Application::init(char *argv)
{
#ifdef __APPLE__
    // Initialise PHYSFS for later on
    if (PHYSFS_init(argv) == 0)
    {
        printf("init failed: %s\n", PHYSFS_getLastError());
        return false;
    }
    Ogre::String path = userPath();
    if (PHYSFS_setWriteDir(PHYSFS_getUserDir()) == 0)
    {
        printf("setWriteDir failed: %s\n", PHYSFS_getLastError());
        return false;
    }
    if (!PHYSFS_exists("Library/Application Support/Sumwars"))
    {
        if (PHYSFS_mkdir("Library/Application Support/Sumwars") == 0)
        {
            printf("mkdir failed: %s\n", PHYSFS_getLastError());
            return false;
        }
    }    
#endif
    
	// Logger initialisieren
	LogManager::instance().addLog("stdout",new StdOutLog(Log::LOGLEVEL_DEBUG));
#ifndef __APPLE__
	LogManager::instance().addLog("logfile",new FileLog("sumwars.log",Log::LOGLEVEL_INFO));
#else
    LogManager::instance().addLog("logfile",new FileLog(path + "/sumwars.log",Log::LOGLEVEL_INFO));
#endif

	Timer tm;
	m_timer.start();
	bool ret = true;

	// Ogre Root erzeugen
	// pluginfile: plugins.cfg
	// configfile: keines
#ifdef WIN32

	m_ogre_root = new Ogre::Root("plugins_win.cfg", "ogre.cfg");
#elif defined __APPLE__
    Ogre::String plugins = macPath();
    m_ogre_root = new Ogre::Root(plugins + "/plugins_mac.cfg", plugins + "/ogre.cfg", path + "/ogre.log");
#else
	m_ogre_root = new Ogre::Root("plugins.cfg", "ogre.cfg");
#endif

	if (m_ogre_root ==0)
		return false;

	// Ogre configurieren
	ret = configureOgre();
	if (ret==false)
	{
		ERRORMSG("Configuring Ogre failed");
		return false;
	}

	// Ogre initialisieren
	ret = initOgre();
	if (ret==false)
	{
		ERRORMSG("Initialisation of Ogre failed");
		return false;
	}

	// Ressourcen festlegen
	ret = setupResources();
	if (ret==false)
	{
		ERRORMSG("Setting up Resources failed");
		return false;
	}

	//Gettext initialisieren
	ret = initGettext();
	if (ret==false)
	{
		ERRORMSG("Initialisation of Gettext failed");
		return false;
	}

	tm.start();
	//CEGUI initialisieren
	ret = initCEGUI();
	if (ret==false)
	{
		ERRORMSG("Initialisation of CEGUI failed");
		return false;
	}

	// Document anlegen
	ret = createDocument();
	if (ret==false)
	{
		ERRORMSG("cant create document");
		return false;
	}
	// View anlegen
	ret = createView();
	if (ret==false)
	{
		ERRORMSG("cant create view");
		return false;
	}

	ret = initOpenAL();
	if (ret == false)
	{
		ERRORMSG("Sound initialisation failed");
	}

	DEBUG("time to start %f",tm.getTime());
	// Ressourcen laden
	ret = loadResources();
	if (ret == false)
	{
		ERRORMSG("could not load ressources");
	}

	DEBUG("application initialized \n\n");
	// debugging
	//MyFrameListener* mfl = new MyFrameListener(m_main_window,m_document);
	//m_ogre_root->addFrameListener(mfl);

	return true;
}


Application::~Application()
{
	// dynamisch angelegte Objekte in umgekehrter Reihenfolge freigeben

	printf("deleting application\n");
	delete m_main_window;
	delete m_document;
    CEGUI::OgreRenderer::destroySystem(); // deletes everything
	delete m_ogre_root;
	
	
	ObjectFactory::cleanup();
	ItemFactory::cleanup();
	SoundSystem::cleanup();
	LogManager::cleanup();
    
#ifdef __APPLE__
    PHYSFS_deinit();
#endif
}

void Application::run()
{
	Ogre::Timer timer;
	float time[7]={0,0,0,0,0,0,0},t;
	float frametime;
	int count=0;
	Ogre::Timer timer2;

	int nr = 100;
	timer.reset();
	while (m_document->getState() != Document::END_GAME)
	{

		frametime =timer.getMicroseconds ()/1000.0;
		time[0] += frametime;
		timer.reset();


		count ++;
		if (count ==nr)
		{

			count =0;
			/*
			DEBUG("average stats over %i frames",nr);
			DEBUG("frame time: %f (%f fps)",time[0]/nr, 1000/(time[0]/nr));
			DEBUG("app update time: %f",time[1]/nr);
			DEBUG("message pump time: %f",time[2]/nr);
			DEBUG("world update time: %f",time[3]/nr);
			DEBUG("scene update time: %f",time[4]/nr);
			DEBUG("gui update time: %f",time[5]/nr);
			DEBUG("ogre  time: %f \n",time[6]/nr);


			for (int i=0; i<7; i++)
			time[i]=0;
			*/

		}

		timer2.reset();

		update();

		t =timer2.getMicroseconds ()/1000.0;
		time[1] += t;
		if (t > 20)
		{
			DEBUG("update time was %f",t);
		}


		timer2.reset();
		// run the message pump
		Ogre::WindowEventUtilities::messagePump();

		t =timer2.getMicroseconds ()/1000.0;
		time[2] += t;
		if (t>20)
		{
			DEBUGX("message pump time was %f",t);
		}

		timer2.reset();

		// Document aktualisieren
		m_document->update(frametime);

		t =timer2.getMicroseconds ()/1000.0;
		time[3] += t;
		if (t> 20)
		{
			DEBUGX("document update time was %f",t);
		}

		try
		{
			timer2.reset();

			// View aktualisieren
			DEBUGX("main window update");
			m_main_window->update(frametime);

			t =timer2.getMicroseconds ()/1000.0;
			time[4] += t;
			if (t > 20)
			{
				DEBUGX("view update time was %f",t);
			}
		}
		catch (CEGUI::Exception e)
		{
			ERRORMSG("Error message: %s",e.getMessage().c_str());
			return;
		}



		timer2.reset();

		m_cegui_system->injectTimePulse(frametime/1000.0);

		t =timer2.getMicroseconds ()/1000.0;
		time[5] += t;

		if (t> 20)
		{
			DEBUGX("cegui update time was %f",t);
		}

		// rendern
		timer2.reset();

		m_ogre_root->renderOneFrame();

		t =timer2.getMicroseconds ()/1000.0;
		time[6] += t;
		if (t > 200)
		{
			DEBUGX("ogre frame time was %f",t);
		}

		// Musik aktualisieren
		MusicManager::instance().update();
	}



}

bool Application::initOgre()
{
	DEBUG("init ogre");
	// Fenster anlegen, Ogre initialisieren
	m_window = m_ogre_root->initialise(true,"Summoning Wars");

	// Szenemanager anlegen
	m_scene_manager = m_ogre_root->createSceneManager(Ogre::ST_GENERIC,"DefaultSceneManager");

    /*// set Shadows enabled before any mesh is loaded
	m_scene_manager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
	m_scene_manager->setShadowTextureSelfShadow(false);
	m_scene_manager->setShadowTextureConfig(0,2048,2048,Ogre::PF_X8R8G8B8);
	m_scene_manager->setShadowColour( Ogre::ColourValue(0.4, 0.4, 0.4) );
	m_scene_manager->setShadowFarDistance(2000);
*/
	Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_LOW);
	return true;

}

bool Application::configureOgre()
{
	DEBUG("configure ogre");
	// Logging nur fuer Fehlermeldungen verwenden
	Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_LOW );
	// Rendering System waehlen
	// Liste aller verfuegbaren Systeme ausgeben lassen und das erste davon nehmen

    if(!m_ogre_root->showConfigDialog())
    {
        //Ogre
        delete m_ogre_root;
        return false; // Exit the application on cancel
    }

	return true;
}

bool Application::setupResources()
{
	DEBUG("initalizing resources");
#ifdef NOMIPMAPS
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(0);
#endif
    Ogre::String path = "";
#ifdef __APPLE__
    path = macPath();
#endif
	
	
	
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");


	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path + archName, typeName, secName);
		}
	}
	
	

#ifndef __APPLE__
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./save", "FileSystem", "Savegame");
#else
    Ogre::String homePath = userPath();
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(homePath, "FileSystem", "Savegame");
#endif



#if defined(WIN32)
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("c:\\windows\\fonts", "FileSystem", "GUI");
#endif

	// Gruppen initialisieren
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Savegame");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("GUI");


	// Debugging: Meshes direkt anlegen

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

	Ogre::MeshManager::getSingleton().createPlane("square44", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,200,200,1,1,true,1,1,1,Ogre::Vector3::UNIT_X);


	Ogre::MeshManager::getSingleton().createPlane("rect81", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,400,50,1,1,true,1,1,1,Ogre::Vector3::UNIT_X);



	return true;
}

bool Application::initGettext()
{
	DEBUG("initializing internationalisation");

	Gettext::init("");
	return true;
}

bool Application::initCEGUI()
{
	DEBUG("init CEGUI\n");
    
	CEGUI::OgreRenderer::bootstrapSystem();

	// Log level
	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
    
	// Scheme laden
	CEGUI::SchemeManager::getSingleton().create((CEGUI::utf8*)"TaharezLook.scheme", (CEGUI::utf8*)"GUI");

	// Imagesets laden
	CEGUI::ImagesetManager::getSingleton().create("skills.imageset");

    CEGUI::Texture &startScreenTex = CEGUI::System::getSingleton().getRenderer()->createTexture("startscreen.png", (CEGUI::utf8*)"GUI");
    
	try
	{
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("startscreen.png", "startscreen.png");
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("worldMap.png","worldMap.png",(CEGUI::utf8*)"GUI");
	}
	catch (CEGUI::Exception& e)
	{
		DEBUG("CEGUI exception %s",e.getMessage().c_str());
	}
    m_cegui_system = CEGUI::System::getSingletonPtr();

	// Mauscursor setzen (evtl eher in View auslagern ? )
	m_cegui_system->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
    m_cegui_system->setDefaultTooltip((CEGUI::utf8*)"TaharezLook/Tooltip");
    
	// Font setzen
	CEGUI::FontManager::getSingleton().create("DejaVuSerif-8.font", (CEGUI::utf8*)"GUI");
	CEGUI::FontManager::getSingleton().create("DejaVuSerif-10.font", (CEGUI::utf8*)"GUI");
	CEGUI::FontManager::getSingleton().create("DejaVuSerif-12.font", (CEGUI::utf8*)"GUI");
	CEGUI::FontManager::getSingleton().create("DejaVuSerif-16.font", (CEGUI::utf8*)"GUI");
	CEGUI::FontManager::getSingleton().create("DejaVuSans-10.font", (CEGUI::utf8*)"GUI");
	m_cegui_system->setDefaultFont((CEGUI::utf8*)"DejaVuSerif-8");

	// eigene Factorys einfuegen
    CEGUI::WindowFactoryManager::getSingleton().addFalagardWindowMapping ("SumwarsTooltip", "DefaultWindow", "TaharezLook/CustomTooltip", "Falagard/Default");
	CEGUI::WindowFactoryManager::getSingleton().addFactory< CEGUI::TplWindowFactory<GuiDebugTab> >();
	CEGUI::WindowFactoryManager::getSingleton().addFactory< CEGUI::TplWindowFactory<LuaScriptTab> >();
	CEGUI::WindowFactoryManager::getSingleton().addFactory< CEGUI::TplWindowFactory<TextFileEditWindow> >();
	
	return true;
}

bool Application::initOpenAL()
{
	SoundSystem::init();
	bool err = SoundSystem::checkErrors();
	return !err;
}

bool Application::createDocument()
{
	DEBUG("create document\n");
	m_document = new Document();

	/*
	m_document->installShortkey(OIS::KC_ESCAPE,CLOSE_ALL);
	m_document->installShortkey(OIS::KC_I,SHOW_INVENTORY);
	m_document->installShortkey(OIS::KC_C,SHOW_CHARINFO);
	m_document->installShortkey(OIS::KC_T,SHOW_SKILLTREE);
	m_document->installShortkey(OIS::KC_P,SHOW_PARTYMENU);
	m_document->installShortkey(OIS::KC_M,SHOW_CHATBOX);
	m_document->installShortkey(OIS::KC_Q,SHOW_QUESTINFO);
	m_document->installShortkey(OIS::KC_TAB,SHOW_MINIMAP);
	m_document->installShortkey(OIS::KC_F10,SHOW_OPTIONS);
	m_document->installShortkey(OIS::KC_W,SWAP_EQUIP);
	m_document->installShortkey(OIS::KC_LMENU,SHOW_ITEMLABELS);

	m_document->installShortkey(OIS::KC_RETURN,SHOW_CHATBOX_NO_TOGGLE,false);
	m_document->installShortkey(OIS::KC_1,USE_POTION,false);
	m_document->installShortkey(OIS::KC_2,(ShortkeyDestination) (USE_POTION+1),false);
	m_document->installShortkey(OIS::KC_3,(ShortkeyDestination) (USE_POTION+2),false);
	m_document->installShortkey(OIS::KC_4,(ShortkeyDestination) (USE_POTION+3),false);
	m_document->installShortkey(OIS::KC_5,(ShortkeyDestination) (USE_POTION+4),false);
	m_document->installShortkey(OIS::KC_6,(ShortkeyDestination) (USE_POTION+5),false);
	m_document->installShortkey(OIS::KC_7,(ShortkeyDestination) (USE_POTION+6),false);
	m_document->installShortkey(OIS::KC_8,(ShortkeyDestination) (USE_POTION+7),false);
	m_document->installShortkey(OIS::KC_9,(ShortkeyDestination) (USE_POTION+8),false);
	m_document->installShortkey(OIS::KC_0,(ShortkeyDestination) (USE_POTION+9),false);

	m_document->installSpecialKey(OIS::KC_ESCAPE);
	m_document->installSpecialKey(OIS::KC_LSHIFT);
//	m_document->installSpecialKey(OIS::KC_LCONTROL);
//	m_document->installSpecialKey(OIS::KC_RCONTROL);
//	m_document->installSpecialKey(OIS::KC_RMENU);
//	m_document->installSpecialKey(OIS::KC_LMENU);
	m_document->installSpecialKey(OIS::KC_TAB);
	m_document->installSpecialKey(OIS::KC_RETURN);
	m_document->installSpecialKey(OIS::KC_BACK);
	m_document->installSpecialKey(OIS::KC_UP);
	m_document->installSpecialKey(OIS::KC_DOWN);
	m_document->installSpecialKey(OIS::KC_LEFT);
	m_document->installSpecialKey(OIS::KC_RIGHT);
	m_document->installSpecialKey(OIS::KC_CAPITAL);
	// CHEATS
	m_document->installShortkey(OIS::KC_L,(ShortkeyDestination) (CHEAT+0));
*/
	m_document->loadSettings();
	

	return true;
}

bool Application::createView()
{
	DEBUG("create view\n");
	m_main_window = new MainWindow(m_ogre_root, m_cegui_system,m_window,m_document);

	
	new DebugPanel();
	DebugPanel::getSingleton().init(true);
	
	TooltipManager *mgr = new TooltipManager();
	mgr->setFadeInTime(200.0f);
	mgr->setFadeOutTime(200.0f);
	mgr->setVisibleTime(0.0f);
	
	return true;
}

bool Application::loadResources()
{
	TiXmlBase::SetCondenseWhiteSpace(false);

	Ogre::FileInfoListPtr files;
	Ogre::FileInfoList::iterator it;
	std::string file;

	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("itempictures","*.png");
	for (it = files->begin(); it != files->end(); ++it)
	{

		file = it->filename;

		CEGUI::ImagesetManager::getSingleton().createFromImageFile(file,file,(CEGUI::utf8*)"itempictures");

		updateStartScreen(0.1);
	}

	// Imagesets laden
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("itempictures","*.imageset");
	for (it = files->begin(); it != files->end(); ++it)
	{

		file = it->filename;

		CEGUI::ImagesetManager::getSingleton().create(file);

		updateStartScreen(0.2);
	}

	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("emotionsets","*.imageset");
	for (it = files->begin(); it != files->end(); ++it)
	{

		file = it->filename;

		CEGUI::ImagesetManager::getSingleton().create(file);

		updateStartScreen(0.3);
	}

	Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("General");
	updateStartScreen(0.4);
	Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("Savegame");
	updateStartScreen(0.5);
	Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("GUI");

	// Spielerklassen initialisieren
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("playerclasses","*.xml");
	for (it = files->begin(); it != files->end(); ++it)
	{
		file = it->archive->getName();
		file += "/";
		file += it->filename;

		ObjectLoader::loadPlayerData(file.c_str());

		updateStartScreen(0.55);
	}

	// Items initialisieren
	ItemFactory::init();
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("items","*.xml");
	for (it = files->begin(); it != files->end(); ++it)
	{
		file = it->archive->getName();
		file += "/";
		file += it->filename;

		ItemLoader::loadItemData(file.c_str());

		updateStartScreen(0.6);
	}

	// Monster initialisieren

	// Objekt Templates
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("obj_templates","*.xml");
	for (it = files->begin(); it != files->end(); ++it)
	{
		file = it->archive->getName();
		file += "/";
		file += it->filename;

		TemplateLoader::loadObjectTemplateData(file.c_str());

		updateStartScreen(0.65);
	}

	// Objekt Gruppen Templates
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("object_groups","*.xml");
	for (it = files->begin(); it != files->end(); ++it)
	{
		file = it->archive->getName();
		file += "/";
		file += it->filename;
		TemplateLoader::loadObjectGroupData(file.c_str());

		updateStartScreen(0.7);
	}

	// Render Infos
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("renderinfo","*.xml");
	for (it = files->begin(); it != files->end(); ++it)
	{
		file = it->archive->getName();
		file += "/";
		file += it->filename;
		GraphicManager::loadRenderInfoData(file.c_str());

		updateStartScreen(0.8);
	}

	// Sounds
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("sounddata","*.xml");
	for (it = files->begin(); it != files->end(); ++it)
	{
		file = it->archive->getName();
		file += "/";
		file += it->filename;

		SoundSystem::loadSoundData(file.c_str());

		updateStartScreen(0.9);
	}

	Ogre::MeshManager& mesh_mgr = Ogre::MeshManager::getSingleton();
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	mesh_mgr.createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 200, 200, 5, 5, true, 1,1,1,Ogre::Vector3::UNIT_X);

	updateStartScreen(1.0);

	m_main_window->setReadyToStart(true);

	return true;
}

void  Application::update()
{

}

void Application::updateStartScreen(float percent)
{
	if (m_timer.getTime() < 20)
	{
		return;
	}

	DEBUGX("update time %f  perc: %f",m_timer.getTime(), percent);
	m_main_window->update(m_timer.getTime()/1000);
	m_main_window->setRessourceLoadingBar(percent);
	m_cegui_system->injectTimePulse(m_timer.getTime()/1000);

	Ogre::WindowEventUtilities::messagePump();

	m_ogre_root->renderOneFrame();
	//MusicManager::instance().update();
	m_timer.start();
}
