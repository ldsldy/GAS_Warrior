// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

// 게임플레이 태그를 정의하는 모듈의 API 매크로
// Enum 타입처럼 WarriorGameplayTags::태그이름으로 c++에서 접근 가능
// 해쉬 기반이므로 성능 빠름

// 선언부 : 이런 게임플레이 태그들이 있음을 선언
namespace WarriorGameplayTags
{
	/**Input Tags**/
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move); // 유저의 입력 동작을 정의하는 태그
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipAxe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipAxe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Roll);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SwitchTarget);

	// 지속 적으로 눌러야 하는 입력을 정의하는 태그
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MustBeHeld);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MustBeHeld_Block);

	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Toggleable);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Toggleable_TargetLock);

	/**Player Tags**/
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Axe); // 어빌리티를 정의하는 태그
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Unequip_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_HitPause);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Roll);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Block);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_TargetLock);

	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe);

	// 이벤트를 정의하는 태그
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unequip_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_HitPause);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_SuccessfulBlock);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_SwitchTarget_Left);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_SwitchTarget_Right);

	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_JumpToFinisher);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rolling);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Blocking);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_TargetLock);

	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Light); // Effect의 세부 설정을 정의하는 SetbyCaller 태그
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Heavy);

	/**Enemy Tags**/
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Melee);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Ranged);

	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon);

	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Strafing); // 현재 상태를 정의하는 태그
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_UnderAttack);

	/**Shared Tags**/
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_HitReact);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_Death);

	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeHit);  // 근접 공격 적중 이벤트 태그
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_HitReact);

	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_BaseDamage); // 기본 데미지 태그
	
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Death);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Front);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Left);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Right);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Back);
}