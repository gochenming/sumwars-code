
#ifndef APPLICATION_H
#define APPLICATION_H



#include "OgreTimer.h"
#include "OgreWindowEventUtilities.h"
#include "OgreLogManager.h"
#include "OgreMeshManager.h"

// needed to be able to create the CEGUI renderer interface
#include "OgreCEGUIRenderer.h"


// CEGUI includes
#include "CEGUISystem.h"
#include "CEGUIInputEvent.h"
#include "CEGUIWindow.h"
#include "CEGUIWindowManager.h"
#include "CEGUISchemeManager.h"
#include "CEGUIFontManager.h"
#include "elements/CEGUIFrameWindow.h"

#include <stdio.h>

#include "mainwindow.h"
#include "timer.h"
#include "graphicmanager.h"

#ifdef __APPLE__
#include <CoreFoundation/CFBundle.h>
#endif

/**
 * \class Application
 * \brief Basisklasse der Anwendung
 */

class Application
{

	public:
		/**
		 * \fn Application()
		 * \brief Konstruktor
		 */
		Application(char *argv);

		/**
		 * \fn ~Application()
		 * \brief Destruktor
		 */
		virtual ~Application();

		/**
		 * \fn run()
		 * \brief Startet die Applikation
		 */
		void run();

		/**
		 * \fn Document* getDocument()
		 * \brief Gibt einen Zeiger auf das Dokument aus
		 */
		Document* getDocument()
		{
			return m_document;
		}

	private:
		// Funktionen
		/**
		* \fn init()
		* \brief Initialisiert die Anwendung
		 */
		bool init(char *argv);

		/**
		 * \fn initOgre()
		 * \brief Initialisiert die Ogre Rendering Engine
		 */
		bool initOgre();

		/**
		 * \fn configureOgre()
		 * \brief Konfiguriert die Ogre Rendering Engine
		 */
		bool configureOgre();

		/**
		 * \fn setupResources()
		 * \brief Stellt die Ressourcen fuer Ogre und CEGUI ein
		 */
		bool setupResources();

		/**
		 * \fn initGettext()
		 * \brief Initialisiert Gettext
		 */
		bool initGettext();

		/**
		 * \fn initCEGUI()
		 * \brief Initialisiert CEGUI
		 */
		bool initCEGUI();

		/**
		 * \brief initialisiert OpenAL
		 */
		bool initOpenAL();

		/**
		 * \fn bool createView()
		 * \brief Erzeugt die Ansichten
		 */
		bool createView();

		/**
		 * \fn bool createDocument()
		 * \brief Erzeugt das Document
		 */
		bool createDocument();

		/**
		 * \fn bool loadResources()
		 * \brief liest die Ressourcen
		 */
		bool loadResources();

		/**
		 * \fn virtual void update()
		 * \brief Aktualisierungen
		 */
		virtual void update();

		/**
		 * \fn void updateStartScreen(float percent)
		 * \brief aktualisiert den Startbildschirm
		 * \param percent Prozentsatz, zu dem das Laden der Daten fortgeschritten ist
		 */
		void updateStartScreen(float percent);
        
#ifdef __APPLE__
        /**
         * \fn Ogre::String macPath()
         * \brief Returns the path to the Resources directory on mac
         */
        Ogre::String macPath()
        {
            Ogre::String path;
            CFBundleRef mainBundle = CFBundleGetMainBundle();
            CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
            char resPath[PATH_MAX];
            CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)resPath, PATH_MAX);
            CFRelease(resourcesURL);
            path = resPath;
            return path;
        }
#endif

		// Member
		/**
		 * \var Ogre::Root *m_ogre_root
		 * \brief Basisobjekt der Ogre Engine
		 */
		Ogre::Root *m_ogre_root;

		/**
		 * \var CEGUI::System* m_cegui_system
		 * \brief Basisobjekt von CEGUI
		 */
		CEGUI::System* m_cegui_system;

		/**
		 * \var CEGUI::OgreCEGUIRenderer* m_ogre_cegui_renderer
		 * \brief Objekt zum Rendern von CEGUI mittels Ogre
		 */
		CEGUI::OgreCEGUIRenderer* m_ogre_cegui_renderer;

		/**
		 * \var Ogre::RenderWindow *m_window
		 * \brief Fenster der Applikation
		 */
		Ogre::RenderWindow *m_window;

		/**
		 * \var Ogre::SceneManager *m_scene_manager
		 * \brief Objekt fuer das Management der Szene
		 */
 		Ogre::SceneManager *m_scene_manager;


		/**
		 * \var bool m_shutdown
		 * \brief true, wenn die Anwendung beendet werden soll
		 */
		bool m_shutdown;


	protected:
		/**
		 * \var Document* m_document
		 * \brief Dokument, enthaelt alle Daten fuer die Darstellung
		 */
		Document* m_document;

		/**
		 * \var MainWindow* m_main_window
		 * \brief Hauptfenster der Applikation
		 */
		MainWindow* m_main_window;

		/**
		 * \var Timer m_timer
		 * \brief Timer fuer verschiedene Verwendungszwecke
		 */
		Timer m_timer;
};




















#endif

