// DgnRCRatingPrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCRatingPrintDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_MsgDll.h"



#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_main\wg_mainres2.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRationPrintDlg dialog


CDgnRCRatingPrintDlg::CDgnRCRatingPrintDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnRCRatingPrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRationPrintDlg)
	m_nOption = -1;	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnRCRatingPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRationPrintDlg)
	DDX_Radio(pDX, IDC_DGN_AR, m_nOption);
	DDX_Control(pDX, IDC_DGN_I_RADIO, m_ratioI);
	DDX_Control(pDX, IDC_DGN_J_RADIO, m_ratioJ);
	DDX_Control(pDX, IDC_DGN_IJ_RADIO, m_ratioIJ);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnRCRatingPrintDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnRationPrintDlg)
// 	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
// 	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_AR, OnDgnAr)
	ON_BN_CLICKED(IDC_DGN_DEL, OnDgnDel)	
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRationPrintDlg message handlers

// void CDgnRCRatingPrintDlg::OnDgnClose() 
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CDgnRCRatingPrintDlg::Execute()
{
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		/*
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetElem(key,eData)) continue;
			if(CDBLib::IsPscDgnFor1st(key,TRUE))	rSelKey.Add(key);
		}
		*/

		T_RPRC_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
		{
			if(m_ratioI.GetCheck())
				rData.iPrintChk = 0;
			else if(m_ratioJ.GetCheck())
				rData.iPrintChk = 1;
			else //if(m_ratioIJ.GetCheck())
				rData.iPrintChk = 2;
			
			if(aSelKey.GetSize() > 0)
			{
	      // Initialize selected Element.
		  	if(m_pDoc->m_pDataCtrl->AddRprc(aSelKey,rData))	Initial_SelectItem();
			}
			//else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
		else	// Delete
		{
			// Initialize selected Element.
			if(m_pDoc->m_pDataCtrl->DelRprc(aSelKey))	Initial_SelectItem();
		}    
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnRCRatingPrintDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnRCRatingPrintDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();	
	
	Initial_Data();
	if(!SetEnableByDgnCode())
		return TRUE;	
	
	SetEnableByCtrl();

	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRCRatingPrintDlg::Initial_Data()
{
	m_nOption	= 0;      // add/replace
	m_ratioI.SetCheck(TRUE);
	m_ratioJ.SetCheck(FALSE);
	m_ratioIJ.SetCheck(FALSE);
	
	UpdateData(FALSE);
}

void CDgnRCRatingPrintDlg::SetEnableByCtrl()
{
	UpdateData(TRUE);

	BOOL bStateAddMod = m_nOption == 0;	

	GetDlgItem(IDC_POSITION_GROUP)->EnableWindow(bStateAddMod);
	GetDlgItem(IDC_DGN_I_RADIO)->EnableWindow(bStateAddMod);
	GetDlgItem(IDC_DGN_J_RADIO)->EnableWindow(bStateAddMod);
	GetDlgItem(IDC_DGN_IJ_RADIO)->EnableWindow(bStateAddMod);		
}

BOOL CDgnRCRatingPrintDlg::SetEnableByDgnCode()
{
	T_RCDC_D Data;
	Data.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRcdc(Data);
	
	if(CDBLib::IsRcRatingCodeKRSince10(Data.nRatingCode))
	{
		GetDlgItem(IDC_DGN_OPTION_GROUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DEL)->EnableWindow(TRUE);

		GetDlgItem(IDC_POSITION_GROUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_I_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_J_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_IJ_RADIO)->EnableWindow(TRUE);		

		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);	

		return TRUE;
	}
	else //BRIDGE_LOAD_RATING
	{
		GetDlgItem(IDC_DGN_OPTION_GROUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DEL)->EnableWindow(FALSE);

		GetDlgItem(IDC_POSITION_GROUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_I_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_J_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_IJ_RADIO)->EnableWindow(FALSE);

		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(FALSE);	

		return FALSE;
	}
}

void CDgnRCRatingPrintDlg::OnDgnAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);

	SetEnableByCtrl();
}

void CDgnRCRatingPrintDlg::OnDgnDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);

	SetEnableByCtrl();
}

void CDgnRCRatingPrintDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
				UpdateBuffer();
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

void CDgnRCRatingPrintDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bSpfcChanged = FALSE;
	BOOL bStorChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		switch(nCmd)
		{
		case(UR_RCDC_ADD)://코드 변경시
		case(UR_RCDC_DEL): 
			{
				if(!SetEnableByDgnCode())
				{
					return;
				}
				SetEnableByCtrl();
				return;
			}		
		default:
		  break;
		}
	}
}
