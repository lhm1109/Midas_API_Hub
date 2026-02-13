#pragma once

#include "../../dgnengine/idesign/DGN_link/IDgnProductLib.h"

class CDgnProductLibImpl : public dgn::link::IDgnProductLib
{
public:
	CDgnProductLibImpl(void);
	virtual ~CDgnProductLibImpl(void);

public:
	virtual bool IsUseLibXL();
};
