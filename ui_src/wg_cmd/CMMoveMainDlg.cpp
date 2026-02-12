// CMMoveMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMoveMainDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\I_GENStateDefine.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "CMSinfDlg.h"
#include "CMMlspDlg.h"
#include "CMMlsrDlg.h"
#include "CMMlttDlg.h"
#include "CMAddImpfDlg.h"
#include "CMMlspAASHTOLRFDDlg.h"
#include "CMMvhlItemEuroDynfDlg.h"
#include "CMJapanRailmpactFactorDlg.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\mit_frx\MBarBaseDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int ____bNotifyComboMv = TRUE;
/////////////////////////////////////////////////////////////////////////////
// CCMMoveMainDlg dialog

#define CFormView CCMDlgBarBase

IMPLEMENT_DYNCREATE(CCMMoveMainDlg, CFormView)

CCMMoveMainDlg::CCMMoveMainDlg(CWnd* pParent /*=NULL*/)
	: CFormView(CCMMoveMainDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMMoveMainDlg)	 
	//}}AFX_DATA_INIT

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) { ASSERT(0); return;}
	T_MVCD_D MvcdD;
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) MvcdD.Initialize();

	BOOL bExistDyfg = pDoc->m_pAttrCtrl2->ExistDyfg();

	// �ڵ忡 ���� �ٲ� 
	CArray<const TCHAR*, const TCHAR*> ComboStrs;
	if(MvcdD.nCodeType == D_MOVE_CODE_AASHTO_STAN ||
		 MvcdD.nCodeType == D_MOVE_CODE_KOREA       ||
		 MvcdD.nCodeType == D_MOVE_CODE_TAIWAN)
	{
		ComboStrs.Add(_LS(IDS_WG_CMD__ADDD__Plate_Element_for_Influence_Surfa));  
		ComboStrs.Add(_LS(IDS_WG_CMD__ADDD__Lane_Supports));
		if (MvcdD.nCodeType == D_MOVE_CODE_KOREA)
		{
#if defined(_CIVIL)
			ComboStrs.Add(_LS(IDS_CMD_ADD_IMPACT_FACTOR));     // MNET:2966-JHLEE-20070921
#endif
		}
	}
	else if(MvcdD.nCodeType == D_MOVE_CODE_AASHTO_LRFD ||
					MvcdD.nCodeType == D_MOVE_CODE_PENDOT)
	{
		ComboStrs.Add(_LS(IDS_WG_CMD__ADDD__Plate_Element_for_Influence_Surfa));  
		ComboStrs.Add(_LS(IDS_WG_CMD__ADDD__Lane_Supports)); 
		ComboStrs.Add(_LS(IDS_WG_CMD__ADDD__Lane_Supports_Reactions));
		// V955
		{
			ComboStrs.Add(_LS(IDS_WG_CMD__ADDD__Lane_Supports_Two_Trucks));
		}
	}
	else if(MvcdD.nCodeType == D_MOVE_CODE_CHINA ||
					MvcdD.nCodeType == D_MOVE_CODE_CANADA ||
					MvcdD.nCodeType == D_MOVE_CODE_KOREA_LRFD_2011)
	{
		ComboStrs.Add(_LS(IDS_WG_CMD__ADDD__Plate_Element_for_Influence_Surfa));  
	}
	else if(MvcdD.nCodeType == D_MOVE_CODE_EURO_BS)
	{
		ComboStrs.Add(_LS(IDS_WG_CMD__ADDD__Plate_Element_for_Influence_Surfa));
		if(bExistDyfg) ComboStrs.Add(_LS(IDS_CMD_MVHL_EURO_BS_DYNAMIC_FACTOR));  
	}
	else if(MvcdD.nCodeType == D_MOVE_CODE_BS || 
		      MvcdD.nCodeType == D_MOVE_CODE_SOUTH_AFRICA)
	{
		ComboStrs.Add(_LS(IDS_WG_CMD__ADDD__Plate_Element_for_Influence_Surfa));
	}
	else if(MvcdD.nCodeType == D_MOVE_CODE_RUSSIA || 
		      MvcdD.nCodeType == D_MOVE_CODE_AUSTRALIA ||
					MvcdD.nCodeType == D_MOVE_CODE_POLAND ||
		      MvcdD.nCodeType == D_MOVE_CODE_NEWZEALAND ||
					MvcdD.nCodeType == D_MOVE_CODE_BRAZIL)
	{
		// Nothing
	}
	else if (MvcdD.nCodeType == D_MOVE_CODE_JAPAN_RAIL)
	{
		ComboStrs.Add(_LS(IDS_CMD_GRID_TITLE_IMPACT_FACTOR));	
	}
	else ASSERT(0); 

	m_ComboMenuStr.RemoveAll();
	int nStrs = ComboStrs.GetSize();
	for(int i = 0 ; i < nStrs ; i++)
	{
		m_ComboMenuStr.Add((TCHAR*) ComboStrs[i]);
	}

	CreateChildDlgs();
}

CCMMoveMainDlg::~CCMMoveMainDlg()
{
	DeleteChildDlgs();
}


void CCMMoveMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMoveMainDlg)
	DDX_Control(pDX, IDC_CMD_TABLE_BUTTON, m_wndTableButton);
	DDX_Control(pDX, IDC_DBAR_FUNC_COMBO, m_ComboMenu);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndPlaceHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMoveMainDlg, CFormView)
	//{{AFX_MSG_MAP(CCMMoveMainDlg)
	ON_BN_CLICKED(IDC_CMD_TABLE_BUTTON, OnCmdTableButton)
	ON_CBN_SELCHANGE(IDC_DBAR_FUNC_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMoveMainDlg message handlers

bool CCMMoveMainDlg::ShowApplyBtn()
{
	if ( m_nCurChildDlg < 0 || m_ChildDlgs.GetSize() - 1 < m_nCurChildDlg )
	{
		return true;
	}

	switch ( m_DlgIDs.GetAt(m_nCurChildDlg) )
	{
	case __TMMODE_CMD_MLSP__:
	{
		if(dynamic_cast<CCMMlspAASHTODlg*>(m_ChildDlgs[m_nCurChildDlg]) != nullptr)
			return false;
		return true;
	}
	default:
		return true;
	}
}

void CCMMoveMainDlg::CreateChildDlgs()
{ 
	m_nCurChildDlg = -1; 
	
//   m_ChildDlgs.Add(new CCMSinfDlg);
//   m_DlgIDs.Add(__TMMODE_CMD_SINF__);
//   m_TableIDs.Add(0);  

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) { ASSERT(0); return;}
	T_MVCD_D MvcdD;
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) MvcdD.Initialize();

	BOOL bExistDyfg = pDoc->m_pAttrCtrl2->ExistDyfg();

	if(MvcdD.nCodeType == D_MOVE_CODE_AASHTO_STAN ||
		 MvcdD.nCodeType == D_MOVE_CODE_KOREA       ||
		 MvcdD.nCodeType == D_MOVE_CODE_TAIWAN)
	{
		m_ChildDlgs.Add(new CCMSinfDlg);
		m_DlgIDs.Add(__TMMODE_CMD_SINF__);
		m_TableIDs.Add(0);  

		m_ChildDlgs.Add(new CCMMlspDlg);  
		m_DlgIDs.Add(__TMMODE_CMD_MLSP__);
		m_TableIDs.Add(0);

		if (MvcdD.nCodeType == D_MOVE_CODE_KOREA)
		{
#if defined(_CIVIL)
			m_ChildDlgs.Add(new CMAddImpfDlg);
			m_DlgIDs.Add(__TMMODE_CMD_ADD_IMPF__);
			m_TableIDs.Add(ID_STRUCT_MOV_ADDIMPF_TB);          
#endif
		}
	}
	else if(MvcdD.nCodeType == D_MOVE_CODE_AASHTO_LRFD ||
					MvcdD.nCodeType == D_MOVE_CODE_PENDOT)
	{
		m_ChildDlgs.Add(new CCMSinfDlg);
		m_DlgIDs.Add(__TMMODE_CMD_SINF__);
		m_TableIDs.Add(0);  

		if (MvcdD.nCodeType == D_MOVE_CODE_AASHTO_LRFD)
		{ 
			m_ChildDlgs.Add(new CCMMlspAASHTODlg);
		}
		else
		{
			m_ChildDlgs.Add(new CCMMlspDlg);  
		}
		m_DlgIDs.Add(__TMMODE_CMD_MLSP__);
		m_TableIDs.Add(0);

		m_ChildDlgs.Add(new CCMMlsrDlg);
		m_DlgIDs.Add(__TMMODE_CMD_MLSR__);
		m_TableIDs.Add(0);

		//if (MvcdD.nCodeType == D_MOVE_CODE_AASHTO_LRFD)
		{
			m_ChildDlgs.Add(new CCMMlttDlg);
			m_DlgIDs.Add(__TMMODE_CMD_MLTT__);
			m_TableIDs.Add(0);
		}
	}
	else if(MvcdD.nCodeType == D_MOVE_CODE_CHINA ||
					MvcdD.nCodeType == D_MOVE_CODE_CANADA ||
					MvcdD.nCodeType == D_MOVE_CODE_KOREA_LRFD_2011 ||
		      MvcdD.nCodeType == D_MOVE_CODE_NEWZEALAND ||
					MvcdD.nCodeType == D_MOVE_CODE_BRAZIL)
	{
		m_ChildDlgs.Add(new CCMSinfDlg);
		m_DlgIDs.Add(__TMMODE_CMD_SINF__);
		m_TableIDs.Add(0);  
	}
	else if(MvcdD.nCodeType == D_MOVE_CODE_EURO_BS)
	{
		m_ChildDlgs.Add(new CCMSinfDlg);
		m_DlgIDs.Add(__TMMODE_CMD_SINF__);
		m_TableIDs.Add(0);  

		if(bExistDyfg)
		{
			m_ChildDlgs.Add(new CCMMvhlItemEuroDynfDlg);
			m_DlgIDs.Add(__TMMODE_CMD_DYNF__);
			m_TableIDs.Add(ID_STRUCT_MOV_DYNF_TB);          
		}
	}
	else if(MvcdD.nCodeType == D_MOVE_CODE_BS ||
		      MvcdD.nCodeType == D_MOVE_CODE_SOUTH_AFRICA)
	{
		m_ChildDlgs.Add(new CCMSinfDlg);
		m_DlgIDs.Add(__TMMODE_CMD_SINF__);
		m_TableIDs.Add(0);  
	}
	else if(MvcdD.nCodeType == D_MOVE_CODE_RUSSIA ||
		      MvcdD.nCodeType == D_MOVE_CODE_AUSTRALIA ||
					MvcdD.nCodeType == D_MOVE_CODE_POLAND)
	{
		// Nothing
	}
	else if (MvcdD.nCodeType == D_MOVE_CODE_JAPAN_RAIL)
	{
		m_ChildDlgs.Add(new CCMJapanRailmpactFactorDlg);
		m_DlgIDs.Add(__TMMODE_CMD_JIMP__);
		m_TableIDs.Add(ID_QUERY_STRUCT_JIMP);
	}
	else ASSERT(0); 

//   if (MvcdD.nCodeType == D_MOVE_CODE_AASHTO_LRFD)   // ���� AASHTO LRFD�� �����Ѵٸ� 
//   { 
//     m_ChildDlgs.Add(new CCMMlspAASHTODlg); 
//   }
//   else                                        
//   { 
//     m_ChildDlgs.Add(new CCMMlspDlg);                // �׿ܿ��� CCMMlspDlg�� �ٿ�
//   }        
//   m_DlgIDs.Add(__TMMODE_CMD_MLSP__);
//   m_TableIDs.Add(0);
// 
//   if (MvcdD.nCodeType == D_MOVE_CODE_KOREA)     // ���� Korea�� �����Ѵٸ� Addtional Impact Factor
//   {
//     m_ChildDlgs.Add(new CMAddImpfDlg);
//     m_DlgIDs.Add(__TMMODE_CMD_ADD_IMPF__);
//     m_TableIDs.Add(ID_STRUCT_MOV_ADDIMPF_TB);
//   }
//   else                                          // �ƴϸ� CMMlsrDlg�� �ٿ���
//   {
//     m_ChildDlgs.Add(new CCMMlsrDlg);
//     m_DlgIDs.Add(__TMMODE_CMD_MLSR__);
//     m_TableIDs.Add(0);
//   } 
}

void CCMMoveMainDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}

// TreeMenu Mode�� �ش��ϴ� Child Dialog�� Ȱ��ȭ ��Ų��. 
BOOL CCMMoveMainDlg::ShowChildDlgByID(int nID)
{
	if(nID == __TMMODE_CMD_MVROOT__)
	{
		____bNotifyComboMv = FALSE;    
		m_ComboMenu.SetCurSel(0);
		____bNotifyComboMv = TRUE;    
		ShowCurChildDlg(0);  
		
		return TRUE;
	}

	int nIDCount = m_DlgIDs.GetSize();
	for(int i = 0 ; i < nIDCount ; i++)
	{
		if(nID == m_DlgIDs[i])
		{
			____bNotifyComboMv = FALSE;    
			m_ComboMenu.SetCurSel(i);
			____bNotifyComboMv = TRUE;    
			ShowCurChildDlg(i);  
			if (m_TableIDs[i] > 0) m_wndTableButton.EnableWindow(TRUE);
			else m_wndTableButton.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CCMMoveMainDlg::ShowCurChildDlg(int nDlgIndex)
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
	static_cast<mit::frx::MBarBaseDlg*>(GetParent())->SetApplyBtnShowHide(ShowApplyBtn() ? SW_SHOW : SW_HIDE);
}

void CCMMoveMainDlg::LayoutChildDialog()
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

int CCMMoveMainDlg::_GetHeightFromFont()
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

void CCMMoveMainDlg::InitComboMenu()
{
	/*
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) { ASSERT(0); return;}
	T_MVCD_D MvcdD;
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) MvcdD.Initialize();

	int nComboNum = 0;
	if (MvcdD.nCodeType == D_MOVE_CODE_AASHTO_STAN) nComboNum = 2;
	else if (MvcdD.nCodeType == D_MOVE_CODE_AASHTO_LRFD) nComboNum = 3;
	else if (MvcdD.nCodeType == D_MOVE_CODE_CHINA) nComboNum = 1;
	else nComboNum = 0;
	*/
	
	int nComboNum = m_ComboMenuStr.GetSize();

	int wHeight;
	wHeight = (nComboNum+1) * _GetHeightFromFont();
	
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL, 0, 0,rectWnd.Width(),wHeight,
				         SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	for( int i=0;  i<nComboNum; ++i )
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}
	m_ComboMenu.SetCurSel(0);

	AdjustComboListBox();

	/*
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

	AdjustComboListBox();
	*/
}

void CCMMoveMainDlg::AdjustComboListBox()
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = m_ComboMenu.GetDC();
	for (int i=0;i < m_ComboMenu.GetCount();i++)
	{
		m_ComboMenu.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	m_ComboMenu.ReleaseDC(pDC);

	if (m_ComboMenu.GetDroppedWidth() < dx)
	{
		m_ComboMenu.SetDroppedWidth(dx);
		ASSERT(m_ComboMenu.GetDroppedWidth() == dx);
	}
}

void CCMMoveMainDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}  


/////////////////////////////////////////////////////////////////////////////
// CCMMoveMainDlg message handlers
void CCMMoveMainDlg::OnInitialUpdate() 
{
	CFormViewInBar::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CCMMoveMainDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{	
	CFormViewInBar::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CCMMoveMainDlg::OnComboMenuSelect() 
{
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg�� ���� �θ��� CBarTreeMenu�� m_nCurMode�� Update
	// ���� �ʾ� Menu���� Select�ص� �ش� Dialog�� ���� �ʴ� ��찡 �߻�
	//if(____bNotifyComboMv)
	//{
		int nCurI = m_ComboMenu.GetCurSel();
		CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//}
	//ShowCurChildDlg(nCurI);	
}

int CCMMoveMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormViewInBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CCMMoveMainDlg::OnCmdTableButton() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));				
}

/////////////////////////////////////////////////////////////////////
// By L.C.G
void CCMMoveMainDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
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
void CCMMoveMainDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{

}


CWnd* CCMMoveMainDlg::GetDlgByID(int nID)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (nID == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}


void CCMMoveMainDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CCMMoveMainDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CCMMoveMainDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		//UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_POST_STEP_CHANGED:
		CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU_MENU__);
		break;
	default:
		break;
	}
}

void CCMMoveMainDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	BOOL bMvcdChanged = FALSE;
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
		case(UR_MVCD_ADD):
		case(UR_MVCD_DEL):
			bMvcdChanged = TRUE;
			break;
		case(UR_DYFG_ADD):
		case(UR_DYFG_DEL):
			bMvcdChanged = TRUE; // Dynamic Factor ���� Data �� ����Ǿ����� ��ȭ���� �ݵ��� ���� 
			break;
		default:
			break;
		}
	} // end of while

	if (bMvcdChanged) 
	{
		// �ڵ尡 �ٲ�� ������ �ݾƹ�����.
		CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU_MENU__);
	}
}
