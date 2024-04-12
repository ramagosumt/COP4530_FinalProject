#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/PathfindingDataStructs.h"
#include "Grid.generated.h"

/** Defines */
#define PFC_Ground ECC_GameTraceChannel1 // User-defined collision channel for Grounds
#define PFC_Obstacle ECC_GameTraceChannel2 // User-defined collision channel for Obstacles

enum class EGroundTypes : uint8;

class UBillboardComponent;

struct FPathfindingData;

UCLASS()
class COP4530_FINALPROJECT_API AGrid : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* BillboardComponent;
	
public:

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	FVector GridLocation; // Map Center Location

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	FVector2D GridSizeWorld; // Map Size

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	float TileSize; // Tile Size

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	float TileSizeOffset; // Offset of Tiles

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	int32 GridTileNumberX; // Number of Horizontal Tiles

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	int32 GridTileNumberY; // Number of Vertical Tiles

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	FVector TilePosition; // Position of Tiles

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	FColor TileColor; // Color of Tiles

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	TMap<FVector2D, FPathfindingData> PathfindingMap;

	UPROPERTY(EditInstanceOnly, Category= "Debug")
	FColor GridBoxColor;

	UPROPERTY(EditInstanceOnly, Category= "Debug")
	FColor GridBottomLeftColor;
	
	AGrid();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
	
	FORCEINLINE FVector GetGridLocation() const { return GridLocation; };
	FORCEINLINE void SetGridLocation(const FVector NewGridLocation) { GridLocation = NewGridLocation; }
	FORCEINLINE TMap<FVector2D, FPathfindingData> GetPathfindingMap() const { return PathfindingMap; };
	FORCEINLINE void SetPathfindingMap(const TMap<FVector2D, FPathfindingData>& NewPathfindingMap) { PathfindingMap = NewPathfindingMap; } 

protected:
	
	virtual void BeginPlay() override;

	FVector GridBottomLeft() const; // Calculate the Position of BottomLeft
	void GridTileNumber(int32& X, int32& Y) const; // Calculate the Amount of Tiles
	void DrawTile(); // Draw the Tiles
	bool TraceSphere(ETraceTypeQuery TraceType, FHitResult& HitActor) const; // Trace the Spheres
	void DebugGroundType(EGroundTypes GroundType, FVector GroundLocation);
	void GenerateMapDataFromWorld();
	void SpawnTiles(const bool SpawnNone);
};
