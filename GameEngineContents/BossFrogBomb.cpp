#include "PrecompileHeader.h"
#include "BossFrogBomb.h"
#include "TileManager.h"
#include "ContentLevel.h"
#include "DustParticle.h"

#include <GameEngineBase/GameEngineRandom.h>

BossFrogBomb::BossFrogBomb()
{
}

BossFrogBomb::~BossFrogBomb()
{
}
void BossFrogBomb::SetTargetTile(const float4& _Start, const float4& _Target)
{
	if (_Target == float4::ZERONULL)
	{
		Death();
		return;
	}
	TileManager::MainManager->InActiveTileToDelay(_Target.iy(), _Target.ix());
	Startpoint = _Start;
	Target = TileManager::MainManager->GetTilePos(_Target.iy(), _Target.ix());
	ResetLiveTime();
	P2 = Startpoint * 0.5f + Target * 0.5f;
	P2.y += 800.0f;
}
void BossFrogBomb::Start()
{
	BombRender = CreateComponent<ContentFBXRenderer>();
	BombRender->SetFBXMesh("FrogBomb.fbx", "ContentMeshDeffered");
	BombRender->SetColor(float4{ 201.0f / 256, 190.0f / 256, 146.0f / 256 }, -1);
	BombRender->SetGlowToUnit(0, 0);
	BombRender->SetColor(float4{ 1.0f, 1.0f, 0.1f }, 4.0f);
	GetTransform()->SetLocalScale(float4::ONE * 100.0f);
}

void BossFrogBomb::Update(float _DeltaTime)
{
	if (true == Target.IsZero())
	{
		return;
	}
	if (GetLiveTime() > 1.0f || Target == float4::ZERONULL)
	{
		Death();

		if (Target == float4::ZERONULL)
		{
			return;
		}

		float4 index = TileManager::MainManager->GetTileIndex(Target);
		TileManager::MainManager->DestroyTile(index.iy(), index.ix());
		// 타일 삭제하기

		GameEngineSound::Play("Frog_Phase2_BreakTileWithShoot.mp3");

		CreateDustParticle();
	}

	GetTransform()->SetWorldPosition(float4::Bezier3LerpClamp(Startpoint, P2, Target, GetLiveTime()));
}


void BossFrogBomb::CreateDustParticle()
{
	for (int i = 0; i < 20; i++)
	{
		std::shared_ptr<DustParticle> Dust = GetLevel()->DynamicThis<ContentLevel>()->GetPivotActor()->CreateComponent<DustParticle>();
		Dust->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + float4{ 0.0f, 100.0f, 0.0f });
		Dust->GetTransform()->SetWorldScale({ 120.0f, 120.0f });
		Dust->SetFadeInAndOut();

		float PositionX = GameEngineRandom::MainRandom.RandomFloat(-80, 80);
		float PositionY = GameEngineRandom::MainRandom.RandomFloat(-80, 80);

		Dust->GetTransform()->AddWorldPosition({ PositionX, PositionY });

		float4 Dir = { 0.0f, 1.0f, 0.0f };
		Dir.Normalize();

		Dust->SetMoveInfo(Dir, 200.0f);
		Dust->SetWorldMove();
	}

	float Angle = 0.0f;

	for (int i = 0; i < 12; i++)
	{
		std::shared_ptr<DustParticle> Dust = GetLevel()->DynamicThis<ContentLevel>()->GetPivotActor()->CreateComponent<DustParticle>();
		Dust->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
		Dust->GetTransform()->SetWorldScale({ 120.0f, 120.0f });
		Dust->SetFadeInAndOut();

		Angle += 30.0f;

		float4 Dir = { cos(Angle), 0.0f, sin(Angle) };
		Dir.Normalize();

		Dust->SetMoveInfo(Dir, 200.0f);
		Dust->SetWorldMove();

		Dust->GetTransform()->AddWorldPosition({ 100 * cos(Angle), 1.0f + i , 100 * sin(Angle) });
		Dust->GetTransform()->SetWorldRotation({ 90.0f, 0.0f , 0.0f });
	}
}