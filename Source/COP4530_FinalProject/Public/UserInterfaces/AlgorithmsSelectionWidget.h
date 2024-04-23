#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AlgorithmsSelectionWidget.generated.h"

class AGrid;
class UButton;

/**
 * 
 */
UCLASS()
class COP4530_FINALPROJECT_API UAlgorithmsSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	AGrid* Grid;

public:

	UPROPERTY(meta=(BindWidget))
	UButton* DepthFirstSearch;

	UPROPERTY(meta=(BindWidget))
	UButton* BreadthFirstSearch;

	UPROPERTY(meta=(BindWidget))
	UButton* Dijkstra;

	UPROPERTY(meta=(BindWidget))
	UButton* AStar;

	FORCEINLINE AGrid* GetGrid() const { return Grid; };
	FORCEINLINE void SetGrid(AGrid* NewGrid) { Grid = NewGrid; }

protected:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetModeDepthFirstSearch();

	UFUNCTION()
	void SetModeBreadthFirstSearch();

	UFUNCTION()
	void SetModeDijkstra();

	UFUNCTION()
	void SetModeAStar();
};
