#pragma once

#include "CoreMinimal.h"
#include "Inventory.generated.h"

UENUM(BlueprintType)
enum class EItemTypes : uint8
{
	Sword UMETA(DisplayName = "Sword"),
	Shield UMETA(DisplayName = "Shield"),
	Eatable UMETA(DisplayName = "Eatable")
};

USTRUCT(BlueprintType)
struct FSlotStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FDataTableRowHandle ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EItemTypes ItemType;
};

USTRUCT(BlueprintType)
struct FItemStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 StackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EItemTypes ItemType;
};

USTRUCT(BlueprintType)
struct FAllItemStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FSlotStruct> Swords;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FSlotStruct> Shields;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FSlotStruct> Eatables;
};

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

