#ifndef GAME_SERVER_ENTITIES_FLAG_H
#define GAME_SERVER_ENTITIES_FLAG_H

#include <game/server/entity.h>

class CFlag : public CEntity
{
	vec2 *m_Goals;
public:
	static const int ms_PhysSize = 14;
	CCharacter *m_pCarryingChar;
	vec2 m_Vel;
	vec2 m_StandPos;

	int m_Team;
	int m_AtStand;
	int m_DropTick;
	int m_Scorer;

	CFlag(CGameWorld *pGameWorld, vec2 Pos, int Team);

	virtual void Reset() override;
	virtual void Tick() override;
	virtual void TickPaused() override;
	virtual void Snap(int SnappingClient) override;

	int FindCarrier();
	void SetCarrierState(CNetObj_GameData *gdata);
	void Drop();
};

#endif
