#include "PrecompileHeader.h"
#include "ContentsCore.h"
#include <GameEngineCore\GameEngineCore.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

#include "ServerWindow.h"
#include "LevelWindow.h"
#include "MapEditorWindow.h"
#include "BossFrogWindow.h"
#include "PlayerInfoWindow.h"

#include "CenterLevel.h"
#include "TestLevel.h"
#include "MapTestLevel.h"
#include "ServerTestLevel.h"
#include "MapEditorLevel.h"
#include "PhysXTestLevel.h"

#include "StartLevel.h"
#include "OfficeLevel.h"
#include "FortressLevel.h"
#include "BossFrogLevel.h"
#include "UITestLevel.h"
#include "BossTestLevel.h"
#include "OldCrowLevel.h"
#include "ExplainLevel.h"

#include "PlayerTestLevel.h"

ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}



void ContentsCore::GameStart()
{
	ContentsResourcesCreate();

#ifdef _DEBUG
	GameEngineGUI::GUIWindowCreate<GameEngineCoreWindow>("CoreWindow");	
#endif

	GameEngineGUI::GUIWindowCreate<LevelWindow>("LevelWindow");
	GameEngineGUI::GUIWindowCreate<ServerWindow>("ServerWindow");
	GameEngineGUI::GUIWindowCreate<MapEditorWindow>("MapEditorWindow");
	GameEngineGUI::GUIWindowCreate<BossFrogWindow>("BossFrogWindow");
	GameEngineGUI::GUIWindowCreate<PlayerInfoWindow>("PlayerInfoWindow");
	

	//InstallFont();

	GameEngineCore::CreateLevel<CenterLevel>();
	//GameEngineCore::CreateLevel<TestLevel>();
	//GameEngineCore::CreateLevel<ServerTestLevel>();
	GameEngineCore::CreateLevel<MapEditorLevel>();
	GameEngineCore::CreateLevel<PhysXTestLevel>();
	GameEngineCore::CreateLevel<StartLevel>();
	GameEngineCore::CreateLevel<UITestLevel>();
	GameEngineCore::CreateLevel<MapTestLevel>();
	GameEngineCore::CreateLevel<OfficeLevel>();
	GameEngineCore::CreateLevel<FortressLevel>();
	GameEngineCore::CreateLevel<BossFrogLevel>();
	GameEngineCore::CreateLevel<OldCrowLevel>();
	GameEngineCore::CreateLevel<PlayerTestLevel>();
	GameEngineCore::CreateLevel<BossTestLevel>();
	GameEngineCore::CreateLevel<ExplainLevel>();

	GameEngineCore::ChangeLevel("StartLevel");
}

void ContentsCore::GameEnd()
{
	MapEditorWindow::EditorGUI->ReleaseMapEditor();
	//RemoveFont();
}


void ContentsCore::InstallFont()
{
	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("ContentResources");
	Dir.Move("ContentResources");
	Dir.Move("Text");
	std::vector<GameEngineFile> FontFiles = Dir.GetAllFile({ ".otf", ".ttf" });
	for (GameEngineFile& File : FontFiles)
	{
		if (0 == AddFontResourceA(File.GetFullPath().c_str()))
		{
			MsgAssert("폰트 설치에 실패했습니다.");
			return;
		}

		SendMessage(GameEngineWindow::GetHWnd(), WM_FONTCHANGE, NULL, NULL);
	}
}

void ContentsCore::RemoveFont()
{
	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("ContentResources");
	Dir.Move("ContentResources");
	Dir.Move("Text");
	std::vector<GameEngineFile> FontFiles = Dir.GetAllFile({ ".otf", ".ttf" });
	for (GameEngineFile& File : FontFiles)
	{
		if (0 == RemoveFontResourceA(File.GetFullPath().data()))
		{
			MsgAssert("폰트 삭제에 실패했습니다.");
			return;
		}

		SendMessage(GameEngineWindow::GetHWnd(), WM_FONTCHANGE, NULL, NULL);
	}
}
