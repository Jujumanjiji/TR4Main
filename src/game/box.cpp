#include "framework.h"
#include "box.h"
#include "3dsystem/3d_gen.h"
#include "control.h"
#include "draw.h"
#include "items.h"
#include "lot.h"
#include "utils/utils.h"

void InitialiseCreature(short itemNumber)
{
	ITEM_INFO* item = &items[itemNumber];
	item->data = NULL;
	item->collidable = true;
}

BOOL CreatureActive(short itemNumber)
{
	ITEM_INFO* item = &items[itemNumber];

	if (CHK_ANY(item->flags, IFLAG_KILLED_ITEM))
		return FALSE;

	if (item->status == FITEM_INVISIBLE)
	{
		int enabled = EnableBaddieAI(itemNumber, FALSE);
		if (!enabled)
			return FALSE;
		item->status = FITEM_ACTIVE;
	}

	return TRUE;
}

int UpdateLOT(LOT_INFO* LOT, int expansion)
{
	if (LOT->required_box != NO_BOX && LOT->required_box != LOT->target_box)
	{
		LOT->target_box = LOT->required_box;
		if (LOT->node[LOT->required_box].next_expansion == NO_BOX && LOT->tail != LOT->required_box)
		{
			LOT->node[LOT->required_box].next_expansion = LOT->head;
			if (LOT->head == NO_BOX)
				LOT->tail = LOT->target_box;
			LOT->head = LOT->target_box;
		}
		LOT->node[LOT->required_box].search_number = ++LOT->search_number;
		LOT->node[LOT->required_box].exit_box = NO_BOX;
	}

	return SearchLOT(LOT, expansion);
}

int SearchLOT(LOT_INFO* LOT, int expansion)
{
	if (expansion <= 0)
		return TRUE;

	short* zone;
	short search_zone;
	BOX_NODE* node, *expand;
	BOX_INFO* box;
	int index, done, change, box_number;

	for (int i = 0; i < expansion; i++)
	{
		zone = ground_zone[LOT->zone][flip_status];
		search_zone = zone[LOT->head];

		if (LOT->head == NO_BOX)
		{
			LOT->tail = NO_BOX;
			return FALSE;
		}

		node = &LOT->node[LOT->head];
		box = &boxes[LOT->head];

		// expand node
		index = box->overlap_index & OVERLAP_INDEX;
		done = FALSE;
		do
		{
			box_number = overlap[index++];
			if (box_number & BOX_END_BIT)
			{
				done = TRUE;
				box_number &= BOX_NUMBER;
			}

			if (LOT->fly == NO_FLY && search_zone != zone[box_number])
				continue;

			change = boxes[box_number].height - box->height;
			if (change > LOT->step || change < LOT->drop)
				continue;

			expand = &LOT->node[box_number];
			if (node->search_number & SEARCH_NUMBER < (expand->search_number & SEARCH_NUMBER))
				continue;

			if (node->search_number & BLOCKED_SEARCH)
			{
				if ((node->search_number & SEARCH_NUMBER) == (expand->search_number & SEARCH_NUMBER))
					continue;

				expand->search_number = node->search_number;
			}
			else
			{
				if ((node->search_number & SEARCH_NUMBER) == (expand->search_number & SEARCH_NUMBER) && !(expand->search_number & BLOCKED_SEARCH))
					continue;

				if (boxes[box_number].overlap_index & LOT->block_mask)
				{
					expand->search_number = node->search_number | BLOCKED_SEARCH;
				}
				else
				{
					expand->search_number = node->search_number;
					expand->exit_box = LOT->head;
				}
			}

			if (expand->next_expansion == NO_BOX && box_number != LOT->tail)
			{
				LOT->node[LOT->tail].next_expansion = box_number;
				LOT->tail = box_number;
			}
		} while (!done);

		LOT->head = node->next_expansion;
		node->next_expansion = NO_BOX;
	}

	return TRUE;
}

void CreatureAIInfo(ITEM_INFO* item, AI_INFO* info)
{
	if (item->data == NULL)
		return;

	CREATURE_INFO* creature;
	ITEM_INFO* enemy;
	OBJECT_INFO* obj;
	ROOM_INFO* r;
	short* zone, angle, laraState;
	int x, y, z;

	creature = (CREATURE_INFO*)item->data;
	obj = &objects[item->object_number];

	enemy = creature->enemy;
	if (!enemy)
	{
		enemy = lara_item;
		creature->enemy = lara_item;
	}

	// if the item is poisoned, deal damage !
	if (item->poisoned)
	{
		if (!obj->undead && obj->hit_points != DONT_TARGET && !(wibble & 0x3F))
		{
			if (item->hit_points > 1)
				item->hit_points -= BADDIE_POISON_DAMAGE;
		}
	}

	zone = ground_zone[creature->LOT.zone][flip_status];

	// baddie
	r = &rooms[item->room_number];
	item->box_number = XZ_GET_SECTOR(r, item->pos.x, item->pos.z).box & NO_BOX;
	info->zone_number = zone[item->box_number];
	
	// enemy
	r = &rooms[enemy->room_number];
	enemy->box_number = XZ_GET_SECTOR(r, enemy->pos.x, enemy->pos.z).box & NO_BOX;
	info->enemy_zone = zone[enemy->box_number];

	// check block mask
	if (boxes[enemy->box_number].overlap_index & creature->LOT.block_mask)
		info->enemy_zone |= BLOCKED;
	else if (creature->LOT.node[item->box_number].search_number == (creature->LOT.search_number | BLOCKED_SEARCH))
		info->enemy_zone |= BLOCKED;
	
	if (enemy == lara_item)
	{
		z = (enemy->pos.z + (enemy->speed * PREDICTIVE_SCALE_FACTOR * COS(lara.move_angle) >> W2V_SHIFT)) - (item->pos.z + (obj->pivot_length * COS(item->pos.y_rot) >> W2V_SHIFT));
		x = (enemy->pos.x + (enemy->speed * PREDICTIVE_SCALE_FACTOR * SIN(lara.move_angle) >> W2V_SHIFT)) - (item->pos.x + (obj->pivot_length * SIN(item->pos.y_rot) >> W2V_SHIFT));
	}
	else
	{
		z = (enemy->pos.z + (enemy->speed * PREDICTIVE_SCALE_FACTOR * COS(enemy->pos.y_rot) >> W2V_SHIFT)) - (item->pos.z + (obj->pivot_length * COS(item->pos.y_rot) >> W2V_SHIFT));
		x = (enemy->pos.x + (enemy->speed * PREDICTIVE_SCALE_FACTOR * SIN(enemy->pos.y_rot) >> W2V_SHIFT)) - (item->pos.x + (obj->pivot_length * SIN(item->pos.y_rot) >> W2V_SHIFT));
	}

	y = item->pos.y - enemy->pos.y;
	angle = phd_atan(z, x);

	if (z > 32000 || z < -32000 || x > 32000 || x < -32000)
	{
		info->distance = 0x7FFFFFFF;
	}
	else
	{
		if (creature->enemy)
			info->distance = SQUARE(z) + SQUARE(x);
		else
			info->distance = 0x7FFFFFFF;
	}

	info->angle = angle - item->pos.y_rot;
	info->enemy_facing = 0x8000 + angle - enemy->pos.y_rot;
	
	x = abs(x);
	z = abs(z);

	if (enemy == lara_item)
	{
		laraState = lara_item->anim_state_current;
		if (laraState == 71 &&
			laraState == 72 &&
			laraState == 80 &&
			laraState == 81 &&
			laraState == 84 &&
			laraState == 85)
		{
			y -= STEPUP_HEIGHT;
		}
	}

	if (x > z)
		info->x_angle = phd_atan(x + (z >> 1), y);
	else
		info->x_angle = phd_atan(z + (x >> 1), y);

	info->ahead = (info->angle > -0x4000 && info->angle < 0x4000);
	// TODO: change STEP_L*2 to a think like item->attack_distance
	info->bite = (info->ahead && enemy->hit_points > 0 && abs(enemy->pos.y - item->pos.y) <= STEP_L*2);
}

void TargetBox(LOT_INFO* LOT, short box_number)
{
	BOX_INFO* box;

	box = &boxes[box_number & NO_BOX];

	LOT->target.z = ((box->right  - box->left - 1) >> 5) * GetRandomControl() + ((box->left << WALL_SHIFT) + WALL_L/2);
	LOT->target.x = ((box->bottom - box->top  - 1) >> 5) * GetRandomControl() + ((box->top << WALL_SHIFT)  + WALL_L/2);

	if (LOT->fly == NO_FLY)
		LOT->target.y = box->height;
	else
		LOT->target.y = box->height - STEPUP_HEIGHT;
}

BOOL EscapeBox(ITEM_INFO *item, ITEM_INFO *enemy, short box_number)
{
	BOX_INFO* box;
	int x, z;

	box = &boxes[box_number];
	x = enemy->pos.x - ((box->top + box->bottom) << 9);
	z = enemy->pos.z - ((box->left + box->right) << 9);

	if (x > -5120 && x < 5120 && z > -5120 && z < 5120)
		return FALSE;

	if ((z > 0) ^ (item->pos.z > enemy->pos.z) &&
		(x > 0) ^ (item->pos.x > enemy->pos.x))
		return FALSE;

	return TRUE;
}

BOOL ValidBox(ITEM_INFO* item, short zone_number, short box_number)
{
	if (item->data == NULL)
		return FALSE;

	CREATURE_INFO* creature;
	BOX_INFO* box;
	short* zone;
	int x, z;

	creature = (CREATURE_INFO*)item->data;
	zone = ground_zone[creature->LOT.zone][flip_status];

	if (!creature->LOT.fly && zone[box_number] != zone_number)
		return FALSE;

	box = &boxes[box_number];
	if (creature->LOT.block_mask & boxes[box_number].overlap_index)
		return FALSE;

	x = item->pos.x;
	z = item->pos.z;

	if (z > box->left << WALL_SHIFT && z < box->right << WALL_SHIFT)
		return FALSE;

	if (x > box->top << WALL_SHIFT && x < box->bottom << WALL_SHIFT)
		return FALSE;

	return TRUE;
}

BOOL StalkBox(ITEM_INFO* item, ITEM_INFO* enemy, short box_number)
{
	if (enemy == NULL)
		return FALSE;

	BOX_INFO *box;
	int x, z;
	int x_box, z_box;
	int box_quad, enemy_quad, baddie_quad;

	box = &boxes[box_number];
	x = enemy->pos.x - ((box->top + box->bottom) << 9);
	z = enemy->pos.z - ((box->right + box->left) << 9);
	x_box = (box->bottom - box->top + 3) << WALL_SHIFT;
	z_box = (box->right - box->left + 3) << WALL_SHIFT;

	if (x > x_box || x < -x_box || z > z_box || z < -z_box)
		return FALSE;

	enemy_quad = (enemy->pos.y_rot >> W2V_SHIFT) + 2;

	if (z > 0)
		box_quad = (x > 0) ? 2 : 1;
	else
		box_quad = (x > 0) ? 3 : 0;

	if (enemy_quad == box_quad)
		return FALSE;

	if (item->pos.z > enemy->pos.z)
		baddie_quad = (item->pos.x > enemy->pos.x) ? 2 : 1;
	else
		baddie_quad = (item->pos.x > enemy->pos.x) ? 3 : 0;

	if (enemy_quad == baddie_quad && abs(enemy_quad - box_quad) == 2)
		return FALSE;

	return TRUE;
}

BOOL BadFloor(int x, int y, int z, int box_height, int next_height, short roomNumber, LOT_INFO* LOT)
{
	FLOOR_INFO* floor;
	BOX_INFO* box;

	floor = GetFloor(x, y, z, &roomNumber);
	box = &boxes[(floor->box >> 4) & BOX_NUMBER];

	if ((floor->box & 0x7FF0) != 0x7FF0)
		return FALSE;

	if (LOT->is_jumping)
		return FALSE;

	if (!(box->overlap_index & LOT->block_mask))
		return FALSE;

	if (box->height - box_height > LOT->drop || box_height - box->height < LOT->step)
		return FALSE;

	if (box_height - box->height > -LOT->step && box->height < next_height)
		return FALSE;

	if (LOT->fly == NO_FLY && y < box->height + LOT->fly)
		return FALSE;

	return TRUE;
}

void CreatureDie(short itemNumber, BOOL explode)
{
	ITEM_INFO* item, *pickup;
	FLOOR_INFO* floor;
	OBJECT_INFO* obj;
	short room_number, pickup_number;

	item = &items[itemNumber];
	item->hit_points = DONT_TARGET;
	item->collidable = false;

	if (explode)
	{
		obj = &objects[item->object_number];
		if (obj->hit_effect == HIT_BLOOD)
			ExplodingDeath(itemNumber, NO_ITEM, 258); // enable blood sprite in body mesh
		else
			ExplodingDeath(itemNumber, NO_ITEM, 256); // no blood when exploding
		KillItem(itemNumber);
	}
	else
	{
		RemoveActiveItem(itemNumber);
	}
	
	DisableBaddieAI(itemNumber);
	item->flags |= (IFLAG_KILLED_ITEM | IFLAG_INVISIBLE);

	SpawnPickup(item);
}

BOOL SameZone(CREATURE_INFO *creature, ITEM_INFO *target_item)
{
	ITEM_INFO* item;
	ROOM_INFO* r;
	short* zone;
	
	item = &items[creature->item_number];
	zone = ground_zone[creature->LOT.zone][flip_status];

	// src
	r = &rooms[item->room_number];
	item->box_number = XZ_GET_SECTOR(r, item->pos.x, item->pos.z).box & NO_BOX;

	// target
	r = &rooms[target_item->room_number];
	target_item->box_number = XZ_GET_SECTOR(r, target_item->pos.x, target_item->pos.z).box & NO_BOX;

	// return true if the src and target are in the same zone !
	return (zone[item->box_number] == zone[target_item->box_number]);
}

void injector::inject_box()
{
	this->inject(0x0043FB30, InitialiseCreature);
	this->inject(0x0043FB70, CreatureActive);
	this->inject(0x0043FBE0, CreatureAIInfo);
	this->inject(0x0043FF70, UpdateLOT);
	this->inject(0x0043FFF0, SearchLOT);
	this->inject(0x004401F0, TargetBox);
	this->inject(0x00440290, EscapeBox);
	this->inject(0x00440340, ValidBox);
	//this->inject(0x004403E0, GetCreatureMood);
	//this->inject(0x00440620, CreatureMood);
	this->inject(0x00440940, StalkBox);
	//this->inject(0x00440A40, CalculateTarget);
	//this->inject(0x00440E90, CreatureCreature);
	this->inject(0x00440FD0, BadFloor);
	this->inject(0x00441080, CreatureDie);
	//this->inject(0x00441230, CreatureAnimation);
	//this->inject(0x00441C60, CreatureTurn);
	//this->inject(0x00441EE0, CreatureTilt);
	//this->inject(0x00441F20, CreatureJoint);
	//this->inject(0x00441F80, CreatureFloat);
	//this->inject(0x00442080, CreatureUnderwater);
	//this->inject(0x00442110, CreatureEffect);
	//this->inject(0x00442170, CreatureEffectAlternate);
	//this->inject(0x004421D0, CreatureVault);
	//this->inject(0x00442380, CreatureKill);
	//this->inject(0x00442550, AlertAllGuards);
	//this->inject(0x004425D0, AlertNearbyGuards);
	//this->inject(0x00442670, AIGuard);
	//this->inject(0x00442720, GetAITarget);
	//this->inject(0x00442A60, FindAITargetObject);
	this->inject(0x00442C40, SameZone);
}