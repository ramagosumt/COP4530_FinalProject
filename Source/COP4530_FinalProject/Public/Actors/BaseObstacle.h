#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseObstacle.generated.h"

#define PFC_GROUND ECC_GameTraceChannel1 // User-defined collision channel for Grounds
#define PFC_OBSTACLE ECC_GameTraceChannel2 // User-defined collision channel for Obstacles

UENUM()
enum class EGroundTypes : uint8
{
	EGT_Normal UMETA(DisplayName = "Normal"),
	EGT_Difficult UMETA(DisplayName= "Difficult"),
	EGT_ReallyDifficult UMETA(DisplayName= "ReallyDifficult"),
	EGT_Impossible UMETA(DisplayName= "Impossible"),
	EGT_None UMETA(DisplayName= "None")
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

	UPROPERTY(EditAnywhere)
	EGroundTypes GroundType;
	
	ABaseObstacle();
	
};
