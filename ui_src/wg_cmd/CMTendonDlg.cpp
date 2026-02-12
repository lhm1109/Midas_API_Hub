// CMTendonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTendonDlg.h"

#include "CMTendonProfileDlg.h"
#include "CMTendonProfileCopyDlg.h"
#include "CMTendonChangePropDlg.h"
#include "CMTendonExportDXFDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTendonDlg dialog
#define CDialog CCMDlgBase

CCMTendonDlg::CCMTendonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMTendonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTendonDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bShowProfDlg = FALSE;
	m_aControls.RemoveAll();
	EnableLayout();
}


void CCMTendonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTendonDlg)
	DDX_Control(pDX, IDC_CMD_TENDON_LIST, m_wndTendonList);
 
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMTendonDlg, CDialog)
	//{{AFX_MSG_MAP(CCMTendonDlg)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_ADD, OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_DELETE1, OnCmdDelete)
	ON_BN_CLICKED(IDC_CMD_MODIFY1, OnCmdModify)
	ON_BN_CLICKED(IDC_CMD_COPY, OnCmdCopy)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_TENDON_LIST, OnDblclkCmdTendonList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_TENDON_LIST, OnItemchangedCmdTendonList)
	ON_NOTIFY(NM_CLICK, IDC_CMD_TENDON_LIST, OnClickCmdTendonList)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CMD_CHANGE_PROPERTY, OnCmdChangeProperty)
	ON_BN_CLICKED(IDC_CMD_EXPORT_DXF     , OnCmdExportToDXF)
	ON_WM_SIZING()
	ON_WM_SIZE()
	//ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMyListCtrl::NLV_KEYUP,OnListCtrlKeyUp)
	ON_REGISTERED_MESSAGE(CMyListCtrl::NLV_LBUTTONUP,OnListCtrlLButtonUp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTendonDlg message handlers
BOOL CCMTendonDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	MakeListHeader();
	UpdateTdnaList();

	m_aControls.RemoveAll();
	m_aControls.Add(IDC_CMD_ADD);
	m_aControls.Add(IDC_CMD_MODIFY1);
#if !defined(_CIVIL) // CIVIL은 Change Tendon Profile 으로 수정 기능이 제공됨.
	m_aControls.Add(IDC_CMD_CHANGE_PROPERTY);
#endif
	m_aControls.Add(IDC_CMD_COPY);
	m_aControls.Add(IDC_CMD_EXPORT_DXF);
	m_aControls.Add(IDC_CMD_DELETE1);
	m_aControls.Add(IDC_CMD_CLOSE);
#if defined(_CIVIL) // CIVIL
	GetDlgItem(IDC_CMD_CHANGE_PROPERTY)->ShowWindow(FALSE);
	AlignControlForCivil();
#endif

	SaveWindowSizeData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTendonDlg::MakeListHeader()
{
	CStringArray         HTitles;

	HTitles.Add(_LS(IDS_CMD_TDNA_LIST_Name));
	HTitles.Add(_LS(IDS_CMD_TDNA_LIST_Property));
	HTitles.Add(_LS(IDS_CMD_TDNA_LIST_Element_No__List));
	HTitles.Add(_LS(IDS_CMD_TDNA_LIST_Type));
	HTitles.Add(_LS(IDS_CMD_TDNA_LIST_Tendon_Num));
	m_fHRatio.Add(0.15f);
	m_fHRatio.Add(0.15f);
	m_fHRatio.Add(0.3f);
	m_fHRatio.Add(0.15f);
	m_fHRatio.Add(0.22f);
	CDlgUtil::_SetListCtrlHeader(&m_wndTendonList, HTitles, &m_fHRatio, NULL);
}

void CCMTendonDlg::OnCmdClose() 
{
	DestroyWindow();
}


void CCMTendonDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	switch(lHint)
	{
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		SaveCurSelectedStagKeys();
		UpdateTdnaList();
		ReselectCurStageKeys();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMTendonDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsTdnaCommand = FALSE;	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_TDNA_ADD):
		case(UR_TDNA_DEL):
				bIsTdnaCommand = TRUE;
				break;
		default:
			break;
		}
	} 
	if(bIsTdnaCommand) 
	{
		SaveCurSelectedStagKeys();
		UpdateTdnaList();
		ReselectCurStageKeys();    
	}
}

void CCMTendonDlg::UpdateTdnaList()
{
	I_GENModelBase::UTIL_DeleteAllDisplayObjST();	
	m_wndTendonList.DeleteAllItems();
	
	CArray<T_TDNA_K, T_TDNA_K> KeyList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdnaKeyList(KeyList);
	int nKeys = KeyList.GetSize();
	CStringArray Contents;
	CString StrTemp,StrTTemp;
	T_TDNA_D TdnaD;
	T_TDNT_D TdntD;
	int Index= 0;
	for(int i = 0;i < nKeys; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdna(KeyList[i],TdnaD))
		{
			Contents.RemoveAll();
			Contents.Add(TdnaD.TendonName);
			int nElem = TdnaD.aElemList.GetSize();
			
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdnt(TdnaD.TendonTypeKey,TdntD))
			{
				Contents.Add(TdntD.TendonTypeName);
				
				
				qsort(TdnaD.aElemList.GetData(),TdnaD.aElemList.GetSize(),sizeof(UINT),
					CCompFunc::UINTAsc);

				CNumericOptimizer Nopt;
				StrTemp = Nopt.Optimize((long*)TdnaD.aElemList.GetData(),TdnaD.aElemList.GetSize());
				/*
				StrTemp.Empty(); 
				for(int j = 0; j< nElem; j++)
				{
					if(j == 0)
						StrTTemp.Format(_T("%d"),TdnaD.aElemList[j]);
					else
						StrTTemp.Format(_T(",%d"),TdnaD.aElemList[j]);
					StrTemp += StrTTemp;
				}
				*/
				Contents.Add(StrTemp);
				CString strShape;
				switch(TdnaD.nTendonShape)
				{
				case 0:
					strShape=_LS(IDS_CMD_TDNA_SHAPE_Straight);
					break;
				case 1:
					strShape=_LS(IDS_CMD_TDNA_SHAPE_Curve);
					break;
				case 2:
					strShape=_LS(IDS_CMD_TDNA_SHAPE_Element);
					break;
				}
				Contents.Add(strShape);
				CString strTendonNum;
				strTendonNum.Format(_T("%g"),TdnaD.dTendonNum);
				Contents.Add(strTendonNum);
				CDlgUtil::SetListItem(&m_wndTendonList,Index,Contents,KeyList[i]);
				Index++;
			}
		}
	}
}

void CCMTendonDlg::OnCmdAdd() 
{
	if(CDBDoc::GetDocPoint())
	{
		I_GENModelBase::UTIL_DeleteAllDisplayObjST();	
		I_GENModelBase::GetCurMySelfST()->ViewInvalidate4SelectedTendonLabel(FALSE);
	}

	ShowItemDlg(0);
	/*
	CCMTendonAddDlg* pAdd;
	pAdd = (CCMTendonAddDlg*) CreateOrActivateDlg((CDBDoc*)CDBDoc::GetDocPoint(),
														CCMTendonAddDlg::IDD,this);
	if(pAdd)
	{
		pAdd->SetTDNA(0);
		pAdd->SetParentDlg(this);
		ShowWindow(SW_HIDE);
	}
	*/
}

void CCMTendonDlg::OnCmdDelete() 
{
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndTendonList,arRet,LVIS_SELECTED);
	int nRet = arRet.GetSize();
	if(nRet == 0) return;

	T_TDNA_D TdnaD;
	T_TDNA_K TdnaK;
	CArray<CString, CString&> Tdnas;

	for(int i = 0; i < nRet; i++)
	{

		TdnaK = (T_TDNA_K)m_wndTendonList.GetItemData(arRet[i]);

		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdna(TdnaK, TdnaD))
			Tdnas.Add(TdnaD.TendonName);
	}
	
	CDBDoc::GetDocPoint()->m_pDataCtrl->DelTdna(Tdnas);
}

void CCMTendonDlg::OnCmdModify() 
{
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndTendonList,arRet,LVIS_SELECTED);
	int nRet = arRet.GetSize();
	if(nRet != 1)
	{
		AfxMessageBox(_LS(IDS_CMD_TDNA_LIST_Error___No_Selected_Tendon_Profil));
		return;
	}

	CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
	T_TDNA_K TdnaK;
	TdnaK = (T_TDNA_K)m_wndTendonList.GetItemData(arRet[0]);
	ShowItemDlg(TdnaK);
	/*
	CCMTendonAddDlg* pAdd;
	pAdd = (CCMTendonAddDlg*) CreateOrActivateDlg((CDBDoc*)CDBDoc::GetDocPoint(),
														CCMTendonAddDlg::IDD,this);
	if(pAdd)
	{
		T_TDNA_K TdnaK;
		TdnaK = (T_TDNA_K)m_wndTendonList.GetItemData(arRet[0]);
		pAdd->SetTDNA(TdnaK);
		pAdd->SetParentDlg(this);
		ShowWindow(SW_HIDE);
	}
	*/
}



void CCMTendonDlg::ShowItemDlg(T_TDNA_K key)
{
	CCMTendonProfileDlg* pDlg = new CCMTendonProfileDlg();
	pDlg->SetTDNA(key);
	pDlg->SetParentDlg(this);
	pDlg->Create(CCMTendonProfileDlg::IDD, this);
	pDlg->SetInitPos(D_INIT_POS_RT);

	ShowWindow(SW_HIDE);

	pDlg->ShowWindow(SW_SHOW);
}

void CCMTendonDlg::OnDblclkCmdTendonList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdModify();
	
	*pResult = 0;
}

BOOL CCMTendonDlg::DestroyWindow() 
{
	if(CDBDoc::GetDocPoint())
	{
		I_GENModelBase::UTIL_DeleteAllDisplayObjST();	
		I_GENModelBase::GetCurMySelfST()->ViewInvalidate4SelectedTendonLabel(FALSE);
	}
	
	return CDialog::DestroyWindow();
}


void CCMTendonDlg::DisplaySelectedTendon()
{
	if(I_GENModelBase::GetCurMySelfST())
	{
		I_GENModelBase::UTIL_DeleteAllDisplayObjST();
			
		CArray<int,int> arRet;
		T_TDNA_K  TdnaK;
		CDlgUtil::GetListItemByMask(&m_wndTendonList,arRet,LVIS_SELECTED);
		int nRet = arRet.GetSize();

		for(int i = 0; i < nRet; i++)
		{
		
			TdnaK = (T_TDNA_K)m_wndTendonList.GetItemData(arRet[i]);
			I_GENModelBase::GetCurMySelfST()->UTIL_SetDisplayObj(UR_TDNA_ADD,&TdnaK);
		}

		I_GENModelBase::GetCurMySelfST()->ViewInvalidate4SelectedTendonLabel(FALSE);
	}
}

 
void CCMTendonDlg::DisplayCurrentTendon(T_TDNA_K TdnaK)
{
	I_GENModelBase::UTIL_DeleteAllDisplayObjST();
	I_GENModelBase::GetCurMySelfST()->UTIL_SetDisplayObj(UR_TDNA_ADD,&TdnaK);
	I_GENModelBase::GetCurMySelfST()->ViewInvalidate4SelectedTendonLabel(FALSE);
}

void CCMTendonDlg::OnItemchangedCmdTendonList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

LRESULT CCMTendonDlg::OnListCtrlKeyUp(WPARAM wParam, LPARAM lParam)
{ 
	DisplaySelectedTendon();
	return 0L;
}

LRESULT CCMTendonDlg::OnListCtrlLButtonUp(WPARAM wParam, LPARAM lParam)
{
	DisplaySelectedTendon();
	return 0L;
}

void CCMTendonDlg::OnClickCmdTendonList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DisplaySelectedTendon();
	*pResult = 0;
}

void CCMTendonDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	if(bShow)	
		DisplaySelectedTendon();
}

void CCMTendonDlg::SaveCurSelectedStagKeys()
{
	m_CurSelTdnaK.RemoveAll();
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndTendonList,arRet,LVIS_SELECTED);
	int nRet = arRet.GetSize();
	if(nRet == 0) return;

	T_TDNA_K TdnaK;
	T_TDNA_D TdnaD;

	for(int i = 0; i < nRet; i++)
	{
		TdnaK = (T_TDNA_K)m_wndTendonList.GetItemData(arRet[i]);
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdna(TdnaK, TdnaD))
			m_CurSelTdnaK.SetAt(TdnaK,TdnaK);
	}
}

void CCMTendonDlg::ReselectCurStageKeys()
{
	//if(!IsWindowVisible()) return;
	int nTendons = m_wndTendonList.GetItemCount();

	if(nTendons == 0) return;

	T_TDNA_K TdnaK,TempT;
	T_TDNA_D TdnaD;
	CArray<int,int> arSel;
	for(int i = 0 ; i < nTendons; i++)
	{
		TdnaK = (T_TDNA_K)m_wndTendonList.GetItemData(i);
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdna(TdnaK, TdnaD))
		{
			if(m_CurSelTdnaK.Lookup(TdnaK,TempT))
			{
				arSel.Add(i);
				I_GENModelBase::GetCurMySelfST()->UTIL_SetDisplayObj(UR_TDNA_ADD,&TdnaK);
			}
		}
	}

	if(arSel.GetSize())
	{
		CDlgUtil::SetListItemByMask(&m_wndTendonList,arSel,LVIS_SELECTED, LVIS_SELECTED);
		I_GENModelBase::GetCurMySelfST()->ViewInvalidate4SelectedTendonLabel(FALSE);
	}

}


BOOL CCMTendonDlg::IsShowProfDlg()
{
	return m_bShowProfDlg;
}

void CCMTendonDlg::SetProfDlgFlag(BOOL bShowProfDlg)
{
	m_bShowProfDlg = bShowProfDlg;
}

// 2003. 09. 27  추가 
void CCMTendonDlg::OnCmdChangeProperty() 
{
	// TODO: Add your control notification handler code here
	CArray<int, int> aSelectedItem;
	CDlgUtil::GetListItemByMask(&m_wndTendonList, aSelectedItem, LVIS_SELECTED);

	T_TDNA_K key;
	CArray<T_TDNA_K, T_TDNA_K> aKey;
	int nNum = aSelectedItem.GetSize();
	for (int i=0; i<nNum; i++)
	{
		key = m_wndTendonList.GetItemData(aSelectedItem[i]);
		aKey.Add(key);
	}

	CCMTendonChangePropDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.SetSelectedProfiles(aKey);
	dlg.DoModal();
}

void CCMTendonDlg::OnCmdExportToDXF() 
{
	CArray<int, int> aSelectedItem;
	CDlgUtil::GetListItemByMask(&m_wndTendonList, aSelectedItem, LVIS_SELECTED);
	
	T_TDNA_K key;
	CArray<T_TDNA_K, T_TDNA_K> aKey;
	int nNum = aSelectedItem.GetSize();
	for (int i=0; i<nNum; i++)
	{
		key = m_wndTendonList.GetItemData(aSelectedItem[i]);
		aKey.Add(key);
	}
	
	CCMTendonExportDXFDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.SetSelectedProfiles(aKey);
	dlg.DoModal();
}

// MNET:1939-BJLEE(BBONG)-20060410
void CCMTendonDlg::OnCmdCopy() 
{
	CArray<T_TDNA_K,T_TDNA_K> arSelected;

	GetSelectedTendon(arSelected);
	if(arSelected.GetSize()==0) 
	{
		AfxMessageBox(_LS(IDS_CMD_TDNA_LIST_Error___No_Selected_Tendon_Profil));
		return;
	}
	CCMTendonProfileCopyDlg* pDlg = new CCMTendonProfileCopyDlg(this);
	//pDlg->SetParentDlg(this);
	pDlg->Create(CCMTendonProfileCopyDlg::IDD, this);

	ShowWindow(SW_HIDE);

	pDlg->ShowWindow(SW_SHOW);
}
void CCMTendonDlg::OnChangeCurrentElem(int nKey)
{
}
void CCMTendonDlg::GetSelectedTendon(CArray<T_TDNA_K,T_TDNA_K>& arSelected)
{
	CArray<int,int> arRet;
	T_TDNA_K  TdnaK;
	CDlgUtil::GetListItemByMask(&m_wndTendonList,arRet,LVIS_SELECTED);
	int nRet = arRet.GetSize();
	arSelected.RemoveAll();
	for(int i = 0; i < nRet; i++)
	{
		TdnaK = (T_TDNA_K)m_wndTendonList.GetItemData(arRet[i]);
		arSelected.Add(TdnaK);
	}
}

void CCMTendonDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CRect r(pRect);
	if (r.Width() < m_MinimumRect.Width())
	{
		if (fwSide == WMSZ_LEFT || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_BOTTOMLEFT)
		{
			pRect->left = pRect->right - m_MinimumRect.Width();
		}
		else
		{
			pRect->right = pRect->left + m_MinimumRect.Width();
		}
	}
	if (r.Height() < m_MinimumRect.Height())
	{
		if (fwSide == WMSZ_TOP || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_TOPRIGHT)
		{
			pRect->top = pRect->bottom - m_MinimumRect.Height();
		}
		else
		{
			pRect->bottom = pRect->top + m_MinimumRect.Height();
		}
	}
	CDialogMove::OnSizing(fwSide, pRect);
}

void CCMTendonDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialogMove::OnSize(nType, cx, cy);

	if(m_aControls.GetSize() == 0) return; // Oninitdialog()가 불리기 전에 Onsize가 들어오는 경우는 skip

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CMD_CLOSE);
	if (pBtn == 0 || pBtn->GetSafeHwnd() == 0 ||
			!IsWindow(pBtn->GetSafeHwnd())) return;

	CRect r;
	GetWindowRect(r);
	AlignControl(r);
}

void CCMTendonDlg::SaveWindowSizeData()
{
	GetWindowRect(m_MinimumRect);
	m_nDlgHeight = m_MinimumRect.Height();
	m_nDlgWidth = m_MinimumRect.Width();
}

void CCMTendonDlg::AlignControl(const CRect& rect)
{
	int nWidthDiff = rect.Width() - m_nDlgWidth;
	m_nDlgWidth = rect.Width();
	int nHeightDiff = rect.Height() - m_nDlgHeight;
	m_nDlgHeight = rect.Height();

	CRect r;
	CWnd* pWnd = GetDlgItem(IDC_CMD_TENDON_LIST);
	pWnd->GetWindowRect(r);
	r.bottom += nHeightDiff;
	r.right  += nWidthDiff;
	ScreenToClient(r);
	pWnd->MoveWindow(r, FALSE);
	for(UINT i=0; i<m_fHRatio.GetSize(); ++i)
	{
		float HRatio = m_fHRatio.GetAt(i);
		m_wndTendonList.SetColumnWidth(i, r.Width()*HRatio);
	}

	CDlgUtil::CtrlMoveDistX(this, m_aControls, nWidthDiff);

	// Close 버튼의 위치를 재 조정한다.
	CRect rClose;
	GetDlgItem(IDC_CMD_CLOSE)->GetWindowRect(rClose);
	int nOffsetCloseX = rect.bottom - rClose.bottom - globalUtils.ScaleByDPI(8);

	CDlgUtil::CtrlMoveDistXYByRect(this, IDC_CMD_CLOSE, 0, nOffsetCloseX, TRUE, TRUE);

	Invalidate();
	UpdateWindow();
}

void CCMTendonDlg::AlignControlForCivil()
{
	// CIVIL 일 경우 Change Property 버튼을 삭제하고, 나머지 컨트롤의 위치를 재 정렬한다.
	CArray<UINT, UINT> aControls;
	CRect rRef, rToMove;
	int nDistX, nDistY;

	aControls.Add(IDC_CMD_COPY);
	aControls.Add(IDC_CMD_EXPORT_DXF);
	aControls.Add(IDC_CMD_DELETE1);

	GetDlgItem(IDC_CMD_MODIFY1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_COPY)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(5);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
}

void CCMTendonDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);

	rc.left = rc.right - ::GetSystemMetrics(SM_CXHSCROLL);
	rc.top = rc.bottom - ::GetSystemMetrics(SM_CYVSCROLL);

	dc.DrawFrameControl(rc, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
}