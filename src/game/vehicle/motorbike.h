#pragma once

#include "vehicle_utils.h"

extern void InitialiseMotorBike(short item_number);
extern void MotorBikeCollision(short item_number, ITEM_INFO* laraitem, COLL_INFO* coll);
extern int MotorBikeControl(void);
extern void DrawMotorBikeEffect(ITEM_INFO* item);
