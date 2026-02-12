// CMGilcListDlg.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGilcListDlg.h"
#include "CMGilcItemDlg.h"

#include "CMGildDeadDlg.h"
#include "CMGildVLiveDlg.h"
#include "CMGildCLiveDlg.h"
#include "CMGildOLiveDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCMGXGilcList 

void CCMGXGilcList::MakeItemList()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	CArray<T_GILC_K, T_GILC_K> aKeyList;
	pDoc->m_pAttrCtrl->GetGilcKeyList(aKeyList);
	int nRecCount = aKeyList.GetSize();
	SetRowCount(0);
	SetRecordCount(nRecCount);
	
	T_GILC_D data;
	CStringArray aValue;
	for (int i = 0; i < nRecCount; i++)
	{
		pDoc->m_pAttrCtrl->GetGilc(aKeyList[i], data);
		ConvertDataToStr(data, aValue);
		SetValue((ROWCOL)(i+1), aKeyList[i], data.LoadCaseId, aValue);
	}
}

void CCMGXGilcList::ModifySerialID(CArray<CString, CString&> &aName, CArray<UINT, UINT> &aSerial)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	T_GILC_D data;
	CArray<T_GILC_D, T_GILC_D&> aData;
	for (int i=0; i<aName.GetSize(); i++)
	{
		if (!pDoc->m_pAttrCtrl->GetGilc(aName[i], data)) ASSERT(0);
		data.LoadCaseId = aSerial[i];
		aData.Add(data);
	}
	pDoc->m_pDataCtrl->ModifyGilc(aName, aData);
}

void CCMGXGilcList::ConvertDataToStr(T_GILC_D data, CStringArray &aValue)
{
	CString aType[5] = { _LS(IDS_CMD_GILC_DEAD_LOAD), _LS(IDS_CMD_GILC_ODEAD_LOAD), 
		_LS(IDS_CMD_GILC_VLIVE_LOAD), _LS(IDS_CMD_GILC_CLIVE_LOAD), _LS(IDS_CMD_GILC_OLIVE_LOAD)};

	aValue.SetSize(2);
	aValue[0] = data.LoadCaseName;
	aValue[1] = aType[data.nLoadCaseType];
}

BOOL CCMGXGilcList::OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	BOOL bReturn = CCMGXGridList::OnLButtonDblClkRowCol(nRow, nCol, nFlags, pt);
	
	if (nRow < 1 || nRow >= GetRowCount()) return bReturn;

	UINT Key = _tstol(GetValueRowCol(nRow, 1));
	if (!m_pDoc->m_pAttrCtrl->ExistGilc(Key)) ASSERT(0);
	((CCMGilcListDlg*)(GetParent()))->DoModalLoadDlg(Key);
	
	return bReturn;
}

/*
void CCMGXGilcList::DoLButtonDblClick(ROWCOL nRow, ROWCOL nCol, UINT key)
{
	T_GILC_D Data;
	CString sMessage;
	//Key = m_pDoc->m_pAttrCtrl->GetGilcKey(aSelName[0]);
	if (!m_pDoc->m_pAttrCtrl->GetGilc(key, Data))
	{
		sMessage.Format(_LS(IDS_CMD_GILC_NO_EXIST_THIE_LOADCSE), Data.LoadCaseName);
		AfxMessageBox(sMessage);
		return;
	}

	if (Data.nLoadCaseType == 0 || Data.nLoadCaseType == 1) // Dead Load, Other Dead Load
	{
		CCMGildDeadDlg dlg;
		dlg.SetInitPos(D_INIT_POS_LB);
		dlg.SetLoadCase(key, Data.nLoadCaseType);
		dlg.DoModal();
	}
	else if (Data.nLoadCaseType == 2)   // Vehicle Live Load
	{
		CCMGildVLiveDlg dlg;
		dlg.SetInitPos(D_INIT_POS_LB);
		dlg.SetLoadCase(key);
		dlg.DoModal();
	}
	else if (Data.nLoadCaseType == 3)   // Crowded Live Load
	{
		CCMGildCLiveDlg dlg;
		dlg.SetInitPos(D_INIT_POS_LB);
		dlg.SetLoadCase(key);
		dlg.DoModal();
	}
	else if (Data.nLoadCaseType == 4)   // Other Live Load
	{
		CCMGildOLiveDlg dlg;
		dlg.SetInitPos(D_INIT_POS_LB);
		dlg.SetLoadCase(key);
		dlg.DoModal();
	}
	else ASSERT(0);
}
*/

/////////////////////////////////////////////////////////////////////////////
// CCMGilcListDlg dialog

CCMGilcListDlg::CCMGilcListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMGilcListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGilcListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
}


void CCMGilcListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGilcListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CMD_GRID, m_wndGrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMGilcListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMGilcListDlg)
	ON_BN_CLICKED(IDC_CMD_ADD, OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_MODIFY, OnCmdModify)
	ON_BN_CLICKED(IDC_CMD_DELETE, OnCmdDelete)
	ON_BN_CLICKED(IDC_CMD_LOADS, OnCmdLoads)
	ON_BN_CLICKED(IDCLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGilcListDlg message handlers

BOOL CCMGilcListDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndGrid.AddColInfo(_LS(IDS_CMD_GILC_GRID_NAME), 80, GX_VT_STRING, 0);
	m_wndGrid.AddColInfo(_LS(IDS_CMD_GILC_GRID_TYPE),160, GX_VT_STRING, 0);
	m_wndGrid.Initialize(m_pDoc);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGilcListDlg::OnCmdAdd() 
{
	CCMGilcItemDlg Dlg;
	Dlg.SetInitPos(D_INIT_POS_LB);
	Dlg.DoModal();
}

void CCMGilcListDlg::OnCmdModify() 
{
	//CArray<CString, CString&> aSelName;
	CArray<UINT, UINT> aSelKey;
	int nCount = m_wndGrid.GetSelectedKeyList(aSelKey);
	if (nCount <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_GILC_NO_SELECTED_LOADCASE));
		return;
	}
	if (nCount > 1)
	{
		AfxMessageBox(_LS(IDS_CMD_GILC_MORE_SELECTED_LOADCASE));
		return;
	}
	
	T_GILC_D data;
	m_pDoc->m_pAttrCtrl->GetGilc(aSelKey[0], data);

	CCMGilcItemDlg dlg;
	dlg.SetParamData(data.LoadCaseName);
	dlg.DoModal();
}


void CCMGilcListDlg::OnCmdDelete() 
{
	CArray<UINT, UINT> aSelKey;
	int nCount = m_wndGrid.GetSelectedKeyList(aSelKey);
	if (nCount <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
	}

	T_GILC_D data;
	CArray<CString, CString&> aSelName;
	for (int i=0; i<aSelKey.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetGilc(aSelKey[i], data);
		aSelName.Add(data.LoadCaseName);
	}
	m_pDoc->m_pDataCtrl->DelGilc(aSelName);
}

void CCMGilcListDlg::OnCmdLoads() 
{
	// TODO: Add your control notification handler code here
	//CArray<CString, CString&> aSelName;
	CArray<UINT, UINT> aSelKey;
	int nCount = m_wndGrid.GetSelectedKeyList(aSelKey);
	if (nCount <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_GILC_NO_SELECTED_LOADCASE));
		return;
	}
	if (nCount > 1)
	{
		AfxMessageBox(_LS(IDS_CMD_GILC_MORE_SELECTED_LOADCASE));
		return;
	}

	DoModalLoadDlg(aSelKey[0]);
}

void CCMGilcListDlg::DoModalLoadDlg(T_GILC_K Key)
{
	T_GILC_D Data;
	CString sMessage;
	if (!m_pDoc->m_pAttrCtrl->GetGilc(Key, Data))
	{
		sMessage.Format(_LS(IDS_CMD_GILC_NO_EXIST_THIE_LOADCSE), Data.LoadCaseName);
		AfxMessageBox(sMessage);
		return;
	}

	if (Data.nLoadCaseType == 0 || Data.nLoadCaseType == 1) // Dead Load, Other Dead Load
	{
		CCMGildDeadDlg dlg;
		dlg.SetInitPos(D_INIT_POS_LB);
		dlg.SetLoadCase(Key, Data.nLoadCaseType);
		dlg.DoModal();
	}
	else if (Data.nLoadCaseType == 2)   // Vehicle Live Load
	{
		CCMGildVLiveDlg dlg;
		dlg.SetInitPos(D_INIT_POS_LB);
		dlg.SetLoadCase(Key);
		dlg.DoModal();
	}
	else if (Data.nLoadCaseType == 3)   // Crowded Live Load
	{
		CCMGildCLiveDlg dlg;
		dlg.SetInitPos(D_INIT_POS_LB);
		dlg.SetLoadCase(Key);
		dlg.DoModal();
	}
	else if (Data.nLoadCaseType == 4)   // Other Live Load
	{
		CCMGildOLiveDlg dlg;
		dlg.SetInitPos(D_INIT_POS_LB);
		dlg.SetLoadCase(Key);
		dlg.DoModal();
	}
	else ASSERT(0);
}

void CCMGilcListDlg::OnClose() 
{
	// Nothing
	 CDialogMove::OnOK();  	
}

void CCMGilcListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	default:
		break;
	}
}

void CCMGilcListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_GILC_ADD):
		case(UR_GILC_DEL):
		case(UR_GILC_MFD): 
		case(UR_GILC_MFS): 
			bChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	if (bChanged) m_wndGrid.MakeItemEx(); 
}
