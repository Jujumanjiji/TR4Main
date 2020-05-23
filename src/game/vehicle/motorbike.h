#pragma once

#include "vehicle_utils.h"

extern void InitialiseMotorBike(short itemNumber);
extern void MotorBikeCollision(short itemNumber, ITEM_INFO* laraitem, COLL_INFO* coll);
extern int MotorBikeControl(void);
extern void DrawMotorBikeEffect(ITEM_INFO* item);
