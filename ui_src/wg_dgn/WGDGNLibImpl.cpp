#include "StdAfx.h"

#include "WGDGNLibImpl.h"

#include "DgnCodeCtrl.h"

#include "..\wg_db\DBCodeDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CWGDGNLibImpl _g_Instance;

CWGDGNLibImpl::CWGDGNLibImpl()
{
    m_pInstance = this;
}

bool CWGDGNLibImpl::IsConCodeUseMeshedPM4DgnEngine(const CString& strCode) const
{
    return CDgnCodeCtrl::IsConCodeUseMeshedPM4DgnEngine(strCode);
}
