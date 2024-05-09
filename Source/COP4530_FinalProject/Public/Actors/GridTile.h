#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTile.generated.h"

class AGrid;
class UWidgetComponent;
class UGridTileWidget;

UCLASS()
class COP4530_FINALPROJECT_API AGridTile : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* WidgetComponent;

	FTimerHandle TimerHandle;
	
public:

	UPROPERTY(EditAnywhere, Category= "Widget")
	TSubclassOf<UGridTileWidget> DesiredGridTile;
	
	AGridTile();

	void SetTileColor() const;
	void SelectTile();
	void DeselectTile();
	void HoverTile();
	void DehoverTile();
	void IsInPath(bool bInPath);
	void IsExploring (bool bExploring);
	void DebugOpenSet() const;
	void DebugClosedSet() const;

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
	bool bIsInPath;
	bool bIsExploring;
	
	virtual void BeginPlay() override;

	void SetTile() const;
	void SetTileSize() const;
	void SetTileWidget() const;
	
	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;
	
};
