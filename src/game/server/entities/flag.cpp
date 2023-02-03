#include <game/server/gamecontext.h>
#include <game/server/gamecontroller.h>
#include <game/server/player.h>
#include <game/mapitems.h>

#include <base/do.h>

#include "character.h"
#include "flag.h"

CFlag::CFlag(CGameWorld *pGameWorld, vec2 Pos, int Team) :
	CEntity(pGameWorld, CGameWorld::ENTTYPE_FLAG)
{
	m_Team = Team;
	m_StandPos = Pos;
	GameWorld()->InsertEntity(this);

	Reset();
}

void CFlag::Reset()
{
	m_pCarryingChar = NULL;
	m_AtStand = 1;
	m_Pos = m_StandPos;
	m_Vel = vec2(0,0);
	m_DropTick = 0;
	m_Scorer = -1;
}

void CFlag::Tick()
{
	CGameContext *gsv;
	IGameController *clr;
	int i, t;

	gsv = GameWorld()->GameServer();

	if (m_pCarryingChar || ((!m_DropTick || m_DropTick + Server()->TickSpeed() * 0.25
	    < Server()->Tick()) && FindCarrier())) {
		m_Pos = m_pCarryingChar->m_Pos;
		m_Vel = m_pCarryingChar->Core()->m_Vel;
	} else if (!m_AtStand) {
		m_Vel.x *= 0.98;
		m_Vel.y += GameWorld()->m_Core.m_aTuning[0].m_Gravity;

		gsv->Collision()->MoveBox(&m_Pos, &m_Vel, vec2(1.f, 1.f) * ms_PhysSize, 0.5f);

		if ((t = gsv->Collision()->GetCollisionAt(m_Pos.x, m_Pos.y)) == TILE_DEATH ||
//		    (t >= TILE_SPIKE_FIRST && t <= TILE_SPIKE_LAST) ||
		    GameLayerClipped(m_Pos)) {
			gsv->Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "game", "flag_return");
	//		gsv->CreateSoundGlobal(SOUND_CTF_RETURN);
			Reset();
		}
	} else
		return;

	clr = GameWorld()->GameServer()->m_pController;
	for (i = 0; i < clr->ngoals[m_Team]; i++)
		if (distance(clr->goalspos[m_Team][i], m_Pos) < 64.f)
			goto thatsagoal;
	/* no goal */
	return;
thatsagoal:
	if (!clr->FlagGoal(this))
		return;
	Drop();
	Reset();
}

void CFlag::TickPaused()
{
	m_DropTick++;
}

#if 0
intern float
disttobox(FPARS(vec2, p, bp, bsz))
{
	vec2 cp; /* closest to p point of the box */

	#define M(I, C)\
		if (p.C < bp.C)\
			cp.C = bp.C;\
		else if (p.C <= bp.C + bsz.C)\
			cp.C = p.C;\
		else\
			cp.C = bp.C + bsz.C;
	do2(M);
	#undef M
	return distance(p, cp);
}
#endif

/* check whether the line segment la->lb collides with the box (bp, bsz) */
intern int
lnsegcolwbox(FPARS(vec2, la, lb, bp, bsz))
{
	vec2 na, nb, bh;
	ivec2 ac, bc;

	bh = bp + bsz;
	#define CN(N, L, H) ((N) < (L) ? -1 : (N) > (H))
	#define M(I, C, V, P) (V).C = CN((P).C, bp.C, bh.C);
onemore:
	do2v(M, bc, lb);
	do2v(M, ac, la);
	#undef M
	#undef CN

	#define E(I, C) ac.C == bc.C && ac.C
	if (do2e(E, ||))
	#undef E
		return 0;

	#define E1(I, C) ac.C == bc.C
	#define E2(I, C) !ac.C
	if (do2e(E1, ||) || do2e(E2, &&))
	#undef E1
	#undef E2
		return 1;

	na.x = ac.x < 0 ? bp.x : bh.x;
	na.y = la.y + (na.x - la.x) / (lb.x - la.x) * (lb.y - la.y);

	nb.x = bc.x < 0 ? bp.x : bh.x;
	nb.y = la.y + (nb.x - la.x) / (lb.x - la.x) * (lb.y - la.y);

	la = na;
	lb = nb;
	goto onemore;
}

intern void
getflbox(FPARS(vec2, *bp, *bsz), float phsz)
{
	vec2 ul, dr;

	ul = -vec2(1.f, 5.f) * phsz;
	dr = vec2(1.f, 3.f) * phsz;

	*bp += ul;
	*bsz = dr - ul;
}

int CFlag::FindCarrier()
{
	CCharacter *ch, *cch;
	vec2 bp, bsz;
	float d, md;
	int h;

	md = ms_PhysSize + CCharacterCore::PhysicalSize();
	cch = 0;
	h = 0;
	ch = (CCharacter *)GameWorld()->FindFirst(CGameWorld::ENTTYPE_CHARACTER);
	for (; ch; ch = (CCharacter *)ch->TypeNext()) {
		if (!ch->IsAlive() || ch->m_pCarryingFlag || ch->Team())
			continue;
		if ((d = distance(ch->m_Pos, m_Pos)) < md) {
			h = 0;
			goto closerone;
		}
		if (ch->Core()->m_HookState != HOOK_FLYING)
			continue;

		bp = m_Pos;
		getflbox(&bp, &bsz, ms_PhysSize);
		if (lnsegcolwbox(ch->m_Pos, ch->Core()->m_HookPos, bp, bsz)) {
			d = 0.f, h = 1;
			goto closerone;
		}
		continue;
	closerone:
		md = d;
		cch = ch;
	}
	if (!cch)
		return 0;
	cch->GiveFlag(this);
	m_AtStand = 0;
	if (h)
		cch->Core()->m_HookState = HOOK_RETRACT_START;
//	GameWorld()->GameServer()->CreateSoundGlobal(SOUND_CTF_GRAB_EN);
	return 1;
}

void CFlag::Drop()
{
	if (m_pCarryingChar) {
		m_pCarryingChar->DropFlag();
		m_pCarryingChar = 0;
	}
	m_DropTick = Server()->Tick();
}

void CFlag::SetCarrierState(CNetObj_GameData *gdata)
{
	int st;

	st = FLAG_ATSTAND;
	if (m_Pos == m_StandPos)
		st = FLAG_ATSTAND;
	else if (!m_pCarryingChar)
		st = FLAG_TAKEN;
	else
		st = m_pCarryingChar->GetPlayer()->GetCID();

	if (m_Team == TEAM_RED)
		gdata->m_FlagCarrierRed = st;
	else
		gdata->m_FlagCarrierBlue = st;
}

void CFlag::Snap(int SnappingClient)
{
	CNetObj_Flag *nfl;

	if (NetworkClipped(SnappingClient))
		return;

	nfl = (CNetObj_Flag *)Server()->SnapNewItem(NETOBJTYPE_FLAG, m_Team, sizeof *nfl);
	if (!nfl)
		return;

	nfl->m_X = (int)m_Pos.x;
	nfl->m_Y = (int)m_Pos.y;
	nfl->m_Team = m_Team;
}
