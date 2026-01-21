#include "AI/BTService_findNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include <BehaviorTree/BTFunctionLibrary.h>

void UBTService_findNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	/* 获取具体的敌人（谁在受到这个Controller控制）*/
	APawn* OwningPawn = AIOwner->GetPawn();

	/* 如果有Player标签，敌人就是目标，否则Player是目标 （友方单位也设置为了Player）*/
	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	/* 拥有Target标签的AActor */
	TArray<AActor*> TargetActors;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, TargetActors);
	/* 找到离自己最近的一个敌人 */
	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;

	for (auto* TargetActor : TargetActors) {
		float Distance = OwningPawn->GetDistanceTo(TargetActor);
		if (Distance < ClosestDistance) {
			ClosestDistance = Distance;
			ClosestActor = TargetActor;
		}
	}

	/* 设置黑板变量 */
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
}