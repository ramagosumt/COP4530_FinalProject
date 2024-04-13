#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPawn.generated.h"

UCLASS()
class COP4530_FINALPROJECT_API AMainPawn : public APawn
{
	GENERATED_BODY()

public:
	
	AMainPawn();

	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	
	virtual void BeginPlay() override;
	
};
