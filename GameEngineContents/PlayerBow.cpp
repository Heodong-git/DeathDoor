#include "PreCompileHeader.h"
#include "PlayerBow.h"
#include "ContentFBXRenderer.h"
#include "PlayerDefinition.h"

PlayerBow::PlayerBow() 
{
}

PlayerBow::~PlayerBow() 
{
}


void PlayerBow::Start()
{
	BowRenderer = CreateComponent< ContentFBXRenderer>(); // BOW_MESH
	BowRenderer->SetFBXMesh("BOW_MESH.fbx", "ContentMeshDeffered");
	BowRenderer->GetTransform()->SetLocalScale(PLAYER_BOW_SCALE);
	BowRenderer->GetTransform()->SetLocalRotation(PLAYER_BOW_DEFAULT_DIR);
	BowRenderer->SetGlowToUnit(0,0);
	BowRenderer->SetColor({ 0.95f, 0.20f, 0.25f }, 2.0f);
}


void PlayerBow::SetTrans(const float4& _Pos, const float4& Rot)
{
	GetTransform()->SetWorldPosition(_Pos);
	GetTransform()->SetWorldRotation(Rot);
}
