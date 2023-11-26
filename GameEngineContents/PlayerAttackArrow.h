#pragma once
#include "AttackBase.h"

// Ό³Έν :
class PlayerAttackArrow : public AttackBase
{
public:
	// constrcuter destructer
	PlayerAttackArrow();
	~PlayerAttackArrow();

	// delete Function
	PlayerAttackArrow(const PlayerAttackArrow& _Other) = delete;
	PlayerAttackArrow(PlayerAttackArrow&& _Other) noexcept = delete;
	PlayerAttackArrow& operator=(const PlayerAttackArrow& _Other) = delete;
	PlayerAttackArrow& operator=(PlayerAttackArrow&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void SetShoot(float _Speed = 1500.0f) override
	{
		AttackBase::SetShoot(_Speed);
		AttackRenderer->GetTransform()->SetLocalRotation(float4{ 90.0f,-0.0f,-90.0f });
		AttackRenderer->GetTransform()->SetLocalScale(float4{ 2.5f,0.8f,2.5f });
		if (nullptr != PhysXComp && nullptr != PhysXComp->GetDynamic())
		{
			PhysXComp->SetDynamicPivot(float4::DOWN * 100.0f);

		}
	}

private:

};

