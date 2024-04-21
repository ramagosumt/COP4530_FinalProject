#include "Actors/Grid.h"
#include "Actors/BaseObstacle.h"
#include "Actors/GridTile.h"
#include "Components/BillboardComponent.h"
#include "Data/PathfindingDataStructs.h"
#include "DrawDebugHelpers.h"
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

	FlushPersistentDebugLines(GetWorld());

	SetGridLocation(SceneComponent->GetComponentLocation());

	GenerateMapDataFromWorld();

	SpawnTiles(true);
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
	
	return UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TilePosition, TilePosition, TileSize - TileSizeOffset, TraceType, false, IgnoreActors, EDrawDebugTrace::None, HitActor, true, FLinearColor::Red, FLinearColor::Green, 20.f);
}

void AGrid::DebugGroundType(EGroundTypes GroundType, FVector GroundLocation)
{
	switch (GroundType)
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
			GroundTypesArray.Add(UEngineTypes::ConvertToTraceType(PFC_GROUND)); // Convert the user-defined PFC_Ground collision channel to ETraceTypeQuery
			
			// Adds PFC_Obstacle as Traced Traces
			TArray<TEnumAsByte<ETraceTypeQuery>> ObstacleTypesArray;
			ObstacleTypesArray.Add(UEngineTypes::ConvertToTraceType(PFC_OBSTACLE)); // Convert the user-defined PFC_Ground collision channel to ETraceTypeQuery

			if (TraceSphere(GroundTypesArray[0], TraceHit))
			{
				if (TraceSphere(ObstacleTypesArray[0], TraceHit))
				{
					const ABaseObstacle* Obstacle = Cast<ABaseObstacle>(TraceHit.GetActor());

					TSharedPtr<FPathfindingData> PathfindingData = MakeShared<FPathfindingData>();

					PathfindingData->GroundType = Obstacle->GroundType;
					PathfindingData->WorldLocation = TilePosition;
					PathfindingData->GridIndex = FVector2D(X, Y);
					PathfindingData->TileCost = CalculateTileCost(Obstacle->GroundType);
					
					PathfindingMap.Add(FVector2D(X, Y), *PathfindingData);
				}
				else
				{
					TSharedPtr<FPathfindingData> PathfindingData = MakeShared<FPathfindingData>();
					
					PathfindingData->GroundType = EGroundTypes::EGT_Normal;
					PathfindingData->WorldLocation = TilePosition;
					PathfindingData->GridIndex = FVector2D(X, Y);
					PathfindingData->TileCost = CalculateTileCost(EGroundTypes::EGT_Normal);
					
					PathfindingMap.Add(FVector2D(X, Y), *PathfindingData);
				}
			}
			else
			{
				TSharedPtr<FPathfindingData> PathfindingData = MakeShared<FPathfindingData>();
				
				PathfindingData->GroundType = EGroundTypes::EGT_None;
				PathfindingData->WorldLocation = TilePosition;
				PathfindingData->GridIndex = FVector2D(X, Y);
				PathfindingData->TileCost = CalculateTileCost(EGroundTypes::EGT_None);
				
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
		FPathfindingData* CurrentData = PathfindingMap.Find(K);

		if (CurrentData->GroundType != EGroundTypes::EGT_None || SpawnNone)
		{
			FTransform CurrentTransform(CurrentData->WorldLocation);
		
			AGridTile* GridTile = GetWorld()->SpawnActorDeferred<AGridTile>(AGridTile::StaticClass(), CurrentTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			if (IsValid(GridTile))
			{
				GridTile->FinishSpawning(CurrentTransform);
				GridTile->SetGrid(this);
				GridTile->SetGridIndex(CurrentData->GridIndex);

				CurrentData->TileActor = GridTile;
				PathfindingMap.Add(K, *CurrentData);
			}
		}
	}
}

int32 AGrid::CalculateTileCost(const EGroundTypes GroundType)
{
	int32 Cost = 0;
	
	switch (GroundType)
	{
	case EGroundTypes::EGT_Normal:
		Cost = 1;
		break;
	case EGroundTypes::EGT_Difficult:
		Cost = 2;
		break;
	case EGroundTypes::EGT_ReallyDifficult:
		Cost = 5;
		break;
	case EGroundTypes::EGT_Impossible:
		Cost = 999;
		break;
	case EGroundTypes::EGT_None:
		Cost = 999;
		break;
	}
	
	return Cost;
}

void AGrid::SelectNewTile(AGridTile* TileToSelect)
{
	if (IsValid(SelectedTile)) SelectedTile->DeselectTile();

	SelectedTile = TileToSelect;
}

void AGrid::HoverNewTile(AGridTile* TileToHover)
{
	// Stop hovering the previous tile, then hover the current tile
	if (IsValid(HoveredTile)) HoveredTile->DehoverTile(); HoveredTile = TileToHover;

	HighlightCurrentPath(false);

	if (IsValid(SelectedTile) && IsValid(HoveredTile))
	{
		FindPathToTarget(SelectedTile->GetGridIndex(), HoveredTile->GetGridIndex());
		HighlightCurrentPath(true);
	}
}

TArray<FVector2D> AGrid::GetTileNeighbors(const FVector2D GridIndex)
{
	TArray<FVector2D> OutNeighbors;
	TArray<FVector2D> PossibleNeighbors = {FVector2D(0.f, 1.f), FVector2D(1.f, 0.f), FVector2D(0.f, -1.f), FVector2D(-1.f, 0.f)};

	for (FVector2D Neighbor : PossibleNeighbors)
	{
		const FVector2D CurrentNeighborIndex = GridIndex + Neighbor;
		const FPathfindingData* CurrentNeighbor = PathfindingMap.Find(CurrentNeighborIndex);
		if (CurrentNeighbor) if (CurrentNeighbor->GroundType != EGroundTypes::EGT_None && CurrentNeighbor->GroundType != EGroundTypes::EGT_Impossible) OutNeighbors.AddUnique(CurrentNeighborIndex);
	}

	return OutNeighbors;
}

int32 AGrid::GetEstimatedCostToTarget(const FVector2D CurrentIndex, const FVector2D TargetIndex)
{
	const FVector2D Distance = CurrentIndex - TargetIndex;
	
	return FMath::RoundToInt(FMath::Abs(Distance.X) + FMath::Abs(Distance.Y));
	
}

void AGrid::FindPathToTarget(const FVector2D StartIndex, const FVector2D TargetIndex)
{
	const FVector2D StartTile = StartIndex;
	const FVector2D TargetTile = TargetIndex;

	TArray<FVector2D> Keys;
	PathfindingMap.GetKeys(Keys);

	const FPathfindingData* TargetTileData = PathfindingMap.Find(TargetTile);
	if (TargetTileData)
	{
		if (TargetTileData->GroundType != EGroundTypes::EGT_None && TargetTileData->GroundType != EGroundTypes::EGT_Impossible)
		{
			for (const FVector2D K : Keys)
			{
				FPathfindingData* CurrentData = PathfindingMap.Find(K);
				FPathfindingData* NewCurrentData = CurrentData;
				NewCurrentData->FinalCost = 999.f;
				NewCurrentData->CostFromStart = 999.f;
				NewCurrentData->EstimatedCostToTarget = 999.f;
				NewCurrentData->PreviousTile = FVector2D(0.f, 0.f);
		
				PathfindingMap.Add(K, *NewCurrentData);
			}
	
			FPathfindingData* StartTileData = PathfindingMap.Find(StartTile);
			FPathfindingData* NewStartTileData = StartTileData;
			NewStartTileData->FinalCost = GetEstimatedCostToTarget(StartTile, TargetTile);
			NewStartTileData->CostFromStart = 0.f;
			NewStartTileData->EstimatedCostToTarget = GetEstimatedCostToTarget(StartTile, TargetTile);
			NewStartTileData->PreviousTile = FVector2D(0.f, 0.f);
	
			PathfindingMap.Add(StartTile, *NewStartTileData);

			TArray<FVector2D> OpenSet;
			TArray<FVector2D> ClosedSet;

			FVector2D CurrentTile = StartTile;

			OpenSet.AddUnique(CurrentTile);

			while (OpenSet.Num() > 0)
			{
				FVector2D CheapestTile = OpenSet[0];

				for (const FVector2D Tile : OpenSet)
				{
					const FPathfindingData* TileData = PathfindingMap.Find(Tile);
					const FPathfindingData* CheapestTileData = PathfindingMap.Find(CheapestTile);

					if (TileData->FinalCost < CheapestTileData->FinalCost) CheapestTile = Tile;
					else if (TileData->FinalCost == CheapestTileData->FinalCost) if (TileData->EstimatedCostToTarget < CheapestTileData->EstimatedCostToTarget) CheapestTile = Tile;
				}

				CurrentTile = CheapestTile;
				OpenSet.Remove(CurrentTile);
				ClosedSet.AddUnique(CurrentTile);

				TArray<FVector2D> CurrentNeighbors = GetTileNeighbors(CurrentTile);
				for (const FVector2D Neighbor : CurrentNeighbors)
				{
					FVector2D CurrentNeighbor = Neighbor;
					if (!ClosedSet.Contains(CurrentNeighbor))
					{
						const int32 CurrentNeighborCostFromStart = PathfindingMap.Find(CurrentTile)->CostFromStart + PathfindingMap.Find(CurrentNeighbor)->TileCost;
						if (!OpenSet.Contains(CurrentNeighbor))
						{
							OpenSet.AddUnique(CurrentNeighbor);

							FPathfindingData* CurrentNeighborData = PathfindingMap.Find(CurrentNeighbor);
							FPathfindingData* NewCurrentNeighborData = CurrentNeighborData;
							NewCurrentNeighborData->FinalCost = CurrentNeighborCostFromStart + GetEstimatedCostToTarget(CurrentNeighbor, TargetTile);
							NewCurrentNeighborData->CostFromStart = CurrentNeighborCostFromStart;
							NewCurrentNeighborData->EstimatedCostToTarget = GetEstimatedCostToTarget(CurrentNeighbor, TargetTile);
							NewCurrentNeighborData->PreviousTile = CurrentTile;
								
							PathfindingMap.Add(CurrentNeighbor, *NewCurrentNeighborData);

							if (CurrentNeighbor == TargetTile) PathToTarget = RetracePath(StartTile, TargetTile);
						}
						else
						{
							if (CurrentNeighborCostFromStart < PathfindingMap.Find(CurrentNeighbor)->CostFromStart)
							{
								FPathfindingData* CurrentNeighborData = PathfindingMap.Find(CurrentNeighbor);
								FPathfindingData* NewCurrentNeighborData = CurrentNeighborData;
								NewCurrentNeighborData->FinalCost = CurrentNeighborCostFromStart + GetEstimatedCostToTarget(CurrentNeighbor, TargetTile);
								NewCurrentNeighborData->CostFromStart = CurrentNeighborCostFromStart;
								NewCurrentNeighborData->EstimatedCostToTarget = GetEstimatedCostToTarget(CurrentNeighbor, TargetTile);
								NewCurrentNeighborData->PreviousTile = CurrentTile;
								
								PathfindingMap.Add(CurrentNeighbor, *NewCurrentNeighborData);

								if (CurrentNeighbor == TargetTile) PathToTarget = RetracePath(StartTile, TargetTile);
							}
						}
					}
				}
			}
		}
	}
}

TArray<FVector2D> AGrid::RetracePath(const FVector2D StartIndex, const FVector2D TargetIndex)
{
	TArray<FVector2D> Path;
	TArray<FVector2D> ReversedPath;
	
	FVector2D CurrentTile = TargetIndex;
	while (CurrentTile != StartIndex)
	{
		ReversedPath.AddUnique(CurrentTile);
		CurrentTile = PathfindingMap.Find(CurrentTile)->PreviousTile;
	}

	for (int32 i = ReversedPath.Num()-1; i >= 0; --i) Path.AddUnique(ReversedPath[i]);

	return Path;
}

void AGrid::HighlightCurrentPath(const bool bIsForHighlighted)
{
	for (const FVector2D P : PathToTarget)
	{
		if (IsValid(PathfindingMap.Find(P)->TileActor))
		{
			PathfindingMap.Find(P)->TileActor->IsInPath(bIsForHighlighted);
		}
	}
}

void AGrid::AStar()
{
	
}
