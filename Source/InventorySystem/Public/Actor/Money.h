// Copyright liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Money.generated.h"

UCLASS()
class INVENTORYSYSTEM_API AMoney : public AActor
{
	GENERATED_BODY()
	
public:	
	AMoney();
	virtual void OnConstruction(const FTransform& Transform) override;
	FDataTableRowHandle GetMoneyData() { return MoneyData; }
	FName GetMoneyType() { return MoneyType; }
	int32 GetMoneyAmount() { return MoneyAmount; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* MoneyDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FDataTableRowHandle MoneyData;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MoneyComponent;

	UPROPERTY()
	FName MoneyType;

	UPROPERTY()
	int32 MoneyAmount;
};
