// MPhiParameterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "MPhiParameterDlg.h"

#include "MPhiGlobalDlg.h"
#include "MPhiHingeDlg.h"
#include "MPhiSectionDlg.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\MatlDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPhiParameterDlg dialog


CMPhiParameterDlg::CMPhiParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CMPhiParameterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMPhiParameterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nKind = 0;
	m_nModifyKey = 0;
}


void CMPhiParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMPhiParameterDlg)
	DDX_Control(pDX, IDC_CMD_TAB_CTRL, m_cTabCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMPhiParameterDlg, CDialogMove)
	//{{AFX_MSG_MAP(CMPhiParameterDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB_CTRL, OnSelchangeCmdTabCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPhiParameterDlg message handlers
BOOL CMPhiParameterDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	SetMembDataByDB();

	// Section Tab에서 데이터 자동세팅에 Rebar정보를 가지고 오는 부분이 있는데.. 거기 있는 함수를 부르기전에 m_pMatlDB->Create_RebarData를 한번 해주어야 함..
	T_UNIT_INDEX currUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(currUnit);
	m_pDoc->m_pMatlDB->Create_RebarData(currUnit.nBase_Length, currUnit.nBase_Force, TRUE);

	// Add Child Dialogs.........
	// Global..
	m_pDlgGlobal = new CMPhiGlobalDlg(&m_DataAll, this);
	m_cTabCtrl.AddTab(m_pDlgGlobal, _LS(IDS_CMD_MPHI_GROBAL), CMPhiGlobalDlg::IDD, TRUE);
	// Inelastic Hinge....
	m_pDlgHinge = new CMPhiHingeDlg(&m_DataAll, this);
	m_cTabCtrl.AddTab(m_pDlgHinge, _LS(IDS_CMD_MPHI_INELASTIC_HINGE), CMPhiHingeDlg::IDD, TRUE);
	// Section....
	m_pDlgSection = new CMPhiSectionDlg(&m_DataAll, this);
	m_pDlgSection->InitData(); 
	m_pDlgSection->SetDlgDataToParent();  // Tab을 안바꾸고 OK해도 기본데이터가 들어가게 하기 위해서 여기서 init을 한다음 Member변수에 세팅..
	m_cTabCtrl.AddTab(m_pDlgSection, _LS(IDS_CMD_MPHI_SECTION), CMPhiSectionDlg::IDD, TRUE);

	// Dialog Page Manager에 Place Holder 설정
	BOOL bSet;
	CWnd* pPH = (CWnd*)GetDlgItem(IDC_CMD_PLACEHOLDER);
	ASSERT(pPH && pPH->GetSafeHwnd() && IsWindow(pPH->GetSafeHwnd()));
	bSet = m_cTabCtrl.SetPlaceHolder(pPH);
	ASSERT(bSet);

	if(m_nKind==1)				
		m_pDlgHinge->SetModifyData(m_nModifyKey);
	m_cTabCtrl.ShowTab(m_nKind);
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// nKind : (0)MPGB (1)MPHG (2)MPST
void CMPhiParameterDlg::SetParamData(int nKind, UINT Key)
{
	m_nKind = nKind;
	m_nModifyKey = Key;
}

void CMPhiParameterDlg::SetMembDataByDB()
{
	m_DataAll.Init();  
	m_pDoc->m_pAttrCtrl->GetMpgb(m_DataAll.MpgbD);

	CArray<T_MPHG_K, T_MPHG_K> aMphgK;
	m_pDoc->m_pAttrCtrl->GetMphgKeyList(aMphgK);
	for (int i = 0; i < aMphgK.GetSize() ; i++)
	{
		T_MPHG_D MphgD;
		m_pDoc->m_pAttrCtrl->GetMphg(aMphgK[i], MphgD);
		m_DataAll.aMphgK.Add(aMphgK[i]);
		m_DataAll.aMphgD.Add(MphgD);
	}

	CArray<T_MPST_K, T_MPST_K> aMpstK;
	m_pDoc->m_pAttrCtrl->GetMpstKeyList(aMpstK);
	for (int i = 0; i < aMpstK.GetSize() ; i++)
	{
		T_MPST_D MpstD;
		m_pDoc->m_pAttrCtrl->GetMpst(aMpstK[i], MpstD);
		m_DataAll.aMpstK.Add(aMpstK[i]);
		m_DataAll.aMpstD.Add(MpstD);
	}
}

void CMPhiParameterDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	//DestroyWindow();
	CDialogMove::OnCancel();
}

void CMPhiParameterDlg::OnOK()
{
	if(m_pDlgGlobal && m_pDlgGlobal->GetSafeHwnd() && IsWindow(m_pDlgGlobal->GetSafeHwnd()))
	{
		m_pDlgGlobal->Dlg2Data();  
	}
	if(m_pDlgSection && m_pDlgSection->GetSafeHwnd() && IsWindow(m_pDlgSection->GetSafeHwnd()))
	{
		m_pDlgSection->Dlg2Data();  
	}

	if(!m_pDoc->m_pDataCtrl->AddMPhiParameters(m_DataAll.MpgbD, m_DataAll.aMphgK, m_DataAll.aMphgD, m_DataAll.aMpstK, m_DataAll.aMpstD))
	{
		return;
	}

	CDialogMove::OnOK();
}
/*
void CMPhiParameterDlg::OnChildOk() 
{
	ASSERT(0); // 지워야 되는 함수
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}*/

void CMPhiParameterDlg::OnSelchangeCmdTabCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CMPhiParameterDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

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


void CMPhiParameterDlg::UpdateBuffer(LPARAM lHint, CObject* pHint)
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	//T_MATL_K Key, KeyBak;
	//T_MATD_D Data, DataBak;

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
			m_pDlgHinge->MyUpdate(lHint, pHint);
			break;
		case(UR_RPSC_ADD):
		case(UR_RPSC_DEL):
		case(UR_RCHK_ADD):
		case(UR_RCHK_DEL):
			m_pDlgSection->MyUpdate(lHint, pHint);
			break;
		default:
			break;
		}
	} // end of while
}
