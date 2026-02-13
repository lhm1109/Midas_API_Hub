#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlInitDeflectionDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\DgnData4Report.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnStlInitDeflectionDlg::CDgnStlInitDeflectionDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlInitDeflectionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlPhibFacDlg)
	m_nOption = 0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnStlInitDeflectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlPhibFacDlg)
	DDX_Radio(pDX, IDC_DGN_FACTOR_AR, m_nOption);
	DDX_Control(pDX, IDC_DGN_STL_EDIT_1,  m_edtOmegaWy);
	DDX_Control(pDX, IDC_DGN_STL_EDIT_2,  m_edtOmegaWz);
	DDX_Control(pDX, IDC_DGN_STL_UNIT_1,  m_untOmegaWy);
	DDX_Control(pDX, IDC_DGN_STL_UNIT_2,  m_untOmegaWz);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlInitDeflectionDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlPhibFacDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDgnStlInitDeflectionDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	m_edtOmegaWy.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtOmegaWz.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untOmegaWy.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untOmegaWz.SetUnitType(D_UNITSYS_BASE_LENGTH);

	UpdateData(FALSE);

	return TRUE;
}

void CDgnStlInitDeflectionDlg::Execute() 
{
	UpdateData(TRUE);
	if (!ErrorCheck())
		return;

	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if (aSelKey.GetSize() > 0)
	{
		for (int i = 0; i < aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(key, eData);
			// Beam, Truss.
			if (m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp))	rSelKey.Add(key);
		}
		T_INDF_D rData;
		rData.Initialize();
		if (m_nOption == 0)	// add/replace
		{
			rData.dOmegaWy = m_edtOmegaWy.GetEditValue();
			rData.dOmegaWz = m_edtOmegaWz.GetEditValue();
			if (rSelKey.GetSize() > 0)
			{
				// Initialize selected Element.
				if (m_pDoc->m_pDataCtrl->AddIndf(rSelKey, rData))	Initial_SelectItem();
			}
			else	
				AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE), MB_OK);
		}
		else	// Delete
		{
			// Initialize selected Element.
			if (m_pDoc->m_pDataCtrl->DelIndf(rSelKey))	Initial_SelectItem();
		}
	}
	else	
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
}

void CDgnStlInitDeflectionDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

BOOL CDgnStlInitDeflectionDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;
	if (m_edtOmegaWy.GetEditValue() <= 0.)		bCheck = FALSE;
	if (m_edtOmegaWz.GetEditValue() <= 0.)		bCheck = FALSE;
	if (!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_INDF_VALUE), MB_OK);
	return bCheck;
}

//void CDgnStlInitDeflectionDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
//{
//	HWND hWnd = GetSafeHwnd();
//	if (!hWnd || !IsWindow(hWnd)) return;
//	
//	switch(lHint)
//	{
//		case D_UPDATE_DEFAULT:
//			// do something...
//			break;
//		case D_UPDATE_BUFFER_BEFORE:
//			// do something...
//			break;
//		case D_UPDATE_BUFFER_AFTER:
//			{
//				// Add by ZINU.('02.3.21).
//				Update_InitDataByCode();
//				break;
//			}
//		case D_UPDATE_UNIT:
//			// do something...
//			break;
//		default:
//			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
//			ASSERT(TRUE);
//	}
//}

