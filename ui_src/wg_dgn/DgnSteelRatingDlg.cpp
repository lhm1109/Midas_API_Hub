// DgnSteelRatingDlg.cpp: implementation of the CDgnSteelRatingDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGFatigueParamDlg.h"  
#include "CPGLateraTorsionDlg.h"
#include "DgnSteelRatingDlg.h"
#include "DgnSteelRatingOptionDlg.h"
#include "DgnStlRatingPrintDlg.h" 
#include "DgnStlRatingWebPanelDlg.h" 
#include "DgnStlRatingShearConnDlg.h"
#include "DgnStlRatingServiceParamDlg.h"
#include "DgnStlRatingEffectLengUframeDlg.h"
#include "DgnStlRatingRivetsDlg.h"
#include "DgnStlRatingAssignMemberTypeDlg.h"
#include "DgnStlRatingMomentDistFactSlenderDlg.h"
#include "DgnStlRatingAssessmentCheckDlg.h"
#include "DgnStlRatingAssessmentReportDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_DBDoc.h"    // For GetDocPoint().
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\wg_base_MsgDll.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CDgnSteelRatingDlg, MChildFormView)

CDgnSteelRatingDlg::CDgnSteelRatingDlg()
 : MChildFormView(CDgnSteelRatingDlg::IDD)
{
	CreateChildDlgs();
}

CDgnSteelRatingDlg::~CDgnSteelRatingDlg()
{
	DeleteChildDlgs();
}

void CDgnSteelRatingDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSteelRatingDlg)
	DDX_Control(pDX, IDC_DGN_RATING_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_DGN_RATING_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_DGN_RATING_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnSteelRatingDlg, MChildFormView)
	//{{AFX_MSG_MAP(CDgnSteelRatingDlg)
	ON_CBN_SELCHANGE(IDC_DGN_RATING_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_DGN_RATING_TABLE_BTN, OnDgnTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSteelRatingDlg diagnostics

#ifdef _DEBUG
void CDgnSteelRatingDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CDgnSteelRatingDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDgnSteelRatingDlg message handlers


void CDgnSteelRatingDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 

	m_ChildDlgs.RemoveAll();
	m_DlgIDs.RemoveAll();
	m_TableIDs.RemoveAll();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_RCDS_D RcdsD; RcdsD.Initialize();
	pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD);

	
#if defined(_CIVIL)	
	if(RcdsD.nRatingCode == STL_AASHTO_LRFD12)
	{
		m_ChildDlgs.Add(new CCPGFatigueParamDlg); 
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_FATIGUE_PARAM__);
		m_TableIDs.Add(ID_QUERY_STEEL_RATING_FATIGUE_PARAM_LRFR11);
		//m_TableIDs.Add(ID_QUERY_STEEL_RATING_FATIGUE_PARAM);
	}
	else if(RcdsD.nRatingCode == STL_AASHTO_LRFD19)
	{
		m_ChildDlgs.Add(new CCPGFatigueParamDlg); 
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_FATIGUE_PARAM__);
		m_TableIDs.Add(ID_QUERY_STEEL_RATING_FATIGUE_PARAM);
	}
	else if (RcdsD.nRatingCode == STL_CS454_20)
	{
		m_ChildDlgs.Add(new CCPGLateraTorsionDlg(NULL, 1));
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_TORSION__);
		m_TableIDs.Add(ID_QUERY_STEEL_RATING_DESIGN_TORSION);

		m_ChildDlgs.Add(new CDgnStlRatingWebPanelDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_WEB_PANEL__);
		m_TableIDs.Add(0);

		m_ChildDlgs.Add(new CDgnStlRatingShearConnDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_SHEAR_CONN__);
		m_TableIDs.Add(ID_QUERY_STEEL_RATING_SHEAR_CONN);
		
		m_ChildDlgs.Add(new CDgnStlRatingServiceParamDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_SERVICE_PARAM__);
		m_TableIDs.Add(ID_QUERY_STEEL_RATING_SERVICE_PARAM);

		m_ChildDlgs.Add(new CDgnStlRatingAssessmentCheckDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_ASSESS_CHECK__);
		m_TableIDs.Add(ID_QUERY_STEEL_RATING_ASSESS_CHECK);

		m_ChildDlgs.Add(new CDgnStlRatingAssessmentReportDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_ASSESS_REPORT__);
		m_TableIDs.Add(ID_QUERY_STEEL_RATING_ASSESS_REPORT);
	}
	else if (RcdsD.nRatingCode == STL_NR_GN_CIV_025_06)
	{
		m_ChildDlgs.Add(new CDgnStlRatingWebPanelDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_WEB_PANEL__);
		m_TableIDs.Add(0);

		m_ChildDlgs.Add(new CDgnStlRatingEffectLengUframeDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_EFFECT_LENGTH_U_FRAME__);
		m_TableIDs.Add(ID_QUERY_STEEL_RATING_EFFECT_LENGTH_U_FRAME);

		m_ChildDlgs.Add(new CDgnStlRatingMomentDistFactSlenderDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_MOMENT_DIST_FACT_SLENDER__);
		m_TableIDs.Add(ID_QUERY_STEEL_RATING_MOMENT_DIST_FACT_SLENDER);

		m_ChildDlgs.Add(new CDgnStlRatingRivetsDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_FASTENER_RIVET__);
		m_TableIDs.Add(ID_QUERY_STEEL_RATING_FASTENER_RIVET);

		m_ChildDlgs.Add(new CDgnStlRatingAssignMemberTypeDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_ASSIGN_MEMB_FACTOR__);
		m_TableIDs.Add(ID_QUERY_STEEL_RATING_ASSIGN_MEMB_FACTOR);

		m_ChildDlgs.Add(new CDgnStlRatingAssessmentCheckDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_ASSESS_CHECK__);
		m_TableIDs.Add(ID_QUERY_STEEL_RATING_ASSESS_CHECK);

		m_ChildDlgs.Add(new CDgnStlRatingAssessmentReportDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_ASSESS_REPORT__);
		m_TableIDs.Add(ID_QUERY_STEEL_RATING_ASSESS_REPORT);
	}

	m_ChildDlgs.Add(new CDgnSteelRatingOptionDlg);
	m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_OPTION__);
	m_TableIDs.Add(ID_QUERY_STEEL_RATING_DESIGN_OPTION);	
	
	m_ChildDlgs.Add(new CDgnStlRationPrintDlg);
	m_DlgIDs.Add(__TMMODE_DGN_STEEL_RATING_PRINT__);
	m_TableIDs.Add(ID_QUERY_STL_RATING_PRINT_OPTION);		
#endif
}

void CDgnSteelRatingDlg::DeleteChildDlgs()
{
	for(int i=0; i<m_ChildDlgs.GetSize(); i++)	delete m_ChildDlgs[i];
}

// TreeMenu Mode에 해당하는 Child Dialog를 활성화 시킨다. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CDgnSteelRatingDlg::ShowChildDlgByID(int nID)
{
	if(nID==__TMMODE_DGN_STEEL_RATING__)
	{
		ShowCurChildDlg(0);  
		m_ComboMenu.SetCurSel(0);
		return TRUE;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_RCDS_D RcdsD; RcdsD.Initialize();
	pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD);

	int iDgnCode = RcdsD.nRatingCode;

	int nIDCount = m_DlgIDs.GetSize();
	for(int i=0; i<nIDCount; i++)
	{
		if(nID==m_DlgIDs[i])
		{
			ShowCurChildDlg(i);  

			switch (nID)
			{
			case __TMMODE_DGN_STEEL_RATING_FATIGUE_PARAM__:		
				{
					CCPGFatigueParamDlg *pFatigueSubDlg = NULL;
					pFatigueSubDlg = dynamic_cast<CCPGFatigueParamDlg*>(m_ChildDlgs[0]);
					if(pFatigueSubDlg != NULL) 
					{
						pFatigueSubDlg->SetDgnCode(iDgnCode);
						pFatigueSubDlg->SubDlgCtrlEnableDisable();
					}
					break;
				}
			case __TMMODE_DGN_STEEL_RATING_SERVICE_PARAM__:
			{
				CDgnStlRatingServiceParamDlg *pServiceParamDlg = NULL;
				pServiceParamDlg = dynamic_cast<CDgnStlRatingServiceParamDlg*>(m_ChildDlgs[0]);
				if (pServiceParamDlg != NULL)
				{
					pServiceParamDlg->SetDgnCode(iDgnCode);
				}
				break;
			}
			default:
				// 다른 것들은 원래대로..
				break;
			}

			m_ComboMenu.SetCurSel(i);
			if(m_TableIDs[i] > 0)	
				SetTableBtnEnable();
			else									
				m_wndTableBtn.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CDgnSteelRatingDlg::ShowCurChildDlg(int nDlgIndex)
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

void CDgnSteelRatingDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	DeleteChildDlgs();
	CreateChildDlgs();

	Update_ComboMenu();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CDgnSteelRatingDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if(bActivate)
	{
		SetTableBtnEnable();

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

void CDgnSteelRatingDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CDgnSteelRatingDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM);

	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnSteelRatingDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg를 직접 부르면 CBarTreeMenu의 m_nCurMode가 Update
	// 되지 않아 Menu에서 Select해도 해당 Dialog가 뜨지 않는 경우가 발생
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);
}

int CDgnSteelRatingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CDgnSteelRatingDlg::OnDgnTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));	
}

// Change by ZINU.('01.8.1). To change Dynamic Combo Menu.
void CDgnSteelRatingDlg::Update_ComboMenu()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
#if defined(_CIVIL) 
	m_ComboMenu.ResetContent();
	m_ComboMenuStr.RemoveAll();

	T_RCDS_D RcdsD; RcdsD.Initialize();	
	pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD);

	//m_ComboMenuStr.SetSize(2);

	if(RcdsD.nRatingCode==STL_AASHTO_LRFD12 || RcdsD.nRatingCode==STL_AASHTO_LRFD19 )
	{
		m_ComboMenuStr.SetSize(3);
		m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_CPG_FATIGUE_PARAM);	
		m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_DIAGNOSTIC_TEST_RESULTS);	// Diagnostic Test Results
		m_ComboMenuStr[2] = _LS(IDS_DGN_TREE_TEXT_POSITION_RATING_OUTPUT);	// Position for Rating Output
	}
	else if (RcdsD.nRatingCode == STL_CS454_20)
	{
		m_ComboMenuStr.SetSize(6);
		m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_STEEL_LATERAL_SPACING);	// Lateral Spacing of Beams
		m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_STEEL_WEB_PANEL_ASSIGNMENT);	// Web Panel Assignment
		m_ComboMenuStr[2] = _LS(IDS_DGN_TREE_TEXT_STEEL_SHEAR_CONNECTOR);	// Shear Connector
		m_ComboMenuStr[3] = _LS(IDS_DGN_TREE_TEXT_STEEL_SERVICE_PARAM);	// Serviceability Parameter
		m_ComboMenuStr[4] = _LS(IDS_DGN_TREE_TEXT_STEEL_ASSESS_CHECK);	// Assessment Check
		m_ComboMenuStr[5] = _LS(IDS_DGN_TREE_TEXT_STEEL_ASSESS_REPORT);	// Assessment Report
	}
	else if (RcdsD.nRatingCode == STL_NR_GN_CIV_025_06)
	{
		m_ComboMenuStr.SetSize(7);
		m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_STEEL_WEB_PANEL_ASSIGNMENT);	    // Web Panel Assignment
		m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_STEEL_EFFECT_LENGTH_U_FRAME);	    // Effective Length of U-frame
		m_ComboMenuStr[2] = _LS(IDS_DGN_TREE_TEXT_STEEL_MOMENT_DIST_FACT_SLENDER);	// Moment Distribution Factor for Slenderness
		m_ComboMenuStr[3] = _LS(IDS_DGN_TREE_TEXT_STEEL_FASTENER_RIVET);	        // Fastener-Rivets
		m_ComboMenuStr[4] = _LS(IDS_DGN_TREE_TEXT_STEEL_ASSIGN_MEMB_FACTOR);	    // Assign Member Type
		m_ComboMenuStr[5] = _LS(IDS_DGN_TREE_TEXT_STEEL_ASSESS_CHECK);          	// Assessment Check
		m_ComboMenuStr[6] = _LS(IDS_DGN_TREE_TEXT_STEEL_ASSESS_REPORT);	            // Assessment Report
	}
	else
	{
		m_ComboMenuStr.SetSize(2);
		m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_STEEL_RATING_OPTION);	// Rating Option
		m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_STEEL_PRINT_OPTION);	// Print Option
	}

	for (int i = 0; i < m_ComboMenuStr.GetSize(); i++)
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}

	int iSelID = m_ComboMenu.GetCurSel();
	if (iSelID < 0)	m_ComboMenu.SetCurSel(0);
	else			m_ComboMenu.SetCurSel(iSelID);
#endif
}

void CDgnSteelRatingDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnSteelRatingDlg::Update_InitDataByCode(BOOL bUpdate)
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
				case(UR_RCDS_ADD)://XXXXX
				case(UR_RCDS_DEL):	{bCODE = TRUE; break;}
				default:	break;
			}
		}
		if(!bCODE) return;
	}

	DeleteChildDlgs();
	CreateChildDlgs();

	Update_ComboMenu();

	SetTableBtnEnable();
}

void CDgnSteelRatingDlg::SetTableBtnEnable() 
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_RCDS_D Data;
	Data.Initialize();	
	pDoc->m_pAttrCtrl2->GetDgnRcds(Data);

    switch ( Data.nRatingCode )
    {
    case STL_KSCE_ASD05:
    case STL_KSCE_RAIL_ASD04:
    case STL_AASHTO_LRFD12:
    case STL_AASHTO_LRFD19:
    case STL_KSCE_ASD10:
    case STL_KSCE_RAIL_ASD11:
    case STL_KSCE_LSD15:
	case STL_CS454_20:
	case STL_NR_GN_CIV_025_06:
        m_wndTableBtn.EnableWindow(TRUE);
        break;
    default:
        m_wndTableBtn.EnableWindow(FALSE);
        break;
    }
}