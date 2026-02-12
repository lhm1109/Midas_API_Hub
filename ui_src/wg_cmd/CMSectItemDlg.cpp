// CMSectItemDlg.cpp : implementation file
//
// 단면 다이얼로그는 CCMSectItemDlg 다이얼로그가 Section Type별로 
// CCMSectItemBase 클래스를 상속받은 CCMSectItemXXX 다이얼로그들을 Child Dialog Class로 가진다. 
// 각 단면타입은 동등위치에 있으며 PSC 단면의 경우, PSC 단면 타입에 따라
// CCMSectItemPSC 단면클래스가 각각의 PSC Child Dialog 클래스를 가지는 형태이다.
//
//   CCMSectItemDlg
//          |                     CCMSectItemBase
//          |                           |
//          |            +--------------+---------------+--------
//          |            |              |               |
//          +----> CCMSectItemCom  CCMSectItemReg CCMSectItemPSC   ...         
//                                                      |                          CCMSectItemPSCBase
//                                                      |                                  |
//                                                      |               +------------------+------------------+---------
//                                                      |               |                  |                  |
//                                                      +-----> CCMSectItemPSCNormal  CCMSectItemPSCMid  CCMSectItemPSCTee   ....
//


#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemDlg.h"

#include "..\wg_base\wg_base_DlgChild.h"

// *^^* Section Type 추가, 변경시 여기 수정 
#include "CMSectItemReg.h"
#include "CMSectItemVal.h"
#include "CMSectItemSrc.h"
#include "CMSectItemCom.h"
#include "CMSectItemTap.h"
#include "CMSectItemCfm.h"
#include "CMSectItemCon.h"
#include "CMSectItemPSC.h"
#include "CMSectItemStlGirder.h"

#include "..\mit_frx\global_Opt4Perf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define PSC_TAB 4
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemDlg dialog
#define CDialog CDialogMove

CCMSectItemDlg::CCMSectItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_bModify = FALSE;
	m_bDesign = FALSE;  // Analysis Section(FALSE) or Design Section(TRUE)
	m_bVirtual = FALSE;
	m_bWorkTab = false;

	m_bDestory = FALSE;
	m_pVoidData = NULL; //Hong,js 03/03/12
	//{{AFX_DATA_INIT(CCMSectItemDlg)
	//}}AFX_DATA_INIT

	m_nActiveTab = 0;
}


void CCMSectItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemDlg)
	DDX_Control(pDX, IDC_CMD_SP_ID_BTN_CALC, m_wndCalcBtn);
	DDX_Control(pDX, IDC_CMD_SP_ID_BTN_APPLY, m_wndBtnApply);
	DDX_Control(pDX, IDC_CMD_TAB_CTRL, m_wndTabCtrl);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// Interface
/////////////////////////////////////////////////////////////////////////////
void CCMSectItemDlg::SetModifyData(T_SECT_K Key, T_SECT_D &Data)
{
	m_Key = Key;
	m_Data = Data;
	m_bModify = TRUE;
}

void CCMSectItemDlg::SetSectionID(UINT key)
{
	m_bWorkTab = true;
	m_FixID = key;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
// *^^* Section Type 추가, 변경시 여기 수정 
BOOL CCMSectItemDlg::SetDialogPageList()
{
	// 4.2변경사항
	// SECTION 수정시 DB/USER 탭은 항상 같이 뜬다.
	CArray<int, int> aTgtPage;
	if (m_bModify)  // Modify
	{
		if (m_bVirtual)
		{
			aTgtPage.Add(m_Data.nStype);
		}
		else
		{
			if (m_Data.nStype != D_SECT_TYPE_REGULAR)   // DB/USER는 항상 뜬다.
				aTgtPage.Add(D_SECT_TYPE_REGULAR);
			aTgtPage.Add(m_Data.nStype);
		}
	}
	else  // Add
	{
#if defined(_CIVIL)
		aTgtPage.Add(D_SECT_TYPE_REGULAR);
		aTgtPage.Add(D_SECT_TYPE_USER);
		aTgtPage.Add(D_SECT_TYPE_SRC);
		aTgtPage.Add(D_SECT_TYPE_COMBINED);
		aTgtPage.Add(D_SECT_TYPE_PSC);
		aTgtPage.Add(D_SECT_TYPE_TAPERED);
		aTgtPage.Add(D_SECT_TYPE_CONSTRUCTION);
		aTgtPage.Add(D_SECT_TYPE_STLG_B);
#elif defined(_MGEN)
		aTgtPage.Add(D_SECT_TYPE_REGULAR);
		aTgtPage.Add(D_SECT_TYPE_USER);
		aTgtPage.Add(D_SECT_TYPE_SRC);
		aTgtPage.Add(D_SECT_TYPE_COMBINED);
		aTgtPage.Add(D_SECT_TYPE_TAPERED);
		aTgtPage.Add(D_SECT_TYPE_CONSTRUCTION); // PMS:4013-JWKWON-20090821 영문에서도 Composite Tab 활성화
//     if(CProduct::IsMovingType(D_PRODUCT_MOVING_ORG) ||
//        CProduct::GetMovingType() == D_PRODUCT_MOVING_CH ||
//        CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) aTgtPage.Add(D_SECT_TYPE_CONSTRUCTION);
#elif defined(_TOWER)
		aTgtPage.Add(D_SECT_TYPE_REGULAR);
#else
	#error _LS(IDS_CMD0417__Unknown_Product_Definition)
#endif
	}

	// Dialog 추가
	CCMSectItemBase *pDlg;
	CString   csSectName;
	UINT      nDlgResID;
	T_SECT_D  TempSect;
	int nPageNum = aTgtPage.GetSize();
	for (int i = 0; i < nPageNum; i++)
	{
		switch(aTgtPage[i])
		{
		case D_SECT_TYPE_REGULAR:
			pDlg = new CCMSectItemReg;
			csSectName = _LS(IDS_WG_CMD__ADDD__DB_User);
			nDlgResID = CCMSectItemReg::IDD;
			break;
		case D_SECT_TYPE_USER:
			pDlg = new CCMSectItemVal;
			csSectName = _LS(IDS_WG_CMD__ADDD__Value);
			nDlgResID = CCMSectItemVal::IDD;
			break;
			/*
		case D_SECT_TYPE_USER:
			pDlg = new CCMSectItemValOld;
			csSectName = _LS(IDS_WG_CMD__ADDD__Value);
			nDlgResID = CCMSectItemValOld::IDD;
			break;
			*/
		case D_SECT_TYPE_SRC:
			pDlg = new CCMSectItemSrc;
			csSectName = _LS(IDS_WG_CMD__ADDD_SECT_SRC);
			nDlgResID = CCMSectItemSrc::IDD;
			break;
		case D_SECT_TYPE_COMBINED:
			pDlg = new CCMSectItemCom;
			csSectName = _LS(IDS_WG_CMD__ADDD__Combined);
			nDlgResID = CCMSectItemCom::IDD;
			break;
		case D_SECT_TYPE_PSC:
			pDlg = new CCMSectItemPSC(this);
			csSectName = _LS(IDS_WG_CMD__ADDD__PSC);  //_ULS(PSC);
			nDlgResID = CCMSectItemPSC::IDD;
			break;
		case D_SECT_TYPE_TAPERED:
			pDlg = new CCMSectItemTap;
			csSectName = _LS(IDS_WG_CMD__ADDD__Tapered);
			nDlgResID = CCMSectItemTap::IDD;
			break;
		/*
		case D_SECT_TYPE_TAPERED:
			pDlg = new CCMSectItemTapOld;
			csSectName = _LS(IDS_WG_CMD__ADDD__Tapered);
			nDlgResID = CCMSectItemTapOld::IDD;
			break;
		*/
		case D_SECT_TYPE_COLDFORMED:
			pDlg = new CCMSectItemCfm;
			csSectName = _LS(IDS_WG_CMD__ADDD__Cold_Formed);
			nDlgResID = CCMSectItemCfm::IDD;
			break;
		case D_SECT_TYPE_CONSTRUCTION:
		case D_SECT_TYPE_COMPO_B:
		case D_SECT_TYPE_COMPO_I:
		case D_SECT_TYPE_COMPO_TUB:
		case D_SECT_TYPE_COMPO_STLG_B:
		case D_SECT_TYPE_COMPO_STLG_I:
		case D_SECT_TYPE_COMPO_STLG_TUB:
		case D_SECT_TYPE_COMPO_CI:
		case D_SECT_TYPE_COMPO_CT:
		case D_SECT_TYPE_COMPO_PC:
		case D_SECT_TYPE_COMPO_G:
			pDlg = new CCMSectItemCon;
			csSectName = _LS(IDS_WG_CMD__ADDD__Composite);
			nDlgResID = CCMSectItemCon::IDD;
			break;
		case D_SECT_TYPE_STLG_B:
		case D_SECT_TYPE_STLG_I:
		case D_SECT_TYPE_STLG_MCELL:
			pDlg = new CCMSectItemStlGirder;
			csSectName = _LS(IDS_WG_CMD__ADDD__STLGIRDER);
			nDlgResID = CCMSectItemStlGirder::IDD;
			break;
		default: return FALSE;
		}

		pDlg->SetMode(m_bDesign);
		pDlg->SetVirual(m_bVirtual);
		m_wndTabCtrl.AddTab(pDlg, csSectName, nDlgResID, TRUE);
		m_aSectType.Add(aTgtPage[i]);
		
		if (m_bModify)  // Modify : 최대 두개
		{
			// Target Page와 동일 타입이면 수정될 데이타 설정
			// 아니면 DB/USER
			if (m_Data.nStype == aTgtPage[i]) 
				pDlg->SetModifyData(m_Key, m_Data);
			else
			{
				TempSect.Initialize();
				TempSect.nStype = D_SECT_TYPE_REGULAR;
				TempSect.SName = m_Data.SName;
				TempSect.SectBefore.Shape = D_SECT_SHAPE_REG_H;
				TempSect.SectBefore.SectI.DBName = m_Data.SectBefore.SectI.DBName;
				pDlg->SetModifyData(m_Key, TempSect);
			}
		}

		if(m_bWorkTab) pDlg->SetSectionID(m_FixID);
	}
	return TRUE;
}

// 현재 Section의 Data 타입에 따라 해당 Dialog를 보여준다.
int CCMSectItemDlg::GetDialogPagePos()
{
	int nSize = m_wndTabCtrl.GetTabCount();
	int i = 0;
	for (i = 0; i < nSize; i++)
		if (m_aSectType[i] == m_Data.nStype) break;
	if (i == nSize) return -1; // not found
	return i;
}

BOOL CCMSectItemDlg::ShowDialogPage(int nPagePos)
{
	if (!m_wndTabCtrl.ShowTab(nPagePos)) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_show_dialog_));
		return FALSE;
	}
	return TRUE;
}

void CCMSectItemDlg::EnableDisableCalcBtn()
{
	int nCurSel = m_wndTabCtrl.GetCurSel();
	if (nCurSel < 0) return;

	int nSectType = m_aSectType.GetAt(nCurSel);
	if (nSectType == D_SECT_TYPE_USER) m_wndCalcBtn.EnableWindow(FALSE);
	else m_wndCalcBtn.EnableWindow(TRUE);
}
void CCMSectItemDlg::ExternalSetCalcBtn(BOOL bSet)
{
	m_wndCalcBtn.EnableWindow(bSet);
}

void CCMSectItemDlg::SetActiveTab(int selectTab)
{
	m_wndTabCtrl.ShowTab(selectTab);
	m_nActiveTab = selectTab;
}

BEGIN_MESSAGE_MAP(CCMSectItemDlg, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemDlg)
	ON_BN_CLICKED(IDC_CMD_SP_ID_BTN_CALC, OnBtnCalculate)
	ON_BN_CLICKED(IDOK, OnBtnOK)
	ON_BN_CLICKED(IDC_CMD_SP_ID_BTN_APPLY, OnBtnApply)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB_CTRL, OnChangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_CMD_TAB_CTRL, OnBeforeChangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemDlg message handlers

BOOL CCMSectItemDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here - 
	
	// Dialog Page 설정 
	BOOL bSet = SetDialogPageList();
	if (!bSet)
	{
		// 에러시 Dialog 닫는다.
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_dialog_page_for_specif));
		SendMessage(WM_CLOSE, 0, 0);
		return TRUE;
	}

	// Dialog Page Manager에 Place Holder 설정
	CWnd* pPH = (CWnd*)GetDlgItem(IDC_CMD_PLACEHOLDER);
	ASSERT(pPH && pPH->GetSafeHwnd() && IsWindow(pPH->GetSafeHwnd()));
	bSet = m_wndTabCtrl.SetPlaceHolder(pPH);
	ASSERT(bSet);

	// Get Dialog page index
	int nPagePos;
	if (m_bModify) nPagePos = GetDialogPagePos();
	else nPagePos = m_nActiveTab;  // Default is zero.

	// Show Dialog
	if (nPagePos < 0) 
	{
		// 에러시 Dialog 닫는다.
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_dialog_page_for_specif));
		SendMessage(WM_CLOSE, 0, 0);
	}
	else 
	{
		ShowDialogPage(nPagePos);
		EnableDisableCalcBtn();
	}

	if(m_bModify) m_wndBtnApply.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemDlg::OnBtnCalculate() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_wndTabCtrl.GetCurSel();
	if (nCurSel < 0) 
	{
		TRACE(_LS(IDS_WG_CMD__ADDD__Error___No_active_page_));
		return;
	}

	CCMSectItemBase* pDlg = (CCMSectItemBase*)m_wndTabCtrl.GetTab(nCurSel);
	pDlg->ShowCalcResult();
}

void CCMSectItemDlg::OnBtnOK() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_wndTabCtrl.GetCurSel();
	if (nCurSel < 0) 
	{
		TRACE(_LS(IDS_WG_CMD__ADDD__Error___No_active_page_));
		return;
	}
	CCMSectItemBase* pDlg = (CCMSectItemBase*)m_wndTabCtrl.GetTab(nCurSel);
	//Add hjs 03/03/12
	if(m_pVoidData==NULL)
	{
		if (pDlg->OnOKPublic()) CDialog::OnOK();
	}
	else
	{
		T_SECT_D *pSect_data = new T_SECT_D;
		if(pDlg->GetSectData(*pSect_data))
		{
			m_pVoidData = (void*)pSect_data;
			CDialog::OnOK();
		}
	}
}

void CCMSectItemDlg::OnBtnApply() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_wndTabCtrl.GetCurSel();
	if (nCurSel < 0) 
	{
		TRACE(_LS(IDS_WG_CMD__ADDD__Error___No_active_page_));
		return;
	}

	CCMSectItemBase* pDlg = (CCMSectItemBase*)m_wndTabCtrl.GetTab(nCurSel);
	if (pDlg->OnApplyPublic())
	{
		// 성공하면 다른 페이지에도 같은 ID를 설정한다.
		// modify 경우는 페이지가 하나 밖에 없으므로 문제 없다.
		// Apply 된 후이므로 GetNewID는 새로 생성된 Section ID를 가져온다.
		// 이미 Active된 Dialog에 SetNewID해도 동일한 값이 설정되므로 문제 없다.
		// 다른 페이지는 Dialog는 활성화 안 되어 있지만 클래스는 살아 있고
		// SetNewID가 Dialog Window와는 상관 없으므로 문제 없다.
		T_SECT_K Key = pDlg->GetNewID();
		int nSectPageNum = m_wndTabCtrl.GetTabCount();
		for (int i = 0; i < nSectPageNum; i++) 
		{
			pDlg = (CCMSectItemBase*)m_wndTabCtrl.GetTab(i);
			pDlg->SetNewID(Key);
		}
	}
}

void CCMSectItemDlg::OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetRedraw(FALSE);

	EnableDisableCalcBtn();
	int nCurSel = m_wndTabCtrl.GetCurSel();
	if (nCurSel == PSC_TAB)
	{
		if (m_wndTabCtrl.GetTab(nCurSel))
		{
			CCMSectItemPSC* pDlg = (CCMSectItemPSC*)m_wndTabCtrl.GetTab(nCurSel);
			pDlg->PostMessage(CCMSectItemPSC::WM_AFTERINITDIALOG);
		}
	}

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

	*pResult = 0;
}

void CCMSectItemDlg::OnBeforeChangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_wndTabCtrl.GetCurSel();
	if (nCurSel < 0) return;
	CCMSectItemBase* pDlg = (CCMSectItemBase*)m_wndTabCtrl.GetTab(nCurSel);  
	pDlg->SaveDlgData();
	
	*pResult = 0;
}

BOOL CCMSectItemDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		// When an item is being edited make sure the edit control
		// receives certain important key strokes
		if(pMsg->wParam == VK_ESCAPE && IsCloseByEsc())
			return CDialogMove::PreTranslateMessage(pMsg); 
	  if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		{
		  return FALSE;
			// DO NOT process further
		}
	}
	return CDialogMove::PreTranslateMessage(pMsg);
}

BOOL CCMSectItemDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DestroyWindow();
}

void CCMSectItemDlg::OnCancel() 
{
	// TODO: Add extra cleanup here  	
	CDialog::OnCancel();
}

void CCMSectItemDlg::SetRetunData(void*& pData)
{
	m_pVoidData = pData;
}