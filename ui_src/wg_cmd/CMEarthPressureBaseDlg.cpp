#include "stdafx.h"
#include "wg_cmd.h"

#include "CMEarthPressureBaseDlg.h"
#include "CMEarthPressureBaseMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMEarthPressureBaseDlg dialog

CCMEarthPressureBaseDlg::CCMEarthPressureBaseDlg(UINT nID)
	: CCMChildBarBase(nID)
{
	m_aWlCodeEnable.RemoveAll();
	m_nOldSel = 0;
}

CCMEarthPressureBaseDlg::~CCMEarthPressureBaseDlg()
{
}

void CCMEarthPressureBaseDlg::End()
{
	CCMChildBarBase::End();
	if (m_pMgr)
	{
		m_pMgr->InitAfterEnd();
	}
}