#include "UserInterfaces/StatisticsPanelWidget.h"
#include "Actors/Grid.h"
#include "Components/TextBlock.h"

void UStatisticsPanelWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	NodesVisited->SetText(FText::FromString("- Nodes Visited: 0 (nodes)"));
	OptimalPathLength->SetText(FText::FromString("- Optimal Path Length: 999 (steps)"));
	ExecutionTime->SetText(FText::FromString("- Execution Time: 0.0 (ms)"));
	GridSize->SetText(FText::FromString("- GridSize: 0 x 0 (tiles)"));
}

void UStatisticsPanelWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(Grid)) NodesVisited->SetText(FText::Format(FText::FromString("- Nodes Visited: {0} (nodes)"), FText::AsNumber(NumberOfNodes)));

	if (IsValid(Grid)) if (IsAValidDestination()) OptimalPathLength->SetText(FText::Format(FText::FromString("- Optimal Path Length: {0} (steps)"), FText::AsNumber(Grid->PathToTarget.Num())));
		else OptimalPathLength->SetText(FText::FromString("- Optimal Path Length: NaN"));

	if (IsValid(Grid)) ExecutionTime->SetText(FText::Format(FText::FromString("- Execution Time: {0} (ms)"), FText::FromString(FString::SanitizeFloat(ExecuteTime))));

	if (IsValid(Grid)) GridSize->SetText(FText::Format(FText::FromString("- Grid Size: {0} x {1} (tiles)"), FText::AsNumber(Grid->GridTileNumberX), FText::AsNumber(Grid->GridTileNumberY)));
}
