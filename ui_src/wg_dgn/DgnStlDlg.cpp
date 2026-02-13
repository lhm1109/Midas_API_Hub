// DgnStlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlDlg.h"

// �ο��� : �߰��� ��ĥ�� 1.
#include "DgnStlServiceLimitDlg.h"
#include "DgnStlCbDlg.h"
#include "DgnStlCvDlg.h"
#include "DgnStlStrDlg.h"
//#include "DgnStlUcfDlg.h"
#include "DgnStlStifDlg.h"
#include "DgnStlSLRSDlg.h"
#include "DgnStlSMSPDlg.h"
#include "DgnStlSPSCDlg.h"
#include "DgnStlSPLBDlg.h"
#include "DgnStlSPCSDlg.h"
#include "DgnStlSPPBDlg.h"
#include "DgnStlSPLSDlg.h"

#ifdef _MGEN_CH
#include "DgnStlPhibFacDlg.h"
#include "DgnStlSectionTypeDlg.h"
#include "DgnStlBendModulusRtoDlg.h"
#include "DgnStlSideswayDlg.h"
#include "DgnStlCrossSectPlasFactorDlg.h"
#include "DgnStlPfmcDlg.h"
#include "DgnStlPfcmDlg.h"
#include "DgnStlPfdlDlg.h"
#include "DgnStlPfmpDlg.h"
#include "DgnStlPfnpDlg.h"
#include "DgnStlInitDeflectionDlg.h"
#include "DgnStlAppraisalImportantElemDlg.h"
#include "DgnStlAppraisalSecurityDlg.h"
#include "DgnStlAppraisalUsabilityDlg.h"
#endif

#include "DgnStlPositionJPDlg.h"
#include "DgnStlBoltDeductionDlg.h"
#include "DgnStlStusJPDlg.h"
#include "DgnStlStreJPDlg.h"
#include "DgnStl_SP14SeisMembParams_Dlg.h"
#include "DgnStl_SP16SafetyMembParams_Dlg.h"
#include "DgnStl_SP16CombinedSectParams_Dlg.h"


#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\AutoResSwitch.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_DBDoc.h"    // For GetDocPoint().
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\DBlib.h"
#include "..\wg_db\DBCodeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlDlg

IMPLEMENT_DYNCREATE(CDgnStlDlg, MChildFormView)

CDgnStlDlg::CDgnStlDlg()
	: MChildFormView(CDgnStlDlg::IDD)
{
	//{{AFX_DATA_INIT(CDgnStlDlg)
	//}}AFX_DATA_INIT
	CreateChildDlgs();
}

CDgnStlDlg::~CDgnStlDlg()
{
	DeleteChildDlgs();
}

void CDgnStlDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlDlg)
	DDX_Control(pDX, IDC_DGN_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_DGN_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_DGN_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlDlg, MChildFormView)
	//{{AFX_MSG_MAP(CDgnStlDlg)
	ON_CBN_SELCHANGE(IDC_DGN_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_DGN_TABLE_BTN, OnDgnTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlDlg diagnostics

#ifdef _DEBUG
void CDgnStlDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CDgnStlDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDgnStlDlg Implementation Functions


void CDgnStlDlg::CreateChildDlgs()
{

	CString strStlCode = CDBLib::GetStlCodeName();

	m_nCurChildDlg = -1; 

	if (strStlCode.CompareNoCase(STLCODE_JROAD_H24) == 0 || strStlCode.CompareNoCase(STLCODE_JROAD_H14) == 0 ||
		strStlCode.CompareNoCase(STLCODE_JROAD_H29) == 0 || strStlCode.CompareNoCase(STLCODE_JROAD_R07) == 0)
	{
		m_ChildDlgs.Add(new CDgnStlStusJPDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_STUS__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_JP_STUS);

		m_ChildDlgs.Add(new CDgnStlStreJPDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_STRE__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_JP_STRE);

		m_ChildDlgs.Add(new CDgnStlBoltDeductionDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_STBD__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_STBD);

		m_ChildDlgs.Add(new CDgnStlPositionJPDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_JP_POSITION__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_JP_STDP);
	}
	else
	{
		// Coded by Seungjun MNet:No.2393 ('20061011)
		m_ChildDlgs.Add(new CDgnStlServiceLimitDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_SERV__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_SERV);

		m_ChildDlgs.Add(new CDgnStlCbDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_CBFT__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_CBFT);

		m_ChildDlgs.Add(new CDgnStlCvDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_CVFT__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_CVFT);

		m_ChildDlgs.Add(new CDgnStlStrDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_DALW__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_DALW);

		//	m_ChildDlgs.Add(new CDgnStlUcfDlg);
		//	m_DlgIDs.Add(__TMMODE_DGN_STL_UCFA__);
		//  m_TableIDs.Add(ID_QUERY_DESIGN_STEEL_UCFA);

#if defined(_MGEN)   
		m_ChildDlgs.Add(new CDgnStlSLRSDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_SLRS__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_SLRS);
#endif

		m_ChildDlgs.Add(new CDgnStlStifDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_STFN__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_STFN);

		m_ChildDlgs.Add(new CDgnStlSMSPDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_SMSP__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_SMSP);
		
#if defined(_MGEN_CH)
		//add by maxiao 2015-10-12
		m_ChildDlgs.Add(new CDgnStlPhibFacDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_PHIB__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_PHIB);

		m_ChildDlgs.Add(new CDgnStlPfmcDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_PFMC__);
		m_TableIDs.Add(ID_QUERY_DGN_PERFOR_MEMBER_COEFFICIENT);

		m_ChildDlgs.Add(new CDgnStlPfcmDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_PFCM__);
		m_TableIDs.Add(ID_QUERY_DGN_PERFOR_MIN_COEFFICEN);

		m_ChildDlgs.Add(new CDgnStlPfdlDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_PFDL__);
		m_TableIDs.Add(ID_QUERY_DGN_PERFOR_DUCTILITY_LEVEL);

		m_ChildDlgs.Add(new CDgnStlPfmpDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_PFMP__);
		m_TableIDs.Add(ID_QUERY_DGN_PERFOR_MEMBER_PROPERTIES);

		m_ChildDlgs.Add(new CDgnStlPfnpDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_PFNP__);
		m_TableIDs.Add(ID_QUERY_DGN_PERFOR_NON_PLASTIC_FORCE_COEFFICIENT);

		//Add by tss 2020/02/12
		m_ChildDlgs.Add(new CDgnStlSectionTypeDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_SETY__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_SECTION_TYPE);

		//Add by tss 2021/01/12
		m_ChildDlgs.Add(new CDgnStlBendModulusRtoDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_BMRO__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_BEND_MODULUS_RTO);

		//Add by tss 2024/02/29
		m_ChildDlgs.Add(new CDgnStlCrossSectPlasFactorDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_SPDF__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_SPDF);

		//add by maxiao 2015-10-20
		m_ChildDlgs.Add(new CDgnStlSideswayDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_SDSW__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_SDSW);

		m_ChildDlgs.Add(new CDgnStlInitDeflectionDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_INDF__);
		m_TableIDs.Add(ID_QUERY_REINFORCE_DGN_STL_INDF);

		//Add by tss 2024/02/29
		m_ChildDlgs.Add(new CDgnStlAppraisalImportantElemDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_IMEL__);
		m_TableIDs.Add(ID_QUERY_DGN_STL_APPRAISAL_IMEL);

		m_ChildDlgs.Add(new CDgnStlAppraisalUsabilityDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_DSAU__);
		m_TableIDs.Add(ID_QUERY_REINFORCE_EVALUATE_MEMB_USABILITY);

		m_ChildDlgs.Add(new CDgnStlAppraisalSecurityDlg);
		m_DlgIDs.Add(__TMMODE_DGN_STL_DSAS__);
		m_TableIDs.Add(ID_QUERY_REINFORCE_EVALUATE_MEMB_SECURITY);
#endif
		if(strStlCode.CompareNoCase(STLCODE_SP_16_13330_2017) == 0)
		{
			m_ChildDlgs.Add(new CDgnStlSPSCDlg);
			m_DlgIDs.Add(__TMMODE_DGN_STL_SPSC__);
			m_TableIDs.Add(ID_QUERY_DGN_STL_SPSC);
			//splb
			m_ChildDlgs.Add(new CDgnStlSPLBDlg);
			m_DlgIDs.Add(__TMMODE_DGN_STL_SPLB__);
			m_TableIDs.Add(ID_QUERY_DGN_STL_SPLB);
			//spcs
			m_ChildDlgs.Add(new CDgnStlSPCSDlg);
			m_DlgIDs.Add(__TMMODE_DGN_STL_SPCS__);
			m_TableIDs.Add(ID_QUERY_DGN_STL_SPCS);
			//spls
			m_ChildDlgs.Add(new CDgnStlSPLSDlg);
			m_DlgIDs.Add(__TMMODE_DGN_STL_SPLS__);
			m_TableIDs.Add(ID_QUERY_DGN_STL_SPLS);
			//sppb
			m_ChildDlgs.Add(new CDgnStlSPPBDlg);
			m_DlgIDs.Add(__TMMODE_DGN_STL_SPPB__);
			m_TableIDs.Add(ID_QUERY_DGN_STL_SPPB);

			m_ChildDlgs.Add(new CDgnStl_SP14SeisMembParams_Dlg(CDBDoc::GetDocPoint()));
			m_DlgIDs.Add(__TMMODE_DGN_STL_SP14SeisMembParams__);
			m_TableIDs.Add(ID_QUERY_DGN_STL_SP14_SEISMIC_ACCOUNTING);

			m_ChildDlgs.Add(new CDgnStl_SP16SafetyMembParams_Dlg(CDBDoc::GetDocPoint()));
			m_DlgIDs.Add(__TMMODE_DGN_STL_SP16SafetyMembParams__);
			m_TableIDs.Add(ID_QUERY_DGN_STL_SP16SafetyMembParams);

			m_ChildDlgs.Add(new CDgnStl_SP16CombinedSectParams_Dlg(CDBDoc::GetDocPoint()));
			m_DlgIDs.Add(__TMMODE_DGN_STL_SP16CombinedSectParams__);
		}
	}

}

void CDgnStlDlg::DeleteChildDlgs()
{
	for(int i=0; i<m_ChildDlgs.GetSize(); i++)	delete m_ChildDlgs[i];
}

// TreeMenu Mode�� �ش��ϴ� Child Dialog�� Ȱ��ȭ ��Ų��. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CDgnStlDlg::ShowChildDlgByID(int nID)
{
	if(nID==__TMMODE_DGN_STL__)  // �ο��� : �� ���� Copy�� ���� ����
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

void CDgnStlDlg::ShowCurChildDlg(int nDlgIndex)
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
	// Scroll Size�� �����Ѵ�. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();

	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();

	ScrSize += TSize;

	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);
}

int CDgnStlDlg::GetHeightFromFont()
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

void CDgnStlDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	// Add by ZINU.('01.1.2).
	Update_InitDataByCode(FALSE);
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);	
}

void CDgnStlDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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

void CDgnStlDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg�� ���� �θ��� CBarTreeMenu�� m_nCurMode�� Update
	// ���� �ʾ� Menu���� Select�ص� �ش� Dialog�� ���� �ʴ� ��찡 �߻�
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);	
}

int CDgnStlDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CDgnStlDlg::OnDgnTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));	
}

// Add by ZINU.('01.1.2).
void CDgnStlDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			//ASSERT(FALSE)->ASSERT(TRUE)�� �ӽú��� ('2000.2.10)
			ASSERT(TRUE);
	}
}

// Add by ZINU.('01.1.2).
void CDgnStlDlg::Update_InitDataByCode(BOOL bUpdate)
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
				case(UR_DSTL_ADD):
				case(UR_DSTL_DEL):	{bCODE = TRUE; break;}
				default:	break;
			}
		}
		if(!bCODE) return;
	}

	CString strStlCode = CDBLib::GetStlCodeName();
	int iSelID = m_ComboMenu.GetCurSel();
	// Initialize Data.
	m_ComboMenu.ResetContent();
	m_ComboMenuStr.RemoveAll();
#if defined(_MGEN)
	if(strStlCode.CompareNoCase(STLCODE_SP_16_13330_2017) == 0)
	{
#ifdef  _CH
		m_ComboMenuStr.SetSize(15 + 14);
#else
		m_ComboMenuStr.SetSize(15);
#endif
		m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SERV);
		m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_CBFT);
		m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_CVFT);
		m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_DALW);
		m_ComboMenuStr[4] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SLRS);
		m_ComboMenuStr[5] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_STFN);
		m_ComboMenuStr[6] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SMSP);
		m_ComboMenuStr[7] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SPSC);
		m_ComboMenuStr[8] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SPLB);
		m_ComboMenuStr[9] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SPCS);
		m_ComboMenuStr[10] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SPLS);
		m_ComboMenuStr[11] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SPPB);
		m_ComboMenuStr[12] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SP14_SEISMIC_ACCOUNTING);
		m_ComboMenuStr[13] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SP16SafetyMembParams);
		m_ComboMenuStr[14] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SP16CombinedSectParams);
	}
	else
	{
#ifdef  _CH
		m_ComboMenuStr.SetSize(7+14);
#else
		m_ComboMenuStr.SetSize(7);
#endif
		m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SERV);
		m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_CBFT);
		m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_CVFT);
		m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_DALW);
		m_ComboMenuStr[4] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SLRS);
		m_ComboMenuStr[5] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_STFN);
		m_ComboMenuStr[6] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SMSP);
#if defined(_CH)
		int nTotalIndex = 7;
		m_ComboMenuStr[nTotalIndex] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_PHIB);
		m_ComboMenuStr[nTotalIndex + 1] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_PERFOR_MEMBER_COEFFICIENT);
		m_ComboMenuStr[nTotalIndex + 2] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_PERFOR_MIN_COEFFICEN);
		m_ComboMenuStr[nTotalIndex + 3] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_PERFOR_DUCTILITY_LEVEL);
		m_ComboMenuStr[nTotalIndex + 4] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_PERFOR_MEMBER_PROPERTIES);
		m_ComboMenuStr[nTotalIndex + 5] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_PERFOR_NON_PLASTIC_FORCE_COEFFICIENT);
		m_ComboMenuStr[nTotalIndex + 6] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SECTION_TYPE);
		m_ComboMenuStr[nTotalIndex + 7] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_BEND_MODULUS_RTO);
		m_ComboMenuStr[nTotalIndex + 8] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SPDF);
		m_ComboMenuStr[nTotalIndex + 9] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SDSW);
		m_ComboMenuStr[nTotalIndex + 10] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_REINFORCE_INIT_DEFLECTION);
		m_ComboMenuStr[nTotalIndex + 11] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_APPRAISAL_IMEL);
		m_ComboMenuStr[nTotalIndex + 12] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_REINFORCE_EVALUATE_MEMB_USABILITY);
		m_ComboMenuStr[nTotalIndex + 13] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_REINFORCE_EVALUATE_MEMB_SECURITY);
#else

#endif
	}
#else
	if (strStlCode.CompareNoCase(STLCODE_JROAD_H24) == 0 || strStlCode.CompareNoCase(STLCODE_JROAD_H14) == 0 ||
		strStlCode.CompareNoCase(STLCODE_JROAD_H29) == 0 || strStlCode.CompareNoCase(STLCODE_JROAD_R07) == 0)
	{
		m_ComboMenuStr.SetSize(4);
		m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_JP_STUS);
		m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_JP_STRE);
		m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_STBD);
		m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_JP_STDP);
	}
	else
	{
		m_ComboMenuStr.SetSize(6);
		m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SERV);
		m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_CBFT);
		m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_CVFT);
		m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_DALW);
		m_ComboMenuStr[4] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_STFN);
		m_ComboMenuStr[5] = CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SMSP);
	}
#endif
	// Recalculate Height.
	int wHeight = (m_ComboMenuStr.GetSize() + 1) * GetHeightFromFont();
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL,0,0,rectWnd.Width(),wHeight,SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
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

// 	if (iSelID < 0 || iSelID >= nCurComMenuNum) 
// 	{
// 		m_ComboMenu.SetCurSel(0); m_ComboMenu.SetWindowText(m_ComboMenuStr[0]); 
// 	}
// 	else 
// 	{
// 		m_ComboMenu.SetCurSel(iSelID); m_ComboMenu.SetWindowText(m_ComboMenuStr[iSelID]); 
// 	}

	if(iSelID < 0)	m_ComboMenu.SetCurSel(0);
	else						m_ComboMenu.SetCurSel(iSelID);
}

void CDgnStlDlg::OnTmExecute()
{
	if ( m_nCurChildDlg < m_ChildDlgs.GetSize() )
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CDgnStlDlg::OnTmClose()
{
	if ( CDBDoc::GetDocPoint() )
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}