/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf, Alexander Boehm

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

/**
 * \file player.h
 * \defgroup Player \ Menschlicher Nutzer
 * \ingroup Player
 * \brief Steuerung eines nutzerkontrollierten Spielers
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/30
 * \note Beinhaltet die Klasse Player
 */
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "creature.h"
//#include "trade.h"
#include "itemlist.h"
#include "dropitem.h"
#include "networkstruct.h"
#include "servernetwork.h"
#include "serveritem.h"

/**
 * \class Player
 * \brief Klasse fuer Spielercharactere
 */
class Player : public Creature {
//Public stuff
public:
	//Fields
	//Constructors
	/**
	 * \fn Player(World* world, unsigned int id)
	 * \brief Konstruktor
	 * \param world Welt
	 * \param id Objekt-ID
	 *
	 * Legt ein neues Player Objekt an.
	 */
	Player(World* world, int id);
	
	
	
	/**
	 * \fn ~Player()
	 * \brief Destruktor
	 *
	 * Gibt den Allokierten Speicher wieder frei
	 */
	~Player();
	
	
	//Accessor Methods
	/**
	 * \fn int getNetworkSlot()
	 * \brief Gibt die Nummer des Netzwerkslots zum Senden von Datenpaketen zur&uuml;ck
	 * \return int Nummer des Netzwerkslots
	 */
	int getNetworkSlot()
	{
		return m_network_slot;
	}
	
	
	/**
	 * \fn void setNetworkSlot(int value)
	 * \brief Setzt die Nummer des Netzwerkslots zum Senden von Datenpaketen
	 * \param value Nummer des Netzwerkslots
	 */
	void setNetworkSlot(int value)
	{
		m_network_slot = value;
	}
	
	
	/**
	 * fn Equipement* getEquipement()
	 * \brief Gibt einen Zeiger auf die Ausruestung des Spielers zurück
	 * \return Zeiger auf die Ausruestung
	 */
	Equipement* getEquipement()
	{
		return m_equipement;
	}
	
	//Operations
	/**
	 * \fn virtual bool init()
	 * \brief Initialisierung des Nutzergesteuerten Spielers
	 * \return bool Gibt an ob die Initialisierung erfolgreich war
	 */
	virtual bool init();
	
	/**
	 * \fn virtual bool destroy()
	 * \brief Zerstoert das Objekt
	 * \return bool der angibt, ob die Zerst&ouml;rung erfolgreich war
	 */
	virtual  bool  destroy();
	
	/**
	 * \fn virtual bool update (float time)
	 * \brief Updatefunktion des Players
	 * \param time Verstrichene Zeit
	 * \return bool, Erfolgreiches Update?
	 *
	 * Die Updatefunktion des Players wird in regelm&auml;ssigen Abst&auml;nden von der World aufgerufen und ben&ouml;tigt die Angabe, wie viel Zeit f&uuml;r das Update zur Verf&uuml;gung gestellt wird. Der R&uuml;ckgabewert gibt an ob die Funktion erfolgreich ausgef&uuml;hrt wurde. Da die Funktion hier virtuell definiert wird, wird sie erst in den abgeleiteten Klassen implementiert.
	 */
	virtual bool update(float time);
	
	/**
	 * \fn virtual void recalcDamage();
	 * \brief Berechnet den Schaden, der in internen Damage Objekten angezeigt wird neu
	 */
	virtual void recalcDamage();
	
	/**
	 * \fn virtual void calcDamage(ActionType type,Damage& dmg)
	 * \brief Berechnet den Basisschaden einer Aktion
	 */
	virtual void calcBaseDamage(Action::ActionType act,Damage& dmg);
	
	/**
	 * \fn virtual void calcBaseAttrMod()
	 * \brief Die modifizierten Basisattribute werden neu berechnet. Verwendet die Basisattribute, verwendete Items, sowie temporaere Modifikationen.
	 */
	virtual void calcBaseAttrMod();
	
	/**
	 * \fn ServerItem* getWeapon()
	 * \brief Gibt die aktuell genutzte Waffe zurueck
	 */
	ServerItem* getWeapon();
	
	/**
	 * \fn ServerItem* getShield()
	 * \brief Gibt das aktuell genutzte Schild zurueck
	 */
	ServerItem* getShield();
	
	
	/**
	 * \fn bool onClientCommand( ClientCommand* command)
	 * \brief  Reaktion auf eine Anweisung des Nutzers
	 * \param command Kommando des Nutzers
	 * \return bool gibt an, ob die Behandlung fehlerfrei verlaufen ist
	 */
	bool onClientCommand( ClientCommand* command);
	
	/**
	 * \fn bool onGamefieldClick(ClientCommand* command)
	 * \brief Reaktion auf einen Klick ins das Spielfeld
	 * \param command Kommando des Nutzers
	 * \return gibt an, ob die Behandlung fehlerfrei verlaufen ist
	 */
	bool onGamefieldClick(ClientCommand* command);
	
	/**
	 * \fn bool onItemClick(ClientCommand* command)
	 * \brief Reaktion auf einen Klick auf ein Item
	 * \param command Kommando des Nutzers
	 * \return gibt an, ob die Behandlung fehlerfrei verlaufen ist
	 */
	bool onItemClick(ClientCommand* command);
	
	
	/**
	 * \fn void sendSavegame()
	 * \brief Sendet ein Savegame an den Client
	 */
	void sendSavegame();
	
	/**
	 * \fn void sendGameDate()
	 * \brief Sendet ein Datenpaket an den Client
	 */
	void sendGameData();
	
	/**
	 * \fn void sendDetailedItem(short pos)
	 * \brief Sendet detaillierte Informationen ueber ein Item
	 * \param pos Position des Items
	 */
	void sendDetailedItem(short pos);
	
	
	/**
	 * \fn sendAbilityDamage((Action::ActionType act)
	 * \brief Sendet den Schaden einer Aktion
	 * \param act Aktionstyp
	 */
	void sendAbilityDamage(Action::ActionType act);
	
	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param buf Ausgabepuffer
	 * \return Zeiger hinter den beschriebenen Datenbereich
		 */
	virtual void toString(CharConv* cv);
	
	/**
	 * \fn void fromString(char* buf)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param buf Objekt als String
	 * \return Zeiger hinter den gelesenen Datenbereich
	 */
	virtual void fromString(CharConv* cv);
	
	/**
	 * \fn void toString(v oi buf)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param buf Ausgabepuffer
	 * \return Zeiger hinter den beschriebenen Datenbereich
	 */
	virtual void toStringComplete(CharConv* cv);
	
	/**
	 * \fn void writeEquipement(CharConv* cv)
	 * \brief Schreibt des Inventar in den Puffer
	 * \param buf Ausgabepuffer
	 * \return Zeiger hinter den beschriebenen Datenbereich
	 */
	void writeEquipement(CharConv* cv);
	
	/**
	 * \fn void loadEquipement(CharConv* cv)
	 * \brief Laedt das Inventar aus dem Puffer
	 * \param buf Ausgabepuffer
	 * \return Zeiger hinter den beschriebenen Datenbereich
	 */
	void loadEquipement(CharConv* cv);
	
	/**
	 * \fn char* toSavegame(char* buf)
	 * \brief Erzeugt ein binaeres Savegame und schreibt es in den Puffer
	 * \param buf Ausgabepuffer
	 * \return Zeiger hinter den beschriebenen Datenbereich
	 */	 
	void toSavegame(CharConv* cv);
	
	
	/**
	 * \fn void fromSavegame(char* buf)
	 * \brief Laedt die Daten aus einem Savegame
	 * \param buf Zeiger auf Savegame
	 * \return Zeiger hinter den Datenbereich
	 */
	void fromSavegame(CharConv* cv);
	
	/**
	 * \fn void increaseAttribute(CreatureBaseAttr::Attribute attr)
	 * \brief Erhoeht das angegebene Attribut
	 */
	void increaseAttribute(CreatureBaseAttr::Attribute attr);

	/**
	 * \fn virtual Action::ActionEquip getActionEquip()
	 * \brief Gibt aus, ob die Aktion einhaendig oder zweihaendig ausgefuehrt wird
	 */
	virtual Action::ActionEquip getActionEquip();
	
	/**
	 * \fn Action::ActionType getRightAction()
	 */
	Action::ActionType getRightAction()
	{
		return m_right_action;
	}
	
	/**
	 * \fn Action::ActionType getLeftAction()
	 */
	Action::ActionType getLeftAction()
	{
		return m_left_action;
	}
	
	/**
	 * \fn short getAttributePoints()
	 */
	short getAttributePoints()
	{
		return m_attribute_points;
	}
	
	/**
	 * \fn short getSkillPoints()
	 */
	short getSkillPoints()
	{
		return m_skill_points;
	}
	
	/**
	 * \fn Damage& getBaseDamage()
	 */
	Damage& getBaseDamage()
	{
		return m_base_damage;
	}
	
	/**
	 * \fn Damage& getLeftDamage()
	 */
	Damage& getLeftDamage()
	{
		return m_left_damage;
	}
	
	/**
	 * \fn Damage& getRightDamage()
	 */
	Damage& getRightDamage()
	{
		return m_right_damage;
	}
	

//Protected stuff
protected:
	//Fields
	//Constructors
	//Accessor Methods
	//Operations

	/**
 * \fn virtual void gainLevel()
 * \brief Wird aufgerufen, wenn das Lebewesen ein Levelup erhält. Passt Erfahrung und alle Basisattribute an.
	 */
	virtual void gainLevel();
	
//Private stuff
private:
	//Fields
	/**
	 * \var m_network_slot
	 * \brief Nummer des Netzwerkslots zum Senden der Datenpakete
	 */
	int m_network_slot;
	
	/**
	 * \var m_package_number
	 * \brief Gibt Anzahl der gesendeten Datenpakete an
	 */
	int m_package_number;
	
	/**
	 * \var float m_save_timer
	 * \brief Timer bei dessen Ablauf ein Savegame gesendet wird
	 */
	float m_save_timer;
	
	
	
	
	/**
	 * \var m_equipement
	 * \brief Enthaelt die Ausruestung des Spielers
	 */
	Equipement* m_equipement;
	
protected:
	
	
	/**
 * \fn virtual void performActionCritPart(float dtime, float goalx, float goaly, Creature* cgoal)
 * \brief Fuehrt den entscheidenden Part einer Action (Schaden austeilen, Status veraendern usw aus
 * \param goalx x-Koordinate des Zieles
 * \param goaly x-Koordinate des Zieles
 * \param goal Zeiger auf der Zielobjekt, ist NULL wenn kein Zielobjekt existiert
	 */
	virtual void performActionCritPart(float goalx, float goaly, ServerWObject* goal);
	
	/**
	 * \var std::string m_name
	 * \brief Name des Spielers
	 */
	std::string m_name;
	
	
	/**
	 * \var short m_attribute_points
	 * \brief Anzahl noch zu verteilender Attributspunkte
	 */
	short m_attribute_points;
	
	/**
	 * \var short m_skill_points
	 * \brief Anzahl noch zu verteilender Skillpunkte
	 */
	short m_skill_points;
	
	/**
	 * \var bool m_secondary_equip
	 * \brief Zeigt an, ob die zweite Ausruestung benutzt wird
	 */
	bool m_secondary_equip;
	
	/**
	 * \var Damage m_base_damage
	 * \brief Schaden der mit der Basisattacke ausgeteilt wird
	 */
	Damage m_base_damage;

	/**
	 * \var Damage m_left_damage
	 * \brief Schaden der mit der Attacke auf der linken Maustaste ausgeteilt wird
	 */
	Damage m_left_damage;

	/**
	 * \var Damage m_right_damage
	 * \brief Schaden der mit der Attacke auf der rechten Maustaste ausgeteilt wird
	 */
	Damage m_right_damage;
	
	/**
	 * \var Action::ActionType m_base_action
	 * \brief Basisaktion des Spielers
	 */
	Action::ActionType m_base_action;
	
	/**
	 * \var Action::ActionType m_left_action
	 * \brief Aktion die auf mit linke Maustaste ausgeloest wird
	 */
	Action::ActionType m_left_action;
	
	/**
	 * \var Action::ActionType m_right_action
	 * \brief Aktion die auf mit rechten Maustaste ausgeloest wird
	 */
	Action::ActionType m_right_action;
	
	//Constructors
	//Accessor Methods
	//Operations
};
#endif //PLAYER_H
