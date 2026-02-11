// CMRefWD_KDS2016.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMRefWD_KDS2016.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_main\wg_mainres2.h"
using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMDRefwdKDS2016Dlg dialog


CCMDRefwdKDS2016Dlg::CCMDRefwdKDS2016Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMDRefwdKDS2016Dlg::IDD, pParent)
{
	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint();
	m_dDensity = 0;

}

void CCMDRefwdKDS2016Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMDTendonAreaDlg)
	DDX_Control(pDX, IDC_ETC_HCRP_TDMT_KDS2016_MATL_CMB, m_wndMatComb);
	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMDRefwdKDS2016Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMDTendonAreaDlg)
	ON_CBN_SELCHANGE(IDC_ETC_HCRP_TDMT_KDS2016_MATL_CMB, OnCmdTdmtKSD2016MatlCmb)
	ON_BN_CLICKED(IDC_ETC_HCRP_TDMT_KDS2016_MATL_BTN, OnCmdTdmtKSD2016MatlBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMDRefwdKDS2016Dlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

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
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMDRefwdKDS2016Dlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	POSITION pos = pViewBuff->GetStartBuffer();
	T_UDRD_BUFFER buffer_ur;
	BOOL bMATL = FALSE;
	while (pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case UR_MATL_ADD:
		case UR_MATL_DEL:
		case UR_MATL_MFD:
		case UR_MATL_MFS:
			InitCombo();
			OnCmdTdmtKSD2016MatlCmb();
			break;
		default:
			break;
		}
	}
}



/////////////////////////////////////////////////////////////////////////////
// CCMDTendonAreaDlg message handlers


// IsKoreaCodeForSteel
// IsJapanCodeForSteel
BOOL CCMDRefwdKDS2016Dlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCombo();
	m_wndMatComb.SetCurSel(0);
	OnCmdTdmtKSD2016MatlCmb();
	/*
	static double TendonArea[]      =        {0.00009871,0.0001387,}; // metric unit

	m_wndTendonAreaCombo.AddString(_T("12.7mm(0.5\"))_T(");
	m_wndTendonAreaCombo.SetItemData(0,(DWORD)(TendonArea));
	m_wndTendonAreaCombo.AddString(")15.2mm(0.6\_T(")"));
	m_wndTendonAreaCombo.SetItemData(1,(DWORD)(TendonArea+1));
	m_wndTendonAreaCombo.SetCurSel(0);
	*/

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMDRefwdKDS2016Dlg::OnOK()
{	
	CDialogMove::OnOK();
}




/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMDRefwdKDS2016Dlg::InitCombo()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	m_wndMatComb.ResetContent();
	T_MATL_D MatlD; MatlD.Initialize();
	CArray<T_MATL_K, T_MATL_K> aMatlK;
	pDoc->m_pAttrCtrl->GetMatlKeyList(aMatlK);

	for (int i = 0; i < aMatlK.GetSize(); i++)
	{
		if (!pDoc->m_pAttrCtrl->GetMatl(aMatlK[i], MatlD)) continue;
		if (MatlD.Type == _T("C") || MatlD.Type == _T("SRC"))
		{
			m_wndMatComb.SetItemData(m_wndMatComb.AddString(MatlD.Name), aMatlK[i]);
		}
	}

	m_wndMatComb.SetCurSel(0);

	CDlgUtil::CobxAdjustListBoxWidth(m_wndMatComb);
}

void CCMDRefwdKDS2016Dlg::OnCmdTdmtKSD2016MatlBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PROP_MATERIAL, 0));
}


void CCMDRefwdKDS2016Dlg::OnCmdTdmtKSD2016MatlCmb()
{
	if (m_wndMatComb.GetCount() == 0) return;

	double dDensity = 0.0;

	T_MATL_K MatlK;
	MatlK = m_wndMatComb.GetItemData(m_wndMatComb.GetCurSel());

	T_MATL_D MatlD; MatlD.Initialize();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl->GetMatl(MatlK, MatlD))
	{
		ASSERT(0);
		return;
	}

	if (MatlD.Type == _T("C"))
	{
		dDensity = MatlD.Data1.Analysis.Density;
	}
	else if (MatlD.Type == _T("SRC"))
	{
		dDensity = MatlD.Data2.Analysis.Density;
	}
	else
	{
		ASSERT(0);
	}

	dDensity = pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KG, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, dDensity);
	m_dDensity = dDensity;
}