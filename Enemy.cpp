// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Main.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.f);

	CombatSpher = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSpher"));
	CombatSpher->SetupAttachment(GetRootComponent());
	CombatSpher->InitSphereRadius(75.f);

	bOverlappingCombatShpere = false;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnoverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnoverlapEnd);

	CombatSpher->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSpherOnoverlapBegin);
	CombatSpher->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSpherOnoverlapEnd);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AgroSphereOnoverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			MoveToTarget(Main);
		}
	}
}

void AEnemy::AgroSphereOnoverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);

		if (Main)
		{
			SetEnemyMovementStatus(EEneymyMovementStatus::EMS_Idle);
			if (AIController)
			{
				AIController->StopMovement();
			}
		}

	}
}


void AEnemy::CombatSpherOnoverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		
		if (Main)
		{
			combatTarget = Main;
			bOverlappingCombatShpere = true;
			SetEnemyMovementStatus(EEneymyMovementStatus::EMS_Attacking);
		}
		
	}
}

void AEnemy::CombatSpherOnoverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);

		if (Main)
		{
			bOverlappingCombatShpere = false;
			if (EnemyMovementStatus != EEneymyMovementStatus::EMS_Attacking)
			{
				MoveToTarget(Main);
				combatTarget = nullptr;
			}
		}

	}
}

void AEnemy::MoveToTarget(AMain* Target)
{
	SetEnemyMovementStatus(EEneymyMovementStatus::EMS_MoveToTarget);

	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.0f);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);

		/** 

		auto PathPoints= NavPath->GetPathPoints();
		for (auto Point : PathPoints)
		{
			FVector Location = Point.Location;

			UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Red, 10.f, 1.5f);
		}
		*/
	}
}

