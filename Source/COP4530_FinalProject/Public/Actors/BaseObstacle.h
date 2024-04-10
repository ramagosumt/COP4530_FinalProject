#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseObstacle.generated.h"

/** Defines */
#define PFC_Ground ECC_GameTraceChannel1 // User-defined collision channel for Grounds
#define PFC_Obstacle ECC_GameTraceChannel2 // User-defined collision channel for Obstacles

/** Enumerations */
UENUM()
enum class EGroundTypes : uint8
{
	EGT_Normal UMETA(DisplayName = "Normal"),
	EGT_Difficult UMETA(DisplayName= "Difficult"),
	EGT_ReallyDifficult UMETA(DisplayName= "ReallyDifficult"),
	EGT_Impossible UMETA(DisplayName= "Impossible")
};

UCLASS()
class COP4530_FINALPROJECT_API ABaseObstacle : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;
	
public:

	UPROPERTY(EditInstanceOnly, Category= "Grid")
	EGroundTypes GroundType;
	
	ABaseObstacle();
	
	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;
	
};
