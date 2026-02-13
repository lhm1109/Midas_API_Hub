// DgnCpgDlg.cpp: implementation of the CDgnCpgDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCpgDlg.h"

#include "DgnGenLengDlg.h"
#include "DgnStlRatingWebPanelDlg.h" 

#include "CPGTranStifDlg.h"
#include "CPGTranShearDlg.h"
#include "CPGTranPosiDlg.h"
#include "CPGPosiDgnOutDlg.h"
#include "CPGLoadApplicationDlg.h"
#include "CPGLateraTorsionDlg.h"
#include "CPGDamageEquivalenceDlg.h"
#include "CPGShearConnDlg.h"
#include "CPGResidualStressDlg.h"
#include "CPGFatigueParamDlg.h"
#include "CPGUnbraLengDlg.h"
#include "CPGCurveBridDlg.h"
#include "CPGDeckOverhangDlg.h"
#include "CPGShearConnGroupDlg.h"
#include "CPGTranShear_IRC_Dlg.h"
#include "CPGLateraTorsion_IRC_Dlg.h"
#include "CPGDamageEquivalence_IRC_Dlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_DBDoc.h"    // For GetDocPoint().
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DBDoc.h"

#include "..\wg_base\product.h"

#include "DgnConServiceLimtDlg.h"
#include "RCDesignOption.h"
#include "RCPrintOption.h"
#include "RCCrackWidth.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\wg_db_ViewCtrl.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CDgnCpgDlg, MChildFormView)

CDgnCpgDlg::CDgnCpgDlg()
	: MChildFormView(CDgnCpgDlg::IDD)
{
	CreateChildDlgs();
}

CDgnCpgDlg::~CDgnCpgDlg()
{
	DeleteChildDlgs();
}

void CDgnCpgDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCpgDlg)
	DDX_Control(pDX, IDC_DGN_CPG_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_DGN_CPG_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_DGN_CPG_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnCpgDlg, MChildFormView)
	//{{AFX_MSG_MAP(CDgnCpgDlg)
	ON_CBN_SELCHANGE(IDC_DGN_CPG_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_DGN_CPG_TABLE_BTN, OnDgnTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCpgDlg diagnostics

#ifdef _DEBUG
void CDgnCpgDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CDgnCpgDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDgnCpgDlg message handlers


void CDgnCpgDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1;

	m_ChildDlgs.RemoveAll();
	m_DlgIDs.RemoveAll();
	m_TableIDs.RemoveAll();

	T_CPGD_D CpgdD; CpgdD.Initialize();
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);
#if defined(_CIVIL)	


	// 기준마다 새롭게 세팅하자..
	if (CpgdD.iDgnCode == AS5100_6_17_CSG) 
	{
		m_ChildDlgs.Add(new CCPGTranPosiDlg);   // 0 - Design position
		m_DlgIDs.Add(__TMMODE_DGN_CPG_POSITION__);
		m_TableIDs.Add(ID_QUERY_CPG_DESIGN_POSI);

		m_ChildDlgs.Add(new CCPGPosiDgnOutDlg); // 1 - Position for Design Output 
		m_DlgIDs.Add(__TMMODE_DGN_CPG_POSI_DGN_OUT__);
		m_TableIDs.Add(ID_QUERY_CPG_POSI_DGN_OUT);

		m_ChildDlgs.Add(new CDgnGenLengDlg);    // 2 - Unbraced Length
		m_DlgIDs.Add(__TMMODE_DGN_CPG_LENG__);  //__TMMODE_DGN_GEN_LENG__
		m_TableIDs.Add(ID_QUERY_DGN_GEN_LENG);

		m_ChildDlgs.Add(new CCPGShearConnDlg);  // 3 - Shear connector
		m_DlgIDs.Add(__TMMODE_DGN_CPG_SHEAR_CONN__);
		m_TableIDs.Add(ID_QUERY_CPG_DESIGN_LSD_SHEAR_CONNECT);

		m_ChildDlgs.Add(new CDgnStlRatingWebPanelDlg);  // 4 - Web Panel
		m_DlgIDs.Add(__TMMODE_DGN_CPG_WEB_PANEL__);     // __TMMODE_DGN_STEEL_RATING_WEB_PANEL__
		m_TableIDs.Add(0);

		m_ChildDlgs.Add(new CCPGResidualStressDlg); // 5 - Residual Stress Type
		m_DlgIDs.Add(__TMMODE_DGN_CPG_RESIDUAL_STRESS__);
		m_TableIDs.Add(ID_QUERY_CPG_DESIGN_RESIDUAL_STRESS);
	}
	else
	{
		m_ChildDlgs.Add(new CCPGTranPosiDlg); // 0
		m_DlgIDs.Add(__TMMODE_DGN_CPG_POSITION__);
		m_TableIDs.Add(ID_QUERY_CPG_DESIGN_POSI);

	m_ChildDlgs.Add(new CCPGPosiDgnOutDlg); // 1
	m_DlgIDs.Add(__TMMODE_DGN_CPG_POSI_DGN_OUT__);
	m_TableIDs.Add(ID_QUERY_CPG_POSI_DGN_OUT);

	if (CpgdD.iDgnCode == IRC22_15_CSG)
	{
		m_ChildDlgs.Add(new CCPGTranShear_IRC_Dlg); // 2
		m_DlgIDs.Add(__TMMODE_DGN_CPG_SHEAR__);
		m_TableIDs.Add(ID_QUERY_CPG_DESIGN_SHEAR_CONNECT);
	}
	else
	{
		m_ChildDlgs.Add(new CCPGTranShearDlg); // 2
		m_DlgIDs.Add(__TMMODE_DGN_CPG_SHEAR__);
		m_TableIDs.Add(ID_QUERY_CPG_DESIGN_SHEAR_CONNECT);
	}

		m_ChildDlgs.Add(new CCPGTranStifDlg); // 3
		m_DlgIDs.Add(__TMMODE_DGN_CPG_STIF__);
		m_TableIDs.Add(ID_QUERY_CPG_DESIGN_TRAN_STIF);

	m_ChildDlgs.Add(new CCPGLoadApplicationDlg); // 4
	m_DlgIDs.Add(__TMMODE_DGN_CPG_LOAD_APPLICATION__);
	m_TableIDs.Add(ID_QUERY_CPG_DESIGN_TYPE_LOAD);

		if (CpgdD.iDgnCode == IRC22_15_CSG)
		{
			m_ChildDlgs.Add(new CCPGLateraTorsion_IRC_Dlg); // 5
			m_DlgIDs.Add(__TMMODE_DGN_CPG_TORSION__);
			m_TableIDs.Add(ID_QUERY_CPG_DESIGN_TORSION);
		}
		else
		{
			m_ChildDlgs.Add(new CCPGLateraTorsionDlg); // 5
			m_DlgIDs.Add(__TMMODE_DGN_CPG_TORSION__);
			m_TableIDs.Add(ID_QUERY_CPG_DESIGN_TORSION);
		}


		if (CpgdD.iDgnCode == IRC22_15_CSG)
		{
			m_ChildDlgs.Add(new CCPGDamageEquivalence_IRC_Dlg); // 6
			m_DlgIDs.Add(__TMMODE_DGN_CPG_DAMAGE__);
			m_TableIDs.Add(ID_QUERY_CPG_DESIGN_DAMAGE);
		}
		else
		{
			m_ChildDlgs.Add(new CCPGDamageEquivalenceDlg); // 6
			m_DlgIDs.Add(__TMMODE_DGN_CPG_DAMAGE__);
			m_TableIDs.Add(ID_QUERY_CPG_DESIGN_DAMAGE);
		}


		//PMS:LRFD12 BSC-20131231
		m_ChildDlgs.Add(new CCPGShearConnDlg); // 7
		m_DlgIDs.Add(__TMMODE_DGN_CPG_SHEAR_CONN__);
		m_TableIDs.Add(ID_QUERY_CPG_DESIGN_LSD_SHEAR_CONNECT);

		m_ChildDlgs.Add(new CCPGFatigueParamDlg); // 8
		m_DlgIDs.Add(__TMMODE_DGN_CPG_FATIGUE_PARAM__);
		m_TableIDs.Add(ID_QUERY_CPG_DESIGN_FATIGUE_PARAM);

		m_ChildDlgs.Add(new CCPGCurveBridDlg); // 9
		m_DlgIDs.Add(__TMMODE_DGN_CPG_CURVE_BRID__);
		m_TableIDs.Add(ID_QUERY_CPG_DESIGN_CURV_BRID);

		m_ChildDlgs.Add(new CCPGDeckOverhangDlg);  // 10
		m_DlgIDs.Add(__TMMODE_DGN_CPG_DECK_OVERHANG__);
		m_TableIDs.Add(ID_QUERY_CPG_DESIGN_DECK_OVERHANG_LOAD);

		// For Russia Steel Composite Design
		// Shear Connector Group Dlg
		m_ChildDlgs.Add(new CCPGShearConnGroupDlg); // 11 
		m_DlgIDs.Add(__TMMODE_DGN_CPG_SHEAR_CONN_GROUP__);
		m_TableIDs.Add(ID_QUERY_CPG_SNIP_RUS_DGN_IND_TB_SHEAR_CONNECTOR);

		// Limit State Group Dlg
		m_ChildDlgs.Add(new CDgnConServiceLimtDlg); // 12
		dynamic_cast<CDgnConServiceLimtDlg*>(m_ChildDlgs[12])->SetIsCpgDesign(TRUE);
		m_DlgIDs.Add(__TMMODE_DGN_CPG_SERV__);
		m_TableIDs.Add(ID_QUERY_CPG_SNIP_RUS_DGN_IND_TB_LIMIT_STATE_GROUP_PARAM);

		m_ChildDlgs.Add(new CCPGPosiDgnOutDlg); // 13 - for Russia Output Table
		m_DlgIDs.Add(__TMMODE_DGN_CPG_POSI_DGN_OUT__);
		m_TableIDs.Add(ID_QUERY_CPG_SNIP_RUS_DGN_IND_TB_PRINT_POSITION);
	}

#endif
}

void CDgnCpgDlg::DeleteChildDlgs()
{
	for (int i = 0; i < m_ChildDlgs.GetSize(); i++)	delete m_ChildDlgs[i];
}

// TreeMenu Mode에 해당하는 Child Dialog를 활성화 시킨다. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CDgnCpgDlg::ShowChildDlgByID(int nID, int nCPG)
{
	T_CPGD_D CpgdD; CpgdD.Initialize();
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	if (nID == __TMMODE_DGN_CPG__)
	{
		ShowCurChildDlg(0);
		m_ComboMenu.SetCurSel(0);
		return TRUE;
	}
	else if (nID == __TMMODE_DGN_CPG_TORSION__)
	{
		if (CpgdD.iDgnCode == IRC22_15_CSG)
		{
			//       ShowCurChildDlg(3);  
			m_ComboMenu.SetCurSel(3);
			//return TRUE;
		}

	}
	else if (nID == __TMMODE_DGN_CPG_DAMAGE__)
	{
		if (CpgdD.iDgnCode == IRC22_15_CSG)
		{
			//       ShowCurChildDlg(3);  
			m_ComboMenu.SetCurSel(4);
			//return TRUE;
		}
	}

	T_RCDS_D RcdsD; RcdsD.Initialize();
	pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD);

	int iDgnCode = nCPG == 0 ? CpgdD.iDgnCode : RcdsD.nRatingCode;
	nID = nCPG == 0 ? nID : ConvertDlgID_Rating2CPG(nID);

	int nIDCount = m_DlgIDs.GetSize();
	int nCurI = 0;
	for (int i = 0; i < nIDCount; i++)
	{
		if (nID == m_DlgIDs[i])
		{
			ShowCurChildDlg(i);
			EnalbeDisableCtrl4SubDlg(nID, iDgnCode);
			GetChildDlgCurl(iDgnCode, nID, nCurI);

			m_ComboMenu.SetCurSel(nCurI);
			if (m_TableIDs[i] > 0)	m_wndTableBtn.EnableWindow(TRUE);
			else					m_wndTableBtn.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CDgnCpgDlg::ShowCurChildDlg(int nDlgIndex)
{
	if (m_nCurChildDlg == nDlgIndex)	return;

	if (nDlgIndex > m_ChildDlgs.GetSize() - 1)
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}

	if (m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize() - 1 >= m_nCurChildDlg)
		m_ChildDlgs[m_nCurChildDlg]->End();

	m_ChildDlgs[nDlgIndex]->CreateInit(&m_wndPlaceHolder, 0, 0);
	m_nCurChildDlg = nDlgIndex;

	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect, ParentRect, PHRect;

	GetClientRect(&ParentRect);
	m_ChildDlgs[nDlgIndex]->GetWindowRect(&DlgRect);
	ScreenToClient(&DlgRect);

	int Sx, Sy, Wx, Wy;
	Sx = (ParentRect.Width() - DlgRect.Width()) / 2;
	Sy = DlgRect.top;

	Wx = DlgRect.Width();
	Wy = DlgRect.Height();

	m_wndPlaceHolder.SetWindowPos(NULL, Sx, Sy, Wx, Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndPlaceHolder.GetWindowRect(&PHRect);

	ScreenToClient(&PHRect);

	/////////////////////////////////////////////////////////////
	// Scroll Size를 조정한다. 
	CSize ScrSize, TSize;
	ScrSize = GetTotalSize();

	TSize.cx = PHRect.Width() - m_RectPlaceHolder.Width();
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();

	ScrSize += TSize;

	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT, ScrSize);
}

int CDgnCpgDlg::GetHeightFromFont()
{
	CFont * pNewFont = GetFont();
	CFont *pSysFont, *pOldFont, TempFont;
	CDC   *pCdc;
	TEXTMETRIC  tmNew, tmSys;
	int     nTemp, nCorrectVal;

	//get the DC for the edit control
	pCdc = GetDC();

	//get the metrics for the system font
	pSysFont = TempFont.FromHandle(((HFONT)GetStockObject(SYSTEM_FONT)));
	pOldFont = pCdc->SelectObject(pSysFont);
	pCdc->GetTextMetrics(&tmSys);

	//get the metrics for the new font
	pCdc->SelectObject(pNewFont);
	pCdc->GetTextMetrics(&tmNew);

	//select the original font back into the DC and release the DC
	pCdc->SelectObject(pOldFont);
	ReleaseDC(pCdc);

	nCorrectVal = (min(tmNew.tmHeight, tmSys.tmHeight) / 2);
	//calculate the new height for the edit control
	nTemp = tmNew.tmHeight + nCorrectVal;
	//nTemp = tmNew.tmHeight + tmNew.tmInternalLeading;
	return nTemp;
}

void CDgnCpgDlg::OnInitialUpdate()
{
	MChildFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	Update_ComboMenu();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CDgnCpgDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: Add your specialized code here and/or call the base class

	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		if (m_nCurChildDlg != -1)
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

void CDgnCpgDlg::OnComboMenuSelect()
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	T_CPGD_D CpgdD; CpgdD.Initialize();
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);
	int nChildIDIndex = 0;
	GetChildDlgSelectCurl(CpgdD.iDgnCode, nCurI, nChildIDIndex);

	// ShowCurChildDlg를 직접 부르면 CBarTreeMenu의 m_nCurMode가 Update
	// 되지 않아 Menu에서 Select해도 해당 Dialog가 뜨지 않는 경우가 발생
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nChildIDIndex));
	//ShowCurChildDlg(nCurI);
}

int CDgnCpgDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	return 0;
}

void CDgnCpgDlg::OnDgnTableBtn()
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	T_CPGD_D CpgdD; CpgdD.Initialize();
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	int nChildIDIndex = 0;
	GetChildDlgSelectCurl(CpgdD.iDgnCode, nCurSel, nChildIDIndex);

	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nChildIDIndex], 0));
}

// Change by ZINU.('01.8.1). To change Dynamic Combo Menu.
void CDgnCpgDlg::Update_ComboMenu()
{
#if defined(_CIVIL) 
	m_ComboMenuStr.RemoveAll();

	T_CPGD_D CpgdD; CpgdD.Initialize();
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);
	
	if (CpgdD.iDgnCode == AS5100_6_17_CSG)
	{
		m_ComboMenuStr.SetSize(6);
		m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_CPG_CHECK);
		m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_CPG_POSU_DGN_OUT);
		m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LENG); // Unbraced Length
		m_ComboMenuStr[3] = _LS(IDS_DGN_TREE_TEXT_CPG_SHEAR);
		m_ComboMenuStr[4] = _LS(IDS_DGN_TREE_TEXT_STEEL_WEB_PANEL_ASSIGNMENT);
		m_ComboMenuStr[5] = _LS(IDS_DGN_TREE_TEXT_STEEL_RESIDUAL_STRESS);	
	}
	else if (CpgdD.iDgnCode == EUROCODE4_2_04_CSG )
	{
		m_ComboMenuStr.SetSize(7);
		m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_CPG_CHECK);
		m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_CPG_POSU_DGN_OUT);
		m_ComboMenuStr[2] = _LS(IDS_DGN_TREE_TEXT_CPG_SHEAR);
		m_ComboMenuStr[3] = _LS(IDS_DGN_TREE_TEXT_CPG_TREN);
		m_ComboMenuStr[4] = _LS(IDS_DGN_TREE_TEXT_CPG_LOAD);
		m_ComboMenuStr[5] = _LS(IDS_DGN_TREE_TEXT_CPG_TORSION);
		m_ComboMenuStr[6] = _LS(IDS_DGN_TREE_TEXT_CPG_DAMAGE);
	}
	else if( CDBLib::IsCSGCodeUS(CpgdD.iDgnCode) || CpgdD.iDgnCode == KSSC_2014_CSG || CDBLib::IsCSGCodeLSD_KR(CpgdD.iDgnCode) )
	{
		m_ComboMenuStr.SetSize(7);
		m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_CPG_CHECK);
		m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_CPG_POSU_DGN_OUT);
		m_ComboMenuStr[2] = _LS(IDS_DGN_TREE_TEXT_CPG_LOAD);
		m_ComboMenuStr[3] = _LS(IDS_DGN_TREE_TEXT_CPG_SHEAR);
		m_ComboMenuStr[4] = _LS(IDS_DGN_TREE_TEXT_CPG_FATIGUE_PARAM);
		m_ComboMenuStr[5] = _LS(IDS_DGN_TREE_TEXT_CPG_CURVED_BRIDG);
		m_ComboMenuStr[6] = _LS(IDS_DGN_TREE_TEXT_CPG_DECK_OVERHANG);
	}
	else if (CpgdD.iDgnCode == SNiP_20503_84_CSG || CpgdD.iDgnCode == SP_35_13330_11_CSG)
	{
		m_ComboMenuStr.SetSize(3);
		m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_CPG_SHEAR_CONN_GROUP);
		m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_CPG_LIMIT_STATE_GROUP_PARAM);
		m_ComboMenuStr[2] = _LS(IDS_DGN_TREE_TEXT_CPG_POSU_DGN_OUT);
	}
	else if (CpgdD.iDgnCode == IRC22_08_CSG)
	{
		m_ComboMenuStr.SetSize(7);
		m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_CPG_CHECK);
		m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_CPG_POSU_DGN_OUT);
		m_ComboMenuStr[2] = _LS(IDS_DGN_TREE_TEXT_CPG_SHEAR);
		m_ComboMenuStr[3] = _LS(IDS_DGN_TREE_TEXT_CPG_TREN);
		m_ComboMenuStr[4] = _LS(IDS_DGN_TREE_TEXT_CPG_LOAD);
		m_ComboMenuStr[5] = _LS(IDS_DGN_TREE_TEXT_CPG_TORSION);
		m_ComboMenuStr[6] = _LS(IDS_DGN_TREE_TEXT_CPG_DAMAGE_IRC);
	}
	else if (CpgdD.iDgnCode == IRC22_15_CSG)
	{
		m_ComboMenuStr.SetSize(5);
		m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_CPG_CHECK);
		m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_CPG_POSU_DGN_OUT);
		m_ComboMenuStr[2] = _LS(IDS_DGN_TREE_TEXT_CPG_SHEAR);
		//m_ComboMenuStr[3] = _LS(IDS_DGN_TREE_TEXT_CPG_TREN);	
	 /* m_ComboMenuStr[3] = _LS(IDS_DGN_TREE_TEXT_CPG_LOAD);	*/
		m_ComboMenuStr[3] = _LS(IDS_DGN_TREE_TEXT_CPG_TORSION);	
		m_ComboMenuStr[4] = _LS(IDS_DGN_TREE_TEXT_CPG_DAMAGE_IRC);	
	}
	else if (CpgdD.iDgnCode == CS457_R1_CSG)
	{
		m_ComboMenuStr.SetSize(3);
		m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_CPG_CHECK);
		m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_CPG_POSU_DGN_OUT);
		m_ComboMenuStr[2] = _LS(IDS_DGN_TREE_TEXT_CPG_SHEAR);
	}
	else
	{
		m_ComboMenuStr.SetSize(5);
		m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_CPG_CHECK);
		m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_CPG_POSU_DGN_OUT);
		m_ComboMenuStr[2] = _LS(IDS_DGN_TREE_TEXT_CPG_SHEAR);
		m_ComboMenuStr[3] = _LS(IDS_DGN_TREE_TEXT_CPG_FATIGUE_PARAM);
		m_ComboMenuStr[4] = _LS(IDS_DGN_TREE_TEXT_CPG_CURVED_BRIDG);
	}

	int iSelID = m_ComboMenu.GetCurSel();
	// Initialize Data.
	m_ComboMenu.ResetContent();
	int wHeight = (m_ComboMenuStr.GetSize() + 1) * GetHeightFromFont();

	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL, 0, 0, rectWnd.Width(), wHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	// Add String at ComboBox.
	for (int i = 0; i < m_ComboMenuStr.GetSize(); i++)
	{
		m_ComboMenu.SetItemData(i, m_ComboMenu.AddString(m_ComboMenuStr[i]));
	}
	// Recalculate Width.
	CClientDC dc(this);
	int nSize = m_ComboMenuStr.GetSize();
	CSize sizeMax, sizeCur;
	sizeMax = dc.GetTextExtent(m_ComboMenuStr[0]);
	for (int k = 1; k < nSize; k++)
	{
		sizeCur = dc.GetTextExtent(m_ComboMenuStr[k]);
		if (sizeCur.cx > sizeMax.cx)	sizeMax.cx = sizeCur.cx;
	}
	if (sizeMax.cx > m_ComboMenu.GetDroppedWidth())	m_ComboMenu.SetDroppedWidth(sizeMax.cx);

	int nCurComMenuNum = m_ComboMenuStr.GetSize();

	if (iSelID < 0 || iSelID >= nCurComMenuNum) { m_ComboMenu.SetCurSel(0); m_ComboMenu.SetWindowText(m_ComboMenuStr[0]); }
	else { m_ComboMenu.SetCurSel(iSelID); m_ComboMenu.SetWindowText(m_ComboMenuStr[iSelID]); }

	if(iSelID < 0 || iSelID >= nCurComMenuNum )	{m_ComboMenu.SetCurSel(0); m_ComboMenu.SetWindowText(m_ComboMenuStr[0]);}
	else						                    {m_ComboMenu.SetCurSel(iSelID); m_ComboMenu.SetWindowText(m_ComboMenuStr[iSelID]);}
 
	int nChildDlgIndex = 0;
	GetChildDlgSelectCurl(CpgdD.iDgnCode, iSelID, nChildDlgIndex);
	ShowCurChildDlg(nChildDlgIndex);

#endif
}


void CDgnCpgDlg::GetChildDlgCurl(int nDgnCode, int nID, int &nCurl)
{
    nCurl = 0;
    switch ( nDgnCode )
    {
    case EUROCODE4_2_04_CSG:
    case IRC22_08_CSG:
        {
            switch ( nID )
            {
            case __TMMODE_DGN_CPG_POSITION__:           nCurl = 0; break;
            case __TMMODE_DGN_CPG_POSI_DGN_OUT__:       nCurl = 1; break;
            case __TMMODE_DGN_CPG_SHEAR__:              nCurl = 2; break;
            case __TMMODE_DGN_CPG_STIF__:               nCurl = 3; break;
            case __TMMODE_DGN_CPG_LOAD_APPLICATION__:	nCurl = 4; break;
            case __TMMODE_DGN_CPG_TORSION__:            nCurl = 5; break;
            case __TMMODE_DGN_CPG_DAMAGE__:             nCurl = 6; break;
            default: ASSERT(0);        break;
            }
        }
        break;
    case AASHTO_LRFD07_CSG:
    case AASHTO_LRFD12_CSG:
    case AASHTO_LRFD16_CSG:
    case AASHTO_LRFD17_CSG:
    case AASHTO_LRFD20_CSG:
    case STL_AASHTO_LRFD12:
    case STL_AASHTO_LRFD19:
    case KSSC_2014_CSG:
    case KSCE_LSD15_CSG:
    case KDS_24_14_31_2018_CSG:
        {
            switch ( nID )
            {
            case __TMMODE_DGN_CPG_POSITION__:					nCurl = 0; break;
			case __TMMODE_DGN_CPG_POSI_DGN_OUT__:				nCurl = 1; break;
			case __TMMODE_DGN_CPG_LOAD_APPLICATION__:			nCurl = 2; break;
            case __TMMODE_DGN_CPG_SHEAR_CONN__:					nCurl = 3; break;
            case __TMMODE_DGN_CPG_FATIGUE_PARAM__:
            case __TMMODE_DGN_STEEL_RATING_FATIGUE_PARAM__:     nCurl = 4; break;
            case __TMMODE_DGN_CPG_CURVE_BRID__:
            case __TMMODE_DGN_STEEL_RATING_CURVE_BRID__:        nCurl = 5; break;
            case __TMMODE_DGN_CPG_DECK_OVERHANG__:
            case __TMMODE_DGN_STEEL_RATING_DECK_OVERHANG__:		nCurl = 6; break;

            default: ASSERT(0);        break;
            }
        }
        break;
    case SNiP_20503_84_CSG:
    case SP_35_13330_11_CSG:
        {
            switch ( nID )
            {
            case __TMMODE_DGN_CPG_SHEAR_CONN_GROUP__:      nCurl = 0; break;
            case __TMMODE_DGN_CPG_SERV__:                  nCurl = 1; break;
            case __TMMODE_DGN_CPG_POSI_DGN_OUT__:          nCurl = 2; break;
            default: ASSERT(0);        break;
            }
        }
        break;
    case CSA_S6_10_CSG:
    case CSA_S6_14_CSG:
	case CSA_S6_19_CSG:
        {
            switch ( nID )
            {
            case __TMMODE_DGN_CPG_POSITION__:					nCurl = 0; break;
            case __TMMODE_DGN_CPG_POSI_DGN_OUT__:				nCurl = 1; break;
            case __TMMODE_DGN_CPG_SHEAR_CONN__:					nCurl = 2; break;
            case __TMMODE_DGN_CPG_FATIGUE_PARAM__:
            case __TMMODE_DGN_STEEL_RATING_FATIGUE_PARAM__:     nCurl = 3; break;
            case __TMMODE_DGN_CPG_CURVE_BRID__:
            case __TMMODE_DGN_STEEL_RATING_CURVE_BRID__:        nCurl = 4; break;
            case __TMMODE_DGN_CPG_DECK_OVERHANG__:
            case __TMMODE_DGN_STEEL_RATING_DECK_OVERHANG__:		nCurl = 5; break;
            default: ASSERT(0);        break;
                break;
            }
        }
        break;
    case IRC22_15_CSG:
        {
            switch ( nID )
            {
            case __TMMODE_DGN_CPG_POSITION__:           nCurl = 0; break;
            case __TMMODE_DGN_CPG_POSI_DGN_OUT__:       nCurl = 1; break;
            case __TMMODE_DGN_CPG_SHEAR__:              nCurl = 2; break;
            case __TMMODE_DGN_CPG_STIF__:               nCurl = 5; break;
            case __TMMODE_DGN_CPG_LOAD_APPLICATION__:	nCurl = 5; break;
            case __TMMODE_DGN_CPG_TORSION__:            nCurl = 5; break;
            case __TMMODE_DGN_CPG_DAMAGE__:             nCurl = 6; break;
            default: ASSERT(0);        break;
            }
        }
        break;
    case CS457_R1_CSG:
        {
            switch ( nID )
            {
            case __TMMODE_DGN_CPG_POSITION__:           nCurl = 0; break;
            case __TMMODE_DGN_CPG_POSI_DGN_OUT__:       nCurl = 1; break;
            case __TMMODE_DGN_CPG_SHEAR__:              nCurl = 2; break;
//             case __TMMODE_DGN_CPG_STIF__:               nCurl = 5; break;
//             case __TMMODE_DGN_CPG_LOAD_APPLICATION__:	nCurl = 5; break;
//             case __TMMODE_DGN_CPG_TORSION__:            nCurl = 5; break;
//             case __TMMODE_DGN_CPG_DAMAGE__:             nCurl = 6; break;
            default: ASSERT(0);        break;
            }

        }
		break;
	case AS5100_6_17_CSG:
		{
			switch ( nID )
			{
			case __TMMODE_DGN_CPG_POSITION__:           nCurl = 0; break;
			case __TMMODE_DGN_CPG_POSI_DGN_OUT__:       nCurl = 1; break;
			case __TMMODE_DGN_CPG_LENG__:				nCurl = 2; break;
			case __TMMODE_DGN_CPG_SHEAR_CONN__:         nCurl = 3; break;
			case __TMMODE_DGN_CPG_WEB_PANEL__:          nCurl = 4; break;
			case __TMMODE_DGN_CPG_RESIDUAL_STRESS__:	nCurl = 5; break;
			default: ASSERT(0);        break;
			}
		}
		break;
	default:
        {
            switch ( nID )
            {
            case __TMMODE_DGN_CPG_POSITION__:			nCurl = 0; break;
            case __TMMODE_DGN_CPG_POSI_DGN_OUT__:       nCurl = 1; break;
            case __TMMODE_DGN_CPG_SHEAR_CONN__:         nCurl = 2; break;
            case __TMMODE_DGN_CPG_FATIGUE_PARAM__:      nCurl = 3; break;
            case __TMMODE_DGN_CPG_CURVE_BRID__:         nCurl = 4; break;
            default: ASSERT(0);        break;
            }
        }
        break;
    }
}

void CDgnCpgDlg::GetChildDlgSelectCurl(int nDgnCode, int nCurl, int &nChildIDIndex)
{
		if(nCurl < 0) nCurl = 0;
		nChildIDIndex = 0;
		switch (nDgnCode)
		{
		case EUROCODE4_2_04_CSG: 
		case IRC22_08_CSG:
			{
				if(nCurl > 6) nCurl = 0;
				switch (nCurl)
				{
				case 0:  nChildIDIndex = 0; break;
				case 1:   nChildIDIndex = 1; break;
				case 2:   nChildIDIndex = 2; break;
				case 3:   nChildIDIndex = 3; break;
				case 4:   nChildIDIndex = 4; break;
				case 5:    nChildIDIndex = 5; break;
				case 6:    nChildIDIndex = 6; break;
				default: ASSERT(0);        break;
				}
			}
			break;
		case AASHTO_LRFD07_CSG: 
		case AASHTO_LRFD12_CSG: 
		case AASHTO_LRFD16_CSG:
        case AASHTO_LRFD17_CSG:
        case AASHTO_LRFD20_CSG:
		case STL_AASHTO_LRFD12: 		
        case STL_AASHTO_LRFD19:
		case KSSC_2014_CSG:
		case KSCE_LSD15_CSG:
		case KDS_24_14_31_2018_CSG:
			{
				if (nCurl > 6) nCurl = 0;
				switch (nCurl)
				{
				case 0:  nChildIDIndex = 0; break;
				case 1:   nChildIDIndex = 1; break;
				case 2:   nChildIDIndex = 4; break;
				case 3:   nChildIDIndex = 7; break;
				case 4:   nChildIDIndex = 8; break;
				case 5:   nChildIDIndex = 9; break;
				case 6:    nChildIDIndex = 10; break;
				default: ASSERT(0);        break;
				}
			}
		break;
		case CSA_S6_14_CSG:
		case CSA_S6_19_CSG:
			{
				if(nCurl > 5) nCurl = 0;
				switch (nCurl)
				{
				case 0:  nChildIDIndex = 0; break;
				case 1:   nChildIDIndex = 1; break;
				case 2:   nChildIDIndex = 7; break;
				case 3:   nChildIDIndex = 8; break;
				case 4:   nChildIDIndex = 9; break;
				case 5:    nChildIDIndex = 10; break;
				default: ASSERT(0);        break;
				}
			}
			break;
		case SNiP_20503_84_CSG: 
		case SP_35_13330_11_CSG: 
			{
				if(nCurl > 2) nCurl = 0;
				switch (nCurl)
				{
				case 0:  nChildIDIndex = 11; break;
				case 1:   nChildIDIndex = 12; break;
				case 2:   nChildIDIndex = 13; break;
				default: ASSERT(0);        break;
				}
			}
			break;
		case IRC22_15_CSG:
			{
				if(nCurl > 6) nCurl = 0;
				switch (nCurl)
				{
				case 0:  nChildIDIndex = 0; break;
				case 1:   nChildIDIndex = 1; break;
				case 2:   nChildIDIndex = 2; break;
				case 3:   nChildIDIndex = 5; break;
				case 4:   nChildIDIndex = 6; break;
				case 5:    nChildIDIndex = 5; break;
				case 6:    nChildIDIndex = 6; break;
				default: ASSERT(0);        break;
				}
			}
			break;
        case CS457_R1_CSG:
            {
                if ( nCurl > 2 ) nCurl = 0;
                switch ( nCurl )
                {
                case 0:  nChildIDIndex = 0; break;
                case 1:  nChildIDIndex = 1; break;
                case 2:  nChildIDIndex = 2; break;
                default: ASSERT(0);        break;
                }
            }
            break;
		case AS5100_6_17_CSG:
			{
				if (nCurl > 5)
				{
					nCurl = 0;
					ASSERT(0);
				}
				nChildIDIndex = nCurl;
			}
			break;
		default:
			{
				switch (nCurl)
				{
				if(nCurl > 4) nCurl = 0;
				case 0:  nChildIDIndex = 0; break;
				case 1:   nChildIDIndex = 1; break;
				case 2:   nChildIDIndex = 7; break;
				case 3:   nChildIDIndex = 8; break;
				case 4:   nChildIDIndex = 9; break;
				default: ASSERT(0);        break;
				}
			}
			break;
		}
}

void CDgnCpgDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DEFAULT:
		Update_ComboMenu();
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
		Update_ComboMenu();
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

void CDgnCpgDlg::Update_InitDataByCode(BOOL bUpdate)
{
	if (bUpdate)
	{
		CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
		int nCount = pViewBuff->GetCount();
		if (nCount == 0) return;
		ASSERT(nCount == 1);

		BOOL bCODE = FALSE;
		T_UDRD_BUFFER buffer_ur;
		POSITION pos = pViewBuff->GetStartBuffer();
		while (pos)
		{
			buffer_ur = pViewBuff->GetNextBuffer(pos);
			int nCmd = buffer_ur.nCmd;
			int nKey = buffer_ur.nKey;

			switch (nCmd)
			{
			case(UR_CPGD_ADD):
			case(UR_CPGD_DEL): {bCODE = TRUE; break; }
			default:	break;
			}
		}
		if (!bCODE) return;
	}

	int iSelID = m_ComboMenu.GetCurSel();
	if (iSelID < 0)	iSelID = 0;

	//  Update_ComboMenu();
	// 	if(m_ComboMenu.GetCount()>0)
	//   {
	//     ShowCurChildDlg(0);  
	//     m_ComboMenu.SetCurSel(0);
	//   }

}

int CDgnCpgDlg::ConvertDlgID_Rating2CPG(int nID)
{
	switch (nID)
	{
	case __TMMODE_DGN_STEEL_RATING_FATIGUE_PARAM__:	return __TMMODE_DGN_CPG_FATIGUE_PARAM__;
	case __TMMODE_DGN_STEEL_RATING_CURVE_BRID__: 	return __TMMODE_DGN_CPG_CURVE_BRID__;
	case __TMMODE_DGN_STEEL_RATING_DECK_OVERHANG__:	return __TMMODE_DGN_CPG_DECK_OVERHANG__;
	default:									return nID;
	}
}


void CDgnCpgDlg::EnalbeDisableCtrl4SubDlg(int nID, int nCurDgnCode)
{
	switch (nID)
	{
	case __TMMODE_DGN_CPG_FATIGUE_PARAM__:
	case __TMMODE_DGN_STEEL_RATING_FATIGUE_PARAM__:
	{
		CCPGFatigueParamDlg *pFatigueSubDlg = NULL;
		pFatigueSubDlg = dynamic_cast<CCPGFatigueParamDlg*>(m_ChildDlgs[8]);
		if (pFatigueSubDlg != NULL)
		{
			pFatigueSubDlg->SetDgnCode(nCurDgnCode);
			pFatigueSubDlg->SubDlgCtrlEnableDisable();
		}
		break;
	}
	case __TMMODE_DGN_CPG_CURVE_BRID__:
	case __TMMODE_DGN_STEEL_RATING_CURVE_BRID__:
	{
		CCPGCurveBridDlg *pCurveSubDlg = NULL;
		pCurveSubDlg = dynamic_cast<CCPGCurveBridDlg*>(m_ChildDlgs[9]);
		if (pCurveSubDlg != NULL)
		{
			pCurveSubDlg->SetDgnCode(nCurDgnCode);
			pCurveSubDlg->SubDlgCtrlEnableDisable();
		}
		break;
	}
	case __TMMODE_DGN_CPG_DECK_OVERHANG__:
	case __TMMODE_DGN_STEEL_RATING_DECK_OVERHANG__:
	{
		CCPGDeckOverhangDlg *pDeckOverhangSubDlg = NULL;
		pDeckOverhangSubDlg = dynamic_cast<CCPGDeckOverhangDlg*>(m_ChildDlgs[10]);
		if (pDeckOverhangSubDlg != NULL)
		{
			pDeckOverhangSubDlg->SetDgnCode(nCurDgnCode);
			pDeckOverhangSubDlg->SubDlgCtrlEnableDisable();
		}
		break;
	}
	default: break;
	}
}

void CDgnCpgDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CDgnCpgDlg::OnTmClose()
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