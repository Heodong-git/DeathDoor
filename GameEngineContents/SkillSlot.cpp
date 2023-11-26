#include "PrecompileHeader.h"
#include "SkillSlot.h"
#include "ContentUIRenderer.h"
#include "Player.h"

SkillSlot::SkillSlot()
{
}

SkillSlot::~SkillSlot()
{
}

void SkillSlot::Start()
{
	SetSlot();

	GetTransform()->SetLocalPosition({ -650, 300 });

	//static Player* MainPlayer;
	//enum class PlayerSkill
	//{
	//	ARROW,
	//	MAGIC,
	//	BOMB,
	//	HOOK,
	//	MAX,
	//};

}

void SkillSlot::Update(float _DeltaTime)
{
	if (Player::MainPlayer == nullptr)
	{
		return;
	}
	CurSkill = Player::MainPlayer->GetPlayerSkill();

	SkillChange();

	if (UpdateFunc.size() != 0)
	{
		std::list<std::function<bool(float)>>::iterator StartIter = UpdateFunc.begin();
		std::list<std::function<bool(float)>>::iterator EndIter = UpdateFunc.end();

		while (StartIter != EndIter)
		{
			std::function<bool(float)> Func = *StartIter;

			if (Func(_DeltaTime) == true)
			{
				StartIter = UpdateFunc.erase(StartIter);
				continue;
			}

			StartIter++;
		}
	}

	float4 Pos = SlotList[1]->SkillRender->GetTransform()->GetLocalPosition();

	int a = 0;
}

void SkillSlot::Render(float _DeltaTime)
{
}

void SkillSlot::SetSlot()
{

	//Arrow
	std::shared_ptr<Slot> ArrowSlot = std::make_shared<Slot>();

	ArrowSlot->HalfSlot_1 = CreateComponent<ContentUIRenderer>();
	ArrowSlot->HalfSlot_1->SetScaleToTexture("SkillSlotHalf_1.png");
	ArrowSlot->HalfSlot_1->GetTransform()->SetLocalScale({ 43.0f, 84.0f });
	ArrowSlot->HalfSlot_1->GetTransform()->SetLocalRotation({ 0.0f, 0.0f, -90.0f });
	ArrowSlot->HalfSlot_1->GetTransform()->SetLocalPosition({ 0.0f, 69.5f });

	ArrowSlot->HalfSlot_2 = CreateComponent<ContentUIRenderer>();
	ArrowSlot->HalfSlot_2->SetScaleToTexture("SkillSlotHalf_2.png");
	ArrowSlot->HalfSlot_2->GetTransform()->SetLocalScale({ 43.0f, 84.0f });
	ArrowSlot->HalfSlot_2->GetTransform()->SetLocalRotation({ 0.0f, 0.0f, -90.0f });
	ArrowSlot->HalfSlot_2->GetTransform()->SetLocalPosition({ 0.0f, 26.5f });

	ArrowSlot->HalfSlotLinker_1 = CreateComponent<ContentUIRenderer>();
	ArrowSlot->HalfSlotLinker_1->SetScaleToTexture("SlotLinker.png");
	ArrowSlot->HalfSlotLinker_1->GetTransform()->SetLocalScale({ 0.0f, 0.0f });
	ArrowSlot->HalfSlotLinker_1->GetTransform()->SetLocalRotation({ 0.0f, 0.0f, -90.0f });
	ArrowSlot->HalfSlotLinker_1->GetTransform()->SetLocalPosition({ 0.0f, 48.0f });

	ArrowSlot->HalfSlotLinker_2 = CreateComponent<ContentUIRenderer>();
	ArrowSlot->HalfSlotLinker_2->SetScaleToTexture("SlotLinker.png");
	ArrowSlot->HalfSlotLinker_2->GetTransform()->SetLocalScale({ 0.0f, 3.5f });
	ArrowSlot->HalfSlotLinker_2->GetTransform()->SetLocalRotation({ 0.0f, 0.0f, -90.0f });
	ArrowSlot->HalfSlotLinker_2->GetTransform()->SetLocalPosition({ 0.0f, 48.0f });

	ArrowSlot->SkillRender = CreateComponent<ContentUIRenderer>();
	ArrowSlot->SkillRender->SetTexture("Icon_Arrow.png");
	ArrowSlot->SkillRender->GetTransform()->SetLocalScale({ 48.0f, 48.0f });
	ArrowSlot->SkillRender->GetTransform()->SetLocalPosition({ 0.0f, 45.0f });

	ArrowSlot->BasicPos_SkillRender = { 0.0f, 45.0f };

	ArrowSlot->BasicPos_Half_1 = { 0.0f, 69.5f };
	ArrowSlot->BasicScale_SkillRender = { 48, 48 };

	SlotList.push_back(ArrowSlot);

	//Fire
	std::shared_ptr<Slot> FireSlot = std::make_shared<Slot>();

	FireSlot->HalfSlot_1 = CreateComponent<ContentUIRenderer>();
	FireSlot->HalfSlot_1->SetScaleToTexture("SkillSlotHalf_1.png");
	FireSlot->HalfSlot_1->GetTransform()->SetLocalPosition({ -69.5f, 0.0f });
	FireSlot->HalfSlot_1->GetTransform()->SetLocalScale({ 43.0f, 84.0f });
	
	FireSlot->HalfSlot_2 = CreateComponent<ContentUIRenderer>();
	FireSlot->HalfSlot_2->SetScaleToTexture("SkillSlotHalf_2.png");
	FireSlot->HalfSlot_2->GetTransform()->SetLocalPosition({ -26.5f, 0.0f });
	FireSlot->HalfSlot_2->GetTransform()->SetLocalScale({ 43.0f, 84.0f });

	FireSlot->HalfSlotLinker_1 = CreateComponent<ContentUIRenderer>();
	FireSlot->HalfSlotLinker_1->SetScaleToTexture("SlotLinker.png");
	FireSlot->HalfSlotLinker_1->GetTransform()->SetLocalPosition({-26.5f, 40.0f });
	FireSlot->HalfSlotLinker_1->GetTransform()->SetLocalScale({ 0.0f, 3.5f });

	FireSlot->HalfSlotLinker_2 = CreateComponent<ContentUIRenderer>();
	FireSlot->HalfSlotLinker_2->SetScaleToTexture("SlotLinker.png");
	FireSlot->HalfSlotLinker_2->GetTransform()->SetLocalPosition({ -26.5f, -41.0f });
	FireSlot->HalfSlotLinker_2->GetTransform()->SetLocalScale({ 0.0f, 3.5f });

	FireSlot->SkillRender = CreateComponent<ContentUIRenderer>();
	FireSlot->SkillRender->SetTexture("Icon_Fireball.png");
	FireSlot->SkillRender->GetTransform()->SetLocalScale({ 64.0f, 64.0f });
	FireSlot->SkillRender->GetTransform()->SetLocalPosition({ -45.0f, 0.0f});

	FireSlot->BasicPos_SkillRender = { -45.0f, 0.0f };

	FireSlot->BasicPos_Half_1 = { -69.5f, 0 };
	FireSlot->BasicScale_SkillRender = { 64, 64 };

	SlotList.push_back(FireSlot);

	//Bomb
	std::shared_ptr<Slot> BombSlot = std::make_shared<Slot>();

	BombSlot->HalfSlot_1 = CreateComponent<ContentUIRenderer>();
	BombSlot->HalfSlot_1->SetScaleToTexture("SkillSlotHalf_1.png");
	BombSlot->HalfSlot_1->GetTransform()->SetLocalScale({ 43.0f, 84.0f });
	BombSlot->HalfSlot_1->GetTransform()->SetLocalRotation({ 0.0f, 0.0f, 180.0f });
	BombSlot->HalfSlot_1->GetTransform()->SetLocalPosition({ 69.5f, 0.0f });

	BombSlot->HalfSlot_2 = CreateComponent<ContentUIRenderer>();
	BombSlot->HalfSlot_2->SetScaleToTexture("SkillSlotHalf_2.png");
	BombSlot->HalfSlot_2->GetTransform()->SetLocalScale({ 43.0f, 84.0f });
	BombSlot->HalfSlot_2->GetTransform()->SetLocalRotation({ 0.0f, 0.0f, 180.0f });
	BombSlot->HalfSlot_2->GetTransform()->SetLocalPosition({ 26.5f, 0.0f });

	BombSlot->HalfSlotLinker_1 = CreateComponent<ContentUIRenderer>();
	BombSlot->HalfSlotLinker_1->SetScaleToTexture("SlotLinker.png");
	BombSlot->HalfSlotLinker_1->GetTransform()->SetLocalScale({ 0.0f, 0.0f });
	BombSlot->HalfSlotLinker_1->GetTransform()->SetLocalRotation({ 0.0f, 0.0f, 0.0f });
	BombSlot->HalfSlotLinker_1->GetTransform()->SetLocalPosition({ 48.0f, 40.0f });

	BombSlot->HalfSlotLinker_2 = CreateComponent<ContentUIRenderer>();
	BombSlot->HalfSlotLinker_2->SetScaleToTexture("SlotLinker.png");
	BombSlot->HalfSlotLinker_2->GetTransform()->SetLocalScale({ 0.0f, 3.5f });
	BombSlot->HalfSlotLinker_2->GetTransform()->SetLocalRotation({ 0.0f, 0.0f, 0.0f });
	BombSlot->HalfSlotLinker_2->GetTransform()->SetLocalPosition({ 48.0f, -41.0f });

	BombSlot->SkillRender = CreateComponent<ContentUIRenderer>();
	BombSlot->SkillRender->SetTexture("Icon_Bomb.png");
	BombSlot->SkillRender->GetTransform()->SetLocalScale({ 64.0f, 64.0f });
	BombSlot->SkillRender->GetTransform()->SetLocalPosition({ 47.5f, 0.0f });

	BombSlot->BasicPos_SkillRender = { 47.5f, 0.0f };

	BombSlot->BasicPos_Half_1 = { 69.5f, 0.0f };
	BombSlot->BasicScale_SkillRender = {64, 64 };

	SlotList.push_back(BombSlot);

	//Hook
	std::shared_ptr<Slot> HookSlot = std::make_shared<Slot>();

	HookSlot->HalfSlot_1 = CreateComponent<ContentUIRenderer>();
	HookSlot->HalfSlot_1->SetScaleToTexture("SkillSlotHalf_1.png");
	HookSlot->HalfSlot_1->GetTransform()->SetLocalScale({ 43.0f, 84.0f });
	HookSlot->HalfSlot_1->GetTransform()->SetLocalRotation({ 0.0f, 0.0f, 90.0f });
	HookSlot->HalfSlot_1->GetTransform()->SetLocalPosition({ 0.0f, -69.5f });

	HookSlot->HalfSlot_2 = CreateComponent<ContentUIRenderer>();
	HookSlot->HalfSlot_2->SetScaleToTexture("SkillSlotHalf_2.png");
	HookSlot->HalfSlot_2->GetTransform()->SetLocalScale({ 43.0f, 84.0f });
	HookSlot->HalfSlot_2->GetTransform()->SetLocalRotation({ 0.0f, 0.0f, 90.0f });
	HookSlot->HalfSlot_2->GetTransform()->SetLocalPosition({ 0.0f, -26.5f });

	HookSlot->HalfSlotLinker_1 = CreateComponent<ContentUIRenderer>();
	HookSlot->HalfSlotLinker_1->SetScaleToTexture("SlotLinker.png");
	HookSlot->HalfSlotLinker_1->GetTransform()->SetLocalScale({ 0.0f, 0.0f });
	HookSlot->HalfSlotLinker_1->GetTransform()->SetLocalRotation({ 0.0f, 0.0f, 90.0f });
	HookSlot->HalfSlotLinker_1->GetTransform()->SetLocalPosition({ 40.0f, 48.0f});

	HookSlot->HalfSlotLinker_2 = CreateComponent<ContentUIRenderer>();
	HookSlot->HalfSlotLinker_2->SetScaleToTexture("SlotLinker.png");
	HookSlot->HalfSlotLinker_2->GetTransform()->SetLocalScale({ 0.0f, 0.0f });
	HookSlot->HalfSlotLinker_2->GetTransform()->SetLocalRotation({ 0.0f, 0.0f, 90.0f });
	HookSlot->HalfSlotLinker_2->GetTransform()->SetLocalPosition({ -41.0f, 48.0f });

	HookSlot->SkillRender = CreateComponent<ContentUIRenderer>();
	HookSlot->SkillRender->SetTexture("Icon_Hook.png");
	HookSlot->SkillRender->GetTransform()->SetLocalScale({ 64.0f, 64.0f });
	HookSlot->SkillRender->GetTransform()->SetLocalPosition({ 0.0f, -47.5f });

	HookSlot->BasicPos_SkillRender = { 0.0f, -47.5f };

	HookSlot->BasicPos_Half_1 = { 0.0f, -69.5f };
	HookSlot->BasicScale_SkillRender = { 64, 64 };

	SlotList.push_back(HookSlot);
}

bool SkillSlot::LerpSlotScaleUp_Fire(float _DeltaTime)
{
	if (LerpUpRatio >= 1.0f)
	{
		LerpUpRatio = 0.0f;

		return true;
	}

	LerpUpRatio += 8.0f * _DeltaTime;

	float4 StartPos_Half_1 = SlotList[1]->BasicPos_Half_1;
	float4 EndPos_Half_1 = StartPos_Half_1 + float4{-25.0f, 0.0f, 0.0f};

	float4 DestPos_Half_1 = float4::Lerp(StartPos_Half_1, EndPos_Half_1, LerpUpRatio);

	SlotList[1]->HalfSlot_1->GetTransform()->SetLocalPosition(DestPos_Half_1);

	float Distance = abs(DestPos_Half_1.x - StartPos_Half_1.x);

	SlotList[1]->HalfSlotLinker_1->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[1]->HalfSlotLinker_1->GetTransform()->SetLocalPosition({ -48.0f - (0.5f * Distance), 40.0f, 0.0f });
	
	SlotList[1]->HalfSlotLinker_2->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[1]->HalfSlotLinker_2->GetTransform()->SetLocalPosition({ -48.0f - (0.5f * Distance), -41.0f, 0.0f });

	float4 StartScale_SkillRender = SlotList[1]->BasicScale_SkillRender;
	float4 EndScale_SkillRender = { 76, 76 };

	float4 DestScale = float4::Lerp(StartScale_SkillRender, EndScale_SkillRender, LerpUpRatio);

	SlotList[1]->SkillRender->GetTransform()->SetLocalScale(DestScale);
	
	float4 StartPos_SkillRender = SlotList[1]->BasicPos_SkillRender;
	float4 EndPos_SkillRender = StartPos_SkillRender + float4{ -12.5f, 0.0f };
	float4 DestPos = float4::Lerp(StartPos_SkillRender, EndPos_SkillRender, LerpUpRatio);

	SlotList[1]->SkillRender->GetTransform()->SetLocalPosition({ DestPos });


	return false;
}

bool SkillSlot::LerpSlotScaleDown_Fire(float _DeltaTime)
{
	if (LerpDownRatio >= 1.0f)
	{
		LerpDownRatio = 0.0f;

		SlotList[1]->HalfSlotLinker_1->GetTransform()->SetLocalScale({ 0.0f, 0.0f });
		SlotList[1]->HalfSlotLinker_2->GetTransform()->SetLocalScale({ 0.0f, 0.0f });

		return true;
	}

	LerpDownRatio += 8.0f * _DeltaTime;

	float4 EndPos_Half_1 = SlotList[1]->BasicPos_Half_1;
	float4 StartPos_Half_1 = EndPos_Half_1 + float4{ -25.0f, 0.0f, 0.0f };

	float4 DestPos_Half_1 = float4::Lerp(StartPos_Half_1, EndPos_Half_1, LerpDownRatio);

	SlotList[1]->HalfSlot_1->GetTransform()->SetLocalPosition(DestPos_Half_1);

	float Distance = abs(DestPos_Half_1.x - EndPos_Half_1.x);

	SlotList[1]->HalfSlotLinker_1->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[1]->HalfSlotLinker_1->GetTransform()->SetLocalPosition({ -48.0f - (0.5f * Distance), 40.0f, 0.0f });
	SlotList[1]->HalfSlotLinker_2->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[1]->HalfSlotLinker_2->GetTransform()->SetLocalPosition({ -48.0f - (0.5f * Distance), -41.0f, 0.0f });

	float4 StartScale_SkillRender = { 76, 76 };
	float4 EndScale_SkillRender = SlotList[1]->BasicScale_SkillRender;

	float4 DestScale = float4::Lerp(StartScale_SkillRender, EndScale_SkillRender, LerpDownRatio);

	SlotList[1]->SkillRender->GetTransform()->SetLocalScale(DestScale);

	float4 StartPos_SkillRender = SlotList[1]->BasicPos_SkillRender + float4{ -12.5f, 0.0f };
	float4 EndPos_SkillRender = SlotList[1]->BasicPos_SkillRender;
	float4 DestPos = float4::Lerp(StartPos_SkillRender, EndPos_SkillRender, LerpUpRatio);

	SlotList[1]->SkillRender->GetTransform()->SetLocalPosition({ DestPos });

	return false;
}

bool SkillSlot::LerpSlotScaleUp_Arrow(float _DeltaTime)
{
	if (LerpUpRatio >= 1.0f)
	{
		LerpUpRatio = 0.0f;

		return true;
	}

	LerpUpRatio += 8.0f * _DeltaTime;

	float4 StartPos_Half_1 = SlotList[0]->BasicPos_Half_1;
	float4 EndPos_Half_1 = StartPos_Half_1 + float4{ 0.0f, 25.0f, 0.0f };

	float4 DestPos_Half_1 = float4::Lerp(StartPos_Half_1, EndPos_Half_1, LerpUpRatio);

	SlotList[0]->HalfSlot_1->GetTransform()->SetLocalPosition(DestPos_Half_1);

	float Distance = abs(DestPos_Half_1.y - StartPos_Half_1.y);

	SlotList[0]->HalfSlotLinker_1->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[0]->HalfSlotLinker_1->GetTransform()->SetLocalPosition({ 40.0f, 48.0f + (0.5f * Distance), 0.0f });

	SlotList[0]->HalfSlotLinker_2->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[0]->HalfSlotLinker_2->GetTransform()->SetLocalPosition({ -41.0f,  48.0f + (0.5f * Distance), 0.0f });

	float4 StartScale_SkillRender = SlotList[0]->BasicScale_SkillRender;
	float4 EndScale_SkillRender = { 64, 64 };

	float4 DestScale = float4::Lerp(StartScale_SkillRender, EndScale_SkillRender, LerpUpRatio);
	SlotList[0]->SkillRender->GetTransform()->SetLocalScale(DestScale);

	float4 StartPos_SkillRender = SlotList[0]->BasicPos_SkillRender;
	float4 EndPos_SkillRender = StartPos_SkillRender + float4{0.0f, 12.5f};
	float4 DestPos = float4::Lerp(StartPos_SkillRender, EndPos_SkillRender, LerpUpRatio);

	SlotList[0]->SkillRender->GetTransform()->SetLocalPosition({ DestPos });

	return false;
}

bool SkillSlot::LerpSlotScaleDown_Arrow(float _DeltaTime)
{
	if (LerpDownRatio >= 1.0f)
	{
		LerpDownRatio = 0.0f;

		SlotList[0]->HalfSlotLinker_1->GetTransform()->SetLocalScale({ 0.0f, 0.0f });
		SlotList[0]->HalfSlotLinker_2->GetTransform()->SetLocalScale({ 0.0f, 0.0f });

		return true;
	}

	LerpDownRatio += 8.0f * _DeltaTime;

	float4 EndPos_Half_1 = SlotList[0]->BasicPos_Half_1;
	float4 StartPos_Half_1 = EndPos_Half_1 + float4{ 0.0f, 25.0f, 0.0f };

	float4 DestPos_Half_1 = float4::Lerp(StartPos_Half_1, EndPos_Half_1, LerpDownRatio);

	SlotList[0]->HalfSlot_1->GetTransform()->SetLocalPosition(DestPos_Half_1);

	float Distance = abs(DestPos_Half_1.y - EndPos_Half_1.y);

	SlotList[0]->HalfSlotLinker_1->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[0]->HalfSlotLinker_1->GetTransform()->SetLocalPosition({ 40.0f, 48.0f + (0.5f * Distance), 0.0f });
	SlotList[0]->HalfSlotLinker_2->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[0]->HalfSlotLinker_2->GetTransform()->SetLocalPosition({ -41.0f,  48.0f + (0.5f * Distance), 0.0f });

	float4 StartScale_SkillRender = { 64, 64 };
	float4 EndScale_SkillRender = SlotList[0]->BasicScale_SkillRender;

	float4 DestScale = float4::Lerp(StartScale_SkillRender, EndScale_SkillRender, LerpDownRatio);

	SlotList[0]->SkillRender->GetTransform()->SetLocalScale(DestScale);

	float4 StartPos_SkillRender = SlotList[0]->BasicPos_SkillRender + float4{ 0.0f, 12.5f };
	float4 EndPos_SkillRender = SlotList[0]->BasicPos_SkillRender;
	float4 DestPos = float4::Lerp(StartPos_SkillRender, EndPos_SkillRender, LerpUpRatio);

	SlotList[0]->SkillRender->GetTransform()->SetLocalPosition({ DestPos });
	return false;
}

bool SkillSlot::LerpSlotScaleUp_Bomb(float _DeltaTime)
{
	if (LerpUpRatio >= 1.0f)
	{
		LerpUpRatio = 0.0f;

		return true;
	}

	LerpUpRatio += 8.0f * _DeltaTime;

	float4 StartPos_Half_1 = SlotList[2]->BasicPos_Half_1;
	float4 EndPos_Half_1 = StartPos_Half_1 + float4{ 25.0f, 0.0f, 0.0f };

	float4 DestPos_Half_1 = float4::Lerp(StartPos_Half_1, EndPos_Half_1, LerpUpRatio);

	SlotList[2]->HalfSlot_1->GetTransform()->SetLocalPosition(DestPos_Half_1);

	float Distance = abs(DestPos_Half_1.x - StartPos_Half_1.x);

	SlotList[2]->HalfSlotLinker_1->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[2]->HalfSlotLinker_1->GetTransform()->SetLocalPosition({ 48.0f + (0.5f * Distance), 40.0f, 0.0f });

	SlotList[2]->HalfSlotLinker_2->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[2]->HalfSlotLinker_2->GetTransform()->SetLocalPosition({ 48.0f + (0.5f * Distance), -41.0f, 0.0f });

	float4 StartScale_SkillRender = SlotList[2]->BasicScale_SkillRender;
	float4 EndScale_SkillRender = { 76, 76 };

	float4 DestScale = float4::Lerp(StartScale_SkillRender, EndScale_SkillRender, LerpUpRatio);

	SlotList[2]->SkillRender->GetTransform()->SetLocalScale(DestScale);

	float4 StartPos_SkillRender = SlotList[2]->BasicPos_SkillRender;
	float4 EndPos_SkillRender = StartPos_SkillRender + float4{12.5f, 0.0f};
	float4 DestPos = float4::Lerp(StartPos_SkillRender, EndPos_SkillRender, LerpUpRatio);

	SlotList[2]->SkillRender->GetTransform()->SetLocalPosition({ DestPos });


	return false;
}

bool SkillSlot::LerpSlotScaleDown_Bomb(float _DeltaTime)
{
	if (LerpDownRatio >= 1.0f)
	{
		LerpDownRatio = 0.0f;

		SlotList[2]->HalfSlotLinker_1->GetTransform()->SetLocalScale({ 0.0f, 0.0f });
		SlotList[2]->HalfSlotLinker_2->GetTransform()->SetLocalScale({ 0.0f, 0.0f });

		return true;
	}

	LerpDownRatio += 8.0f * _DeltaTime;

	float4 EndPos_Half_1 = SlotList[2]->BasicPos_Half_1;
	float4 StartPos_Half_1 = EndPos_Half_1 + float4{ 25.0f, 0.0f, 0.0f };

	float4 DestPos_Half_1 = float4::Lerp(StartPos_Half_1, EndPos_Half_1, LerpDownRatio);

	SlotList[2]->HalfSlot_1->GetTransform()->SetLocalPosition(DestPos_Half_1);

	float Distance = abs(DestPos_Half_1.x - EndPos_Half_1.x);

	SlotList[2]->HalfSlotLinker_1->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[2]->HalfSlotLinker_1->GetTransform()->SetLocalPosition({ 48.0f + (0.5f * Distance), 40.0f, 0.0f });
	SlotList[2]->HalfSlotLinker_2->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[2]->HalfSlotLinker_2->GetTransform()->SetLocalPosition({ 48.0f + (0.5f * Distance), -41.0f, 0.0f });

	float4 StartScale_SkillRender = { 76, 76 };
	float4 EndScale_SkillRender = SlotList[2]->BasicScale_SkillRender;

	float4 DestScale = float4::Lerp(StartScale_SkillRender, EndScale_SkillRender, LerpDownRatio);

	SlotList[2]->SkillRender->GetTransform()->SetLocalScale(DestScale);

	float4 StartPos_SkillRender = SlotList[2]->BasicPos_SkillRender + float4{12.5f, 0.0f};
	float4 EndPos_SkillRender = SlotList[2]->BasicPos_SkillRender;
	float4 DestPos = float4::Lerp(StartPos_SkillRender, EndPos_SkillRender, LerpUpRatio);

	SlotList[2]->SkillRender->GetTransform()->SetLocalPosition({ DestPos });

	return false;
}

bool SkillSlot::LerpSlotScaleUp_Hook(float _DeltaTime)
{
	if (LerpUpRatio >= 1.0f)
	{
		LerpUpRatio = 0.0f;

		return true;
	}

	LerpUpRatio += 8.0f * _DeltaTime;

	float4 StartPos_Half_1 = SlotList[3]->BasicPos_Half_1;
	float4 EndPos_Half_1 = StartPos_Half_1 + float4{ 0.0f, -25.0f, 0.0f };

	float4 DestPos_Half_1 = float4::Lerp(StartPos_Half_1, EndPos_Half_1, LerpUpRatio);

	SlotList[3]->HalfSlot_1->GetTransform()->SetLocalPosition(DestPos_Half_1);

	float Distance = abs(DestPos_Half_1.y - StartPos_Half_1.y);

	SlotList[3]->HalfSlotLinker_1->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[3]->HalfSlotLinker_1->GetTransform()->SetLocalPosition({ 41.0f, - 48.0f - (0.5f * Distance), 0.0f });

	SlotList[3]->HalfSlotLinker_2->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[3]->HalfSlotLinker_2->GetTransform()->SetLocalPosition({ -40.0f,  - 48.0f - (0.5f * Distance), 0.0f });

	float4 StartScale_SkillRender = SlotList[3]->BasicScale_SkillRender;
	float4 EndScale_SkillRender = { 76, 76 };

	float4 DestScale = float4::Lerp(StartScale_SkillRender, EndScale_SkillRender, LerpUpRatio);
	SlotList[3]->SkillRender->GetTransform()->SetLocalScale(DestScale);

	float4 StartPos_SkillRender = SlotList[3]->BasicPos_SkillRender;
	float4 EndPos_SkillRender = StartPos_SkillRender + float4{ 0.0f, -12.5f };
	float4 DestPos = float4::Lerp(StartPos_SkillRender, EndPos_SkillRender, LerpUpRatio);

	SlotList[3]->SkillRender->GetTransform()->SetLocalPosition({ DestPos });

	return false;
}

bool SkillSlot::LerpSlotScaleDown_Hook(float _DeltaTime)
{
	if (LerpDownRatio >= 1.0f)
	{
		LerpDownRatio = 0.0f;

		SlotList[3]->HalfSlotLinker_1->GetTransform()->SetLocalScale({ 0.0f, 0.0f });
		SlotList[3]->HalfSlotLinker_2->GetTransform()->SetLocalScale({ 0.0f, 0.0f });

		return true;
	}

	LerpDownRatio += 8.0f * _DeltaTime;

	float4 EndPos_Half_1 = SlotList[3]->BasicPos_Half_1;
	float4 StartPos_Half_1 = EndPos_Half_1 + float4{ 0.0f, -25.0f, 0.0f };

	float4 DestPos_Half_1 = float4::Lerp(StartPos_Half_1, EndPos_Half_1, LerpDownRatio);

	SlotList[3]->HalfSlot_1->GetTransform()->SetLocalPosition(DestPos_Half_1);

	float Distance = abs(DestPos_Half_1.y - EndPos_Half_1.y);

	SlotList[3]->HalfSlotLinker_1->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[3]->HalfSlotLinker_1->GetTransform()->SetLocalPosition({ 41.0f, -48.0f - (0.5f * Distance), 0.0f });
	SlotList[3]->HalfSlotLinker_2->GetTransform()->SetLocalScale({ Distance, 3.5f, 0.0f });
	SlotList[3]->HalfSlotLinker_2->GetTransform()->SetLocalPosition({ -40.0f, -48.0f - (0.5f * Distance), 0.0f });

	float4 StartScale_SkillRender = { 76, 76 };
	float4 EndScale_SkillRender = SlotList[3]->BasicScale_SkillRender;

	float4 DestScale = float4::Lerp(StartScale_SkillRender, EndScale_SkillRender, LerpDownRatio);

	SlotList[3]->SkillRender->GetTransform()->SetLocalScale(DestScale);

	float4 StartPos_SkillRender = SlotList[3]->BasicPos_SkillRender + float4{ 0.0f, -12.5f };
	float4 EndPos_SkillRender = SlotList[3]->BasicPos_SkillRender;
	float4 DestPos = float4::Lerp(StartPos_SkillRender, EndPos_SkillRender, LerpUpRatio);

	SlotList[3]->SkillRender->GetTransform()->SetLocalPosition({ DestPos });
	return false;
}

void SkillSlot::SkillChange()
{
	if (UpdateFunc.size() != 0)
	{
		return;
	}

	if (CurSkill == PrevSkill)
	{
		return;
	}

	if (CurSkill == Player::PlayerSkill::ARROW)
	{
		std::function<bool(float)> Func_1 = std::bind(&SkillSlot::LerpSlotScaleUp_Arrow, this, std::placeholders::_1);
		UpdateFunc.push_back(Func_1);

		if (PrevSkill == Player::PlayerSkill::MAGIC)
		{
			std::function<bool(float)> Func_2 = std::bind(&SkillSlot::LerpSlotScaleDown_Fire, this, std::placeholders::_1);
			UpdateFunc.push_back(Func_2);
		}
		else if (PrevSkill == Player::PlayerSkill::BOMB)
		{
			std::function<bool(float)> Func_2 = std::bind(&SkillSlot::LerpSlotScaleDown_Bomb, this, std::placeholders::_1);
			UpdateFunc.push_back(Func_2);
		}
		else if (PrevSkill == Player::PlayerSkill::HOOK)
		{
			std::function<bool(float)> Func_2 = std::bind(&SkillSlot::LerpSlotScaleDown_Hook, this, std::placeholders::_1);
			UpdateFunc.push_back(Func_2);
		}

		PrevSkill = CurSkill;

	}
	else if (CurSkill == Player::PlayerSkill::MAGIC)
	{
		std::function<bool(float)> Func_1 = std::bind(&SkillSlot::LerpSlotScaleUp_Fire, this, std::placeholders::_1);
		UpdateFunc.push_back(Func_1);

		if (PrevSkill == Player::PlayerSkill::ARROW)
		{
			std::function<bool(float)> Func_2 = std::bind(&SkillSlot::LerpSlotScaleDown_Arrow, this, std::placeholders::_1);
			UpdateFunc.push_back(Func_2);
		}
		else if (PrevSkill == Player::PlayerSkill::BOMB)
		{
			std::function<bool(float)> Func_2 = std::bind(&SkillSlot::LerpSlotScaleDown_Bomb, this, std::placeholders::_1);
			UpdateFunc.push_back(Func_2);
		}
		else if (PrevSkill == Player::PlayerSkill::HOOK)
		{
			std::function<bool(float)> Func_2 = std::bind(&SkillSlot::LerpSlotScaleDown_Hook, this, std::placeholders::_1);
			UpdateFunc.push_back(Func_2);
		}

		PrevSkill = CurSkill;
	}
	else if (CurSkill == Player::PlayerSkill::BOMB)
	{
		std::function<bool(float)> Func_1 = std::bind(&SkillSlot::LerpSlotScaleUp_Bomb, this, std::placeholders::_1);
		UpdateFunc.push_back(Func_1);

		if (PrevSkill == Player::PlayerSkill::ARROW)
		{
			std::function<bool(float)> Func_2 = std::bind(&SkillSlot::LerpSlotScaleDown_Arrow, this, std::placeholders::_1);
			UpdateFunc.push_back(Func_2);
		}
		else if (PrevSkill == Player::PlayerSkill::MAGIC)
		{
			std::function<bool(float)> Func_2 = std::bind(&SkillSlot::LerpSlotScaleDown_Fire, this, std::placeholders::_1);
			UpdateFunc.push_back(Func_2);
		}
		else if (PrevSkill == Player::PlayerSkill::HOOK)
		{
			std::function<bool(float)> Func_2 = std::bind(&SkillSlot::LerpSlotScaleDown_Hook, this, std::placeholders::_1);
			UpdateFunc.push_back(Func_2);
		}

		PrevSkill = CurSkill;
	}
	else if (CurSkill == Player::PlayerSkill::HOOK)
	{
		std::function<bool(float)> Func_1 = std::bind(&SkillSlot::LerpSlotScaleUp_Hook, this, std::placeholders::_1);
		UpdateFunc.push_back(Func_1);

		if (PrevSkill == Player::PlayerSkill::ARROW)
		{
			std::function<bool(float)> Func_2 = std::bind(&SkillSlot::LerpSlotScaleDown_Arrow, this, std::placeholders::_1);
			UpdateFunc.push_back(Func_2);
		}
		else if (PrevSkill == Player::PlayerSkill::MAGIC)
		{
			std::function<bool(float)> Func_2 = std::bind(&SkillSlot::LerpSlotScaleDown_Fire, this, std::placeholders::_1);
			UpdateFunc.push_back(Func_2);
		}
		else if (PrevSkill == Player::PlayerSkill::BOMB)
		{
			std::function<bool(float)> Func_2 = std::bind(&SkillSlot::LerpSlotScaleDown_Bomb, this, std::placeholders::_1);
			UpdateFunc.push_back(Func_2);
		}

		PrevSkill = CurSkill;
	}
}
