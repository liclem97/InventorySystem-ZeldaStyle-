// Copyright liclem97.


#include "Actor/Money.h"

#include "Components/StaticMeshComponent.h"

AMoney::AMoney()
{
	PrimaryActorTick.bCanEverTick = false;

	MoneyComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Money"));
	MoneyComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

void AMoney::OnConstruction(const FTransform& Transform)
{
	if (!IsValid(MoneyDataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("Money : Money DataTable is nullptr"));
		return;
	}

	FString ContextString;
	FMoneyStruct* RowData = MoneyDataTable->FindRow<FMoneyStruct>(MoneyData.RowName, ContextString);

	if (RowData)
	{
		MoneyComponent->SetStaticMesh(RowData->Mesh);
		MoneyType = RowData->Name;
		MoneyAmount = RowData->Amount;
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

