#pragma once
#include <GameEngineCore/GameEngineFBXRenderer.h>

class ContentFBXRenderer : public GameEngineFBXRenderer
{

public:

	ContentFBXRenderer();
	~ContentFBXRenderer();

	ContentFBXRenderer(const ContentFBXRenderer& _Other) = delete;
	ContentFBXRenderer(ContentFBXRenderer&& _Other) noexcept = delete;
	ContentFBXRenderer& operator=(const ContentFBXRenderer& _Other) = delete;
	ContentFBXRenderer& operator=(ContentFBXRenderer&& _Other) noexcept = delete;

	void SetAllUnitTexture(const std::string_view& _SettingName, const std::string_view& _ImageName);
	
	void SetFadeMask(const std::string_view& _MaskTextureName = "MaskType2_0.png");
	void SetCrackMask();

	void FadeOut(float _MaxTime, float _DeltaTime);
	void FadeIn(float _MaxTime, float _DeltaTime);

	void SetFBXMesh(const std::string& _MeshName, const std::string _SettingName) override;
	void SetCrackAmount(float _Amount);

	void SetBlurColor(float4 _Color)
	{
		BlurColor = _Color;
	}

	void SetClipData(float4 _ClipData)
	{
		ClipData = _ClipData;
	}

	void SetGlowToUnit(int _IndexY, int _IndexX, const std::string_view& _MaskName = "WholeMask.png")
	{
		GetAllRenderUnit()[_IndexY][_IndexX]->ShaderResHelper.SetTexture("CrackTexture", _MaskName);
		GetAllRenderUnit()[_IndexY][_IndexX]->Mask.UV_MaskingValue = 1.0f;
	}

protected:
	void Start() override;
	void Render(float _DeltaTime) override;
private:
	void LinkConstantBuffer();
	float4 BlurColor = {0.99f, 0.356f, 0.407f};
	float4 ClipData = { 0.0f, 0.0f, 1.0f, 1.0f };

	std::string MaterialName;
};

