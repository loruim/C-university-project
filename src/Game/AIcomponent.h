#pragma once

class CloseCombat;

class AIcomponent
{
public:
	AIcomponent(CloseCombat* pParrentCloseCombatUnit);

	void update(const double delta);

private:
	CloseCombat* m_pParrentCloseCombatUnit;
};