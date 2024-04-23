#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GridTileTooltipWidget.generated.h"

enum class EGameMode : uint8;

class UTextBlock;
class AGrid;

/**
 * 
 */
UCLASS()
class COP4530_FINALPROJECT_API UGridTileTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	AGrid* Grid;

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	EGameMode GameMode;

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	FVector2D StartLocation;

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	FVector2D GridLocation;

public:

	UPROPERTY(meta=(BindWidget))
	UTextBlock* GameModeBlock;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* StartIndex;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* GridIndex;

	FORCEINLINE AGrid* GetGrid() const { return Grid; };
	FORCEINLINE void SetGrid(AGrid* NewGrid) { Grid = NewGrid; }
	FORCEINLINE EGameMode GetGameMode() const { return GameMode; };
	FORCEINLINE void SetGameMode(const EGameMode NewGameMode) { GameMode = NewGameMode; }
	FORCEINLINE FVector2D GetStartLocation() const { return StartLocation; };
	FORCEINLINE void SetStartLocation(const FVector2D NewStartLocation) { StartLocation = NewStartLocation; }
	FORCEINLINE FVector2D GetGridLocation() const { return GridLocation; };
	FORCEINLINE void SetGridLocation(const FVector2D NewGridLocation) { GridLocation = NewGridLocation; }

protected:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
