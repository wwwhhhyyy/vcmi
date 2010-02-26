#pragma once
#include "../global.h"
#include <string>
#include <list>

/*
 * HeroBonus.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

struct DLL_EXPORT HeroBonus
{
	enum BonusType
	{
		//handled
		NONE, 
		MOVEMENT, //both water/land
		LAND_MOVEMENT, 
		SEA_MOVEMENT, 
		MORALE, 
		LUCK, 
		MORALE_AND_LUCK, 
		PRIMARY_SKILL, //uses subtype to pick skill
		SIGHT_RADIOUS, 
		MANA_REGENERATION, //points per turn apart from normal (1 + mysticism)
		FULL_MANA_REGENERATION, //all mana points are replenished every day
		NONEVIL_ALIGNMENT_MIX, //good and neutral creatures can be mixed without morale penalty
		HP_REGENERATION, //regenerates a certain amount of hp for the top of each stack every turn, val - hp regained
		LEVEL_SPELL_IMMUNITY, //val - spell level creatures become immune to and below
		//might not be handled yet:
		MAGIC_RESISTANCE, // %
		SECONDARY_SKILL_PREMY, //%
		SURRENDER_DISCOUNT, //%
		STACKS_SPEED,
		FLYING_MOVEMENT, SPELL_DURATION, AIR_SPELL_DMG_PREMY, EARTH_SPELL_DMG_PREMY, FIRE_SPELL_DMG_PREMY, 
		WATER_SPELL_DMG_PREMY, BLOCK_SPELLS_ABOVE_LEVEL, WATER_WALKING, NO_SHOTING_PENALTY, DISPEL_IMMUNITY, 
		NEGATE_ALL_NATURAL_IMMUNITIES, STACK_HEALTH, STACK_HEALTH_PERCENT, //the second one of stack health - value in % of base HP to be added to overall stack HP
		SPELL_IMMUNITY, BLOCK_MORALE, BLOCK_LUCK, FIRE_SPELLS,
		AIR_SPELLS, WATER_SPELLS, EARTH_SPELLS, 
		GENERATE_RESOURCE, //daily value, uses subtype (resource type)
		CREATURE_GROWTH, //for legion artifacts: value - week growth bonus, subtype - monster level
		WHIRLPOOL_PROTECTION, //hero won't lose army when teleporting through whirlpool
		SPELL, //hero knows spell, val - skill level (0 - 3), subtype - spell id
		SPELLS_OF_LEVEL, //hero knows all spells of given level, val - skill level; subtype - level
		ENEMY_CANT_ESCAPE, //for shackles of war
		MAGIC_SCHOOL_SKILL, //eg. for magic plains terrain, subtype: school of magic (0 - all, 1 - fire, 2 - air, 4 - water, 8 - earth), value - level
		FREE_SHOOTING, //stacks can shoot even if otherwise blocked (sharpshooter's bow effect)
		OPENING_BATTLE_SPELL, //casts a spell at expert level at beginning of battle, val - spell power, subtype - spell id
		IMPROVED_NECROMANCY, //allows Necropolis units other than skeletons to be raised by necromancy
		CREATURE_GROWTH_PERCENT, //increases growth of all units in all towns, val - percentage
		FREE_SHIP_BOARDING //movement points preserved with ship boarding and landing
	};
	enum BonusDuration{PERMANENT, ONE_BATTLE, ONE_DAY, ONE_WEEK};
	enum BonusSource{ARTIFACT, OBJECT};

	ui8 duration; //uses BonusDuration values
	ui8 type; //uses BonusType values - says to what is this bonus
	si32 subtype; //-1 if not applicable
	ui8 source;//uses BonusSource values - what gave that bonus
	si32 val;//for morale/luck [-3,+3], others any
	ui32 id; //id of object/artifact
	std::string description; 

	HeroBonus(ui8 Dur, ui8 Type, ui8 Src, si32 Val, ui32 ID, std::string Desc, si32 Subtype=-1)
		:duration(Dur), type(Type), subtype(Subtype), source(Src), val(Val), id(ID), description(Desc) 
	{}
	HeroBonus(ui8 Dur, ui8 Type, ui8 Src, si32 Val, ui32 ID, si32 Subtype=-1)
		:duration(Dur), type(Type), subtype(Subtype), source(Src), val(Val), id(ID) 
	{}
	HeroBonus()
	{
		subtype = -1;
	}

// 	//comparison
// 	bool operator==(const HeroBonus &other)
// 	{
// 		return &other == this;
// 		//TODO: what is best logic for that?
// 	}
// 	bool operator<(const HeroBonus &other)
// 	{
// 		return &other < this;
// 		//TODO: what is best logic for that?
// 	}

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & duration & type & subtype & source & val & id & description;
	}

	static bool OneDay(const HeroBonus &hb)
	{
		return hb.duration==HeroBonus::ONE_DAY;
	}
	static bool OneWeek(const HeroBonus &hb)
	{
		return hb.duration==HeroBonus::ONE_WEEK;
	}
	static bool OneBattle(const HeroBonus &hb)
	{
		return hb.duration==HeroBonus::ONE_BATTLE;
	}
	static bool IsFrom(const HeroBonus &hb, ui8 source, ui32 id) //if id==0xffffff then id doesn't matter
	{
		return hb.source==source && (id==0xffffff  ||  hb.id==id);
	}

};

static const HeroBonus::BonusType MORALE_AFFECTING[] =  {HeroBonus::MORALE, HeroBonus::MORALE_AND_LUCK};
static const HeroBonus::BonusType LUCK_AFFECTING[] =  {HeroBonus::LUCK, HeroBonus::MORALE_AND_LUCK};
typedef std::vector<std::pair<int,std::string> > TModDescr; //modifiers values and their descriptions

class BonusList : public std::list<HeroBonus>
{
public:
	int DLL_EXPORT valOfBonuses(HeroBonus::BonusType type, int subtype = -1) const; //subtype -> subtype of bonus, if -1 then any
	bool DLL_EXPORT hasBonusOfType(HeroBonus::BonusType type, int subtype = -1) const;
	const DLL_EXPORT HeroBonus * getBonus( int from, int id ) const;
	void DLL_EXPORT getModifiersWDescr( std::vector<std::pair<int,std::string> > &out, HeroBonus::BonusType type, int subtype = -1 ) const;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & static_cast<std::list<HeroBonus>&>(*this);
	}
};