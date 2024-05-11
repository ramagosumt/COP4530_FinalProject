#include "Actors/GridTile.h"
#include "Actors/Grid.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UserInterfaces/GridTileWidget.h"

AGridTile::AGridTile()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a SceneComponent and set it as the RootComponent
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	// Create a StaticMeshComponent and attach it to RootComponent
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Assets/Grid/SM_GridTile_01")).Object);

	// Create a WidgetComponent and attach it to RootComponent
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetWindowFocusable(false);
	WidgetComponent->SetGenerateOverlapEvents(false);
	WidgetComponent->SetCollisionProfileName(FName("NoCollision"));
	WidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.1f));
	WidgetComponent->SetRelativeRotation(FRotator(90.f, 540.f, 720.f));
	WidgetComponent->SetWidgetClass(ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("/Game/UserInterfaces/WBP_GridTileWidget")).Class);
}

void AGridTile::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGridTile::SetTile, 0.02f, false);
	
}

void AGridTile::SetTile()
{
	SetTileColor();
	SetTileSize();
	SetTileWidget();
}

void AGridTile::SetTileColor() const
{
	FColor TileColor;
	
	switch (Grid->GetPathfindingMap().Find(GridIndex)->GroundType)
	{
	case EGroundTypes::EGT_Normal:
		TileColor = FColor::FromHex(FString("808080"));
		break;
	case EGroundTypes::EGT_Difficult:
		TileColor = FColor::Turquoise;
		break;
	case EGroundTypes::EGT_ReallyDifficult:
		TileColor = FColor::FromHex(FString("FF8503"));
		break;
	case EGroundTypes::EGT_Impossible:
		TileColor = FColor::FromHex(FString("8B0000"));
		break;
	case EGroundTypes::EGT_None:
		TileColor = FColor::Blue;
		break;
	}

	// If selected, brighten up the tile
	if (bIsSelected) StaticMeshComponent->SetScalarParameterValueOnMaterials(FName("bIsSelected"), 1.f);
		else StaticMeshComponent->SetScalarParameterValueOnMaterials(FName("bIsSelected"), 0.f);

	// If hovered, brighten up the tile
	if (bIsHovered) TileColor = UKismetMathLibrary::Conv_LinearColorToColor(UKismetMathLibrary::Conv_VectorToLinearColor(3 * UKismetMathLibrary::Conv_LinearColorToVector(UKismetMathLibrary::Conv_ColorToLinearColor(TileColor))));

	if (bIsExploring) TileColor = FColor::Yellow;
	
	if (bIsInPath) TileColor = FColor::Purple;
	
	StaticMeshComponent->SetVectorParameterValueOnMaterials(FName("TileColor"), UKismetMathLibrary::Conv_LinearColorToVector(UKismetMathLibrary::Conv_ColorToLinearColor(TileColor)));
}

void AGridTile::SetTileSize() const
{
	const FVector NewScale = this->GetActorScale() * Grid->GetTileSize() / 50.f;
	StaticMeshComponent->SetWorldScale3D(FVector(NewScale.X, NewScale.Y, 1.f));
}

void AGridTile::SetTileWidget()
{
	GridTileWidget = Cast<UGridTileWidget>(WidgetComponent->GetUserWidgetObject());
	GridTileWidget->SetGrid(GetGrid());
	GridTileWidget->SetGridIndex(GetGridIndex());
}

void AGridTile::SelectTile()
{
	SetIsSelected(true);
	SetTileColor();
}

void AGridTile::DeselectTile()
{
	SetIsSelected(false);
	SetTileColor();
}

void AGridTile::HoverTile()
{
	SetIsHovered(true);
	SetTileColor();
}

void AGridTile::DehoverTile()
{
	SetIsHovered(false);
	SetTileColor();
}

void AGridTile::IsInPath(const bool bInPath)
{
	bIsInPath = bInPath;
	SetTileColor();
	ResetTileState();
}

void AGridTile::IsExploring(const bool bExploring)
{
	bIsExploring = bExploring;
	SetTileColor();
	ResetTileState();
}

void AGridTile::DebugOpenSet() const
{
	StaticMeshComponent->SetVectorParameterValueOnMaterials(FName("TileColor"), UKismetMathLibrary::Conv_LinearColorToVector(UKismetMathLibrary::Conv_ColorToLinearColor(FColor::Cyan)));
}

void AGridTile::DebugClosedSet() const
{
	StaticMeshComponent->SetVectorParameterValueOnMaterials(FName("TileColor"), UKismetMathLibrary::Conv_LinearColorToVector(UKismetMathLibrary::Conv_ColorToLinearColor(FColor::Blue)));
}

void AGridTile::ResetTileState() const
{
	GridTileWidget->SetTileState();
}

void AGridTile::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();

	HoverTile();
	Grid->HoverNewTile(this);
}

void AGridTile::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();

	DehoverTile();
}

void AGridTile::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

	SelectTile();
	Grid->SelectNewTile(this);
}
