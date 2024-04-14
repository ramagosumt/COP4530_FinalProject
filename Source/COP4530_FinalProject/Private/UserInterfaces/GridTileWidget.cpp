#include "UserInterfaces/GridTileWidget.h"
#include "Actors/Grid.h"
#include "Components/TextBlock.h"

void UGridTileWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
}

void UGridTileWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UGridTileWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGridTileWidget::SetCost, 0.1f, false);
}

void UGridTileWidget::SetCost() const
{
	SetFinalCost();
	SetIndividualCost();
}

void UGridTileWidget::SetFinalCost() const
{
	if (IsValid(Grid)) FinalCost->SetText(FText::FromString(FString::FromInt(Grid->PathfindingMap.Find(GridIndex)->FinalCost)));
		else FinalCost->SetText(FText(FText::FromString("0")));
}

void UGridTileWidget::SetIndividualCost() const
{
	if (IsValid(Grid)) IndividualCost->SetText(FText::FromString(FString::FromInt(Grid->CalculateTileCost(GridIndex))));
		else IndividualCost->SetText(FText(FText::FromString("0")));
}
