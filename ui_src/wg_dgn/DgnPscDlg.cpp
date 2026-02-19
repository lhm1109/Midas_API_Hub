// DgnPscDlg.cpp: implementation of the CDgnPscDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnPscDlg.h"

#include "CRCDgnOption.h"
#include "CRCPrtOption.h"
#include "DgnPscCrackWidthDlg.h"
#include "DgnPscSegmentDlg.h"
#include "ExposureClassDlg.h"
#include "ShearConnectorDlg.h"
#include "InterfaceShearDlg.h"
#include "InterfaceShearDlg_CSA.h"
#include "DgnConCondiFactorDlg.h"
#include "FatigueCheckDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_DBDoc.h"    // For GetDocPoint().
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CDgnPscDlg, MChildFormView)

CDgnPscDlg::CDgnPscDlg()
 : MChildFormView(CDgnPscDlg::IDD)
{
	CreateChildDlgs();
}

CDgnPscDlg::~CDgnPscDlg()
{
	DeleteChildDlgs();
}

void CDgnPscDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnPscDlg)
	DDX_Control(pDX, IDC_DGN_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_DGN_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_DGN_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnPscDlg, MChildFormView)
	//{{AFX_MSG_MAP(CDgnPscDlg)
	ON_CBN_SELCHANGE(IDC_DGN_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_DGN_TABLE_BTN, OnDgnTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnPscDlg diagnostics

#ifdef _DEBUG
void CDgnPscDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CDgnPscDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDgnPscDlg message handlers


void CDgnPscDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 

	m_ChildDlgs.RemoveAll();
	m_DlgIDs.RemoveAll();
	m_TableIDs.RemoveAll();

#if defined(_CIVIL)	
	// Change by ZINU.('04.12.28). 임시수정 by 신미영DR, 개발항목 1103.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	T_PSCD_D PscdD; PscdD.Initialize();
	pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

    switch ( PscdD.iDgnCode )
    {
    case KSCE_USD03:
    case KSCE_USD05:
    case KSCE_USD10:
    case KSCE_RAIL_USD04:
    case KSCE_RAIL_USD11:
        CreateChildDlgsKSCE();
        break;
    case JSCE02:
        CreateChildDlgsJSCE();
        break;
    case AASHTO_LRFD08:
    case AASHTO_LRFD12:
    case AASHTO_LRFD14:
    case AASHTO_LRFD16:
    case AASHTO_LRFD17:
    case AASHTO_LRFD20:
    case AASHTO_LRFD24:
    case AREMA_23:
        CreateChildDlgsAASHTO();
        break;
    case CSA_S6_10:
    case CSA_S6_14:
	case CSA_S6_19:
        CreateChildDlgsCSA();
        break;
    case EUROCODE2_2_05_PSC:
        CreateChildDlgsEURO();
        break;
    case ABNT_NBR_6118_2023_PSC:
        CreateChildDlgsBRA();
        break;
    case IRC_112_2011_PSC:
    case IRC_112_2020_PSC:
        CreateChildDlgsIRC();
        break;
    case IRS_PSC:
        CreateChildDlgsIRS();
        break;
    case AS_5100_5_17_PSC:
        CreateChildDlgsAS();
        break;
    case KDS_24_14_21_2022_PSC:
    case KSCE_LSD15_PSC:
        CreateChildDlgsKSCELSD15();
        break;
	case TMH07_3_1989:
    case BS5400_90_PSC:
    case BD_44_15_PSC:
    case CS_455_PSC:
        CreateChildDlgsBS();
        break;
    case SNiP_20503_84_PSC:
    case SP_35_13330_11_PSC:
    case SNiP_20503_84_PSC_MKS:
    case SP_35_13330_11_PSC_MKS:
        CreateChildDlgsRUS();
        break;
    case JTG_D62_04:
    case CJJ_11_2011:
    case TB_10002_3_05:
        CreateChildDlgsElse();
        break;
    default:
        ASSERT(0);
        break;
    }
#endif
}

void CDgnPscDlg::CreateChildDlgsKSCE()
{
    m_ChildDlgs.Add(new CCRCDgnOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSC__);
    m_TableIDs.Add(ID_QUERY_PSC_DESIGN_OPTION);
    m_ChildDlgs.Add(new CCRCPrtOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSX__);
    m_TableIDs.Add(ID_QUERY_PSC_PRINT_OPTION);
}

void CDgnPscDlg::CreateChildDlgsJSCE()
{
    m_ChildDlgs.Add(new CCRCDgnOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSC__);
    m_TableIDs.Add(ID_QUERY_PSC_DESIGN_OPTION);
}

void CDgnPscDlg::CreateChildDlgsElse()
{
    m_ChildDlgs.Add(new CCRCDgnOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSC__);
    m_TableIDs.Add(ID_QUERY_PSC_DESIGN_OPTION);
    //open prtOption by maxiao
    m_ChildDlgs.Add(new CCRCPrtOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSX__);
    m_TableIDs.Add(ID_QUERY_PSC_PRINT_OPTION);
    m_ChildDlgs.Add(new CDgnPscCrackWidthDlg);
    m_DlgIDs.Add(__TMMODE_DGN_PSC_CRACK_PSCW__);
    m_TableIDs.Add(ID_QUERY_PSC_CRACK_WIDTH);
}

void CDgnPscDlg::CreateChildDlgsAASHTO()
{
    m_ChildDlgs.Add(new CCRCDgnOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSC__);
    m_TableIDs.Add(ID_QUERY_PSC_DESIGN_OPTION);
    m_ChildDlgs.Add(new CCRCPrtOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSX__);
    m_TableIDs.Add(ID_QUERY_PSC_PRINT_OPTION);
    m_ChildDlgs.Add(new CDgnPscSegmentDlg); // Add, Jaeoh. [6/1/2009] 
    m_DlgIDs.Add(__TMMODE_DGN_PSC_SEGMENT__);
    m_TableIDs.Add(0);
    m_ChildDlgs.Add(new CInterfaceShearDlg);            //Add, PSC Composite AASHTO
    m_DlgIDs.Add(__TMMODE_DGN_PSC_INTERFACE_SHEAR__);
    m_TableIDs.Add(ID_QUERY_PSC_INTERFACE_SHEAR);
}

void CDgnPscDlg::CreateChildDlgsCSA()
{
    m_ChildDlgs.Add(new CCRCDgnOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSC__);
    m_TableIDs.Add(ID_QUERY_PSC_DESIGN_OPTION);
    m_ChildDlgs.Add(new CCRCPrtOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSX__);
    m_TableIDs.Add(ID_QUERY_PSC_PRINT_OPTION);
    m_ChildDlgs.Add(new CDgnPscSegmentDlg); // Add, Jaeoh. [6/1/2009] 
    m_DlgIDs.Add(__TMMODE_DGN_PSC_SEGMENT__);
    m_TableIDs.Add(0);
    m_ChildDlgs.Add(new CInterfaceShearDlg_CSA);            //Add, PSC Composite CSA
    m_DlgIDs.Add(__TMMODE_DGN_PSC_INTERFACE_SHEAR__);
    m_TableIDs.Add(ID_QUERY_PSC_INTERFACE_SHEAR);
}

void CDgnPscDlg::CreateChildDlgsEURO()
{
    m_ChildDlgs.Add(new CCRCDgnOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSC__);
    m_TableIDs.Add(ID_QUERY_PSC_DESIGN_OPTION);

    m_ChildDlgs.Add(new CCRCPrtOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSX__);
    m_TableIDs.Add(ID_QUERY_PSC_PRINT_OPTION);

    m_ChildDlgs.Add(new CExposureClassDlg(this, 0));
    m_DlgIDs.Add(__TMMODE_DGN_PSC_EXPOSURE_CLASS__);
    m_TableIDs.Add(ID_QUERY_PSC_EXPOSURE_CLASS);

    m_ChildDlgs.Add(new CShearConnectorDlg);
    m_DlgIDs.Add(__TMMODE_DGN_PSC_SHEAR_CONNECTOR__);
    m_TableIDs.Add(ID_QUERY_PSC_SHEAR_CONNECTOR);
}

void CDgnPscDlg::CreateChildDlgsBRA()
{
    m_ChildDlgs.Add(new CCRCDgnOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSC__);
    m_TableIDs.Add(ID_QUERY_PSC_DESIGN_OPTION);

    m_ChildDlgs.Add(new CCRCPrtOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSX__);
    m_TableIDs.Add(ID_QUERY_PSC_PRINT_OPTION);

    m_ChildDlgs.Add(new CExposureClassDlg(this, 0));
    m_DlgIDs.Add(__TMMODE_DGN_PSC_EXPOSURE_CLASS__);
    m_TableIDs.Add(ID_QUERY_PSC_EXPOSURE_CLASS);

    m_ChildDlgs.Add(new CShearConnectorDlg);
    m_DlgIDs.Add(__TMMODE_DGN_PSC_SHEAR_CONNECTOR__);
    m_TableIDs.Add(ID_QUERY_PSC_SHEAR_CONNECTOR);
}

void CDgnPscDlg::CreateChildDlgsIRC()
{
    m_ChildDlgs.Add(new CCRCDgnOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSC__);
    m_TableIDs.Add(ID_QUERY_PSC_DESIGN_OPTION);

    m_ChildDlgs.Add(new CCRCPrtOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSX__);
    m_TableIDs.Add(ID_QUERY_PSC_PRINT_OPTION);

    m_ChildDlgs.Add(new CExposureClassDlg(this, 0));
    m_DlgIDs.Add(__TMMODE_DGN_PSC_EXPOSURE_CLASS__);
    m_TableIDs.Add(ID_QUERY_PSC_EXPOSURE_CLASS);

    m_ChildDlgs.Add(new CShearConnectorDlg);
    m_DlgIDs.Add(__TMMODE_DGN_PSC_SHEAR_CONNECTOR__);
    m_TableIDs.Add(ID_QUERY_PSC_SHEAR_CONNECTOR);
}

void CDgnPscDlg::CreateChildDlgsIRS()
{
    m_ChildDlgs.Add(new CCRCDgnOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSC__);
    m_TableIDs.Add(ID_QUERY_PSC_DESIGN_OPTION);

    m_ChildDlgs.Add(new CCRCPrtOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSX__);
    m_TableIDs.Add(ID_QUERY_PSC_PRINT_OPTION);

    m_ChildDlgs.Add(new CExposureClassDlg(this, 0));
    m_DlgIDs.Add(__TMMODE_DGN_PSC_EXPOSURE_CLASS__);
    m_TableIDs.Add(ID_QUERY_PSC_EXPOSURE_CLASS);

    m_ChildDlgs.Add(new CShearConnectorDlg);
    m_DlgIDs.Add(__TMMODE_DGN_PSC_SHEAR_CONNECTOR__);
    m_TableIDs.Add(ID_QUERY_PSC_SHEAR_CONNECTOR);

    m_ChildDlgs.Add(new CDgnPscSegmentDlg); // Add, Jaeoh. [6/1/2009] 
    m_DlgIDs.Add(__TMMODE_DGN_PSC_SEGMENT__);
    m_TableIDs.Add(ID_PSC_SEGMENT);
}

void CDgnPscDlg::CreateChildDlgsAS()
{
    m_ChildDlgs.Add(new CCRCDgnOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSC__);
    m_TableIDs.Add(ID_QUERY_PSC_DESIGN_OPTION);

    m_ChildDlgs.Add(new CCRCPrtOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSX__);
    m_TableIDs.Add(ID_QUERY_PSC_PRINT_OPTION);

    m_ChildDlgs.Add(new CExposureClassDlg(this, 0));
    m_DlgIDs.Add(__TMMODE_DGN_PSC_EXPOSURE_CLASS__);
    m_TableIDs.Add(ID_QUERY_PSC_EXPOSURE_CLASS);
}

void CDgnPscDlg::CreateChildDlgsKSCELSD15()
{
    m_ChildDlgs.Add(new CCRCDgnOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSC__);
    m_TableIDs.Add(ID_QUERY_PSC_DESIGN_OPTION);

    m_ChildDlgs.Add(new CCRCPrtOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSX__);
    m_TableIDs.Add(ID_QUERY_PSC_PRINT_OPTION);

    m_ChildDlgs.Add(new CExposureClassDlg(this, 0));
    m_DlgIDs.Add(__TMMODE_DGN_PSC_EXPOSURE_CLASS__);
    m_TableIDs.Add(ID_QUERY_PSC_EXPOSURE_CLASS);

    m_ChildDlgs.Add(new CShearConnectorDlg);
    m_DlgIDs.Add(__TMMODE_DGN_PSC_SHEAR_CONNECTOR__);
    m_TableIDs.Add(ID_QUERY_PSC_SHEAR_CONNECTOR);
}

void CDgnPscDlg::CreateChildDlgsBS()
{
    m_ChildDlgs.Add(new CCRCDgnOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSC__);
    m_TableIDs.Add(ID_QUERY_PSC_DESIGN_OPTION);

    m_ChildDlgs.Add(new CCRCPrtOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSX__);
    m_TableIDs.Add(ID_QUERY_PSC_PRINT_OPTION);

    m_ChildDlgs.Add(new CExposureClassDlg(this, 0));
    m_DlgIDs.Add(__TMMODE_DGN_PSC_EXPOSURE_CLASS__);
    m_TableIDs.Add(ID_QUERY_PSC_EXPOSURE_CLASS);

    m_ChildDlgs.Add(new CShearConnectorDlg);
    m_DlgIDs.Add(__TMMODE_DGN_PSC_SHEAR_CONNECTOR__);
    m_TableIDs.Add(ID_QUERY_PSC_SHEAR_CONNECTOR);

    m_ChildDlgs.Add(new CDgnPscSegmentDlg);
    m_DlgIDs.Add(__TMMODE_DGN_PSC_SEGMENT__);
    m_TableIDs.Add(0);
}

void CDgnPscDlg::CreateChildDlgsRUS()
{
    m_ChildDlgs.Add(new CCRCDgnOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSC__);
    m_TableIDs.Add(ID_QUERY_PSC_DESIGN_OPTION);
    m_ChildDlgs.Add(new CCRCPrtOption);
    m_DlgIDs.Add(__TMMODE_DGN_CRC_DGN_POSX__);
    m_TableIDs.Add(ID_QUERY_PSC_PRINT_OPTION);
    m_ChildDlgs.Add(new CExposureClassDlg(this, 0));
    m_DlgIDs.Add(__TMMODE_DGN_PSC_EXPOSURE_CLASS__);
    m_TableIDs.Add(ID_QUERY_PSC_EXPOSURE_CLASS);
    m_ChildDlgs.Add(new CDgnConCondiFactorDlg);
    m_DlgIDs.Add(__TMMODE_DGN_PSC_OCDF__);
    m_TableIDs.Add(ID_QUERY_DGN_PSC_OCDF);
}

void CDgnPscDlg::DeleteChildDlgs()
{
	for(int i=0; i<m_ChildDlgs.GetSize(); i++)	delete m_ChildDlgs[i];
}

// TreeMenu Mode에 해당하는 Child Dialog를 활성화 시킨다. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CDgnPscDlg::ShowChildDlgByID(int nID)
{
	if(nID==__TMMODE_DGN_PSC__)
	{
		ShowCurChildDlg(0);  
		m_ComboMenu.SetCurSel(0);
		return TRUE;
	}

	int nIDCount = m_DlgIDs.GetSize();
	for(int i=0; i<nIDCount; i++)
	{
		if(nID==m_DlgIDs[i])
		{
			ShowCurChildDlg(i);  
			m_ComboMenu.SetCurSel(i);
			if(m_TableIDs[i] > 0)	m_wndTableBtn.EnableWindow(TRUE);
			else									m_wndTableBtn.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CDgnPscDlg::ShowCurChildDlg(int nDlgIndex)
{
	if(m_nCurChildDlg==nDlgIndex)	return;

	if(nDlgIndex > m_ChildDlgs.GetSize()-1)
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}

	if(m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize()-1 >= m_nCurChildDlg)
		m_ChildDlgs[m_nCurChildDlg]->End();

	m_ChildDlgs[nDlgIndex]->CreateInit(&m_wndPlaceHolder,0,0);
	m_nCurChildDlg = nDlgIndex;

	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect,ParentRect,PHRect;
	
	GetClientRect(&ParentRect);
	m_ChildDlgs[nDlgIndex]->GetWindowRect(&DlgRect);
	ScreenToClient(&DlgRect);

	int Sx,Sy,Wx,Wy;
	Sx = (ParentRect.Width() - DlgRect.Width()) / 2 ;
	Sy = DlgRect.top;

	Wx = DlgRect.Width();
	Wy = DlgRect.Height();

	m_wndPlaceHolder.SetWindowPos(NULL, Sx,Sy,Wx,Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndPlaceHolder.GetWindowRect(&PHRect);

	ScreenToClient(&PHRect);

	/////////////////////////////////////////////////////////////
	// Scroll Size를 조정한다. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();

	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();

	ScrSize += TSize;

	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);
}

int CDgnPscDlg::GetHeightFromFont()
{
	 CFont * pNewFont  = GetFont();
	 CFont *pSysFont,*pOldFont,TempFont;
	 CDC   *pCdc;
	 TEXTMETRIC  tmNew,tmSys;
	 int     nTemp,nCorrectVal;
 
	 //get the DC for the edit control
	 pCdc = GetDC();
 
	 //get the metrics for the system font
	 pSysFont = TempFont.FromHandle(((HFONT)GetStockObject(SYSTEM_FONT)));
	 pOldFont  = pCdc->SelectObject( pSysFont);
	 pCdc->GetTextMetrics(&tmSys);
 
	 //get the metrics for the new font
	 pCdc->SelectObject(pNewFont);
	 pCdc->GetTextMetrics(&tmNew);
 
	 //select the original font back into the DC and release the DC
	 pCdc->SelectObject(pOldFont);
	 ReleaseDC(pCdc);
	 
	 nCorrectVal = (min(tmNew.tmHeight, tmSys.tmHeight)/2);
	 //calculate the new height for the edit control
	 nTemp = tmNew.tmHeight + nCorrectVal;
	 //nTemp = tmNew.tmHeight + tmNew.tmInternalLeading;
	 return nTemp;
}

void CDgnPscDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	Update_ComboMenu();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CDgnPscDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if(bActivate)
	{
		if(m_nCurChildDlg != -1)	
		{
			ShowCurChildDlg(m_nCurChildDlg);
			CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[m_nCurChildDlg]);
		}
		else
		{
			ShowCurChildDlg(0);
			CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[0]);
		}
	}
}

void CDgnPscDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg를 직접 부르면 CBarTreeMenu의 m_nCurMode가 Update
	// 되지 않아 Menu에서 Select해도 해당 Dialog가 뜨지 않는 경우가 발생
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);
}

int CDgnPscDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CDgnPscDlg::OnDgnTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));	
}

// Change by ZINU.('01.8.1). To change Dynamic Combo Menu.
void CDgnPscDlg::Update_ComboMenu()
{
#if defined(_CIVIL) 
	m_ComboMenuStr.RemoveAll();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	T_PSCD_D PscdD; PscdD.Initialize();
	pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
    BOOL bPscComposite = pDoc->m_pPostCtrl->IsPSCCompositeSect();

    switch ( PscdD.iDgnCode )
    {
    case KSCE_USD03:
    case KSCE_USD05:
    case KSCE_USD10:
    case KSCE_RAIL_USD04:
    case KSCE_RAIL_USD11:
        Update_ComboMenuKSCE();
        break;
    case JSCE02:
        Update_ComboMenuJSCE();
        break;
    case AASHTO_LRFD08:
    case AASHTO_LRFD12:
    case AASHTO_LRFD14:
    case AASHTO_LRFD16:
    case AASHTO_LRFD17:
    case AASHTO_LRFD20:
    case AASHTO_LRFD24:
    case CSA_S6_10:
    case CSA_S6_14:
	case CSA_S6_19:
    case AREMA_23:
        Update_ComboMenuAASHTO(bPscComposite);
        break;
    case EUROCODE2_2_05_PSC:
        Update_ComboMenuEURO();
        break;
    case ABNT_NBR_6118_2023_PSC:
        Update_ComboMenuEURO();
        break;
    case IRC_112_2011_PSC:
    case IRC_112_2020_PSC:
    case IRS_PSC:
        Update_ComboMenuIND();
        break;
    case AS_5100_5_17_PSC:
        Update_ComboMenuAS();
        break;
    case KDS_24_14_21_2022_PSC:
    case KSCE_LSD15_PSC:
        Update_ComboMenuKSCELSD15(bPscComposite);
        break;
	case TMH07_3_1989:
    case BS5400_90_PSC:
    case BD_44_15_PSC:
    case CS_455_PSC:
        Update_ComboMenuBS();
        break;
    case SNiP_20503_84_PSC:
    case SP_35_13330_11_PSC:
    case SNiP_20503_84_PSC_MKS:
    case SP_35_13330_11_PSC_MKS:
        Update_ComboMenuRUS();
        break;
    case JTG_D62_04:
    case CJJ_11_2011:
    case TB_10002_3_05:
        Update_ComboMenuElse();
        break;
    default:
        ASSERT(0);
        break;
    }

	int iSelID = m_ComboMenu.GetCurSel();
	// Initialize Data.
	m_ComboMenu.ResetContent();
	int wHeight = (m_ComboMenuStr.GetSize() + 1) * GetHeightFromFont();

	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL,0,0,rectWnd.Width(),wHeight,SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	// Add String at ComboBox.
	for(int i=0; i<m_ComboMenuStr.GetSize(); i++)
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}
	// Recalculate Width.
	CClientDC dc(this);
	int nSize = m_ComboMenuStr.GetSize();
	CSize sizeMax, sizeCur;
	sizeMax = dc.GetTextExtent(m_ComboMenuStr[0]);
	for(int k=1; k<nSize; k++)
	{
		sizeCur = dc.GetTextExtent(m_ComboMenuStr[k]);
		if(sizeCur.cx > sizeMax.cx)	sizeMax.cx = sizeCur.cx;
	}
	if(sizeMax.cx > m_ComboMenu.GetDroppedWidth())	m_ComboMenu.SetDroppedWidth(sizeMax.cx);

	if(iSelID < 0)	m_ComboMenu.SetCurSel(0);
	else						m_ComboMenu.SetCurSel(iSelID);
 
#endif
}

void CDgnPscDlg::Update_ComboMenuKSCE()
{
    m_ComboMenuStr.SetSize(2);
    m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_DESIGN_OPTION);
    m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_PRINT_OPTION);
}

void CDgnPscDlg::Update_ComboMenuJSCE()
{
    m_ComboMenuStr.SetSize(1);
    m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_DESIGN_OPTION);
}

void CDgnPscDlg::Update_ComboMenuAASHTO( BOOL bPscComposite )
{
    if ( bPscComposite )
    {
        m_ComboMenuStr.SetSize(4);
        m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_DESIGN_OPTION);
        m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_PRINT_OPTION);
        m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_SEGMENT);
        m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_INTERFACE_SHEAR);
    }
    else
    {
        m_ComboMenuStr.SetSize(3);
        m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_DESIGN_OPTION);
        m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_PRINT_OPTION);
        m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_SEGMENT);
    }
}

void CDgnPscDlg::Update_ComboMenuEURO()
{
    m_ComboMenuStr.SetSize(4);
    m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_DESIGN_OPTION);
    m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_PRINT_OPTION);
    m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_EXPOSURE_CLASS);
    m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_SHEAR_CONNECTOR);
}

void CDgnPscDlg::Update_ComboMenuIND()
{
    m_ComboMenuStr.SetSize(5);
    m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_DESIGN_OPTION);
    m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_PRINT_OPTION);
    m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_EXPOSURE_CLASS);
    m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_SHEAR_CONNECTOR);
    m_ComboMenuStr[4] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_SEGMENT);
}

void CDgnPscDlg::Update_ComboMenuAS()
{
    m_ComboMenuStr.SetSize(3);
    m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_DESIGN_OPTION);
    m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_PRINT_OPTION);
    m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_EXPOSURE_CLASS);
}

void CDgnPscDlg::Update_ComboMenuKSCELSD15( BOOL bPscComposite )
{
    if ( bPscComposite )
    {
        m_ComboMenuStr.SetSize(4);
        m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_DESIGN_OPTION);
        m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_PRINT_OPTION);
        m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_EXPOSURE_CLASS);
        m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_SHEAR_CONNECTOR);
    }
    else
    {
        m_ComboMenuStr.SetSize(3);
        m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_DESIGN_OPTION);
        m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_PRINT_OPTION);
        m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_EXPOSURE_CLASS);
    }
}

void CDgnPscDlg::Update_ComboMenuBS()
{
    m_ComboMenuStr.SetSize(5);
    m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_DESIGN_OPTION);
    m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_PRINT_OPTION);
    m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_EXPOSURE_CLASS);
    m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_SHEAR_CONNECTOR);
    m_ComboMenuStr[4] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_SEGMENT);
}

void CDgnPscDlg::Update_ComboMenuRUS()
{
    m_ComboMenuStr.SetSize(4);
    m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_DESIGN_OPTION);
    m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_PRINT_OPTION);
    m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_EXPOSURE_CLASS);
    m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_DGN_PSC_OCDF);
}

void CDgnPscDlg::Update_ComboMenuElse()
{
    // Modify by GAY. MNET:2444. ('06.09.08). Make List not including _T("PSC Print Option").
    m_ComboMenuStr.SetSize(3);
    m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_DESIGN_OPTION);
    m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_PRINT_OPTION);//open by maxiao
    m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitlePsc(ID_PSC_CRACK_WIDTH);
}

void CDgnPscDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			{
				//DeleteChildDlgs();
				//CreateChildDlgs();
				// Add by ZINU.('01.1.2).
				Update_InitDataByCode(TRUE);
				break;
			}
		case D_UPDATE_UNIT:
			// do something...
			break;
		default:
			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
			ASSERT(TRUE);
	}
}

void CDgnPscDlg::Update_InitDataByCode(BOOL bUpdate)
{	
	if(bUpdate)
	{
		CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
		int nCount = pViewBuff->GetCount();
		if(nCount==0) return; 
		ASSERT(nCount==1);

		BOOL bCODE=FALSE;
		T_UDRD_BUFFER buffer_ur;
		POSITION pos = pViewBuff->GetStartBuffer();
		while(pos)
		{
			buffer_ur = pViewBuff->GetNextBuffer(pos);
			int nCmd = buffer_ur.nCmd;
			int nKey = buffer_ur.nKey;
			
			switch(nCmd)
			{
				case(UR_PSCD_ADD):
				case(UR_PSCD_DEL):	{bCODE = TRUE; break;}
				default:	break;
			}
		}
		if(!bCODE) return;
	}

	DeleteChildDlgs();
	CreateChildDlgs();

	Update_ComboMenu();
	if(m_ComboMenu.GetCount()>0)
	{
		ShowCurChildDlg(0);  
		m_ComboMenu.SetCurSel(0);
	}

}

void CDgnPscDlg::OnTmExecute()
{
	if ( m_nCurChildDlg < m_ChildDlgs.GetSize() )
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CDgnPscDlg::OnTmClose()
{
	if ( CDBDoc::GetDocPoint() )
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}