// Copyrlght liclem97.


#include "Actor/Money.h"

#include "Components/StaticMeshComponent.h"

AMoney::AMoney()
{
	PrimaryActorTick.bCanEverTick = false;

	MoneyComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Money"));

}

void AMoney::OnConstruction(const FTransform& Transform)
{
	if (!IsValid(MoneyDataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("Money : Money DataTable is nullptr"));
		return;
	}

	FString ContextString;
	FMoneyStruct* RowData = MoneyDataTable->FindRow<FMoneyStruct>(MoneyAmount.RowName, ContextString);

	if (RowData)
	{
		MoneyComponent->SetStaticMesh(RowData->Mesh);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Money : Can't find RowData."));
	}
}

void AMoney::BeginPlay()
{
	Super::BeginPlay();
}

