#include "stdafx.h"
#include "wg_cmd.h"

#include "CMEarthPressureBaseMgr.h"
#include "CMEarthPressureBaseDlg.h"
#include "CMEarthPressureDlgMover.h"
#include "CMEarthPressureBaseChildDlg.h"

// *^^* 코드 추가시 여기수정 <- LoadDgnCodeCheck 파일에서 Lock Check Option 수정/추가 할 것!!
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCMEarthPressureBaseMgr::CCMEarthPressureBaseMgr(CWnd* pParent)
{
	m_bCobxMsgChecker = FALSE;
	m_pParent = NULL;	
	ASSERT(pParent);
	m_pParent = (CCMEarthPressureBaseDlg*)pParent;

	m_nCurCodeIndex = -1;
	m_nInitCodeIndex = 0;

	m_pDlgMover = NULL;
	m_pDlgMover = new CCMEarthPressureDlgMover((CWnd*)m_pParent, m_pParent->GetBaseDlg());

	CArray<UINT, UINT> caCtrls;
	UINT uiHolderID;
	m_pParent->GetMoveCtrlHolderID(caCtrls, uiHolderID);
	m_pDlgMover->SetSubCtrls(caCtrls, uiHolderID);
}

CCMEarthPressureBaseMgr::~CCMEarthPressureBaseMgr()
{
	DeleteCodeData();

	if (m_pDlgMover)
		delete m_pDlgMover;
	m_pDlgMover = NULL;
}

void CCMEarthPressureBaseMgr::DeleteCodeData()
{
	int nCount = m_aCodeDlg.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		delete m_aCodeDlg[i];
	}

	m_aCodeName.RemoveAll();
	m_aCodeDlg.RemoveAll();
	m_aCodeType.RemoveAll();
}

void CCMEarthPressureBaseMgr::InitAfterEnd()
{
	m_nCurCodeIndex = -1;
	m_nInitCodeIndex = 0;

	int i;
	int nSize = m_aCodeDlg.GetSize();
	for (i = 0; i < nSize; i++)
	{
		if (m_aCodeDlg[i])
		{
			m_aCodeDlg[i]->End();
		}
	}
}
