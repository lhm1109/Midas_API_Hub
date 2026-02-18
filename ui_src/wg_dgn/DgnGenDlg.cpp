// DgnGenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenDlg.h"

// 인영근 : 추가시 고칠곳 1.
#include "DgnGenMembDlg.h"
#include "DgnGenMembChgDlg.h"
#include "DgnGenLengDlg.h"
#include "DgnGenSplnDlg.h"
#include "DgnGenKFacDlg.h"
#include "DgnGenSlenDlg.h"
#include "DgnGenCmDlg.h"
#include "DgnGenCmDlg_CH.h"
#include "DgnGenFMagDlg.h"
#include "DgnGenBcifDlg.h"
#include "DgnGenPmcrDlg.h"
#include "DgnGenLrdfModDlg.h"
#include "DgnGenMemberTypeDlg.h"
#include "DgnGenSueqDlg.h"// Add by Hong,jiseon 2002.7.24
#include "DgnTowerTypeDlg.h"
#include "DgnGenGreqDlg.h"// Add By RSH 2003.08.25
#include "DgnGenSeismicDlg.h"
#include "DgnGenSeisDgnTypeByMembDlg.h"
#include "DgnGenHcbmDlg.h"
#include "DgnGenEwsdDlg.h"
#include "DgnGenUnderLcomTypeDlg.h"
#include "DgnGenDampSpecifySpecialTypeDlg.h"
#include "DgnGenAssignReinforceSectDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_DBDoc.h"    // For GetDocPoint().
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\DBCodeCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\DBLib.h"
#include "..\mit_frx\MBarBaseDlg.h"
#include "..\wg_base\AutoResSwitch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenDlg dialog

IMPLEMENT_DYNCREATE(CDgnGenDlg, MChildFormView)

CDgnGenDlg::CDgnGenDlg()
	: MChildFormView(CDgnGenDlg::IDD)
{
	//{{AFX_DATA_INIT(CDgnGenDlg)
	//}}AFX_DATA_INIT
	CreateChildDlgs();
}

CDgnGenDlg::~CDgnGenDlg()
{
	DeleteChildDlgs();
}

void CDgnGenDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenDlg)
	DDX_Control(pDX, IDC_DGN_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_DGN_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_DGN_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenDlg, MChildFormView)
	//{{AFX_MSG_MAP(CDgnGenDlg)
	ON_CBN_SELCHANGE(IDC_DGN_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_DGN_TABLE_BTN, OnDgnTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenDlg diagnostics

#ifdef _DEBUG
void CDgnGenDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CDgnGenDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDgnGenDlg Implementation Functions

bool CDgnGenDlg::ShowApplyBtn()
{
	if (m_nCurChildDlg < 0 || m_ChildDlgs.GetSize() - 1 < m_nCurChildDlg)
	{
		return true;
	}
	int nCase = m_DlgIDs.GetAt(m_nCurChildDlg);

	switch (nCase)
	{
	case __TMMODE_DGN_GEN_HCBM__:
		return false;
	default:
		return true;
	}
}

// 인영근 : 추가시 고칠곳 3 : class와 TreeMenuMode에 정의한 ID
void CDgnGenDlg::CreateChildDlgs()
{
#if defined(_TOWER)
	m_nCurChildDlg = -1; 
	m_ChildDlgs.Add(new CDgnTowerTypeDlg);
	m_DlgIDs.Add(__TMMODE_DGN_TWR_MBTP__);
	m_TableIDs.Add(ID_QUERY_DESIGN_TOWER_SLTYPE);

// #elif defined(_CIVIL_JP)
// 	m_nCurChildDlg = -1; 
// 	m_ChildDlgs.Add(new CDgnGenMemberTypeDlg);
// 	m_DlgIDs.Add(__TMMODE_DGN_GEN_MBTP__);
// 	m_TableIDs.Add(ID_QUERY_DGN_GEN_MBTP);

#elif defined(_CIVIL_JP)
	m_nCurChildDlg = -1;

	m_ChildDlgs.Add(new CDgnGenMembDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_MEMB__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CDgnGenMembChgDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_MEMB_CHG__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CDgnGenLengDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_LENG__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_LENG);

	m_ChildDlgs.Add(new CDgnGenKFacDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_KFAC__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_KFAC);

	m_ChildDlgs.Add(new CDgnGenSlenDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_LTSR__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_LTSR);

	m_ChildDlgs.Add(new CDgnGenMemberTypeDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_MBTP__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_MBTP);

#else
	m_nCurChildDlg = -1; 
	
	m_ChildDlgs.Add(new CDgnGenMembDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_MEMB__);
	m_TableIDs.Add(0);
//  m_TableIDs.Add(ID_QUERY_DGN_GEN_MEMB);

	m_ChildDlgs.Add(new CDgnGenMembChgDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_MEMB_CHG__);
	m_TableIDs.Add(0);

#if !defined(_CIVIL)
	m_ChildDlgs.Add(new CDgnGenHcbmDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_HCBM__);
	m_TableIDs.Add(0);
#endif

	m_ChildDlgs.Add(new CDgnGenLengDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_LENG__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_LENG);

#if defined(_CIVIL)
	m_ChildDlgs.Add(new CDgnGenSplnDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_SPLN__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_SPLN);
#endif

	m_ChildDlgs.Add(new CDgnGenKFacDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_KFAC__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_KFAC);

	m_ChildDlgs.Add(new CDgnGenSlenDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_LTSR__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_LTSR);

#ifdef _MGEN_CH
	const CString& strStlCode = CDBLib::GetStlCodeName();
	if(strStlCode == STLCODE_GB50017_17)
		m_ChildDlgs.Add(new CDgnGenCmDlg_CH);
	else
		m_ChildDlgs.Add(new CDgnGenCmDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_CMFT__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_CMFT);
#else
	m_ChildDlgs.Add(new CDgnGenCmDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_CMFT__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_CMFT);
#endif
	
	m_ChildDlgs.Add(new CDgnGenFMagDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_FMAG__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_FMAG);

	m_ChildDlgs.Add(new CDgnGenBCifDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_BCIF__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_BCIF);

#if defined(_MGEN)
	m_ChildDlgs.Add(new CDgnGenPmcrDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_PMCR__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_PMCR);
	m_ChildDlgs.Add(new CDgnGenLrdfModDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_REDU__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_REDU);
	m_ChildDlgs.Add(new CDgnGenSueqDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_SUEQ__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_SUEQ);
	m_ChildDlgs.Add(new CDgnGenGreqDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_GREQ__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_GREQ);
#endif

	m_ChildDlgs.Add(new CDgnGenMemberTypeDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_MBTP__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_MBTP);

#if defined(_MGEN)
	//#if defined(_ORG)
	m_ChildDlgs.Add(new CDgnGenSeismicDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_SEIS__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_SLCM);

	m_ChildDlgs.Add(new CDgnGenSeisDgnTypeByMembDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_SEIS_DGN__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_SDGN);

	m_ChildDlgs.Add(new CDgnGenUnderLcomTypeDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_ULCT__);
	m_TableIDs.Add(ID_QUERY_DGN_GEN_ULCT);
	//#endif
#endif

#ifdef _MGEN_CH
	m_ChildDlgs.Add(new CDgnGenDampSpecifySpecialTypeDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_DSST__);
	m_TableIDs.Add(ID_QUERY_DAMP_CODE_CHECK_TYPE_TB);

	m_ChildDlgs.Add(new CDgnGenAssignReinforceSectDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_ARST__);
	m_TableIDs.Add(ID_QUERY_REINFORCE_SECT_TB);
#endif


	#if defined(_CIVIL)
	m_ChildDlgs.Add(new CDgnGenEwsdDlg);
	m_DlgIDs.Add(__TMMODE_DGN_GEN_EWSD__);
	m_TableIDs.Add(0);//XXXXX m_TableIDs.Add(ID_QUERY_DGN_GEN_EWSD);	
	#endif

#endif
}

void CDgnGenDlg::DeleteChildDlgs()
{
	for(int i=0; i<m_ChildDlgs.GetSize(); i++)	delete m_ChildDlgs[i];
}

// TreeMenu Mode에 해당하는 Child Dialog를 활성화 시킨다. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CDgnGenDlg::ShowChildDlgByID(int nID)
{
	if(nID==__TMMODE_DGN_GEN__)  // 인영근 : 이 파일 Copy시 여기 수정
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

			BOOL bEnable = FALSE;
			
			if(m_TableIDs[i] > 0)	bEnable = TRUE;
			else									bEnable = FALSE;
			
			if(m_TableIDs[i]==ID_QUERY_DGN_GEN_SDGN)
			{
				bEnable = IsEnbleForSdgn(m_TableIDs[i]);
			}
			
			m_wndTableBtn.EnableWindow(bEnable);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDgnGenDlg::IsEnbleForSdgn(int nTableID)
{
	BOOL bEnable = TRUE;
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc==NULL) {ASSERT(0); return FALSE;}
	
	T_DCON_D DconD;
	if(!pDoc->m_pAttrCtrl->GetDcon(DconD)) DconD.Initialize();
	
	if(nTableID==ID_QUERY_DGN_GEN_SDGN)
	{
		bEnable = CDBCodeCtrl::IsConCodeSeisDgnType(DconD.DesignCode) ? TRUE : FALSE;    
		if (!DconD.bSpecialSeismic)    bEnable = FALSE;
	}
	return bEnable;
}

void CDgnGenDlg::ShowCurChildDlg(int nDlgIndex)
{
	if(m_nCurChildDlg==nDlgIndex)	return;

	if(nDlgIndex > m_ChildDlgs.GetSize()-1)
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}

	if(m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize()-1 >= m_nCurChildDlg)
		m_ChildDlgs[m_nCurChildDlg]->End();
	{
		AUTO_SWITICH_RESDLL(wg_dgn);
		m_ChildDlgs[nDlgIndex]->CreateInit(&m_wndPlaceHolder, 0, 0);
	}	
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

	static_cast<mit::frx::MBarBaseDlg*>(GetParent())->SetApplyBtnShowHide(ShowApplyBtn() ? SW_SHOW : SW_HIDE);
}

int CDgnGenDlg::GetHeightFromFont()
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

/////////////////////////////////////////////////////////////////////////////
// CDgnGenDlg message handlers

void CDgnGenDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	// Add by ZINU.('01.1.2).
	Update_InitDataByCode(FALSE);
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);	
}

void CDgnGenDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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

void CDgnGenDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg를 직접 부르면 CBarTreeMenu의 m_nCurMode가 Update
	// 되지 않아 Menu에서 Select해도 해당 Dialog가 뜨지 않는 경우가 발생
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);	
}

int CDgnGenDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CDgnGenDlg::OnDgnTableBtn() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));	
}

// Add by ZINU.('01.1.2).
void CDgnGenDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

// Add by ZINU.('01.1.2).
void CDgnGenDlg::Update_InitDataByCode(BOOL bUpdate)
{
	// Add by ZINU.('02.11.29).	Call only if Code Changed.
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
				case(UR_DCFS_ADD):
				case(UR_DSTL_ADD):
				case(UR_DCON_ADD):
				case(UR_DSRC_ADD):
				case(UR_DCFS_DEL):
				case(UR_DSTL_DEL):
				case(UR_DCON_DEL):
				case(UR_DSRC_DEL):	{bCODE = TRUE; break;}
				default:	break;
			}
		}
		if(!bCODE) return;
	}

	int iSelID = m_ComboMenu.GetCurSel();
	if(iSelID < 0)	iSelID = 0;
	// If added items, must change the number of array in header file.
	// Initialize Data. 
	m_ComboMenu.ResetContent();
#if defined(_TOWER)
	m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DESIGN_TOWER_SLTYPE);
// #elif defined(_CIVIL_JP)
// 	m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_MBTP);
#elif defined(_CIVIL)
	#if defined(_CIVIL_JP)
		m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_MEMB);
		m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_MEMB_CHANGE);
		m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LENG);
		m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_KFAC);
		m_ComboMenuStr[4] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LTSR);
	#else
		m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_MEMB);  
		m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_MEMB_CHANGE);  
		m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LENG);
		m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_SPLN);
		m_ComboMenuStr[4] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_KFAC);
		m_ComboMenuStr[5] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LTSR);
		m_ComboMenuStr[6] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_CMFT);
		m_ComboMenuStr[7] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_FMAG);
		m_ComboMenuStr[8] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_BCIF);
		m_ComboMenuStr[9] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_MBTP);
	#endif
#elif defined(_MGEN_CH)
	m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_MEMB);
	m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_MEMB_CHANGE);
	m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_HCBM);
	m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LENG);
	m_ComboMenuStr[4] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_KFAC);
	m_ComboMenuStr[5] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LTSR);
	m_ComboMenuStr[6] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_CMFT);
	m_ComboMenuStr[7] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_FMAG);
	m_ComboMenuStr[8] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_BCIF);
	m_ComboMenuStr[9] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_PMCR);
	m_ComboMenuStr[10] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_REDU);
	m_ComboMenuStr[11] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_SUEQ);
	m_ComboMenuStr[12] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_GREQ);
	m_ComboMenuStr[13] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_MBTP);
	m_ComboMenuStr[14] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_SEIS);
	m_ComboMenuStr[15] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_SDGN);
	m_ComboMenuStr[16] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_ULCT);
	m_ComboMenuStr[17] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_DSST);
	m_ComboMenuStr[18] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_REINFORCE_MODEL_ASSIGN_SECT);
#else
	m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_MEMB);  
	m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_MEMB_CHANGE);  
	m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_HCBM);  
	m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LENG);
	m_ComboMenuStr[4] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_KFAC);
	m_ComboMenuStr[5] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LTSR);
	m_ComboMenuStr[6] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_CMFT);
	m_ComboMenuStr[7] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_FMAG);
	m_ComboMenuStr[8] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_BCIF);
	m_ComboMenuStr[9] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_PMCR);
	m_ComboMenuStr[10] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_REDU);
	m_ComboMenuStr[11] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_SUEQ); // MQC.11685 빌드 국가에 종속될 필요 없어서 모두 열도록.
	m_ComboMenuStr[12] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_GREQ);
	m_ComboMenuStr[13] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_MBTP);
	m_ComboMenuStr[14] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_SEIS);
	m_ComboMenuStr[15] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_SDGN);
	m_ComboMenuStr[16] = CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_ULCT);
#endif

	// Recalculate Height.
	int wHeight = (sizeof(m_ComboMenuStr)/sizeof(*m_ComboMenuStr)+1) * GetHeightFromFont();
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL,0,0,rectWnd.Width(),wHeight,SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	// Add String at ComboBox.
	for(int i=0; i<sizeof(m_ComboMenuStr)/sizeof(*m_ComboMenuStr); i++)
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}
	// Recalculate Width.
	CClientDC dc(this);
	int nSize = sizeof(m_ComboMenuStr)/sizeof(CString);
	CSize sizeMax, sizeCur;
	sizeMax = dc.GetTextExtent(m_ComboMenuStr[0]);
	for(int k=1; k<nSize; k++)
	{
		sizeCur = dc.GetTextExtent(m_ComboMenuStr[k]);
		if(sizeCur.cx > sizeMax.cx)	sizeMax.cx = sizeCur.cx;
	}
	if(sizeMax.cx > m_ComboMenu.GetDroppedWidth())	m_ComboMenu.SetDroppedWidth(sizeMax.cx);

	m_ComboMenu.SetCurSel(iSelID);
	
	if(m_TableIDs[iSelID]==ID_QUERY_DGN_GEN_SDGN)
	{
		BOOL bEnable = IsEnbleForSdgn(m_TableIDs[iSelID]);
		m_wndTableBtn.EnableWindow(bEnable);
	}
}

void CDgnGenDlg::OnTmExecute()
{
	if ( m_nCurChildDlg < m_ChildDlgs.GetSize() )
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CDgnGenDlg::OnTmClose()
{
	if ( CDBDoc::GetDocPoint() )
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM);

	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}