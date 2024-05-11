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
			ApplicationBoard->SetText(FText::FromString(
				"- Topological Sorting, Scheduling Problems, Cycle Detection in Graphs\n\n"
				"- Solving Puzzles with Only one Solution (Maze, Sudoku)\n\n"
				"- Analyzing Networks (Testing if a Graph is bipartite)\n\n"
				"- Subroutine in Network Flow Algorithms (Ford-Fulkerson Algorithm)\n\n"
				"- Subroutine in Matching Algorithms in Graph Theory (Hopcroft-Karp Algorithm)\n\n"
				"- Mapping Routes, Scheduling, and Finding Spanning Trees\n\n"
			));
			break;
		case EGameMode::EGM_BFS:
			ApplicationBoard->SetText(FText::FromString(
				"- Solve games where a series of choices result in either winning or losing state (Rubik)\n\n"
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
			ApplicationBoard->SetText(FText::FromString(
				"- Topological Sorting, Scheduling Problems, Cycle Detection in Graphs\n\n"
				"- Solving Puzzles with Only one Solution (Maze, Sudoku)\n\n"
				"- Analyzing Networks (Testing if a Graph is bipartite)\n\n"
				"- Subroutine in Network Flow Algorithms (Ford-Fulkerson Algorithm)\n\n"
				"- Subroutine in Matching Algorithms in Graph Theory (Hopcroft-Karp Algorithm)\n\n"
				"- Mapping Routes, Scheduling, and Finding Spanning Trees\n\n"
			));
			break;
		case EGameMode::EGM_BFS:
			ApplicationBoard->SetText(FText::FromString(
				"- Solve games where a series of choices result in either winning or losing state (Rubik)\n\n"
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
