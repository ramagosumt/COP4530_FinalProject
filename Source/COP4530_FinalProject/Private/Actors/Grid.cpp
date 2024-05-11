#include "Actors/Grid.h"
#include "Actors/BaseObstacle.h"
#include "Actors/GridTile.h"
#include "Components/BillboardComponent.h"
#include "Data/PathfindingDataStructs.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UserInterfaces/AlgorithmsSelectionWidget.h"
#include "UserInterfaces/GridTileTooltipWidget.h"
#include "UserInterfaces/StatisticsPanelWidget.h"

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
	
	// Create a UserWidget that stores the WBP
	DesiredSelectionWidget = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("/Game/UserInterfaces/WBP_AlgorithmsSelectionWidget")).Class;
	DesiredTooltipWidget = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("/Game/UserInterfaces/WBP_GridTileTooltipWidget")).Class;
	DesiredStatisticsPanelWidget = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("/Game/UserInterfaces/WBP_StatisticsPanelWidget")).Class;

	// Initialize Variables
	GridLocation = FVector(0.f);
	GridSizeWorld = FVector2D(500.f);
	TileSize = 50.f;
	TileSizeOffset = 5.f;
	GridTileNumberX = 0;
	GridTileNumberY = 0;
	GameMode = EGameMode::EGM_DFS;
	GridBoxColor = FColor::Blue;
	GridBottomLeftColor = FColor::Purple;
}

void AGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FlushPersistentDebugLines(GetWorld());
	
	SetGridLocation(SceneComponent->GetComponentLocation());
	
	DrawDebugBox(GetWorld(), GridLocation, FVector(GridSizeWorld.Y, GridSizeWorld.X, 5.f), GridBoxColor, true, 1000000.f, 0.f, 10.f);
	DrawDebugBox(GetWorld(), GetGridBottomLeft(), FVector(20.f), GridBottomLeftColor, true, 1000000.f, 0.f, 10.f);

	GetGridTileNumbers(GridTileNumberX, GridTileNumberY);

	GenerateMapDataFromWorld(); 

	DrawTile();
}

void AGrid::BeginPlay()
{
	Super::BeginPlay();

	const FInputModeGameAndUI InputModeGameAndUI;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputModeGameAndUI);

	SetSelectionWidget();

	FlushPersistentDebugLines(GetWorld());

	SetGridLocation(SceneComponent->GetComponentLocation());

	GenerateMapDataFromWorld();

	SpawnTiles(true);
}

/**
 * Sets up and adds selection and tooltip widgets to the viewport.
 * The selection widget allows the player to choose algorithms.
 * The tooltip widget displays information about grid tiles.
 */
void AGrid::SetSelectionWidget()
{
	// Create and configure the selection widget
	UAlgorithmsSelectionWidget* AlgorithmsSelectionWidget = CreateWidget<UAlgorithmsSelectionWidget>(GetWorld()->GetFirstPlayerController(), DesiredSelectionWidget);
	AlgorithmsSelectionWidget->SetGrid(this); // Associate the grid instance with the selection widget
	AlgorithmsSelectionWidget->AddToViewport(); // Add the selection widget to the viewport
	AlgorithmsSelectionWidget->bIsFocusable = false; // Ensure the selection widget does not receive focus
	AlgorithmsSelectionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible); // Set the visibility of the selection widget

	// Create and configure the tooltip widget
	GridTileTooltipWidget = CreateWidget<UGridTileTooltipWidget>(GetWorld()->GetFirstPlayerController(), DesiredTooltipWidget);
	GridTileTooltipWidget->SetGrid(this); // Associate the grid instance with the tooltip widget
	GridTileTooltipWidget->AddToViewport(); // Add the tooltip widget to the viewport

	// Create and configure the StatisticsPanel widget
	StatisticsPanelWidget = CreateWidget<UStatisticsPanelWidget>(GetWorld()->GetFirstPlayerController(), DesiredStatisticsPanelWidget);
	StatisticsPanelWidget->SetGrid(this); // Associate the grid instance with the tooltip widget
	StatisticsPanelWidget->AddToViewport(); // Add the tooltip widget to the viewport
}

// Returns the bottom-left corner of the grid in world space
FVector AGrid::GetGridBottomLeft() const
{
	// Calculate horizontal and vertical offsets using the grid size
	const FVector RightOffset  = SceneComponent->GetRightVector() * GridSizeWorld.X; // Offset horizontally by grid width
	const FVector UpOffset = SceneComponent->GetForwardVector() * GridSizeWorld.Y; // Offset vertically by grid height

	// Calculate the bottom-left corner of the grid
	const FVector GridBottomLeft  = GridLocation - RightOffset - UpOffset;
	
	return GridBottomLeft;
}

// Calculates and returns the number of tiles horizontally and vertically
void AGrid::GetGridTileNumbers(int32& NumHorizontalTiles, int32& NumVerticalTiles) const	
{
	// Calculate the number of tiles horizontally and vertically
	NumHorizontalTiles = FMath::RoundToInt(GridSizeWorld.X / TileSize); // Number of tiles horizontally
	NumVerticalTiles = FMath::RoundToInt(GridSizeWorld.Y / TileSize); // Number of tiles vertically
}

// Draws all tiles in the grid based on pathfinding map data
void AGrid::DrawTile()
{
	// Retrieve the locations of all tiles in the map
	TArray<FVector2D> Keys;
	PathfindingMap.GetKeys(Keys);

	// Iterate through each key and draw the corresponding tile
	for (const FVector2D& Key : Keys)
	{
		// Find the current pathfinding data based on the key
		const FPathfindingData* CurrentData = PathfindingMap.Find(Key);
		if (CurrentData) DebugGroundType(CurrentData->GroundType, CurrentData->WorldLocation);
	}
}

/**
 * Performs a sphere trace in the world.
 * @param TraceType The type of trace to perform.
 * @param HitActor (out) Information about the hit result.
 * @return True if a hit is detected, false otherwise.
 */
bool AGrid::TraceSphere(const ETraceTypeQuery TraceType, FHitResult& HitActor) const
{
	// List of actors to ignore during the trace
	const TArray<AActor*> IgnoreActors;

	// Perform the sphere trace and return the result
	return UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			TilePosition, // Start position of the trace (center of the sphere)
			TilePosition, // End position of the trace (also center of the sphere, no movement)
			TileSize - TileSizeOffset, // Radius of the sphere
			TraceType, // Type of trace to perform
			false, // Do not trace complex collision
			IgnoreActors, // Actors to ignore during the trace
			EDrawDebugTrace::None, // No debug drawing
			HitActor, // Hit result information
			true, // Trace against complex collision
			FLinearColor::Red, // Color of the debug trace
			FLinearColor::Green, // Color of the hit result debug point
			20.f // Duration of the debug display
		);
}

/**
 * Debugs the ground type by drawing a colored plane at the specified location.
 * The color of the plane corresponds to the type of ground.
 * @param GroundType The type of ground to debug.
 * @param GroundLocation The location where the ground is located in the world.
 */
void AGrid::DebugGroundType(const EGroundTypes GroundType, const FVector GroundLocation)
{
	switch (GroundType)
	{
	case EGroundTypes::EGT_Normal:
		TileColor = FColor::FromHex(FString("808080")); // Normal ground type represented by gray
		break;
	case EGroundTypes::EGT_Difficult:
		TileColor = FColor::Turquoise; // Difficult ground type represented by turquoise
		break;
	case EGroundTypes::EGT_ReallyDifficult:
		TileColor = FColor::FromHex(FString("FF8503")); // Really difficult ground type represented by orange
		break;
	case EGroundTypes::EGT_Impossible:
		TileColor = FColor::FromHex(FString("8B0000")); // Impossible ground type represented by dark red
		break;
	case EGroundTypes::EGT_None:
		TileColor = FColor::Blue; // None ground type represented by blue
		return; // Skip drawing
	}

	// Draw the tile in the corresponding color
	DrawDebugSolidPlane(
		GetWorld(),
		FPlane(0.f, 0.f, 1.f, GroundLocation.Z),
		GroundLocation,
		TileSize - TileSizeOffset,
		TileColor,
		true,
		5.f
	);
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
			
			TilePosition = GetGridBottomLeft() + DrawRight + DrawUp;

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
					PathfindingData->FinalCost = 999.f;
					PathfindingData->EstimatedCostToTarget = 999.f;
					PathfindingData->CostFromStart = 999.f;
					PathfindingData->TileCost = CalculateTileCost(Obstacle->GroundType);
					
					PathfindingMap.Add(FVector2D(X, Y), *PathfindingData);
				}
				else
				{
					TSharedPtr<FPathfindingData> PathfindingData = MakeShared<FPathfindingData>();
					
					PathfindingData->GroundType = EGroundTypes::EGT_Normal;
					PathfindingData->WorldLocation = TilePosition;
					PathfindingData->GridIndex = FVector2D(X, Y);
					PathfindingData->FinalCost = 999.f;
					PathfindingData->EstimatedCostToTarget = 999.f;
					PathfindingData->CostFromStart = 999.f;
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
				PathfindingData->FinalCost = 999.f;
				PathfindingData->EstimatedCostToTarget = 999.f;
				PathfindingData->CostFromStart = 999.f;
				PathfindingData->TileCost = CalculateTileCost(EGroundTypes::EGT_None);
				
				PathfindingMap.Add(FVector2D(X, Y), *PathfindingData);
			}
		}
	}
}

// Spawns tiles based on the pathfinding map data
// If `SpawnNone` is true, tiles of type `EGroundTypes::EGT_None` will also be spawned
void AGrid::SpawnTiles(const bool SpawnNone)
{
	// Retrieve the locations of all tiles in the map
	TArray<FVector2D> Keys;
	PathfindingMap.GetKeys(Keys);

	// Iterate through each key and spawn the corresponding tile
	for (const FVector2D& Key : Keys)
	{
		// Find the current pathfinding data based on the key
		FPathfindingData* CurrentData = PathfindingMap.Find(Key);

		// Ensure the data is valid and determine if we should spawn the tile
		if (CurrentData->GroundType != EGroundTypes::EGT_None || SpawnNone)
		{
			FTransform CurrentTransform(CurrentData->WorldLocation);

			// Spawn the grid tile using deferred spawning
			AGridTile* GridTile = GetWorld()->SpawnActorDeferred<AGridTile>(
				AGridTile::StaticClass(),
				CurrentTransform,
				nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);

			// Finalize the spawning if the grid tile is valid
			if (IsValid(GridTile))
			{
				GridTile->FinishSpawning(CurrentTransform);
				GridTile->SetGrid(this);
				GridTile->SetGridIndex(CurrentData->GridIndex);

				// Associate the tile actor with the current data
				CurrentData->TileActor = GridTile;
				PathfindingMap.Add(Key, *CurrentData);
			}
		}
	}
}

// Calculates and returns the tile cost based on the ground type
int32 AGrid::CalculateTileCost(const EGroundTypes GroundType)
{
	constexpr int32 NormalCost = 1;
	constexpr int32 DifficultCost = 2;
	constexpr int32 ReallyDifficultCost = 5;
	constexpr int32 HighCost = 999; // Used for "Impossible" and "None" types
	
	switch (GroundType)
	{
	case EGroundTypes::EGT_Normal:
		return NormalCost;
	case EGroundTypes::EGT_Difficult:
		return DifficultCost;
	case EGroundTypes::EGT_ReallyDifficult:
		return ReallyDifficultCost;
	case EGroundTypes::EGT_Impossible:
	case EGroundTypes::EGT_None:
		return HighCost;
	default:
		return HighCost;
	}
}

void AGrid::ResetGrid()
{
	for (const FVector2D P : PathToTarget)
	{
		PathfindingMap.Find(P)->FinalCost = 999.f;
		PathfindingMap.Find(P)->CostFromStart = 999.f;
		PathfindingMap.Find(P)->EstimatedCostToTarget = 999.f;
		PathfindingMap.Find(P)->PreviousTile = FVector2D(0.f, 0.f);
		PathfindingMap.Find(P)->bVisited = false;
		HighlightCurrentPath(false);
	}

	PathToTarget.Empty();
}

void AGrid::SelectNewTile(AGridTile* TileToSelect)
{
	if (IsValid(SelectedTile)) SelectedTile->DeselectTile();
	
	SelectedTile = TileToSelect;
	if (GridTileTooltipWidget) if (IsValid(SelectedTile)) GridTileTooltipWidget->SetStartLocation(SelectedTile->GetGridIndex());
}

void AGrid::HoverNewTile(AGridTile* TileToHover)
{
	// Stop hovering the previous tile, then hover the current tile
	if (IsValid(HoveredTile)) HoveredTile->DehoverTile(); HoveredTile = TileToHover;
	if (IsValid(HoveredTile)) if (!IsValid(SelectedTile)) if (GridTileTooltipWidget) GridTileTooltipWidget->SetStartLocation(HoveredTile->GetGridIndex());

	HighlightCurrentPath(false);
	HighlightExploringPath(false);

	if (IsValid(SelectedTile) && IsValid(HoveredTile))
	{
		if (GridTileTooltipWidget) GridTileTooltipWidget->SetTargetLocation(HoveredTile->GetGridIndex());
		if (StatisticsPanelWidget) StatisticsPanelWidget->SetGridLocation(HoveredTile->GetGridIndex());
		
		for (const auto& Key : ExploringPath)
		{
			const FVector2D Location = Key.Value;
			if (IsValid(PathfindingMap.Find(Location)->TileActor)) PathfindingMap.Find(Location)->bVisited = false;
			if (IsValid(PathfindingMap.Find(Location)->TileActor)) PathfindingMap.Find(Location)->TileActor->ResetTileState();
		}
		
		switch (GameMode)
		{
		case EGameMode::EGM_DFS:
			DepthFirstSearch(SelectedTile->GetGridIndex());
			break;
		case EGameMode::EGM_BFS:
			BreadthFirstSearch(SelectedTile->GetGridIndex());
			break;
		case EGameMode::EGM_Dijkstra:
			break;
		case EGameMode::EGM_AStar:
			FindPathToTarget(SelectedTile->GetGridIndex(), HoveredTile->GetGridIndex());
			break;
		case EGameMode::EGM_None:
			break;
		}
		
		HighlightExploringPath(true);
	}
}

TArray<FVector2D> AGrid::GetTileNeighbors(const FVector2D GridIndex)
{
	TArray<FVector2D> OutNeighbors;
	TArray<FVector2D> PossibleNeighbors = {FVector2D(-1.f, 0.f), FVector2D(0.f, -1.f), FVector2D(1.f, 0.f), FVector2D(0.f, 1.f)};

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
			StatisticsPanelWidget->SetIsAValidDestination(true);
			
			for (const FVector2D K : Keys)
			{
				FPathfindingData* CurrentData = PathfindingMap.Find(K);
				FPathfindingData* NewCurrentData = CurrentData;
				NewCurrentData->FinalCost = 999.f;
				NewCurrentData->CostFromStart = 999.f;
				NewCurrentData->EstimatedCostToTarget = 999.f;
				NewCurrentData->PreviousTile = FVector2D(0.f, 0.f);
				NewCurrentData->TileActor->SetTileColor();
		
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
			PathfindingMap.Find(CurrentTile)->TileActor->DebugOpenSet();

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
				PathfindingMap.Find(CurrentTile)->TileActor->DebugClosedSet();

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
							PathfindingMap.Find(CurrentNeighbor)->TileActor->DebugOpenSet();

							FPathfindingData* CurrentNeighborData = PathfindingMap.Find(CurrentNeighbor);
							FPathfindingData* NewCurrentNeighborData = CurrentNeighborData;
							NewCurrentNeighborData->FinalCost = CurrentNeighborCostFromStart + GetEstimatedCostToTarget(CurrentNeighbor, TargetTile);
							NewCurrentNeighborData->CostFromStart = CurrentNeighborCostFromStart;
							NewCurrentNeighborData->EstimatedCostToTarget = GetEstimatedCostToTarget(CurrentNeighbor, TargetTile);
							NewCurrentNeighborData->PreviousTile = CurrentTile;
								
							PathfindingMap.Add(CurrentNeighbor, *NewCurrentNeighborData);

							if (CurrentNeighbor == TargetTile)
							{
								PathToTarget = RetracePath(StartTile, TargetTile);

								return;
							}
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

								if (CurrentNeighbor == TargetTile)
								{
									PathToTarget = RetracePath(StartTile, TargetTile);

									return;
								}
							}
						}
					}
				}
			}
		}
		else StatisticsPanelWidget->SetIsAValidDestination(false);
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
	CurrentPathIndex = 0;
	bIsHighlightedModeOn = bIsForHighlighted;

	if (bIsForHighlighted) GetWorld()->GetTimerManager().SetTimer(PathHighlight, this, &AGrid::HighlightCurrentTile, 0.05f);
	else
	{
		for (const FVector2D P : PathToTarget)
		{
			if (IsValid(PathfindingMap.Find(P)->TileActor)) PathfindingMap.Find(P)->TileActor->IsInPath(bIsForHighlighted);
		}
	}
}

void AGrid::HighlightCurrentTile()
{
	if (CurrentPathIndex >= PathToTarget.Num())
	{
		CurrentPathIndex = 0;
		return;
	}

	const FVector2D CurrentTile = PathToTarget[CurrentPathIndex];
	const FPathfindingData* CurrentTileData = PathfindingMap.Find(CurrentTile);

	if (CurrentTileData && CurrentTileData->TileActor) CurrentTileData->TileActor->IsInPath(bIsHighlightedModeOn);

	CurrentPathIndex++;

	if (CurrentPathIndex <= PathToTarget.Num()) GetWorld()->GetTimerManager().SetTimer(PathHighlight, this, &AGrid::HighlightCurrentTile, 0.05f);
	
}

void AGrid::HighlightExploringPath(const bool bIsForHighlighted)
{
	CurrentPathIndex = 0;
	bIsHighlightedModeOn = bIsForHighlighted;
	
	if (bIsForHighlighted) GetWorld()->GetTimerManager().SetTimer(PathExplore, this, &AGrid::HighlightExploringTile, 0.05f);
	else
	{
		for (const auto& Key : ExploringPath)
		{
			const FVector2D Location = Key.Value;
			if (IsValid(PathfindingMap.Find(Location)->TileActor)) PathfindingMap.Find(Location)->TileActor->IsExploring(bIsHighlightedModeOn);
		}
	}
}

void AGrid::HighlightExploringTile()
{
	if (CurrentPathIndex >= ExploringPath.Num())
	{
		CurrentPathIndex = 0;
		HighlightCurrentPath(true);
		return;
	}

	const FVector2D* Location = ExploringPath.Find(CurrentPathIndex);
	const FVector2D CurrentTile = *Location;
	const FPathfindingData* CurrentTileData = PathfindingMap.Find(CurrentTile);

	if (CurrentTileData && CurrentTileData->TileActor) CurrentTileData->TileActor->IsExploring(bIsHighlightedModeOn);

	CurrentPathIndex++;

	if (CurrentPathIndex <= ExploringPath.Num()) GetWorld()->GetTimerManager().SetTimer(PathExplore, this, &AGrid::HighlightExploringTile, 0.125f);
}

/**
 * Performs a depth-first search starting from the given tile index.
 * @param StartIndex The index of the tile to start the search from.
 */
void AGrid::DepthFirstSearch(const FVector2D StartIndex)
{
	// Ensure the starting tile is valid
	if (IsValid(PathfindingMap.Find(StartIndex)->TileActor))
	{
		// Get the starting execution time using FPlatformTime Cycles
		const uint64 StartCycles = FPlatformTime::Cycles64();
		
		// Create a stack to store visiting tiles
		TArray<FVector2D> Stack;
		Stack.Add(StartIndex);

		int32 ExploringLevel = 0;

		// Initialize the number of nodes visited
		int32 NumberOfNodesVisited = 0;

		// Clear the previous path to target
		PathToTarget.Empty();
		ExploringPath.Empty();
	    
	    // Retrieve the locations of all tiles in the map
		TArray<FVector2D> Keys;
		PathfindingMap.GetKeys(Keys);

		const FPathfindingData* HoveredTileData = PathfindingMap.Find(HoveredTile->GetGridIndex());
		
	    // Check if the hovered tile is valid and traversable
		if (HoveredTileData && HoveredTileData->GroundType != EGroundTypes::EGT_None && HoveredTileData->GroundType != EGroundTypes::EGT_Impossible)
		{
			StatisticsPanelWidget->SetIsAValidDestination(true);
			
	        // If yes, traverse all tiles in the map and reset their respective data
			for (const FVector2D K : Keys)
			{
				FPathfindingData* CurrentData = PathfindingMap.Find(K);
				if (CurrentData)
				{
					// Reset previous tile data to (0.0, 0.0) and mark as unvisited
					CurrentData->PreviousTile = FVector2D(0.f, 0.f);
					CurrentData->bVisited = false;
				}
			}
		}
		else StatisticsPanelWidget->SetIsAValidDestination(false);
		
		// Locate and prepare the starting tile
		FPathfindingData* StartIndexData = PathfindingMap.Find(StartIndex);
		if (StartIndexData)
		{
			// Reset previous tile data to (0.0, 0.0) and mark as unvisited
			StartIndexData->PreviousTile = FVector2D(0.f, 0.f);
			StartIndexData->bVisited = false;
		}

		// Run depth-first search
		while (Stack.Num() > 0)
		{
			// Locating the current tile within the map 
			const FVector2D CurrentTile = Stack.Pop();
			FPathfindingData* CurrentTileData = PathfindingMap.Find(CurrentTile);

			if (CurrentTileData && !CurrentTileData->bVisited)
			{
				// Mark the current tile as visited
				CurrentTileData->bVisited = true;
				NumberOfNodesVisited++;
				ExploringPath.Add(ExploringLevel, CurrentTile);
				ExploringLevel++;

				// Check if the current tile is the target tile
				if (CurrentTileData->GridIndex == HoveredTile->GetGridIndex())
				{
					// If target tile found, retrace the path and return
					const uint64 EndCycles = FPlatformTime::Cycles64();
					const float Timespan = FPlatformTime::ToSeconds64(EndCycles - StartCycles) * 1000;
					PathToTarget = RetracePath(StartIndex, HoveredTile->GetGridIndex());
					StatisticsPanelWidget->SetNumberOfNodes(NumberOfNodesVisited);
					StatisticsPanelWidget->SetExecuteTime(Timespan);
					return;
				}
			}

			// Explore all possible neighboring tiles from the current tile
			TArray<FVector2D> PossibleNeighbors = GetTileNeighbors(CurrentTile);
			for (const FVector2D Neighbor : PossibleNeighbors)
			{
	            // Visit each neighboring tile
				FPathfindingData* NeighborData = PathfindingMap.Find(Neighbor);
				if (NeighborData && !NeighborData->bVisited && NeighborData->GroundType != EGroundTypes::EGT_None && NeighborData->GroundType != EGroundTypes::EGT_Impossible)
				{
					// Mark the neighbor as visited, set its previous tile, and add it to the stack
					NeighborData->PreviousTile = CurrentTile;
					Stack.Add(Neighbor);
				}
			}
		}
	}
}

/**
 * Performs a breadth-first search starting from the given tile index.
 * @param StartIndex The index of the tile to start the search from.
 */
void AGrid::BreadthFirstSearch(const FVector2D StartIndex)
{
	// Get the starting execution time using FPlatformTime Cycles
	const uint64 StartCycles = FPlatformTime::Cycles64();
	
	// Create a queue to store visiting tiles
	TQueue<FVector2D> VisitingTiles;
	VisitingTiles.Enqueue(StartIndex);

	int32 ExploringLevel = 0;

	// Initialize the number of nodes visited
	int32 NumberOfNodesVisited = 0;

	// Clear the previous path to target
	PathToTarget.Empty();
	ExploringPath.Empty();

	/* Reset pathfinding data for all tiles, ensuring they are unvisited and without previous tiles. */

	// Retrieve the locations of all tiles in the map
	TArray<FVector2D> Keys;
	PathfindingMap.GetKeys(Keys);

	const FPathfindingData* HoveredTileData = PathfindingMap.Find(HoveredTile->GetGridIndex());

	// Check if the hovered tile is valid and traversable
	if (HoveredTileData && HoveredTileData->GroundType != EGroundTypes::EGT_None && HoveredTileData->GroundType != EGroundTypes::EGT_Impossible)
	{
		StatisticsPanelWidget->SetIsAValidDestination(true);
		
		// If yes, traverse all tiles in the map and reset their respective data
		for (const FVector2D K : Keys)
		{
			FPathfindingData* CurrentData = PathfindingMap.Find(K);
			if (CurrentData)
			{
				// Reset previous tile data to (0.0, 0.0) and mark as unvisited
				CurrentData->PreviousTile = FVector2D(0.f, 0.f);
				CurrentData->bVisited = false;
			}
		}
	}
	else StatisticsPanelWidget->SetIsAValidDestination(false);
	
	// Locate and prepare the starting tile
	FPathfindingData* StartIndexData = PathfindingMap.Find(StartIndex);
	if (StartIndexData)
	{
		// Reset previous tile data to (0.0, 0.0) and mark as unvisited
		StartIndexData->PreviousTile = FVector2D(0.f, 0.f);
		StartIndexData->bVisited = false;
	}

	// Run breadth-first search
	while (!VisitingTiles.IsEmpty())
	{
		// Locating the current tile within the map
		FVector2D CurrentTile;
		VisitingTiles.Dequeue(CurrentTile);

		// Check if the current tile is the target tile
		FPathfindingData* CurrentTileData = PathfindingMap.Find(CurrentTile);
		if (CurrentTileData && !CurrentTileData->bVisited)
		{
			// Mark the current tile as visited
			CurrentTileData->bVisited = true;
			NumberOfNodesVisited++;
			ExploringPath.Add(ExploringLevel, CurrentTile);
			ExploringLevel++;

			// Check if the current tile is the target tile
			if (CurrentTileData->GridIndex == HoveredTile->GetGridIndex())
			{
				// If target tile found, retrace the path and return
				const uint64 EndCycles = FPlatformTime::Cycles64();
				const float Timespan = FPlatformTime::ToSeconds64(EndCycles - StartCycles) * 1000;
				PathToTarget = RetracePath(StartIndex, HoveredTile->GetGridIndex());
				StatisticsPanelWidget->SetNumberOfNodes(NumberOfNodesVisited);
				StatisticsPanelWidget->SetExecuteTime(Timespan);
				return;
			}
		}

		// Explore all possible neighboring tiles from the current tile
		TArray<FVector2D> PossibleNeighbors = GetTileNeighbors(CurrentTile);
		for (const FVector2D Neighbor : PossibleNeighbors)
		{
			// Visit each neighboring tile
			FPathfindingData* NeighborData = PathfindingMap.Find(Neighbor);
			if (NeighborData && !NeighborData->bVisited)
			{
				// Mark the neighbor as visited, set its previous tile, and add it to the queue
				NeighborData->PreviousTile = CurrentTile;
				VisitingTiles.Enqueue(Neighbor);
			}
		}
	}
}

void AGrid::Dijkstra(const FVector2D StartIndex)
{
}

void AGrid::AStar()
{
	
}
