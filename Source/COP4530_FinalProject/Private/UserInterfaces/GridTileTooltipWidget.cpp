#include "UserInterfaces/GridTileTooltipWidget.h"
#include "Actors/Grid.h"
#include "Components/TextBlock.h"

void UGridTileTooltipWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Grid)
	{
		switch (GameMode)
		{
		case EGameMode::EGM_DFS:
			GameModeBlock->SetText(FText::FromString("DFS"));
			break;
		case EGameMode::EGM_BFS:
			GameModeBlock->SetText(FText::FromString("BFS"));
			break;
		case EGameMode::EGM_Djikstra:
			GameModeBlock->SetText(FText::FromString("Dijkstra's"));
			break;
		case EGameMode::EGM_AStar:
			GameModeBlock->SetText(FText::FromString("A*"));
			break;
		case EGameMode::EGM_None:
			break;
		}

		StartIndex->SetText(FText::Format(FText::FromString("Start Location: {0}, {1}"), FText::AsNumber(StartLocation.X), FText::AsNumber(StartLocation.Y)));
		GridIndex->SetText(FText::Format(FText::FromString("Grid Location: {0}, {1}"), FText::AsNumber(GridLocation.X), FText::AsNumber(GridLocation.Y)));
	}
}
