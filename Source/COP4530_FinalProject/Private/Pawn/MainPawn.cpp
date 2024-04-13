#include "Pawn/MainPawn.h"


AMainPawn::AMainPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMainPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

