#include "Actors/Grid.h"
#include "Actors/BaseObstacle.h"
#include "Actors/GridTile.h"
#include "Components/BillboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Data/PathfindingDataStructs.h"
#include "Kismet/KismetSystemLibrary.h"

AGrid::AGrid()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a SceneComponent and set it as the RootComponent
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	// Create a BillboardComponent and attach it to RootComponent
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	BillboardComponent->SetupAttachment(GetRootComponent());
	BillboardComponent->SetWorldLocation(FVector(0.f, -700.f, 0.f));
	BillboardComponent->EditorScale = .25f;

	// Initialize Variables
	GridLocation = FVector(0.f);
	GridSizeWorld = FVector2D(500.f);
	TileSize = 50.f;
	TileSizeOffset = 5.f;
	GridTileNumberX = 0;
	GridTileNumberY = 0;
	GridBoxColor = FColor::Blue;
	GridBottomLeftColor = FColor::Purple;
}

void AGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FlushPersistentDebugLines(GetWorld());
	
	SetGridLocation(SceneComponent->GetComponentLocation());
	
	DrawDebugBox(GetWorld(), GridLocation, FVector(GridSizeWorld.Y, GridSizeWorld.X, 5.f), GridBoxColor, true, 1000000.f, 0.f, 10.f);
	DrawDebugBox(GetWorld(), GridBottomLeft(), FVector(20.f), GridBottomLeftColor, true, 1000000.f, 0.f, 10.f);

	GridTileNumber(GridTileNumberX, GridTileNumberY);

	GenerateMapDataFromWorld();

	DrawTile();
	
}

void AGrid::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	GetWorld()->GetFirstPlayerController()->bEnableMouseOverEvents = true;

	FlushPersistentDebugLines(GetWorld());

	SetGridLocation(SceneComponent->GetComponentLocation());

	GenerateMapDataFromWorld();

	SpawnTiles(true);
}

void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AGrid::GridBottomLeft() const
{
	// Movement = Move Direction * Move Length
	const FVector MoveRight = SceneComponent->GetRightVector() * GridSizeWorld.X; // Move Horizontally
	const FVector MoveUp = SceneComponent->GetForwardVector() * GridSizeWorld.Y; // Move Vertically
	
	const FVector BottomLeft = GridLocation - MoveRight - MoveUp;
	
	return BottomLeft;
}

void AGrid::GridTileNumber(int32& X, int32& Y) const 
{
	// Number of Tiles = Size of World / Size of Tiles
	X = FMath::RoundToInt(GridSizeWorld.X / TileSize); // Horizontal Amount of Tiles
	Y = FMath::RoundToInt(GridSizeWorld.Y / TileSize); // Vertical Amount of Tiles
}

void AGrid::DrawTile()
{
	TArray<FVector2D> Keys;
	PathfindingMap.GetKeys(Keys);

	for (const FVector2D K : Keys)
	{
		const FPathfindingData* CurrentData = PathfindingMap.Find(K);
		DebugGroundType(CurrentData->GroundType, CurrentData->WorldLocation);
	}
}

bool AGrid::TraceSphere(ETraceTypeQuery TraceType, FHitResult& HitActor) const
{
	const TArray<AActor*> IgnoreActors;
	
	return UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TilePosition, TilePosition, TileSize - TileSizeOffset, TraceType, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitActor, true, FLinearColor::Red, FLinearColor::Green, 20.f);
}

void AGrid::DebugGroundType(EGroundTypes GroundType, FVector GroundLocation)
{
	switch (GroundType)
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
		return;
	}

	DrawDebugSolidPlane(GetWorld(), FPlane(0.f, 0.f, 1.f, TilePosition.Z), GroundLocation, TileSize - TileSizeOffset, TileColor, true, 5.f);
}

void AGrid::GenerateMapDataFromWorld()
{
	for (int X = 0; X < GridTileNumberX; X++)
	{
		for (int Y = 0; Y < GridTileNumberY; Y++)
		{
			// Draw = Draw Direction * Draw Length
			const FVector DrawRight = SceneComponent->GetRightVector() * (TileSize + X * TileSize * 2); // Draw Horizontally
			const FVector DrawUp = SceneComponent->GetForwardVector() * (TileSize + Y * TileSize * 2); // Draw Vertically
			
			TilePosition = GridBottomLeft() + DrawRight + DrawUp;

			FHitResult TraceHit;

			// Adds PFC_Ground as Traced Traces
			TArray<TEnumAsByte<ETraceTypeQuery>> GroundTypesArray;
			GroundTypesArray.Add(UEngineTypes::ConvertToTraceType(PFC_Ground)); // Convert the user-defined PFC_Ground collision channel to ETraceTypeQuery
			
			// Adds PFC_Obstacle as Traced Traces
			TArray<TEnumAsByte<ETraceTypeQuery>> ObstacleTypesArray;
			ObstacleTypesArray.Add(UEngineTypes::ConvertToTraceType(PFC_Obstacle)); // Convert the user-defined PFC_Ground collision channel to ETraceTypeQuery

			if (TraceSphere(GroundTypesArray[0], TraceHit))
			{
				if (TraceSphere(ObstacleTypesArray[0], TraceHit))
				{
					const ABaseObstacle* Obstacle = Cast<ABaseObstacle>(TraceHit.GetActor());

					TSharedPtr<FPathfindingData> PathfindingData = MakeShared<FPathfindingData>();

					PathfindingData->GroundType = Obstacle->GroundType;
					PathfindingData->WorldLocation = TilePosition;
					PathfindingData->GridIndex = FVector2D(X, Y);
					
					PathfindingMap.Add(FVector2D(X, Y), *PathfindingData);
				}
				else
				{
					TSharedPtr<FPathfindingData> PathfindingData = MakeShared<FPathfindingData>();
					
					PathfindingData->GroundType = EGroundTypes::EGT_Normal;
					PathfindingData->WorldLocation = TilePosition;
					PathfindingData->GridIndex = FVector2D(X, Y);
					
					PathfindingMap.Add(FVector2D(X, Y), *PathfindingData);
				}
			}
			else
			{
				TSharedPtr<FPathfindingData> PathfindingData = MakeShared<FPathfindingData>();
				
				PathfindingData->GroundType = EGroundTypes::EGT_None;
				PathfindingData->WorldLocation = TilePosition;
				PathfindingData->GridIndex = FVector2D(X, Y);
				
				PathfindingMap.Add(FVector2D(X, Y), *PathfindingData);
			}
		}
	}
}

void AGrid::SpawnTiles(const bool SpawnNone)
{
	TArray<FVector2D> Keys;
	PathfindingMap.GetKeys(Keys);

	for (const FVector2D K : Keys)
	{
		const FPathfindingData* CurrentData = PathfindingMap.Find(K);

		if (CurrentData->GroundType != EGroundTypes::EGT_None || SpawnNone)
		{
			FTransform CurrentTransform(CurrentData->WorldLocation);
		
			AGridTile* GridTile = GetWorld()->SpawnActorDeferred<AGridTile>(AGridTile::StaticClass(), CurrentTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			if (IsValid(GridTile))
			{
				GridTile->FinishSpawning(CurrentTransform);
				GridTile->SetGrid(this);
				GridTile->SetGridIndex(CurrentData->GridIndex);
			}
		}
	}
}
