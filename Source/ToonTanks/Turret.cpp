// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (InFireRange())
		RotateTurret(Tank->GetActorLocation());
}

void ATurret::HandleDestruction() {
	Super::HandleDestruction();
	Destroy();
}
void ATurret::BeginPlay() {
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurret::CheckFireCondition, FireRate, true);
}

void ATurret::CheckFireCondition() {
	if (InFireRange())
		Fire();
}

bool ATurret::InFireRange() {
	if (Tank) {
		float distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		if (distance <= FireRange)
			return true;
	}
	return false;
}
