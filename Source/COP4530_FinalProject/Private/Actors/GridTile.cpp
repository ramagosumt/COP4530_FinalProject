#include "Actors/GridTile.h"
#include "Actors/Grid.h"
#include "Kismet/KismetMathLibrary.h"

AGridTile::AGridTile()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a SceneComponent and set it as the RootComponent
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	// Create a StaticMeshComponent and attach it to RootComponent
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	UStaticMesh* TileMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Assets/Grid/SM_GridTile_01")).Object;
	StaticMeshComponent->SetStaticMesh(TileMesh);
}

void AGridTile::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGridTile::SetTileColor, 0.2f, false);
}

void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridTile::SetTileColor() const
{
	const FPathfindingData* CurrentData = Grid->GetPathfindingMap().Find(GridIndex);
	
	FColor TileColor;
	
	switch (CurrentData->GroundType)
	{
	case EGroundTypes::EGT_Normal:
		TileColor = FColor::Silver;
		break;
	case EGroundTypes::EGT_Difficult:
		TileColor = FColor::Turquoise;
		break;
	case EGroundTypes::EGT_ReallyDifficult:
		TileColor = FColor::Orange;
		break;
	case EGroundTypes::EGT_Impossible:
		TileColor = FColor::Purple;
		break;
	case EGroundTypes::EGT_None:
		TileColor = FColor::Blue;
		break;
	}

	if (bIsSelected) TileColor = UKismetMathLibrary::Conv_LinearColorToColor(UKismetMathLibrary::Conv_VectorToLinearColor(3 * UKismetMathLibrary::Conv_LinearColorToVector(UKismetMathLibrary::Conv_ColorToLinearColor(TileColor))));

	StaticMeshComponent->SetVectorParameterValueOnMaterials(FName("TileColor"), UKismetMathLibrary::Conv_LinearColorToVector(UKismetMathLibrary::Conv_ColorToLinearColor(TileColor)));
}

void AGridTile::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();

	SetIsSelected(true);
	
	SetTileColor();
}

void AGridTile::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();

	SetIsSelected(false);

	SetTileColor();
}
