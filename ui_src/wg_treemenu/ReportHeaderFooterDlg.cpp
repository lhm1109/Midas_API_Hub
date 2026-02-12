// ReportHeaderFooterDlg.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReportHeaderFooterDlg.h"
#include "ReportCommonFunc.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"


#include "..\wg_main\wg_mainRes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportHeaderFooterDlg dialog

CReportHeaderFooterDlg::CReportHeaderFooterDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CReportHeaderFooterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportHeaderFooterDlg)
	m_bApply = FALSE;
	//}}AFX_DATA_INIT

	m_aSelectedHead.RemoveAll();
	m_aSelectedFoot.RemoveAll();
	m_aAllProjStr.RemoveAll();

	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
}

CReportHeaderFooterDlg::~CReportHeaderFooterDlg() 
{
}

void CReportHeaderFooterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportHeaderFooterDlg)
	DDX_Control(pDX, IDC_TM_REPORT_PROJ_LIST, m_listProj);
	DDX_Control(pDX, IDC_TM_REPORT_HEAD_LIST, m_listHead);
	DDX_Control(pDX, IDC_TM_REPORT_FOOT_LIST, m_listFoot);
	DDX_Control(pDX, IDC_TM_REPORT_HEAD_UP_BTN, m_btnHeadUp);
	DDX_Control(pDX, IDC_TM_REPORT_FOOT_UP_BTN, m_btnFootUp);
	DDX_Control(pDX, IDC_TM_REPORT_HEAD_DW_BTN, m_btnHeadDown);
	DDX_Control(pDX, IDC_TM_REPORT_FOOT_DW_BTN, m_btnFootDown);
	DDX_Check  (pDX, IDC_TM_REPORT_FOOTER_APPLY_CHK, m_bApply);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CReportHeaderFooterDlg, CDialogMove)
	//{{AFX_MSG_MAP(CReportHeaderFooterDlg)
	ON_BN_CLICKED(IDC_TM_REPORT_PROJECT_BTN     , OnProjectButton  )
	ON_BN_CLICKED(IDC_TM_REPORT_HEAD_SEL_BTN    , OnHeadSelButton  )
	ON_BN_CLICKED(IDC_TM_REPORT_HEAD_UNSEL_BTN  , OnHeadUnselButton)
	ON_BN_CLICKED(IDC_TM_REPORT_FOOT_SEL_BTN    , OnFootSelButton  )
	ON_BN_CLICKED(IDC_TM_REPORT_FOOT_UNSEL_BTN  , OnFootUnselButton)
	ON_BN_CLICKED(IDC_TM_REPORT_HEAD_UP_BTN     , OnHeadUpButton   )
	ON_BN_CLICKED(IDC_TM_REPORT_HEAD_DW_BTN     , OnHeadDownButton )
	ON_BN_CLICKED(IDC_TM_REPORT_FOOT_UP_BTN     , OnFootUpButton   )
	ON_BN_CLICKED(IDC_TM_REPORT_FOOT_DW_BTN     , OnFootDownButton )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportHeaderFooterDlg message handlers

BOOL CReportHeaderFooterDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	SetIcon();
	InitHeadFootArray();
	InitProjList();
	InitHeadList();
	InitFootList();

	m_bApply = TRUE;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CReportHeaderFooterDlg::OnOK() 
{
	UpdateData(TRUE);

	if(!Dlg2Data()) return;
	if(m_bApply)
		CReportCommonFunc::InsertReportHeaderFooter();

	CDialogMove::OnOK();
}

void CReportHeaderFooterDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}
void CReportHeaderFooterDlg::PostNcDestroy() 
{
	CDialogMove::PostNcDestroy();
}

void CReportHeaderFooterDlg::OnHeadSelButton() 
{
	DoSelectHeadItem();
}
void CReportHeaderFooterDlg::OnFootSelButton() 
{
	DoSelectFootItem();
}
void CReportHeaderFooterDlg::OnHeadUnselButton() 
{
	DoUnselectHeadItem();
}
void CReportHeaderFooterDlg::OnFootUnselButton() 
{
	DoUnselectFootItem();
}
void CReportHeaderFooterDlg::OnProjectButton() 
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_FILE_PJINFO, 0), 0);
}
void CReportHeaderFooterDlg::OnHeadUpButton() 
{
	DoUpHeadItem();
}
void CReportHeaderFooterDlg::OnHeadDownButton() 
{
	DoDownHeadItem();
}
void CReportHeaderFooterDlg::OnFootUpButton() 
{
	DoUpFootItem();
}
void CReportHeaderFooterDlg::OnFootDownButton() 
{
	DoDownFootItem();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CReportHeaderFooterDlg::SetIcon()
{
	m_btnHeadUp.SetIcon(IDI_TM_UP);
	m_btnHeadUp.SetFlat(TRUE);
	m_btnHeadUp.SetAlign(CButtonSTNew::ST_ALIGN_VERT);
	m_btnHeadUp.SetWindowText(_T(""));

	m_btnFootUp.SetIcon(IDI_TM_UP);
	m_btnFootUp.SetFlat(TRUE);
	m_btnFootUp.SetAlign(CButtonSTNew::ST_ALIGN_VERT);
	m_btnFootUp.SetWindowText(_T(""));

	m_btnHeadDown.SetIcon(IDI_TM_DOWN);
	m_btnHeadDown.SetFlat(TRUE);
	m_btnHeadDown.SetAlign(CButtonSTNew::ST_ALIGN_VERT);
	m_btnHeadDown.SetWindowText(_T(""));

	m_btnFootDown.SetIcon(IDI_TM_DOWN);
	m_btnFootDown.SetFlat(TRUE);
	m_btnFootDown.SetAlign(CButtonSTNew::ST_ALIGN_VERT);
	m_btnFootDown.SetWindowText(_T(""));
}

void CReportHeaderFooterDlg::InitHeadFootArray()
{
	m_aSelectedHead.RemoveAll();
	m_aSelectedFoot.RemoveAll();

	if(m_pDoc->m_pAttrCtrl2->ExistUftr())
	{
		T_UFTR_D UftrD;
		if(m_pDoc->m_pAttrCtrl2->GetUftr(UftrD))
		{
			for(int i=0; i<UftrD.nHeaderNum; i++)
				m_aSelectedHead.Add(UftrD.aHeader[i]);

			for(int i=0; i<UftrD.nFooterNum; i++)
				m_aSelectedFoot.Add(UftrD.aFooter[i]);
		}
	}

	m_aAllProjStr.RemoveAll();
	CReportCommonFunc::GetAllProjStr(m_aAllProjStr);
}

void CReportHeaderFooterDlg::InitProjList()
{
	m_listProj.ResetContent();

	int ix=0;
	for(int i=D_UFTR_START+1; i<D_UFTR_END; i++)
	{
		if(!IsSelectedHead(i) && !IsSelectedFoot(i))
		{
			ix = m_listProj.AddString(m_aAllProjStr[i]);
			m_listProj.SetItemData(ix, i);
		}
	}
	if(m_listProj.GetCount() > 0)
		m_listProj.SetSel(0);
}

BOOL CReportHeaderFooterDlg::IsSelectedHead(UINT nIndex)
{
	BOOL bResult=FALSE;
	CMapEx<UINT,UINT,int,int> mSelected;
	for(int i=0; i<m_aSelectedHead.GetSize(); i++)
	{
		mSelected.SetAt(m_aSelectedHead[i], 1);
	}

	int nData=0;
	if(mSelected.Lookup(nIndex, nData) && nData==1)
		bResult=TRUE;

	return bResult;
}

BOOL CReportHeaderFooterDlg::IsSelectedFoot(UINT nIndex)
{
	BOOL bResult=FALSE;
	CMapEx<UINT,UINT,int,int> mSelected;
	for(int i=0; i<m_aSelectedFoot.GetSize(); i++)
	{
		mSelected.SetAt(m_aSelectedFoot[i], 1);
	}
	
	int nData=0;
	if(mSelected.Lookup(nIndex, nData) && nData==1)
		bResult=TRUE;
	
	return bResult;
}

void CReportHeaderFooterDlg::InitHeadList()
{
	m_listHead.ResetContent();

	int ix=0;
	int nIndex=0;
	for(int i=0; i<m_aSelectedHead.GetSize(); i++)
	{
		nIndex = m_aSelectedHead[i];
		ix = m_listHead.AddString(m_aAllProjStr[nIndex]);
		m_listHead.SetItemData(ix, nIndex);
	}
	if(m_aSelectedHead.GetSize() > 0) 
		m_listHead.SetSel(0);
}

void CReportHeaderFooterDlg::InitFootList()
{
	m_listFoot.ResetContent();
	
	int ix=0;
	int nIndex=0;
	for(int i=0; i<m_aSelectedFoot.GetSize(); i++)
	{
		nIndex = m_aSelectedFoot[i];
		ix = m_listFoot.AddString(m_aAllProjStr[nIndex]);
		m_listFoot.SetItemData(ix, nIndex);
	}
	if(m_aSelectedFoot.GetSize() > 0) 
		m_listFoot.SetSel(0);
}

void CReportHeaderFooterDlg::DoSelectHeadItem()
{
	int nCount = m_listProj.GetSelCount();
	if(nCount <= 0) return;

	UINT nIndex=0;
	CArray<int, int> aSelectedItem;
	aSelectedItem.SetSize(nCount);
	m_listProj.GetSelItems(nCount, aSelectedItem.GetData()); 

	for(int i=0; i<nCount; i++)
	{
		nIndex = m_listProj.GetItemData(aSelectedItem[i]);
		m_aSelectedHead.Add(nIndex);
	}

	InitProjList();
	InitHeadList();
}

void CReportHeaderFooterDlg::DoSelectFootItem()
{
	int nCount = m_listProj.GetSelCount();
	if(nCount <= 0) return;
	
	UINT nIndex=0;
	CArray<int, int> aSelectedItem;
	aSelectedItem.SetSize(nCount);
	m_listProj.GetSelItems(nCount, aSelectedItem.GetData()); 
	
	for(int i=0; i<nCount; i++)
	{
		nIndex = m_listProj.GetItemData(aSelectedItem[i]);
		m_aSelectedFoot.Add(nIndex);
	}
	
	InitProjList();
	InitFootList();
}

void CReportHeaderFooterDlg::DoUnselectHeadItem()
{
	int nCount = m_listHead.GetSelCount();
	if(nCount <= 0) return;

	UINT nIndex=0;
	CArray<int, int> aSelectedItem;
	aSelectedItem.SetSize(nCount);
	m_listHead.GetSelItems(nCount, aSelectedItem.GetData()); 

	for(int i=m_aSelectedHead.GetSize()-1; i>=0; i--)
	{
		int k = 0;
		for(k=0; k<nCount; k++)
		{
			nIndex = m_listHead.GetItemData(aSelectedItem[k]);
			if(nIndex == m_aSelectedHead[i]) break;
		}
		if(k != nCount) m_aSelectedHead.RemoveAt(i);
	}

	InitProjList();
	InitHeadList();
}

void CReportHeaderFooterDlg::DoUnselectFootItem()
{
	int nCount = m_listFoot.GetSelCount();
	if(nCount <= 0) return;
	
	UINT nIndex=0;
	CArray<int, int> aSelectedItem;
	aSelectedItem.SetSize(nCount);
	m_listFoot.GetSelItems(nCount, aSelectedItem.GetData()); 
	
	for(int i=m_aSelectedFoot.GetSize()-1; i>=0; i--)
	{
		int k = 0;
		for(k=0; k<nCount; k++)
		{
			nIndex = m_listFoot.GetItemData(aSelectedItem[k]);
			if(nIndex == m_aSelectedFoot[i]) break;
		}
		if(k != nCount) m_aSelectedFoot.RemoveAt(i);
	}
	
	InitProjList();
	InitFootList();
}

void CReportHeaderFooterDlg::DoUpHeadItem()
{
	int nCount = m_listHead.GetSelCount();
	if(nCount <= 0) return;
	
	CArray<int, int>  aSelectedItem;
	aSelectedItem.SetSize(nCount);
	m_listHead.GetSelItems(nCount, aSelectedItem.GetData()); 
	
	CMap<UINT,UINT,int,int> mSelectedItem;
	
	CArray<UINT, UINT> aSelectedHeadNew;
	aSelectedHeadNew.SetSize(m_aSelectedHead.GetSize());
	
	BOOL bFirst=FALSE;
	int nSel=0, nSelIndex=-1;
	for(int i=0; i<m_listHead.GetCount(); i++)
	{
		nSel = m_listHead.GetSel(i);
		if(nSel==1)
			nSelIndex = i;
	}
	bFirst = nSelIndex==nCount-1;

	UINT nIndex=0;
	int nAdd=0;
	for(int i=0; i<nCount; i++)
	{
		nIndex = m_listHead.GetItemData(aSelectedItem[i]);
		for(int k=0; k<m_aSelectedHead.GetSize(); k++)
		{
			if(nIndex == m_aSelectedHead[k])
			{
				if(k-1<0)       nAdd = k; // 하나 선택시 맨 처음 일때
				else if(bFirst) nAdd = k; // 여러개 선택시 선택한 그룹이 처음 일때
				else            nAdd = k-1;
				
				aSelectedHeadNew[nAdd] = nIndex;
				break;
			}
		}
		
		mSelectedItem.SetAt(nIndex, nAdd);
	}
	
	int nData=0, k=aSelectedHeadNew.GetSize()-1;
	for(int i=aSelectedHeadNew.GetSize()-1; i>=0; i--)
	{
		if(k<0) break;
		
		if(aSelectedHeadNew[i] != 0)
			continue;
		
		if(mSelectedItem.Lookup(m_aSelectedHead[k], nData))
		{
			++i; --k;
			continue;
		}
		
		aSelectedHeadNew[i] = m_aSelectedHead[k];
		--k;
	}
	m_aSelectedHead.Copy(aSelectedHeadNew);
	
	InitHeadList();

	// select
	for(int i=0; i<m_listHead.GetCount(); i++)
		m_listHead.SetSel(i, FALSE);
	
	nIndex=0; nData=0;
	POSITION Pos = mSelectedItem.GetStartPosition();
	while(Pos)
	{
		mSelectedItem.GetNextAssoc(Pos,nIndex,nData);
		m_listHead.SetSel(nData);
	}
}

void CReportHeaderFooterDlg::DoDownHeadItem()
{
	int nCount = m_listHead.GetSelCount();
	if(nCount <= 0) return;
	
	CArray<int, int>  aSelectedItem;
	aSelectedItem.SetSize(nCount);
	m_listHead.GetSelItems(nCount, aSelectedItem.GetData()); 
	
	CMap<UINT,UINT,int,int> mSelectedItem;
	
	CArray<UINT, UINT> aSelectedHeadNew;
	aSelectedHeadNew.SetSize(m_aSelectedHead.GetSize());
	
	UINT nIndex=0;
	int nAdd=0;
	for(int i=0; i<nCount; i++)
	{
		nIndex = m_listHead.GetItemData(aSelectedItem[i]);
		for(int k=0; k<m_aSelectedHead.GetSize(); k++)
		{
			if(nIndex == m_aSelectedHead[k])
			{
				if(k+1==m_aSelectedHead.GetSize())              nAdd = k; // 하나 선택시 맨 마지막 일때
				else if(k==m_aSelectedHead.GetSize()-nCount+i)  nAdd = k; // 여러개 선택시 선택한 그룹이 마지막 일때
				else                                            nAdd = k+1;

				aSelectedHeadNew[nAdd] = nIndex;
				break;
			}
		}
		
		mSelectedItem.SetAt(nIndex, nAdd);
	}
	
	int nData=0, k=0;
	for(int i=0; i<aSelectedHeadNew.GetSize(); i++)
	{
		if(k==aSelectedHeadNew.GetSize()) break;

		if(aSelectedHeadNew[i] != 0)
			continue;

		if(mSelectedItem.Lookup(m_aSelectedHead[k], nData))
		{
			--i; ++k;
			continue;
		}

		aSelectedHeadNew[i] = m_aSelectedHead[k];
		++k;
	}
	m_aSelectedHead.Copy(aSelectedHeadNew);
	
	InitHeadList();

	// select
	for(int i=0; i<m_listHead.GetCount(); i++)
		m_listHead.SetSel(i, FALSE);

	nIndex=0; nData=0;
	POSITION Pos = mSelectedItem.GetStartPosition();
	while(Pos)
	{
		mSelectedItem.GetNextAssoc(Pos,nIndex,nData);
		m_listHead.SetSel(nData);
	}
}

void CReportHeaderFooterDlg::DoUpFootItem()
{
	int nCount = m_listFoot.GetSelCount();
	if(nCount <= 0) return;
	
	CArray<int, int>  aSelectedItem;
	aSelectedItem.SetSize(nCount);
	m_listFoot.GetSelItems(nCount, aSelectedItem.GetData()); 
	
	CMap<UINT,UINT,int,int> mSelectedItem;
	
	CArray<UINT, UINT> aSelectedHeadNew;
	aSelectedHeadNew.SetSize(m_aSelectedFoot.GetSize());
	
	BOOL bFirst=FALSE;
	int nSel=0, nSelIndex=-1;
	for(int i=0; i<m_listFoot.GetCount(); i++)
	{
		nSel = m_listFoot.GetSel(i);
		if(nSel==1)
			nSelIndex = i;
	}
	bFirst = nSelIndex==nCount-1;
	
	UINT nIndex=0;
	int nAdd=0;
	for(int i=0; i<nCount; i++)
	{
		nIndex = m_listFoot.GetItemData(aSelectedItem[i]);
		for(int k=0; k<m_aSelectedFoot.GetSize(); k++)
		{
			if(nIndex == m_aSelectedFoot[k])
			{
				if(k-1<0)       nAdd = k; // 하나 선택시 맨 처음 일때
				else if(bFirst) nAdd = k; // 여러개 선택시 선택한 그룹이 처음 일때
				else            nAdd = k-1;
				
				aSelectedHeadNew[nAdd] = nIndex;
				break;
			}
		}
		
		mSelectedItem.SetAt(nIndex, nAdd);
	}
	
	int nData=0, k=aSelectedHeadNew.GetSize()-1;
	for(int i=aSelectedHeadNew.GetSize()-1; i>=0; i--)
	{
		if(k<0) break;
		
		if(aSelectedHeadNew[i] != 0)
			continue;
		
		if(mSelectedItem.Lookup(m_aSelectedFoot[k], nData))
		{
			++i; --k;
			continue;
		}
		
		aSelectedHeadNew[i] = m_aSelectedFoot[k];
		--k;
	}
	m_aSelectedFoot.Copy(aSelectedHeadNew);
	
	InitFootList();
	
	// select
	for(int i=0; i<m_listFoot.GetCount(); i++)
		m_listFoot.SetSel(i, FALSE);
	
	nIndex=0; nData=0;
	POSITION Pos = mSelectedItem.GetStartPosition();
	while(Pos)
	{
		mSelectedItem.GetNextAssoc(Pos,nIndex,nData);
		m_listFoot.SetSel(nData);
	}
}

void CReportHeaderFooterDlg::DoDownFootItem()
{
	int nCount = m_listFoot.GetSelCount();
	if(nCount <= 0) return;
	
	CArray<int, int>  aSelectedItem;
	aSelectedItem.SetSize(nCount);
	m_listFoot.GetSelItems(nCount, aSelectedItem.GetData()); 
	
	CMap<UINT,UINT,int,int> mSelectedItem;
	
	CArray<UINT, UINT> aSelectedHeadNew;
	aSelectedHeadNew.SetSize(m_aSelectedFoot.GetSize());
	
	UINT nIndex=0;
	int nAdd=0;
	for(int i=0; i<nCount; i++)
	{
		nIndex = m_listFoot.GetItemData(aSelectedItem[i]);
		for(int k=0; k<m_aSelectedFoot.GetSize(); k++)
		{
			if(nIndex == m_aSelectedFoot[k])
			{
				if(k+1==m_aSelectedFoot.GetSize())              nAdd = k; // 하나 선택시 맨 마지막 일때
				else if(k==m_aSelectedFoot.GetSize()-nCount+i)  nAdd = k; // 여러개 선택시 선택한 그룹이 마지막 일때
				else                                            nAdd = k+1;
				
				aSelectedHeadNew[nAdd] = nIndex;
				break;
			}
		}
		
		mSelectedItem.SetAt(nIndex, nAdd);
	}
	
	int nData=0, k=0;
	for(int i=0; i<aSelectedHeadNew.GetSize(); i++)
	{
		if(k==aSelectedHeadNew.GetSize()) break;
		
		if(aSelectedHeadNew[i] != 0)
			continue;
		
		if(mSelectedItem.Lookup(m_aSelectedFoot[k], nData))
		{
			--i; ++k;
			continue;
		}
		
		aSelectedHeadNew[i] = m_aSelectedFoot[k];
		++k;
	}
	m_aSelectedFoot.Copy(aSelectedHeadNew);
	
	InitFootList();
	
	// select
	for(int i=0; i<m_listFoot.GetCount(); i++)
		m_listFoot.SetSel(i, FALSE);
	
	nIndex=0; nData=0;
	POSITION Pos = mSelectedItem.GetStartPosition();
	while(Pos)
	{
		mSelectedItem.GetNextAssoc(Pos,nIndex,nData);
		m_listFoot.SetSel(nData);
	}
}

BOOL CReportHeaderFooterDlg::Dlg2Data() 
{
	int nHeaderNum=0, nFooterNum=0;
	nHeaderNum = m_aSelectedHead.GetSize();
	nFooterNum = m_aSelectedFoot.GetSize();
	
	if(nHeaderNum <=0 && nFooterNum <=0)
	{
		AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_CHOOSE_HEADER_FOOTER));
		return FALSE;
	}
	
	T_UFTR_D UftrD;
	
	UftrD.nHeaderNum = nHeaderNum;
	UftrD.nFooterNum = nFooterNum;
	for(int i=0; i<nHeaderNum; i++)
		UftrD.aHeader[i] = m_aSelectedHead[i];
	for(int i=0; i<nFooterNum; i++)
		UftrD.aFooter[i] = m_aSelectedFoot[i];
	
	if(!m_pDoc->m_pDataCtrl->AddUftr(UftrD)) 
		return FALSE;
	
	return TRUE;
}
