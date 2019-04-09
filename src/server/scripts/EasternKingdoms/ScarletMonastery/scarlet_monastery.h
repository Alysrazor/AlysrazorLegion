/*
 * Copyright (C) 2008-2019 TrinityCore <https://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCARLET_M_
#define SCARLET_M_

#include "CreatureAIImpl.h"

#define SMScriptName "instance_scarlet_monastery"
#define DataHeader "SM"

uint32 const EncounterCount = 3;

enum SMDataTypes
{
	DATA_THALNOS                     = 1,
	DATA_KORLOFF                     = 2,
	
	DATA_DURAND_AND_WHITE_EVENT      = 3,
	DATA_DURAND		    			 = 4,
	DATA_WHITEMANE					 = 5,
	
	DATA_HORSEMAN_EVENT				 = 6,
	DATA_PUMPKIN_SHRINE				 = 7
};

enum SMCreatureIds
{
	NPC_THALNOS                     = 59789,
	
	NPC_KORLOFF                     = 59223,
	
    NPC_DURAND                      = 60040,
    NPC_WHITEMANE                   = 3977,

    NPC_HORSEMAN                    = 23682,
    NPC_HEAD                        = 23775,
    NPC_PUMPKIN                     = 23694
};

enum SMGameObjectIds
{
    GO_HIGH_INQUISITORS_DOOR        = 104600,
    GO_PUMPKIN_SHRINE               = 186267
};

template<typename AI>
inline AI* GetScarletMonasteryAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, SMScriptName);
}

#endif // SCARLET_M_
