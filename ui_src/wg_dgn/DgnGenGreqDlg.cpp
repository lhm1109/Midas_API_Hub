// DgnGenGreqDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenGreqDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_GREQ

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenGreqDlg dialog


CDgnGenGreqDlg::CDgnGenGreqDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenGreqDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenGreqDlg)
	m_nOption = -1;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnGenGreqDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenGreqDlg)
	DDX_Radio(pDX, IDC_DGN_GREQ_AR, m_nOption);
	DDX_Control(pDX, IDC_DGN_GREQ_CON, m_ConEqGrade);
	DDX_Control(pDX, IDC_DGN_GREQ_STL, m_StlEqGrade);
	DDX_Control(pDX, IDC_DGN_GREQ_SRC, m_SrcEqGrade);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenGreqDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenGreqDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_GREQ_AR, OnDgnGreqAr)
	ON_BN_CLICKED(IDC_DGN_GREQ_DEL, OnDgnGreqDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenGreqDlg message handlers

BOOL CDgnGenGreqDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenGreqDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnGenGreqDlg::OnDgnExecute() 
{
	UpdateData(TRUE);

	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	CArray<T_GREQ_K, T_GREQ_K> arKey; arKey.RemoveAll();
	CArray<T_GREQ_D, T_GREQ_D&> arData; arData.RemoveAll();
	if(aSelKey.GetSize() > 0)
	{
		T_GREQ_D rData; rData.Initialize();
		rData.iEqGradeCon = m_ConEqGrade.GetCurSel();
		rData.iEqGradeStl = m_StlEqGrade.GetCurSel();
		rData.iEqGradeSrc = m_SrcEqGrade.GetCurSel();

		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(key,eData);
			// Beam, Truss.
			if(m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp) || m_pDoc->m_pAttrCtrl->IsWall(eData.eltyp))	
			{
				T_MATD_D MatdData; MatdData.Initialize();
				if(m_pDoc->m_pAttrCtrl->GetMatlDesign(eData.elmat, MatdData))
				{
					if(MatdData.Type == _T("C"))
					{
						rData.strMatlType = _T("C");
						arKey.Add(key);
						arData.Add(rData);
					}
					else if(MatdData.Type == _T("S"))
					{
						rData.strMatlType = _T("S");
						arKey.Add(key);
						arData.Add(rData);
					}
					else if(MatdData.Type == _T("SRC"))
					{
						rData.strMatlType = _T("SRC");
						arKey.Add(key);
						arData.Add(rData);
					}
				}
			}
		}
		if(m_nOption==0)	// add/replace
	  {
			if(arKey.GetSize() > 0)
			{
				// Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddGreq(arKey,arData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelGreq(arKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnGenGreqDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

void CDgnGenGreqDlg::Initial_Data()
{
	m_nOption  = 0;	// add/replace

	m_ConEqGrade.ResetContent();
	CStringArray ConEqName; ConEqName.RemoveAll();
	CDBLib::GetConEqGradeList(ConEqName);
	for(int i=0; i<ConEqName.GetSize(); i++)	m_ConEqGrade.AddString(ConEqName[i]);
	if(ConEqName.GetSize() > 0) m_ConEqGrade.SetCurSel(0);
	AdjustComboListBox(m_ConEqGrade);


	m_SrcEqGrade.ResetContent();
	CStringArray SrcEqName; SrcEqName.RemoveAll();
	CDBLib::GetSrcEqGradeList(SrcEqName);
	for(int i=0; i<SrcEqName.GetSize(); i++)	m_SrcEqGrade.AddString(SrcEqName[i]);
	if(SrcEqName.GetSize() > 0) m_SrcEqGrade.SetCurSel(0);
	AdjustComboListBox(m_SrcEqGrade);

	Update_InitDataByCode();

	UpdateData(FALSE);
}

void CDgnGenGreqDlg::OnDgnGreqAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnGenGreqDlg::OnDgnGreqDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

// Add by ZINU.('02.3.21).
void CDgnGenGreqDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			Update_InitDataByCode();
			break;
		case D_UPDATE_UNIT:
			// do something...
			break;
		default:
			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
			ASSERT(TRUE);
	}
}

void CDgnGenGreqDlg::AdjustComboListBox(CComboBox& rComboBox)
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = rComboBox.GetDC();
	for (int i=0;i < rComboBox.GetCount();i++)
	{
		rComboBox.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	rComboBox.ReleaseDC(pDC);

//  dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2*::GetSystemMetrics(SM_CXEDGE);

	if (rComboBox.GetDroppedWidth() < dx)
	{
		rComboBox.SetDroppedWidth(dx);
		ASSERT(rComboBox.GetDroppedWidth() == dx);
	}
}

void CDgnGenGreqDlg::Update_InitDataByCode()
{
	CString strTitle=_T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleGen(ID_DGN_GEN_GREQ, strTitle, bShow);
	if(bShow)
	{
		GetDlgItem(IDC_DGN_GREQ_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_GREQ_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_GREQ_CON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_GREQ_STL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_GREQ_SRC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_GREQ_CON_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_GREQ_STL_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_GREQ_SRC_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_GREQ_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_GREQ_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_GREQ_CON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_GREQ_STL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_GREQ_SRC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_GREQ_CON_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_GREQ_STL_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_GREQ_SRC_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(FALSE);
	}
	//
	int nCurSel = m_StlEqGrade.GetCurSel();
	m_StlEqGrade.ResetContent();
	CStringArray StlEqName; StlEqName.RemoveAll();
	CDBLib::GetStlEqGradeList(StlEqName);
	for (int i = 0; i<StlEqName.GetSize(); i++)	
		m_StlEqGrade.AddString(StlEqName[i]);
	if (StlEqName.GetSize() > 0)
	{
		nCurSel = max(nCurSel, 0);
		m_StlEqGrade.SetCurSel(nCurSel);
	}
	AdjustComboListBox(m_StlEqGrade);
}