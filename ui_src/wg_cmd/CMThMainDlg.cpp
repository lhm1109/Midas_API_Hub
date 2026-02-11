// CMThMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThMainDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\I_GENStateDefine.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\mit_frx\MBarBaseDlg.h"

#include "CMThgaDlg.h"
#include "CMThnlDlg.h"
#include "CMThspDlg.h"
#include "CMThmsDlg.h"
#include "CMThcrDlg.h"
#include "CMThatDlg.h"

#include "CMThrdDlg.h"
#include "CMThrdDisp.h"
#include "CMThrdBeam.h"
#include "CMThrdWall.h"
#include "CMThrdHnge.h"
#include "CMThrdNllk.h"
#include "CMThrdPlate.h"
#include "CMThrdPnStrn.h"
#include "CMThrdPnStrs.h"
#include "CMThrdSolid.h"
#include "CMThrdTruss.h"
#include "CMThrdLoadIncrement.h"
#include "CMThrdDispStory.h"

#include "CMThrdStepBeamDlg.h"
#include "CMThrdStepWallDlg.h"
#include "CMThrdStepDdofDlg.h"
#include "CMThrdStepGnlkDlg.h"
#include "CMThrdStepDispDlg.h"
#include "CMThrdStepTimeDlg.h"
#include "CMThrdStepTrussDlg.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int ____bNotifyCombo = TRUE;
/////////////////////////////////////////////////////////////////////////////
// CCMThMainDlg dialog

#define CFormView CCMDlgBarBase

IMPLEMENT_DYNCREATE(CCMThMainDlg, CFormView)

TCHAR * CCMThMainDlg::m_ComboMenuStr[]=
{
	_T(" "),
	_T(" "),
	_T(" "),
	_T(" "),
	_T(" "),
#if defined(_CIVIL)
	_T(" "),
#endif
#if !defined(_CIVIL)
	_T(" ")
#endif
};





CCMThMainDlg::CCMThMainDlg(CWnd* pParent /*=NULL*/)
	: CFormView(CCMThMainDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMThMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	CArray<const TCHAR*, const TCHAR*> ComboStrs;
	ComboStrs.Add(_LS(IDS_CMD_0524Ground_Acceleration));  
	ComboStrs.Add(_LS(IDS_CMD_0524Dynamic_Nodal_Loads)); 
	ComboStrs.Add(_LS(IDS_CMD_0524Dynamic_Spatial_Load));
	ComboStrs.Add(_LS(IDS_CMD_PJST_THMS));
	ComboStrs.Add(_LS(IDS_CMD_TIME_RESULT_FUNC));
#if !defined(_CIVIL)
	ComboStrs.Add(_LS(IDS_CMD_DEFINE_ANIMATION_TIME));
#endif

#if defined(_CIVIL)
	ComboStrs.Add(_LS(IDS_CMD_TIME_CONCUR_RESULT_GROP));
#endif

	int nStrs = ComboStrs.GetSize();
	for(int i = 0 ; i < nStrs ; i++)
	{
		m_ComboMenuStr[i] = (TCHAR*) ComboStrs[i];
	}

	CreateChildDlgs();
}

CCMThMainDlg::~CCMThMainDlg()
{
	DeleteChildDlgs();
}

void CCMThMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThMainDlg)
	DDX_Control(pDX, IDC_CMD_TABLE_BUTTON, m_wndTableButton);
	DDX_Control(pDX, IDC_DBAR_FUNC_COMBO, m_ComboMenu);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndPlaceHolder);
	//}}AFX_DATA_MAP
}

bool CCMThMainDlg::ShowApplyBtn()
{
	if ( m_nCurChildDlg < 0 || m_ChildDlgs.GetSize() - 1 < m_nCurChildDlg )
	{
		return true;
	}

	switch ( m_DlgIDs.GetAt(m_nCurChildDlg) )
	{
	case __TMMODE_CMD_THGA__:
	case __TMMODE_CMD_THSP__:
	case __TMMODE_CMD_THRD__:
	case __TMMODE_CMD_THCR__:
		return false;
	default:
		return true;
	}
}

BEGIN_MESSAGE_MAP(CCMThMainDlg, CFormView)
	//{{AFX_MSG_MAP(CCMThMainDlg)
	ON_BN_CLICKED(IDC_CMD_TABLE_BUTTON, OnCmdTableButton)
	ON_CBN_SELCHANGE(IDC_DBAR_FUNC_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThMainDlg message handlers

void CCMThMainDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 
	
	m_ChildDlgs.Add(new CCMThgaDlg);
	m_DlgIDs.Add(__TMMODE_CMD_THGA__);
	m_TableIDs.Add(0);  

	m_ChildDlgs.Add(new CCMThnlDlg);
	m_DlgIDs.Add(__TMMODE_CMD_THNL__);
	m_TableIDs.Add(ID_QUERY_STRUCT_THNL);

	m_ChildDlgs.Add(new CCMThspDlg);
	m_DlgIDs.Add(__TMMODE_CMD_THSP__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThmsDlg);
	m_DlgIDs.Add(__TMMODE_CMD_THMS__);
	m_TableIDs.Add(ID_QUERY_STRUCT_THMS);

	m_ChildDlgs.Add(new CCMThrdDlg);
	m_DlgIDs.Add(__TMMODE_CMD_THRD__);
	m_TableIDs.Add(0);

#if !defined(_CIVIL)
	m_ChildDlgs.Add(new CCMThatDlg);
	m_DlgIDs.Add(__TMMODE_CMD_THAT__);
	m_TableIDs.Add(0);
#endif

#if defined(_CIVIL)
	m_ChildDlgs.Add(new CCMThcrDlg);
	m_DlgIDs.Add(__TMMODE_CMD_THCR__);
	m_TableIDs.Add(0);
#endif

	// �Ʒ����ʹ� THRD�� submenu��.. ���� �������� �־��ֵ���..
	// Combo �����̶� �������� �����Ƿ�.. -> �غ�
	m_ChildDlgs.Add(new CCMThrdDisp);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_DISP__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdTruss);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_TRUSS__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdBeam);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_BEAM__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdPnStrs);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_PLANE_STRESS__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdPnStrn);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_PLANE_STRAIN__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdPlate);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_PLATE__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdSolid);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_SOLID__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdNllk);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_GENERAL_LINK__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdHnge);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_INELASTIC_HINGE__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdLoadIncrement);
	m_DlgIDs.Add(__TMMODE_CMD_THRC_TIME_INCREMENT__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdDispStory);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_DISP_STORY__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdStepTimeDlg);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_STEP_TIMESTEP__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdStepDispDlg);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_STEP_DISPVELACC__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdStepTrussDlg);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_STEP_TRUSS__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdStepBeamDlg);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_STEP_BEAM__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdStepDdofDlg);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_STEP_DESIGDOF__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdStepGnlkDlg);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_STEP_GNLK__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdWall);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_WALL__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CCMThrdStepWallDlg);
	m_DlgIDs.Add(__TMMODE_CMD_THRD_STEP_WALL__);
	m_TableIDs.Add(0);
}

void CCMThMainDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}

void CCMThMainDlg::SetThrdMain(int nFuncType, int nComboIndex)
{
	ShowChildDlgByID(__TMMODE_CMD_THRD__);
	int nIDCount = m_DlgIDs.GetSize();
	BOOL bFind = FALSE;
	int nIndex;
	for (nIndex=0; nIndex<nIDCount; nIndex++)
	{
		if(__TMMODE_CMD_THRD__ == m_DlgIDs[nIndex])
		{
			bFind = TRUE;
			break;
		}
	}
	if (bFind)
		((CCMThrdDlg*)m_ChildDlgs[nIndex])->DlgInit(nFuncType, nComboIndex);
}

// TreeMenu Mode�� �ش��ϴ� Child Dialog�� Ȱ��ȭ ��Ų��. 
BOOL CCMThMainDlg::ShowChildDlgByID(int nID)
{
	if(nID == __TMMODE_CMD_THROOT__)
	{
		____bNotifyCombo = FALSE;    
		m_ComboMenu.SetCurSel(0);
		____bNotifyCombo = TRUE;    
		ShowCurChildDlg(0);  
		
		return TRUE;
	}

	int nIDCount = m_DlgIDs.GetSize();
	for(int i = 0 ; i < nIDCount ; i++)
	{
		if(nID == m_DlgIDs[i])
		{
			____bNotifyCombo = FALSE;    
			m_ComboMenu.SetCurSel(i);
			____bNotifyCombo = TRUE;    
			ShowCurChildDlg(i);  
			if (m_TableIDs[i] > 0) m_wndTableButton.EnableWindow(TRUE);
			else m_wndTableButton.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CCMThMainDlg::InitChildDlgByID(int nID, UINT key)
{
	ShowChildDlgByID(nID);

	int nIDCount = m_DlgIDs.GetSize();
	for (int i=0; i<nIDCount; i++)
	{
		if(nID == m_DlgIDs[i])
		{
			m_ChildDlgs[i]->ExternalInit(key);
			break;
		}
	}
}

void CCMThMainDlg::ShowCurChildDlg(int nDlgIndex)
{
	if(m_nCurChildDlg == nDlgIndex)
		return;
		
	if(nDlgIndex > m_ChildDlgs.GetSize() - 1 )
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
	// Scroll Size�� �����Ѵ�. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();
	
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
 
	ScrSize += TSize;
		
	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);

	// ����ó��. Apply�� ���ܾ��ϴ� ���
	// Child Dlg �� �������� ���
	mit::frx::MBarBaseDlg* pParent = (mit::frx::MBarBaseDlg*)GetParent();
	pParent->SetApplyBtnShowHide(ShowApplyBtn() ? SW_SHOW : SW_HIDE);
}

void CCMThMainDlg::LayoutChildDialog()
{
	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect,ParentRect,PHRect;
	
	GetClientRect(&ParentRect);
	m_ChildDlgs[m_nCurChildDlg]->GetWindowRect(&DlgRect);
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
	// Scroll Size�� �����Ѵ�. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();
	
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
 
	ScrSize += TSize;
		
	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);
}

int CCMThMainDlg::_GetHeightFromFont()
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

void CCMThMainDlg::InitComboMenu()
{
	int wHeight;
	wHeight = (sizeof(m_ComboMenuStr)/sizeof(*m_ComboMenuStr)+1) * _GetHeightFromFont();
	
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL, 0, 0,rectWnd.Width(),wHeight,
				         SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	for( int i=0;  i<sizeof(m_ComboMenuStr)/sizeof(*m_ComboMenuStr); ++i )
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}
	m_ComboMenu.SetCurSel(0);
}

void CCMThMainDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}  


/////////////////////////////////////////////////////////////////////////////
// CCMThMainDlg message handlers
void CCMThMainDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CCMThMainDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CCMThMainDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CCMThMainDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}

	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CCMThMainDlg::OnComboMenuSelect() 
{
	
	// TODO: Add your control notification handler code here
	

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg�� ���� �θ��� CBarTreeMenu�� m_nCurMode�� Update
	// ���� �ʾ� Menu���� Select�ص� �ش� Dialog�� ���� �ʴ� ��찡 �߻�
	//if(____bNotifyCombo)
	//{
		int nCurI = m_ComboMenu.GetCurSel();
		CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//}
	//ShowCurChildDlg(nCurI);	
}

int CCMThMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CCMThMainDlg::OnCmdTableButton() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));				
}

/////////////////////////////////////////////////////////////////////
// By L.C.G
void CCMThMainDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	/*
	CMouseEdit::ImNotAssocWindowST();
	CMouseEdit::DisConnectAssocWindowST();

	if(CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE); 
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE); 
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1,-1);
	}
//  if(bActivate)
	{
		int nCurID;
		if(m_nCurChildDlg != -1) nCurID = m_nCurChildDlg;
		else nCurID = 0;

		ShowCurChildDlg(nCurID);
		CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[nCurID]);

		// ���̺� ���� �׸��� ��� Table ��ư Disable
		if (m_TableIDs[nCurID] > 0) m_wndTableButton.EnableWindow(TRUE);
		else m_wndTableButton.EnableWindow(FALSE);
	}

	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (0 > m_nCurChildDlg || m_nCurChildDlg >= m_ComboMenu.GetCount()) return;

	if(m_DlgIDs[m_nCurChildDlg] == __TMMODE_STRUCT_STLD_PRES__)  // Pressure Load
		((CStldPresDlg *)m_ChildDlgs[m_nCurChildDlg])->CheckMode(TRUE);

	if(m_DlgIDs[m_nCurChildDlg] == __TMMODE_STRUCT_STLD_HPRS__)   // Hydro static Load
		((CStldHprsDlg *)m_ChildDlgs[m_nCurChildDlg])->CheckMode(TRUE);
	*/
}

/////////////////////////////////////////////////////////////////////
// By L.C.G
// ���� Unselect �� Notify ���� ���� 
void CCMThMainDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{

}


CWnd* CCMThMainDlg::GetDlgByID(int nID)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (nID == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}


