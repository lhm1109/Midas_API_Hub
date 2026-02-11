// ModifyColmLocPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ModifyColmLocPage.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifyColmLocPage dialog

CModifyColmLocPage::CModifyColmLocPage(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CModifyColmLocPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModifyColmLocPage)
	m_nAddDel = 0;
	m_nColLoc = 0;
	m_bEnable = TRUE;
	m_pDoc = CDBDoc::GetDocPoint();

	m_aCtrlsDist1.Add(IDC_TM_COLM_LOC_DIR1_CMB);
	m_aCtrlsDist1.Add(IDC_TM_COLM_LOC_DIST1_EDT);
	m_aCtrlsDist1.Add(IDC_TM_COLM_LOC_DIST1_UNT);
	m_aCtrlsDist2.Add(IDC_TM_COLM_LOC_DIR2_CMB);
	m_aCtrlsDist2.Add(IDC_TM_COLM_LOC_DIST2_EDT);
	m_aCtrlsDist2.Add(IDC_TM_COLM_LOC_DIST2_UNT);

	m_aCtrlsAll.Add(IDC_TM_COLM_LOC_INTR_RDO);
	m_aCtrlsAll.Add(IDC_TM_COLM_LOC_EDGE_RDO);
	m_aCtrlsAll.Add(IDC_TM_COLM_LOC_CONR_RDO);
	m_aCtrlsAll.Add(IDC_TM_COLM_LOC_DIR1_CMB);
	m_aCtrlsAll.Add(IDC_TM_COLM_LOC_DIST1_EDT);
	m_aCtrlsAll.Add(IDC_TM_COLM_LOC_DIST1_UNT);
	m_aCtrlsAll.Add(IDC_TM_COLM_LOC_DIR2_CMB);
	m_aCtrlsAll.Add(IDC_TM_COLM_LOC_DIST2_EDT);
	m_aCtrlsAll.Add(IDC_TM_COLM_LOC_DIST2_UNT); 
}

CModifyColmLocPage::~CModifyColmLocPage()
{

}

void CModifyColmLocPage::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifyColmLocPage)
	DDX_Radio(pDX, IDC_TM_COLM_LOC_ADDREP_RDO , m_nAddDel );
	DDX_Radio(pDX, IDC_TM_COLM_LOC_INTR_RDO   , m_nColLoc );
	DDX_Control(pDX, IDC_TM_COLM_LOC_DIR1_CMB , m_cmbDir1 );
	DDX_Control(pDX, IDC_TM_COLM_LOC_DIR2_CMB , m_cmbDir2 );
	DDX_Control(pDX, IDC_TM_COLM_LOC_DIST1_EDT, m_edtDist1);
	DDX_Control(pDX, IDC_TM_COLM_LOC_DIST2_EDT, m_edtDist2);
	DDX_Control(pDX, IDC_TM_COLM_LOC_DIST1_UNT, m_untDist1);
	DDX_Control(pDX, IDC_TM_COLM_LOC_DIST2_UNT, m_untDist2);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CModifyColmLocPage, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CModifyColmLocPage)
	ON_BN_CLICKED(IDC_TM_COLM_LOC_ADDREP_RDO, EnableDisableCtrls)
	ON_BN_CLICKED(IDC_TM_COLM_LOC_DELETE_RDO, EnableDisableCtrls)
	ON_BN_CLICKED(IDC_TM_COLM_LOC_INTR_RDO, EnableDisableCtrls)
	ON_BN_CLICKED(IDC_TM_COLM_LOC_EDGE_RDO, EnableDisableCtrls)
	ON_BN_CLICKED(IDC_TM_COLM_LOC_CONR_RDO, EnableDisableCtrls)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyColmLocPage message handlers

void CModifyColmLocPage::Execute()
{
	if(!m_bEnable) return;

	UpdateData(TRUE);

	CArray<T_ELEM_K,T_ELEM_K> aElemKAll, aElemK;
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemKAll);

	CString strMsg;
	const int MBTP_COLUMN=1;
	T_ELEM_K ElemK=0;
	T_ELEM_D ElemD;
	T_SECT_D SectD;
	for(int i=0; i<aElemKAll.GetSize(); i++)
	{
		ElemK = aElemKAll[i];

		// 기둥인가?
		if(m_pDoc->m_pAttrCtrl->ExistMbtp(ElemK))
		{
			T_MBTP_D rData;
			rData.Initialize();
			m_pDoc->m_pAttrCtrl->GetMbtp(ElemK,rData); // 사용자 정의방식
			if(rData.nMbType!=MBTP_COLUMN) 
			{
				strMsg.Format(_LS(IDS_DB_COLC_ELEM_NOT_COLUMN), ElemK);
				GSaveHistoryNF(strMsg);
				continue;
			}
		}
		else
		{      
			// 일반적인 방식
			if(m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=MBTP_COLUMN) 
			{
				strMsg.Format(_LS(IDS_DB_COLC_ELEM_NOT_COLUMN), ElemK);
				GSaveHistoryNF(strMsg);
				continue; 
			}
		}

		// Solid Box인가?
		if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); continue; }
		if(!m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro , SectD)) { ASSERT(0); continue; }
		if(SectD.SectBefore.Shape != D_SECT_SHAPE_REG_SB) 
		{
			strMsg.Format(_LS(IDS_DB_COLC_ELEM_NOT_SB), ElemK);
			GSaveHistoryNF(strMsg);
			continue;
		}

		aElemK.Add(ElemK);
	}

	if(m_nAddDel == 0) // ADD/REPLACE
	{
		T_COLC_D ColcD;
		ColcD.nColumnLocation = m_nColLoc;
		ColcD.nCriSectDir[0]  = m_cmbDir1.GetCurSel();
		ColcD.nCriSectDir[1]  = m_cmbDir2.GetCurSel();
		ColcD.dCriSectDis[0]  = m_edtDist1.GetEditValue();
		ColcD.dCriSectDis[1]  = m_edtDist2.GetEditValue();
		m_pDoc->m_pDataCtrl->AddColc(aElemK, ColcD);
	}
	else // DELETE
	{
		m_pDoc->m_pDataCtrl->DelColc(aElemK);
	}  

	Initial_Data();
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

BOOL CModifyColmLocPage::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	Initial_Data();

	Update_InitDataByCode();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CModifyColmLocPage::Initial_Data()
{
	m_nAddDel = 0;
	m_nColLoc = 0;

	UpdateData(FALSE);
	
	InitCtrls();

	EnableDisableCtrls();
}

void CModifyColmLocPage::InitCtrls()
{
	m_cmbDir1.ResetContent();
	m_cmbDir2.ResetContent();
	CString strDist[4] = { _T("+y"), _T("-y"), _T("+z"), _T("-z") };
	for(int i=0; i<4; i++)
	{
		m_cmbDir1.AddString(strDist[i]);
		m_cmbDir2.AddString(strDist[i]);
	}
	m_cmbDir1.SetCurSel(0);
	m_cmbDir2.SetCurSel(2);

	m_edtDist1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtDist2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	double dLength = CUnitCtrl::ConvValueTgtUnit2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 0.); // 0.m
	m_edtDist1.SetEditUnit(dLength);
	m_edtDist2.SetEditUnit(dLength);
}
///////////////////////////////////////////////////////////////////////////////
//
// DB Update

void CModifyColmLocPage::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		Update_InitDataByCode();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CModifyColmLocPage::PostNcDestroy()
{
	CMenuBarChildDlg::PostNcDestroy();
}


void CModifyColmLocPage::EnableDisableCtrls()
{
	UpdateData(TRUE);

	if(m_nAddDel==0)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlsAll, TRUE);
		switch (m_nColLoc)
		{
		case 0:
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlsDist1, FALSE);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlsDist2, FALSE);
			break;
		case 1: 
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlsDist1, TRUE);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlsDist2, FALSE);
			break;
		case 2: 
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlsDist1, TRUE);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlsDist2, TRUE);
			break;
		default: ASSERT(0); break;
		}
	}
	else CDlgUtil::CtrlEnableDisable(this, m_aCtrlsAll, FALSE);
	
}

void CModifyColmLocPage::Update_InitDataByCode()
{
	BOOL bEnable=FALSE;

	if(CDBLib::IsCodeForMeshedDesign() ||
		 CDBLib::IsCodeForMeshedDesignACI() ||
		 CDBLib::IsCodeForMeshedDesignKCI() ) bEnable=TRUE;

	if(m_bEnable==bEnable) return;

	m_bEnable=bEnable;
	CArray<UINT, UINT> aCtrl2;
	aCtrl2.Add(IDC_TM_COLM_LOC_ADDREP_RDO);
	aCtrl2.Add(IDC_TM_COLM_LOC_DELETE_RDO);

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlsAll, m_bEnable);
	CDlgUtil::CtrlEnableDisable(this, aCtrl2, m_bEnable);

	if(bEnable) EnableDisableCtrls();
}