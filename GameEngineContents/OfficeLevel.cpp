#include "PrecompileHeader.h"
#include "OfficeLevel.h"

#include "Map_Office.h"
#include "Player.h"
#include "PhysXCapsuleComponent.h"
#include "PhysXControllerComponent.h"

#include "Dust.h"
#include "ShortCutDoor.h"
#include "FadeEffect.h"
#include "FadeWhite.h"

#include <GameEngineCore/GameEngineCoreWindow.h>
#include "PlayerInfoWindow.h"

OfficeLevel::OfficeLevel()
{
}

OfficeLevel::~OfficeLevel()
{
}

void OfficeLevel::Start()
{
	// SetLevelType(PacketLevelType::OfficeLevel);
	InitKey();
	SetPointLight();

	std::shared_ptr<FadeWhite>pWhite = CreateActor<FadeWhite>();
	pWhite->FadeIn();
	pWhite->FadeUpdate();
	//m_pFadeEffect = GetLastTarget()->CreateEffect<FadeEffect>();
	//m_pFadeEffect.lock()->FadeIn();


}


void OfficeLevel::Update(float _DeltaTime)
{
	KeyUpdate(_DeltaTime);

	// test 
	if (false == GetMainCamera()->IsFreeCamera())
	{
		float4 nextPos = Player::MainPlayer->GetTransform()->GetWorldPosition();
		nextPos.y += 1000.0f;
		nextPos.z -= 1000.0f * tanf((90.0f - m_CameraRot.x) * GameEngineMath::DegToRad);
		GetMainCamera()->GetTransform()->SetWorldPosition(float4::LerpClamp(GetMainCamera()->GetTransform()->GetWorldPosition(),nextPos, _DeltaTime * 3.0f));
	}

	GraphicUpdate();
}

void OfficeLevel::LevelChangeStart()
{
	LevelInit({800, 450, 800, 450});
	CreateScene();

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalRotation(m_CameraRot);
	GetMainCamera()->GetTransform()->SetLocalPosition(m_CameraPos);
	GetMainCamera()->SetZoomRatio(2.5f);

	CreateNewCamera(-1);
	GetCamera(-1)->SetProjectionType(CameraType::Orthogonal);

	Create_Light();
	Create_Map();
	Create_TriggerObject();
	Create_Player();

	CreateActor<Dust>();

	MainBGM = GameEngineSound::Play("OfficeLevel_BGM.mp3");
	MainBGM.SetLoop();
	MainBGM.SetLoopPoint(0, 152);

	MainBGM.SoundFadeIn(2.0f);

	PlayerInfoWindow::PlayerGUI->On();

	// 테스트용 커밋~ 
}

void OfficeLevel::LevelChangeEnd()
{
	PlayerInfoWindow::PlayerGUI->Off();

	MainBGM.SoundFadeOut(1.0f);

	AllActorDestroy();
}

void OfficeLevel::Create_Light()
{
	std::shared_ptr<GameEngineLight> Light = CreateActor<GameEngineLight>();
	Light->GetTransform()->SetLocalRotation(float4{ 60, 0, 0 });
}

void OfficeLevel::Create_Map()
{
	m_pMap = CreateActor<Map_Office>();
}

void OfficeLevel::Create_Player()
{
	// 플레이어 생성후 Set_StartPos함수 호출하면 해당 위치에 세팅
	std::shared_ptr<Player> Obj = CreateActor<Player>();

	Set_PlayerStartPos();

	if (false == GetMainCamera()->IsFreeCamera())
	{
		float4 nextPos = Player::MainPlayer->GetTransform()->GetWorldPosition();
		nextPos.y += 3000.0f;
		nextPos.z -= 3000.0f * tanf((90.0f - m_CameraRot.x) * GameEngineMath::DegToRad);
		GetMainCamera()->GetTransform()->SetWorldPosition(nextPos);
	}
}


void OfficeLevel::InitKey()
{
	if (false == GameEngineInput::IsKey("NaviMesh_Switch"))
	{
		GameEngineInput::CreateKey("NaviMesh_Swtich", 'M');
	}
}

void OfficeLevel::KeyUpdate(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("NaviMesh_Swtich"))
	{
		if (nullptr != m_pMap)
		{
			m_pMap->NaviRenderSwitch();
		}
	}
}

void OfficeLevel::Set_PlayerStartPos()
{
	if (nullptr == Player::MainPlayer)
	{
		MsgAssert("Player 가 nullptr 입니다.");
		return;
	}

	std::shared_ptr<PhysXControllerComponent> Comp = Player::MainPlayer->GetPhysXComponent();

	if (nullptr == Comp)
	{
		MsgAssert("Player 의 PhysXComponent가 nullptr 입니다.");
		return;
	}
	
	switch (m_eType)
	{
	case PrevLevelType::OldCrowLevel:
	{
		Comp->SetWorldPosWithParent(m_f4OldCrowToOfficePos, float4::ZERO);
		return;
	}
		break;
	case PrevLevelType::FortressLevel:
	{
		Comp->SetWorldPosWithParent(m_f4FortressToOfficePos, float4::ZERO);
		return;
		break;
	}
	case PrevLevelType::BossFrogLevel:
		break;
	case PrevLevelType::None:
		break;
	}

	if (GameEngineCore::GetPrevLevel().get()->GetNameToString() == ("EXPLAINLEVEL"))
	{
		Comp->SetWorldPosWithParent(m_f4ExplainToOfficePos, float4::ZERO);
		std::shared_ptr<FadeWhite>pWhite = CreateActor<FadeWhite>();
		pWhite->FadeIn();
		pWhite->FadeUpdate();
		return;
	}

	Comp->SetWorldPosWithParent(m_StartPos, float4::ZERO);
}

void OfficeLevel::SetPointLight()
{
	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 3375 , 75 , -5050 },.MaxDist = 200.0f,.Intensity = 10.0f });
	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 2010 , 75 , -5050 },.MaxDist = 150.0f,.Intensity = 10.0f });
	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 910 , 75 , -5050 },.MaxDist = 150.0f,.Intensity = 10.0f });

	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 875 , 125 ,-3200 },.MaxDist = 150.0f,.Intensity = 10.0f });

	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { -850 ,550 , -3125 },.MaxDist = 150.0f,.Intensity = 10.0f });
	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 250 , 850 , 1000 },.MaxDist = 150.0f,.Intensity = 10.0f });

	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 2825 , 825 , 2325 },.MaxDist = 150.0f,.Intensity = 10.0f });
	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 3150 , 950 , 2350 },.MaxDist = 150.0f,.Intensity = 10.0f });

	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 525 , 1450 , 4225 },.MaxDist = 100.0f,.Intensity = 5.0f });
}

void OfficeLevel::Create_TriggerObject()
{
	{
		// 포트리스 포탈 
		std::shared_ptr<ShortCutDoor> Obj = CreateActor<ShortCutDoor>();
		Obj->GetPhysXComponent()->SetWorldPosWithParent(float4{ 1164,1256, 5221 }, float4{ 0 , -45, 0 });
		Obj->SetTriggerFunction([=]
			{
				std::shared_ptr<GameEngineLevel> NextLevel = GameEngineCore::ChangeLevel("FortressLevel");
				std::shared_ptr<OfficeLevel> Level = NextLevel->DynamicThis<OfficeLevel>();
			}
		);


		if (PrevLevelType::FortressLevel == m_eType)
		{
			Obj->SetState(StartState::OPEN);
		}
	}

	{
		// 까마귀 포탈 
		std::shared_ptr<ShortCutDoor> Obj = CreateActor<ShortCutDoor>();
		Obj->GetPhysXComponent()->SetWorldPosWithParent(float4{ -1168, 1656, 6259 });
		Obj->SetTriggerFunction([=]
			{
				GameEngineCore::ChangeLevel("OldCrowLevel");
			}
		);
	}
	{
		// 소개레벨 포탈 
		std::shared_ptr<ShortCutDoor> Obj = CreateActor<ShortCutDoor>();
		Obj->GetPhysXComponent()->SetWorldPosWithParent(float4{ 1065, -720, -5131 });
		Obj->SetTriggerFunction([=]
			{
				GameEngineCore::ChangeLevel("ExplainLevel");
			}
		);
	}
}

