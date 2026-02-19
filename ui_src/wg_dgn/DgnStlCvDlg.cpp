// DgnStlCvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlCvDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_STL_CVFT

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnStlCvDlg dialog


CDgnStlCvDlg::CDgnStlCvDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlCvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlCvDlg)
	m_nOption = -1;
	m_Cv = 0.0;
	m_bAutoCv = FALSE;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnStlCvDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlCvDlg)
	DDX_Radio(pDX, IDC_DGN_CV_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_CV_CV, m_Cv);
	DDX_Check(pDX, IDC_DGN_CV_PROGRAM, m_bAutoCv);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlCvDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlCvDlg)
	ON_BN_CLICKED(IDC_DGN_CV_AR, OnDgnCvAr)
	ON_BN_CLICKED(IDC_DGN_CV_DEL, OnDgnCvDel)
	ON_BN_CLICKED(IDC_DGN_CV_PROGRAM, OnDgnCvProgram)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlCvDlg message handlers

void CDgnStlCvDlg::Execute() 
{
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(key,eData);
			// Beam, Truss.
			if(m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp))	rSelKey.Add(key);
		}
		T_CVFT_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);
		  rData.bAuto = m_bAutoCv;
			rData.Cv	  = m_Cv;
			if(rSelKey.GetSize() > 0)
			{
	      // Initialize selected Element.
		  	if(m_pDoc->m_pDataCtrl->AddCvft(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelCvft(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnStlCvDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount())	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnStlCvDlg::ErrorCheck()
{
	BOOL bCheck=TRUE;

	if(m_bAutoCv == 0)		//	Not Automation
	{
		if(m_Cv <= 0. || m_Cv >= 10.)	bCheck = FALSE;
	}
	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_SHEAR_FACTOR),MB_OK);

	return bCheck;
}

BOOL CDgnStlCvDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
//**/  if(m_pDoc->m_pViewCtrl->SelectedElemCount())  m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	// Default 값 초기화
	Initial_Data();

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlCvDlg::Initial_Data()
{
	m_nOption	= 0;    //	add/replace
	m_bAutoCv = 0;  	//	Not Automation
	m_Cv	  	= 0.;
	// Add by ZINU.('01.1.3).
	Update_InitDataByCode();

	UpdateData(FALSE);
}

void CDgnStlCvDlg::OnDgnCvAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnStlCvDlg::OnDgnCvDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnStlCvDlg::OnDgnCvProgram() 
{
	UpdateData(TRUE);

	if(m_bAutoCv)
	{
		m_Cv = 0.;
	  GetDlgItem(IDC_DGN_CV_CV)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CV_CV)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}

// Add by ZINU.('01.1.3).
void CDgnStlCvDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
				// Add by ZINU.('01.1.3).
				Update_InitDataByCode();
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

// Add by ZINU.('01.1.3).
void CDgnStlCvDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleStl(ID_DGN_STL_CVFT, strTitle, bShow);
	if(bShow)
	{
		GetDlgItem(IDC_DGN_CV_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CV_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CV_CV)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CV_PROGRAM)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CV_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CV_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CV_CV)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CV_PROGRAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(FALSE);
	}
}