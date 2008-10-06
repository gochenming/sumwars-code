/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf, Daniel Erler

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
 * \file item.h
 * \defgroup Item \ Gegenstand
 * \brief Beschreibt den Aufbau von Gegenst&auml;nden, die ein Spieler haben kann.
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Beinhaltet die Klasse Item
 */
#ifndef ITEM_H
#define ITEM_H


#include <string>
#include <cstring>
#ifdef WIN32

#else
    #include <libintl.h>
#endif
#include <locale.h>
#include "charconv.h"
#include "debug.h"

#include "damage.h"
#include "creaturestruct.h"

#include <math.h>



/**
 * \struct WeaponAttr
 * \brief Eigenschaften und Attribute einer Waffe
 */
struct WeaponAttr
{
	/**
	 * \var m_damage
	 * \brief Schaden, den die Waffe austeilt
	 */
	Damage m_damage;

	/**
	 * \var m_attack_range
	 * \brief Gibt die Reichweite der Waffe an. Fuer Schusswaffen auf  ITEM_BOW_RANGE setzen
	 */
	float m_attack_range;

	/**
	 * \var m_two_handed
	 * \brief Ist auf true gesetzt, wenn die Waffe beidhaendig benutzt wird.
	 */
	bool m_two_handed;

	/**
	 * \var m_dattack_speed
	 * \brief Gibt die Aenderung der Angriffsgeschwindigkeit bei Benutzen dieser Waffe an
	 */
	short m_dattack_speed;

	/**
	 * \fn void init()
	 * \brief initialisiert die Datenfelder
	 */
	void init()
	{
		m_attack_range=1;
		m_dattack_speed=0;
		m_two_handed=false;
		m_damage.init();
	}

	/**
	 * \fn WeaponAttr()
	 * \brief Konstruktor
	 */
	WeaponAttr()
	{
		init();
	}
};

struct ItemBasicData;


/**
 * \struct Item
 * \brief Dieses Objekt ist ein Gegenstand und hat als solches Eigenschaften, wie Preis, Anzahl oder Gewicht.
 */
struct Item {
	/**
	 * \enum Size
	 * \brief TODO
	 */
	enum Size
	{
		GOLD = 3,
		SMALL = 2,
		MEDIUM = 1,
		BIG = 0,
	};


	/**
	 * \enum Type
	 * \brief TODO
	 */
	enum Type
	{
		NOITEM = 0,
		ARMOR  = 1,
		HELMET = 2,
		GLOVES = 6,
		WEAPON = 8,
		SHIELD = 5,
		POTION = 4,
		RING = 3,
		AMULET= 7,
 		GOLD_TYPE = 10
	};

	/**
	 * Subtype
	 */
	typedef std::string Subtype;

	/**
	 * \enum CharRequirement
	 * \brief Aufzaehlung der Charakterklassen als Vorraussetzung fuer das Verwendung eines Gegenstandes
	 */
	enum CharRequirement
	{
		REQ_WARRIOR=1,
		REQ_MAGE=2,
		REQ_ARCHER=4,
		REQ_PRIEST=8,
		REQ_NONE=15,
	};
	
	/**
	 * \fn enum InfoFlags
	 * \brief Zaehlt auf, welche Informationen zu einem Item vorliegen koennen
	 */
	enum InfoFlags
	{
		NOINFO=0,
		USEUP_INFO=1,
		EQUIP_INFO=2,
		WEAPON_INFO=4,
	};

	/**
	 * \fn Item()
	 * \brief Konstruktor
	 */
	Item();

	
	/**
	 * \fn Item(ItemBasicData& data)
	 * \brief erzeugt ein neuen Gegenstand mit den vorgegebenen Daten
	 * \param data Daten auf deren Basis der Gegenstand erzeugt wird
	 */
	Item(ItemBasicData& data);
	
	/**
	 * \fn virtual ~Item()
	 * \brief Destruktor
	 */
	virtual ~Item();
	

	/**
	 * \fn String getName()
	 * \brief Gibt den lesbaren Name des Objektes aus
	 */
	virtual std::string getName();

	/**
	 * \fn String getString()
	 * \brief Gibt den Name des Objektes aus
	 */
	virtual std::string getString();

	/**
	 * \fn virtual void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param cv Ausgabepuffer
	 */
	virtual void toString(CharConv* cv);


	/**
	 * \fn virtual void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param cv Eingabepuffer
	 */
	virtual void fromString(CharConv* cv);
	
	
	/**
	 * \fn void toSavegame(CharConv* cv)
	 * \brief Schreibt das Item inklusive allen Extradaten in den Puffer
	 * \param cv Ausgabepuffer
	 */
	void toStringComplete(CharConv* cv);


	/**
	 * \fn void fromStringComplete(CharConv* cv)
	 * \brief Laedt das Item inklusive allen Extradaten aus dem Puffer
	 * \param cv Eingabepuffer
	 */
	void fromStringComplete(CharConv* cv);

	/**
	 * \fn std::string getDescription()
	 * \brief Gibt eine Beschreibung in Textform aus
	 */
	std::string getDescription();

	/**
	 * \fn calcPrice()
	 * \brief Berechnet den Wert des Items
	 */
	void calcPrice();
	

	/**
	* \var m_subtype;
	* \brief Gibt die Unterart eines Gegenstands an ((Langschwert, Dolch usw)
	*/
	Subtype m_subtype;

	/**
	 * \var int m_id
	 * \brief Id
	 */
	int m_id;

	/**
	 * \var m_type;
	 * \brief Gibt Typ des Gegenstands an (Waffe, Helm usw...)
	 */
	Type m_type;

	/**
	* \var m_size
	\brief Gibt die Groesse des Gegenstandes (klein,mittel, gross) an
	*/
	Size m_size;


	/**
	 * \var m_price;
	 * \brief Gibt Preis des Gegenstands an
	 */
	int m_price;

	/**
	 * \var m_useup_effect
	 * \brief Gibt die Veraenderung der dynamischen Attribute an, die beim verbrauchen des Gegenstandes eintritt. Wenn der Zeiger auf NULL gesetzt ist, kann der Gegenstand nicht verbraucht werden.
	 */
	CreatureDynAttrMod* m_useup_effect;

	/**
	 * \var m_equip_effect
	 * \brief Gibt die Veraenderung der Basisattribute an, die beim verwenden des Gegenstandes aus Ausruestungsgegenstand eintritt. Wenn der Zeiger auf NULL gesetzt ist, kann der Gegenstand nicht auf diese Weise verwendet werden.
	 */
	CreatureBaseAttrMod* m_equip_effect;

	/**
	 * \var m_weapon_attr
	 * \brief Gibt den Attribute des Gegenstandes als Waffe an. Sollte bei allen Gegenstaenden, die nicht Waffen sind auf NULL gesetzt sein
	 */
	WeaponAttr* m_weapon_attr;

	/**
	 * \var char m_level_req
	 * \brief Erforderliches Level um das Item zu verwenden
	 */
	char m_level_req;

	/**
	 * \var char m_char_req
	 * \brief Anforderungen an die Charakterklasse
	 */
	char m_char_req;

	/**
	 * \var float m_magic_power
	 * \brief Gibt Verzauberungsstärke an
	 */
	float m_magic_power;

};


/**
 * \struct ItemBasicData
 *  \brief Informationen die noetig sind, um ein bestimmtes Item zu erzeugen
 */
struct ItemBasicData
{
    /**
	 * \var m_useup_effect
	 * \brief Gibt die Veraenderung der dynamischen Attribute an, die beim verbrauchen des Gegenstandes eintritt. Wenn der Zeiger auf NULL gesetzt ist, kann der Gegenstand nicht verbraucht werden.
	 */
	CreatureDynAttrMod* m_useup_effect;

	/**
	 * \var m_equip_effect
	 * \brief Gibt die Veraenderung der Basisattribute an, die beim verwenden des Gegenstandes aus Ausruestungsgegenstand eintritt. Wenn der Zeiger auf NULL gesetzt ist, kann der Gegenstand nicht auf diese Weise verwendet werden.
	 */
	CreatureBaseAttrMod* m_equip_effect;

	/**
	 * \var m_weapon_attr
	 * \brief Gibt den Attribute des Gegenstandes als Waffe an. Sollte bei allen Gegenstaenden, die nicht Waffen sind auf NULL gesetzt sein
	 */
	WeaponAttr* m_weapon_attr;

	/**
	 * \var char m_level_req
	 * \brief Erforderliches Level um das Item zu verwenden
	 */
	char m_level_req;

	/**
	 * \var char m_char_req
	 * \brief Anforderungen an die Charakterklasse
	 */
	char m_char_req;

	/**
	 * \var m_subtype;
	 * \brief Gibt die Unterart eines Gegenstands an ((Langschwert, Dolch usw)
	 */
	Item::Subtype m_subtype;


	/**
	 * \var m_type;
	 * \brief Gibt Typ des Gegenstands an (Waffe, Helm usw...)
	 */
	Item::Type m_type;

	/**
	 * \var m_size
	\brief Gibt die Groesse des Gegenstandes (klein,mittel, gross) an
	 */
	Item::Size m_size;
	
	/**
	 * \var int m_price
	 * \brief Wert des Gegenstandes
	 */
	int m_price;

	/**
	 * \var float m_modchance[31]
	 * \brief relative Chance auf die verschiedenen Modifikationen
	 */
	float m_modchance[31];

	/**
	 * \var float m_min_enchant
	 * \brief minimale Starke einer Verzauberung des Items
	 */
	float m_min_enchant;

	/**
	 * \var float m_max_enchant
	 * \brief maximale Starke einer Verzauberung des Items
	 */
	float m_max_enchant;

	/**
	 * \fn ItemBasicData()
	 * \brief Konstruktor
	 */
	ItemBasicData();

};



#endif //ITEM_H

