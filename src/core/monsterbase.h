#ifndef MONSTER_BASE_H
#define MONSTER_BASE_H

#include "dropslot.h"
#include "worldobject.h"
#include "creaturestruct.h"

/**
 * \struct MonsterBasicData
 * \brief enthaelt alle Daten die zur Erzeugung eines Monsters noetig sind
 */
struct MonsterBasicData
{
	
	/**
	 * \var CreatureBaseAttr m_base_attr
	 * \brief Basisattribute
	 */
	CreatureBaseAttr m_base_attr;
	
	/**
	 * \var WorldObject::TypeInfo m_type_info
	 * \brief Informationen zum Typ
	 */
	WorldObject::TypeInfo m_type_info;
	
	/**
	 * \var WorldObject::Category m_category
	 * \brief Kategorie des Objektes
	 */
	WorldObject::Category m_category;

	/**
	 * \var WorldObject::Fraction m_fraction
	 * \brief Fraktion des Objektes
	 */
	WorldObject::Fraction m_fraction;
	
	/**
	 * \var DropSlot m_drop_slots[4]
	 * \brief Beschreibung der Items die das Monster droppen kann, wenn es stirbt
	 */
	DropSlot m_drop_slots[4];
	
	/**
	 * \var float m_ai_sight_range
	 * \brief Sichtweite der AI
	 */
	float m_ai_sight_range;
	
	/**
	 * \var m_radius
	 * \brief Radius des Grundkreises
	 */
	float m_radius;
	
	/**
	 * \var short m_layer
	 * \brief Ebene in der das Monster sich befindet
	 */
	short m_layer;
	
	/**
	 * \var std::string m_name
	 * \brief Name des Monsters
	 */
	std::string m_name;
};

/**
 * Name fuer eine Gruppe von Monstern die erzeugt wird
 **/ 
typedef std::string MonsterGroupName;

/**
 * \struct MonsterGroup
 * \brief Struktur fuer eine Gruppe von Monstern
 */
class MonsterGroup
{
	/**
	 * \struct SubGroup
	 * \brief Untergruppe der Monster bei denen alle Monster den selben Typ haben
	 */
	struct SubGroup
	{
		/**
		 * \var WorldObject::TypeInfo::ObjectSubtype m_subtype
		 * \brief Subtype der erzeugten Monster
		 */
		WorldObject::TypeInfo::ObjectSubtype m_subtype;
		
		/**
		 * \var float m_prob
		 * \brief Wahrscheinlichkeit mit der ein Monster erzeugt wird
		 */
		float m_prob;
		
		/**
		 * \var int m_number
		 * \brief Anzahl der Monster die maximal erzeugt werden. Jedes Monster wird gemaess der Wahrscheinlichkeit prob erzeugt. Im Erwartungswert also prob * number Monster
	 	 */
		int m_number;
	};
	
	
	
	public:
		
		/**
		 * \fn void addMonsterGroup(WorldObject::TypeInfo::ObjectSubtype subtype, int number =1, float prob = 1.0)
		 * \brief Fuegt der Monstergruppe eine Subgruppe hinzu
		 * \param subtype Typ der Monster
		 * \param number Anzahl der Monster
		 * \param prob Wahrscheinlichkeit mit der ein Monster erzeugt wird
		 */
		void addMonsters(WorldObject::TypeInfo::ObjectSubtype subtype, int number =1, float prob = 1.0)
		{
			SubGroup sg;
			sg.m_subtype = subtype;
			sg.m_number = number;
			sg.m_prob = prob;
			m_monsters.push_back(sg);
		}
		
		/**
		 * \fn std::list<SubGroup> m_monsters
		 * \brief die Monster die zu der Gruppe gehoeren
		 */
		std::list<SubGroup> m_monsters;
};

#endif

