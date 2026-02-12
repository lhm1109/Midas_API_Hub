#include "stdafx.h"
#include "wg_treemenu.h"
#include "DesignStripAutoGenDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDesignStripAutoGenDlg::CDesignStripAutoGenDlg(CWnd* pParent)
    : CMenuBarChildDlg(CDesignStripAutoGenDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	
	nMethod = 0;
}

CDesignStripAutoGenDlg::~CDesignStripAutoGenDlg()
{
}

void CDesignStripAutoGenDlg::DoDataExchange(CDataExchange* pDX)
{
    CMenuBarChildDlg::DoDataExchange(pDX);
     DDX_Radio(pDX, IDC_TM_DGNSTRIP_METHOD1_RDO, nMethod);
}

BEGIN_MESSAGE_MAP(CDesignStripAutoGenDlg, CMenuBarChildDlg)
	// Add message map entries here
	ON_BN_CLICKED(IDC_TM_DGNSTRIP_METHOD1_RDO, OnChangeMethod)
END_MESSAGE_MAP()

BOOL CDesignStripAutoGenDlg::OnInitDialog()
{
    CMenuBarChildDlg::OnInitDialog();
    
    return TRUE;
}

void CDesignStripAutoGenDlg::OnChangeMethod()
{

}

void CDesignStripAutoGenDlg::Execute()
{
}

void CDesignStripAutoGenDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
		break;
	default: ASSERT(TRUE);
	}
}

void CDesignStripAutoGenDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;

		switch (nCmd)
		{
		case(UR_EVGP_ADD):
		case(UR_EVGP_DEL):
		case(UR_EVGP_MFD):
		case(UR_EVGP_MFS):
			break;
		case(UR_SECT_ADD):
		case(UR_SECT_DEL):
		case(UR_SECT_MFD):
		case(UR_SECT_MFS):
		case(UR_ELEM_ADD):
		case(UR_ELEM_DEL):
		case(UR_ELEM_MFD):
		case(UR_ELEM_MFS):
			break;
		default:
			break;
		}
	}
}