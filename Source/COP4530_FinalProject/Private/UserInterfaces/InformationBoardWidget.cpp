#include "UserInterfaces/InformationBoardWidget.h"
#include "Actors/Grid.h"
#include "Components/TextBlock.h"

void UInformationBoardWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInformationBoardWidget::SetInformation, 0.03f, false);
}

void UInformationBoardWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Grid)
	{
		switch (GameMode)
		{
		case EGameMode::EGM_DFS:
			break;
		case EGameMode::EGM_BFS:
			ApplicationBoard->SetText(FText::FromString(
				"- Solve games where a series of choices result in either winning or losing state\n\n"
				"- Dijkstra's Algorithm: Computing the shortest path in a graph\n\n"
				"- Ford-Fulkerson Algorithm: Computing the maximum flow in a network\n\n"
				));
			break;
		case EGameMode::EGM_Dijkstra:
			break;
		case EGameMode::EGM_AStar:
			break;
		default:
			break;
		}
	}
}

void UInformationBoardWidget::SetInformation() const
{
	if (Grid)
	{
		switch (GameMode)
		{
		case EGameMode::EGM_DFS:
			UE_LOG(LogTemp, Warning, TEXT("Duh."))
			break;
		case EGameMode::EGM_BFS:
			ApplicationBoard->SetText(FText::FromString(
				"- Solve games where a series of choices result in either winning or losing state\n\n"
				"- Dijkstra's Algorithm: Computing the shortest path in a graph\n\n"
				"- Ford-Fulkerson Algorithm: Computing the maximum flow in a network\n\n"
				));
			break;
		case EGameMode::EGM_Dijkstra:
			break;
		case EGameMode::EGM_AStar:
			break;
		default:
			break;
		}
	}
}
