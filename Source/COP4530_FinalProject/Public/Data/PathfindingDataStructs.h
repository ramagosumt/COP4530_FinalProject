/** Creates an user-defined data table for the pathfinding algorithms
*	Uses enums for item classifications
 *	Uses structs for item variables
 *	Uses one last struct to group the enums and structs into one item
 */

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseObstacle.h"
#include "PathfindingDataStructs.generated.h"

class AGridTile;

USTRUCT()
struct FPathfindingData
{
	GENERATED_BODY()

	UPROPERTY()
	AGridTile* TileActor;
	
	EGroundTypes GroundType;
	FVector WorldLocation;
	FVector2D GridIndex;
	int32 FinalCost;
	int32 CostFromStart;
	int32 EstimatedCostToTarget;
	int32 TileCost;
	FVector2D PreviousTile;
};
