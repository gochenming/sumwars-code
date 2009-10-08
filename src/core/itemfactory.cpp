#include "itemfactory.h"


#include <iostream>

std::map<Item::Subtype, DropChance> ItemFactory::m_item_probabilities;

std::map<Item::Subtype, Item::Type> ItemFactory::m_item_types;

std::map<Item::Subtype,ItemBasicData*> ItemFactory::m_item_data;


ItemFactory::ItemFactory()
{
}


Item* ItemFactory::createItem(Item::Type type, Item::Subtype subtype, int id, float magic_power, Item::Rarity rarity)
{
	
	if (type == Item::NOITEM)
		return 0;
	
	DEBUG5("creating item %i / %s",type, subtype.c_str());
	
	if (type == Item::GOLD_TYPE)
	{
		return createGold(0,id);
	}
	
	std::map<Item::Subtype,ItemBasicData*>::iterator it;
	it = m_item_data.find(subtype);
	Item * item;

	if (it != m_item_data.end())
	{
		ItemBasicData* idata = it->second;
		item = new Item(*idata,id);
		item->m_rarity = rarity;
		

		createMagicMods(item,idata->m_modchance,magic_power, idata->m_min_enchant, idata->m_max_enchant);

		// Preis ausrechnen
		item->calcPrice();

		return item;
	}
	else
	{
		WARNING("Item type %s unknown",subtype.c_str());
		item = new Item();
		item->m_id = id;
		item->m_type = type;
		item->m_subtype = subtype;
		
		return item;
	}
	return 0;

}

Item* ItemFactory::createGold(int value, int id)
{
	Item* item = new Item;
	if (id ==0)
	{
		id = World::getWorld()->getValidId();
	}
	
	item->m_size = Item::GOLD;
	item->m_type = Item::GOLD_TYPE;
	item->m_id = id;
	item->m_subtype = "gold";
	item->m_price = value;
	
	return item;
}


void ItemFactory::createMagicMods(Item* item, float* modchance, float magic_power, float min_enchant, float max_enchant)
{
	DEBUG5("magic power %f min_enchant %f",magic_power, min_enchant);
	if (magic_power < min_enchant && magic_power != 0)
	{
		if (item->m_type == Item::RING || item->m_type == Item::AMULET)
		{
			magic_power = min_enchant+1;
		}
		else
		{
			return;
		}
	}
	
	// Modifikationen des Items auswuerfeln
	DEBUG5("mods auswuerfeln");

	magic_power = std::min(magic_power, max_enchant*4);
	item->m_magic_power =0;
	
	// bisher zugeteilte Staerke der Modifikation
	float mod_power[31];
	float modprob[31];
	for (int i=0; i<31; i++)
		modprob[i] = modchance[i];
	
	memset(mod_power,0,31*sizeof(float));

	float sum =0;
	int i;
	for (i=0;i<31;i++)
	{
		sum += modprob[i];
	}
	
	if (sum < 0.001)
		return;
	
	// Staerke der aktuellen Verzauberung
	float mp;
	float sqrtmp;
	float logmp;

	float dmgdiff,dmgavg;

		// durch Verzauberungen benoetigtes Level
	int levelreq =0;

		// aktuelle Verzauberung
	int mod;

		// Anzahl verschiedene Verzauberungen
	int num_mods=0;
	while (magic_power>=min_enchant && num_mods<4)
	{
		if (item->m_rarity == Item::NORMAL)
		{
			item->m_rarity = Item::MAGICAL;
		}
		// Staerke auswuerfeln
		mp = Random::randrangef(min_enchant,max_enchant);
		mp = std::min(mp, magic_power);
		item->m_magic_power += mp;
		
		sqrtmp = sqrt(mp);
		logmp = log(mp);
		magic_power -= mp;
		
			// Modifikation auswuerfeln
		mod = Random::randDiscrete(modprob,31,sum);
		DEBUG5("ausgewuerfelt: Starke der Verzauberung: %f",mp);
		DEBUG5("Art der Verzauberung: %i",mod);

		num_mods++;

		levelreq = std::max(levelreq,(int) (mp*0.06-1));
		levelreq = std::min(80,levelreq);

		dmgavg = mp*0.06;
		
		// bei Ringen und Amuletten koennen die Strukturen noch fehlen
		if (mod <=RESIST_ALL_MOD && item->m_equip_effect==0)
		{
			item->m_equip_effect = new CreatureBaseAttrMod;
		}
		else if (mod > RESIST_ALL_MOD && item->m_weapon_attr ==0)
		{
			item->m_weapon_attr = new WeaponAttr;
			item->m_weapon_attr->m_weapon_type = "notype";
		}

			// Wirkung der Modifikation
		switch (mod)
		{
			case HEALTH_MOD:
				item->m_equip_effect->m_dmax_health += ceil(mp*0.15);
				break;

			case ARMOR_MOD:
				item->m_equip_effect->m_darmor += (short) ceil(mp*0.08);
				break;

			case BLOCK_MOD:
				item->m_equip_effect->m_dblock += (short) ceil(mp*0.08);
				break;

			case STRENGTH_MOD:
				item->m_equip_effect->m_dstrength += (short) ceil(mp*0.03);
				break;

			case DEXTERITY_MOD:
				item->m_equip_effect->m_ddexterity += (short) ceil(mp*0.03);
				break;

			case WILLPOWER_MOD:
				item->m_equip_effect->m_dwillpower += (short) ceil(mp*0.03);
				break;

			case MAGIC_POWER_MOD:
				item->m_equip_effect->m_dmagic_power += (short) ceil(mp*0.03);
				break;

			case RESIST_PHYS_MOD:
				item->m_equip_effect->m_dresistances[Damage::PHYSICAL] += (short) ceil(sqrtmp*0.3);
				break;

			case RESIST_FIRE_MOD:
				item->m_equip_effect->m_dresistances[Damage::FIRE] += (short) ceil(sqrtmp*0.6);
				break;

			case RESIST_ICE_MOD:
				item->m_equip_effect->m_dresistances[Damage::ICE] += (short) ceil(sqrtmp*0.6);
				break;

			case RESIST_AIR_MOD:
				item->m_equip_effect->m_dresistances[Damage::AIR] += (short) ceil(sqrtmp*0.6);
				break;

			case RESIST_ALL_MOD:
				item->m_equip_effect->m_dresistances[Damage::FIRE] += (short) ceil(sqrtmp*0.3);
				item->m_equip_effect->m_dresistances[Damage::ICE] += (short) ceil(sqrtmp*0.3);
				item->m_equip_effect->m_dresistances[Damage::AIR] += (short) ceil(sqrtmp*0.3);
				break;

			case DAMAGE_PHYS_MOD:
				dmgavg *=0.6;
				dmgdiff = Random::randrangef(dmgavg*0.2,dmgavg*0.6);
				item->m_weapon_attr->m_damage.m_min_damage[Damage::PHYSICAL] += ceil(dmgavg-dmgdiff);
				item->m_weapon_attr->m_damage.m_max_damage[Damage::PHYSICAL] += ceil(dmgavg+dmgdiff);
				break;

			case DAMAGE_FIRE_MOD:
				dmgdiff = Random::randrangef(dmgavg*0.2,dmgavg*0.5);
				item->m_weapon_attr->m_damage.m_min_damage[Damage::FIRE] += ceil(dmgavg-dmgdiff);
				item->m_weapon_attr->m_damage.m_max_damage[Damage::FIRE] += ceil(dmgavg+dmgdiff);
				break;

			case DAMAGE_ICE_MOD:
				dmgdiff = Random::randrangef(dmgavg*0.1,dmgavg*0.3);
				item->m_weapon_attr->m_damage.m_min_damage[Damage::ICE] += ceil(dmgavg-dmgdiff);
				item->m_weapon_attr->m_damage.m_max_damage[Damage::ICE] += ceil(dmgavg+dmgdiff);
				break;

			case DAMAGE_AIR_MOD:
				dmgdiff = Random::randrangef(dmgavg*0.4,dmgavg*0.8);
				item->m_weapon_attr->m_damage.m_min_damage[Damage::AIR] += ceil(dmgavg-dmgdiff);
				item->m_weapon_attr->m_damage.m_max_damage[Damage::AIR] += ceil(dmgavg+dmgdiff);
				break;

			case DAMAGE_MULT_PHYS_MOD:
				item->m_weapon_attr->m_damage.m_multiplier[Damage::PHYSICAL] *= (1+sqrtmp*0.01);
				break;

			case DAMAGE_MULT_FIRE_MOD:
				item->m_weapon_attr->m_damage.m_multiplier[Damage::FIRE] *= (1+sqrtmp*0.006);
				break;

			case DAMAGE_MULT_ICE_MOD:
				item->m_weapon_attr->m_damage.m_multiplier[Damage::ICE] *= (1+sqrtmp*0.006);
				break;

			case DAMAGE_MULT_AIR_MOD:
				item->m_weapon_attr->m_damage.m_multiplier[Damage::AIR] *= (1+sqrtmp*0.006);
				break;

			case ATTACK_SPEED_MOD:
				item->m_equip_effect->m_dattack_speed += (short)  (sqrtmp*15);
				break;

			case ATTACK_MOD:
				item->m_weapon_attr->m_damage.m_attack += ceil(mp*0.05);
				break;

			case POWER_MOD:
				item->m_weapon_attr->m_damage.m_power += ceil(mp*0.05);
				break;

		}

			// jede Modifikation darf nur einmal vorkommen, entfernen

		sum -= modprob[mod];
		modprob[mod]=0;
	}

	item->m_level_req = std::max(item->m_level_req,(char) levelreq);
	DEBUG5("level req %i",item->m_level_req);
}

Item::Type  ItemFactory::getBaseType(Item::Subtype subtype)
{
	std::map<Item::Subtype,Item::Type>::iterator it;
	it = m_item_types.find(subtype);
	
	if (it != m_item_types.end())
	{
		return it->second;
	}
	
	return Item::NOITEM;
}

std::string ItemFactory::getItemName(Item::Subtype subtype)
{
	std::map<Item::Subtype,ItemBasicData*>::iterator it;
	it = m_item_data.find(subtype);
	
	if (it== m_item_data.end())
	{
		return subtype;
	}
	return it->second->m_name;
}

void ItemFactory::registerItemDrop(Item::Subtype subtype, DropChance chance)
{
	m_item_probabilities.insert(make_pair(subtype, chance));
}

void ItemFactory::registerItem(Item::Type type,Item::Subtype subtype, ItemBasicData* data)
{
	DEBUG5("registered item %s %p",subtype.c_str(), data->m_weapon_attr);
	m_item_data.insert(make_pair(subtype, data));
	m_item_types.insert(make_pair(subtype,type));
}

void ItemFactory::init()
{

}

void ItemFactory::cleanup()
{
	DEBUG("cleanup");

	std::map<Item::Subtype,ItemBasicData*>::iterator it;
	for (it = m_item_data.begin(); it != m_item_data.end(); ++it)
	{
		DEBUG5("deleting item data %s",it->first.c_str());
		delete it->second;
	}
}

Item* ItemFactory::createItem(DropSlot &slot, float factor )
{
	DEBUG5("get item by dropslot");
	// wenn maximales Level unter 0 liegt kein Item ausgeben
	if (slot.m_max_level<0)
		return 0;

	DEBUG5("drop item %i %i",slot.m_min_level, slot.m_max_level);
	
	float probs[5];
	probs[Item::SIZE_NONE] =1;
	for (int i=0; i<4; i++)
	{
		probs[i] = slot.m_size_probability[i]* factor;
		probs[Item::SIZE_NONE] -= slot.m_size_probability[i]* factor;
	}
	
	if (probs[Item::SIZE_NONE] < 0)
	{
		float div = 1-probs[Item::SIZE_NONE];
		for (int i=0; i<4; i++)
		{
			probs[i] /= div;
		}
	}
		
	int size = Random::randDiscrete(probs,5);

	// kein Item ausgewurfelt
	if (size ==4)
		return 0;

	
	
	DEBUG5("item size: %i",size);
	Item* item =0;
	if (size  == Item::GOLD)
	{
		
		// Gold gedroppt
		item = createGold(Random::randrangei(slot.m_min_gold, slot.m_max_gold));

		return item;
	}
	else
	{
		// *richtiges* Item gedroppt

		// Vector fuer die moeglichen Items
		std::vector<Item::Subtype> types;

		// Vector fuer die Wahrscheinlickeit
		std::vector<float> prob;

		float p;

		std::map<Item::Subtype, DropChance>::iterator i;
		for (i= m_item_probabilities.begin();i!=m_item_probabilities.end();++i)
		{
			// Testen ob die Groesse passt
			if (i->second.m_size != size)
				continue;

			// Testen ob Level ausreicht
			if (fabs(i->second.m_level) > slot.m_max_level || (i->second.m_level>0 && i->second.m_level <slot.m_min_level))
				continue;

			// Wahrscheinlichkeit fuer Drop berechnen
			p = i->second.m_probability;

			if (p>0)
			{
				DEBUG5("possible item subtype %s prob %f",i->first.c_str(),p);
				types.push_back(i->first);
				prob.push_back(p);

			}


		}

		if (prob.empty())
			return 0;

		// Item auswuerfeln
		int res = Random::randDiscrete(prob);

		// Typ, Subtyp bestimmen
		Item::Subtype subtype= types[res];
		Item::Type type = m_item_types.find(subtype)->second;

		// Magiestaerke berechnen
		float magic =0;
		if (Random::random() < slot.m_magic_probability || type == Item::RING  || type == Item::AMULET)
		{
			magic = Random::randrangef(slot.m_magic_power/4,slot.m_magic_power);
		}
		

		item = createItem(type,subtype,0,magic);
		DEBUG5("item type %i  subtype %s level %i magic %f",type,subtype.c_str(), item->m_level_req,magic);
		return item;
	}
	return 0;

}

