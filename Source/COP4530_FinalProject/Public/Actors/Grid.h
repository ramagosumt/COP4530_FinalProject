#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/PathfindingDataStructs.h"
#include "Grid.generated.h"

#define PFC_GROUND ECC_GameTraceChannel1
#define PFC_OBSTACLE ECC_GameTraceChannel2

enum class EGroundTypes : uint8;
class UAlgorithmsSelectionWidget;
class UBillboardComponent;
class UGridTileTooltipWidget;
class UStatisticsPanelWidget;
class AGridTile;
struct FPathfindingData;

UENUM()
enum class EGameMode : uint8
{
	EGM_DFS UMETA(DisplayName = "DFS"),
	EGM_BFS UMETA(DisplayName= "BFS"),
	EGM_Dijkstra UMETA(DisplayName= "Dijkstra"),
	EGM_AStar UMETA(DisplayName= "AStar"),
	EGM_None UMETA(DisplayName= "None")
};

UCLASS()
class COP4530_FINALPROJECT_API AGrid : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* BillboardComponent;
	
public:
	
	UPROPERTY(EditInstanceOnly, Category= "Grid")
	FVector GridLocation; // Map Center Location

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	FVector2D GridSizeWorld; // Map Size

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	float TileSize; // Tile Size

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	float TileSizeOffset; // Offset of Tiles

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	int32 GridTileNumberX; // Number of Horizontal Tiles

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	int32 GridTileNumberY; // Number of Vertical Tiles

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	FVector TilePosition; // Position of Tiles

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	FColor TileColor; // Color of Tiles

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	TMap<FVector2D, FPathfindingData> PathfindingMap;

	UPROPERTY(VisibleInstanceOnly, Category= "Grid")
	TMap<int32, FVector2D> ExploringPath;

	UPROPERTY(VisibleInstanceOnly)
	AGridTile* SelectedTile;

	UPROPERTY(VisibleInstanceOnly)
	AGridTile* HoveredTile;

	UPROPERTY(VisibleInstanceOnly)
	TArray<FVector2D> PathToTarget;

	EGameMode GameMode;

	UPROPERTY(VisibleInstanceOnly)
	TSubclassOf<UUserWidget> DesiredSelectionWidget;

	UPROPERTY(VisibleInstanceOnly)
	TSubclassOf<UUserWidget> DesiredTooltipWidget;

	UPROPERTY(VisibleInstanceOnly)
	TSubclassOf<UUserWidget> DesiredStatisticsPanelWidget;

	UPROPERTY()
	UGridTileTooltipWidget* GridTileTooltipWidget;

	UPROPERTY()
	UStatisticsPanelWidget* StatisticsPanelWidget;

	UPROPERTY(EditInstanceOnly, Category= "Debug")
	FColor GridBoxColor;

	UPROPERTY(EditInstanceOnly, Category= "Debug")
	FColor GridBottomLeftColor;
	
	AGrid();
	
	virtual void OnConstruction(const FTransform& Transform) override;

	/** Event Functions */
	
	void SelectNewTile(AGridTile* TileToSelect);
	void HoverNewTile(AGridTile* TileToHover);
	static int32 CalculateTileCost(const EGroundTypes GroundType);
	void ResetGrid();

	/** Getters & Setters */
	
	FORCEINLINE FVector GetGridLocation() const { return GridLocation; };
	FORCEINLINE void SetGridLocation(const FVector NewGridLocation) { GridLocation = NewGridLocation; }
	FORCEINLINE float GetTileSize() const { return TileSize; };
	FORCEINLINE void SetTileSize(const float NewTileSize) { TileSize = NewTileSize; }
	FORCEINLINE TMap<FVector2D, FPathfindingData> GetPathfindingMap() const { return PathfindingMap; };
	FORCEINLINE void SetPathfindingMap(const TMap<FVector2D, FPathfindingData>& NewPathfindingMap) { PathfindingMap = NewPathfindingMap; }
	
	FORCEINLINE UGridTileTooltipWidget* GetGridTileTooltipWidget() const { return GridTileTooltipWidget; };
	FORCEINLINE void SetGridTileTooltipWidget(UGridTileTooltipWidget* NewGridTileTooltipWidget) { GridTileTooltipWidget = NewGridTileTooltipWidget; }

protected:

	int32 CurrentPathIndex;
	FTimerHandle PathHighlight;
	FTimerHandle PathExplore;
	bool bIsHighlightedModeOn;
	
	virtual void BeginPlay() override;

	/** Grid Functions */
	
	FVector GetGridBottomLeft() const; // Calculate the Position of BottomLeft
	void GetGridTileNumbers(int32& NumHorizontalTiles, int32& NumVerticalTiles) const; // Calculate the Amount of Tiles
	void DrawTile(); // Draw the Tiles
	bool TraceSphere(const ETraceTypeQuery TraceType, FHitResult& HitActor) const; // Trace the Spheres
	void DebugGroundType(const EGroundTypes GroundType, const FVector GroundLocation);
	void GenerateMapDataFromWorld();
	void SpawnTiles(const bool SpawnNone);

	void SetSelectionWidget();

	/** Pathfinding Helper Functions */
	
	TArray<FVector2D> GetTileNeighbors(const FVector2D GridIndex); // Get all neighbors from tile
	static int32 GetEstimatedCostToTarget(const FVector2D CurrentIndex, const FVector2D TargetIndex); // Calculate the number of steps
	void FindPathToTarget(const FVector2D StartIndex, const FVector2D TargetIndex);
	TArray<FVector2D> RetracePath(const FVector2D StartIndex, const FVector2D TargetIndex);
	void HighlightCurrentPath(const bool bIsForHighlighted);
	void HighlightCurrentTile();
	void HighlightExploringPath(const bool bIsForHighlighted);
	void HighlightExploringTile();

	/** Pathfinding Algorithms
	 * Depth-First Search [DFS]
	 * Breadth-First Search [BFS]
	 * Dijkstra's [Dis]
	 * A* [As]
	 * Greedy Best-First Search [GBFS]
	 */

	void DepthFirstSearch(const FVector2D StartIndex); // Pathfinding Algorithm - Depth-First Search
	void BreadthFirstSearch(const FVector2D StartIndex); // Pathfinding Algorithm - Breadth-First Search
	void Dijkstra(const FVector2D StartIndex); // Pathfinding Algorithm - Dijkstra's
	void AStar(); // Pathfinding Algorithm - A*
};
