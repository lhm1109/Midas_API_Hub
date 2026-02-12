// CMLoadSTDGWizardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadSTDGWizardDlg.h"
#include "CMLoadSTDGWizardSub.h"

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
// CCMLoadSTDGWizardDlg dialog

CCMLoadSTDGWizardDlg::CCMLoadSTDGWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMLoadSTDGWizardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadSTDGWizardDlg)
	m_dStageDur = 20.0;
	m_dMembrAge = 5.0;
	//}}AFX_DATA_INIT
	m_wndMainGrid = NULL;   
	m_wndSubGrid  = NULL;  
	m_wndSpreadGrid = NULL;

	m_pDoc = CDBDoc::GetDocPoint();
	m_wndMainGrid   = new CCMLoadESEQMainGrid;
	m_wndSubGrid    = new CCMLoadESEQSubGrid;
	m_wndSpreadGrid = new CCMLoadESEQSpreadGrid;

	for(int i=0; i<3; i++) 
	{
		m_wndLCaseGrid[i] = NULL;
		m_wndLCaseGrid[i] = new CCMLoadSTDGGrid;
	}

	m_aCtrlNormalStyle.Add(IDC_CMD_ESEQ_GROUP1);
	m_aCtrlNormalStyle.Add(IDC_CMD_LOAD_ESEQ_GRID1);
	m_aCtrlNormalStyle.Add(IDC_CMD_ESEQ_GROUP2);
	m_aCtrlNormalStyle.Add(IDC_CMD_LOAD_ESEQ_GRID2);
	m_aCtrlSpreadStyle.Add(IDC_CMD_ESEQ_GROUP3);
	m_aCtrlSpreadStyle.Add(IDC_CMD_LOAD_ESEQ_GRID3);

	m_bSpreadSheetStyle = TRUE;
	m_bUpdateCheck = TRUE;

}

CCMLoadSTDGWizardDlg::~CCMLoadSTDGWizardDlg()
{
	if(m_wndMainGrid != NULL)   {delete m_wndMainGrid; m_wndMainGrid = NULL;}
	if(m_wndSubGrid != NULL)    {delete m_wndSubGrid; m_wndSubGrid = NULL;}
	if(m_wndSpreadGrid != NULL) {delete m_wndSpreadGrid; m_wndSpreadGrid = NULL;}

	for(int i=0; i<3; i++)
		if(m_wndLCaseGrid[i]){ delete m_wndLCaseGrid[i]; m_wndLCaseGrid[i] = NULL;}
}

void CCMLoadSTDGWizardDlg::AlignControl()
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

void CCMLoadSTDGWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadSTDGWizardDlg)
	DDX_Control(pDX, IDC_CMD_LOAD_ESEQ_LOADCASE_CBO, m_cboLoadCase);
	DDX_Text(pDX, IDC_STAGE_DURATION_EDIT, m_dStageDur);
	DDX_Text(pDX, IDC_MEMBER_AGE_EDIT, m_dMembrAge);
	//}}AFX_DATA_MAP

	CArray<UINT, UINT> arComboIDs;
	arComboIDs.Add(IDC_CMD_LOAD_ESQW_LOADCASE1_CBO);
	arComboIDs.Add(IDC_CMD_LOAD_ESQW_LOADCASE2_CBO);
	arComboIDs.Add(IDC_CMD_LOAD_ESQW_LOADCASE3_CBO);

	for(int i=0; i<3; i++)
	{
	  DDX_Control(pDX, arComboIDs[i], m_ESQWLoadCb[i]);
	}

	DDX_Control(pDX, IDC_CMD_LOAD_ESEQ_GRID1, *m_wndMainGrid);
	DDX_Control(pDX, IDC_CMD_LOAD_ESEQ_GRID2, *m_wndSubGrid);
	DDX_Control(pDX, IDC_CMD_LOAD_ESEQ_GRID3, *m_wndSpreadGrid);
	DDX_Control(pDX, IDC_CMD_LOAD_ESQW_LOADCASE1_GRID, *m_wndLCaseGrid[0]);
	DDX_Control(pDX, IDC_CMD_LOAD_ESQW_LOADCASE2_GRID, *m_wndLCaseGrid[1]);
	DDX_Control(pDX, IDC_CMD_LOAD_ESQW_LOADCASE3_GRID, *m_wndLCaseGrid[2]);
}

BEGIN_MESSAGE_MAP(CCMLoadSTDGWizardDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMLoadSTDGWizardDlg)
	ON_BN_CLICKED(IDC_CMD_LOAD_ESEQ_SPREAD_BTN, OnCmdLoadEseqSpreadBtn)
	ON_BN_CLICKED(IDC_CMD_LOAD_ESEQ_OK, OnCmdLoadEseqOk)
	ON_BN_CLICKED(IDC_CMD_LOAD_ESEQ_CANCEL, OnCmdLoadEseqCancel)
	ON_BN_CLICKED(IDC_CMD_ESEQ_LOADCASE_BTN, OnCmdEseqLoadcaseBtn)
	ON_BN_CLICKED(IDC_CMD_LOAD_ESEQ_REMOVE_BTN, OnCmdLoadEseqRemoveBtn)
	ON_NOTIFY(UDN_DELTAPOS, IDC_MEMBER_AGE_SPIN, OnDeltaposMemberAgeSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_STAGE_DURATION_SPIN, OnDeltaposStageDurationSpin)
	ON_BN_CLICKED(IDC_CMD_LOAD_ESQW_AUTOGEN_BTN, OnCmdLoadEsqwAutogenBtn)
	ON_CBN_SELCHANGE(IDC_CMD_LOAD_ESQW_LOADCASE1_CBO, OnSelchangeCmdLoadEsqwLoadcase1Cbo)
	ON_CBN_SELCHANGE(IDC_CMD_LOAD_ESQW_LOADCASE2_CBO, OnSelchangeCmdLoadEsqwLoadcase2Cbo)
	ON_CBN_SELCHANGE(IDC_CMD_LOAD_ESQW_LOADCASE3_CBO, OnSelchangeCmdLoadEsqwLoadcase3Cbo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadSTDGWizardDlg message handlers
BOOL CCMLoadSTDGWizardDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	AlignControl();
	CDlgUtil::CtrlShowHide(this, m_aCtrlNormalStyle, FALSE);

	
	m_wndMainGrid->Initialize(m_pDoc);
	m_wndSubGrid->Initialize(m_pDoc);
	m_wndSpreadGrid->Initialize(m_pDoc);
	for(int i=0; i<3; i++)m_wndLCaseGrid[i]->Initialize(m_pDoc);

	m_wndMainGrid->m_pESEQSubGrid    = m_wndSubGrid;
	m_wndSubGrid->m_pESEQMainGrid    = m_wndMainGrid;
	m_wndSpreadGrid->m_pESEQMainGrid = m_wndMainGrid;

	InitLoadData();
		
	GetDlgItem(IDC_CMD_LOAD_ESEQ_OK)->EnableWindow(m_pDoc->m_pStagCtrl->IsBaseStag());

//  if(m_ESQWData.dAge >= 0.)      m_dMembrAge = m_ESQWData.dAge;
//  if(m_ESQWData.dDuration >= 0.) m_dStageDur = m_ESQWData.dDuration;
	UpdateData(FALSE);

	m_ATGNData.Initialize();
	OnSelchangeCmdLoadEsqwLoadcase1Cbo();
	OnSelchangeCmdLoadEsqwLoadcase2Cbo();
	OnSelchangeCmdLoadEsqwLoadcase3Cbo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLoadSTDGWizardDlg::InitLoadData()
{
	CArray<T_STLD_D,T_STLD_D> arStldData;
	T_STLD_D StldData;
	CArray<T_STLD_K,T_STLD_K> arStldKey;
	
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKey);

	for(int j=0; j<3; j++) m_ESQWLoadCb[j].AddString(_LS(IDS_WG_CMD__ADDD__None));
	for(int i=0; i<arStldKey.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetStld(arStldKey.GetAt(i), StldData);
	  m_cboLoadCase.AddString(StldData.LoadCaseName);
		for(int j=0; j<3; j++) m_ESQWLoadCb[j].AddString(StldData.LoadCaseName);
	}

	if(m_pDoc->m_pAttrCtrl->GetEsqwFromBaseDB(m_ESQWData))
	{
		T_STLD_D StldData;
		//m_ESQWData.Initialize();  // by jbseon
		//m_pDoc->m_pAttrCtrl->GetEsqw(m_ESQWData); // by jbseon
		if(m_pDoc->m_pAttrCtrl->ExistStor(m_ESQWData.StldKey))
		{
			m_pDoc->m_pAttrCtrl->GetStld(m_ESQWData.StldKey, StldData);
			m_cboLoadCase.SelectString(-1, StldData.LoadCaseName);
		}
		else	m_cboLoadCase.SetWindowText(_T(""));

// 수정 
//  	m_pDoc->m_pAttrCtrl->GetEsqw((T_ESQW_D)m_wndSpreadGrid->m_ESEQData);
		T_ESQW_D tmpESQWData = m_ESQWData;
		//m_pDoc->m_pAttrCtrl->GetEsqw(tmpESQWData);  // by jbseon
		m_wndSpreadGrid->m_ESEQData.StldKey = tmpESQWData.StldKey;
		m_wndSpreadGrid->m_ESEQData.arEseq.Copy(tmpESQWData.arEseq);
		m_wndSpreadGrid->InitESEQData();

		((CButton*)GetDlgItem(IDC_CMD_LOAD_ESEQ_REMOVE_BTN))->EnableWindow(TRUE);

		for(int i=0; i<m_ESQWData.arAddDeadLoad.GetSize(); i++)
		{
			m_wndLCaseGrid[i]->m_ESQWData.StldKey = m_ESQWData.arAddDeadLoad[i].StldKey;
			m_wndLCaseGrid[i]->m_ESQWData.arStory.Copy(m_ESQWData.arAddDeadLoad[i].arStory);
			m_wndLCaseGrid[i]->InitESQWData();
		}
			
		switch(m_ESQWData.arAddDeadLoad.GetSize())
		{
		case 1:
			m_pDoc->m_pAttrCtrl->GetStld(m_ESQWData.arAddDeadLoad[0].StldKey, StldData);
			m_ESQWLoadCb[0].SelectString(-1, StldData.LoadCaseName);
			m_ESQWLoadCb[1].SelectString(-1, _LS(IDS_WG_CMD__ADDD__None));
			m_ESQWLoadCb[2].SelectString(-1, _LS(IDS_WG_CMD__ADDD__None));
			break;
		case 2:
			m_pDoc->m_pAttrCtrl->GetStld(m_ESQWData.arAddDeadLoad[0].StldKey, StldData);
			m_ESQWLoadCb[0].SelectString(-1, StldData.LoadCaseName);
			m_pDoc->m_pAttrCtrl->GetStld(m_ESQWData.arAddDeadLoad[1].StldKey, StldData);
			m_ESQWLoadCb[1].SelectString(-1, StldData.LoadCaseName);
			m_ESQWLoadCb[2].SelectString(-1, _LS(IDS_WG_CMD__ADDD__None));
			break;
		case 3:
			m_pDoc->m_pAttrCtrl->GetStld(m_ESQWData.arAddDeadLoad[0].StldKey, StldData);
			m_ESQWLoadCb[0].SelectString(-1, StldData.LoadCaseName);
			m_pDoc->m_pAttrCtrl->GetStld(m_ESQWData.arAddDeadLoad[1].StldKey, StldData);
			m_ESQWLoadCb[1].SelectString(-1, StldData.LoadCaseName);
			m_pDoc->m_pAttrCtrl->GetStld(m_ESQWData.arAddDeadLoad[2].StldKey, StldData);
			m_ESQWLoadCb[2].SelectString(-1, StldData.LoadCaseName);
			break;
		default:
			for(int j=0; j<3; j++) m_ESQWLoadCb[j].SelectString(-1, _LS(IDS_WG_CMD__ADDD__None));
			break;
		}
	}
	else
	{
		m_ESQWData.Initialize();
		m_wndMainGrid->m_ESEQData.Initialize();
		m_wndSpreadGrid->m_ESEQData.Initialize();
		((CButton*)GetDlgItem(IDC_CMD_LOAD_ESEQ_REMOVE_BTN))->EnableWindow(FALSE);

		for(int i=0; i<3; i++)
		{
			m_ESQWLoadCb[i].SelectString(-1, _LS(IDS_WG_CMD__ADDD__None));
			m_wndLCaseGrid[i]->m_ESQWData.Initialize();
		}
	}
}

void CCMLoadSTDGWizardDlg::OnCmdLoadEseqSpreadBtn() 
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

void CCMLoadSTDGWizardDlg::OnCmdLoadEseqOk() 
{
	m_bUpdateCheck = FALSE;
	if(SaveESQWData())
	{
		if(!m_pDoc->m_pDataCtrl->AddEsqw(m_ESQWData))return;
		CDialogMove::OnOK();
	}
	else  m_bUpdateCheck = TRUE;	
}

void CCMLoadSTDGWizardDlg::OnCmdLoadEseqCancel() 
{
	CDialogMove::OnCancel();
}

void CCMLoadSTDGWizardDlg::OnCmdEseqLoadcaseBtn() 
{
	CString strTempLCaseName;
	m_cboLoadCase.GetWindowText(strTempLCaseName);
	m_StldKeyOld = m_pDoc->m_pAttrCtrl->GetStldKey(strTempLCaseName);

	for(int i=0; i<3; i++)
	{
	  m_ESQWLoadCb[i].GetWindowText(strTempLCaseName);
		m_SubStldKeyOld[i] = m_pDoc->m_pAttrCtrl->GetStldKey(strTempLCaseName);
	}
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void  CCMLoadSTDGWizardDlg::SaveSubGridData(UINT nGridID, T_ESQW_D& rESQWData)
{
	CString strTempLCaseName;
	T_ESQW_ADD_LOAD tmpESQWLoad;
	tmpESQWLoad.Initialize();
	
	m_ESQWLoadCb[nGridID].GetWindowText(strTempLCaseName);
	if(strTempLCaseName !=_LS(IDS_CMD0417__None) && strTempLCaseName !=_T(""))
	{    
		int nLastRow = m_wndLCaseGrid[nGridID]->GetRowCount();
		if(m_wndLCaseGrid[nGridID]->m_nOldRow < nLastRow)
		{
			if(nLastRow-1 > 0) m_wndLCaseGrid[nGridID]->AddESQWData(nLastRow-1);      // 마지막 Row 저장 
			m_wndLCaseGrid[nGridID]->AddESQWData(m_wndLCaseGrid[nGridID]->m_nOldRow); // Current Cell 저장
		}
		tmpESQWLoad.StldKey = m_pDoc->m_pAttrCtrl->GetStldKey(strTempLCaseName);
		tmpESQWLoad.arStory.Copy(m_wndLCaseGrid[nGridID]->m_ESQWData.arStory);
		
		if(tmpESQWLoad.arStory.GetSize()>0) rESQWData.arAddDeadLoad.Add(tmpESQWLoad);
	}
}

BOOL CCMLoadSTDGWizardDlg::SaveESQWData()
{
	m_ESQWData.Initialize();

	if(m_bSpreadSheetStyle)
	{
		if(!m_wndSpreadGrid->SaveESEQData())
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Invalid_Erection_Sequence));
			return FALSE;
		}
		m_ESQWData.StldKey = m_wndSpreadGrid->m_ESEQData.StldKey;
		m_ESQWData.arEseq.Copy(m_wndSpreadGrid->m_ESEQData.arEseq);
	}
	else
	{
		m_ESQWData.StldKey = m_wndMainGrid->m_ESEQData.StldKey;
		m_ESQWData.arEseq.Copy(m_wndMainGrid->m_ESEQData.arEseq);

		if(m_ESQWData.arEseq.GetSize() != m_wndMainGrid->GetRowCount() - 1)
		{
			if(m_wndMainGrid->CheckField())
			{
				m_wndMainGrid->AddESEQData(m_wndMainGrid->m_nOldRow);
				m_ESQWData.Initialize();
				m_ESQWData.StldKey = m_wndMainGrid->m_ESEQData.StldKey;
				m_ESQWData.arEseq.Copy(m_wndMainGrid->m_ESEQData.arEseq);
			}
			else  return FALSE;
		}
		else if(m_wndMainGrid->m_bModified)
		{
			if(m_wndMainGrid->CheckField())
			{
				m_wndMainGrid->AddESEQData(m_wndMainGrid->m_nOldRow);
				m_ESQWData.Initialize();
				m_ESQWData.StldKey = m_wndMainGrid->m_ESEQData.StldKey;
				m_ESQWData.arEseq.Copy(m_wndMainGrid->m_ESEQData.arEseq);
			}
		}
	}

	switch(IsEmptyESQWData(m_ESQWData))
	{
		case 1:		return TRUE;		break;  
		case 2:		return FALSE;		break;  
		case 3:		return TRUE;		break;  
		case 4:               		break;  
	}

	CString strTempLCaseName;
	m_cboLoadCase.GetWindowText(strTempLCaseName);
	if(strTempLCaseName == _T(""))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___LoadCase_Name_was_not_sel));
		return FALSE;
	}

	m_ESQWData.StldKey = m_pDoc->m_pAttrCtrl->GetStldKey(strTempLCaseName);
 
	UpdateData(TRUE);
//  m_ESQWData.dAge      = m_dMembrAge;
//  m_ESQWData.dDuration = m_dStageDur;
	
	// Save Additional Load Grid Data
	for(int i=0; i<3; i++)
		SaveSubGridData(i, m_ESQWData);

	return TRUE;
}

int CCMLoadSTDGWizardDlg::IsEmptyESQWData(T_ESQW_D ESQWData)
{
	if(ESQWData.arEseq.GetSize() == 0)
	{
		if(m_pDoc->m_pAttrCtrl->ExistEsqw())
		{
			if(AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Delete_existing_Erection_Sequence),MB_OKCANCEL) == IDOK)
			{
				if(m_pDoc->m_pDataCtrl->DelEsqw()) return 1;
				else return 2;
			}
			else return 2;
		}
		return 3;
	}
	return 4;
}

void CCMLoadSTDGWizardDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCMLoadSTDGWizardDlg::ChangeLoadCase()
{
	if(!m_bUpdateCheck) return;

	m_cboLoadCase.ResetContent();
	for(int j=0; j<3; j++) m_ESQWLoadCb[j].ResetContent();
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
	  m_cboLoadCase.AddString(StldData.LoadCaseName);
		for(int j=0; j<3; j++) m_ESQWLoadCb[j].AddString(StldData.LoadCaseName);
	}
	for(int j=0; j<3; j++) m_ESQWLoadCb[j].AddString(_LS(IDS_WG_CMD__ADDD__None));

	m_pDoc->m_pAttrCtrl->GetStld(m_StldKeyOld, StldData);
	if(CB_ERR==m_cboLoadCase.SelectString(-1, StldData.LoadCaseName))
		m_cboLoadCase.SetWindowText(_T(""));

	for(int j=0; j<3; j++)
	{
	  m_pDoc->m_pAttrCtrl->GetStld(m_SubStldKeyOld[j], StldData);
		m_ESQWLoadCb[j].SelectString(-1, StldData.LoadCaseName);  
	}

}

void CCMLoadSTDGWizardDlg::OnCmdLoadEseqRemoveBtn() 
{
	m_bUpdateCheck = FALSE;
	if(m_pDoc->m_pDataCtrl->DelEsqw())
		CDialogMove::OnOK();
	else
		m_bUpdateCheck = TRUE;	
}

void CCMLoadSTDGWizardDlg::OnDeltaposMemberAgeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDlgUtil::SetSpinValue(this,IDC_MEMBER_AGE_EDIT,0,-1,pNMHDR,pResult);	
}

void CCMLoadSTDGWizardDlg::OnDeltaposStageDurationSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDlgUtil::SetSpinValue(this,IDC_STAGE_DURATION_EDIT,0,-1,pNMHDR,pResult);	
}

void CCMLoadSTDGWizardDlg::CheckGridCondition(UINT nComboID)
{
	if(nComboID<0 || nComboID>2) return;  // 0, 1, 2만 허용

	CString strTempLCaseName;
	m_ESQWLoadCb[nComboID].GetWindowText(strTempLCaseName);

	if(strTempLCaseName == _LS(IDS_CMD0417__None) || strTempLCaseName == _T("")) 
		m_wndLCaseGrid[nComboID]->EnableGrid(FALSE);
	else                                                     
		m_wndLCaseGrid[nComboID]->EnableGrid(TRUE);
}

void CCMLoadSTDGWizardDlg::OnSelchangeCmdLoadEsqwLoadcase1Cbo() 
{  
	CheckGridCondition(0);
}

void CCMLoadSTDGWizardDlg::OnSelchangeCmdLoadEsqwLoadcase2Cbo() 
{
	CheckGridCondition(1);
}

void CCMLoadSTDGWizardDlg::OnSelchangeCmdLoadEsqwLoadcase3Cbo() 
{
	CheckGridCondition(2);
}

///////////////////////////////////////////////////////////
//
// 2002. 4. 1 (SubDlg 추가분)
//
void CCMLoadSTDGWizardDlg::OnCmdLoadEsqwAutogenBtn() 
{
	/*
	CCMLoadSTDGWizardSub* pDlg;
	pDlg = new CCMLoadSTDGWizardSub(this, &m_ATGNData);
	pDlg->DoModal();	
	delete pDlg;
	*/
}

void CCMLoadSTDGWizardDlg::InitAutoGenerationData()
{
	UINT nIndex, nSize; 

	T_STLD_D StldData;
	CArray<T_STOR_K, T_STOR_K> StoryKeyList;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(StoryKeyList);

	/////////////////////////////////
	// Construction Dead Load
	if(m_pDoc->m_pAttrCtrl->ExistStor(m_ATGNData.StldKey))
	{
	  m_pDoc->m_pAttrCtrl->GetStld(m_ATGNData.StldKey, StldData);
		m_cboLoadCase.SelectString(-1, StldData.LoadCaseName);
	}
	else	m_cboLoadCase.SetWindowText(_T(""));

	if(m_ATGNData.dMemberAge >= 0.) m_dMembrAge = m_ATGNData.dMemberAge;
	if(m_ATGNData.dStageDurt >= 0.) m_dStageDur = m_ATGNData.dStageDurt;

	T_ESQW_D    tmpESQWData;
	T_ESEQ_BASE tmpESEQBase;  
	T_ESEQ_SEL  tmpESEQSel; 
	nSize  = floor(((StoryKeyList.GetSize()-2) / m_ATGNData.dStoryIncr));

	for(int i=1; i<=nSize ; i++)
	{
		tmpESEQBase.Initialize();
		tmpESEQSel.Initialize();

		nIndex = m_ATGNData.dStoryIncr * i;
		tmpESEQBase.KeyStorLoad = StoryKeyList[nIndex];
		tmpESEQSel.nType        = 1;
		tmpESEQSel.KeyMatl      = 1;
		tmpESEQSel.KeyStorModel = StoryKeyList[nIndex];
		tmpESEQBase.EseqSel[0]  = tmpESEQSel;
		tmpESQWData.arEseq.Add(tmpESEQBase);
	}
	// Last Floor
	tmpESEQBase.Initialize();
	tmpESEQSel.Initialize();

	nIndex = StoryKeyList.GetSize() - 1;
	tmpESEQBase.KeyStorLoad = StoryKeyList[nIndex];
	tmpESEQSel.nType        = 1;
	tmpESEQSel.KeyMatl      = 1;
	tmpESEQSel.KeyStorModel = StoryKeyList[nIndex];
	tmpESEQBase.EseqSel[0]  = tmpESEQSel;
	tmpESQWData.arEseq.Add(tmpESEQBase);

	// Init Grid Data
	m_wndSpreadGrid->m_ESEQData.StldKey = m_ATGNData.StldKey;
	m_wndSpreadGrid->m_ESEQData.arEseq.Copy(tmpESQWData.arEseq);
	m_wndSpreadGrid->InitESEQData();

	((CButton*)GetDlgItem(IDC_CMD_LOAD_ESEQ_REMOVE_BTN))->EnableWindow(TRUE);
	
	////////////////////////////////
	// Additional Dead Load
	T_ESQW_ADD_LOAD       tmpESQWLoad;
	T_ESQW_ADD_STOR_BASE  tmpESQWBase;

	for(int i=0; i<m_ATGNData.arAddDeadLoad.GetSize(); i++)
	{
		tmpESQWLoad.Initialize();
		m_wndLCaseGrid[i]->m_ESQWData.StldKey = m_ATGNData.arAddDeadLoad[i].StldKey;

		nSize  = floor(((StoryKeyList.GetSize()-2) / m_ATGNData.arAddDeadLoad[i].dSotryIncr));
		for(int j=1; j<=nSize; j++)
		{
			tmpESQWBase.Initialize();
			nIndex = m_ATGNData.arAddDeadLoad[i].dSotryIncr * j;
			tmpESQWBase.StoryKey = StoryKeyList[nIndex];
			tmpESQWBase.dStartingDay = m_ATGNData.arAddDeadLoad[i].dStartDay + m_ATGNData.arAddDeadLoad[i].dDayIncr * (j -1);
			tmpESQWLoad.arStory.Add(tmpESQWBase);
		}
		// Last Story
		tmpESQWBase.Initialize();
		nIndex = StoryKeyList.GetSize()-1;
		tmpESQWBase.StoryKey = StoryKeyList[nIndex];
		tmpESQWBase.dStartingDay = m_ATGNData.arAddDeadLoad[i].dStartDay + m_ATGNData.arAddDeadLoad[i].dDayIncr*nSize;
		tmpESQWLoad.arStory.Add(tmpESQWBase);

		m_wndLCaseGrid[i]->m_ESQWData.arStory.Copy(tmpESQWLoad.arStory);
		m_wndLCaseGrid[i]->InitESQWData();
	}

	// Select Load Type 
	switch(m_ATGNData.arAddDeadLoad.GetSize())
	{
	case 1: 
		m_pDoc->m_pAttrCtrl->GetStld(m_ATGNData.arAddDeadLoad[0].StldKey, StldData); 
		m_ESQWLoadCb[0].SelectString(-1, StldData.LoadCaseName);
		m_ESQWLoadCb[1].SelectString(-1, _LS(IDS_CMD0417__None));
		m_ESQWLoadCb[2].SelectString(-1, _LS(IDS_CMD0417__None));
		break;
	case 2: 
		m_pDoc->m_pAttrCtrl->GetStld(m_ATGNData.arAddDeadLoad[0].StldKey, StldData); 
		m_ESQWLoadCb[0].SelectString(-1, StldData.LoadCaseName);
		m_pDoc->m_pAttrCtrl->GetStld(m_ATGNData.arAddDeadLoad[1].StldKey, StldData); 
		m_ESQWLoadCb[1].SelectString(-1, StldData.LoadCaseName);
		m_ESQWLoadCb[2].SelectString(-1, _LS(IDS_CMD0417__None));
		break;
	case 3:
		{
			for(int j=0; j<3; j++)
			{
				m_pDoc->m_pAttrCtrl->GetStld(m_ATGNData.arAddDeadLoad[j].StldKey, StldData); 
				m_ESQWLoadCb[j].SelectString(-1, StldData.LoadCaseName);
			}
		}
		break;
	default:
		{
			for(int j=0; j<3; j++) m_ESQWLoadCb[j].SelectString(-1, _LS(IDS_CMD0417__None));
		}
		break;
	}
	UpdateData(FALSE);
}

