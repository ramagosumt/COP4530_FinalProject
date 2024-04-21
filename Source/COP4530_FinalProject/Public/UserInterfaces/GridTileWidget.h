#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GridTileWidget.generated.h"

class UTextBlock;
class AGrid;

/**
 * 
 */
UCLASS()
class COP4530_FINALPROJECT_API UGridTileWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	AGrid* Grid;

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	FVector2D GridIndex;

	FTimerHandle TimerHandle;

public:

	UPROPERTY(meta=(BindWidget))
	UTextBlock* FinalCost;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* IndividualCost;

	FORCEINLINE AGrid* GetGrid() const { return Grid; };
	FORCEINLINE void SetGrid(AGrid* NewGrid) { Grid = NewGrid; } 
	FORCEINLINE FVector2D GetGridIndex() const { return GridIndex; };
	FORCEINLINE void SetGridIndex(const FVector2D NewGridIndex) { GridIndex = NewGridIndex; }

protected:
	
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetCost() const;
	void SetFinalCost() const;
	void SetIndividualCost() const;
};
