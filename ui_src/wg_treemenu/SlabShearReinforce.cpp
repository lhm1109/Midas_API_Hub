// ModifyColmLocPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SlabShearReinforce.h"

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

CSlabShearReinforcePage::CSlabShearReinforcePage(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSlabShearReinforcePage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModifyColmLocPage)
	m_nAddDel = 0;
	m_pDoc = CDBDoc::GetDocPoint();

	m_aCtrlID.Add(IDC_TM_SLAB_SHEAR_AV_Y_STC);
	m_aCtrlID.Add(IDC_TM_SLAB_SHEAR_AV_Y_EDT);
	m_aCtrlID.Add(IDC_TM_SLAB_SHEAR_AV_Y_UNT);

	m_aCtrlID.Add(IDC_TM_SLAB_SHEAR_AV_Z_STC);
	m_aCtrlID.Add(IDC_TM_SLAB_SHEAR_AV_Z_EDT);
	m_aCtrlID.Add(IDC_TM_SLAB_SHEAR_AV_Z_UNT);

	m_aCtrlID.Add(IDC_TM_SLAB_SHEAR_SAPCE_STC);
	m_aCtrlID.Add(IDC_TM_SLAB_SHEAR_SPACE_EDT);
	m_aCtrlID.Add(IDC_TM_SLAB_SHEAR_SPACE_UNT);

	m_aCtrlID.Add(IDC_TM_SLAB_SHEAR_YIELD_STC);
	m_aCtrlID.Add(IDC_TM_SLAB_SHEAR_YIELD_EDT);
	m_aCtrlID.Add(IDC_TM_SLAB_SHEAR_YIELD_UNT);


	m_aCtrlID2.Add(IDC_TM_SLAB_SHEAR_ADDREP_RDO);
	m_aCtrlID2.Add(IDC_TM_SLAB_SHEAR_DELETE_RDO);

}

CSlabShearReinforcePage::~CSlabShearReinforcePage()
{

}

void CSlabShearReinforcePage::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSlabShearReinforcePage)
	DDX_Radio(pDX, IDC_TM_SLAB_SHEAR_ADDREP_RDO , m_nAddDel );

	DDX_Control(pDX, IDC_TM_SLAB_SHEAR_AV_Y_EDT, m_edtAvY);
	DDX_Control(pDX, IDC_TM_SLAB_SHEAR_AV_Z_EDT, m_edtAvZ);

	DDX_Control(pDX, IDC_TM_SLAB_SHEAR_SPACE_EDT, m_edtSpace);
	DDX_Control(pDX, IDC_TM_SLAB_SHEAR_YIELD_EDT, m_edtYield);

	DDX_Control(pDX, IDC_TM_SLAB_SHEAR_AV_Y_UNT, m_untAvY);
	DDX_Control(pDX, IDC_TM_SLAB_SHEAR_AV_Z_UNT, m_untAvZ);
	DDX_Control(pDX, IDC_TM_SLAB_SHEAR_SPACE_UNT, m_untSpace);
	DDX_Control(pDX, IDC_TM_SLAB_SHEAR_YIELD_UNT, m_untYield);
	DDX_Control(pDX, IDC_TM_SLAB_SHEAR_PIC_STC, m_wndShearImg);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CSlabShearReinforcePage, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CSlabShearReinforcePage)
	ON_BN_CLICKED(IDC_TM_SLAB_SHEAR_ADDREP_RDO , OnChangeOption)
	ON_BN_CLICKED(IDC_TM_SLAB_SHEAR_DELETE_RDO   , OnChangeOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyColmLocPage message handlers

void CSlabShearReinforcePage::Execute()
{
	if(!CDBLib::IsCodeForMeshedDesignKCI()) return;

	UpdateData(TRUE);

	CArray<T_ELEM_K,T_ELEM_K> aElemKAll, aElemK;
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemKAll);

	CString strMsg;
	const int MBTP_COLUMN=1;
	T_ELEM_K ElemK=0;
//   T_ELEM_D ElemD;
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
				strMsg.Format(_LS(IDS_DB_SSRI_ELEM_NOT_COLUMN), ElemK);
				GSaveHistoryNF(strMsg);
				continue;
			}
		}
		else
		{      
			// 일반적인 방식
			if(m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=MBTP_COLUMN) 
			{
				strMsg.Format(_LS(IDS_DB_SSRI_ELEM_NOT_COLUMN), ElemK);
				GSaveHistoryNF(strMsg);
				continue; 
			}
		}

		// Solid Box인가?
//     if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); continue; }
//     if(!m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro , SectD)) { ASSERT(0); continue; }
//     if(SectD.SectBefore.Shape != D_SECT_SHAPE_REG_SB) 
//     {
//       strMsg.Format(_LS(IDS_DB_COLC_ELEM_NOT_SB), ElemK);
//       GSaveHistoryNF(strMsg);
//       continue;
//     }

		aElemK.Add(ElemK);
	}

	if(m_nAddDel == 0) // ADD/REPLACE
	{
		T_SSRI_D SsriD;
		SsriD.dAvY = m_edtAvY.GetEditValue();
		SsriD.dAvZ = m_edtAvZ.GetEditValue();
		SsriD.dSpace = m_edtSpace.GetEditValue();
		SsriD.dYield = m_edtYield.GetEditValue();

		m_pDoc->m_pDataCtrl->AddSsri(aElemK, SsriD);
	}
	else // DELETE
	{
		m_pDoc->m_pDataCtrl->DelSsri(aElemK);
	}  

	Initial_Data();
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

BOOL CSlabShearReinforcePage::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	InitCtrls();

	// 비활성화 처리..(KCI-USD12가 아닐때)
	Initial_Data();

	Update_InitDataByCode();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSlabShearReinforcePage::InitCtrls()
{
	m_edtAvY.SetUnitType(D_UNITSYS_BASE_AREA);
	m_edtAvZ.SetUnitType(D_UNITSYS_BASE_AREA);
	m_edtSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtYield.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_untAvY.SetUnitType(D_UNITSYS_BASE_AREA);
	m_untAvZ.SetUnitType(D_UNITSYS_BASE_AREA);
	m_untSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untYield.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_wndShearImg.SetImage(_T("SVG\\Illustration\\Dialog\\SlabShearReinfor.svg"));
}


void CSlabShearReinforcePage::Initial_Data()
{
	m_nAddDel	= 0;	// add/replace
	m_edtAvY.SetEditUnit(0);
	m_edtAvZ.SetEditUnit(0);
	m_edtSpace.SetEditUnit(0);
	m_edtYield.SetEditUnit(0);

	UpdateData(FALSE);
}
///////////////////////////////////////////////////////////////////////////////
//
// DB Update
// 
void CSlabShearReinforcePage::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
	case D_UPDATE_UNIT:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CSlabShearReinforcePage::PostNcDestroy()
{
	CMenuBarChildDlg::PostNcDestroy();
}

void CSlabShearReinforcePage::OnChangeOption()
{
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlID, m_nAddDel == 0);
}

void CSlabShearReinforcePage::Update_InitDataByCode()
{
	BOOL bEnable=TRUE;
	if(!CDBLib::IsCodeForMeshedDesignKCI()) bEnable=FALSE;

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlID, bEnable);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlID2, bEnable);

	if(bEnable) OnChangeOption();
}