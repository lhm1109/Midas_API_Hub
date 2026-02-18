// DgnConBarSpaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBarSpaceDlg.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_QSort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarSpaceDlg dialog


CDgnConBarSpaceDlg::CDgnConBarSpaceDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConBarSpaceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBarSpaceDlg)
	m_RebarSpaceRadio = -1;
	m_iSpaceEdit = 0;
	//}}AFX_DATA_INIT
	m_TotalVSpace = _T("");
	m_arSelSpaceMM.RemoveAll();
	m_arSelSpaceIN.RemoveAll();
}


void CDgnConBarSpaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBarSpaceDlg)
	DDX_Control(pDX, IDC_DGN_SEL_LIST, m_aRebarSelList);
	DDX_Control(pDX, IDC_DGN_ALL_LIST, m_aRebarAllList);
	DDX_Radio(pDX, IDC_DGN_KS_BARSPACE_RADIO, m_RebarSpaceRadio);
	DDX_Text(pDX, IDC_DGN_SPACE_EDIT, m_iSpaceEdit);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConBarSpaceDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnConBarSpaceDlg)
	ON_BN_CLICKED(IDC_DGN_KS_BARSPACE_RADIO, OnDgnKsBarspaceRadio)
	ON_BN_CLICKED(IDC_DGN_ASTM_BARSPACE_RADIO, OnDgnKsBarspaceRadio)
	ON_BN_CLICKED(IDC_DGN_ADD_DB_BTN, OnDgnAddDbBtn)
	ON_BN_CLICKED(IDC_DGN_ADD_USER_BTN, OnDgnAddUserBtn)
	ON_BN_CLICKED(IDC_DGN_DEL_BTN, OnDgnDelBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarSpaceDlg message handlers


void CDgnConBarSpaceDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CDgnConBarSpaceDlg::OnOK() 
{
	if(m_aRebarSelList.GetCount() <= 50 && m_aRebarSelList.GetCount() > 0)
	{
		// Create String RebarSpaces by Selected Items.
		m_TotalVSpace = Get_SelectRebarSpace();
		CDialogMove::OnOK();
	}
	else	AfxMessageBox(_LS(IDS_DGN_CHK_SEL_OVER_ITEM50), MB_OK);	
}

BOOL CDgnConBarSpaceDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	// TODO: Add extra initialization here
	m_aRebarSelList.ResetContent();
	if(m_TotalVSpace!=_T(""))
	{
		// Saved Rebar Space.
		CStringArray aReBarSpace;
		BOOL bCheck = Cutting_RebarSpaceString(m_TotalVSpace, aReBarSpace);
		ASSERT(bCheck);
		// All Rebar Space.
		if(aReBarSpace[0].Right(1)==_T("\\"))	m_RebarSpaceRadio = 1;	// in.
		else															m_RebarSpaceRadio = 0;	// mm.
		Show_RebarKind(m_RebarSpaceRadio);
		for(int i=0; i<aReBarSpace.GetSize(); i++)
		{
			m_aRebarSelList.AddString(aReBarSpace[i]);
		}
		UpdateData(FALSE);
	}
	else	ASSERT(0);

	// Change User input Unit.
	CString strUnit=_T("");
	if(m_RebarSpaceRadio==0)	// mm.
	{
		GetDlgItem(IDC_DGN_KS_BARSPACE_RADIO)->GetWindowText(strUnit);
	}
	else if(m_RebarSpaceRadio==1)	// in.
	{
		GetDlgItem(IDC_DGN_ASTM_BARSPACE_RADIO)->GetWindowText(strUnit);
	}
	else	ASSERT(0);
	GetDlgItem(IDC_DGN_SPACE_UNIT)->SetWindowText(strUnit);

	return TRUE;
}

CString CDgnConBarSpaceDlg::Get_SelectRebarSpace()
{
	CString strSpace=_T("");
	int iSelListNum = m_aRebarSelList.GetCount();
	for(int i=0; i<iSelListNum; i++)
	{
		CString strCurSpace=_T("");
		m_aRebarSelList.GetText(i,strCurSpace);
		strSpace = strSpace + strCurSpace;
		if(i < iSelListNum-1)	strSpace = strSpace + _T(",");
	}
	return strSpace;
}

BOOL CDgnConBarSpaceDlg::Cutting_RebarSpaceString(CString str, CStringArray& arBarSpace)
{
	BOOL bCheck=TRUE;

	TCHAR ch;
	CString temp=_T("");
	int iLength = str.GetLength();
	for(int i=0; i<iLength; i++)
	{
		ch = str.GetAt(i);
		if(ch==',' || i+1 == iLength)
		{
			if(ch==',') arBarSpace.Add(temp);
			else			  arBarSpace.Add(temp+ch);
			temp=_T("");
		}
		else	temp = temp+ch;
	}
	return bCheck;
}

void CDgnConBarSpaceDlg::OnDgnKsBarspaceRadio() 
{
	int iPrevRadio = m_RebarSpaceRadio;
	UpdateData(TRUE);
	if(iPrevRadio!=m_RebarSpaceRadio)
	{
		if(iPrevRadio==0)	// mm.
		{
			m_arSelSpaceMM.RemoveAll();
			m_arSelSpaceMM.SetSize(m_aRebarSelList.GetCount());
			for(int i=0; i<m_aRebarSelList.GetCount(); i++)
			{
				CString strSpace=_T("");
				m_aRebarSelList.GetText(i,strSpace);
				m_arSelSpaceMM.SetAt(i,strSpace);
			}
		}
		else if(iPrevRadio==1)	// in.
		{
			m_arSelSpaceIN.RemoveAll();
			m_arSelSpaceIN.SetSize(m_aRebarSelList.GetCount());
			for(int i=0; i<m_aRebarSelList.GetCount(); i++)
			{
				CString strSpace=_T("");
				m_aRebarSelList.GetText(i,strSpace);
				m_arSelSpaceIN.SetAt(i,strSpace);
			}
		}
		// Remove All.
		m_aRebarSelList.ResetContent();
		Show_RebarKind(m_RebarSpaceRadio);
		// Change User input Unit.
		CString strUnit=_T("");
		if(m_RebarSpaceRadio==0)	// mm.
		{
			for(int i=0; i<m_arSelSpaceMM.GetSize(); i++)	m_aRebarSelList.AddString(m_arSelSpaceMM[i]);
			GetDlgItem(IDC_DGN_KS_BARSPACE_RADIO)->GetWindowText(strUnit);
		}
		else if(m_RebarSpaceRadio==1)	// in.
		{
			for(int i=0; i<m_arSelSpaceIN.GetSize(); i++)	m_aRebarSelList.AddString(m_arSelSpaceIN[i]);
			GetDlgItem(IDC_DGN_ASTM_BARSPACE_RADIO)->GetWindowText(strUnit);
		}
		else	ASSERT(0);
		GetDlgItem(IDC_DGN_SPACE_UNIT)->SetWindowText(strUnit);
		UpdateData(FALSE);
	}
}

BOOL CDgnConBarSpaceDlg::Show_RebarKind(int iRebarUnit)
{
	m_aRebarAllList.ResetContent();

	CStringArray aRebarSpaceAll;
	CDBLib::GetRebarSpaceAll(iRebarUnit, aRebarSpaceAll);
	for(int i=0; i<aRebarSpaceAll.GetSize(); i++)
	{
		m_aRebarAllList.AddString(aRebarSpaceAll.GetAt(i));
	}
	return TRUE;
}

void CDgnConBarSpaceDlg::OnDgnAddDbBtn() 
{
	// Get the indexes of all the selected items.
	CArray<int,int> arSelList;
	int iTotalSelCount = m_aRebarAllList.GetSelCount();
	arSelList.SetSize(iTotalSelCount);
	m_aRebarAllList.GetSelItems(iTotalSelCount, arSelList.GetData()); 
	int iCount=0;
	while(iCount < iTotalSelCount)
	{
		int iSelOrder = arSelList.GetAt(iCount);
		CString strSelData=_T("");
		m_aRebarAllList.GetText(iSelOrder, strSelData);
		m_aRebarSelList.AddString(strSelData);
		iCount++;
	}
	// Sort Selected space by ascending.
	Sort_SelRebarSpace();
}

void CDgnConBarSpaceDlg::OnDgnAddUserBtn() 
{
	UpdateData(TRUE);
	if(m_iSpaceEdit<1 || m_iSpaceEdit>500)
	{
		AfxMessageBox(_LS(IDS_DGN_USER_INPUT_SPACE_1_TO_500));
		return;
	}
	// Get Data from EditBox.
	CString strSpace=_T("");
	if(m_RebarSpaceRadio==0)			strSpace.Format(_T("@%d"), m_iSpaceEdit);		// mm.
	else if(m_RebarSpaceRadio==1)	strSpace.Format(_T("@%d\""), m_iSpaceEdit);	// in.
	else	ASSERT(0);
	m_aRebarSelList.AddString(strSpace);
	// Sort Selected space by ascending.
	Sort_SelRebarSpace();
}

void CDgnConBarSpaceDlg::OnDgnDelBtn() 
{
	// Get the indexes of all the selected items.
	CArray<int,int> arSelList;
	int iTotalSelCount = m_aRebarSelList.GetSelCount();
	arSelList.SetSize(iTotalSelCount);
	m_aRebarSelList.GetSelItems(iTotalSelCount, arSelList.GetData()); 
	int iCount = iTotalSelCount;
	while(iCount > 0)
	{
		int iSelOrder = arSelList.GetAt(iCount-1);
		m_aRebarSelList.DeleteString(iSelOrder);
		iCount--;
	}
	// Sort Selected space by ascending.
	Sort_SelRebarSpace();
}

BOOL CDgnConBarSpaceDlg::Sort_SelRebarSpace()
{
	int iTotSelSpace = m_aRebarSelList.GetCount();

	int* pSortSpace = new int[iTotSelSpace];
	for(int i=0; i<iTotSelSpace; i++)
	{
		CString strList=_T("");
		m_aRebarSelList.GetText(i,strList);
		int iTextCount=0;
		if(m_RebarSpaceRadio==0)			iTextCount = strList.GetLength()-1;	// mm.
		else if(m_RebarSpaceRadio==1)	iTextCount = strList.GetLength()-2;	// in.
		else	ASSERT(0);
		CString strSpace = strList.Mid(1,iTextCount);	// 0 is @.
		pSortSpace[i] = _ttoi(strSpace);
	}
	// Remove All data before saving.
	m_aRebarSelList.ResetContent();
	// Sorting by Ascending order.
	CQSort::QSortInt(pSortSpace, iTotSelSpace);
	for(int i=0; i<iTotSelSpace; i++)
	{
		int iSortSpace=0;
		if(i==0)
		{
			CString strSortSpace=_T("");
			if(m_RebarSpaceRadio==0)			strSortSpace.Format(_T("@%d"),   pSortSpace[i]);	// mm.
			else if(m_RebarSpaceRadio==1)	strSortSpace.Format(_T("@%d\""), pSortSpace[i]);	// in.
			else	ASSERT(0);
			m_aRebarSelList.AddString(strSortSpace);
		}
		if(i < iTotSelSpace-1)
		{
			if(pSortSpace[i] < pSortSpace[i+1])
			{
				CString strSortSpace=_T("");
				if(m_RebarSpaceRadio==0)			strSortSpace.Format(_T("@%d"),   pSortSpace[i+1]);	// mm.
				else if(m_RebarSpaceRadio==1)	strSortSpace.Format(_T("@%d\""), pSortSpace[i+1]);	// in.
				else	ASSERT(0);
				m_aRebarSelList.AddString(strSortSpace);
			}
		}
	}
	delete[] pSortSpace;

	return TRUE;
}
