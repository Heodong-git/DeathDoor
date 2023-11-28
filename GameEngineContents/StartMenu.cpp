#include "PrecompileHeader.h"
#include "StartMenu.h"

#include "ContentUIFontRenderer.h"
#include "ContentUIRenderer.h"
#include "FadeEffect.h"

StartMenu::StartMenu()
{
}

StartMenu::~StartMenu()
{
}

void StartMenu::Start()
{
	Set_Logo();
	Set_MenuButton();
	Set_SelectedTexture();
	Set_RealStartButton();

	CreateKey();
	
	m_pFadeEffect = GetLevel()->GetLastTarget()->CreateEffect<FadeEffect>();
}

void StartMenu::Update(float _DeltaTime)
{
	if (isReady == true && GameEngineInput::IsDown("MyTest"))
	{
		//레벨체인지
		//페이드아웃
		GameEngineSound::Play("ButtonSelectedSFX.mp3");
		m_pFadeEffect->FadeOut();
		m_bIsFadeEffect = true;
	}

	if(isSelectUpdate == true)
	{
		Move_SelectedTexture(_DeltaTime);
		SelectMenu();
	}

	if (GameEngineInput::IsDown("MyTest") == true && isReady == false)
	{
		MenuButtonList[0]->ClickEvent();
		isReady = true;

	}

	if (true == m_bIsFadeEffect)
	{
		if (0.0f >= m_fFadeTime)
		{
			GameEngineCore::ChangeLevel("ExplainLevel");
			return;
		}

		m_fFadeTime -= _DeltaTime;
	}

	if (UpdateFunc != nullptr)
	{
		if (true == UpdateFunc(_DeltaTime))
		{
			UpdateFunc = nullptr;
		}
	}
}

void StartMenu::Render(float _DeltaTime)
{

}

void StartMenu::CreateKey()
{
	if (GameEngineInput::IsKey("MyTest") == false)
	{
		GameEngineInput::CreateKey("MyTest", VK_RETURN);
	}

	if (GameEngineInput::IsKey("MenuDown") == false)
	{
		GameEngineInput::CreateKey("MenuDown", VK_DOWN);
	}

	if (GameEngineInput::IsKey("MenuUp") == false)
	{
		GameEngineInput::CreateKey("MenuUp", VK_UP);
	}
}

void StartMenu::Set_Logo()
{
	LogoRender = CreateComponent<GameEngineUIRenderer>();
	LogoRender->SetTexture("Logo_DeathDoor.png");

	LogoRender->GetTransform()->SetLocalScale({ 592, 394 });
	LogoRender->GetTransform()->SetLocalPosition({ 0, 200 });
}

void StartMenu::Set_SelectedTexture()
{
	Arrow_Left = CreateComponent<ContentUIRenderer>();
	Arrow_Left->SetScaleToTexture("SelectedMenu.png");
	Arrow_Left->GetTransform()->SetLocalScale({ 78.0f, 77.0f, 0.0f });
	Arrow_Left->GetTransform()->SetLocalRotation({0.0f, 0.0f, -90.0f});
	Arrow_Left->GetTransform()->SetLocalPosition({-150.0f, -185.0f, 0.0f});

	Arrow_Right = CreateComponent<ContentUIRenderer>();
	Arrow_Right->SetScaleToTexture("SelectedMenu.png");
	Arrow_Right->GetTransform()->SetLocalScale({ 78.0f, 77.0f, 0.0f });
	Arrow_Right->GetTransform()->SetLocalRotation({ 00.0f, 00.0f, 90.0f });
	Arrow_Right->GetTransform()->SetLocalPosition({ 150.0f, -185.0f, 0.0f });
	Arrow_Right->ColorOptionValue.MulColor = { 1.1f, 1.1f, 1.1f, 1.0f };
}

void StartMenu::Set_MenuButton()
{
	std::shared_ptr<MenuButton> StartButton = std::make_shared<MenuButton>();
	
	StartButton->TextShadow = CreateComponent<ContentUIFontRenderer>();
	StartButton->TextShadow->SetFont("맑은 고딕");
	StartButton->TextShadow->SetText("시작");
	StartButton->TextShadow->SetScale(40.0f);
	StartButton->TextShadow->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	StartButton->TextShadow->SetFontFlag(FW1_TEXT_FLAG::FW1_CENTER);
	StartButton->TextShadow->GetTransform()->SetLocalPosition({ 0, -161.0f, 0.0f });

	StartButton->Text = CreateComponent<ContentUIFontRenderer>();
	StartButton->Text->SetFont("맑은 고딕");
	StartButton->Text->SetText("시작");
	StartButton->Text->SetScale(40.0f);
	StartButton->Text->SetColor({ 0.8f, 0.8f, 0.8f, 1.0f });
	StartButton->Text->SetFontFlag(FW1_TEXT_FLAG::FW1_CENTER);
	StartButton->Text->GetTransform()->SetLocalPosition({ 0, -160.0f, 0.0f });

	StartButton->Collision = CreateComponent<GameEngineCollision>();
	StartButton->Collision->GetTransform()->SetLocalScale({ 40, 30 });
	StartButton->Collision->GetTransform()->SetLocalPosition({ 0, -185.0f, 0.0f });
	StartButton->Collision->SetColType(ColType::AABBBOX2D);
	
	StartButton->StartPos = { 0.0f, -160.0f, 0.0f };
	//설정해줘야 함.

	//StartButton->Collision->SetOrder();
	
	std::weak_ptr<MenuButton> Weak_St_Bt = StartButton;
	StartButton->ColEvent = [=] 
	{
		if(Weak_St_Bt.lock()->isSelected == false)
		{
			MenuButtonList[ButtonIndex]->isSelected = false;
			ButtonIndex = 0;

			MenuButtonList[ButtonIndex]->isSelected = true;
		}
	};

	StartButton->ClickEvent = [=]
	{
		GameEngineSound::Play("ButtonSelectedSFX.mp3");

		isSelectUpdate = false;
		UpdateFunc = std::bind(&StartMenu::LerpArrowScale, this, std::placeholders::_1);
	};

	MenuButtonList.push_back(StartButton);

	std::shared_ptr<MenuButton> ExitButton = std::make_shared<MenuButton>();

	ExitButton->TextShadow = CreateComponent<ContentUIFontRenderer>();
	ExitButton->TextShadow->SetFont("맑은 고딕");
	ExitButton->TextShadow->SetText("종료");
	ExitButton->TextShadow->SetScale(40.0f);
	ExitButton->TextShadow->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	ExitButton->TextShadow->SetFontFlag(FW1_TEXT_FLAG::FW1_CENTER);
	ExitButton->TextShadow->GetTransform()->SetLocalPosition({ 0, -251.0f, 0.0f });

	ExitButton->Text = CreateComponent<ContentUIFontRenderer>();
	ExitButton->Text->SetFont("맑은 고딕");
	ExitButton->Text->SetText("종료");
	ExitButton->Text->SetScale(40.0f);
	ExitButton->Text->SetColor({ 0.8f, 0.8f, 0.8f, 1.0f });
	ExitButton->Text->SetFontFlag(FW1_TEXT_FLAG::FW1_CENTER);
	ExitButton->Text->GetTransform()->SetLocalPosition({ 0, -250.0f, 0.0f });

	ExitButton->Collision = CreateComponent<GameEngineCollision>();
	ExitButton->Collision->GetTransform()->SetLocalScale({ 40, 30 });
	ExitButton->Collision->GetTransform()->SetLocalPosition({ 0, -275.0f, 0.0f });
	ExitButton->Collision->SetColType(ColType::AABBBOX2D);
	 
	ExitButton->StartPos = { 0.0f, -250.0f, 0.0f };

	//설정해줘야 함.
	//StartButton->Collision->SetOrder();
	
	std::weak_ptr<MenuButton> Weak_Ex_Bt = ExitButton;
	ExitButton->ColEvent = [=]
	{
		if (Weak_Ex_Bt.lock()->isSelected == false)
		{
			MenuButtonList[ButtonIndex]->isSelected = false;
			ButtonIndex = 1;

			MenuButtonList[ButtonIndex]->isSelected = true;
		}
	};

	ExitButton->ClickEvent = [=]
	{
		GameEngineSound::Play("ButtonSelectedSFX.mp3");

		exit(0);
	};

	MenuButtonList.push_back(ExitButton);
}

void StartMenu::Set_RealStartButton()
{
	RealStart = std::make_shared<RealStartButton>();

	RealStart->Window = CreateComponent<ContentUIRenderer>();
	RealStart->Window->SetScaleToTexture("TextWindow.png");
	RealStart->Window->GetTransform()->SetLocalPosition({ -1200.0f, 0.0f, 0.0f });

	RealStart->Collision = CreateComponent<GameEngineCollision>();
	RealStart->Collision->GetTransform()->SetLocalScale(RealStart->Window->GetTransform()->GetLocalScale());
	RealStart->Collision->GetTransform()->SetLocalPosition({-1200.0f, 0.0f, 0.0f});
	RealStart->Collision->SetColType(ColType::AABBBOX2D);
	//RealStart->Collision->SetOrder();
}

void StartMenu::Move_SelectedTexture(float _DeltaTime)
{
	MoveAngle += 5.0f * _DeltaTime;
	
	if (MoveAngle >= 360.0f)
	{
		MoveAngle -= 360.0f;
	}

	float Sin = sin(MoveAngle);
	float Cos = cos(MoveAngle);

	Arrow_Left->GetTransform()->SetLocalPosition({ -150.0f + 7.5f * Cos, -185.0f + ButtonIndex * -90.0f ,  7.5f * Sin});
	Arrow_Right->GetTransform()->SetLocalPosition({ 150.0f - 7.5f * Cos, -185.0f + ButtonIndex * -90.0f,  7.5f * Sin });
}

void StartMenu::SelectMenu()
{
	if(GameEngineInput::IsDown("MenuDown") == true)
	{
		if (ButtonIndex < MenuButtonList.size() - 1)
		{
			GameEngineSound::Play("MenuNavigation2021.mp3");

			ButtonIndex++;
			MenuButtonList[ButtonIndex]->isSelected = true;
			MenuButtonList[ButtonIndex - 1]->isSelected = false;
		}
	}

	if (GameEngineInput::IsDown("MenuUp") == true)
	{
		if (ButtonIndex >= 1)
		{
			GameEngineSound::Play("MenuNavigation2021.mp3");

			ButtonIndex--;
			MenuButtonList[ButtonIndex]->isSelected = true;
			MenuButtonList[ButtonIndex + 1]->isSelected = false;
		}
	}

	for (int i = 0; i < MenuButtonList.size(); i++)
	{
		//마우스와 충돌하면 해당 버튼의 ColEvent 실행
	}
}

bool StartMenu::LerpArrowScale(float _DeltaTime)
{
	if (LerpRatio >= 1.0f)
	{
		LerpRatio = 0.0f;
		UpdateFunc = std::bind(&StartMenu::LerpArrowPos, this, std::placeholders::_1);

		LeftArrowPos = Arrow_Left->GetTransform()->GetLocalPosition();
		RightArrowPos = Arrow_Right->GetTransform()->GetLocalPosition();

		return false;
	}

	LerpRatio += 2.0f * _DeltaTime;

	float4 StartArrowScale = { 78.0f, 77.0f, 0.0f };
	float4 EndArrowScale = { 57.0f, 56.0f, 0.0f };

	float4 DestArrowScale = float4::Lerp(StartArrowScale, EndArrowScale, LerpRatio);

	Arrow_Left->GetTransform()->SetLocalScale(DestArrowScale);
	Arrow_Right->GetTransform()->SetLocalScale(DestArrowScale);

	return false;
}


bool StartMenu::LerpArrowPos(float _DeltaTime)
{
	if (LerpRatio >= 1.0f)
	{
		LerpRatio = 0.0f;
		UpdateFunc = std::bind(&StartMenu::HideMenu, this, std::placeholders::_1);

		return false;
	}

	LerpRatio += 3.0f * _DeltaTime;

	float4 StartLeftArrowPos = LeftArrowPos;
	float4 EndLeftArrowPos = { -100.0f , LeftArrowPos.y, LeftArrowPos.z};

	float4 StartRightArrowPos = RightArrowPos;
	float4 EndRightArrowPos = { 100.0f , RightArrowPos.y, RightArrowPos.z };

	float4 DestLeftArrowPos = float4::Lerp(StartLeftArrowPos, EndLeftArrowPos, LerpRatio);
	float4 DestRightArrowPos = float4::Lerp(StartRightArrowPos, EndRightArrowPos, LerpRatio);

	Arrow_Left->GetTransform()->SetLocalPosition(DestLeftArrowPos);
	Arrow_Right->GetTransform()->SetLocalPosition(DestRightArrowPos);

	return false;
}

bool StartMenu::HideMenu(float _DeltaTime)
{
	LerpRatio += 3.0f * _DeltaTime;

	if (LerpRatio >= 1.0f)
	{
		LerpRatio = 0.0f;
		UpdateFunc = std::bind(&StartMenu::AppearRealStart, this, std::placeholders::_1);

		return false; 
	}

	for (int i = 0; i < MenuButtonList.size(); i++)
	{
		float4 StartPos = MenuButtonList[i]->StartPos;
		float4 EndPos = StartPos + float4{0.0f, -400.0f, 0.0f};

		float4 DestPos = float4::Lerp(StartPos, EndPos, LerpRatio);

		MenuButtonList[i]->Text->GetTransform()->SetLocalPosition(DestPos);
		MenuButtonList[i]->TextShadow->GetTransform()->SetLocalPosition(DestPos + float4{0.0f, -1.0f, 0.0f});
		MenuButtonList[i]->Collision->GetTransform()->SetLocalPosition(DestPos + float4{ 0.0f, -25.0f, 0.0f });
	}

	float4 LogoStartPos = { 0.0f, 200.0f, 0.0f };
	float4 LogoEndPos = LogoStartPos + float4{ 0.0f, 500.0f, 0.0f };
	
	float4 LogoDestPos = float4::Lerp(LogoStartPos, LogoEndPos, LerpRatio);

	LogoRender->GetTransform()->SetLocalPosition(LogoDestPos);

	float4 SelectedMenuPos = MenuButtonList[ButtonIndex]->Text->GetTransform()->GetLocalPosition();

	float4 LeftArrowPos = SelectedMenuPos + float4{ Arrow_Left->GetTransform()->GetLocalPosition().x, -25.0f, 0.0f };
	float4 RightArrowPos = SelectedMenuPos + float4{ Arrow_Right->GetTransform()->GetLocalPosition().x, -25.0f, 0.0f };
	
	Arrow_Left->GetTransform()->SetLocalPosition(LeftArrowPos);
	Arrow_Right->GetTransform()->SetLocalPosition(RightArrowPos);

	return false;
}

bool StartMenu::AppearRealStart(float _DeltaTime)
{
	LerpRatio += 3.0f * _DeltaTime;

	if (LerpRatio >= 1.0f)
	{
		LerpRatio = 0.0f;

		RealStart->Window->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
		RealStart->Collision->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });

		return true;
	}

	float4 StartPos = { -1200.0f, 0.0f, 0.0f };
	float4 EndPos = { 0.0f, 0.0f, 0.0f };
	
	float4 DestPos = float4::Lerp(StartPos, EndPos, LerpRatio);

	RealStart->Window->GetTransform()->SetLocalPosition(DestPos);
	RealStart->Collision->GetTransform()->SetLocalPosition(DestPos);

	return false;
}