// Copyrlght liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Money.generated.h"

USTRUCT(BlueprintType)
struct FMoneyStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UStaticMesh* Mesh;
};

UCLASS()
class INVENTORYSYSTEM_API AMoney : public AActor
{
	GENERATED_BODY()
	
public:	
	AMoney();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MoneyComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* MoneyDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FDataTableRowHandle MoneyData;
};
