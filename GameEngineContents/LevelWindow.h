#pragma once
#include <GameEngineCore/GameEngineGUI.h>

// Ό³Έν :
class LevelWindow : public GameEngineGUIWindow
{
public:
	// constrcuter destructer
	LevelWindow();
	~LevelWindow();

	// delete Function
	LevelWindow(const LevelWindow& _Other) = delete;
	LevelWindow(LevelWindow&& _Other) noexcept = delete;
	LevelWindow& operator=(const LevelWindow& _Other) = delete;
	LevelWindow& operator=(LevelWindow&& _Other) noexcept = delete;

protected:
	virtual void Start() override;
	virtual void OnGUI(std::shared_ptr<class GameEngineLevel> Level, float _DeltaTime) override;

private:
	float m_fFrameTime = 0.f;
	float m_fDeltaTime = 0.f;
	float m_fFrameRate = 0.f;
	std::string m_CurLevelName = "";

	std::string m_CurCameraMode = "Play Mode";
};

