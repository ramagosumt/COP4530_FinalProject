#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InformationBoardWidget.generated.h"

enum class EGameMode : uint8;

class UTextBlock;
class AGrid;

/**
 * 
 */
UCLASS()
class COP4530_FINALPROJECT_API UInformationBoardWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	AGrid* Grid;

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	FVector2D GridIndex;

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	EGameMode GameMode;

	FTimerHandle TimerHandle;

public:

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ApplicationBoard;

	FORCEINLINE AGrid* GetGrid() const { return Grid; };
	FORCEINLINE void SetGrid(AGrid* NewGrid) { Grid = NewGrid; }
	FORCEINLINE FVector2D GetGridIndex() const { return GridIndex; };
	FORCEINLINE void SetGridIndex(const FVector2D NewGridIndex) { GridIndex = NewGridIndex; }
	FORCEINLINE EGameMode GetGameMode() const { return GameMode; };
	FORCEINLINE void SetGameMode(const EGameMode NewGameMode) { GameMode = NewGameMode; }

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetInformation() const;
	
};
