#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

class UBillboardComponent;

UCLASS()
class COP4530_FINALPROJECT_API AGrid : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY()
	UBillboardComponent* BillboardComponent;
	
public:

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	FVector GridLocation; // Map Center Location

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	FVector2D GridSizeWorld; // Map Size

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	float TileSize; // Tile Size

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	int32 GridTileNumberX; // Number of Horizontal Tiles

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	int32 GridTileNumberY; // Number of Vertical Tiles

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	FVector TilePosition; // Position of Tiles

	UPROPERTY(EditInstanceOnly, Category= "Debug")
	FColor GridBoxColor;

	UPROPERTY(EditInstanceOnly, Category= "Debug")
	FColor GridBottomLeftColor;
	
	AGrid();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
	
	FORCEINLINE FVector GetGridLocation() const { return GridLocation;};
	FORCEINLINE void SetGridLocation(const FVector NewGridLocation) { GridLocation = NewGridLocation; } 

protected:
	
	virtual void BeginPlay() override;

	FVector GridBottomLeft() const; // Calculate the Position of BottomLeft
	void GridTileNumber(int32& X, int32& Y) const; // Calculate the Amount of Tiles

	void DrawTile(); // Draw the Tiles
};
