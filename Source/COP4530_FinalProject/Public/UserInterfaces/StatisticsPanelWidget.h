#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatisticsPanelWidget.generated.h"

class UTextBlock;
class AGrid;

/**
 * 
 */
UCLASS()
class COP4530_FINALPROJECT_API UStatisticsPanelWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	AGrid* Grid;

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	FVector2D GridLocation;

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	int32 NumberOfNodes;

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	float ExecuteTime;

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	bool bIsAValidDestination;

public:

	UPROPERTY(meta=(BindWidget))
	UTextBlock* NodesVisited;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* OptimalPathLength;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ExecutionTime;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* GridSize;

	FORCEINLINE AGrid* GetGrid() const { return Grid; };
	FORCEINLINE void SetGrid(AGrid* NewGrid) { Grid = NewGrid; }
	FORCEINLINE FVector2D GetGridLocation() const { return GridLocation; };
	FORCEINLINE void SetGridLocation(const FVector2D NewGridLocation) { GridLocation = NewGridLocation; }
	FORCEINLINE int32 GetNumberOfNodes() const { return NumberOfNodes; };
	FORCEINLINE void SetNumberOfNodes(const int32 NewNumberOfNodes) { NumberOfNodes = NewNumberOfNodes; }
	FORCEINLINE float GetExecuteTime() const { return ExecuteTime; };
	FORCEINLINE void SetExecuteTime(const float NewExecuteTime) { ExecuteTime = NewExecuteTime; }
	FORCEINLINE bool IsAValidDestination() const { return bIsAValidDestination; };
	FORCEINLINE void SetIsAValidDestination(const bool IsAValidDestination) { bIsAValidDestination = IsAValidDestination; }

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
