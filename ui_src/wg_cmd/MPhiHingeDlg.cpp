// MPhiHingeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "MPhiHingeDlg.h"

#include "MPhiHingeChildDlg.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPhiHingeDlg dialog


CMPhiHingeDlg::CMPhiHingeDlg(MPhiDataAll* pMPhiDataAll, CWnd* pParent /*=NULL*/)
	: CDlgChild(CMPhiHingeDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pMPhiDataAll = pMPhiDataAll;
	//{{AFX_DATA_INIT(CMPhiHingeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nModifyKey = 0;
	m_bModify = FALSE;
}


void CMPhiHingeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMPhiHingeDlg)
	DDX_Control(pDX, IDC_CMD_TAB_CTRL, m_cTabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMPhiHingeDlg, CDlgChild)
	//{{AFX_MSG_MAP(CMPhiHingeDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB_CTRL, OnSelchangeCmdTabCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPhiHingeDlg message handlers
BOOL CMPhiHingeDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDlgChild::OnInitDialog();

	// set tab dialogs....
	SetTabDlg();

	// Dialog Page Manager¿¡ Place Holder ¼³Á¤
	CWnd* pPH = (CWnd*)GetDlgItem(IDC_CMD_PLACEHOLDER);
	ASSERT(pPH && pPH->GetSafeHwnd() && IsWindow(pPH->GetSafeHwnd()));
	m_cTabCtrl.SetPlaceHolder(pPH);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMPhiHingeDlg::SetModifyData(UINT Key)
{
	m_nModifyKey = (T_MATL_K)Key;
	m_bModify = TRUE;
}

void CMPhiHingeDlg::SetTabDlg()
{
	UINT dlgID;

	m_RcPscDlg = new CMPhiHingeChildDlg(m_pMPhiDataAll, this, 0);
	m_SteelDlg	= new CMPhiHingeChildDlg(m_pMPhiDataAll, this, 1);
	m_SRCDlg		= new CMPhiHingeChildDlg(m_pMPhiDataAll, this, 2);
	dlgID = CMPhiHingeChildDlg::IDD;	
	
	m_cTabCtrl.AddTab(m_RcPscDlg, _LS(IDS_CMD_MPHI_RC_PSC), dlgID, TRUE);
	m_cTabCtrl.AddTab(m_SteelDlg, _LS(IDS_CMD_MPHI_STEEL), dlgID, TRUE);
	m_cTabCtrl.AddTab(m_SRCDlg,		_LS(IDS_CMD_ESQW_Material_SRC), dlgID, TRUE);

	if(m_bModify==TRUE)
	{
		T_MATD_D Data;
		m_pDoc->m_pAttrCtrl->GetMatlDesign(m_nModifyKey, Data);
		if(Data.Type == _T("C"))
		{
			m_RcPscDlg->SetModifyData(m_nModifyKey);
			m_cTabCtrl.ShowTab(0);
		}
		else if(Data.Type == _T("S"))
		{
			m_SteelDlg->SetModifyData(m_nModifyKey);
			m_cTabCtrl.ShowTab(1);
		}
		else if(Data.Type == _T("SRC"))
		{
			m_SRCDlg->SetModifyData(m_nModifyKey);
			m_cTabCtrl.ShowTab(2);
		}
		else
		{
			ASSERT(0);
			return;
		}
	}
	else
		m_cTabCtrl.ShowTab(0);
}

void CMPhiHingeDlg::OnSelchangeCmdTabCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	Invalidate();
	UpdateWindow();
	*pResult = 0;
}

//void CMPhiHingeDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
void CMPhiHingeDlg::MyUpdate(LPARAM lHint, CObject* pHint)
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer(lHint, pHint);
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer(lHint, pHint);
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START:
		//DestroyWindow(); return;
	case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}

}


void CMPhiHingeDlg::UpdateBuffer(LPARAM lHint, CObject* pHint)
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MATL_K Key;
	T_MATD_D Data;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MATD_ADD):
		case(UR_MATD_DEL):
		case(UR_MATD_MFD):
		case(UR_MATD_MFS):
				pViewBuff->GetMatd(nKey, Key, Data);
				if(Data.Type==_T("C"))
					m_RcPscDlg->UpdateList(lHint, pHint);
				else if(Data.Type==_T("S"))
					m_SteelDlg->UpdateList(lHint, pHint);
				else if(Data.Type==_T("SRC"))
					m_SRCDlg->UpdateList(lHint, pHint);
				break;
		default:
			break;
		}
	} // end of while
}


void CMPhiHingeDlg::OnOK()
{
	return;
}

void CMPhiHingeDlg::OnCancel() 
{
	return;
}
