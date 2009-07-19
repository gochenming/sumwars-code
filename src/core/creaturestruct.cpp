#include "creaturestruct.h"
#include "damage.h"
#include "eventsystem.h"


void CreatureBaseAttr::init()
{
	DEBUG5("init CreatureBaseAttrMod");
		// alles nullen
	m_level =1;
	m_armor =0;
	m_block =0;
	m_max_health =0;
	m_attack =0;
	m_power =0;
	m_strength =0;
	m_dexterity =0;
	m_magic_power =0;
	m_willpower =0;
	for (int i=0; i<4; i++)
	{
		m_resistances[i] =0;
		m_resistances_cap[i] =0;
	}
	m_walk_speed =0;
	m_attack_speed=0;
	m_step_length = 1;
	m_special_flags=0;
	m_abilities.clear();
	m_immunity =0;
	m_max_experience = 100;
	m_attack_range = 1;
}

void CreatureBaseAttr::operator=(CreatureBaseAttr other)
{
	m_level =other.m_level;
	m_armor =other.m_armor;
	m_block =other.m_block;
	m_max_health =other.m_max_health;
	m_attack =other.m_attack;
	m_power =other.m_power;
	m_strength =other.m_strength;
	m_dexterity =other.m_dexterity;
	m_magic_power =other.m_magic_power;
	m_willpower =other.m_willpower;
	for (int i=0; i<4; i++)
	{
		m_resistances[i] =other.m_resistances[i];
		m_resistances_cap[i] = other.m_resistances_cap[i];
	}
	m_walk_speed =other.m_walk_speed;
	m_attack_speed=other.m_attack_speed;
	m_step_length = other.m_step_length;
	m_special_flags=other.m_special_flags;
	m_abilities = other.m_abilities;
	m_immunity =other.m_immunity;
	m_max_experience = other.m_max_experience;
	m_attack_range = other.m_attack_range;
}

int CreatureBaseAttr::getValue(std::string valname)
{
	if (valname =="level")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_level );
		return 1;
	}
	else if (valname =="strength")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_strength );
		return 1;
	}
	else if (valname =="dexterity")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_dexterity );
		return 1;
	}
	else if (valname =="willpower")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_willpower );
		return 1;
	}
	else if (valname =="magic_power")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_magic_power );
		return 1;
	}
	else if (valname =="max_health")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_max_health );
		return 1;
	}
	else if (valname =="block")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_block );
		return 1;
	}
	else if (valname =="attack")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_attack );
		return 1;
	}
	else if (valname =="armor")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_armor );
		return 1;
	}
	else if (valname =="resist_fire")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances[Damage::FIRE] );
		return 1;
	}
	else if (valname =="resist_ice")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances[Damage::ICE] );
		return 1;
	}
	else if (valname =="resist_air")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances[Damage::AIR] );
		return 1;
	}
	else if (valname =="resist_phys")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances[Damage::PHYSICAL] );
		return 1;
	}
	else if (valname =="max_resist_ice")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances_cap[Damage::ICE] );
		return 1;
	}
	else if (valname =="max_resist_air")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances_cap[Damage::AIR] );
		return 1;
	}
	else if (valname =="max_resist_fire")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances_cap[Damage::FIRE] );
		return 1;
	}
	else if (valname =="max_resist_phys")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances_cap[Damage::PHYSICAL] );
		return 1;
	}
	else if (valname =="attack_speed")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_attack_speed );
		return 1;
	}
	else if (valname =="walk_speed")
	{
		lua_pushinteger(EventSystem::getLuaState() ,m_walk_speed );
		return 1;
	}
	
	return 0;
}

bool CreatureBaseAttr::setValue(std::string valname, int& event_mask)
{
	if (valname =="level")
	{
		m_level = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTRIBUTES_LEVEL;
		return true;
	}
	else if (valname =="strength")
	{
		m_strength = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTRIBUTES_LEVEL;
		return true;
	}
	else if (valname =="dexterity")
	{
		m_dexterity = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTRIBUTES_LEVEL;
		return true;
	}
	else if (valname =="willpower")
	{
		m_willpower = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTRIBUTES_LEVEL;
		return true;
	}
	else if (valname =="magic_power")
	{
		m_magic_power = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTRIBUTES_LEVEL;
		return true;
	}
	else if (valname =="max_health")
	{
		m_max_health = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTRIBUTES_LEVEL;
		return true;
	}
	else if (valname =="block")
	{
		m_block = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="attack")
	{
		m_attack = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="armor")
	{
		m_armor = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="resist_fire")
	{
		m_resistances[Damage::FIRE] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
		
	}
	else if (valname =="resist_ice")
	{
		m_resistances[Damage::ICE] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="resist_air")
	{
		m_resistances[Damage::AIR] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="resist_phys")
	{
		m_resistances[Damage::PHYSICAL] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="max_resist_ice")
	{
		m_resistances_cap[Damage::ICE] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="max_resist_air")
	{
		m_resistances_cap[Damage::AIR] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="max_resist_fire")
	{
		m_resistances_cap[Damage::FIRE] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="max_resist_phys")
	{
		m_resistances_cap[Damage::PHYSICAL] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="attack_speed")
	{
		m_attack_speed = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTACK_WALK_SPEED;
		return true;
	}
	else if (valname =="walk_speed")
	{
		m_walk_speed = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTACK_WALK_SPEED;
		return true;
	}
	
	return false;
}


void CreatureBaseAttrMod::init()
{
	DEBUG5("init CreatureBaseAttrMod");
		// alles nullen
	m_darmor =0;
	m_dblock =0;
	m_dmax_health =0;
	m_dattack =0;
	m_dpower =0;
	m_dstrength =0;
	m_ddexterity =0;
	m_dmagic_power =0;
	m_dwillpower =0;
	for (int i=0; i<4; i++)
	{
		m_dresistances[i] =0;
		m_dresistances_cap[i] =0;
	}
	m_dwalk_speed =0;
	m_dattack_speed=0;
	m_xspecial_flags=0;
	m_time =0;
	m_xabilities.clear();
	m_ximmunity =0;
}


void CreatureBaseAttrMod::operator=(CreatureBaseAttrMod other)
{
	m_darmor =other.m_darmor;
	m_dblock =other.m_dblock;
	m_dmax_health =other.m_dmax_health;
	m_dattack =other.m_dattack;
	m_dpower =other.m_dpower;
	m_dstrength =other.m_dstrength;
	m_ddexterity =other.m_ddexterity;
	m_dmagic_power =other.m_dmagic_power;
	m_dwillpower =other.m_dwillpower;
	for (int i=0; i<4; i++)
	{
		m_dresistances[i] =other.m_dresistances[i];
		m_dresistances_cap[i] =other.m_dresistances_cap[i];
	}
	m_dwalk_speed =other.m_dwalk_speed;
	m_dattack_speed=other.m_dattack_speed;
	m_xspecial_flags=other.m_xspecial_flags;
	m_time =other.m_time;
	m_xabilities = other.m_xabilities;
	m_ximmunity =other.m_ximmunity;
}

int CreatureDynAttr::getValue(std::string valname)
{
	if (valname =="health")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_health );
		return 1;
	}
	else if (valname =="experience")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_experience );
		return 1;
	}
	else if (valname =="blind_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::BLIND]);
		return 1;
		
	}
	else if (valname =="poisoned_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::POISONED]);
		return 1;
	}
	else if (valname =="berserk_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::BERSERK]);
		return 1;
	}
	else if (valname =="confused_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::CONFUSED]);
		return 1;
	}
	else if (valname =="mute_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::MUTE]);
		return 1;
	}
	else if (valname =="paralyzed_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::PARALYZED]);
		return 1;
	}
	else if (valname =="frozen_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::FROZEN]);
		return 1;
	}
	else if (valname =="burning_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::BURNING]);
		return 1;
	}
	
	return 0;
}

bool CreatureDynAttr::setValue(std::string valname, int& event_mask)
{
	if (valname =="health")
	{
		m_health = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_HP;
		return true;
	}
	else if (valname =="experience")
	{
		m_experience = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_EXPERIENCE;
		return true;
	}
	else if (valname =="blind_time")
	{
		m_status_mod_time[Damage::BLIND] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	else if (valname =="poisoned_time")
	{
		m_status_mod_time[Damage::POISONED] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	else if (valname =="berserk_time")
	{
		m_status_mod_time[Damage::BERSERK] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	else if (valname =="confused_time")
	{
		m_status_mod_time[Damage::CONFUSED] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	else if (valname =="mute_time")
	{
		m_status_mod_time[Damage::MUTE] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	else if (valname =="paralyzed_time")
	{
		m_status_mod_time[Damage::PARALYZED] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	else if (valname =="frozen_time")
	{
		m_status_mod_time[Damage::FROZEN] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	else if (valname =="burning_time")
	{
		m_status_mod_time[Damage::BURNING] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	
	return false;
}

void CreatureDynAttr::operator=(CreatureDynAttr other)
{
	m_health = other.m_health;
	m_experience = other.m_experience;
	for (int i=0; i<NR_EFFECTS; i++)
		m_effect_time[i] = other.m_effect_time[i];
	for (int i=0; i<NR_STATUS_MODS; i++)
	{
		m_status_mod_time[i] = other.m_status_mod_time[i];
		m_status_mod_immune_time[i] = other.m_status_mod_immune_time[i];
	}
	m_temp_mods = other.m_temp_mods;
	m_timer = other.m_timer;
}

void CreatureDynAttrMod::operator=(CreatureDynAttrMod other)
{
	m_dhealth = other.m_dhealth;
	for (int i=0; i< NR_STATUS_MODS; i++)
	{
		m_dstatus_mod_immune_time[i] = other.m_dstatus_mod_immune_time[i];
	}
}

void CreatureSpeakText::operator=( CreatureSpeakText& other)
{
	m_text = other.m_text;
	m_time = other.m_time;
	m_answers = other.m_answers;
}

bool CreatureSpeakText::empty()
{
	return (m_text == "" && m_answers.empty());
}

void CreatureSpeakText::clear()
{
	m_text = "";
	m_answers.clear();
}

