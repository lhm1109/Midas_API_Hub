// CMLoadSTDGWizardDlgNew.cpp : implementation file
//
// 2003. 10. 06    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadSTDGWizardDlgNew.h"

#include "CMLoadESEQMainGrid.h"
#include "CMLoadESEQSubGrid.h"
#include "CMStageSpreadGrid.h"
#include "CMStageStdgSubGrid.h"
#include "CMLoadSTDGWizardSub.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"

#include "CMLoadCaseDlg.h"
#include "..\wg_base\wg_base_ModelessDlgCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_STLD_NONE  _LS(IDS_WG_CMD__ADDD__None)

/////////////////////////////////////////////////////////////////////////////
// CCMLoadSTDGWizardDlgNew dialog

CCMLoadSTDGWizardDlgNew::CCMLoadSTDGWizardDlgNew(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMLoadSTDGWizardDlgNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadSTDGWizardDlgNew)
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

	m_bSpreadFormat = TRUE;
	m_bUpdateCheck = TRUE;

	m_pSpreadGrid = NULL;
	m_pSpreadGrid = new CCMStageSpreadGrid();
	m_pNormalGrid = NULL;
	m_pNormalGrid = new CCMLoadESEQMainGrid();
	m_pNormalSubGrid = NULL;
	m_pNormalSubGrid = new CCMLoadESEQSubGrid();

	for (int i=0; i<3; i++)
	{
		m_SuperData[i].Initialize();
		m_pSuperGrid[i] = NULL;
		m_pSuperGrid[i] = new CCMStageStdgSubGrid();
	}

	m_ATGNData.Initialize();
	m_Data.Initialize();
	if (m_pDoc->m_pAttrCtrl->ExistEsqw())
	{
		m_pDoc->m_pAttrCtrl->GetEsqw(m_Data);
		for (int i=0; i<m_Data.arAddDeadLoad.GetSize(); i++)
			m_SuperData[i] = m_Data.arAddDeadLoad[i];
	}

	m_aCtrlNormal.Add(IDC_CMD_ESEQ_GROUP1);
	m_aCtrlNormal.Add(IDC_CMD_ESEQ_GRID1);
	m_aCtrlNormal.Add(IDC_CMD_ESEQ_GROUP2);
	m_aCtrlNormal.Add(IDC_CMD_ESEQ_GRID2);
	m_aCtrlSpread.Add(IDC_CMD_ESEQ_GROUP3);
	m_aCtrlSpread.Add(IDC_CMD_ESEQ_GRID3);
}

CCMLoadSTDGWizardDlgNew::~CCMLoadSTDGWizardDlgNew()
{
	if (m_pSpreadGrid)
	{
		delete m_pSpreadGrid; m_pSpreadGrid = NULL;
	}
	if (m_pNormalGrid)
	{
		delete m_pNormalGrid; m_pNormalGrid = NULL;
	}
	if (m_pNormalSubGrid)
	{
		delete m_pNormalSubGrid; m_pNormalSubGrid = NULL;
	}
	for (int i=0; i<3; i++)
	{
		if (m_pSuperGrid[i] == NULL) continue;
		delete m_pSuperGrid[i];
		m_pSuperGrid[i] = NULL;
	}
}

void CCMLoadSTDGWizardDlgNew::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadSTDGWizardDlgNew)
	DDX_Control(pDX, IDC_CMD_LOADCASE_COMBO, m_cobxLoadCaseMain);
	//}}AFX_DATA_MAP
	UINT aID[3] = {IDC_CMD_ESQW_LOADCASE1_CBO, 
								 IDC_CMD_ESQW_LOADCASE2_CBO, 
								 IDC_CMD_ESQW_LOADCASE3_CBO};
	for (int i=0; i<3; i++)
	{
		DDX_Control(pDX, aID[i], m_cobxLoadCase[i]);
	}

	DDX_Control(pDX, IDC_CMD_ESEQ_GRID3, *m_pSpreadGrid);
	DDX_Control(pDX, IDC_CMD_ESEQ_GRID1, *m_pNormalGrid);
	DDX_Control(pDX, IDC_CMD_ESEQ_GRID2, *m_pNormalSubGrid);
	DDX_Control(pDX, IDC_CMD_ESEQ_GRID4, *m_pSuperGrid[0]);
	DDX_Control(pDX, IDC_CMD_ESEQ_GRID5, *m_pSuperGrid[1]);
	DDX_Control(pDX, IDC_CMD_ESEQ_GRID6, *m_pSuperGrid[2]);
}

BEGIN_MESSAGE_MAP(CCMLoadSTDGWizardDlgNew, CDialogMove)
	//{{AFX_MSG_MAP(CCMLoadSTDGWizardDlgNew)
	ON_BN_CLICKED(IDC_CMD_LOADCASE_MORE_BTN, OnCmdLoadcaseMoreBtn)
	ON_BN_CLICKED(IDC_CMD_FORMAT_BTN, OnCmdFormatBtn)
	ON_CBN_SELCHANGE(IDC_CMD_ESQW_LOADCASE1_CBO, OnSelchangeCmdEsqwLoadcase1Cbo)
	ON_CBN_SELCHANGE(IDC_CMD_ESQW_LOADCASE2_CBO, OnSelchangeCmdEsqwLoadcase2Cbo)
	ON_CBN_SELCHANGE(IDC_CMD_ESQW_LOADCASE3_CBO, OnSelchangeCmdEsqwLoadcase3Cbo)
	ON_BN_CLICKED(IDC_CMD_ESEQ_REMOVE_BTN, OnCmdEseqRemoveBtn)
	ON_BN_CLICKED(IDC_CMD_ESEQ_GEN_BTN, OnCmdEseqGenBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadSTDGWizardDlgNew message handlers

BOOL CCMLoadSTDGWizardDlgNew::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	// TODO: Add extra initialization here
	AlignControl();
		
	m_pSpreadGrid->Initialize(/*&m_Data.arEseq*/);
	m_pNormalGrid->Initialize(m_pDoc);
	m_pNormalSubGrid->Initialize(m_pDoc);

	m_Data.arAddDeadLoad.SetSize(3);
	for (int i=0; i<3; i++)
		m_pSuperGrid[i]->Initialize(&m_SuperData[i]);

	m_pNormalGrid->m_pESEQSubGrid     = m_pNormalSubGrid;
	m_pNormalGrid->m_pSpreadGrid      = m_pSpreadGrid;
	m_pNormalSubGrid->m_pESEQMainGrid = m_pNormalGrid;

	UpdateLoadcaseCombo(&m_cobxLoadCaseMain, FALSE);
	for (int i=0; i<3; i++)
		UpdateLoadcaseCombo(&m_cobxLoadCase[i]);

	Data2Dlg();

	m_ATGNData.Initialize();
	OnSelchangeCmdEsqwLoadcase1Cbo();
	OnSelchangeCmdEsqwLoadcase2Cbo();
	OnSelchangeCmdEsqwLoadcase3Cbo();

	ChangeFormat(m_bSpreadFormat);
	GetDlgItem(IDOK)->EnableWindow(m_pDoc->m_pStagCtrl->IsBaseStag());
 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLoadSTDGWizardDlgNew::OnOK() 
{
	// TODO: Add extra validation here
	m_bUpdateCheck = FALSE;
	if (Dlg2Data())
	{
		if (!m_pDoc->m_pDataCtrl->AddEsqw(m_Data))return;
		CDialogMove::OnOK();
	}
	else m_bUpdateCheck = TRUE;		
}

void CCMLoadSTDGWizardDlgNew::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CCMLoadSTDGWizardDlgNew::OnCmdLoadcaseMoreBtn() 
{
	// TODO: Add your control notification handler code here
	CCMDlgBase* pDlg = (CCMDlgBase*)(m_pDoc->m_pDlgCtrl->GetDlg(CCMLoadCaseDlg::IDD));

	DWORD dwLastErr = NO_ERROR;
	if (pDlg == 0) // Dialog가 없으면 만든다.
	{
		pDlg = new CCMLoadCaseDlg();
		pDlg->m_pDoc = m_pDoc;
		pDlg->Create(CCMLoadCaseDlg::IDD, this);
		dwLastErr = ::GetLastError();
	}
	pDlg->ShowWindow(SW_SHOW);

	//AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CCMLoadSTDGWizardDlgNew::OnCmdFormatBtn() 
{
	// TODO: Add your control notification handler code here
	m_bSpreadFormat = !m_bSpreadFormat;
	ChangeFormat(m_bSpreadFormat);

	// 그리드에 데이터 뿌려주기 
	if (m_bSpreadFormat) 
	{
		m_pSpreadGrid->MakeItemEx();
	}
	else
	{
		m_pNormalGrid->m_ESEQData.Initialize();
		m_pNormalGrid->m_ESEQData.arEseq.Copy(m_pSpreadGrid->m_aData);
		m_pNormalGrid->InitESEQData();		
	}
}

void CCMLoadSTDGWizardDlgNew::OnSelchangeCmdEsqwLoadcase1Cbo() 
{
	// TODO: Add your control notification handler code here
	if (m_pSuperGrid[0]->IsReadOnly()) m_pSuperGrid[0]->SetReadOnly(FALSE);

	CString csLoadcase = D_STLD_NONE;
	int ix = m_cobxLoadCase[0].GetCurSel();
	if (ix != CB_ERR) m_cobxLoadCase[0].GetLBText(ix, csLoadcase);
	if (csLoadcase != D_STLD_NONE) return;

	// 데이터 모두 없애기 
	CArray<double, double> aDelData;
	T_ESQW_ADD_LOAD data = m_SuperData[0];
	for (int i=0; i<data.arStory.GetSize(); i++)
		aDelData.Add(data.arStory[i].dStartingDay);
	m_pSuperGrid[0]->DelData(aDelData);
	m_pSuperGrid[0]->SetReadOnly(TRUE); 
}

void CCMLoadSTDGWizardDlgNew::OnSelchangeCmdEsqwLoadcase2Cbo() 
{
	// TODO: Add your control notification handler code here
	if (m_pSuperGrid[1]->IsReadOnly()) m_pSuperGrid[1]->SetReadOnly(FALSE);

	CString csLoadcase = D_STLD_NONE;
	int ix = m_cobxLoadCase[1].GetCurSel();
	if (ix != CB_ERR) m_cobxLoadCase[1].GetLBText(ix, csLoadcase);
	if (csLoadcase != D_STLD_NONE) return;

	// 데이터 모두 없애기 
	CArray<double, double> aDelData;
	T_ESQW_ADD_LOAD data = m_SuperData[1];
	for (int i=0; i<data.arStory.GetSize(); i++)
		aDelData.Add(data.arStory[i].dStartingDay);
	m_pSuperGrid[1]->DelData(aDelData);
	m_pSuperGrid[1]->SetReadOnly(TRUE); 
}

void CCMLoadSTDGWizardDlgNew::OnSelchangeCmdEsqwLoadcase3Cbo() 
{
	// TODO: Add your control notification handler code here
	if (m_pSuperGrid[2]->IsReadOnly()) m_pSuperGrid[2]->SetReadOnly(FALSE);

	CString csLoadcase = D_STLD_NONE;
	int ix = m_cobxLoadCase[2].GetCurSel();
	if (ix != CB_ERR) m_cobxLoadCase[2].GetLBText(ix, csLoadcase);
	if (csLoadcase != D_STLD_NONE) return;

	// 데이터 모두 없애기 
	CArray<double, double> aDelData;
	T_ESQW_ADD_LOAD data = m_SuperData[2];
	for (int i=0; i<data.arStory.GetSize(); i++)
		aDelData.Add(data.arStory[i].dStartingDay);
	m_pSuperGrid[2]->DelData(aDelData);
	m_pSuperGrid[2]->SetReadOnly(TRUE); 
}

void CCMLoadSTDGWizardDlgNew::OnCmdEseqRemoveBtn() 
{
	// TODO: Add your control notification handler code here
	m_bUpdateCheck = FALSE;
	if(m_pDoc->m_pDataCtrl->DelEsqw())
		CDialogMove::OnOK();
	else
		m_bUpdateCheck = TRUE;	
}

void CCMLoadSTDGWizardDlgNew::OnCmdEseqGenBtn() 
{
	// TODO: Add your control notification handler code here
	CCMLoadSTDGWizardSub dlg(this, &m_ATGNData);
	dlg.DoModal();	
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function
void CCMLoadSTDGWizardDlgNew::AlignControl()
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

	GetDlgItem(IDC_CMD_ESEQ_GRID1)->GetWindowRect(rRef1);
	GetDlgItem(IDC_CMD_ESEQ_GRID2)->GetWindowRect(rRef2);
	CWnd* pWnd2 = GetDlgItem(IDC_CMD_ESEQ_GRID3);
	ScreenToClient(rRef1);
	ScreenToClient(rRef2);
	rToMove.top = rRef1.top;
	rToMove.left = rRef1.left;
	rToMove.bottom = rRef1.bottom;
	rToMove.right = rRef2.right;
	pWnd2->MoveWindow(rToMove);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDOK);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+10;

	MoveWindow(r);
}

void CCMLoadSTDGWizardDlgNew::UpdateLoadcaseCombo(CComboBox* pCombo, BOOL bIncludeNone/*=TRUE*/)
{
	ASSERT(pCombo);
	HWND hWnd = pCombo->GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	// 현재 선택되어 있는 데이터 저장 
	CString csPreData = D_STLD_NONE;
	int ix = pCombo->GetCurSel();
	if (ix != CB_ERR) pCombo->GetLBText(ix, csPreData);
	
	pCombo->ResetContent();
	ix = pCombo->AddString(D_STLD_NONE);
	pCombo->SetItemData(ix, 0);

	T_STLD_D StldD;
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	int nNum = aStldK.GetSize();
	for (int i=0; i<nNum; i++)
	{
		m_pDoc->m_pAttrCtrl->GetStld(aStldK[i], StldD);
		ix = pCombo->AddString(StldD.LoadCaseName);
		pCombo->SetItemData(ix, aStldK[i]);
	}
	pCombo->SelectString(-1, csPreData);
}

BOOL CCMLoadSTDGWizardDlgNew::IsProperData(T_ESEQ_SEL &data)
{
	if (data.nType != 1 && data.nType != 2) return FALSE;
	if (!m_pDoc->m_pAttrCtrl->ExistStor(data.KeyStorModel)) return FALSE;
	if (data.nType == 1)
	{
		if (data.KeyMatl < 1 && data.KeyMatl > 5) return FALSE;
	}
	else 
	{
		if (!m_pDoc->m_pAttrCtrl->ExistMatl(data.KeyMatl)) return FALSE;
	}
	return TRUE;
}

void CCMLoadSTDGWizardDlgNew::Data2Dlg()
{
	BOOL bExistData = m_pDoc->m_pAttrCtrl->GetEsqwFromBaseDB(m_Data);
	if (bExistData) // DB에 데이터가 있으면...
	{
		T_STLD_D StldD;
		if (m_pDoc->m_pAttrCtrl->ExistStor(m_Data.StldKey))
		{
			m_pDoc->m_pAttrCtrl->GetStld(m_Data.StldKey, StldD);
			m_cobxLoadCaseMain.SelectString(-1, StldD.LoadCaseName);
		}
		else	m_cobxLoadCaseMain.SetWindowText(_T(""));

		// 데이터 생성과정
		T_KEYSTEP BaseKey;
		T_ESEQ_BASE BaseData;
		m_pSpreadGrid->m_aData.RemoveAll();
		for (int i=0; i<m_Data.arEseq.GetSize(); i++)
		{
			for (int k=0; k<15; k++)
			{
				if (!IsProperData(m_Data.arEseq[i].EseqSel[k])) break;

				BaseKey.key.entity = m_pSpreadGrid->m_CurKey; // key
				BaseKey.key.serial = i+1; // step

				BaseData.Initialize();
				BaseData.dAge = m_Data.arEseq[i].dAge;
				BaseData.dDuration = m_Data.arEseq[i].dDuration;
				BaseData.KeyStorLoad = m_Data.arEseq[i].KeyStorLoad;
				BaseData.EseqSel[0].nType = m_Data.arEseq[i].EseqSel[k].nType;
				BaseData.EseqSel[0].KeyStorModel = m_Data.arEseq[i].EseqSel[k].KeyStorModel;
				BaseData.EseqSel[0].KeyMatl = m_Data.arEseq[i].EseqSel[k].KeyMatl;

				m_pSpreadGrid->AddData(BaseKey, BaseData, FALSE);
			}
		}

		int nNum = m_Data.arAddDeadLoad.GetSize();
		for (int i=0; i<nNum; i++)
		{
			m_pDoc->m_pAttrCtrl->GetStld(m_Data.arAddDeadLoad[i].StldKey, StldD);
			m_cobxLoadCase[i].SelectString(-1, StldD.LoadCaseName);
			m_SuperData[i] = m_Data.arAddDeadLoad[i];
			m_pSuperGrid[i]->MakeItemEx();
		} 
		for (int i=nNum; i<3; i++)
		{
			m_cobxLoadCase[i].SelectString(-1, _LS(IDS_WG_CMD__ADDD__None));
		}

		((CButton*)GetDlgItem(IDC_CMD_ESEQ_REMOVE_BTN))->EnableWindow(TRUE);
	}
	else
	{
		m_Data.Initialize();
		m_pSpreadGrid->m_aData.RemoveAll();
		m_pNormalGrid->m_ESEQData.Initialize();
		for (int i=0; i<m_Data.arAddDeadLoad.GetSize(); i++) 
		{
			m_SuperData[i] = m_Data.arAddDeadLoad[i];
		}
		((CButton*)GetDlgItem(IDC_CMD_ESEQ_REMOVE_BTN))->EnableWindow(FALSE);
	}
}

void CCMLoadSTDGWizardDlgNew::ChangeFormat(BOOL bSpreadFormat)
{
	CDlgUtil::CtrlShowHide(this, m_aCtrlNormal, !bSpreadFormat);
	CDlgUtil::CtrlShowHide(this, m_aCtrlSpread,  bSpreadFormat);

	CString csText;
	if (bSpreadFormat) csText = _LS(IDS_WG_CMD__ADDD__Normal_Form);
	else csText = _LS(IDS_WG_CMD__ADDD__Spread_Sheet_Form);
	CWnd* pWnd = GetDlgItem(IDC_CMD_FORMAT_BTN); ASSERT(pWnd);
	pWnd->SetWindowText(csText);
}

BOOL CCMLoadSTDGWizardDlgNew::Dlg2Data()
{
	// (주의) 저장시 편집중인 그리드 데이터까지 무조건 저장 
	m_Data.Initialize();
	
	int ix = m_cobxLoadCaseMain.GetCurSel();
	if (ix == CB_ERR) return FALSE;
	m_Data.StldKey = m_cobxLoadCaseMain.GetItemData(ix);
	
	if (m_bSpreadFormat)
	{
		if (!m_pSpreadGrid->OnTerminate(D_TB_SAVE_ALWAYS)) return FALSE;

		for (int i=0; i<m_pSpreadGrid->m_aData.GetSize(); i++)
		{
			for (int k=0; k<15; k++)
			{
				if (!IsProperData(m_pSpreadGrid->m_aData[i].EseqSel[k])) 
					m_pSpreadGrid->m_aData[i].EseqSel[k].Initialize();        
			}
		}
		m_Data.arEseq.Copy(m_pSpreadGrid->m_aData);
	}
	/*
	else
	{
		//m_Data.StldKey = m_pNormalGrid->m_ESEQData.StldKey;
		m_Data.arEseq.Copy(m_pNormalGrid->m_ESEQData.arEseq);

		if(m_Data.arEseq.GetSize() != m_pNormalGrid->GetRowCount() - 1)
		{
			if(m_pNormalGrid->CheckField())
			{
				m_pNormalGrid->AddESEQData(m_pNormalGrid->m_nOldRow);
				//m_Data.Initialize();
				//m_Data.StldKey = m_pNormalGrid->m_ESEQData.StldKey;
				m_Data.arEseq.Copy(m_pNormalGrid->m_ESEQData.arEseq);
			}
			else  return FALSE;
		}
		else if(m_pNormalGrid->m_bModified)
		{
			if(m_pNormalGrid->CheckField())
			{
				m_pNormalGrid->AddESEQData(m_pNormalGrid->m_nOldRow);
				//m_Data.Initialize();
				//m_Data.StldKey = m_pNormalGrid->m_ESEQData.StldKey;
				m_Data.arEseq.Copy(m_pNormalGrid->m_ESEQData.arEseq);
			}
		}
	}
	*/

	switch(IsEmptyData(m_Data))
	{
		case 1:		return TRUE;		break;  
		case 2:		return FALSE;		break;  
		case 3:		return TRUE;		break;  
		case 4:               		break;  
	}

	// Save Additional Load Grid Data
	int nIndex;
	CString csLcaseName;
	T_STLD_K StldK;
	for (int i=0; i<3; i++)
	{
		if (!m_pSuperGrid[i]->OnTerminate(D_TB_SAVE_ALWAYS)) return FALSE;

		nIndex = m_cobxLoadCase[i].GetCurSel();
		if (nIndex == CB_ERR) continue;
		m_cobxLoadCase[i].GetLBText(nIndex, csLcaseName);
		StldK = m_pDoc->m_pAttrCtrl->GetStldKey(csLcaseName);
		if (StldK == 0) continue;
		m_SuperData[i].StldKey = StldK;
		m_Data.arAddDeadLoad.Add(m_SuperData[i]);
	}
	return TRUE;
}

int CCMLoadSTDGWizardDlgNew::IsEmptyData(T_ESQW_D &data)
{
	if(data.arEseq.GetSize() == 0)
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

void CCMLoadSTDGWizardDlgNew::InitAutoGenerationData()
{
	int i, nNum, ix;
	T_ESQW_D    tmpESQWData;
	T_ESEQ_BASE tmpESEQBase;  
	T_ESEQ_SEL  tmpESEQSel; 
	T_STLD_D StldD;
	CArray<T_STOR_K, T_STOR_K> aStorK;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);

	if (m_ATGNData.bCheck)
	{
		if(m_pDoc->m_pAttrCtrl->ExistStor(m_ATGNData.StldKey))
		{
		  m_pDoc->m_pAttrCtrl->GetStld(m_ATGNData.StldKey, StldD);
			m_cobxLoadCaseMain.SelectString(-1, StldD.LoadCaseName);
		}
		else	m_cobxLoadCaseMain.SetWindowText(_T(""));

		if (m_ATGNData.dStoryIncr == 0) nNum = 0;
		else nNum  = floor(((aStorK.GetSize()-2) / m_ATGNData.dStoryIncr));    
		for (i=1; i<=nNum ; i++)
		{
			tmpESEQBase.Initialize();
			tmpESEQSel.Initialize();
			ix = m_ATGNData.dStoryIncr * i;
			tmpESEQBase.KeyStorLoad = aStorK[ix];
			tmpESEQBase.dAge = m_ATGNData.dMemberAge;
			tmpESEQBase.dDuration  = m_ATGNData.dStageDurt;
			tmpESEQSel.nType        = 1;
			tmpESEQSel.KeyMatl      = 1;
			tmpESEQSel.KeyStorModel = aStorK[ix];
			tmpESEQBase.EseqSel[0]  = tmpESEQSel;
			tmpESQWData.arEseq.Add(tmpESEQBase);
		}
		// Last Floor
		tmpESEQBase.Initialize();
		tmpESEQSel.Initialize();

		ix = aStorK.GetSize() - 1;
		tmpESEQBase.KeyStorLoad = aStorK[ix];
		tmpESEQBase.dAge = m_ATGNData.dMemberAge;
		tmpESEQBase.dDuration  = m_ATGNData.dStageDurt;
		tmpESEQSel.nType        = 1;
		tmpESEQSel.KeyMatl      = 1;
		tmpESEQSel.KeyStorModel = aStorK[ix];
		tmpESEQBase.EseqSel[0]  = tmpESEQSel;
		tmpESQWData.arEseq.Add(tmpESEQBase);

		// 기존 데이터 지우기
		unsigned int uKey;
		int uData;
		T_KEYSTEP keystep;
		CArray<T_KEYSTEP, T_KEYSTEP> aDelKey;
		POSITION pos = m_pSpreadGrid->m_KeyMapper.GetStartPosition();
		while (pos != NULL)
		{
			m_pSpreadGrid->m_KeyMapper.GetNextAssoc(pos, uKey, uData);
			keystep.keymap = uKey;
			aDelKey.Add(keystep);
		}
		m_pSpreadGrid->DelData(aDelKey, FALSE);

		// 데이터 생성과정
		T_KEYSTEP BaseKey;
		T_ESEQ_BASE BaseData;
		//m_pSpreadGrid->m_aData.RemoveAll(); 
		for (i=0; i<tmpESQWData.arEseq.GetSize(); i++)
		{
			for (int k=0; k<15; k++)
			{
				if (!IsProperData(tmpESQWData.arEseq[i].EseqSel[k])) break;

				BaseKey.key.entity = m_pSpreadGrid->m_CurKey; // key
				BaseKey.key.serial = i+1; // step

				BaseData.Initialize();
				BaseData.dAge = tmpESQWData.arEseq[i].dAge;
				BaseData.dDuration = tmpESQWData.arEseq[i].dDuration;
				BaseData.KeyStorLoad = tmpESQWData.arEseq[i].KeyStorLoad;
				BaseData.EseqSel[0].nType = tmpESQWData.arEseq[i].EseqSel[k].nType;
				BaseData.EseqSel[0].KeyStorModel = tmpESQWData.arEseq[i].EseqSel[k].KeyStorModel;
				BaseData.EseqSel[0].KeyMatl = tmpESQWData.arEseq[i].EseqSel[k].KeyMatl;

				m_pSpreadGrid->AddData(BaseKey, BaseData, FALSE);
			}
		}

		m_pNormalGrid->m_ESEQData.StldKey = m_ATGNData.StldKey;
		m_pNormalGrid->m_ESEQData.arEseq.Copy(tmpESQWData.arEseq);
		m_pNormalGrid->InitESEQData();
	}
	
	///////////////////////////////////////////////
	// Additional Dead Load
	// Select Load Type 
	for (i=0; i<m_ATGNData.arAddDeadLoad.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetStld(m_ATGNData.arAddDeadLoad[i].StldKey, StldD); 
		m_cobxLoadCase[i].SelectString(-1, StldD.LoadCaseName);
	}
	for (i=m_ATGNData.arAddDeadLoad.GetSize(); i<3; i++)
	{
		m_cobxLoadCase[i].SelectString(-1, _LS(IDS_CMD0417__None));
	}

	int k;
	double dStartingDay;
	CArray<double, double> aDelData;
	for (i=0; i<m_ATGNData.arAddDeadLoad.GetSize(); i++)
	{
		if (m_pSuperGrid[i]->IsReadOnly()) m_pSuperGrid[i]->SetReadOnly(FALSE);

		// 데이터 초기화
		aDelData.RemoveAll();
		for (k=0; k<m_SuperData[i].arStory.GetSize(); k++)
			aDelData.Add(m_SuperData[i].arStory[k].dStartingDay);
		m_pSuperGrid[i]->DelData(aDelData);
		
		// 새로운 데이터 추가 
		m_SuperData[i].StldKey = m_ATGNData.arAddDeadLoad[i].StldKey;
		if (m_ATGNData.arAddDeadLoad[i].dSotryIncr == 0) nNum = 0;
		else nNum  = floor(((aStorK.GetSize()-2) / m_ATGNData.arAddDeadLoad[i].dSotryIncr));
		for (k=1; k<=nNum; k++)
		{
			ix = m_ATGNData.arAddDeadLoad[i].dSotryIncr * k;
			dStartingDay = m_ATGNData.arAddDeadLoad[i].dStartDay + m_ATGNData.arAddDeadLoad[i].dDayIncr * (k -1);
			m_pSuperGrid[i]->AddData(dStartingDay, aStorK[ix]);
		}
		// Last Story
		ix = aStorK.GetSize()-1;
		dStartingDay = m_ATGNData.arAddDeadLoad[i].dStartDay + m_ATGNData.arAddDeadLoad[i].dDayIncr * (k - 1);
		m_pSuperGrid[i]->AddData(dStartingDay, aStorK[ix]);
	}

	if (m_pSpreadGrid->m_aData.GetSize() > 0)
		((CButton*)GetDlgItem(IDC_CMD_ESEQ_REMOVE_BTN))->EnableWindow(TRUE);
}



/////////////////////////////////////////////////////////////////////////////
// override

void CCMLoadSTDGWizardDlgNew::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
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
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMLoadSTDGWizardDlgNew::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bChangedStld = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STLD_ADD):
		case(UR_STLD_DEL):
		case(UR_STLD_MFD):
		case(UR_STLD_MFS):
				bChangedStld = TRUE;
				break;
		default:
			break;
		}
	} // end of while

	if (bChangedStld)
	{
		UpdateLoadcaseCombo(&m_cobxLoadCaseMain, FALSE);
		for (int i=0; i<3; i++)
			UpdateLoadcaseCombo(&m_cobxLoadCase[i]);
	}
}

