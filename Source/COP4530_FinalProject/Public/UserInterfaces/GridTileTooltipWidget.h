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
	FVector2D TargetLocation;

public:

	UPROPERTY(meta=(BindWidget))
	UTextBlock* GameModeBlock;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* StartIndex;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TargetIndex;

	FORCEINLINE AGrid* GetGrid() const { return Grid; };
	FORCEINLINE void SetGrid(AGrid* NewGrid) { Grid = NewGrid; }
	FORCEINLINE EGameMode GetGameMode() const { return GameMode; };
	FORCEINLINE void SetGameMode(const EGameMode NewGameMode) { GameMode = NewGameMode; }
	FORCEINLINE FVector2D GetStartLocation() const { return StartLocation; };
	FORCEINLINE void SetStartLocation(const FVector2D NewStartLocation) { StartLocation = NewStartLocation; }
	FORCEINLINE FVector2D GetTargetLocation() const { return TargetLocation; };
	FORCEINLINE void SetTargetLocation(const FVector2D NewTargetLocation) { TargetLocation = NewTargetLocation; }

protected:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
