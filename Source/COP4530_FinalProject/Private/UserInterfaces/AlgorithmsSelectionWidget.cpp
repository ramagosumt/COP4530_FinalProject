#include "UserInterfaces/AlgorithmsSelectionWidget.h"
#include "Components/Button.h"
#include "Actors/Grid.h"
#include "UserInterfaces/GridTileTooltipWidget.h"

void UAlgorithmsSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DepthFirstSearch->OnClicked.AddUniqueDynamic(this, &UAlgorithmsSelectionWidget::SetModeDepthFirstSearch);
	BreadthFirstSearch->OnClicked.AddUniqueDynamic(this, &UAlgorithmsSelectionWidget::SetModeBreadthFirstSearch);
	Dijkstra->OnClicked.AddUniqueDynamic(this, &UAlgorithmsSelectionWidget::SetModeDijkstra);
	AStar->OnClicked.AddUniqueDynamic(this, &UAlgorithmsSelectionWidget::SetModeAStar);
}

void UAlgorithmsSelectionWidget::SetModeDepthFirstSearch()
{
	if (Grid)
	{
		Grid->GameMode = EGameMode::EGM_DFS;
		GetGrid()->GetGridTileTooltipWidget()->SetGameMode(EGameMode::EGM_DFS);
		Grid->ResetGrid();
	}
}

void UAlgorithmsSelectionWidget::SetModeBreadthFirstSearch()
{
	if (Grid)
	{
		Grid->GameMode = EGameMode::EGM_BFS;
		GetGrid()->GetGridTileTooltipWidget()->SetGameMode(EGameMode::EGM_BFS);
		Grid->ResetGrid();
	}
}

void UAlgorithmsSelectionWidget::SetModeDijkstra()
{
	if (Grid)
	{
		Grid->GameMode = EGameMode::EGM_Dijkstra;
		GetGrid()->GetGridTileTooltipWidget()->SetGameMode(EGameMode::EGM_Dijkstra);
		Grid->ResetGrid();
	}
}

void UAlgorithmsSelectionWidget::SetModeAStar()
{
	if (Grid)
	{
		Grid->GameMode = EGameMode::EGM_AStar;
		GetGrid()->GetGridTileTooltipWidget()->SetGameMode(EGameMode::EGM_AStar);
		Grid->ResetGrid();
	}
}
