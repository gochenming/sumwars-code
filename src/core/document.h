/*
	Summoning Wars - document.h
	Copyright (C) 2007  Hans Wulf

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/**
 * \file document.h
 * \defgroup Document \ Dokument
 * \brief Stellt das Dokumentkonzept dar
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Beinhaltet die Klasse Document
 */

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <list>
#include <map>
#include <set>


// OIS
/*
#ifdef WIN32
    #include "ois\OISEvents.h"
    #include "ois\OISInputManager.h"
    #include "ois\OISMouse.h"
    #include "ois\OISKeyboard.h"
    #include "ois\OISJoyStick.h"
#else
    #include "OISEvents.h"
    #include "OISInputManager.h"
    #include "OISMouse.h"
    #include "OISKeyboard.h"
    #include "OISJoyStick.h"
#endif
*/

#include "networkstruct.h"
#include "projectile.h"
#include "party.h"
#include "debug.h"
#include "damage.h"
#include "itemlist.h"
#include "random.h"
#include "item.h"
#include "matrix2d.h"
#include "tiles.h"
#include "dropitem.h"
#include "world.h"
#include "player.h"
#include "timer.h"
#include <pthread.h>

#include "gettext.h"

/**
 * \class Document
 * \brief Diese Klasse enthaelt alle Daten fuer Repraesentation der Spielwelt auf der Clientseite.
 *
 * 
 */

typedef int KeyCode;

class Document
{


	public:

	


	/**
	 * \enum State
	 * \brief zaehlt verschiedene Zustaende auf, in denen sich das Dokument befinden kann
	 */
	enum State
	{
		INACTIVE =0,

		START_GAME =1,

		LOAD_SAVEGAME =2,

		RUNNING =3,

		SHUTDOWN_REQUEST =4,
  
		SHUTDOWN_WRITE_SAVEGAME=8,

		SHUTDOWN =5,

		DISCONNECTED =6,
  
 		START_SERVER =7,
	};


	/**
	 * \enum Modifications
	 * \brief Zaehlt Inhalte auf, die veraendert werden koennen
	 */
	enum Modifications
	{
		NO_MODIFICATIONS=0,
		OBJECTS_MODIFIED=1,
		GUISHEET_MODIFIED=2,
		WINDOWS_MODIFIED=4,
		ITEM_MODIFIED=8,
		ABILITY_MODIFIED=16,
		REGION_MODIFIED = 32,
	};

	/**
	 * \enum GUISheets
	 * \brief Verschiedene Ebenen die angezeigt werden können
	 */
	enum GUISheets
	{
		MAIN_MENU=1,
		GAME_SCREEN=2,
	};

	/**
	 * \enum Windows
	 * \brief Fenster die waerend des Spieles angezeigt werden koennen
	 */
	enum Windows
	{
		NO_WINDOWS =0,
		INVENTORY =1,
		CHARINFO=2,
		SKILLTREE=4,
		MENU =8,
		CHAT = 16,
		PARTY = 32,
		SAVEGAME_LIST = 64,
		START_MENU = 128,
	};

	/**
	 * \enum ShortkeyDestination
	 * \brief Listet Aktion auf, die ueber Kurztasten verfuegbar sind
	 */
	enum ShortkeyDestination
	{
		NO_KEY=0,
		SHOW_INVENTORY =1,
		SHOW_CHARINFO =2,
		SHOW_SKILLTREE =3,
		SHOW_PARTYMENU=4,
		SHOW_CHATBOX =5,
		CLOSE_ALL=9,
		SWAP_EQUIP=10,
		USE_POTION = 30,
		USE_SKILL_LEFT=100,
		USE_SKILL_RIGHT=300,
	};


	
	/**
	 * \struct GUIState
	 * \brief Informationen zum aktuellen Zustand der GUI
	 */
	struct GUIState
	{

		/**
		 * \var m_chat_window_content
		 * \brief Inhalt des Chat-Nachrichtenfensters
		 */
		string m_chat_window_content;


		/**
		 * \var m_chat_sendline_content
		 * \brief Inhalt der Chat-Eingabezeile
	 	 */
		string m_chat_sendline_content;

		 /**
		 * \var m_left_mouse_hold
		 * \brief Gibt an, ob die linke Maustaste festgehalten wird
	 	 */
		bool m_left_mouse_hold;

		 /**
		 * \var m_right_mouse_hold
		 * \brief Gibt an, ob die rechte Maustaste festgehalten wird
	 	 */
		bool m_right_mouse_hold;

		/**
		 * \var m_mouse_hold
		 * \brief Gibt an, ob die Shift-Taste festgehalten wird
		 */
		bool m_shift_hold;

		 /**
		 * \var m_clicked_object_id
		 * \brief ID des angeklickten Objekts
	 	 */
		int m_clicked_object_id;

		 /**
		 * \var m_clicked_x
		 * \brief angeklickte x-Koordinate
		  */
		float m_clicked_x;

		 /**
		 * \var m_clicked_y
		 * \brief angeklickte y-Koordinate
		  */
		float m_clicked_y;

		/**
		 * \var float m_left_mouse_hold_time
		 * \brief Zaehler der die Anzahl Frames zaehlt die die linke Maustaste festgehalten wurde
		 */
		float m_left_mouse_hold_time;


		/**
		 * \var float m_right_mouse_hold_time
		 * \brief Zaehler der die Anzahl Frames zaehlt die die rechte Maustaste festgehalten wurde
		 */
		float m_right_mouse_hold_time;

		/**
		 * \var GUISheets* m_sheet
		 * \brief Aktuell angezeigte Ebene
		 */
		GUISheets m_sheet;

		/**
		 * \var int m_shown_windows
		 * \brief aktuell angezeigte Fenster als Bitmaske
		 */
		int m_shown_windows;

		/**
		 * \var KeyCode m_pressed_key
		 * \brief aktuell festgehaltene Taste
		 */
		KeyCode m_pressed_key;

		/**
		 * \var std::string m_cursor_object
		 * \brief Ogre Name des Objektes auf das gerade der Mauszeiger zeigt
		 */
		std::string m_cursor_object;

		/**
		 * \var int m_cursor_object_id
		 * \brief ID des Objektes auf das gerade der Mauszeiger zeigt
		 */
		int m_cursor_object_id;

		/**
		 * \var int m_cursor_item_id
		 * \brief ID des Gegenstandes auf den gerade der Mauszeiger zeigt
		 */
		int m_cursor_item_id;

	};



// Konstruktor / Destruktor
	/**
	 * \fn Document()
	 * \brief Konstruktor
	 *
	 * Legt ein neues Document-Objekt an
	 */
	Document();
	
	
	/**
	 * \fn ~Document()
	 * \brief Destruktor
	 *
	 * Gibt den allokierten Speicher wieder frei
	 */
	~Document();
// Setter und Getter

	/**
	 * \fn void setSaveFile(string s)
	 * \brief Setzt den Name des Savefiles
	 * \param s Name des Savefiles
	 */
	void setSaveFile(string s)
	{
		m_save_file =s;
	}

	/**
	 * \fn string getSaveFile()
	 * \brief Gibt den Name des Savefiles aus
	 */
	string getSaveFile()
	{
		return m_save_file;
	}

	/**
	 * \fn static void* writeSaveFile(void* doc_ptr)
	 * \param doc_ptr Zeiger auf das Dokument
	 * \brief Schreibt das Savefile
	 */
	static void* writeSaveFile(void* doc_ptr);

	
	/**
	 * \fn GUISTate* getGUIState()
	 * \brief Gibt Zustand der GUI Datenfelder zurueck
	 * \return Zustand der GUI
	 */
	GUIState* getGUIState()
	{
		return &m_gui_state;
	}


	/**
	 * \fn State getState()
	 * \brief Gibt aktuellen Status des Dokuments aus
	 */
	State getState()
	{
		return m_state;
	}

	/**
	 * \fn void setState(State s)
	 * \brief Setzt den Status des Dokuments
	 */
	void setState(State s)
	{
		m_state =s;
	}

	/**
	 * \fn bool getModified()
	 * \brief Gibt aus, ob Daten des Dokumentes geaendert wurden
	 * \return modifiziert
	 */
	int getModified()
	{
		return m_modified;
	}

	/**
	 * \fn void setModified(int mod)
	 * \brief Setzt den Zustand modifiziert
	 * \param mod neuer Zustand
	 */
	void setModified(int mod)
	{
		m_modified = mod;
	}


	/**
	 * \fn Party* getParty()
	 * \brief Gibt die Party aus der der Spieler angehoert
	 * \return Party
	 */
	Party* getParty()
	{
		return 0;
	}

	/**
	 * \fn void setLeftAction(Action::ActionType a)
	 * \brief Setzt die Aktion, die durch die linke Maustaste ausgeloest wird
	 */
	void setLeftAction(Action::ActionType a);

	
	/**
	 * \fn Action::ActionType getLeftAction()
	 * \brief Gibt die Aktion, die durch die linke Maustaste ausgeloest wird, zurueck
	 */
	/*
	Action::ActionType getLeftAction()
	{
		return m_left_action;
	}
	*/

	/**
	 * \fn void setRightAction(Action::ActionType a)
	 * \brief Setzt die Aktion, die durch die rechte Maustaste ausgeloest wird
	 */
	void setRightAction(Action::ActionType a);

	
	/**
	 * \fn Action::ActionType getRightAction()
	 * \brief Gibt die Aktion, die durch die rechte Maustaste ausgeloest wird, zurueck
	 */
	/*
	Action::ActionType getRightAction()
	{
		return m_right_action;
	}
	*/

	/**
	 * \fn map<int,DropItem>* getDropItems()
	 * \brief Gibt die Liste aller fuer den Spieler sichtbaren Gegenstaende am Boden zurueck
	 */
	/*
	map<int,DropItem>* getDropItems()
	{
		return m_drop_items;
	}
*/
	
	/**
	/* \fn void setServer(bool server)
	 * \brief Stellt ein ob das Spiel der Server ist
	 */
	void setServer(bool server)
	{
		m_server = server;
	}

	/**
	 * \fn std:.string getAbilityDescription(Action::ActionType ability)
	 * \brief Gibt die Beschreibung der Faehigkeit, ueber der Mauszeiger ist, aus
	 */
	std::string getAbilityDescription(Action::ActionType ability);

	/**
	 * \fn KeyCode getMappedKey(ShortkeyDestination sd)
	 * \brief Gibt die Taste zurueck, die das angegebene Ereignis ausloest
	 */
	KeyCode getMappedKey(ShortkeyDestination sd);

	/**
	 * \fn void installShortkey(KeyCode key,ShortkeyDestination dest)
	 * \brief Bindet eine Taste an ein Ereignis
	 * \param key Taste
	 * \param dest Ereignis, das auf die Taste hin ausgeloest werden soll
	 */
	void installShortkey(KeyCode key,ShortkeyDestination dest);

	/**
	 * \fn void lock()
	 * \brief Sperrt das Dokument
	 */
	void lock();

	/**
	 * \fn void unlock()
	 * \brief Entsperrt das Dokument
	 */
	void unlock();



	/**
	 * \fn void onButtonSendMessageClicked ( )
	 * \brief Button zum Absenden einer Nachricht wurde angeklickt
	 */
	void onButtonSendMessageClicked ( );




	/**
	 * \fn void onButtonSaveExitClicked ( )
	 * \brief Button zum Speichern und Beenden des Spiels wurde angeklickt
	 */
	void onButtonSaveExitClicked ( );


	/**
	 * \fn void onButtonInventoryClicked()
	 * \brief Behandelt Klick auf Inventar Button
	 */
	void onButtonInventoryClicked();

	/**
	 * \fn void onButtonCharInfoClicked()
	 * \brief Behandelt Klick auf CharakterInfo Button
	 */
	void onButtonCharInfoClicked();


	/**
	 * \fn void onButtonSkillTreeClicked()
	 * \brief Behandelt Klick auf Skilltree Button
	 */
	void onButtonSkilltreeClicked();

	/**
	 * \fn void onButtonOpenChatClicked()
	 * \brief Behandelt Klick auf Chat oeffnen Button
	 */
	void onButtonOpenChatClicked();

	/**
	 * \fn void onSwapEquip()
	 * \brief Wechselt die Ausruestung
	 */
	void onSwapEquip();

	/**
	 * \fn void onLeftMouseButtonClick (float x, float y)
	 * \brief Behandelt die Reaktion auf einen Linksklick ins Spielfeld
	 * \param x X-Koordinate
	 * \param y Y-Koordinate
	 */
	void onLeftMouseButtonClick(float x, float y);


	/**
	 * \fn void onRightMouseButtonClick (float x, float y)
	 * \brief Behandelt die Reaktion auf einen Rechtsklick ins Spielfeld
	 * \param x X-Koordinate
	 * \param y Y-Koordinate
	 */
	void onRightMouseButtonClick(float x, float y);


	/**
	 * \fn void onItemLeftClick(short pos)
	 * \brief Behandelt Linksklick auf ein Item
	 */
	void onItemLeftClick(short pos);

	/**
	 * \fn void onItemRightClick(short pos)
	 * \brief Behandelt Rechtsklick auf ein Item
	 */
	void onItemRightClick(short pos);

	/**
	 * \fn void requestItemDetailedInfo( short pos)
	 * \brief Fordert vom Server weitere Informationen zu einem Item an
	 */
	//void requestItemDetailedInfo( short pos);

	/**
	 * \fn void requestAbilityDamage(Action::ActionType abl)
	 * \brief Fordert vom Server den Schaden einer Faehigkeit an
	 */
	//void requestAbilityDamage(Action::ActionType abl);

	/**
	 * \fn void increaseAttribute(CreatureBaseAttr::Attribute attr)
	 * \brief Erhoeht das angegebene Attribut
	 */
	void increaseAttribute(CreatureBaseAttr::Attribute attr);

	/**
	 * \fn void learnAbility(Action::ActionType act)
	 * \brief Lernt die angegebene Faehigkeit
	 */
	void learnAbility(Action::ActionType act);

	/**
	 * \fn void dropCursorItem()
	 * \brief Laesst das Item fallen, das aktuell am Cursor haengt
	 */
	void dropCursorItem();

	/**
	 * \fn bool onKeyPress(KeyCode key)
	 * \brief Behandelt einen Tastendruck
	 * \param key Keycode der Taste
	 */
	bool onKeyPress(KeyCode key);

	/**
	 * \fn bool onKeyRelease(KeyCode key)
	 * \brief Behandelt loslassen einer Taste
	 */
	bool onKeyRelease(KeyCode key);

	void onButtonPartyApply(int pnr);

	void onButtonPartyAccept(int cnr);

	void emitDebugSignal(int i=0);

	/**
	 * \fn void update( float time=0)
	 * \brief Aktualisiert den Inhalt der GUI, holt alle Daten neu vom Netzwerk
	 */
	void update(float time=0);

	/**
	 * \fn void startGame(bool server)
	 * \brief Baut Verbindung zum Server auf
	 */
	void startGame(bool server);
	
	/**
	 * \fn Player* getLocalPlayer()
	 * \brief Gibt den Spieler aus, der lokal an dem Rechner ist
	 */
	Player* getLocalPlayer()
	{
		if (m_world ==0)
			return 0;
		
		return static_cast<Player*>(m_world->getLocalPlayer());
	}
	
	/**
	 * \fn World* getWorld()
	 * \brief gibt Zeiger auf die Spielwelt aus
	 */
	World* getWorld()
	{
		return m_world;
	}


	// Private stuff
	private:
	//Fields
		/**
	 * \fn void sendCommand(ClientCommand* command)
	 * \param command Kommando, das gesendet werden soll
	 * \return Status des Netzwerkes nach der Sendeanfrage
	 * \brief sendet ein Kommando an den Server
		 **/
		void sendCommand(ClientCommand* comm);

		/**
		 * \fn void loadSavegame()
		 * \brief laedt ein Savegame
		 */
		void loadSavegame();
		
		/**
		 * \fn void updateContent(float time)
		 * \brief Aktualisiert den Inhalt des Dokuments
		 */
		void updateContent(float time);

		/**
		 * \fn void handleDatapkg(CharConv* cv)
		 * \param datap Zeiger auf das Datenpaket
		 * \param headerp Zeiger auf den Header des Datenpaketes
		 * \brief Behandlung von Datenpaketen
	 	*/
		//void handleDataPkg(CharConv* cv,ServerHeader* headerp);


		/**
		 * \fn void handleSavegame(CharConv* cv)
		 * \brief Behandlung von Savegamepaketen
		 * \param datap Zeiger auf die Daten
	 	*/
		//void handleSavegame(CharConv* cv);

		/**
		 * \fn void handleDetailedItem(CharConv* cv)
		 * \brief Behandlung von Informationen zu einem Item
		 * \param datap Zeiger auf die Daten
	 	*/
		//void handleDetailedItem(CharConv* cv);

		/**
		 * \fn void handleRegionData(CharConv* cv)
		 * \brief Behandlung der Daten einer Region
	 	 */
		//void handleRegionData(CharConv* cv);

		/**
		 * \fn void handleDatapkg(CharConv* cv)
		 * \param datap Zeiger auf das Datenpaket
		 * \param headerp Zeiger auf den Header des Datenpaketes
		 * \brief Behandlung von informationen zum Schaden einer Aktion
		 */
		//void handleAbilityDamage(CharConv* cv, ServerHeader* headerp);

	/**
	 * \fn int getObjectAt(float x,float y)
	 * \brief Ermittelt die ID des Objektes an den angegebenen Koordinaten
	 * \return ID
	 */
	int getObjectAt(float x,float y);

	
	

	/**
	 * \var GUIState m_gui_state
	 * \brief Enthaelt alle Informationen zum Zustand der GUI
	 */
		GUIState m_gui_state;


	/**
	 * \var m_data_locks
	 * \brief Gibt die Anzahl der Locks auf den Daten des Dokuments an. Die Daten koennen nur veraendert und von aussen gelesen werden, wenn die Anzahl gleich 0 ist
	 */
		int m_data_locks;

	/**
	 * \var char* m_savegame
	 * \brief Aktueller Spielstand in Binaerformat
	 */
		char* m_savegame;


	/**
	 * \var int m_modified
	 * \brief Bitmaske welche angibt welche Teile des Dokuments veraendert wurden
	 */
		int m_modified;

	/**
	 * \var string m_save_file
	 * \brief Dateiname des Savefiles
	 */
		string m_save_file;

	/**
	 * \var State m_state
	 * \brief Status des Dokuments
	 */
		State m_state;

	/**
	 * \var map<unsigned int,ShortkeyDestination> m_shortkey_map
	 * \brief Bildet Taste auf Ereignis, das per Shortkey ausgeloest werden kann ab
	 */
		map<KeyCode,ShortkeyDestination> m_shortkey_map;

	/**
	 * \var set<KeyCode> m_special_keys
	 * \brief Menge der Tasten, die eine besondere Bedeutung haben und die deswegen nicht frei zugewiesen werden koennen
	 */
		set<KeyCode> m_special_keys;



	char m_server_ip[16];
		
	/**
	 * \var World* m_world
	 * \brief Welt die von dem Spiel simuliert wird
	 */
	World* m_world;
	
	/**
	 * \var bool m_server
	 * \brief true, wenn der aktuelle Rechner der Server ist
	 */
	bool m_server;
	
	/**
	 * \var Timer m_timer
	 * \brief Timer fuer die Updatezyklen der Spielwelt
	 */
	Timer m_timer;
	
	/**
	 * \var m_shutdown_timer
	 * \brief Timer fuer Shutdown
	 */
	float m_shutdown_timer;

};


#endif //DOCUMENT_H



