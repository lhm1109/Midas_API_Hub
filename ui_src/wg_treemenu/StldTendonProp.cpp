// StldTendonProp.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldTendonProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldTendonProp dialog
#define CDialog CMenuBarChildDlg


CStldTendonProp::CStldTendonProp(CWnd* pParent /*=NULL*/)
	: CDialog(CStldTendonProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldTendonProp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CStldTendonProp::~CStldTendonProp()
{
	
}


void CStldTendonProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldTendonProp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldTendonProp, CDialog)
	//{{AFX_MSG_MAP(CStldTendonProp)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldTendonProp message handlers

BOOL CStldTendonProp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CStldTendonProp::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	/*
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	 	//UpdateBuffer();
		MakeItemExMatlist();
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
	*/
}


void CStldTendonProp::UpdateBuffer()
{
	/*
	CViewBuff* pViewBuff=  CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount ==0 ) return;

	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsTdmtCommand = FALSE;
	BOOL bIsTmatCommand = FALSE;
	BOOL bIsMatlCommand = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MATL_ADD):
		case(UR_MATL_DEL):
		case(UR_MATL_MFD):
		case(UR_MATL_MFS):
			bIsMatlCommand = TRUE;
			break;
		case(UR_TDMT_ADD):
		case(UR_TDMT_DEL):
		case(UR_TDMT_MFD): 
		case(UR_TDMT_MFS):
			bIsTdmtCommand = TRUE;
			break;
		case(UR_TMAT_ADD):
		case(UR_TMAT_DEL):
			bIsTmatCommand = TRUE;
			break;
		default:
			break;
		}
	}

	if(bIsTdmtCommand || bIsTmatCommand) 
	{
		MakeItemExMatlist();
	}
	if(bIsMatlCommand)
	{
		UpdateLList();
		UpdateRList();
		MakeItemExMatlist();
	}
	*/


}


void CStldTendonProp::OnTmClose() 
{
	CloseDlg();
}
