#include "stdafx.h"

#include "DgnProductLibImpl.h"

CDgnProductLibImpl g_singleton;

CDgnProductLibImpl::CDgnProductLibImpl(void)
{
}


CDgnProductLibImpl::~CDgnProductLibImpl(void)
{
}

bool CDgnProductLibImpl::IsUseLibXL()
{
	return true;
}

