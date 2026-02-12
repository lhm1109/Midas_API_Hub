// CMGridCrossBeamListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGridCrossBeamListDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h" 
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h" 

#include "..\wg_base\wg_base_NumericOptimizer.h"

#include "CMGridCrossBeamItemDlg.h"
#include "ExportFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLCOUNT      2

/////////////////////////////////////////////////////////////////////////////
// CCMGXMgdrList
void CCMGXCbemList::MakeItemList()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	CArray<T_CBEM_K, T_CBEM_K> aKeyList;
	pDoc->m_pAttrCtrl->GetCbemKeyList(aKeyList);
	int nRecCount = aKeyList.GetSize();
	SetRowCount(0);
	SetRecordCount(nRecCount);
	
	T_CBEM_D data;
	CStringArray aValue;
	for (int i = 0; i < nRecCount; i++)
	{
		pDoc->m_pAttrCtrl->GetCbem(aKeyList[i], data);
		ConvertDataToStr(data, aValue);
		SetValue((ROWCOL)(i+1), aKeyList[i], data.nSeq, aValue);
	}
}

void CCMGXCbemList::ModifySerialID(CArray<CString, CString&> &aName, CArray<UINT, UINT> &aSerial)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	T_CBEM_D data;
	CArray<T_CBEM_D, T_CBEM_D&> aData;
	for (int i=0; i<aName.GetSize(); i++)
	{
		if (!pDoc->m_pAttrCtrl->GetCbem(aName[i], data)) ASSERT(0);
		data.nSeq = aSerial[i];
		aData.Add(data);
	}
	pDoc->m_pDataCtrl->ModifyCbem(aName, aData);
}

void CCMGXCbemList::ConvertDataToStr(T_CBEM_D &Data, CStringArray &aValue)
{
	aValue.SetSize(2);
	aValue[0] = Data.Name;
	aValue[1] = DataToStr(Data);
}

CString CCMGXCbemList::DataToStr(T_CBEM_D &Data)
{
	CString str;
	CNumericOptimizer optimizer;

	long nSize = Data.aData.GetSize();
	str = _T("");
	if(nSize !=0)  
	{
		long* aNum = new long[nSize];
		for(int i=0; i<nSize; i++) aNum[i] = Data.aData[i].KeyNode;

		qsort((void*)aNum,nSize,sizeof(long),CNumericOptimizer::comparei);
		str = optimizer.Optimize(aNum,nSize);
		delete [] aNum;
	}
	return str;
}

BOOL CCMGXCbemList::OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	BOOL bReturn;
	bReturn = CCMGXGridList::OnLButtonDblClkRowCol(nRow, nCol, nFlags, pt);

	if (nRow < 1)
		return bReturn;

	int nRowCount = GetRowCount();
	if (nRowCount == nRow)
		return bReturn;

	UINT Key = _tstol(GetValueRowCol(nRow, 1));

	if (Key < 0)
		return bReturn;

	((CCMGridCrossBeamListDlg*)(GetParent()))->OnLButtonDblClkOnGrid(Key);
	return bReturn;
}

/////////////////////////////////////////////////////////////////////////////
// CCMGridCrossBeamListDlg dialog


CCMGridCrossBeamListDlg::CCMGridCrossBeamListDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMGridCrossBeamListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGridCrossBeamListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_pWndChild = NULL;
}


void CCMGridCrossBeamListDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGridCrossBeamListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CMD_GRID, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGridCrossBeamListDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMGridCrossBeamListDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGridCrossBeamListDlg message handlers

void CCMGridCrossBeamListDlg::OnCmdBtnAdd()
{
	CCMGridCrossBeamItemDlg* pWndChild = NULL;
	pWndChild = (CCMGridCrossBeamItemDlg*)CreateOrActivateDlg(m_pDoc, CCMGridCrossBeamItemDlg::IDD, this);
	ASSERT(pWndChild);
	pWndChild->SetModify(FALSE);

	/*
	if (m_pWndChild == NULL)
		m_pWndChild = (CCMGridCrossBeamItemDlg*)CreateOrActivateDlg(m_pDoc, CCMGridCrossBeamItemDlg::IDD, this);
	m_pWndChild->SetModify(FALSE);
	*/
}

void CCMGridCrossBeamListDlg::OnLButtonDblClkOnGrid(UINT key)
{
	CCMGridCrossBeamItemDlg* pWndChild = NULL;
	pWndChild = (CCMGridCrossBeamItemDlg*)CreateOrActivateDlg(m_pDoc, CCMGridCrossBeamItemDlg::IDD, this);
	ASSERT(pWndChild);
	pWndChild->SetInitModify(key);
	pWndChild->SetModify(TRUE);

	/*
	if (m_pWndChild == NULL)
		m_pWndChild = (CCMGridCrossBeamItemDlg*)CreateOrActivateDlg(m_pDoc, CCMGridCrossBeamItemDlg::IDD, this);
	
	m_pWndChild->SetInitModify(key);
	m_pWndChild->SetModify(TRUE);
	*/
}

void CCMGridCrossBeamListDlg::OnCmdBtnModify()
{
	CArray<UINT, UINT> aSelKey;
	int nCount = m_wndGrid.GetSelectedKeyList(aSelKey);
	if (nCount <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_CROSS_BEAM_NO_SELECTED));
		return;
	}
	if (nCount > 1)
	{
		AfxMessageBox(_LS(IDS_CMD_CROSS_BEAM_MORE_SELECTED));
		return;
	}
	
	CCMGridCrossBeamItemDlg* pWndChild = NULL;
	pWndChild = (CCMGridCrossBeamItemDlg*)CreateOrActivateDlg(m_pDoc, CCMGridCrossBeamItemDlg::IDD, this);
	ASSERT(pWndChild);
	pWndChild->SetInitModify(aSelKey[0]);
	pWndChild->SetModify(TRUE);

	/*
	if (m_pWndChild == NULL)
		m_pWndChild = (CCMGridCrossBeamItemDlg*)CreateOrActivateDlg(m_pDoc, CCMGridCrossBeamItemDlg::IDD, this);
	
	m_pWndChild->SetInitModify(aSelKey[0]);
	m_pWndChild->SetModify(TRUE);
	*/
}

void CCMGridCrossBeamListDlg::OnCmdBtnDelete()
{
	CArray<UINT, UINT> aSelKey;
	int nCount = m_wndGrid.GetSelectedKeyList(aSelKey);
	if (nCount <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD_CROSS_BEAM_NO_SELECTED_CROSS_BEAM));
		return;
	}

	T_CBEM_D data;
	CArray<CString, CString&> aSelName;
	for (int i=0; i<aSelKey.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetCbem(aSelKey[i], data);
		aSelName.Add(data.Name);
	}
	m_pDoc->m_pDataCtrl->DelCbem(aSelName);
}

void CCMGridCrossBeamListDlg::OnCmdBtnClose()
{
	/*
	if (m_pWndChild != NULL)
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_CBEM_CLOSE_DIALOG));
		return;
	}
	*/
	DestroyWindow();
}

BOOL CCMGridCrossBeamListDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndGrid.AddColInfo(_LS(IDS_WG_CMD_MAIN_GIRDER_NAME), 60, GX_VT_STRING, 0);
	m_wndGrid.AddColInfo(_LS(IDS_WG_CMD_MAIN_GIRDER_NODELIST),115, GX_VT_STRING, 0);
	m_wndGrid.Initialize(m_pDoc);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGridCrossBeamListDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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
	case D_UPDATE_UNIT:
	case D_UPDATE_SEL_ADD: 
	case D_UPDATE_SEL_DEL:
	case D_UPDATE_EXE_START: 
	case D_UPDATE_EXE_END:
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_POST_STEP_CHANGED:
		//DestroyWindow();
		m_wndGrid.MakeItemEx();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMGridCrossBeamListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	BOOL bCbemChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_CBEM_ADD):
		case(UR_CBEM_DEL):
		case(UR_CBEM_MFD):
		case(UR_CBEM_MFS):
			bCbemChanged = TRUE;
			break;
		default:
			break;
		}
		if (bCbemChanged)
			break;
	} // end of while

	if (bCbemChanged) m_wndGrid.MakeItemEx(); 
}
