#include "UserInterfaces/GridTileWidget.h"
#include "Actors/Grid.h"
#include "Components/TextBlock.h"

void UGridTileWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGridTileWidget::SetCost, 0.03f, false);
}

void UGridTileWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(Grid)) if (Grid->PathfindingMap.Find(GridIndex)->CostFromStart != 999.f) FinalCost->SetText(FText::FromString(FString::FromInt(Grid->PathfindingMap.Find(GridIndex)->CostFromStart)));
			else FinalCost->SetText(FText(FText::FromString(" ")));
		else FinalCost->SetText(FText(FText::FromString("0")));
}

void UGridTileWidget::SetCost() const
{
	SetFinalCost();
	SetIndividualCost();
	
	if (IsValid(Grid)) TileState->SetText(FText::FromString("undiscovered"));
}

void UGridTileWidget::SetFinalCost() const
{
	if (IsValid(Grid)) if (Grid->PathfindingMap.Find(GridIndex)->FinalCost != 999.f) FinalCost->SetText(FText::FromString(FString::FromInt(Grid->PathfindingMap.Find(GridIndex)->FinalCost)));
			else FinalCost->SetText(FText(FText::FromString("0")));
		else FinalCost->SetText(FText(FText::FromString("0")));
}

void UGridTileWidget::SetIndividualCost() const
{
	if (IsValid(Grid)) IndividualCost->SetText(FText::FromString(FString::FromInt(Grid->PathfindingMap.Find(GridIndex)->TileCost)));
		else IndividualCost->SetText(FText(FText::FromString(" ")));
}

void UGridTileWidget::SetTileState() const
{
	if (IsValid(Grid))
	{
		if (Grid->PathfindingMap.Find(GridIndex)->bVisited)
		{
			TileState->SetText(FText::FromString("discovered"));
			TileState->SetColorAndOpacity(FSlateColor(FLinearColor::Blue));
		}
		else
		{
			TileState->SetText(FText::FromString("undiscovered"));
			TileState->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
		}
	}
	else TileState->SetText(FText::FromString("undiscovered"));
}