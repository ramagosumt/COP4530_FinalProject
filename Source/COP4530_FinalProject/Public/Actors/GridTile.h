#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTile.generated.h"

class AGrid;

UCLASS()
class COP4530_FINALPROJECT_API AGridTile : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;

	FTimerHandle TimerHandle;
	
public:
	
	AGridTile();

	virtual void Tick(float DeltaTime) override;

	void SelectTile();
	void DeselectTile();

	FORCEINLINE AGrid* GetGrid() const { return Grid; };
	FORCEINLINE void SetGrid(AGrid* NewGrid) { Grid = NewGrid; } 
	FORCEINLINE FVector2D GetGridIndex() const { return GridIndex; };
	FORCEINLINE void SetGridIndex(const FVector2D NewGridIndex) { GridIndex = NewGridIndex; }
	FORCEINLINE bool GetIsHovered() const { return bIsHovered; };
	FORCEINLINE void SetIsHovered(const bool IsHovered) { bIsHovered = IsHovered; }
	FORCEINLINE bool GetIsSelected() const { return bIsSelected; };
	FORCEINLINE void SetIsSelected(const bool IsSelected) { bIsSelected = IsSelected; } 

protected:

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	AGrid* Grid;

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	FVector2D GridIndex;

	bool bIsHovered;
	bool bIsSelected;
	
	virtual void BeginPlay() override;

	void SetTile() const;
	void SetTileColor() const;
	void SetTileSize() const;
	
	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;
	
};
