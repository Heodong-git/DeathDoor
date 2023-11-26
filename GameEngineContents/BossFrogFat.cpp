#include "PreCompileHeader.h"
#include <GameEngineBase/GameEngineRandom.h>
#include "BossFrogFat.h"
#include "BossFrogBomb.h"
#include "DustParticle.h"
#include "ContentLevel.h"
#include "HitParticle.h"
#include "PhysXBoxComponent.h"
#include "Particle3D.h"
#include "ShockWave.h"
#include "WaterDrop.h"
//#include "BossFrogSoul.h"

BossFrogFat::BossFrogFat()
{
}

BossFrogFat::~BossFrogFat()
{
}




void BossFrogFat::Start()
{
	EnemyBase::Start();
	BossFrog::Start();
	// physx
	{
		m_pCapsuleComp = CreateComponent<PhysXControllerComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
		m_pCapsuleComp->CreatePhysXActors(float4{0.0f,1000.0f,400.0f});
		m_pCapsuleComp->TurnOffGravity();
		m_pCapsuleComp->SetRotation(GetTransform()->GetWorldRotation() + float4{ 0.0f, 135.0f,0.0f });
		m_pCapsuleComp->SetFilterData(PhysXFilterGroup::MonsterDynamic);
		if (nullptr != Player::MainPlayer)
		{
			m_pCapsuleComp->SetFilter(*Player::MainPlayer->GetPhysXComponent()->GetController());
			m_pCapsuleComp->RigidSwitch(false);
		}
		m_pCapsuleComp->SetWorldPosWithParent(BossFrog::WPointNorth);
		m_pCapsuleComp->CreateSubShape(SubShapeType::BOX, float4{ 800.0f,200.0f,200.0f }, float4{ 0.0f, 100.0f, 400.0f },true);
		m_pCapsuleComp->SetSubShapeFilter(PhysXFilterGroup::MonsterDynamic);
	}

	if (false == GameEngineInput::IsKey("PressK"))
	{
		GameEngineInput::CreateKey("PressK", 'K');
	}
	BGMSound = GameEngineSound::Play("SwampKingPhase2MainLoop.mp3");
	BGMSound.SetLoopPoint(0.0f, 48.0f);
	BGMSound.SetLoop();
	SetEnemyHP(30);
}
float4 BossFrogFat::GetRandomTileIndex()
{
	int cnt = 0;
	while (true)
	{
		if (cnt > 50)
		{
			return float4::ZERONULL;
		}
		int y = GameEngineRandom::MainRandom.RandomInt(0, 4);
		int x = GameEngineRandom::MainRandom.RandomInt(0, 4);
		if (true == IsTile(y, x))
		{
			return float4{ static_cast<float>(x),static_cast<float>(y) };
		}
		cnt++;
	}
}

float4 BossFrogFat::GetRandomTilePos()
{
	float4 index = GetRandomTileIndex();
	if (index == float4::ZERONULL)
	{
		return index;
	}
	return GetTilePos(index.iy(), index.ix());
}

float Time = 0.0f;

void BossFrogFat::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("PressK"))
	{
		SetNextState(BossFrogFatState::DEATH);
		SetFrogDeath();
	}

	AnimationBoneData BoneData = EnemyRenderer->GetBoneData("_FROG_SEPTRE_BONE");
	WeaponRenderer->GetTransform()->SetLocalPosition(WeaponPivotPos + BoneData.Pos);
	WeaponRenderer->GetTransform()->SetLocalRotation(WeaponPivotRot + BoneData.RotEuler);
	
	if (true == DeathCheck())
	{
		SetNextState(BossFrogFatState::DEATH);
		SetFrogDeath();
	}

	FSMObjectBase::Update(_DeltaTime);
}

void BossFrogFat::InitAnimation()
{
	EnemyRenderer = CreateComponent<ContentFBXRenderer>();
	EnemyRenderer->SetFBXMesh("FROG_FAT_MESH.FBX", "ContentAniMeshDeffered");


	EnemyRenderer->CreateFBXAnimation("DIE_LAND", "FROG_FAT_DIE_LAND.fbx", { 1.0f / 30, false });					//¿£µù
	EnemyRenderer->CreateFBXAnimation("DIE_STANDING", "FROG_FAT_DIE_STANDING.fbx", { 1.0f / 30, false });

	EnemyRenderer->CreateFBXAnimation("JUMP_SCREAM", "FROG_FAT_JUMP_SCREAM.fbx", { 1.0f / 30, false });				   //ÀÎÆ®·Î
	EnemyRenderer->FadeOut(0.01f, 0.01f);

	EnemyRenderer->SetAnimationStartFunc("JUMP_SCREAM", 10, [this]
		{
			GameEngineSound::Play("Frog_Phase2_JumpPrevScream.mp3");
		});
	EnemyRenderer->SetAnimationStartFunc("JUMP_SCREAM", 47, [this]
		{
			JumpStartPoint = OnGroundCenter;
			JumpEndPoint = FatPointSouth;
			CalJumpPoint();
			ResetStateDuration();
			isJumpTime = true;
		});
	EnemyRenderer->SetAnimationStartFunc("JUMP_SCREAM", 61, [this]
		{
			GameEngineSound::Play("Frog_Phase2_LandToGround.mp3");
		});
	EnemyRenderer->SetAnimationStartFunc("JUMP_SCREAM", 77, [this]
		{
			isJumpTime = false;
		});
	EnemyRenderer->CreateFBXAnimation("IDLE", "FROG_FAT_IDLE.fbx", { 1.0f / 30, true });
	
	EnemyRenderer->CreateFBXAnimation("FAT_JUMP", "FROG_FAT_JUMP.fbx", { 1.0f / 30, false });						   //¹Ù´Ú to ¹° Â«Çª
	EnemyRenderer->SetAnimationStartFunc("FAT_JUMP", 33, [this]
		{
			GameEngineSound::Play("Frog_Phase2_JumpScream.mp3");
		});
	EnemyRenderer->SetAnimationStartFunc("FAT_JUMP", 36, [this]// 40~70 ÇÁ·¹ÀÓ Á¡ÇÁ // 1ÃÊ°£ Á¡ÇÁ
		{
			ResetStateDuration();
			isJumpTime = true;
		});
	EnemyRenderer->SetAnimationStartFunc("FAT_JUMP", 56, [this]
		{
			GameEngineSound::Play("Frog_Phase2_LandToGround.mp3");
		});
	EnemyRenderer->SetAnimationStartFunc("FAT_JUMP", 66, [this]
		{
			CreateWaterParticle();
			isJumpTime = false;
		});

	EnemyRenderer->CreateFBXAnimation("TILT_JUMP", "FROG_FAT_TILT_JUMP.fbx", { 1.0f / 30, false });					   //¹° to ¹Ù´Ú Àâ°í Á¡ÇÁ ²Ê´ç
	EnemyRenderer->SetAnimationStartFunc("TILT_JUMP", 1, [this]
		{
			GameEngineSound::Play("Frog_Phase2_JumpScream2.mp3");
		});
	EnemyRenderer->SetAnimationStartFunc("TILT_JUMP", 14, [this]
		{
			ResetStateDuration();
			isJumpTime = true;
		});
	EnemyRenderer->SetAnimationStartFunc("TILT_JUMP", 32, [this]
		{
			GameEngineSound::Play("Frog_Phase2_LandToGround.mp3");
		});
	EnemyRenderer->SetAnimationStartFunc("TILT_JUMP", 37, [this]
		{
			CreateShockEffect();
		});
	EnemyRenderer->SetAnimationStartFunc("TILT_JUMP", 40, [this]
		{
			isJumpTime = false;
		});
	EnemyRenderer->CreateFBXAnimation("SHOOT", "FROG_FAT_SHOOT.fbx", { 2.0f / 30, true});							   //¿ÞÂÊ¿¡¼­ 6¹ø ´øÁü
	EnemyRenderer->SetAnimationStartFunc("SHOOT", 0, [this]
		{
			++LoopCnt;
			GameEngineSound::Play("Frog_Phase2_PrevShootScream.mp3");
		});
	EnemyRenderer->SetAnimationStartFunc("SHOOT", 8 , [this]
		{
			std::shared_ptr< BossFrogBomb> Bomb = GetLevel()->CreateActor< BossFrogBomb>();
			float4 StartP = GetTransform()->GetWorldPosition();
			StartP.y = 100.0f;
			float4 Destindex = GetRandomTileIndex();
			Bomb->SetTargetTile(StartP, Destindex);
		});
	EnemyRenderer->CreateFBXAnimation("TURN", "FROG_FAT_TURN.fbx", { 1.0f / 30, false ,-1,-1,1.0f / 30,0.0f });
	EnemyRenderer->SetAnimationStartFunc("TURN", 56, [this] 
		{
			SetStateCheckerOn();
		});
	EnemyRenderer->CreateFBXAnimation("TILT", "FROG_FAT_TILT.fbx", { 1.0f / 30, false ,17,-1,0.0f});							   //¶¥ Àâ±â
	EnemyRenderer->SetAnimationStartFunc("TILT", 35, [this]
		{
			FieldRotationStart();
		});
	EnemyRenderer->CreateFBXAnimation("TILT_GRABBED", "FROG_FAT_TILT_GRABBED.fbx", { 1.0f / 30, false ,-1,-1,0.0f});			   //´©¸£±â
	EnemyRenderer->CreateFBXAnimation("GRABBED_IDLE", "FROG_FAT_TILT_GRABBED.fbx", { 1.0f / 30, false ,1,8,0.0f});			   //´©¸£±â
	
	EnemyRenderer->CreateFBXAnimation("SUCK", "FROG_FAT_SUCK.fbx", { 2.0f / 30, false });							   //¿À¸¥ÂÊ¿¡¼­ ¹ßÆÇ 5°³ ¸ÔÀ½

	EnemyRenderer->SetAnimationStartFunc("SUCK", 10, [this]
		{
			GameEngineSound::Play("Frog_Phase2_Suck_Scream.mp3");

		});
	EnemyRenderer->SetAnimationStartFunc("SUCK", 40, std::bind(&BossFrogFat::SuckTile, this));
	EnemyRenderer->SetAnimationStartFunc("SUCK", 50, std::bind(&BossFrogFat::SuckTile, this));
	EnemyRenderer->SetAnimationStartFunc("SUCK", 60, std::bind(&BossFrogFat::SuckTile, this));
	EnemyRenderer->SetAnimationStartFunc("SUCK", 65, std::bind(&BossFrogFat::SuckTile, this));
	EnemyRenderer->SetAnimationStartFunc("SUCK", 70, std::bind(&BossFrogFat::SuckTile, this));



	EnemyRenderer->CreateFBXAnimation("SUCK_BOMB", "FROG_FAT_SUCK_BOMB.fbx", { 1.0f / 30, false });					   //ÈíÀÔ Áß ÆøÅº ¸ÔÀ½

	EnemyRenderer->SetAnimationStartFunc("SUCK_BOMB", 3, [this]
		{
			GameEngineSound::Play("Frog_Phase2_EatBomb_Scream.mp3");
		});
	EnemyRenderer->SetAnimationStartFunc("SUCK_BOMB", 40, [this]
		{
			GameEngineSound::Play("Frog_Phase2_EatBomb_Boom.mp3");
		});
	EnemyRenderer->SetAnimationStartFunc("SUCK_BOMB", 61, [this]
		{
			GameEngineSound::Play("Frog_Phase2_EatBomb_Stun.mp3");
		});

	EnemyRenderer->CreateFBXAnimation("SUCK_BOMB_GETUP", "FROG_FAT_SUCK_BOMB_GETUP.fbx", { 1.0f / 30, false });		   //´«¾Ë ºùºù ÈÄ ÀÏ¾î³²
	EnemyRenderer->CreateFBXAnimation("SUCK_BOMB_LOOP", "FROG_FAT_SUCK_BOMB_LOOP.fbx", { 1.0f / 30, true });		   
	EnemyRenderer->CreateFBXAnimation("DIE", "FROG_FAT_DIE_LAND.fbx", { 1.0f / 30, false });		   //Death
	EnemyRenderer->CreateFBXAnimation("DIE_STAND", "FROG_FAT_DIE_STANDING.fbx", { 1.0f / 30, false });		   //

	JumpStartPoint = FatPointNorth;
	JumpEndPoint = OnGroundCenter;
	CalJumpPoint();

	//EnemyRenderer->Off();
	EnemyRenderer->ChangeAnimation("FAT_JUMP");
	EnemyRenderer->GetTransform()->SetLocalScale(float4::ONE * 130.0f);

	WeaponRenderer = CreateComponent<ContentFBXRenderer>();
	WeaponRenderer->SetFBXMesh("FROG_WEAPONMESH.FBX", "ContentMeshDeffered");
	WeaponRenderer->GetTransform()->SetParent(EnemyRenderer->GetTransform());
	WeaponRenderer->GetTransform()->SetLocalScale(float4::ONE * 1.4f);
	WeaponRenderer->SetGlowToUnit(1, 0);
	WeaponRenderer->SetUnitColor(1, 0, { 244.0f / 255.0f, 74.0f / 255.0f, 96.0f / 255.0f , 1.0f }, 5.0f);
	WeaponRenderer->FadeOut(0.01f, 0.01f);

}
void BossFrogFat::SuckTile()
{
	float4 index = GetRandomTileIndex();
	if (index == float4::ZERONULL)
	{
		return;
	}
	ShakeTile(index.iy(), index.ix(), 1.0f);

	GameEngineSound::Play("Frog_Phase2_SuckTileBreak.mp3");
}

void BossFrogFat::CalJumpPoint()
{
	JumpP3 = JumpEndPoint;
	JumpP3.y += 450;
	JumpP2 = JumpStartPoint * 0.1f + JumpEndPoint * 0.9f;
	JumpP2.y = JumpP3.y;
}


void BossFrogFat::SetFSMFUNC()
{
	InitFSM(BossFrogFatState::MAX);

	SetChangeFSMCallBack([this]
		{
		});

	SetFSM(BossFrogFatState::INTRO, // ÀÎÆ®·Î 1.5f
		[this]
		{
		},
		[this](float Delta)
		{
			EnemyRenderer->FadeIn(1.0f, Delta);
			WeaponRenderer->FadeIn(1.0f, Delta);

			const float4 m_CameraRot = { 55.0f, 0.0f , 0.0f };
			if(IntroDone == false && false == GetLevel()->GetMainCamera()->IsFreeCamera())
			{
				Player::MainPlayer->CameraControl = false;

				float4 nextPos = GetTransform()->GetWorldPosition();
				nextPos.y += 2000.0f; // Ä«¸Þ¶ó ³ôÀÌ
				float4 xzPos = float4::FORWARD * 2000.0f * tanf((90.0f - m_CameraRot.x) * GameEngineMath::DegToRad); //xz¿¬»ê
				xzPos.RotaitonYDeg(m_CameraRot.y);
				nextPos -= xzPos;
				GetLevel()->GetMainCamera()->GetTransform()->SetWorldPosition(float4::LerpClamp(GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition(), nextPos, Delta));
				GetLevel()->GetMainCamera()->GetTransform()->SetWorldRotation(float4::LerpClamp(GetLevel()->GetMainCamera()->GetTransform()->GetWorldRotation(), m_CameraRot, Delta));

			}

			if (true == isJumpTime)
			{
				m_pCapsuleComp->SetWorldPosWithParent(float4::Bezier4LerpClamp(JumpStartPoint, JumpP2, JumpP3, JumpEndPoint, GetStateDuration()));
			}
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				if (false == GetStateChecker())
				{
					EnemyRenderer->ChangeAnimation("JUMP_SCREAM");
					SetStateCheckerOn();
				}
				else
				{
					SetNextState(BossFrogFatState::IDLE);
				}
			}

		},
		[this]
		{
			IntroDone = true;
		}
	);

	SetFSM(BossFrogFatState::IDLE, // Â«Çª ÈÄ Àá±ñ idleÅ¸ÀÓ
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE");
		},
		[this](float Delta)
		{
			bool Hit = CheckHit();
			if (false == isTurned && Hit)
			{
				std::weak_ptr<ShockWave> Wave = CreateComponent<ShockWave>();
				Wave.lock()->GetTransform()->SetLocalPosition(float4{ 0.0f, 500.0f, 0.0f });
				Wave.lock()->GetTransform()->SetWorldRotation({ 90.0f, 0.0f, 0.0f });
				Wave.lock()->GetTransform()->SetLocalScale({ 10.0f, 10.0f, 1.0f });

				AllTileReset();
				GameEngineSound::Play("GimmickSound.mp3");
			}
			if (GetStateDuration() > 1.0f && false == isTurned)
			{
				SetNextState(BossFrogFatState::TURN);
			}
		},
		[this]
		{
		}
	);
	SetFSM(BossFrogFatState::GRABBED_IDLE, // Â«Çª ÈÄ Àá±ñ idleÅ¸ÀÓ
		[this]
		{
			EnemyRenderer->ChangeAnimation("GRABBED_IDLE", true, -1, 0.0f);
			m_pCapsuleComp->AttachShape();
		},
		[this](float Delta)
		{
			CheckHit();
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				if (0 < LoopCnt)
				{
					SetNextState(BossFrogFatState::JUMP_TO_GROUND);
					LoopCnt = 0;
				}
				else
				{
					if (true == isRightPattern)
					{
						SetNextState(BossFrogFatState::TILT);
					}
					else
					{
						SetNextState(BossFrogFatState::SHOOT);
					}

				}
			}
		},
		[this]
		{
			m_pCapsuleComp->DetachShape();
		}
	);

	SetFSM(BossFrogFatState::JUMP_TO_WATER,// ¶¥->¹°
		[this]
		{
			EnemyRenderer->ChangeAnimation("FAT_JUMP");
			JumpStartPoint = OnGroundCenter;
			if (isRightPattern)
			{
				JumpEndPoint = FatPointSouth;
			}
			else
			{
				JumpEndPoint = FatPointNorth;
			}
			CalJumpPoint();
		},
		[this](float Delta)
		{
			CheckHit();
			if (true == isJumpTime)
			{
				m_pCapsuleComp->SetWorldPosWithParent(float4::Bezier4LerpClamp(JumpStartPoint, JumpP2, JumpP3, JumpEndPoint, GetStateDuration()));
			}
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogFatState::IDLE);
			}
		},
		[this]
		{
			isTurned = false;
			m_pCapsuleComp->RigidSwitch(true);
		}
	);
	
	SetFSM(BossFrogFatState::JUMP_TO_GROUND,// ¹°->¶¥
		[this]
		{
			EnemyRenderer->ChangeAnimation("TILT_JUMP");
			JumpStartPoint = GetTransform()->GetWorldPosition();
			JumpEndPoint = GetPlayerPosition();
			JumpEndPoint.y = -180.0f;
			CalJumpPoint();
			m_pCapsuleComp->RigidSwitch(false);
		},
		[this](float Delta)
		{

			CheckHit();
			if (true == isJumpTime)
			{
				m_pCapsuleComp->SetWorldPosWithParent(float4::Bezier4LerpClamp(JumpStartPoint, JumpP2, JumpP3, JumpEndPoint, GetStateDuration()* 1.153846f));
			}
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogFatState::JUMP_TO_WATER);
			}
		},
		[this]
		{
			isRightPattern = !isRightPattern;
		}
	);

	SetFSM(BossFrogFatState::TURN,//¹° Â«Çª ÈÄ ÅÏ
		[this]
		{
			EnemyRenderer->ChangeAnimation("TURN");
			isTurned = true;
		},
		[this](float Delta)
		{
			CheckHit();
			if (false == GetStateChecker() && true == CheckHit())
			{
				std::weak_ptr<ShockWave> Wave = CreateComponent<ShockWave>();
				Wave.lock()->GetTransform()->SetLocalPosition(float4{ 0.0f, 500.0f, 0.0f });
				Wave.lock()->GetTransform()->SetWorldRotation({ 90.0f, 0.0f, 0.0f });
				Wave.lock()->GetTransform()->SetLocalScale({ 10.0f, 10.0f, 1.0f });

				AllTileReset();
				GameEngineSound::Play("GimmickSound.mp3");
			}
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogFatState::GRABBED_IDLE);
			}
		},
		[this]
		{
			m_pCapsuleComp->SetRotation(GetTransform()->GetLocalRotation() + float4{0.0f,180.0f,0.0f});

		}
	);


	SetFSM(BossFrogFatState::TILT, // tilt¿Í suck
		[this]
		{
			EnemyRenderer->ChangeAnimation("TILT", false, -1, 1.0f / 30);
			m_pCapsuleComp->AttachShape();
			GameEngineSound::Play("Frog_Phase2_PrevSuckScream.mp3");
		},
		[this](float Delta)
		{
			CheckHit();
			if (GetStateDuration() > 1.0f)
			{
				SetNextState(BossFrogFatState::SUCK);
			}
		},
		[this]
		{
			m_pCapsuleComp->DetachShape();

		}
	);
	SetFSM(BossFrogFatState::SUCK, // tilt¿Í suck 3ÃÊ¿¡ ¼¼¿ì°í 4ÃÊ¿¡
		[this]
		{
			LoopCnt = 0;
			EnemyRenderer->ChangeAnimation("SUCK");
			m_pCapsuleComp->AttachShape();
		},
		[this](float Delta)
		{
			if (true == CheckCollision(PhysXFilterGroup::PlayerBomb)) // ÀÓ½Ã(ÆøÅº¿¡ ¸Â¾Ò´Ù¸é)
			{
				FieldRotationEnd();
				if (false == GetStateChecker())
				{
					m_pCapsuleComp->DetachShape();
				}
				SetNextState(BossFrogFatState::SUCK_BOMB);
				return;
			}

			

			CheckHit();
			if (false == GetStateChecker())
			{
				if(GetStateDuration() > 5.5f)
				{
					FieldRotationEnd();
					m_pCapsuleComp->DetachShape();
					SetStateCheckerOn();
				}
				else if(GetStateDuration() > 1.0f)
				{
					Player::MainPlayer->GetPhysXComponent()->SetMoveSpeed(-GetPlayerDir()* 500.0f);
					
					SuckParticleCount += Delta;
					if (SuckParticleCount >= 0.1f)
					{
						SuckParticleCount = 0.0f;
						CreateSuckParticle();
					}
				}
			}
			else if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogFatState::JUMP_TO_GROUND);
				return;
			}
		},
		[this]
		{
		}
	);

	SetFSM(BossFrogFatState::SHOOT, // ÆøÅº ´øÁö±â
		[this]
		{
			LoopCnt = 0;
			EnemyRenderer->ChangeAnimation("SHOOT", false, -1, 3.0f / 30);
			m_pCapsuleComp->AttachShape();

		},
		[this](float Delta)
		{
			CheckHit();
			if (LoopCnt >=6)
			{
				SetNextState(BossFrogFatState::GRABBED_IDLE);
			}
		},
		[this]
		{
			m_pCapsuleComp->DetachShape();
		}
	);

	SetFSM(BossFrogFatState::SUCK_BOMB, // ÆÐÅÏ ÆÄÈÑ ½Ã
		[this]
		{
			EnemyRenderer->ChangeAnimation("SUCK_BOMB");
			
		},
		[this](float Delta)
		{
			CheckHit(); 
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogFatState::SUCK_BOMB_END);
				return;
			}
		},
		[this]
		{
		}
	);
	SetFSM(BossFrogFatState::SUCK_BOMB_END, // ÆÐÅÏ ÆÄÈÑ ½Ã
		[this]
		{
			EnemyRenderer->ChangeAnimation("SUCK_BOMB_LOOP");
		},
		[this](float Delta)
		{
			CheckHit();
			if (false == GetStateChecker() && GetStateDuration() > 2.0f)
			{
				EnemyRenderer->ChangeAnimation("SUCK_BOMB_GETUP");
				SetStateCheckerOn();
			}
			else if (true == GetStateChecker() && true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogFatState::JUMP_TO_GROUND);
			}
		},
		[this]
		{
		}
	);

	SetFSM(BossFrogFatState::DEATH,
		[this]
		{
			float4 CollPoint = float4::ZERO; // Ãæµ¹Ã¼Å©ÇÒ º¯¼ö
			float4 FrogPos = GetTransform()->GetWorldPosition();
			FrogPos.y += 50.0f;

			GameEngineSound::Play("Frog_Phase2_Death.mp3");

			//std::shared_ptr<GameEngineActor> Soul= GetLevel()->CreateActor<BossFrogSoul>();
			if (true == m_pCapsuleComp->RayCast(FrogPos, float4::DOWN, CollPoint, 2000.0f)) // ÇÃ·¹ÀÌ¾î À§Ä¡¿¡¼­ float4::DOWN ¹æÇâÀ¸·Î 2000.0f ±æÀÌ¸¸Å­ Ã¼Å©ÇÑ´Ù.
			{
				EnemyRenderer->ChangeAnimation("DIE");
				//Soul->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + float4{ 0.0f , 800.0f, 0.0f });
			}
			else
			{
				EnemyRenderer->ChangeAnimation("DIE_STAND");
				//Soul->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + float4{ 0.0f , 1000.0f, 0.0f });
			}
		},
		[this](float Delta)
		{
			LerpRatio += 0.25f * Delta;

			if(LerpRatio < 1.0f)
			{
				float4 LerpColor = float4::Lerp(StartColor, EndColor, LerpRatio);
				EnemyRenderer->SetBlurColor(LerpColor, 3.0f - 3.0f * LerpRatio);
			}
			else
			{
				LerpRatio = 1.0f;
				EnemyRenderer->SetBlurColor(EndColor, -1.0f);
			}
			if (GetStateDuration() > 3.0f)
			{
				IntroDone = false;
			}
			if (GetStateDuration() > 6.0f)
			{
				EnemyRenderer->FadeOut(1.f, Delta);
			}
			if (GetStateDuration() > 7.0f)
			{
				Death();
			}
		},
		[this]
		{
		}
	);

}

void BossFrogFat::CreateShockEffect()
{
	std::shared_ptr<GameEngineRenderer> ShockCircle = CreateComponent<GameEngineRenderer>();
	
	ShockCircle->SetMesh("Rect");
	ShockCircle->SetMaterial("ShockCircle", RenderPath::Alpha);
	ShockCircle->GetUnit()->ShaderResHelper.SetTexture("Diffusetex", "ShockCircle.png");
	ShockCircle->GetTransform()->SetWorldScale({ 1000.0f, 1000.0f });
	ShockCircle->GetTransform()->SetWorldPosition(EnemyRenderer->GetTransform()->GetWorldPosition() + float4{0.0f, -120.0f, 0.0f});
	ShockCircle->GetTransform()->SetWorldRotation({90.0f, 0.0f});
	ShockCircle->GetUnit()->Color.MulColor = { 1.0f, 1.0f, 1.0f, 0.5f };

	std::weak_ptr<GameEngineRenderer> Weak = ShockCircle;

	GetLevel()->TimeEvent.AddEvent(0.1f, [this, Weak](GameEngineTimeEvent::TimeEvent&, GameEngineTimeEvent*)
		{
			Weak.lock()->Death();
		}
	);

	float Angle = 0.0f;
	
	for (int i = 0; i < 18; i++)
	{
		std::shared_ptr<DustParticle> Dust = GetLevel()->DynamicThis<ContentLevel>()->GetPivotActor()->CreateComponent<DustParticle>();
		Dust->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + float4{ 0.0f, -110.0f, 0.0f });
		Dust->GetTransform()->SetWorldScale({ 160.0f, 160.0f });
		Dust->SetFadeInAndOut();

		float4 Dir = { cos(Angle), 0.0f, sin(Angle) };
		Dir.Normalize();

		Dust->SetMoveInfo(Dir, 200.0f);
		Dust->SetWorldMove();

		float Distance = GameEngineRandom::MainRandom.RandomFloat(160, 200);

		Dust->GetTransform()->AddWorldPosition({ Distance * cos(Angle), 1.0f + i , Distance * sin(Angle) });
		Dust->GetTransform()->SetWorldRotation({ 90.0f, 0.0f , 0.0f });

		Angle += 20.0f;
	}

	Angle = 0.0f;

	for(int i = 0; i < 12; i++)
	{
		std::shared_ptr<Particle3D> New = CreateComponent<Particle3D>();

		float YScale = GameEngineRandom::MainRandom.RandomFloat(15.0f, 30.0f);

		New->GetTransform()->SetLocalScale({ 1.0f, YScale, 1.0f });
		New->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());

		float X = GameEngineRandom::MainRandom.RandomFloat(45.0f, 90.0f);
		float Y = Angle;

		float4x4 Mat1 = DirectX::XMMatrixRotationX(X * GameEngineMath::DegToRad);
		float4x4 Mat2 = DirectX::XMMatrixRotationY(Y * GameEngineMath::DegToRad);
		Mat1 *= Mat2;

		float4 Quat = float4::ZERO;
		Quat = Quat.MatrixToQuaternion(Mat1);
		float4 Euler = Quat.QuaternionToEulerDeg();

		New->GetTransform()->SetWorldRotation(Euler);
		
		float4 AngleVector = { X, Y };
		AngleVector = AngleVector.EulerDegToQuaternion();
		AngleVector = DirectX::XMVector3Rotate({ 0.0f, 1.0f, 0.0f }, AngleVector);
		AngleVector.Normalize();

		New->SetAutoMove(AngleVector, 2500.0f);
		New->SetScaleDecrease({ 1.0f, YScale, 1.0f }, 30.0f);

		Angle += 30.0f;
	}
}

void BossFrogFat::CreateSuckParticle()
{
	std::shared_ptr<Particle3D> New = CreateComponent<Particle3D>();

	float YScale = GameEngineRandom::MainRandom.RandomFloat(30.0f, 45.0f);

	New->GetTransform()->SetLocalScale({ 1.0f, YScale, 1.0f });

	float X = GameEngineRandom::MainRandom.RandomFloat(75.0f, 90.0f);
	float Y = GameEngineRandom::MainRandom.RandomFloat(270.0f, 360.0f);

	float4x4 Mat1 = DirectX::XMMatrixRotationX(X * GameEngineMath::DegToRad);
	float4x4 Mat2 = DirectX::XMMatrixRotationY(Y * GameEngineMath::DegToRad);

	Mat1 *= Mat2;

	float4 Quat = float4::ZERO;
	Quat = Quat.MatrixToQuaternion(Mat1);
	Quat = Quat.QuaternionToEulerDeg();

	New->GetTransform()->SetWorldRotation(Quat);

	float4 AngleVector = { X, Y };
	AngleVector = AngleVector.EulerDegToQuaternion();
	AngleVector = DirectX::XMVector3Rotate({ 0.0f, 1.0f, 0.0f }, AngleVector);
	AngleVector.Normalize();

	New->SetAutoMoveLerp(AngleVector * 1000.0f + GetTransform()->GetWorldPosition() + float4{ -600.0f, 900.0f, 200.0f }, GetTransform()->GetWorldPosition() + float4{ -600.0f, 900.0f, 200.0f }, 2.0f);
	New->SetScaleDecreaseLerp({ 1.0f, YScale, 1.0f }, {1.0f, 0.0f, 1.0f}, 2.0f);
}

void BossFrogFat::CreateWaterParticle()
{
	float Angle = 0.0f;

	for (int i = 0; i < 18; i++)
	{
		std::shared_ptr<WaterDrop> Drop = CreateComponent<WaterDrop>();
		float Scale = GameEngineRandom::MainRandom.RandomFloat(45, 75);
		Drop->GetTransform()->SetWorldScale({ Scale, Scale, Scale });

		Drop->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + float4{ 0.0f, 150.0f, 0.0f });
		float DirX = GameEngineRandom::MainRandom.RandomFloat(-0.5, 0.5);
		float DirY = GameEngineRandom::MainRandom.RandomFloat(0.8f, 1.0f);
		float DirZ = GameEngineRandom::MainRandom.RandomFloat(-0.5, 0.5);
		Drop->SetParabola({ DirX, DirY, DirZ }, 500.0f, 1000.0f);

		float4 Dir = { cos(Angle), 0.0f, sin(Angle) };
		Dir.Normalize();

		float Distance = GameEngineRandom::MainRandom.RandomFloat(300, 400);

		Drop->GetTransform()->AddWorldPosition({ Distance * cos(Angle), 1.0f + i , Distance * sin(Angle) });

		Angle += 20;
	}
}

