// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "ToonTanks/GameModes/TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = DefaultHealth;
	GameModeRef = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	Owner = GetOwner();
	if (Owner)
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Health comp has no ref to the owner"));
	}
	
	
	
}

void UHealthComponent::TakeDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType, AController *InstigatedBy, AActor *DamageCauser)
{
	if (Damage == 0 || Health <= 0)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth); // Da ne moze da ide ispod 0

	if (Health <= 0)
	{
		if (GameModeRef)
		{
			GameModeRef->ActorDied(GetOwner());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Health Comp has no reference to the GameMode"));
		}
	}
}
