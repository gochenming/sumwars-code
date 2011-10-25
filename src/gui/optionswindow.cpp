/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "optionswindow.h"
#include "sound.h"
#include "listitem.h"
#include "music.h"
#include "sumwarshelper.h"

// The following includes are added to support video mode changes in the options window.
#include <OGRE/OgreConfigFile.h>
#include <OGRE/OgreException.h>

OptionsWindow::OptionsWindow (Document* doc, OIS::Keyboard *keyboard)
	:Window(doc)
{
	m_keyboard = keyboard;

	DEBUGX("setup main menu");
	// GUI Elemente erzeugen

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	CEGUI::FrameWindow* options = (CEGUI::FrameWindow*) win_mgr.loadWindowLayout("OptionsWindow.layout");
	m_window = options;
	options->setAlwaysOnTop(true);
	
	// Rahmen fuer das Menue Savegame auswaehlen
	CEGUI::TabControl* optionstab = (CEGUI::TabControl*) win_mgr.getWindow("OptionsWindowTab");
	optionstab->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&OptionsWindow::onAreaMouseButtonPressed, this));
	CEGUI::Window* label;
	CEGUI::Scrollbar* slider;

	label = win_mgr.getWindow("OptionsWindowTab__auto_TabPane__Buttons__auto_btnOptionsShortkeys");
	label->setInheritsAlpha(false);
	
	label = win_mgr.getWindow("OptionsWindowTab__auto_TabPane__Buttons__auto_btnOptionsGameplay");
	label->setInheritsAlpha(false);
	
	label = win_mgr.getWindow("OptionsWindowTab__auto_TabPane__Buttons__auto_btnOptionsSound");
	label->setInheritsAlpha(false);
	
	label = win_mgr.getWindow("OptionsWindowTab__auto_TabPane__Buttons__auto_btnOptionsGraphic");
	label->setInheritsAlpha(false);
	
	label = win_mgr.getWindow("OptionsWindowTab__auto_TabPane__Buttons__auto_btnOptionsMisc");
	label->setInheritsAlpha(false);
	


	int targets[9] = {SHOW_INVENTORY, SHOW_CHARINFO, SHOW_SKILLTREE, SHOW_PARTYMENU, SHOW_CHATBOX, SHOW_QUESTINFO, SHOW_MINIMAP, SWAP_EQUIP, SHOW_ITEMLABELS};

	std::ostringstream stream;
	for (int i=0; i<9; ++i)
	{
		stream.str("");
		stream << "ShortkeyLabel"<<i;

		label = win_mgr.getWindow(stream.str());
		label->setID(targets[i]);
		label->setWantsMultiClickEvents(false);

		stream.str("");
		stream << "ShortkeyValueLabel"<<i;

		label = win_mgr.getWindow(stream.str());
		label->setID(targets[i]);
		label->setWantsMultiClickEvents(false);
		label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&OptionsWindow::onShortkeyLabelClicked,  this));
	}
	
	CEGUI::Combobox* diffcbo = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("DifficultyBox"));
	diffcbo->addItem(new ListItem((CEGUI::utf8*) gettext("Easy"),Options::EASY));
	diffcbo->addItem(new ListItem((CEGUI::utf8*) gettext("Normal"),Options::NORMAL));
	diffcbo->addItem(new ListItem((CEGUI::utf8*) gettext("Hard"),Options::HARD));
	diffcbo->addItem(new ListItem((CEGUI::utf8*) gettext("Insane"),Options::INSANE));
	diffcbo->setReadOnly(true);
	diffcbo->setItemSelectState((size_t) 0,true);
	diffcbo->handleUpdatedListItemData();
	diffcbo->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&OptionsWindow::onDifficultyChanged, this));
	diffcbo->setItemSelectState( (size_t) (Options::getInstance()->getDifficulty()-1),true);
	
	slider = static_cast<CEGUI::Scrollbar*>(win_mgr.getWindow("TextSpeedSlider"));
	slider->setPageSize (0.01f);
	slider->setDocumentSize(1.0f);
	slider->setStepSize(0.01f);
	slider->setWantsMultiClickEvents(false);
	slider->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber(&OptionsWindow::onTextSpeedChanged,  this));
	
	slider = static_cast<CEGUI::Scrollbar*>(win_mgr.getWindow("MusicVolumeSlider"));
	slider->setPageSize (0.01f);
	slider->setDocumentSize(1.0f);
	slider->setStepSize(0.01f);
	slider->setWantsMultiClickEvents(false);
	slider->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber(&OptionsWindow::onMusicVolumeChanged,  this));

	slider = static_cast<CEGUI::Scrollbar*>(win_mgr.getWindow("SoundVolumeSlider"));
	slider->setPageSize (0.01f);
	slider->setDocumentSize(1.0f);
	slider->setStepSize(0.01f);
	slider->setWantsMultiClickEvents(false);
	slider->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber(&OptionsWindow::onSoundVolumeChanged,  this));

	CEGUI::Combobox* ehlcbo = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("EHColorBox"));
	ehlcbo->addItem(new StrListItem((CEGUI::utf8*) gettext("White"), "white", 0));
	ehlcbo->addItem(new StrListItem((CEGUI::utf8*) gettext("Black"), "black", 0));
	ehlcbo->addItem(new StrListItem((CEGUI::utf8*) gettext("Red"), "red", 0));
	ehlcbo->addItem(new StrListItem((CEGUI::utf8*) gettext("Green"), "green", 0));
	ehlcbo->addItem(new StrListItem((CEGUI::utf8*) gettext("Blue"), "blue", 0));
	ehlcbo->addItem(new StrListItem((CEGUI::utf8*) gettext("Yellow"), "yellow", 0));
	ehlcbo->addItem(new StrListItem((CEGUI::utf8*) gettext("Magenta"), "magenta", 0));
	ehlcbo->addItem(new StrListItem((CEGUI::utf8*) gettext("Cyan"), "cyan", 0));
	ehlcbo->addItem(new StrListItem((CEGUI::utf8*) gettext("Orange"), "orange", 0));
	ehlcbo->addItem(new StrListItem((CEGUI::utf8*) gettext("Pink"), "pink", 0));
	ehlcbo->addItem(new StrListItem((CEGUI::utf8*) gettext("Purple"), "purple", 0));
	ehlcbo->addItem(new StrListItem((CEGUI::utf8*) gettext("Cornflower Blue"), "cornflower_blue", 0));

	ehlcbo->setReadOnly(true);
	ehlcbo->setItemSelectState((size_t) getColorSelectionIndex("red"), true);
	ehlcbo->handleUpdatedListItemData();
	ehlcbo->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&OptionsWindow::onEnemyHighlightChanged, this));
	ehlcbo->setItemSelectState((size_t) (getColorSelectionIndex(Options::getInstance()->getEnemyHighlightColor())), true);


	CEGUI::Checkbox *chkbox = (CEGUI::Checkbox *) win_mgr.getWindow("GrabMouseInWindowedModeBox");
	chkbox->setSelected(Options::getInstance()->getGrabMouseInWindowedMode());
	chkbox->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&OptionsWindow::onGrabMouseChanged, this));
	
	CEGUI::PushButton* btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("ResetGraphicsButton"));
	btn->setID(5);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsWindow::onResetGraphics, this));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("OptionsCloseButton"));
	btn->setID(5);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsWindow::onButtonOkClicked, this));

	CEGUI::Combobox* cbo = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("LanguageBox"));
	cbo->addItem(new StrListItem((CEGUI::utf8*) gettext("System default"),"",0));
	cbo->addItem(new StrListItem("German","de_DE",0));
	cbo->addItem(new StrListItem("English GB","en_GB",0));
	cbo->addItem(new StrListItem("English US","en_US",0));
	cbo->addItem(new StrListItem("Italian IT","it_IT",0));
	cbo->addItem(new StrListItem("Russian RU","ru_RU",0));
	cbo->addItem(new StrListItem("Ukrainian UK","uk_UA",0));

	cbo->setReadOnly(true);

	int selection = 0;
	std::string locale = Options::getInstance()->getLocale();
	int pos = locale.find(".");
	if (pos != std::string::npos)
	{
		locale = locale.substr(0,pos);
	}
	DEBUG("%s",locale.c_str());
	
	for (int i=0; i < (int)cbo->getItemCount(); i++)
	{
		StrListItem* item = static_cast<StrListItem*>(cbo->getListboxItemFromIndex(i));
		if (locale == item->m_data)
		{
			selection = i;
		}
	}
	cbo->setItemSelectState((size_t) selection,true);
	cbo->handleUpdatedListItemData();

	cbo->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&OptionsWindow::onLanguageSelected, this));

	// TODO: add comments.
	cbo = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("DisplayModeBox"));
	cbo->addItem(new StrListItem((CEGUI::utf8*) gettext("Fullscreen (Exclusive Mode)"),"",0));
	cbo->addItem(new StrListItem((CEGUI::utf8*) gettext("Window (With Borders)"),"",0));

	if (Options::getInstance ()->isWindowedFullscreenModeSupported ())
	{
		DEBUG ("Windowed fullscreen mode supported! Adding combobox item.");
		std::string modeName = gettext("Window (Fullscreen)");
		std::string recommended = gettext (" - recommended");
		std::string sum = modeName + recommended;

		cbo->addItem(new StrListItem(sum,"",0));
	}
	else
	{
		DEBUG ("Windowed fullscreen mode NOT supported!");
	}

	cbo->setReadOnly(true);
	DisplayModes myDisplayMode = Options::getInstance ()->getUsedDisplayMode ();
	if (myDisplayMode == WINDOWED_FULLSCREEN && !Options::getInstance ()->isWindowedFullscreenModeSupported ())
	{
		myDisplayMode = WINDOWED_WITH_BORDER;
	}
	cbo->setItemSelectState((size_t) myDisplayMode, true);
	cbo->handleUpdatedListItemData();
	cbo->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&OptionsWindow::onDisplayModeSelected, this));

	// TODO: add comments.
	cbo = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("VideoDriverBox"));
	std::vector <std::string> videoDrivers = Options::getInstance ()->getEditableAvailableVideoDrivers ();
	std::string selectedVideoDriver = Options::getInstance ()->getUsedVideoDriver ();
	for (std::vector <std::string>::const_iterator it = videoDrivers.begin (); it != videoDrivers.end (); ++ it)
	{
		std::string myName = *it;
		cbo->addItem(new StrListItem((CEGUI::utf8*) myName.c_str (),"",0));
		if (myName == selectedVideoDriver)
		{
			cbo->setItemSelectState (cbo->getItemCount () - 1, true);
		}
	}
	cbo->setReadOnly (true);
	cbo->setEnabled (false);

	cbo->handleUpdatedListItemData();
//	cbo->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&OptionsWindow::onDisplayModeSelected, this));

	// TODO: add comments.
	std::string usedResolution = Options::getInstance ()->getUsedResolution ();
	DEBUG ("Used resolution is: [%s]", usedResolution.c_str ());
	cbo = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("ResolutionBox"));
	std::vector <std::string> resolutions = Options::getInstance ()->getEditableResolutionsMapping ()[selectedVideoDriver];
	for (std::vector <std::string>::const_iterator it = resolutions.begin (); it != resolutions.end (); ++ it)
	{
		std::string myName = *it;
		cbo->addItem(new StrListItem((CEGUI::utf8*) myName.c_str (),"",0));
		if (myName == usedResolution)
		{
			cbo->setItemSelectState (cbo->getItemCount () - 1, true);
		}
	}
	cbo->setReadOnly (true);

	// For windowed (fullscreen) mode, make sure the resolution can't be edited.
	if (myDisplayMode == WINDOWED_FULLSCREEN)
	{
		cbo->setEnabled (false);
		// Clear the selection flag.
		if (cbo->getSelectedItem () != 0)
		{
			cbo->getSelectedItem ()->setSelected (false);
		}
		cbo->setText ("");
	}

	cbo->handleUpdatedListItemData();
//	cbo->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&OptionsWindow::onResolutionChanged, this));

	reset();
	updateTranslation();
}

void OptionsWindow::update()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	Options* options = Options::getInstance();

	std::ostringstream stream;
	KeyCode key;
	std::string keyname;
	// Schleife ueber die Labels der Kurztasten
	for (int i=0; i<9; i++)
	{
		stream.str("");
		stream << "ShortkeyValueLabel"<<i;
		label = win_mgr.getWindow(stream.str());

		if (m_key_destination == (int) label->getID())
		{
			// die Kurztaste soll gerade eingegeben werden
			keyname = "_";
		}
		else
		{
			key = options->getMappedKey( (ShortkeyDestination) label->getID());
			keyname = m_keyboard->getAsString ( (OIS::KeyCode) key);
		}

		if (label->getText() != keyname)
		{
			label->setText(keyname);
		}
	}

	CEGUI::Scrollbar* slider = static_cast<CEGUI::Scrollbar*>(win_mgr.getWindow( "SoundVolumeSlider"));
	if ( fabs ( slider->getScrollPosition() - SoundSystem::getSoundVolume()) > 0.01f)
	{
		slider->setScrollPosition(SoundSystem::getSoundVolume());
	}

	slider = static_cast<CEGUI::Scrollbar*>(win_mgr.getWindow( "MusicVolumeSlider"));
	if ( fabs ( slider->getScrollPosition() - MusicManager::instance().getMusicVolume()) > 0.01f)
	{
		slider->setScrollPosition(MusicManager::instance().getMusicVolume());
	}
	
	slider = static_cast<CEGUI::Scrollbar*>(win_mgr.getWindow( "TextSpeedSlider"));
	float slidpos = (2.0 -Options::getInstance()->getTextSpeed()) / 1.4;
	if ( fabs ( slider->getScrollPosition() - slidpos) > 0.01f)
	{
		slider->setScrollPosition(slidpos);
	}

}

void OptionsWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	CEGUI::Checkbox* box;
	
	CEGUI::DefaultWindow* keys =  (CEGUI::DefaultWindow*) win_mgr.getWindow("OptionsShortkeys");
	keys->setText((CEGUI::utf8*) gettext("Shortkeys"));
	CEGUI::DefaultWindow* gameplay =  (CEGUI::DefaultWindow*) win_mgr.getWindow("OptionsGameplay");
	gameplay->setText((CEGUI::utf8*) gettext("Gameplay"));
	CEGUI::DefaultWindow* sound = (CEGUI::DefaultWindow*) win_mgr.getWindow("OptionsSound");
	sound->setText((CEGUI::utf8*) gettext("Audio"));
	CEGUI::DefaultWindow* graphic = (CEGUI::DefaultWindow*) win_mgr.getWindow("OptionsGraphic");
	graphic->setText((CEGUI::utf8*) gettext("Graphic"));
	CEGUI::DefaultWindow* misc = (CEGUI::DefaultWindow*) win_mgr.getWindow("OptionsMisc");
	misc->setText((CEGUI::utf8*) gettext("Language"));

	label = win_mgr.getWindow("OptionsLabel");
	label->setText((CEGUI::utf8*) gettext("Options"));

	label = win_mgr.getWindow("ShortkeyLabel0");
	label->setText((CEGUI::utf8*) gettext("Inventory"));

	label = win_mgr.getWindow("ShortkeyLabel1");
	label->setText((CEGUI::utf8*) gettext("Character screen"));

	label = win_mgr.getWindow("ShortkeyLabel2");
	label->setText((CEGUI::utf8*) gettext("Skilltree"));

	label = win_mgr.getWindow("ShortkeyLabel3");
	label->setText((CEGUI::utf8*) gettext("Party screen"));

	label = win_mgr.getWindow("ShortkeyLabel4");
	label->setText((CEGUI::utf8*) gettext("Chatbox"));

	label = win_mgr.getWindow("ShortkeyLabel5");
	label->setText((CEGUI::utf8*) gettext("Quests"));

	label = win_mgr.getWindow("ShortkeyLabel6");
	label->setText((CEGUI::utf8*) gettext("Minimap"));

	label = win_mgr.getWindow("ShortkeyLabel7");
	label->setText((CEGUI::utf8*) gettext("Swap equipement"));

	label = win_mgr.getWindow("ShortkeyLabel8");
	label->setText((CEGUI::utf8*) gettext("Item Labels"));

	label = win_mgr.getWindow("GameplayDifficultyLabel");
	label->setText((CEGUI::utf8*) gettext("Difficulty"));
	
	label = win_mgr.getWindow("TextSpeedLabel");
	label->setText((CEGUI::utf8*) gettext("Text Speed"));
	
	label = win_mgr.getWindow("SoundVolumeLabel");
	label->setText((CEGUI::utf8*) gettext("Sound"));

	label = win_mgr.getWindow("MusicVolumeLabel");
	label->setText((CEGUI::utf8*) gettext("Music"));

	label = win_mgr.getWindow("EnemyHighlightLabel");
	label->setText((CEGUI::utf8*) gettext("Enemy Highlight Color"));

	label = win_mgr.getWindow("LanguageLabel");
	label->setText((CEGUI::utf8*) gettext("Language"));
	
	label = win_mgr.getWindow("ResetGraphicsLabel");
	label->setText((CEGUI::utf8*) gettext("Restart required for setting new options"));

	box = static_cast<CEGUI::Checkbox*>(win_mgr.getWindow("GrabMouseInWindowedModeBox"));
	box->setText((CEGUI::utf8*) gettext("Grab mouse in windowed mode (needs restart)"));
	
	label = win_mgr.getWindow("DisplayMode");
	label->setText((CEGUI::utf8*) gettext("Display Mode"));
	
	// Set the close button to "ok"
	CEGUI::PushButton* btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow ("OptionsCloseButton"));
	btn->setText((CEGUI::utf8*) gettext("Ok"));

	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( "ResetGraphicsButton"));
	btn->setText((CEGUI::utf8*) gettext("Reset graphic options"));
}


void OptionsWindow::reset()
{
	m_key_destination = NO_KEY;
}

bool OptionsWindow::onShortkeyLabelClicked(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	int id = (int) we.window->getID();

	if (m_key_destination == id)
	{
		m_key_destination = NO_KEY;
	}
	else
	{
		m_key_destination =(ShortkeyDestination) id;
	}
	return true;
}

void OptionsWindow::setKeyCode(KeyCode kc)
{
	if (requestsForKey())
	{
		ShortkeyMap& ablt_map = m_document->getAbilityShortkeys();
		ablt_map.erase(kc);
		
		Options::getInstance()->setShortkey(kc,m_key_destination);
		m_key_destination = NO_KEY;
	}
}

bool OptionsWindow::onAreaMouseButtonPressed(const CEGUI::EventArgs& evt)
{
	m_key_destination = NO_KEY;
	return true;
}

bool OptionsWindow::onButtonOkClicked (const CEGUI::EventArgs& evt)
{
	// TODO: move video mode checks to separate function? or just wait for the full integration with the rest of the options?

	// Check to see if the display mode was updated. The display mode is stored in a different file at the moment.
	// TODO: move towards integrated settings file (single file for most settings).
	{
		CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
		CEGUI::Combobox* cbo = cbo = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("DisplayModeBox"));

		if (cbo == 0)
		{
			return false;
		}

		CEGUI::ListboxItem* item = cbo->getSelectedItem();

		if (item == 0)
		{
			return false;
		}

		size_t selectionIndex = (int)cbo->getItemIndex (item);

		DEBUG ("selected display mode %s",item->getText().c_str());
		DEBUG ("idx of selection is: %d", selectionIndex);

		bool someVideoSettingsWereChanged (false);
		DisplayModes userMode = (DisplayModes)selectionIndex;
		someVideoSettingsWereChanged |= (userMode != Options::getInstance ()->getUsedDisplayMode ());

		std::string userResolution;
		cbo = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("ResolutionBox"));

		if (cbo == 0)
		{
			return false;
		}

		userResolution = string (cbo->getText ().c_str ());
		if (userResolution.length () == 0)
		{
			userResolution = Options::getInstance ()->getUsedResolution ();
		}
		DEBUG ("selected userResolution %s", userResolution.c_str());

		if (userMode == WINDOWED_FULLSCREEN)
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			int desktopWidth = (int)GetSystemMetrics (SM_CXSCREEN);
			int desktopHeight = (int)GetSystemMetrics (SM_CYSCREEN);
			userResolution = SumwarsHelper::getUpdatedResolutionString (userResolution, desktopWidth, desktopHeight);
#endif
			DEBUG ("Updated selected userResolution to [%s]", userResolution.c_str());
		}

		someVideoSettingsWereChanged |= (userResolution != Options::getInstance ()->getUsedResolution ());

		if (someVideoSettingsWereChanged)
		{
			// Store it into the options; for the time being it is only saved there, not read from there!
			// The read is performed from the actual settings!
			// TODO: initialize the graphics subsystem with the settings in the options.xml file instead of the default ogre file.
			Options::getInstance ()->setUsedDisplayMode (userMode);
			Options::getInstance ()->setUsedResolution (userResolution);

			std::string configpath;
#ifdef _WIN32
			configpath =  SumwarsHelper::userPath() + "/ogre.cfg";
#elif defined __APPLE__
			configpath = SumwarsHelper::macPath() + "/ogre.cfg";
#else
			configpath =  SumwarsHelper::userPath() + "/ogre.cfg";
#endif

			// TODO: move handling to specialized function.
#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
#pragma message("Iphone support not added for saving video mode!")
#endif
	        Ogre::ConfigFile cfg;
			try
			{
				// Don't trim whitespace
				cfg.load (configpath, "\t:=", false);
			}
			catch (Ogre::Exception& e)
			{
				if (e.getNumber () == Ogre::Exception::ERR_FILE_NOT_FOUND)
				{
					return false;
				}
				else
				{
					throw;
				}
	        }

			// Prepare the list of values to write.

			Ogre::String fullScreenValueToSet ("No");
			if (Options::getInstance ()->getUsedDisplayMode () == FULLSCREEN_EX)
			{
				fullScreenValueToSet = "Yes";
			}

			Ogre::String resolutionValueToSet ("");
			resolutionValueToSet = Options::getInstance ()->getUsedResolution ();
			DEBUG ("Saving options; will set resolution to : [%s]", resolutionValueToSet);

			// Update the settings; 
			Ogre::ConfigFile::SectionIterator iSection = cfg.getSectionIterator ();
			while (iSection.hasMoreElements ())
			{
				const Ogre::String& renderSystemName = iSection.peekNextKey ();
				Ogre::ConfigFile::SettingsMultiMap& settings = *iSection.getNext();

				Ogre::ConfigFile::SettingsMultiMap::iterator it;

				// Note: this will update for ALL render systems.
				it = settings.find ("Full Screen");
				if (it != settings.end ())
				{
					it->second = fullScreenValueToSet;
				}

				// Note: this will update only for the selected render systems.
				if (renderSystemName == Options::getInstance ()->getUsedVideoDriver ())
				{
					DEBUG ("*** checking video mode setting");
					it = settings.find ("Video Mode");
					if (it != settings.end ())
					{
						it->second = resolutionValueToSet;
						DEBUG ("*** Set res entry");
					}
				}
			}

			// ----------------------------------------------------------------
			// Write the Ogre.cfg file
			std::ofstream of (configpath);

	        if (!of)
			{
				ERRORMSG ("Cannot write config");
			}
	
			iSection = cfg.getSectionIterator ();
			while (iSection.hasMoreElements ())
			{
				const Ogre::String& renderSystemName = iSection.peekNextKey ();
				if (renderSystemName.length () > 0)
				{
					of << "[" << renderSystemName << "]" << std::endl;
				}
				Ogre::ConfigFile::SettingsMultiMap& settings = *iSection.getNext ();

				Ogre::ConfigFile::SettingsMultiMap::const_iterator it;
				for (it = settings.begin (); it != settings.end (); ++ it)
				{
					of << it->first << "=" << it->second << std::endl;
				}
				of << std::endl;
			}

	        of.close();
			// ----------------------------------------------------------------
	
			// Show a notification.
			CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton ();
			CEGUI::FrameWindow* message = (CEGUI::FrameWindow*) win_mgr.getWindow ("WarningDialogWindow");
			message->setInheritsAlpha (false);
			message->setVisible (true);
			message->setModalState (true);
			win_mgr.getWindow ("WarningDialogLabel")->setText ((CEGUI::utf8*) gettext ("Please restart the game\n for the changes to take effect!"));

		} // user display mode changed.
	}

	m_document->onButtonOptionsClicked();
	return true;
}

bool OptionsWindow::onSoundVolumeChanged(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);

	CEGUI::Scrollbar* slider = static_cast<CEGUI::Scrollbar*>(we.window);
	float vol = slider->getScrollPosition();
	DEBUGX("sound volume change to %f",vol);
	SoundSystem::setSoundVolume(vol);
	return true;
}

bool OptionsWindow::onDifficultyChanged(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
	static_cast<const CEGUI::MouseEventArgs&>(evt);
	
	CEGUI::Combobox* cbo = static_cast<CEGUI::Combobox*>(we.window);
	
	CEGUI::ListboxItem* item = cbo->getSelectedItem();
	if (item != 0)
	{
		Options::getInstance()->setDifficulty(static_cast<Options::Difficulty>(item->getID()));
	}
	
	return true;
}

bool OptionsWindow::onTextSpeedChanged(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
	static_cast<const CEGUI::MouseEventArgs&>(evt);
	
	CEGUI::Scrollbar* slider = static_cast<CEGUI::Scrollbar*>(we.window);
	float speed = 2.0 - slider->getScrollPosition()*1.4;
	DEBUGX("text speed changed to %f",speed);
	Options::getInstance()->setTextSpeed(speed);
	return true;
}

bool OptionsWindow::onMusicVolumeChanged(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);

	CEGUI::Scrollbar* slider = static_cast<CEGUI::Scrollbar*>(we.window);
	float vol = slider->getScrollPosition();
	DEBUGX("music volume changed to %f",vol);
	MusicManager::instance().setMusicVolume(vol);
	return true;
}

bool OptionsWindow::onEnemyHighlightChanged(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);

	CEGUI::Combobox* cbo = static_cast<CEGUI::Combobox*>(we.window);
	CEGUI::ListboxItem* item = cbo->getSelectedItem();

	if (item != 0)
	{
		DEBUGX("enemy highlight color changed to %s", item->getText().c_str());
		StrListItem* sitem = static_cast<StrListItem*>(item);
		Options::getInstance()->setEnemyHighlightColor(sitem->m_data.c_str());
	}

	return true;
}

bool OptionsWindow::onResetGraphics(const CEGUI::EventArgs& evt)
{
	std::string configpath;
#ifdef _WIN32
	configpath =  SumwarsHelper::userPath() + "/ogre.cfg";
#elif defined __APPLE__
	configpath = macPath() + "/ogre.cfg";
#else
	configpath =  SumwarsHelper::userPath() + "/ogre.cfg";
#endif
	remove(configpath.c_str());
	return true;
}

bool OptionsWindow::onLanguageSelected(const CEGUI::EventArgs& evt)
{

	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);

	CEGUI::Combobox* cbo = static_cast<CEGUI::Combobox*>(we.window);

	CEGUI::ListboxItem* item = cbo->getSelectedItem();

	if (item != 0)
	{
		DEBUGX("selected Language %s",item->getText().c_str());
		StrListItem* sitem = static_cast<StrListItem*>(item);
		Gettext::setLocale(sitem->m_data.c_str());
	}

	return true;
}

bool OptionsWindow::onDisplayModeSelected (const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);

	CEGUI::Combobox* cbo = static_cast<CEGUI::Combobox*>(we.window);

	if (cbo == 0)
	{
		return false;
	}

	CEGUI::ListboxItem* item = cbo->getSelectedItem();

	if (item == 0)
	{
		return false;
	}

	size_t selectionIndex = (int)cbo->getItemIndex (item);

	//DEBUG ("selected display mode %s",item->getText().c_str());
	//DEBUG ("idx of selection is: %d", selectionIndex);
	//Options::getInstance ()->setUsedDisplayMode ((DisplayModes)selectionIndex);

	DisplayModes myDisplayMode = (DisplayModes)selectionIndex;

	// For windowed (fullscreen) mode, make sure the resolution can't be edited.

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	cbo = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("ResolutionBox"));
	if (myDisplayMode == WINDOWED_FULLSCREEN)
	{
		cbo->setEnabled (false);
		// Clear the selection flag.
		if (cbo->getSelectedItem () != 0)
		{
			cbo->getSelectedItem ()->setSelected (false);
		}
		cbo->setText ("");
		cbo->handleUpdatedListItemData();
	}
	else
	{
		cbo->setEnabled (true);

		std::string usedResolution = Options::getInstance ()->getUsedResolution ();
		
		DEBUG ("Will try to show that the used resolution is [%s]", usedResolution.c_str ());

		for (size_t i = 0; i < cbo->getItemCount (); ++ i)
		{
			CEGUI::ListboxItem* item = cbo->getListboxItemFromIndex (i);
			CEGUI::String itemText = item->getText ();
			if (usedResolution == itemText)
			{
				cbo->setItemSelectState(i, true);
				
				DEBUG ("Located item [%s] at [%d]", itemText.c_str (), i);
			}
			DEBUG ("Compared with [%s]", itemText.c_str ());
		}
		cbo->handleUpdatedListItemData();
	}

	return true;
}

bool OptionsWindow::onGrabMouseChanged(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);

	CEGUI::Checkbox* cbo = static_cast<CEGUI::Checkbox*>(we.window);
	
	Options::getInstance()->setGrabMouseInWindowedMode(cbo->isSelected());
	
	return true;
}


unsigned int OptionsWindow::getColorSelectionIndex(std::string name)
{
	std::string list[] = {
			"white",
			"black",
			"red",
			"green",
			"blue",
			"yellow",
			"magenta",
			"cyan",
			"orange",
			"pink",
			"purple",
			"cornflower_blue"
	};

	for (unsigned int i = 0; i < 12; i++)
	{
		if (name == list[i])
		{
			return i;
		}
	}

	return 0;
}

