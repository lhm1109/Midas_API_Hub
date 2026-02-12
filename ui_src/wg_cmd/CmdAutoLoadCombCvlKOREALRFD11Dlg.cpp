// CmdAutoLoadCombCvlKOREALRFD11Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlKOREALRFD11Dlg.h"
#include "CmdAutoLoadCombCvlDlg.h"
#include "CmdAutoLoadCombCvlHelpDlg.h"
#include "LoadCombCtrl.h"


#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_DBDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlKOREALRFD11Dlg dialog
CCmdAutoLoadCombCvlKOREALRFD11Dlg::CCmdAutoLoadCombCvlKOREALRFD11Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCmdAutoLoadCombCvlKOREALRFD11Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadCombCvlKOREALRFD11Dlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_pParent = (CCmdAutoLoadCombCvlDlg*)pParent;

	m_nConstLoad = 0;

	m_nLrfd11DC = 0;
	m_nLrfd11DD = 0;
	m_nLrfd11DW = 0;
	m_nLrfd11EHType = 0;
	m_nLrfd11EH1 = 0;
	m_nLrfd11EH2 = 0;
	m_nLrfd11EVType = 0;
	m_nLrfd11EV2 = 0;
	m_nLrfd11EV3 = 0;
	m_nLrfd11EV4 = 0;
	m_nLrfd11EV5 = 0;
	m_nLrfd11EV6 = 0;
	m_nLrfd11ES = 0;
	m_bLrfd11CulvertChk = FALSE;
	m_bLrfd11GravityLoadChk = FALSE;
	m_nLrfd11Deformation = 0;

	m_aDLData.RemoveAll();
}

void CCmdAutoLoadCombCvlKOREALRFD11Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadCombCvlKOREALRFD11Dlg)  
	DDX_Control(pDX, IDC_CMD_KOREA_LRFD11_LOAD_MODIFIER_EDT,      m_edtLrfd11Modifier);
	DDX_Radio  (pDX, IDC_CMD_KOREA_LRFD11_DC_MAX_RDO,             m_nLrfd11DC);
	DDX_Radio  (pDX, IDC_CMD_KOREA_LRFD11_DD_MAX_RDO,             m_nLrfd11DD);
	DDX_Radio  (pDX, IDC_CMD_KOREA_LRFD11_DW_MAX_RDO,             m_nLrfd11DW);
	DDX_Radio  (pDX, IDC_CMD_KOREA_LRFD11_EH_RDO1,                m_nLrfd11EHType);
	DDX_Radio  (pDX, IDC_CMD_KOREA_LRFD11_EH1_MAX_RDO,            m_nLrfd11EH1);
	DDX_Radio  (pDX, IDC_CMD_KOREA_LRFD11_EH2_MAX_RDO,            m_nLrfd11EH2);
	DDX_Radio  (pDX, IDC_CMD_KOREA_LRFD11_EV_RDO1,                m_nLrfd11EVType);
	DDX_Radio  (pDX, IDC_CMD_KOREA_LRFD11_EV2_MAX_RDO,            m_nLrfd11EV2);
	DDX_Radio  (pDX, IDC_CMD_KOREA_LRFD11_EV3_MAX_RDO,            m_nLrfd11EV3);
	DDX_Radio  (pDX, IDC_CMD_KOREA_LRFD11_EV4_MAX_RDO,            m_nLrfd11EV4);
	DDX_Radio  (pDX, IDC_CMD_KOREA_LRFD11_EV5_MAX_RDO,            m_nLrfd11EV5);
	DDX_Radio  (pDX, IDC_CMD_KOREA_LRFD11_EV6_MAX_RDO,            m_nLrfd11EV6);
	DDX_Radio  (pDX, IDC_CMD_KOREA_LRFD11_ES_MAX_RDO,             m_nLrfd11ES);
	DDX_Control(pDX, IDC_CMD_KOREA_LRFD11_SETTLEMENT_EDT,         m_edtLrfd11Settlement);
	DDX_Check  (pDX, IDC_CMD_KOREA_LRFD11_CULVERT_CHK,            m_bLrfd11CulvertChk);
	DDX_Radio  (pDX, IDC_CMD_KOREA_LRFD11_DEFORMATION_RDO1,       m_nLrfd11Deformation);
	DDX_Control(pDX, IDC_CMD_KOREA_LRFD11_PS_CR_SH_CMB,           m_cmbPSCRSHType);
	DDX_Control(pDX, IDC_CMD_KOREA_LRFD11_FATI_LL_CMB,            m_cmbLLCase);
	DDX_Control(pDX, IDC_CMD_KOREA_LRFD11_FATI_DD_CMB,            m_cmbDLCase);
	DDX_Control(pDX, IDC_CMD_KOREA_LRFD11_FATI_DD_LST,            m_lstDL);
	DDX_Check  (pDX, IDC_CMD_KOREA_LRFD11_GRAVLOAD_CHK,           m_bLrfd11GravityLoadChk);
	DDX_Control(pDX, IDC_CMD_KOREA_LRFD11_EFFLOAD_EDT,            m_edtLrfd11EffLoadFactor);
	DDX_Control(pDX, IDC_CMD_KOREA_LRFD11_LOAD_FACTOR_GRP,        m_chkLoadFactor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlKOREALRFD11Dlg, CChildDialog)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombCvlKOREALRFD11Dlg)
	ON_BN_CLICKED(IDC_CMD_KOREA_LRFD11_EH_RDO1,          OnCmdKoreaLRFD11EHTypeRdo)
	ON_BN_CLICKED(IDC_CMD_KOREA_LRFD11_EH_RDO2,          OnCmdKoreaLRFD11EHTypeRdo)
	ON_BN_CLICKED(IDC_CMD_KOREA_LRFD11_EV_RDO1,          OnCmdKoreaLRFD11EVTypeRdo)
	ON_BN_CLICKED(IDC_CMD_KOREA_LRFD11_EV_RDO2,          OnCmdKoreaLRFD11EVTypeRdo)
	ON_BN_CLICKED(IDC_CMD_KOREA_LRFD11_EV_RDO3,          OnCmdKoreaLRFD11EVTypeRdo)
	ON_BN_CLICKED(IDC_CMD_KOREA_LRFD11_EV_RDO4,          OnCmdKoreaLRFD11EVTypeRdo)
	ON_BN_CLICKED(IDC_CMD_KOREA_LRFD11_EV_RDO5,          OnCmdKoreaLRFD11EVTypeRdo)
	ON_BN_CLICKED(IDC_CMD_KOREA_LRFD11_EV_RDO6,          OnCmdKoreaLRFD11EVTypeRdo)
	ON_BN_CLICKED(IDC_CMD_KOREA_LRFD11_PS_CR_SH_BTN,     OnCmdKoreaLRFD11PSCRSHBtn)
	ON_BN_CLICKED(IDC_CMD_KOREA_LRFD11_FATI_DD_ADD_BTN,  OnCmdKoreaLRFD11DLAddBtn)
	ON_BN_CLICKED(IDC_CMD_KOREA_LRFD11_FATI_DD_DEL_BTN,  OnCmdKoreaLRFD11DLDelBtn)
	ON_BN_CLICKED(IDC_CMD_KOREA_LRFD11_GRAVLOAD_CHK,     OnCmdKoreaLRFD11GRACHK)

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCmdAutoLoadCombCvlKOREALRFD11Dlg::InitPSCRSHCombo()
{
	m_cmbPSCRSHType.ResetContent();
	m_cmbPSCRSHType.AddString(_T("A"));
	m_cmbPSCRSHType.AddString(_T("B"));
	m_cmbPSCRSHType.AddString(_T("C"));
	m_cmbPSCRSHType.AddString(_T("D"));
	m_cmbPSCRSHType.AddString(_T("E"));
	m_cmbPSCRSHType.SetCurSel(0);
}

void CCmdAutoLoadCombCvlKOREALRFD11Dlg::InitLLCombo()
{
	m_cmbLLCase.ResetContent();
	m_cmbLLCase.AddString(_LS(IDS_WG_CMD__ADDD__None));

	CArray<T_MVLD_K, T_MVLD_K> KeyList; KeyList.RemoveAll();
	CArray<T_MVLDjp_K, T_MVLDjp_K> KeyListjp; KeyListjp.RemoveAll();
	CArray<T_MVLDch_K, T_MVLDch_K> KeyListch; KeyListch.RemoveAll();
	CArray<T_MVLDid_K, T_MVLDid_K> KeyListid; KeyListid.RemoveAll();
	CArray<T_MVLDbs_K, T_MVLDbs_K> KeyListbs; KeyListbs.RemoveAll();
	CArray<T_MVLDfr_K, T_MVLDfr_K> KeyListfr; KeyListfr.RemoveAll();
	CArray<T_MVLDtr_K, T_MVLDtr_K> KeyListtr; KeyListtr.RemoveAll();

	// 코드에 따른 방식으로 수정 
	int nCount = 0;
	T_MVCD_D DataMvcd; DataMvcd.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();

	switch (DataMvcd.nCodeType)
	{
	case D_MOVE_CODE_NONE:
		break;
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_AASHTO_LRFD:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_KOREA:
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_PENDOT:
	case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_SOUTH_AFRICA:
	case D_MOVE_CODE_JAPAN_RAIL:
	case D_MOVE_CODE_NEWZEALAND:
	case D_MOVE_CODE_BRAZIL:
		m_pDoc->m_pAttrCtrl->GetMvldKeyList(KeyList);      nCount = KeyList.GetSize(); 
		break;
	case D_MOVE_CODE_CHINA:
		m_pDoc->m_pAttrCtrl->GetMvldchKeyList(KeyListch);  nCount = KeyListch.GetSize();
		break;
	case D_MOVE_CODE_JAPAN:
		m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(KeyListjp);  nCount = KeyListjp.GetSize();
		break;
	case D_MOVE_CODE_INDIA:
		m_pDoc->m_pAttrCtrl->GetMvldidKeyList(KeyListid);  nCount = KeyListid.GetSize(); 
		break;
	case D_MOVE_CODE_BS:
		m_pDoc->m_pAttrCtrl->GetMvldbsKeyList(KeyListbs);  nCount = KeyListbs.GetSize(); 
		break;
	case D_MOVE_CODE_FRANCE:
		m_pDoc->m_pAttrCtrl->GetMvldfrKeyList(KeyListfr);  nCount = KeyListfr.GetSize();
		break;
	case D_MOVE_CODE_TRANS:
		m_pDoc->m_pAttrCtrl->GetMvldtrKeyList(KeyListtr);  nCount = KeyListtr.GetSize(); 
		break;
	default:
		ASSERT(0);
		break;
	}

	for(int i=0; i<nCount; i++)
	{      
		T_MVLD_D rData; rData.Initialize();
		T_MVLDjp_D rDatajp; m_pDoc->m_pAttrCtrl->InitializeMvldjp(rDatajp);
		T_MVLDch_D rDatach; rDatach.Initialize();
		T_MVLDid_D rDataid; rDataid.Initialize();
		T_MVLDbs_D rDatabs; rDatabs.Initialize();
		T_MVLDfr_D rDatafr; rDatafr.Initialize();
		T_MVLDtr_D rDatatr; rDatatr.Initialize();

		T_MVCD_D DataMvcd;
		if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
		switch (DataMvcd.nCodeType)
		{
		case D_MOVE_CODE_NONE:
			break;
		case D_MOVE_CODE_AASHTO_STAN:
		case D_MOVE_CODE_AASHTO_LRFD:
		case D_MOVE_CODE_TAIWAN:
		case D_MOVE_CODE_KOREA:
		case D_MOVE_CODE_CANADA:
		case D_MOVE_CODE_PENDOT:
		case D_MOVE_CODE_EURO_BS:
		case D_MOVE_CODE_RUSSIA:
		case D_MOVE_CODE_KOREA_LRFD_2011:
		case D_MOVE_CODE_AUSTRALIA:
		case D_MOVE_CODE_POLAND:
		case D_MOVE_CODE_SOUTH_AFRICA:
		case D_MOVE_CODE_JAPAN_RAIL:
		case D_MOVE_CODE_NEWZEALAND:
		case D_MOVE_CODE_BRAZIL:
			{
				if(!m_pDoc->m_pAttrCtrl->GetMvld(KeyList.GetAt(i), rData)) ASSERT(0); 
				m_cmbLLCase.AddString(rData.LoadCaseName);
			}
			break;
		case D_MOVE_CODE_CHINA:
			{
				if(!m_pDoc->m_pAttrCtrl->GetMvldch(KeyListch.GetAt(i), rDatach)) ASSERT(0); 
				m_cmbLLCase.AddString(rDatach.LoadCaseName);
			}
			break;
		case D_MOVE_CODE_JAPAN:
			{
				if(!m_pDoc->m_pAttrCtrl->GetMvldjp(KeyListjp.GetAt(i), rDatajp)) ASSERT(0); 
				m_cmbLLCase.AddString(rDatajp.LoadCaseName);
			}
			break;
		case D_MOVE_CODE_INDIA:
			{
				if(!m_pDoc->m_pAttrCtrl->GetMvldid(KeyListid.GetAt(i), rDataid)) ASSERT(0); 
				m_cmbLLCase.AddString(rDataid.LoadCaseName);
			}
			break;
		case D_MOVE_CODE_BS:
			{
				if(!m_pDoc->m_pAttrCtrl->GetMvldbs(KeyListbs.GetAt(i), rDatabs)) ASSERT(0); 
				m_cmbLLCase.AddString(rDatabs.LoadCaseName);
			}
			break;
		case D_MOVE_CODE_FRANCE:
			{
				if(!m_pDoc->m_pAttrCtrl->GetMvldfr(KeyListfr.GetAt(i), rDatafr)) ASSERT(0); 
				m_cmbLLCase.AddString(rDatafr.LoadCaseName);
			}
			break;
		case D_MOVE_CODE_TRANS:
			{
				if(!m_pDoc->m_pAttrCtrl->GetMvldtr(KeyListtr.GetAt(i), rDatatr)) ASSERT(0); 
				m_cmbLLCase.AddString(rDatatr.LoadCaseName);
			}
			break;
		default:
			ASSERT(0);
			break;
		}
	}

	m_cmbLLCase.SetCurSel(0);
}

void CCmdAutoLoadCombCvlKOREALRFD11Dlg::InitDLCombo()
{
	CArray<T_STLD_K, T_STLD_K> arKeyList; arKeyList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int nCount = m_pDoc->m_pAttrCtrl->GetCountStld();

	for(int i=0; i<nCount; i++)
	{
		T_STLD_K StldK = arKeyList.GetAt(i);
		T_STLD_D StldD; StldD.Initialize();

		if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) continue;

		if(StldD.LoadCaseType==_T("D")  || StldD.LoadCaseType==_T("DC") || 
			StldD.LoadCaseType==_T("DD") || StldD.LoadCaseType==_T("DW"))
		{
			m_cmbDLCase.AddString(StldD.LoadCaseName);
		}
	}

	m_cmbDLCase.SetCurSel(0);
}

void CCmdAutoLoadCombCvlKOREALRFD11Dlg::InitDLListBox()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstDL.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstDL.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	CString aTitle[] = { _LS(IDS_WG_CMD__ADDD__D_Load_Case) };

	CString title;
	int width[1] = {130};
	for(int i = 0; i < sizeof(aTitle) / sizeof(CString); i++)
	{
		title = aTitle[i];

		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_LEFT;
		lvcolumn.pszText  = title.GetBuffer(0);
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_lstDL.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCmdAutoLoadCombCvlKOREALRFD11Dlg::CheckDLCaseName(CString strDLName)
{
	BOOL bCheck = TRUE;
	int nCount = m_lstDL.GetItemCount();
	for(int i=0; i<nCount; i++)
	{
		CString strLcName = m_lstDL.GetItemText(i, 0);
		if(strDLName == strLcName)  
		{
			bCheck = FALSE;
		}
	}
	return bCheck;
}

void CCmdAutoLoadCombCvlKOREALRFD11Dlg::GetKoreaLRFD11LoadFactor()
{
	UpdateData(TRUE);

	switch(m_nLrfd11DC)  // Component and Attachments
	{
	case 0: m_nKoreaLr11DcNum = 1; m_dKoreaLr11Dc1 = 1.25; m_dKoreaLr11Dc2 = 1.25; break; 
	case 1: m_nKoreaLr11DcNum = 1; m_dKoreaLr11Dc1 = 0.90; m_dKoreaLr11Dc2 = 0.90; break; 
	case 2: m_nKoreaLr11DcNum = 2; m_dKoreaLr11Dc1 = 1.25; m_dKoreaLr11Dc2 = 0.90; break;
	}
	switch(m_nLrfd11DD)  // Downdrag
	{
	case 0: m_nKoreaLr11DdNum = 1; m_dKoreaLr11Dd1 = 1.80; m_dKoreaLr11Dd2 = 1.80; break; 
	case 1: m_nKoreaLr11DdNum = 1; m_dKoreaLr11Dd1 = 0.45; m_dKoreaLr11Dd2 = 0.45; break; 
	case 2: m_nKoreaLr11DdNum = 2; m_dKoreaLr11Dd1 = 1.80; m_dKoreaLr11Dd2 = 0.45; break;
	}
	switch(m_nLrfd11DW)  // Wearing Surfaces and Utilities
	{
	case 0: m_nKoreaLr11DwNum = 1; m_dKoreaLr11Dw1 = 1.50; m_dKoreaLr11Dw2 = 1.50; break; 
	case 1: m_nKoreaLr11DwNum = 1; m_dKoreaLr11Dw1 = 0.65; m_dKoreaLr11Dw2 = 0.65; break; 
	case 2: m_nKoreaLr11DwNum = 2; m_dKoreaLr11Dw1 = 1.50; m_dKoreaLr11Dw2 = 0.65; break;
	}
	switch(m_nLrfd11EHType)  // Horizontal Earth Pressure
	{
	case 0:   // Active
		if     (m_nLrfd11EH1 == 0) {m_nKoreaLr11EhNum = 1; m_dKoreaLr11Eh1 = 1.50; m_dKoreaLr11Eh2 = 1.50;} 
		else if(m_nLrfd11EH1 == 1) {m_nKoreaLr11EhNum = 1; m_dKoreaLr11Eh1 = 0.90; m_dKoreaLr11Eh2 = 0.90;} 
		else if(m_nLrfd11EH1 == 2) {m_nKoreaLr11EhNum = 2; m_dKoreaLr11Eh1 = 1.50; m_dKoreaLr11Eh2 = 0.90;}      
		break;
	case 1:  // At-Rest
		if     (m_nLrfd11EH2 == 0) {m_nKoreaLr11EhNum = 1; m_dKoreaLr11Eh1 = 1.35; m_dKoreaLr11Eh2 = 1.35;} 
		else if(m_nLrfd11EH2 == 1) {m_nKoreaLr11EhNum = 1; m_dKoreaLr11Eh1 = 0.90; m_dKoreaLr11Eh2 = 0.90;} 
		else if(m_nLrfd11EH2 == 2) {m_nKoreaLr11EhNum = 2; m_dKoreaLr11Eh1 = 1.35; m_dKoreaLr11Eh2 = 0.90;}            
		break;
	}
	m_nKoreaLr11ElNum = 1; m_dKoreaLr11El1 = 1.00; m_dKoreaLr11El2 = 1.00;  // Locked-in Erection Stresses    

	switch(m_nLrfd11EVType)
	{
	case 0: // Overall Stability
		m_nKoreaLr11EvNum = 1; m_dKoreaLr11Ev1 = 1.00; m_dKoreaLr11Ev2 = 1.00;      
		break;
	case 1: // Retaining Walls and Abutments
		if     (m_nLrfd11EV2 == 0) {m_nKoreaLr11EvNum = 1; m_dKoreaLr11Ev1 = 1.35; m_dKoreaLr11Ev2 = 1.35;}
		else if(m_nLrfd11EV2 == 1) {m_nKoreaLr11EvNum = 1; m_dKoreaLr11Ev1 = 1.00; m_dKoreaLr11Ev2 = 1.00;} 
		else if(m_nLrfd11EV2 == 2) {m_nKoreaLr11EvNum = 2; m_dKoreaLr11Ev1 = 1.35; m_dKoreaLr11Ev2 = 1.00;}      
		break;
	case 2:  // Rigid Buried Structure
		if     (m_nLrfd11EV3 == 0) {m_nKoreaLr11EvNum = 1; m_dKoreaLr11Ev1 = 1.30; m_dKoreaLr11Ev2 = 1.30;} 
		else if(m_nLrfd11EV3 == 1) {m_nKoreaLr11EvNum = 1; m_dKoreaLr11Ev1 = 0.90; m_dKoreaLr11Ev2 = 0.90;} 
		else if(m_nLrfd11EV3 == 2) {m_nKoreaLr11EvNum = 2; m_dKoreaLr11Ev1 = 1.30; m_dKoreaLr11Ev2 = 0.90;}      
		break;
	case 3:  // Rigid Frames
		if     (m_nLrfd11EV4 == 0) {m_nKoreaLr11EvNum = 1; m_dKoreaLr11Ev1 = 1.35; m_dKoreaLr11Ev2 = 1.35;} 
		else if(m_nLrfd11EV4 == 1) {m_nKoreaLr11EvNum = 1; m_dKoreaLr11Ev1 = 0.90; m_dKoreaLr11Ev2 = 0.90;} 
		else if(m_nLrfd11EV4 == 2) {m_nKoreaLr11EvNum = 2; m_dKoreaLr11Ev1 = 1.35; m_dKoreaLr11Ev2 = 0.90;}      
		break;
	case 4:  // Flexible buried structures other than metal box culverts
		if     (m_nLrfd11EV5 == 0) {m_nKoreaLr11EvNum = 1; m_dKoreaLr11Ev1 = 1.95; m_dKoreaLr11Ev2 = 1.95;} 
		else if(m_nLrfd11EV5 == 1) {m_nKoreaLr11EvNum = 1; m_dKoreaLr11Ev1 = 0.90; m_dKoreaLr11Ev2 = 0.90;} 
		else if(m_nLrfd11EV5 == 2) {m_nKoreaLr11EvNum = 2; m_dKoreaLr11Ev1 = 1.95; m_dKoreaLr11Ev2 = 0.90;}      
		break;
	case 5:  // Flexible Metal Box Culverts
		if     (m_nLrfd11EV6 == 0) {m_nKoreaLr11EvNum = 1; m_dKoreaLr11Ev1 = 1.50; m_dKoreaLr11Ev2 = 1.50;} 
		else if(m_nLrfd11EV6 == 1) {m_nKoreaLr11EvNum = 1; m_dKoreaLr11Ev1 = 0.90; m_dKoreaLr11Ev2 = 0.90;} 
		else if(m_nLrfd11EV6 == 2) {m_nKoreaLr11EvNum = 2; m_dKoreaLr11Ev1 = 1.50; m_dKoreaLr11Ev2 = 0.90;}      
		break;
	}
	switch(m_nLrfd11ES) // Earth Surcharge
	{
	case 0: m_nKoreaLr11EsNum = 1; m_dKoreaLr11Es1 = 1.50; m_dKoreaLr11Es2 = 1.50; break; 
	case 1: m_nKoreaLr11EsNum = 1; m_dKoreaLr11Es1 = 0.75; m_dKoreaLr11Es2 = 0.75; break; 
	case 2: m_nKoreaLr11EsNum = 2; m_dKoreaLr11Es1 = 1.50; m_dKoreaLr11Es2 = 0.75; break;
	}

	int nPSCRSHType = m_cmbPSCRSHType.GetCurSel();
	m_nKoreaLr11PSCRSHType = nPSCRSHType;

	if(nPSCRSHType == 0) // A : Superstructures - Segmental , Concrete Substructures supporting Segmental, Superstructures 
	{
		m_nKoreaLr11PsNum = 1; m_dKoreaLr11Ps1 = 1.0; m_dKoreaLr11Ps2 = 1.0;
		if(m_nLrfd11DC == 0) 
		{
			m_nKoreaLr11CrNum = 1; m_dKoreaLr11Cr1 = 1.25; m_dKoreaLr11Cr2 = 1.25;
			m_nKoreaLr11ShNum = 1; m_dKoreaLr11Sh1 = 1.25; m_dKoreaLr11Sh2 = 1.25;
		}
		else if(m_nLrfd11DC == 1) 
		{
			m_nKoreaLr11CrNum = 1; m_dKoreaLr11Cr1 = 0.90; m_dKoreaLr11Cr2 = 0.90;
			m_nKoreaLr11ShNum = 1; m_dKoreaLr11Sh1 = 0.90; m_dKoreaLr11Sh2 = 0.90;
		}
		else if(m_nLrfd11DC == 2) 
		{
			m_nKoreaLr11CrNum = 2; m_dKoreaLr11Cr1 = 1.25; m_dKoreaLr11Cr2 = 0.90;
			m_nKoreaLr11ShNum = 2; m_dKoreaLr11Sh1 = 1.25; m_dKoreaLr11Sh2 = 0.90;
		}
	}
	else if(nPSCRSHType == 1) // B : Concrete Superstructures - non-segmental
	{
		m_nKoreaLr11PsNum = 1; m_dKoreaLr11Ps1 = 1.0; m_dKoreaLr11Ps2 = 1.0;
		m_nKoreaLr11CrNum = 1; m_dKoreaLr11Cr1 = 1.0; m_dKoreaLr11Cr2 = 1.0;
		m_nKoreaLr11ShNum = 1; m_dKoreaLr11Sh1 = 1.0; m_dKoreaLr11Sh2 = 1.0;
	}
	else if(nPSCRSHType == 2) // C : SubStructures supporting non-segmental Superstructures (using Ig)
	{
		m_nKoreaLr11PsNum = 1; m_dKoreaLr11Ps1 = 0.5; m_dKoreaLr11Ps2 = 0.5;
		m_nKoreaLr11CrNum = 1; m_dKoreaLr11Cr1 = 0.5; m_dKoreaLr11Cr2 = 0.5;
		m_nKoreaLr11ShNum = 1; m_dKoreaLr11Sh1 = 0.5; m_dKoreaLr11Sh2 = 0.5;
	}
	else if(nPSCRSHType == 3) // D : SubStructures supporting non-segmental Superstructures (using Ieffective)
	{
		m_nKoreaLr11PsNum = 1; m_dKoreaLr11Ps1 = 1.0; m_dKoreaLr11Ps2 = 1.0;
		m_nKoreaLr11CrNum = 1; m_dKoreaLr11Cr1 = 1.0; m_dKoreaLr11Cr2 = 1.0;
		m_nKoreaLr11ShNum = 1; m_dKoreaLr11Sh1 = 1.0; m_dKoreaLr11Sh2 = 1.0;
	}
	else if(nPSCRSHType == 4) // E : Steel Substructures
	{
		m_nKoreaLr11PsNum = 1; m_dKoreaLr11Ps1 = 1.0; m_dKoreaLr11Ps2 = 1.0;
		m_nKoreaLr11CrNum = 1; m_dKoreaLr11Cr1 = 1.0; m_dKoreaLr11Cr2 = 1.0;
		m_nKoreaLr11ShNum = 1; m_dKoreaLr11Sh1 = 1.0; m_dKoreaLr11Sh2 = 1.0;
	}
}

void CCmdAutoLoadCombCvlKOREALRFD11Dlg::OnCmdKoreaLRFD11EnableDisable()
{  
	CArray <unsigned int, unsigned int> arMLoad;   // Moving Load
	CArray <unsigned int, unsigned int> arSmLoad;  // Settlement (not static)

	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();

	BOOL bDcExist = FALSE;
	BOOL bDdExist = FALSE;
	BOOL bDwExist = FALSE;
	BOOL bEhExist = FALSE;
	BOOL bEvExist = FALSE;
	BOOL bEsExist = FALSE;
	BOOL bLExist  = FALSE;
	BOOL bTExist  = FALSE;  
	BOOL bPSCRSHExist = FALSE;
	BOOL bStlExist= FALSE;
	BOOL bMovExist= FALSE;

	if(iCount!=0)
	{
		for(int i = 0; i < iCount; i++)
		{
			int iKey = arKeyList.GetAt(i);
			T_STLD_D rData; rData.Initialize();

			if(!m_pDoc->m_pAttrCtrl->GetStld(iKey, rData)) { ASSERT(0); continue; }

			if     (rData.LoadCaseType==_T("D"))	 { bDcExist = TRUE; }  // General Dead Load
			else if(rData.LoadCaseType==_T("DC"))	 { bDcExist = TRUE; }  // Component and Attachments
			else if(rData.LoadCaseType==_T("DD"))	 { bDdExist = TRUE; }  // Downdrag
			else if(rData.LoadCaseType==_T("DW"))	 { bDwExist = TRUE; }  // Wearing Surfaces and Utilities
			else if(rData.LoadCaseType==_T("EP"))	 { bEhExist = TRUE; }  // General earth pressure load
			else if(rData.LoadCaseType==_T("EH"))	 { bEhExist = TRUE; }  // horizontal earth pressure load
			else if(rData.LoadCaseType==_T("EV"))	 { bEvExist = TRUE; }  // vertical earth pressure load
			else if(rData.LoadCaseType==_T("ES"))	 { bEsExist = TRUE; }  // Earth Surcharge      
			else if(rData.LoadCaseType==_T("L"))	 { bLExist  = TRUE; }  // Live Load
			else if(rData.LoadCaseType==_T("T"))	 { bTExist  = TRUE; }	 // 온도변화, 크리프, 건조수축 하중
			else if(rData.LoadCaseType==_T("PS"))	 { bPSCRSHExist = TRUE; }
			else if(rData.LoadCaseType==_T("CR"))	 { bTExist  = TRUE; bPSCRSHExist = TRUE; } // 온도변화, 크리프, 건조수축 하중
			else if(rData.LoadCaseType==_T("SH"))	 { bTExist  = TRUE; bPSCRSHExist = TRUE; }  // 온도변화, 크리프, 건조수축 하중
			else if(rData.LoadCaseType==_T("STL")) { bStlExist= TRUE; }  // 부등침하 하중      
		}
		m_pDoc->m_pAttrCtrl->GetSmlcKeyList(arSmLoad);

		// 코드에 따른 방식으로 수정 
		T_MVCD_D DataMvcd;
		if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
		switch (DataMvcd.nCodeType)
		{
		case D_MOVE_CODE_NONE:
			break;
		case D_MOVE_CODE_AASHTO_STAN:
		case D_MOVE_CODE_AASHTO_LRFD:
		case D_MOVE_CODE_TAIWAN:
		case D_MOVE_CODE_KOREA:
		case D_MOVE_CODE_CANADA:
		case D_MOVE_CODE_PENDOT:
		case D_MOVE_CODE_EURO_BS:
		case D_MOVE_CODE_RUSSIA:
		case D_MOVE_CODE_KOREA_LRFD_2011:
		case D_MOVE_CODE_AUSTRALIA:
		case D_MOVE_CODE_POLAND:
		case D_MOVE_CODE_SOUTH_AFRICA:
		case D_MOVE_CODE_JAPAN_RAIL:
		case D_MOVE_CODE_NEWZEALAND:
		case D_MOVE_CODE_BRAZIL:
			m_pDoc->m_pAttrCtrl->GetMvldKeyList(arMLoad);
			break;
		case D_MOVE_CODE_CHINA:
			m_pDoc->m_pAttrCtrl->GetMvldchKeyList(arMLoad);
			break;
		case D_MOVE_CODE_JAPAN:
			m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(arMLoad);
			break;
		case D_MOVE_CODE_INDIA:
			m_pDoc->m_pAttrCtrl->GetMvldidKeyList(arMLoad);
			break;
		case D_MOVE_CODE_BS:
			m_pDoc->m_pAttrCtrl->GetMvldbsKeyList(arMLoad);
			break;
		case D_MOVE_CODE_FRANCE:
			m_pDoc->m_pAttrCtrl->GetMvldfrKeyList(arMLoad);
			break;
		case D_MOVE_CODE_TRANS:
			m_pDoc->m_pAttrCtrl->GetMvldtrKeyList(arMLoad);
			break;
		default:
			ASSERT(0);
			break;
		}
		if(arMLoad.GetSize() > 0)  { bLExist = TRUE; bMovExist = TRUE; }
		if(arSmLoad.GetSize() > 0) { bStlExist = TRUE; }
	}
	//////////////////////////////////////////////////////////////////
	UpdateData(TRUE);

	if(m_nConstLoad!=0)
	{
		CArray<T_SGLD_K,T_SGLD_K> aSgldK; aSgldK.RemoveAll();
		if(m_pDoc->IsPostMode()) m_pDoc->m_pPostCtrl->GetStageInfo()->GetSgldKeyListPost(aSgldK);
		else                     m_pDoc->m_pAttrCtrl->GetSgldKeyList(aSgldK);

		T_STCT_D StctD; StctD.Initialize();
		if(m_pDoc->m_pAttrCtrl->ExistStct()) m_pDoc->m_pAttrCtrl->GetStct(StctD);

		BOOL bCEL = FALSE;
		for(int iKey=0; iKey<aSgldK.GetSize(); iKey++)
		{
			T_SGLD_K SgldK = aSgldK.GetAt(iKey);
			if     (SgldK==D_SGLD_DL_CVL)    { bDcExist = TRUE; }
			else if(SgldK >= D_SGLD_ER1_CVL && SgldK <= D_SGLD_ER20_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER1_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER2_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER3_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER4_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER5_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER6_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER7_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER8_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER9_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER10_CVL) { bCEL = TRUE; }
			else if(SgldK==D_SGLD_CP_CVL)    { bTExist  = TRUE; bPSCRSHExist = TRUE; }
			else if(SgldK==D_SGLD_CS_CVL)    { bTExist  = TRUE; bPSCRSHExist = TRUE; }
			else if(SgldK==D_SGLD_SP_CVL)    { bTExist  = TRUE; bPSCRSHExist = TRUE; }
			else if(SgldK==D_SGLD_SS_CVL)    { bTExist  = TRUE; bPSCRSHExist = TRUE; }
		}
		if(bCEL)
		{
			for(int i=0; i<StctD.aErection.GetSize(); i++)
			{
				if     (StctD.aErection[i].LoadCaseType==_T("D"))	bDcExist = TRUE;  // General Dead Load
				else if(StctD.aErection[i].LoadCaseType==_T("DC"))	bDcExist = TRUE;  // Component and Attachments
				else if(StctD.aErection[i].LoadCaseType==_T("DW"))	bDwExist = TRUE;  // Wearing Surfaces and Utilities
				else if(StctD.aErection[i].LoadCaseType==_T("L"))	bLExist  = TRUE;  // Live Load
				else if(StctD.aErection[i].LoadCaseType==_T("T"))	bTExist  = TRUE;  // 온도변화, 크리프, 건조수축 하중
			}			
		}
	}
	//////////////////////////////////////////////////////////////////
	GetDlgItem(IDC_CMD_KOREA_LRFD11_DC_MAX_RDO )->EnableWindow(bDcExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_DC_MIN_RDO )->EnableWindow(bDcExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_DC_BOTH_RDO)->EnableWindow(bDcExist);

	GetDlgItem(IDC_CMD_KOREA_LRFD11_DD_MAX_RDO )->EnableWindow(bDdExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_DD_MIN_RDO )->EnableWindow(bDdExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_DD_BOTH_RDO)->EnableWindow(bDdExist);

	GetDlgItem(IDC_CMD_KOREA_LRFD11_DW_MAX_RDO )->EnableWindow(bDwExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_DW_MIN_RDO )->EnableWindow(bDwExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_DW_BOTH_RDO)->EnableWindow(bDwExist);

	GetDlgItem(IDC_CMD_KOREA_LRFD11_EH_RDO1)->EnableWindow(bEhExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EH_RDO2)->EnableWindow(bEhExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EH1_MAX_RDO )->EnableWindow(bEhExist && m_nLrfd11EHType==0);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EH1_MIN_RDO )->EnableWindow(bEhExist && m_nLrfd11EHType==0);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EH1_BOTH_RDO)->EnableWindow(bEhExist && m_nLrfd11EHType==0);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EH2_MAX_RDO )->EnableWindow(bEhExist && m_nLrfd11EHType==1);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EH2_MIN_RDO )->EnableWindow(bEhExist && m_nLrfd11EHType==1);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EH2_BOTH_RDO)->EnableWindow(bEhExist && m_nLrfd11EHType==1);

	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV_RDO1)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV_RDO2)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV_RDO3)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV_RDO4)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV_RDO5)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV_RDO6)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV2_MAX_RDO )->EnableWindow(bEvExist && m_nLrfd11EVType==1);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV2_MIN_RDO )->EnableWindow(bEvExist && m_nLrfd11EVType==1);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV2_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd11EVType==1);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV3_MAX_RDO )->EnableWindow(bEvExist && m_nLrfd11EVType==2);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV3_MIN_RDO )->EnableWindow(bEvExist && m_nLrfd11EVType==2);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV3_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd11EVType==2);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV4_MAX_RDO )->EnableWindow(bEvExist && m_nLrfd11EVType==3);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV4_MIN_RDO )->EnableWindow(bEvExist && m_nLrfd11EVType==3);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV4_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd11EVType==3);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV5_MAX_RDO )->EnableWindow(bEvExist && m_nLrfd11EVType==4);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV5_MIN_RDO )->EnableWindow(bEvExist && m_nLrfd11EVType==4);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV5_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd11EVType==4);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV6_MAX_RDO )->EnableWindow(bEvExist && m_nLrfd11EVType==5);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV6_MIN_RDO )->EnableWindow(bEvExist && m_nLrfd11EVType==5);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV6_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd11EVType==5);

	GetDlgItem(IDC_CMD_KOREA_LRFD11_ES_MAX_RDO)->EnableWindow(bEsExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_ES_MIN_RDO)->EnableWindow(bEsExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_ES_BOTH_RDO)->EnableWindow(bEsExist);

	GetDlgItem(IDC_CMD_KOREA_LRFD11_CULVERT_CHK)->EnableWindow(bLExist);

	GetDlgItem(IDC_CMD_KOREA_LRFD11_SETTLEMENT_EDT)->EnableWindow(bStlExist);

	GetDlgItem(IDC_CMD_KOREA_LRFD11_DEFORMATION_RDO1)->EnableWindow(bTExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_DEFORMATION_RDO2)->EnableWindow(bTExist);

	GetDlgItem(IDC_CMD_KOREA_LRFD11_PS_CR_SH_STC)->EnableWindow(bPSCRSHExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_PS_CR_SH_CMB)->EnableWindow(bPSCRSHExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_PS_CR_SH_BTN)->EnableWindow(bPSCRSHExist);

	GetDlgItem(IDC_CMD_KOREA_LRFD11_FATI_LL_CMB)->EnableWindow(bMovExist);

	GetDlgItem(IDC_CMD_KOREA_LRFD11_FATI_DD_CMB)->EnableWindow(bDcExist || bDdExist || bDwExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_FATI_DD_LST)->EnableWindow(bDcExist || bDdExist || bDwExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_FATI_DD_ADD_BTN)->EnableWindow(bDcExist || bDdExist || bDwExist);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_FATI_DD_DEL_BTN)->EnableWindow(bDcExist || bDdExist || bDwExist);

	GetDlgItem(IDC_CMD_KOREA_LRFD11_EFFLOAD_EDT)->EnableWindow(m_bLrfd11GravityLoadChk);
}

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlKOREALRFD11Dlg message handlers
BOOL CCmdAutoLoadCombCvlKOREALRFD11Dlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_chkLoadFactor.InitControl(this, CCmdAutoLoadCombCvlKOREALRFD11Dlg::IDD, IDC_CMD_KOREA_LRFD11_LOAD_FACTOR_GRP,TRUE,TRUE);

	m_edtLrfd11Modifier.SetEditUnit(1.0);
	m_edtLrfd11Settlement.SetEditUnit(1.0);
	m_edtLrfd11EffLoadFactor.SetEditUnit(0.5);

	InitPSCRSHCombo();
	InitLLCombo();
	InitDLCombo();
	InitDLListBox();

	UpdateData(FALSE);


	OnCmdKoreaLRFD11EHTypeRdo();
	OnCmdKoreaLRFD11EVTypeRdo();
	OnCmdKoreaLRFD11EnableDisable();   // Load Case의 존재 여부에 따라 Ctrl을 활성화 및 비활성화
	OnCmdKoreaLRFD11GRACHK();

	return TRUE;
}

void CCmdAutoLoadCombCvlKOREALRFD11Dlg::OnCmdKoreaLRFD11EHTypeRdo()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EH1_MAX_RDO )->EnableWindow(m_nLrfd11EHType==0);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EH1_MIN_RDO )->EnableWindow(m_nLrfd11EHType==0);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EH1_BOTH_RDO)->EnableWindow(m_nLrfd11EHType==0);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EH2_MAX_RDO )->EnableWindow(m_nLrfd11EHType==1);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EH2_MIN_RDO )->EnableWindow(m_nLrfd11EHType==1);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EH2_BOTH_RDO)->EnableWindow(m_nLrfd11EHType==1);
}

void CCmdAutoLoadCombCvlKOREALRFD11Dlg::OnCmdKoreaLRFD11EVTypeRdo() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV2_MAX_RDO )->EnableWindow(m_nLrfd11EVType==1);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV2_MIN_RDO )->EnableWindow(m_nLrfd11EVType==1);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV2_BOTH_RDO)->EnableWindow(m_nLrfd11EVType==1);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV3_MAX_RDO )->EnableWindow(m_nLrfd11EVType==2);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV3_MIN_RDO )->EnableWindow(m_nLrfd11EVType==2);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV3_BOTH_RDO)->EnableWindow(m_nLrfd11EVType==2);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV4_MAX_RDO )->EnableWindow(m_nLrfd11EVType==3);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV4_MIN_RDO )->EnableWindow(m_nLrfd11EVType==3);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV4_BOTH_RDO)->EnableWindow(m_nLrfd11EVType==3);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV5_MAX_RDO )->EnableWindow(m_nLrfd11EVType==4);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV5_MIN_RDO )->EnableWindow(m_nLrfd11EVType==4);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV5_BOTH_RDO)->EnableWindow(m_nLrfd11EVType==4);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV6_MAX_RDO )->EnableWindow(m_nLrfd11EVType==5);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV6_MIN_RDO )->EnableWindow(m_nLrfd11EVType==5);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EV6_BOTH_RDO)->EnableWindow(m_nLrfd11EVType==5);		
}

void CCmdAutoLoadCombCvlKOREALRFD11Dlg::OnCmdKoreaLRFD11PSCRSHBtn() 
{
	CCmdAutoLoadCombCvlHelpDlg dlg;
	dlg.DoModal();
}

void CCmdAutoLoadCombCvlKOREALRFD11Dlg::OnCmdKoreaLRFD11DLAddBtn() 
{
	UpdateData(TRUE);

	int Index = m_cmbDLCase.GetCurSel();
	if(Index != -1)
	{
		CString strName = _T("");
		m_cmbDLCase.GetLBText(Index, strName);

		if(CheckDLCaseName(strName))
		{      
			T_STLD_K StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strName);

			//if(StldK != 0) m_DLData.SetAt(StldK, 0);      
			if(StldK != 0) m_aDLData.Add(StldK);
			int nSize = m_lstDL.GetItemCount();

			//  ListCtrl Box에 값을 입력합니다.	
			LV_ITEM lvitem;
			lvitem.mask		  = LVIF_TEXT;
			lvitem.iItem  	= nSize;
			lvitem.iSubItem = 0;
			lvitem.pszText	= (LPTSTR)(LPCTSTR)strName;
			m_lstDL.InsertItem(&lvitem);	
		}
		else
		{
			CString msg;      
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), strName);
			AfxMessageBox(msg);
			return;
		}
	}
	else AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));  	
}

void CCmdAutoLoadCombCvlKOREALRFD11Dlg::OnCmdKoreaLRFD11DLDelBtn() 
{
	int nItem = m_lstDL.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
		return;
	}

	//   CString strName=_T("");
	//   strName = m_lstDL.GetItemText(nItem, 0);
	// 
	//   T_STLD_K StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strName);
	//   
	//   if (StldK == 0) return;
	//   m_DLData.RemoveKey(StldK);
	m_aDLData.RemoveAt(nItem);
	m_lstDL.DeleteItem(nItem);
	int nCount = m_lstDL.GetItemCount();
	if (nCount <= nItem) nItem -= 1;
	if (nItem >= 0) 
		m_lstDL.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

LRESULT CCmdAutoLoadCombCvlKOREALRFD11Dlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	m_pParent->AlignControl();

	return 0L;
}

void CCmdAutoLoadCombCvlKOREALRFD11Dlg::OnCmdKoreaLRFD11GRACHK() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EFFLOAD_EDT )->EnableWindow(m_bLrfd11GravityLoadChk);
	GetDlgItem(IDC_CMD_KOREA_LRFD11_EFFLOAD_STC )->EnableWindow(m_bLrfd11GravityLoadChk);  
}