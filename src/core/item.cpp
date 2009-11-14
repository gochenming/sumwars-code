#include "item.h"
#include "itemfactory.h"

void WeaponAttr::operator=(WeaponAttr& other)
{
	m_weapon_type = other.m_weapon_type;
	m_damage = other.m_damage;
	m_attack_range = other.m_attack_range;
	m_two_handed = other.m_two_handed;
	m_dattack_speed = other.m_dattack_speed;
}

int WeaponAttr::getValue(std::string valname)
{
	if (valname =="weapon_type")
	{
		lua_pushstring(EventSystem::getLuaState() , m_weapon_type.c_str() );
		return 1;
	}
	else if (valname == "attack_range")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_attack_range );
		return 1;
	}
	else if (valname == "two_handed")
	{
		lua_pushboolean(EventSystem::getLuaState() , m_two_handed );
		return 1;
	}
	else if (valname == "attack_speed")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_dattack_speed );
		return 1;
	}
	else
	{
		return m_damage.getValue(valname);
	}
	return 0;
}

bool WeaponAttr::setValue(std::string valname)
{
	if (valname =="weapon_type")
	{
		m_weapon_type = lua_tostring(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname == "attack_range")
	{
		m_attack_range = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname == "two_handed")
	{
		m_two_handed = lua_toboolean(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname == "attack_speed")
	{
		m_dattack_speed =(short) lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else
	{
		return m_damage.setValue(valname);
	}
	return false;
}

ItemBasicData::ItemBasicData()
{
	m_useup_effect=0;
	m_equip_effect=0;
	m_weapon_attr=0;
	m_level_req = 0;
	m_char_req = "all";

	for (int i=0;i<31;i++)
	{
		m_modchance[i] =0;
	}
	m_min_enchant =0;
	m_max_enchant =0;

}

ItemBasicData::~ItemBasicData()
{
	if (m_useup_effect!=0)
		delete m_useup_effect;

	if (m_equip_effect!=0)
		delete m_equip_effect;

	if (m_weapon_attr!=0)
		delete m_weapon_attr;

}


Item::Item(int id)
{
	if (id ==0)
	{
		if (World::getWorld() !=0)
		{
			m_id = World::getWorld()->getValidId();
		}
		else
		{
			m_id =0;
		}
	}
	else
	{
		m_id = id;
	}
	m_useup_effect=0;
	m_equip_effect=0;
	m_weapon_attr=0;
	m_level_req = 0;
	m_magic_power =0;
	m_rarity = NORMAL;
	m_char_req = "all";
	m_size = SMALL;
}

Item::Item(ItemBasicData& data, int id)
{
	m_magic_power =0;
	m_rarity = NORMAL;
	
	if (id ==0)
	{
		if (World::getWorld() !=0)
		{
			m_id = World::getWorld()->getValidId();
		}
		else
		{
			m_id =0;
		}
	}
	else
	{
		m_id = id;
	}
	
	m_type = data.m_type;
	m_subtype = data.m_subtype;
	m_size = data.m_size;
	m_price = data.m_price;

	if (data.m_useup_effect)
	{
		DEBUG5("copy useup effect");
		m_useup_effect = new CreatureDynAttrMod;
		*m_useup_effect = *(data.m_useup_effect);
	}
	else
	{
		m_useup_effect =0;
	}

	if (data.m_equip_effect)
	{
		DEBUG5("copy equip effect");
		m_equip_effect = new CreatureBaseAttrMod;
		*m_equip_effect = *(data.m_equip_effect);
	}
	else
	{
		m_equip_effect =0;
	}

	if (data.m_weapon_attr)
	{
		DEBUG5("copy weapon attr");

		m_weapon_attr = new WeaponAttr;
		*m_weapon_attr = *(data.m_weapon_attr);
		m_weapon_attr->m_damage = data.m_weapon_attr->m_damage;

	}
	else
	{
		m_weapon_attr =0;
	}

	m_level_req = data.m_level_req;
	m_char_req = data.m_char_req;
}

Item::~Item()
{
	if (m_useup_effect) delete m_useup_effect;
	if (m_equip_effect) delete m_equip_effect;
	if (m_weapon_attr) delete  m_weapon_attr;
}


std::string Item::getName()
{
	std::ostringstream ret;

	if (m_size == GOLD)
	{
		ret << m_price << " ";
	}
    ret <<  (dgettext("sumwars_xml",(getString()).c_str()));


	return ret.str();
}


std::string Item::getString()
{
	return ItemFactory::getItemName(m_subtype);
}


void Item::toString(CharConv* cv)
{

	cv->toBuffer((char) m_type);
	cv->toBuffer(m_subtype);
	cv->toBuffer(m_id);
	cv->toBuffer<char>(m_rarity);
	if (m_type == GOLD_TYPE)
	{
		cv->toBuffer(m_price);
	}

}

void Item::fromString(CharConv* cv)
{
	// Daten werden extern eingelesen

	char tmp;
	cv->fromBuffer<char>(tmp);
	m_rarity = (Rarity) tmp;
	if (m_type == GOLD_TYPE)
	{
		cv->fromBuffer(m_price);
	}
}

int Item::getValue(std::string valname)
{
	if (valname =="type")
	{
		if (m_type ==ARMOR)
			lua_pushstring(EventSystem::getLuaState() , "armor" );
		else if (m_type ==HELMET)
			lua_pushstring(EventSystem::getLuaState() , "helmet" );
		else  if (m_type ==GLOVES)
			lua_pushstring(EventSystem::getLuaState() , "gloves" );
		else  if (m_type ==WEAPON)
			lua_pushstring(EventSystem::getLuaState() , "weapon" );
		else  if (m_type ==SHIELD)
			lua_pushstring(EventSystem::getLuaState() , "shield" );
		else  if (m_type ==POTION)
			lua_pushstring(EventSystem::getLuaState() , "potion" );
		else  if (m_type ==RING)
			lua_pushstring(EventSystem::getLuaState() , "ring" );
		else  if (m_type ==AMULET)
			lua_pushstring(EventSystem::getLuaState() , "amulet" );
		else  if (m_type ==GOLD_TYPE)
			lua_pushstring(EventSystem::getLuaState() , "gold" );
		else 
			lua_pushstring(EventSystem::getLuaState() , "noitem" );
		return 1;
	}
	else if (valname =="subtype")
	{
		lua_pushstring(EventSystem::getLuaState() , m_subtype.c_str() );
		return 1;
	}
	else if (valname == "size")
	{
		if (m_size == BIG)
			lua_pushstring(EventSystem::getLuaState() , "big" );
		else if (m_size == MEDIUM)
			lua_pushstring(EventSystem::getLuaState() , "medium" );
		else if (m_size == SMALL)
			lua_pushstring(EventSystem::getLuaState() , "small" );
		else if (m_size == GOLD)
			lua_pushstring(EventSystem::getLuaState() , "gold" );
		return 1;
	}
	else if (valname == "price" || valname == "value")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_price );
		return 1;
	}
	else if (valname == "magic_power")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_magic_power );
		return 1;
	}
	else if (valname == "char_requirement")
	{
		lua_pushstring(EventSystem::getLuaState() , m_char_req.c_str() );
		return 1;		
	}
	else if (valname == "level_requirement")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_level_req );
		return 1;
	}
	else if (valname == "rarity")
	{
		if (m_rarity == NORMAL)
			lua_pushstring(EventSystem::getLuaState() , "normal" );
		else if (m_rarity == MAGICAL)
			lua_pushstring(EventSystem::getLuaState() , "magical" );
		else if (m_rarity == RARE)
			lua_pushstring(EventSystem::getLuaState() , "rare" );
		else if (m_rarity == UNIQUE)
			lua_pushstring(EventSystem::getLuaState() , "unique" );
		
		return 1;
	}
	else
	{
		int ret = 0;
		if (m_weapon_attr != 0)
			ret = m_weapon_attr->getValue(valname);
		if (ret != 0)
			return ret;
		
		if (m_useup_effect != 0)
			ret = m_useup_effect->getValue(valname);
		if (ret != 0)
			return ret;
		
		if (m_equip_effect != 0)
			ret = m_equip_effect->getValue(valname);
		if (ret != 0)
			return ret;
		
	}
	return 0;
}

bool Item::setValue(std::string valname)
{
	if (valname =="subtype")
	{
		m_subtype = lua_tostring(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname == "size")
	{
		std::string sizestr = lua_tostring(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		if (sizestr == "big")
			m_size = BIG;
		else if (sizestr == "medium")
			m_size = MEDIUM;
		else if (sizestr == "small")
			m_size = SMALL;
		
		return true;
	}
	else if (valname == "price" || valname == "value")
	{
		m_price = (int) lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname == "magic_power")
	{
		m_magic_power = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname == "char_requirement")
	{
		m_char_req = lua_tostring(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;		
	}
	else if (valname == "level_requirement")
	{
		m_level_req = char(lua_tonumber(EventSystem::getLuaState() ,-1));
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname == "rarity")
	{
		std::string rstr = lua_tostring(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		if (rstr == "normal")
			m_rarity = NORMAL;
		if (rstr == "magical")
			m_rarity = MAGICAL;
		if (rstr == "rare")
			m_rarity = RARE;
		if (rstr == "unique")
			m_rarity = UNIQUE;
		
		return true;
	}
	else
	{
		bool ret = false;
		if (m_weapon_attr != 0)
			ret = m_weapon_attr->setValue(valname);
		if (ret == true)
			return ret;
		
		if (m_useup_effect != 0)
			ret = m_useup_effect->setValue(valname);
		if (ret == true)
			return ret;
		
		// Equip Effekt wird auf Verdacht erzeugt und wieder geloescht, wenn kein Wert geschrieben wurde
		bool created = false;
		if (m_equip_effect != 0)
		{
			m_equip_effect = new CreatureBaseAttrMod;
			created = true;
		}
		ret = m_equip_effect->setValue(valname);
		if (ret == true)
			return ret;
		
		if (created)
		{
			delete m_equip_effect;
			m_equip_effect = 0;
		}
	}
	return false;
}


void Item::toStringComplete(CharConv* cv)
{
	toString(cv);

	if (m_type == GOLD_TYPE)
		return;

	cv->toBuffer(m_price);
	cv->toBuffer(m_level_req);
	cv->toBuffer(m_char_req);

	cv->toBuffer(m_magic_power);

	char mask = NOINFO;

	if (m_useup_effect!=0)
		mask |= USEUP_INFO;

	if (m_equip_effect!=0)
		mask |= EQUIP_INFO;

	if (m_weapon_attr!=0)
		mask |= WEAPON_INFO;

	cv->toBuffer(mask);

	int i;
	if (m_useup_effect!=0)
	{
		DEBUG5("writing useup effect");
		cv->toBuffer(m_useup_effect->m_dhealth);
		for (i=0;i<NR_STATUS_MODS;i++)
		{
			cv->toBuffer(m_useup_effect->m_dstatus_mod_immune_time[i]);
		}
	}

	if (m_equip_effect!=0)
	{
		DEBUG5("writing equip effect");
		cv->toBuffer(m_equip_effect->m_darmor);
		cv->toBuffer(m_equip_effect->m_dblock);
		cv->toBuffer(m_equip_effect->m_dmax_health);
		cv->toBuffer(m_equip_effect->m_dattack);
		cv->toBuffer(m_equip_effect->m_dstrength);
		cv->toBuffer(m_equip_effect->m_ddexterity);
		cv->toBuffer(m_equip_effect->m_dmagic_power);
		cv->toBuffer(m_equip_effect->m_dwillpower);
		for (i=0;i<4;i++)
		{
			cv->toBuffer(m_equip_effect->m_dresistances[i]);
		}

		for (i=0;i<4;i++)
		{
			cv->toBuffer(m_equip_effect->m_dresistances_cap[i]);
		}

		cv->toBuffer(m_equip_effect->m_dwalk_speed);
		cv->toBuffer(m_equip_effect->m_dattack_speed);
		cv->toBuffer(m_equip_effect->m_xspecial_flags);

		cv->toBuffer<short>(m_equip_effect->m_xabilities.size());
		std::set<std::string>::iterator it;
		for (it = m_equip_effect->m_xabilities.begin(); it != m_equip_effect->m_xabilities.end(); ++it)
		{
			cv->toBuffer(*it);
		}
		cv->toBuffer(m_equip_effect->m_ximmunity);

	}

	if (m_weapon_attr!=0)
	{
		DEBUG5("writing weapon attr");
		cv->toBuffer(m_weapon_attr->m_weapon_type);
		m_weapon_attr->m_damage.toString(cv);
		cv->toBuffer(m_weapon_attr->m_attack_range);
		cv->toBuffer(m_weapon_attr->m_two_handed);
		cv->toBuffer(m_weapon_attr->m_dattack_speed);

	}




}

void Item::fromStringComplete(CharConv* cv)
{
	fromString(cv);

	if (m_type == GOLD_TYPE)
		return;

	cv->fromBuffer<int>(m_price);
	cv->fromBuffer<char>(m_level_req);
	cv->fromBuffer(m_char_req);
	cv->fromBuffer<float>(m_magic_power);

	char mask = NOINFO;
	cv->fromBuffer<char>(mask);

	if (mask & USEUP_INFO)
	{
		if (m_useup_effect !=0)
			delete m_useup_effect;

		m_useup_effect= new CreatureDynAttrMod();
	}

	if (mask & EQUIP_INFO)
	{
		if (m_equip_effect !=0)
			delete m_equip_effect;

		m_equip_effect=new CreatureBaseAttrMod();
	}

	if (mask & WEAPON_INFO)
	{
		if (m_weapon_attr !=0)
			delete m_weapon_attr;

		m_weapon_attr = new WeaponAttr();
	}


	int i;
	if (m_useup_effect!=0)
	{
		DEBUG5("loading useup effect");
		cv->fromBuffer<float>(m_useup_effect->m_dhealth );
		for (i=0;i<NR_STATUS_MODS;i++)
		{
			cv->fromBuffer<float>(m_useup_effect->m_dstatus_mod_immune_time[i]);
		}
	}

	if (m_equip_effect!=0)
	{
		DEBUG5("loading equip effect");
		cv->fromBuffer<short>(m_equip_effect->m_darmor);
		cv->fromBuffer<short>(m_equip_effect->m_dblock);
		cv->fromBuffer<float>(m_equip_effect->m_dmax_health);
		cv->fromBuffer<short>(m_equip_effect->m_dattack);
		cv->fromBuffer<short>(m_equip_effect->m_dstrength);
		cv->fromBuffer<short>(m_equip_effect->m_ddexterity);
		cv->fromBuffer<short>(m_equip_effect->m_dmagic_power);
		cv->fromBuffer<short>(m_equip_effect->m_dwillpower);
		for (i=0;i<4;i++)
		{
			cv->fromBuffer<short>(m_equip_effect->m_dresistances[i]);
		}

		for (i=0;i<4;i++)
		{
			cv->fromBuffer<short>(m_equip_effect->m_dresistances_cap[i]);
		}

		cv->fromBuffer<short>(m_equip_effect->m_dwalk_speed);
		cv->fromBuffer<short>(m_equip_effect->m_dattack_speed);
		cv->fromBuffer<int>(m_equip_effect->m_xspecial_flags );

		short nr;
		cv->fromBuffer<short>(nr);
		Action::ActionType type;
		for (int i=0; i<nr; i++)
		{
			cv->fromBuffer(type);
			m_equip_effect->m_xabilities.insert(type);
		}
		
		cv->fromBuffer<char>(m_equip_effect->m_ximmunity);

	}

	if (m_weapon_attr!=0)
	{
		DEBUG5("loading weapon attr");
		cv->fromBuffer(m_weapon_attr->m_weapon_type);
		m_weapon_attr->m_damage.fromString(cv);
		cv->fromBuffer<float>(m_weapon_attr->m_attack_range);
		cv->fromBuffer<bool>(m_weapon_attr->m_two_handed);
		cv->fromBuffer<short>(m_weapon_attr->m_dattack_speed);

	}


}

std::string Item::getDescription(float price_factor)
{

	// String fuer die Beschreibung
	std::ostringstream out_stream;
	out_stream.str("");
	out_stream<<getName()<<"\n";
	int i;
	// Levelbeschraenkung
	out_stream <<gettext("Value")<<": "<<m_price;
	if (price_factor != 0 && price_factor != 1)
	{
		out_stream <<"\n" << gettext("Selling Value")<<": "<<std::max(1,int(m_price*price_factor));
	}
	if (m_level_req>0)
	{
		out_stream<<"\n" << gettext("Required level")<<": "<<(int) m_level_req;
	}
	
	if (m_char_req != "15" && m_char_req != "all")
	{
		size_t pos=0,pos2;
		out_stream<<"\n" << gettext("Required class")<<": ";
		
		std::string type;
		bool end = false;
		
		// alle Klassen ermitteln, die das Item verwenden koennen
		//  fuer jedes Teilstueck in dem string werden alle Klassen gesucht, die das entsprechende Tag besitzen
		std::set<std::string> classes;
		std::set<std::string>::iterator ct;
		std::map<GameObject::Subtype, PlayerBasicData*>& pdata = ObjectFactory::getPlayerData();
		std::map<GameObject::Subtype, PlayerBasicData*>::iterator it;
		do
		{
			pos2 = m_char_req.find_first_of(",|",pos);
			if (pos2 == std::string::npos)
			{
				pos2 = m_char_req.length();
				end = true;
			}
			type = m_char_req.substr(pos,pos2-pos);
			
			// Schleife ueber die Spielerklassen
			for (it = pdata.begin(); it != pdata.end(); ++it)
			{
				// Schleife ueber die Tags der Klasse
				std::list<std::string>::iterator jt;
				for (jt = it->second->m_item_req_tags.begin(); jt != it->second->m_item_req_tags.end(); ++jt)
				{
					if (*jt == type)
					{
						classes.insert(it->second->m_name);
					}
				}
				
			}
			pos = pos2+1;
		}
		while (!end);
		
		// Ausgabe schreiben
		bool first = true;
		for (ct = classes.begin(); ct != classes.end(); ++ct)
		{
			if (!first)
				out_stream<<", ";
			out_stream<<gettext(ct->c_str());
			
			first = false;
		}
		
	}

	// Effekt beim Verbrauchen
	if (m_useup_effect)
	{
		// HP Heilung
		if (m_useup_effect->m_dhealth>0)
		{
			out_stream <<"\n"<< gettext("Heals ")<<(int) m_useup_effect->m_dhealth<<gettext(" hitpoints");
		}

		// Heilen/ Immunisieren gegen Statusmods
		for (i=0;i<8;i++)
		{
			if (	m_useup_effect->m_dstatus_mod_immune_time[i]>0)
			{
				out_stream <<"\n"<< gettext("Heals ")<<Damage::getStatusModName((Damage::StatusMods) i);
				if (m_useup_effect->m_dstatus_mod_immune_time[i]>=1000)
				{
					out_stream <<", "<< gettext("immune for ")<< (int) (m_useup_effect->m_dstatus_mod_immune_time[i]*0.001f)<<"s";
				}
			}
		}

	}

	// Daten einer Waffe
	if (m_weapon_attr)
	{
		if (m_weapon_attr->m_two_handed)
		{
			out_stream <<"\n"<< gettext("Two-handed weapon");
		}
		// Reichweite / Angriffsgeschwindigkeit
		if (m_type == WEAPON)
		{
			out_stream << "\n" << gettext("Range")<<": "<<m_weapon_attr->m_attack_range;
		}

		//out_stream << "\n" << "Angriffe: "<<m_weapon_attr->m_attack_speed*0.001f<<"/s";

		// Schaden
		std::string dmgstring = m_weapon_attr->m_damage.getDamageString(Damage::ITEM);
		if (dmgstring != "")
		{
			if (m_type == WEAPON)
			{
				out_stream << "\n" << gettext("Damage")<<":";
			}
			out_stream<<"\n"<<dmgstring;
		}

	}

	// Effekte von Ausruestungsgegenstaenden
	if (m_equip_effect)
	{
		if (m_equip_effect->m_darmor>0)
		{
			out_stream<<"\n"<<gettext("Armor")<<": "<<m_equip_effect->m_darmor;
		}

		if (m_equip_effect->m_dblock>0)
		{
			out_stream<<"\n"<<gettext("Block")<<": "<<m_equip_effect->m_dblock;
		}

		if (m_equip_effect->m_dmax_health>0)
		{
			out_stream<<"\n"<<"+"<<(int) m_equip_effect->m_dmax_health<< " "<<gettext("max hitpoints");
		}

		if (m_equip_effect->m_dstrength>0)
		{
			out_stream<<"\n"<<"+"<<m_equip_effect->m_dstrength<< " "<<gettext("Strength");
		}

		if (m_equip_effect->m_ddexterity>0)
		{
			out_stream<<"\n"<<"+"<<m_equip_effect->m_ddexterity<< " "<<gettext("Dexterity");
		}

		if (m_equip_effect->m_dmagic_power>0)
		{
			out_stream<<"\n"<<"+"<<m_equip_effect->m_dmagic_power<< " "<<gettext("Magic Power");
		}

		if (m_equip_effect->m_dwillpower>0)
		{
			out_stream<<"\n"<<"+"<<m_equip_effect->m_dwillpower<< " "<<gettext("Willpower");
		}

		for (i=0;i<4;i++)
		{
			if (m_equip_effect->m_dresistances[i]>0)
			{
				out_stream<<"\n"<<"+"<<m_equip_effect->m_dresistances[i]<<" "<<Damage::getDamageResistanceName((Damage::DamageType) i);
			}
		}

		for (i=0;i<4;i++)
		{
			if (m_equip_effect->m_dresistances_cap[i]>0)
			{
				out_stream<<"\n"<<"+"<<m_equip_effect->m_dresistances_cap[i]<<gettext(" max. ")<<Damage::getDamageResistanceName((Damage::DamageType) i);
			}
		}


		// TODO: Angriffsgeschwindigkeit
		// TODO: special Flags
		// TODO: Faehigkeiten
		// TODO: Immunitaeten

	}


	return out_stream.str();
}

void Item::calcPrice()
{
	if (m_useup_effect !=0)
	{
		// Trank, beim generieren erzeugten wert nutzen
		return;
	}

	// Nutzwert des Gegenstandes
	float value =0;
	// Faktor fuer den wert
	float mult =1;

	int i;
	if (m_weapon_attr !=0)
	{
		// Wert des Schadens;
		float dvalue=0;
		
		// Schaden der Waffe
		Damage & dmg = m_weapon_attr->m_damage;
		// phys Schaden
		dvalue += 0.5*(dmg.m_min_damage[0] + dmg.m_max_damage[0])*dmg.m_multiplier[0] * (1+dmg.m_crit_perc*2);
		if (dmg.m_multiplier[0] >1)
		{
			dvalue += (dmg.m_multiplier[0]-1)*100;
		}
		// elementar Schaden
		for (i=1;i<4;i++)
		{
			dvalue += 0.5*(dmg.m_min_damage[i] + dmg.m_max_damage[i])*dmg.m_multiplier[i];
			if (dmg.m_multiplier[i] >1)
			{
				dvalue += (dmg.m_multiplier[i]-1)*100;
			}
		}
		dvalue *= (2000 + m_weapon_attr->m_dattack_speed)/1000.0;

		dvalue += dmg.m_attack*0.1;
		dvalue += dmg.m_power*0.1;
		for (i=0;i<8;i++)
		{
			dvalue += dmg.m_status_mod_power[i]*0.2;
		}
		// TODO: Flags einberechnen
		value += dvalue * std::max(1.0,std::min(1.5,sqrt(m_weapon_attr->m_attack_range)));
		
	}

	if (m_equip_effect!=0)
	{
		// Modifikation beim anlegen
		CreatureBaseAttrMod* cbasm = m_equip_effect;

		value += cbasm->m_darmor;
		value += cbasm->m_dblock;
		value += cbasm->m_dattack*0.1;
		value += cbasm->m_dmax_health*0.2;
		value += cbasm->m_dstrength;
		value += cbasm->m_ddexterity;
		value += cbasm->m_dwillpower;
		value += cbasm->m_dmagic_power;
		mult *= (1+cbasm->m_dattack_speed/2000.0);

		for (i=0;i<4;i++)
		{
			value += cbasm->m_dresistances[i]*0.6;
		}

		for (i=0;i<4;i++)
		{
			value += cbasm->m_dresistances_cap[i];
		}


		// TODO: Special Flags einberechnen

		// TODO: Immunitaeten einberechnen

		// TODO: Skills mit einberechnen

	}
	
	value = (2+0.5*(1+0.1*value)*value)* mult;

	value = std::min (value,100000.0f);
	m_price += (int) value;
}




