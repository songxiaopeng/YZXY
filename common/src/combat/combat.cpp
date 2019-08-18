#include "combat.h"
#include "message.h"
#include "skill.h"
#ifndef SIMPLE_SERVER
#include "input_manager.h"
#include "animation/frame_animation.h"
#include "actor/player_state.h"
#endif
#include "actor/action.h"

TurnCommand::TurnCommand()
{

}


TurnCommand::~TurnCommand()
{

}

CombatSystem::CombatSystem()
	:m_Actors(0),
	m_TurnCounter(0),
	m_BattleState(BATTLE_DEFAULT),
	m_CombatBGPath("")
{
	//SKILL_MANAGER_INSTANCE;
}

CombatSystem::~CombatSystem()
{

}


void CombatSystem::Update()
{
#ifndef SIMPLE_SERVER
	if (m_BattleState == BATTLE_DEFAULT) {

	}
	else if (m_BattleState == BATTLE_TURN_STAND_BY) {
		bool allReady = true;
		for (auto& it : m_Actors) {
			if (!it->IsTurnReady()) {
				allReady = false;
				break;
			}
		}
		if (allReady) {
			m_BattleState = BTTALE_TURN_EXECUTE;
		}
	}
	else if (m_BattleState == BTTALE_TURN_EXECUTE) {
		if (m_Commands.size() > 0) {
			TurnCommand& cmd = m_Commands.front();
			if (cmd.Finished()) {
				m_Commands.pop_front();
			}
			else {
				cmd.Update();
			}
		}
		if (m_Commands.size() == 0) {
			m_BattleState = BTTALE_TURN_NEXT;
		}
	}
	else if (m_BattleState == BTTALE_TURN_NEXT) {
		m_TurnCounter++;
		m_BattleState = BATTLE_TURN_STAND_BY;
		for (auto& it : m_Actors) {
			it->SetTurnReady(false);
		}
	}
	else if (m_BattleState == BATTLE_END) {
		m_TurnCounter = 0;
		for (auto& it : m_Actors) {
			it->SetIsCombat(false);
		}
	}
	
	if (m_BattleState != BATTLE_DEFAULT && m_BattleState != BATTLE_DEFAULT) {
		for (auto& it : m_Actors) {
			it->OnUpdate();
		}
	}

	//INPUT_MANAGER_INSTANCE->RegisterOnKeyClickEvent(GLFW_KEY_1,
	//	[this]() {
	//	int selfID = RANDOM_INSTANCE->NextInt(0, (int)m_Ourselves.size() - 1);
	//	int enemyID = RANDOM_INSTANCE->NextInt(0, (int)m_Enemies.size() - 1);
	//	auto& self = m_Ourselves[selfID];
	//	auto& enemy = m_Enemies[enemyID];
	//	self->SetCombatTargetPos({ enemy->GetCombatPos().x + 88 , enemy->GetCombatPos().y + 73 });
	//	self->SetTargetID(enemyID + 10);
	//	self->GetFSM()->ChangeState(PlayerCombatMoveState::GetInstance());
	//}
	//);

	//INPUT_MANAGER_INSTANCE->RegisterOnKeyClickEvent(GLFW_KEY_2,
	//	[this]() {
	//	int selfID = RANDOM_INSTANCE->NextInt(0, (int)m_Enemies.size() - 1);
	//	auto& self = m_Ourselves[selfID];
	//	self->GetFSM()->ChangeState(PlayerCombatCastAttackState::GetInstance());
	//}
	//);
#endif
}

void CombatSystem::Draw()
{
#ifndef SIMPLE_SERVER
	if (m_BattleState != BATTLE_DEFAULT && m_BattleState != BATTLE_DEFAULT) {
		for (auto& it : m_Actors) {
			it->OnDraw(it->GetX(),it->GetY());
		}
	}
#endif
}


void CombatSystem::AddActor(Actor* actor)
{
	m_Actors.push_back(actor);
}


void CombatSystem::NextTurn()
{
	m_TurnCounter++;
}

void CombatSystem::StartBattle()
{
	m_BattleState = BATTLE_START;
	m_TurnCounter = 0;
	for (auto& it : m_Actors) {
		Pos  pos = it->GetPos();
		it->SetIsCombat(true);
		it->SetPos(pos);
		it->SetActionID(Action::Batidle);
	}
	m_BattleState = BTTALE_TURN_NEXT;
}

void CombatSystem::EndBattle()
{
	m_BattleState = BATTLE_END;
}

int combat_system_add_actor(lua_State* L)
{
	Actor* actor = lua_check_actor(L, 1);
	COMBAT_SYSTEM_INSTANCE->AddActor(actor);
	return 0;
}

void combat_system_start_battle()
{
	COMBAT_SYSTEM_INSTANCE->StartBattle();
}

void combat_system_end_battle()
{
	COMBAT_SYSTEM_INSTANCE->EndBattle();
}

void combat_system_update()
{
	COMBAT_SYSTEM_INSTANCE->Update();
}

void combat_system_draw()
{
	COMBAT_SYSTEM_INSTANCE->Draw();
}


void luaopen_combat_system(lua_State* L)
{
	script_system_register_luac_function(L, combat_system_add_actor);
	script_system_register_function(L, combat_system_start_battle);
	script_system_register_function(L, combat_system_end_battle);
	script_system_register_function(L, combat_system_update);
	script_system_register_function(L, combat_system_draw);
}
