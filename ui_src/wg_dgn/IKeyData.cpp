#include "StdAfx.h"

#include "IKeyData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::converter;

IKeyData::IKeyData()
{
}

void IKeyData::Init()
{
    return DoInit();
}