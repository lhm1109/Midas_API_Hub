// CmdAutoLoadCombCvlSP35133302011Page.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlAS51002017Page.h"
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


enum { RoadWay = 0, PedestrianCyclist = 1};

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlAS51002017Page dialog
CCmdAutoLoadCombCvlAS51002017Page::CCmdAutoLoadCombCvlAS51002017Page(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCmdAutoLoadCombCvlAS51002017Page::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadCombCvlAS51002017Page)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_pParent = (CCmdAutoLoadCombCvlDlg*)pParent;

	m_nConstLoad = 0;

	m_nAS5100D = 0;
	m_nAS5100DW = 0;
	m_nAS5100SL = 0;
	m_nAS5100GL = 0;


}

void CCmdAutoLoadCombCvlAS51002017Page::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadCombCvlAS51002017Page)  
	DDX_Control(pDX, IDC_CMD_AS_5100_2017_BRIDGE_TYPE_CMB,			m_cmbBridgeType);
	DDX_Radio  (pDX, IDC_CMD_AS_5100_2017_D_MAX_RDO,						m_nAS5100D);
	DDX_Control(pDX, IDC_CMD_AS_5100_2017_D_MAX_EDT,						m_edtAS5100D_Max);
	DDX_Control(pDX, IDC_CMD_AS_5100_2017_D_MIN_EDT,						m_edtAS5100D_Min);
	DDX_Radio  (pDX, IDC_CMD_AS_5100_2017_DW_MAX_RDO,						m_nAS5100DW);
	DDX_Control(pDX, IDC_CMD_AS_5100_2017_DW_MAX_EDT,						m_edtAS5100DW_Max);
	DDX_Control(pDX, IDC_CMD_AS_5100_2017_DW_MIN_EDT,						m_edtAS5100DW_Min);
	DDX_Radio  (pDX, IDC_CMD_AS_5100_2017_EP_MAX_RDO,						m_nAS5100SL);
	DDX_Control(pDX, IDC_CMD_AS_5100_2017_EP_MAX_EDT,						m_edtAS5100SL_Max);
	DDX_Control(pDX, IDC_CMD_AS_5100_2017_EP_MIN_EDT,						m_edtAS5100SL_Min);
	DDX_Radio  (pDX, IDC_CMD_AS_5100_2017_WP_MAX_RDO,						m_nAS5100GL);
	DDX_Control(pDX, IDC_CMD_AS_5100_2017_WP_MAX_EDT,						m_edtAS5100GL_Max);
	DDX_Control(pDX, IDC_CMD_AS_5100_2017_WP_MIN_EDT,						m_edtAS5100GL_Min);
 
	//2018-07-09 Fatigue
	DDX_Control(pDX, IDC_CMD_AS_5100_2017_FATI_MVL_CMB,					m_MoveLcase);
	DDX_Control(pDX, IDC_CMD_AS_5100_2017_FATI_MVL_LST,					m_MovingList);
	//2018-07-09 Fatigue

	DDX_Control(pDX, IDC_CMD_AS_5100_2017_LOAD_FACTOR_GRP,			m_chkLoadFactor);


	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlAS51002017Page, CChildDialog)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombCvlAS51002017Page)
	//}}AFX_MSG_MAP
	//2018-07-09 Fatigue
	ON_CBN_SELCHANGE(IDC_CMD_AS_5100_2017_BRIDGE_TYPE_CMB, OnCmdAS5100Fatigue)
	ON_BN_CLICKED(IDC_CMD_AS_5100_2017_FATI_MVL_ADD_BTN,   OnCmdAS5100RTAddBtn)
	ON_BN_CLICKED(IDC_CMD_AS_5100_2017_FATI_MVL_DEL_BTN,   OnCmdAS5100RTDelBtn)
	//2018-07-09 Fatigue

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()


void CCmdAutoLoadCombCvlAS51002017Page::InitCombo()
{
	m_cmbBridgeType.ResetContent();
	m_cmbBridgeType.AddString(_LS(IDS_CMD_LOAD_COMB_BRIDGE_TYPE_ROAD));
	m_cmbBridgeType.AddString(_LS(IDS_CMD_LOAD_COMB_BRIDGE_TYPE_PEDESTRIAN_CYCLIST));
	m_cmbBridgeType.SetCurSel(0);

}

//2018-07-09 Fatigue
void CCmdAutoLoadCombCvlAS51002017Page::InitRTCombo()
{
	m_MoveLcase.ResetContent();
	//m_MoveLcase.AddString(_LS(IDS_WG_CMD__ADDD__None));
	
	CArray<T_MVLD_K, T_MVLD_K> KeyList; /*KeyList.RemoveAll();*/
	CArray<T_MVLDjp_K, T_MVLDjp_K> KeyListjp; /*KeyListjp.RemoveAll();*/
	CArray<T_MVLDch_K, T_MVLDch_K> KeyListch; /*KeyListch.RemoveAll();*/
	CArray<T_MVLDid_K, T_MVLDid_K> KeyListid; /*KeyListid.RemoveAll();*/
	CArray<T_MVLDbs_K, T_MVLDbs_K> KeyListbs; /*KeyListbs.RemoveAll();*/
	CArray<T_MVLDfr_K, T_MVLDfr_K> KeyListfr; /*KeyListfr.RemoveAll();*/
	CArray<T_MVLDtr_K, T_MVLDtr_K> KeyListtr; /*KeyListtr.RemoveAll();*/

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

	if(nCount > 0)
	{
		for(int i=0; i<nCount; i++)
		{      
		T_MVLD_D	 rData;
		T_MVLDjp_D rDatajp;        
		T_MVLDch_D rDatach;
		T_MVLDid_D rDataid;
		T_MVLDbs_D rDatabs;
		T_MVLDfr_D rDatafr;
		T_MVLDtr_D rDatatr;


		rData.Initialize();
		m_pDoc->m_pAttrCtrl->InitializeMvldjp(rDatajp);
		rDatach.Initialize();

		int key;
		BOOL bCheck;

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
			key = KeyList.GetAt(i);   bCheck = m_pDoc->m_pAttrCtrl->GetMvld(key,rData);
			break;
		case D_MOVE_CODE_CHINA:
			key = KeyListch.GetAt(i); bCheck = m_pDoc->m_pAttrCtrl->GetMvldch(key,rDatach);
			break;
		case D_MOVE_CODE_JAPAN:
			key = KeyListjp.GetAt(i); bCheck = m_pDoc->m_pAttrCtrl->GetMvldjp(key,rDatajp);
			break;
		case D_MOVE_CODE_INDIA:
			key = KeyListid.GetAt(i);   bCheck = m_pDoc->m_pAttrCtrl->GetMvldid(key,rDataid);
			break;
		case D_MOVE_CODE_BS:
			key = KeyListbs.GetAt(i);   bCheck = m_pDoc->m_pAttrCtrl->GetMvldbs(key,rDatabs);
			break;
		case D_MOVE_CODE_FRANCE:
			key = KeyListfr.GetAt(i);   bCheck = m_pDoc->m_pAttrCtrl->GetMvldfr(key,rDatafr);
			break;
		case D_MOVE_CODE_TRANS:
			key = KeyListtr.GetAt(i);   bCheck = m_pDoc->m_pAttrCtrl->GetMvldtr(key, rDatatr);
			break;
		default:
			ASSERT(0);
			break;
		}

		ASSERT(bCheck);

		CString str;
		// 코드에 따른 방식으로 수정 
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
			str = rData.LoadCaseName;
			break;
		case D_MOVE_CODE_CHINA:
			str = rDatach.LoadCaseName;
			break;
		case D_MOVE_CODE_JAPAN:
			str = rDatajp.LoadCaseName;
			break;
		case D_MOVE_CODE_INDIA:
			str = rDataid.LoadCaseName;
			break;
		case D_MOVE_CODE_BS:
			str = rDatabs.LoadCaseName;
			break;
		case D_MOVE_CODE_FRANCE:
			str = rDatafr.LoadCaseName;
			break;
		case D_MOVE_CODE_TRANS:
			str = rDatatr.LoadCaseName;
			break;
		default:
			ASSERT(0);
			break;
		}
		m_MoveLcase.AddString(str);
}
Show_MoveRTComboBox(TRUE);
}
else
{
	Show_MoveRTComboBox(FALSE);
}
m_MoveLcase.SetCurSel(0);
}
//2018-07-09 Fatigue


//2018-07-09 Fatigue
void CCmdAutoLoadCombCvlAS51002017Page::Show_MoveRTComboBox(BOOL bCheck)
{
	m_MoveLcase.EnableWindow(bCheck);
	m_MovingList.EnableWindow(bCheck);
	//GetDlgItem(IDC_CMD_AS_5100_2017_FATI_MVL_ADD_BTN)->EnableWindow(bCheck);
	//GetDlgItem(IDC_CMD_AS_5100_2017_FATI_MVL_DEL_BTN)->EnableWindow(bCheck);  
}
//2018-07-09 Fatigue

void CCmdAutoLoadCombCvlAS51002017Page::Initial_RTListBox()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_MovingList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_MovingList.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	CString aTitle[] = { _LS(IDS_WG_CMD__ADDD__Load_Case) };

	CString title;
	int width[1] = {100};
	for(int i = 0; i < sizeof(aTitle) / sizeof(CString); i++)
	{
		title = aTitle[i];

		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_LEFT;
		lvcolumn.pszText  = title.GetBuffer(0);
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_MovingList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCmdAutoLoadCombCvlAS51002017Page::SetDefault()
{
	// Dead Load
	double dDMax = 1.20, dDMin = 0.85;
	m_edtAS5100D_Max.SetEditUnit(dDMax, 2);
	m_edtAS5100D_Min.SetEditUnit(dDMin, 2);

	// Superimposed Dead Load
	double dDWMax = 2.0, dDWMin = 0.8;
	m_edtAS5100DW_Max.SetEditUnit(dDWMax, 2);
	m_edtAS5100DW_Min.SetEditUnit(dDWMin, 2);

	// Soil and Groundwater Load - Soil Load
	double dSLMax = 1.5, dSLMin = 0.7;
	m_edtAS5100SL_Max.SetEditUnit(dSLMax, 2);
	m_edtAS5100SL_Min.SetEditUnit(dSLMin, 2);

	// Soil and Groundwater Load - Groundwater Load
	double dGLMax = 1.0, dGLMin = 1.0;
	m_edtAS5100GL_Max.SetEditUnit(dGLMax, 2);
	m_edtAS5100GL_Min.SetEditUnit(dGLMin, 2);

}

void CCmdAutoLoadCombCvlAS51002017Page::GetAS5100LoadFactor()
{
	UpdateData(TRUE);
	
	m_nASBridgeType = m_cmbBridgeType.GetCurSel();

	// Dead Load
	double dDMax = m_edtAS5100D_Max.GetEditValue();
	double dDMin = m_edtAS5100D_Min.GetEditValue();

	switch(m_nAS5100D)
	{
		case 0: m_nAS5100DNum = 1; m_dAS5100DMax = dDMax; m_dAS5100DMin = dDMax; break; 
		case 1: m_nAS5100DNum = 1; m_dAS5100DMax = dDMin; m_dAS5100DMin = dDMin; break; 
		case 2: m_nAS5100DNum = 2; m_dAS5100DMax = dDMax; m_dAS5100DMin = dDMin; break;
	}

	
	// Superimposed Dead Load
	double dDWMax = m_edtAS5100DW_Max.GetEditValue();
	double dDWMin = m_edtAS5100DW_Min.GetEditValue();

	switch(m_nAS5100DW)
	{
		case 0: m_nAS5100DWNum = 1; m_dAS5100DWMax = dDWMax; m_dAS5100DWMin = dDWMax; break; 
		case 1: m_nAS5100DWNum = 1; m_dAS5100DWMax = dDWMin; m_dAS5100DWMin = dDWMin; break; 
		case 2: m_nAS5100DWNum = 2; m_dAS5100DWMax = dDWMax; m_dAS5100DWMin = dDWMin; break;
	}
	
	// Soil Load
	double dSLMax = m_edtAS5100SL_Max.GetEditValue();
	double dSLMin = m_edtAS5100SL_Min.GetEditValue();
	
	switch(m_nAS5100SL)
	{
	case 0: m_nAS5100SLNum = 1; m_dAS5100SLMax = dSLMax; m_dAS5100SLMin = dSLMax; break; 
	case 1: m_nAS5100SLNum = 1; m_dAS5100SLMax = dSLMin; m_dAS5100SLMin = dSLMin; break; 
	case 2: m_nAS5100SLNum = 2; m_dAS5100SLMax = dSLMax; m_dAS5100SLMin = dSLMin; break;
	}

	// Groundwater Load
	double dGLMax = m_edtAS5100GL_Max.GetEditValue();
	double dGLMin = m_edtAS5100GL_Min.GetEditValue();

	switch(m_nAS5100GL)
	{
	case 0: m_nAS5100GLNum = 1; m_dAS5100GLMax = dGLMax; m_dAS5100GLMin = dGLMax; break; 
	case 1: m_nAS5100GLNum = 1; m_dAS5100GLMax = dGLMin; m_dAS5100GLMin = dGLMin; break; 
	case 2: m_nAS5100GLNum = 2; m_dAS5100GLMax = dGLMax; m_dAS5100GLMin = dGLMin; break;
	}

}

void CCmdAutoLoadCombCvlAS51002017Page::OnCmdAS5100EnableDisable()
{  
	CArray <unsigned int, unsigned int> arMLoad;   // Moving Load
	//CArray <unsigned int, unsigned int> arSmLoad;  // Settlement (not static)

	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();

	BOOL bDExist  = FALSE;
	BOOL bDWExist  = FALSE;
	BOOL bSLExist = FALSE;
	BOOL bGLExist = FALSE;

	BOOL bLExist  = FALSE;
	BOOL bStlExist= FALSE;

	BOOL bMovExist= FALSE;

	// 항상 활성화 하도록 수정 
	bDExist  = TRUE;
	bDWExist = TRUE;
	bSLExist = TRUE;
	bGLExist = TRUE;


	bMovExist= TRUE;

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


	GetDlgItem(IDC_CMD_AS_5100_2017_D_MAX_RDO )->EnableWindow(bDExist);
	GetDlgItem(IDC_CMD_AS_5100_2017_D_MAX_EDT )->EnableWindow(bDExist);
	GetDlgItem(IDC_CMD_AS_5100_2017_D_MIN_RDO )->EnableWindow(bDExist);
	GetDlgItem(IDC_CMD_AS_5100_2017_D_MIN_EDT )->EnableWindow(bDExist);
	GetDlgItem(IDC_CMD_AS_5100_2017_D_BOTH_RDO)->EnableWindow(bDExist);

	GetDlgItem(IDC_CMD_AS_5100_2017_DW_MAX_RDO )->EnableWindow(bDWExist);
	GetDlgItem(IDC_CMD_AS_5100_2017_DW_MAX_EDT )->EnableWindow(bDWExist);
	GetDlgItem(IDC_CMD_AS_5100_2017_DW_MIN_RDO )->EnableWindow(bDWExist);
	GetDlgItem(IDC_CMD_AS_5100_2017_DW_MIN_EDT )->EnableWindow(bDWExist);
	GetDlgItem(IDC_CMD_AS_5100_2017_DW_BOTH_RDO)->EnableWindow(bDWExist);

	GetDlgItem(IDC_CMD_AS_5100_2017_EP_MAX_RDO )->EnableWindow(bSLExist); //
	GetDlgItem(IDC_CMD_AS_5100_2017_EP_MAX_EDT )->EnableWindow(bSLExist);
	GetDlgItem(IDC_CMD_AS_5100_2017_EP_MIN_RDO )->EnableWindow(bSLExist); // 
	GetDlgItem(IDC_CMD_AS_5100_2017_EP_MIN_EDT )->EnableWindow(bSLExist);
	GetDlgItem(IDC_CMD_AS_5100_2017_EP_BOTH_RDO)->EnableWindow(bSLExist); // 

	GetDlgItem(IDC_CMD_AS_5100_2017_WP_MAX_RDO )->EnableWindow(bGLExist); // 
	GetDlgItem(IDC_CMD_AS_5100_2017_WP_MAX_EDT )->EnableWindow(bGLExist);
	GetDlgItem(IDC_CMD_AS_5100_2017_WP_MIN_RDO )->EnableWindow(bGLExist); // 
	GetDlgItem(IDC_CMD_AS_5100_2017_WP_MIN_EDT )->EnableWindow(bGLExist);
	GetDlgItem(IDC_CMD_AS_5100_2017_WP_BOTH_RDO)->EnableWindow(bGLExist); //

	//2018-07-09 Fatigue
	GetDlgItem(IDC_CMD_AS_5100_2017_FATI_MVL_CMB		)->EnableWindow(bMovExist && m_nASBridgeType == RoadWay);
	GetDlgItem(IDC_CMD_AS_5100_2017_FATI_MVL_ADD_BTN)->EnableWindow(bMovExist && m_nASBridgeType == RoadWay);
	GetDlgItem(IDC_CMD_AS_5100_2017_FATI_MVL_DEL_BTN)->EnableWindow(bMovExist && m_nASBridgeType == RoadWay);
	GetDlgItem(IDC_CMD_AS_5100_2017_FATI_MVL_LST		)->EnableWindow(bMovExist && m_nASBridgeType == RoadWay);
	GetDlgItem(IDC_CMD_AS_5100_2017_LOAD_FACTOR_GRP	)->EnableWindow(bMovExist && m_nASBridgeType == RoadWay);
	//2018-07-09 Fatigue

}

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlAS51002017Page message handlers
BOOL CCmdAutoLoadCombCvlAS51002017Page::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_chkLoadFactor.InitControl(this, CCmdAutoLoadCombCvlAS51002017Page::IDD, IDC_CMD_AS_5100_2017_LOAD_FACTOR_GRP,TRUE,TRUE);

	InitCombo();
	SetDefault();
	//2018-07-09 Fatigue
	InitRTCombo();
	Initial_RTListBox();
		//2018-07-09 Fatigue

	UpdateData(FALSE);
	GetAS5100LoadFactor();

	//2018-07-09 Fatigue
	OnCmdAS5100Fatigue();
	//2018-07-09 Fatigue

	OnCmdAS5100EnableDisable();   // Load Case의 존재 여부에 따라 Ctrl을 활성화 및 비활성화
 
	return TRUE;
}

BOOL CCmdAutoLoadCombCvlAS51002017Page::CheckMovingCaseName(CString str,int& Index)
{
	BOOL bCheck = TRUE;
	int nCount = m_MovingList.GetItemCount();
	for(int i=0; i<nCount; i++)
	{
		CString strLcName = m_MovingList.GetItemText(i, 0);
		if(str == strLcName)  
		{
			Index = i;
			bCheck = FALSE;
		}
	}
	return bCheck;
}

//2018-07-09 Fatigue
void CCmdAutoLoadCombCvlAS51002017Page::OnCmdAS5100RTAddBtn()
{
	UpdateData(TRUE);

	int nTrailer = 1;  // 평판과차 여대차
	//  Moving Load Case Data의 순번을 얻어옵니다.
	int nKey = 0;
	int ListNo;
	int Index = m_MoveLcase.GetCurSel();
	if(Index != -1)
	{
		CString strName=_T("");
		m_MoveLcase.GetLBText(Index,strName);

		if(CheckMovingCaseName(strName, ListNo))  //  TRUE=Add
		{      
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
				nKey = m_pDoc->m_pAttrCtrl->GetMvldKey(strName);
				break;
			case D_MOVE_CODE_CHINA:
				nKey = m_pDoc->m_pAttrCtrl->GetMvldchKey(strName);
				break;
			case D_MOVE_CODE_JAPAN:
				nKey = m_pDoc->m_pAttrCtrl->GetMvldjpKey(strName);
				break;
			case D_MOVE_CODE_INDIA:
				nKey = m_pDoc->m_pAttrCtrl->GetMvldidKey(strName);
				break;
			case D_MOVE_CODE_BS:
				nKey = m_pDoc->m_pAttrCtrl->GetMvldbsKey(strName);
				break;
			case D_MOVE_CODE_FRANCE:
				nKey = m_pDoc->m_pAttrCtrl->GetMvldfrKey(strName);
				break;
			case D_MOVE_CODE_TRANS:
				nKey = m_pDoc->m_pAttrCtrl->GetMvldtrKey(strName);
				break;
			default:
				ASSERT(0);
				break;
			}
			if(nKey != 0) m_pParent->m_MovingData.SetAt(nKey, nTrailer); 
		 
			int No = m_MovingList.GetItemCount();
			//Write_RTMoveListBox(No, strName);
			LV_ITEM lvitem;
			lvitem.mask		  = LVIF_TEXT;
			lvitem.iItem  	= No;
			lvitem.iSubItem = 0;
			lvitem.pszText	= (LPTSTR)(LPCTSTR)strName;
			m_MovingList.InsertItem(&lvitem);	
		}
		else                        //  FALSE=Modify or Cancel
		{
			CString msg;      
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), strName);
			AfxMessageBox(msg);
			return;
		}
	}
	else AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));

}

void CCmdAutoLoadCombCvlAS51002017Page::OnCmdAS5100RTDelBtn()
{
	int nItem = m_MovingList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
		return;
	}
	CString strName=_T("");
	strName = m_MovingList.GetItemText(nItem, 0);
	
	int nKey = 0;
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
		nKey = m_pDoc->m_pAttrCtrl->GetMvldKey(strName);
		break;
	case D_MOVE_CODE_CHINA:
		nKey = m_pDoc->m_pAttrCtrl->GetMvldchKey(strName);
		break;
	case D_MOVE_CODE_JAPAN:
		nKey = m_pDoc->m_pAttrCtrl->GetMvldjpKey(strName);
		break;
	case D_MOVE_CODE_INDIA:
		nKey = m_pDoc->m_pAttrCtrl->GetMvldidKey(strName);
		break;
	case D_MOVE_CODE_BS:
		nKey = m_pDoc->m_pAttrCtrl->GetMvldbsKey(strName);
		break;
	case D_MOVE_CODE_FRANCE:
		nKey = m_pDoc->m_pAttrCtrl->GetMvldfrKey(strName);
		break;
	case D_MOVE_CODE_TRANS:
		nKey = m_pDoc->m_pAttrCtrl->GetMvldtrKey(strName);
		break;
	default:
		ASSERT(0);
		break;
	}
 
	if (nKey == 0) return;
	m_pParent->m_MovingData.RemoveKey(nKey);
	m_MovingList.DeleteItem(nItem);
	int nCount = m_MovingList.GetItemCount();
	if (nCount <= nItem) nItem -= 1;
	if (nItem >= 0) 
		m_MovingList.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}
//2018-07-09 Fatigue


LRESULT CCmdAutoLoadCombCvlAS51002017Page::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	m_pParent->AlignControl();

	return 0L;
}


//2018-07-09 Fatigue
void CCmdAutoLoadCombCvlAS51002017Page::OnCmdAS5100Fatigue()
{
	UpdateData(TRUE);
	//Pedestrian 이나 Cyclist에 대한 피로 한계 상태는 호주 기준에서 고려하지 않음
	m_nASBridgeType = m_cmbBridgeType.GetCurSel();
	GetDlgItem(IDC_CMD_AS_5100_2017_FATI_MVL_ADD_BTN)->EnableWindow(m_nASBridgeType==RoadWay);
	GetDlgItem(IDC_CMD_AS_5100_2017_FATI_MVL_DEL_BTN)->EnableWindow(m_nASBridgeType==RoadWay);
	GetDlgItem(IDC_CMD_AS_5100_2017_FATI_MVL_CMB)->EnableWindow(m_nASBridgeType==RoadWay);
	GetDlgItem(IDC_CMD_AS_5100_2017_FATI_MVL_LST)->EnableWindow(m_nASBridgeType==RoadWay);
	GetDlgItem(IDC_CMD_AS_5100_2017_LOAD_FACTOR_GRP)->EnableWindow(m_nASBridgeType==RoadWay);
	int nCount = m_MovingList.GetItemCount();
	if (m_nASBridgeType == PedestrianCyclist)
	{
		COLORREF crTextColor = ::GetSysColor(COLOR_GRAYTEXT);
		m_MovingList.SetTextColor(crTextColor);
		m_MovingList.SetItemState(-1, 0, LVIS_SELECTED);
	}
	else
	{
		COLORREF crTextColor = ::GetSysColor(COLOR_WINDOWTEXT);
		m_MovingList.SetTextColor(crTextColor);
	}
}
//2018-07-09 Fatigue
