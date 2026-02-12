// CMMvctJPInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctJPInfoDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvctJPInfoDlg dialog

#define CDialog CCMDlgBase


CCMMvctJPInfoDlg::CCMMvctJPInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvctJPInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctJPInfoDlg)
	m_nSelectBy = -1;
	m_bP1Load = FALSE;
	m_bP2Load = FALSE;
	m_bCrowdLoad = FALSE;
	m_bTLoadCheck = FALSE;
	m_bExtraCoeff = FALSE;
	m_bImpactCoeff = FALSE;
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CCMMvctJPInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctJPInfoDlg)
	DDX_Control(pDX, IDC_CMD_P1LOAD_EDIT, m_wndP1Edit);
	DDX_Control(pDX, IDC_CMD_P2LOAD_EDIT, m_wndP2Edit);
	DDX_Control(pDX, IDC_CMD_CROWDLOAD_EDIT, m_wndCrowdLoad);
	DDX_Control(pDX, IDC_CMD_TLOAD_EDIT, m_wndTLoadEdit);
	DDX_Control(pDX, IDC_CMD_IMPACTCOEFF_EDIT, m_wndImpactCoeff);
	DDX_Control(pDX, IDC_CMD_EXTRACOEFF_EDIT, m_wndExtraCoeff);
	DDX_Control(pDX, IDC_CMD_P1LOAD_UNIT, m_wndP1Unit);
	DDX_Control(pDX, IDC_CMD_P2LOAD_UNIT, m_wndP2Unit);
	DDX_Control(pDX, IDC_CMD_CROWDLOAD_UNIT, m_CrowdUnit);
	DDX_Control(pDX, IDC_CMD_TLOAD_UNIT, m_wndTLoadUnit);
	DDX_Control(pDX, IDC_CMD_SELBY_EDIT1, m_wndSelBy1);
	DDX_Control(pDX, IDC_CMD_SELBY_EDIT2, m_wndSelBy2);
	DDX_Control(pDX, IDC_CMD_SELBY_UNIT1, m_wndSelByUnit1);
	DDX_Control(pDX, IDC_CMD_SELBY_UNIT2, m_wndSelByUnit2);
	DDX_Control(pDX, IDC_CMD_LENGTH_EDIT, m_wndLength);
	DDX_Control(pDX, IDC_CMD_LENGTH_UNIT, m_wndLengthUnit);
	DDX_Radio(pDX, IDC_2POINTS_RADIO, m_nSelectBy);
	DDX_Check(pDX, IDC_CMD_P1LOAD_CHECK, m_bP1Load);
	DDX_Check(pDX, IDC_CMD_P2LOAD_CHECK, m_bP2Load);
	DDX_Check(pDX, IDC_CMD_CROWDLOAD_CHECK, m_bCrowdLoad);
	DDX_Check(pDX, IDC_CMD_TLOAD_CHECK, m_bTLoadCheck);
	DDX_Check(pDX, IDC_CMD_EXTRACOEFF_CHECK, m_bExtraCoeff);
	DDX_Check(pDX, IDC_CMD_IMPACTCOEFF_CHECK, m_bImpactCoeff);
	DDX_Control(pDX, IDC_CMD_ELEM_LIST, m_wndElemList);
	DDX_Text(pDX, IDC_CMD_NAME_EDIT, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvctJPInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CCMMvctJPInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvctJPInfoDlg message handlers

BOOL CCMMvctJPInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvctJPInfoDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	*m_ppMySelf = NULL;
	CDialog::PostNcDestroy();
}

void CCMMvctJPInfoDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	DestroyWindow();
	//	CDialog::OnCancel();  호출 하면 안됨.. Modeless Dialog에서는 주의할것..
	//                        PostNCDestroy 호출 안됨 
}

void CCMMvctJPInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	DestroyWindow();
	//	CDialog::OnOK();      호출 하면 안됨.. Modeless Dialog에서는 주의할것..
	//                        PostNCDestroy 호출 안됨     
}


void CCMMvctJPInfoDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		//UpdateUnit();
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_POST_STEP_CHANGED:
		DestroyWindow();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMMvctJPInfoDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	BOOL bMvcdChanged = FALSE;
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
		case(UR_MVCD_ADD):
		case(UR_MVCD_DEL):
			bMvcdChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while
	// 코드가 바뀌면 무조건 닫는다.
	if (bMvcdChanged)
	{
		DestroyWindow();
	}
}

/*
BOOL CCMMvctJPInfoDlg::IsRelevantCode(int nCode)
{
	if (nCode == D_MOVE_CODE_JAPAN) return TRUE;
	return FALSE;
}

void CCMMvctJPInfoDlg::ShowHideByCode()
{
	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	if (!IsRelevantCode(MvcdData.nCodeType)) DestroyWindow();
}
*/
