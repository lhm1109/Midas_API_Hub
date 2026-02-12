// CMLoadESEQDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadESEQDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_main\wg_mainRes2.h"

#include "CMLoadCaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQDlg dialog
CCMLoadESEQDlg::CCMLoadESEQDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMLoadESEQDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadESEQDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_wndMainGrid = new CCMLoadESEQMainGrid;
	m_wndSubGrid = new CCMLoadESEQSubGrid;
	m_wndSpreadGrid = new CCMLoadESEQSpreadGrid;

	m_aCtrlNormalStyle.Add(IDC_CMD_ESEQ_GROUP1);
	m_aCtrlNormalStyle.Add(IDC_CMD_LOAD_ESEQ_GRID1);
	m_aCtrlNormalStyle.Add(IDC_CMD_ESEQ_GROUP2);
	m_aCtrlNormalStyle.Add(IDC_CMD_LOAD_ESEQ_GRID2);
	m_aCtrlSpreadStyle.Add(IDC_CMD_ESEQ_GROUP3);
	m_aCtrlSpreadStyle.Add(IDC_CMD_LOAD_ESEQ_GRID3);

	m_bSpreadSheetStyle = TRUE;
	m_bUpdateCheck = TRUE;
}

CCMLoadESEQDlg::~CCMLoadESEQDlg()
{
	if(m_wndMainGrid) delete m_wndMainGrid;
	if(m_wndSubGrid) delete m_wndSubGrid;
	if(m_wndSpreadGrid) delete m_wndSpreadGrid;
}

void CCMLoadESEQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadESEQDlg)
	DDX_Control(pDX, IDC_CMD_LOAD_ESEQ_LOADCASE_CBO, m_cboLoadCase);
	DDX_Control(pDX, IDC_CMD_LOAD_ESEQ_GRID1, *m_wndMainGrid);
	DDX_Control(pDX, IDC_CMD_LOAD_ESEQ_GRID2, *m_wndSubGrid);
	DDX_Control(pDX, IDC_CMD_LOAD_ESEQ_GRID3, *m_wndSpreadGrid);
	//}}AFX_DATA_MAP
}

void CCMLoadESEQDlg::AlignControl()
{
	CRect rRef1, rRef2;
	CRect rToMove;

	GetDlgItem(IDC_CMD_ESEQ_GROUP1)->GetWindowRect(rRef1);
	GetDlgItem(IDC_CMD_ESEQ_GROUP2)->GetWindowRect(rRef2);
	CWnd* pWnd1 = GetDlgItem(IDC_CMD_ESEQ_GROUP3);
	ScreenToClient(rRef1);
	ScreenToClient(rRef2);
	rToMove.top = rRef1.top;
	rToMove.left = rRef1.left;
	rToMove.bottom = rRef1.bottom;
	rToMove.right = rRef2.right;
	pWnd1->MoveWindow(rToMove);

	GetDlgItem(IDC_CMD_LOAD_ESEQ_GRID1)->GetWindowRect(rRef1);
	GetDlgItem(IDC_CMD_LOAD_ESEQ_GRID2)->GetWindowRect(rRef2);
	CWnd* pWnd2 = GetDlgItem(IDC_CMD_LOAD_ESEQ_GRID3);
	ScreenToClient(rRef1);
	ScreenToClient(rRef2);
	rToMove.top = rRef1.top;
	rToMove.left = rRef1.left;
	rToMove.bottom = rRef1.bottom;
	rToMove.right = rRef2.right;
	pWnd2->MoveWindow(rToMove);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_LOAD_ESEQ_CANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+10;

	MoveWindow(r);
}


BEGIN_MESSAGE_MAP(CCMLoadESEQDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMLoadESEQDlg)
	ON_BN_CLICKED(IDC_CMD_LOAD_ESEQ_REMOVE_BTN, OnCmdLoadEseqRemoveBtn)
	ON_BN_CLICKED(IDC_CMD_LOAD_ESEQ_OK, OnCmdLoadEseqOk)
	ON_BN_CLICKED(IDC_CMD_LOAD_ESEQ_CANCEL, OnCmdLoadEseqCancel)
	ON_BN_CLICKED(IDC_CMD_ESEQ_LOADCASE_BTN, OnCmdEseqLoadcaseBtn)
	ON_BN_CLICKED(IDC_CMD_LOAD_ESEQ_SPREAD_BTN, OnCmdLoadEseqSpreadBtn)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQDlg message handlers

BOOL CCMLoadESEQDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	AlignControl();
	CDlgUtil::CtrlShowHide(this, m_aCtrlNormalStyle, FALSE);

	m_wndMainGrid->Initialize(m_pDoc);
	m_wndSubGrid->Initialize(m_pDoc);
	m_wndSpreadGrid->Initialize(m_pDoc);

	m_wndMainGrid->m_pESEQSubGrid = m_wndSubGrid;
	m_wndSubGrid->m_pESEQMainGrid = m_wndMainGrid;
	m_wndSpreadGrid->m_pESEQMainGrid = m_wndMainGrid;

	InitLoadData();

	GetDlgItem(IDC_CMD_LOAD_ESEQ_OK)->EnableWindow(m_pDoc->m_pStagCtrl->IsBaseStag());

	return TRUE; 
}

void CCMLoadESEQDlg::InitLoadData()
{
	CArray<T_STLD_D,T_STLD_D> arStldData;
	T_STLD_D StldData;
	CArray<T_STLD_K,T_STLD_K> arStldKey;
	
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKey);

	for(int i=0; i<arStldKey.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetStld(arStldKey.GetAt(i), StldData);
//		if(StldData.LoadCaseType == _T("D"))    //Dead Load만 하는것이 이상하다
			m_cboLoadCase.AddString(StldData.LoadCaseName);
	}

	m_ESEQData.Initialize();
	if(m_pDoc->m_pAttrCtrl->GetEseqFromBaseDB(m_ESEQData))  // by jbseon : base db에서 가져온다
	{
		T_STLD_D StldData;
		//m_ESEQData.Initialize();    // by jbseon
		//m_pDoc->m_pAttrCtrl->GetEseq(m_ESEQData); // by jbseon
		if(m_pDoc->m_pAttrCtrl->ExistStor(m_ESEQData.StldKey))
		{
			m_pDoc->m_pAttrCtrl->GetStld(m_ESEQData.StldKey, StldData);
			m_cboLoadCase.SelectString(-1, StldData.LoadCaseName);
		}
		else
			m_cboLoadCase.SetWindowText(_T(""));

//  	m_pDoc->m_pAttrCtrl->GetEseq(m_wndMainGrid->m_ESEQData);
		//m_pDoc->m_pAttrCtrl->GetEseq(m_wndSpreadGrid->m_ESEQData); // by jbseon
		m_wndSpreadGrid->m_ESEQData = m_ESEQData; // by jbseon
//		m_wndMainGrid->InitESEQData();	// Spread Form이 초기폼이 되었다.
		m_wndSpreadGrid->InitESEQData();

		((CButton*)GetDlgItem(IDC_CMD_LOAD_ESEQ_REMOVE_BTN))->EnableWindow(TRUE);
	}
	else
	{
		m_wndMainGrid->m_ESEQData.Initialize();
		m_wndSpreadGrid->m_ESEQData.Initialize();
		((CButton*)GetDlgItem(IDC_CMD_LOAD_ESEQ_REMOVE_BTN))->EnableWindow(FALSE);
	}
}

void CCMLoadESEQDlg::OnCmdLoadEseqRemoveBtn() 
{
	m_bUpdateCheck = FALSE;
	if(m_pDoc->m_pDataCtrl->DelEseq())
		CDialogMove::OnOK();
	else
		m_bUpdateCheck = TRUE;
}

void CCMLoadESEQDlg::OnCmdLoadEseqOk() 
{
	m_bUpdateCheck = FALSE;
	if(SaveESEQData())
		CDialogMove::OnOK();
	else
		m_bUpdateCheck = TRUE;
}

void CCMLoadESEQDlg::OnCmdLoadEseqCancel() 
{
	CDialogMove::OnCancel();
}

BOOL CCMLoadESEQDlg::SaveESEQData()
{
	m_ESEQData.Initialize();

	if(m_bSpreadSheetStyle)
	{
		if(!m_wndSpreadGrid->SaveESEQData())
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Invalid_Erection_Sequence));
			return FALSE;
		}
		m_ESEQData = m_wndSpreadGrid->m_ESEQData;
	}
	else
	{
		m_ESEQData = m_wndMainGrid->m_ESEQData;

		if(m_ESEQData.arEseq.GetSize() != m_wndMainGrid->GetRowCount() - 1)
		{
//			if((m_wndMainGrid->CheckField()) && (m_wndSubGrid->CheckField()))
			if(m_wndMainGrid->CheckField())
			{
				m_wndMainGrid->AddESEQData(m_wndMainGrid->m_nOldRow);
				m_ESEQData.Initialize();
				m_ESEQData = m_wndMainGrid->m_ESEQData;
			}
			else
			{
				return FALSE;
			}
		}
		else if(m_wndMainGrid->m_bModified)
		{
//			if((m_wndMainGrid->CheckField()) && (m_wndSubGrid->CheckField()))  // 다른 특수한 경우는 어떻게?
			if(m_wndMainGrid->CheckField())
			{
				m_wndMainGrid->AddESEQData(m_wndMainGrid->m_nOldRow);
				m_ESEQData.Initialize();
				m_ESEQData = m_wndMainGrid->m_ESEQData;
			}
		}
	}

	switch(IsEmptyESEQData(m_ESEQData))
	{
		case 1:		return TRUE;		break;  // 데이타가 하나도 없고, 기존에 Erection데이타 존재. 지우고 창 닫음. (Ok)
		case 2:		return FALSE;		break;  // 데이타가 하나도 없고, 기존에 Erection데이타 존재. 안지우고 상태유지(Cancel)
		case 3:		return TRUE;		break;  // 데이타가 하나도 없고, Erection데이타 존재않음. 그냥 창 닫음.
		case 4:               		break;  // 데이타가 존재하므로 저장하고 닫음.
	}

	CString strTempLCaseName;
	m_cboLoadCase.GetWindowText(strTempLCaseName);
	if(strTempLCaseName == _T(""))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___LoadCase_Name_was_not_sel));
		return FALSE;
	}

	m_ESEQData.StldKey = m_pDoc->m_pAttrCtrl->GetStldKey(strTempLCaseName);

	return m_pDoc->m_pDataCtrl->AddEseq(m_ESEQData);
}

int CCMLoadESEQDlg::IsEmptyESEQData(T_ESEQ_D ESEQData)
{
	if(ESEQData.arEseq.GetSize() == 0)
	{
		if(m_pDoc->m_pAttrCtrl->ExistEseq())
		{
			if(AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Delete_existing_Erection_Sequence),MB_OKCANCEL) == IDOK)
			{
				if(m_pDoc->m_pDataCtrl->DelEseq()) return 1;
				else return 2;
			}
			else return 2;
		}
		return 3;
	}

	return 4;
}

void CCMLoadESEQDlg::OnCmdEseqLoadcaseBtn() 
{
	CString strTempLCaseName;
	m_cboLoadCase.GetWindowText(strTempLCaseName);
	m_StldKeyOld = m_pDoc->m_pAttrCtrl->GetStldKey(strTempLCaseName);
	
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CCMLoadESEQDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		ChangeLoadCase();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		ChangeLoadCase();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMLoadESEQDlg::ChangeLoadCase()
{
	if(!m_bUpdateCheck) return;

	m_cboLoadCase.ResetContent();

	CArray<T_STLD_D,T_STLD_D> arStldData;
	T_STLD_D StldData;
	CArray<T_STLD_K,T_STLD_K> arStldKey;
	
	if(!m_pDoc->m_pAttrCtrl->GetCountStld())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Load_Case_was_not_defined));  
		((CButton*)GetDlgItem(IDC_CMD_LOAD_ESEQ_CANCEL))->SetFocus();
		return;
	}

	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKey);

	for(int i=0; i<arStldKey.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetStld(arStldKey.GetAt(i), StldData);
//		if(StldData.LoadCaseType == _T("D"))    //Dead Load만 하는것이 이상하다
			m_cboLoadCase.AddString(StldData.LoadCaseName);
	}

	m_pDoc->m_pAttrCtrl->GetStld(m_StldKeyOld, StldData);
	if(CB_ERR==m_cboLoadCase.SelectString(-1, StldData.LoadCaseName))
		m_cboLoadCase.SetWindowText(_T(""));
}

void CCMLoadESEQDlg::OnCmdLoadEseqSpreadBtn() 
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_LOAD_ESEQ_SPREAD_BTN);
	if(m_bSpreadSheetStyle)
	{
		if(!m_wndSpreadGrid->SaveESEQData())   // Normal Form 으로 못가게 한다.
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Invalid_Erection_Sequence));
			return;
		}
		m_bSpreadSheetStyle = FALSE;
		pWnd->SetWindowText(_LS(IDS_WG_CMD__ADDD__Spread_Sheet_Form));
		CDlgUtil::CtrlShowHide(this, m_aCtrlNormalStyle, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlSpreadStyle, FALSE);
		m_wndMainGrid->m_ESEQData.Initialize();
		m_wndMainGrid->m_ESEQData = m_wndSpreadGrid->m_ESEQData;
		m_wndMainGrid->InitESEQData();		
	}
	else
	{
		if(m_wndMainGrid->m_bModified)
			if((m_wndMainGrid->CheckField()) && (m_wndSubGrid->CheckField(FALSE)))
				m_wndMainGrid->AddESEQData(m_wndMainGrid->m_nOldRow);
			else 
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Invalid_Erection_Sequence));
				return;
			}
		
		m_bSpreadSheetStyle = TRUE;
		pWnd->SetWindowText(_LS(IDS_WG_CMD__ADDD__Normal_Form));
		CDlgUtil::CtrlShowHide(this, m_aCtrlNormalStyle, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlSpreadStyle, TRUE);
		m_wndSpreadGrid->m_ESEQData.Initialize();
		m_wndSpreadGrid->m_ESEQData = m_wndMainGrid->m_ESEQData;
		m_wndSpreadGrid->InitESEQData();		
	}	
}
