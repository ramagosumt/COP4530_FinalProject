#include "Actors/Grid.h"
#include "Components/BillboardComponent.h"
#include "DrawDebugHelpers.h"

AGrid::AGrid()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a SceneComponent and set it as the RootComponent
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	// Create a BillboardComponent and attach it to RootComponent
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	BillboardComponent->SetupAttachment(GetRootComponent());

	// Initialize Variables
	GridLocation = FVector(0.f);
	GridSizeWorld = FVector2D(500.f);
	TileSize = 50.f;
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

	DrawTile();
	
}

void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
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
	for (int X = 0; X < GridTileNumberX; X++)
	{
		for (int Y = 0; Y < GridTileNumberY; Y++)
		{
			// Draw = Draw Direction * Draw Length
			const FVector DrawRight = SceneComponent->GetRightVector() * (TileSize + X * TileSize * 2); // Draw Horizontally
			const FVector DrawUp = SceneComponent->GetForwardVector() * (TileSize + Y * TileSize * 2); // Draw Vertically
			
			TilePosition = GridBottomLeft() + DrawRight + DrawUp;
			
			DrawDebugSolidPlane(GetWorld(), FPlane(0.f, 0.f, 1.f, TilePosition.Z), TilePosition, TileSize - 5.f, FColor::Yellow, true, 1000000.f);
		}
	}
}
