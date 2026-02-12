// CMLoadSTDGWizardSub.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadSTDGWizardSub.h"
#include "CMLoadSTDGWizardDlgNew.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_main\wg_mainRes2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CDialogMove

/////////////////////////////////////////////////////////////////////////////
// CCMLoadSTDGWizardSub dialog
CCMLoadSTDGWizardSub::CCMLoadSTDGWizardSub(CCMLoadSTDGWizardDlgNew* pDlg, T_ATGN_D* pATGNData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMLoadSTDGWizardSub::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadSTDGWizardSub)
	m_nLoad1 = FALSE;
	m_nLoad2 = FALSE;
	m_nLoad3 = FALSE;
	m_nStage = FALSE;
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_pATGNData = NULL; ASSERT(pATGNData);
	m_pATGNData = pATGNData;

	m_pParentDlg = NULL; ASSERT(pDlg);
	m_pParentDlg = pDlg;
}

void CCMLoadSTDGWizardSub::SetATGNData(T_ATGN_D* pData)
{
	/*
	m_pATGNData = NULL;
	ASSERT(pData);
	m_pATGNData = pData;
	*/
}

void CCMLoadSTDGWizardSub::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadSTDGWizardSub)
	DDX_Control(pDX, IDC_STAGE_LOADCASE_CB, m_StageCB);
	DDX_Control(pDX, IDC_DAED2_LOADCASE_CB, m_Load2CB);
	DDX_Control(pDX, IDC_DAED3_LOADCASE_CB, m_Load3CB);
	DDX_Control(pDX, IDC_DAED1_LOADCASE_CB, m_Load1CB);
	DDX_Check(pDX, IDC_ADDITIONAL_DEADLOAD_CHK1, m_nLoad1);
	DDX_Check(pDX, IDC_ADDITIONAL_DEADLOAD_CHK2, m_nLoad2);
	DDX_Check(pDX, IDC_ADDITIONAL_DEADLOAD_CHK3, m_nLoad3);
	DDX_Check(pDX, IDC_CONSTRUCTION_STAGE_CHK, m_nStage);
	DDX_Control(pDX, IDC_DAED1_STORY_INCR, m_dStoryIncr1);
	DDX_Control(pDX, IDC_DAED2_STORY_INCR, m_dStoryIncr2);
	DDX_Control(pDX, IDC_DAED3_STORY_INCR, m_dStoryIncr3);
	DDX_Control(pDX, IDC_DEAD1_DAY_INCREASE, m_dDayIncr1);
	DDX_Control(pDX, IDC_DEAD2_DAY_INCREASE, m_dDayIncr2);
	DDX_Control(pDX, IDC_DEAD3_DAY_INCREASE, m_dDayIncr3);
	DDX_Control(pDX, IDC_DEAD1_STARTING_DAY, m_dStartDay1);
	DDX_Control(pDX, IDC_DEAD2_STARTING_DAY, m_dStartDay2);
	DDX_Control(pDX, IDC_DEAD3_STARTING_DAY, m_dStartDay3);
	DDX_Control(pDX, IDC_STAGE_MEMBER_AGE, m_dMembAge);
	DDX_Control(pDX, IDC_STAGE_STAGE_DURATION, m_dStageDur);
	DDX_Control(pDX, IDC_STAGE_STORY_INCR, m_dStoryIncr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMLoadSTDGWizardSub, CDialog)
	//{{AFX_MSG_MAP(CCMLoadSTDGWizardSub)
	ON_BN_CLICKED(IDC_ADDITIONAL_DEADLOAD_CHK1, OnAdditionalDeadloadChk1)
	ON_BN_CLICKED(IDC_ADDITIONAL_DEADLOAD_CHK2, OnAdditionalDeadloadChk2)
	ON_BN_CLICKED(IDC_ADDITIONAL_DEADLOAD_CHK3, OnAdditionalDeadloadChk3)
	ON_BN_CLICKED(IDC_CONSTRUCTION_STAGE_CHK, OnConstructionStageChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadSTDGWizardSub message handlers

BOOL CCMLoadSTDGWizardSub::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitLoadCaseCombo();
	SetData2Dlg();

	OnConstructionStageChk();
	OnAdditionalDeadloadChk1();
	OnAdditionalDeadloadChk2();
	OnAdditionalDeadloadChk3();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLoadSTDGWizardSub::InitLoadCaseCombo() 
{
	T_STLD_D StldData;
	CArray<T_STLD_K,T_STLD_K> arStldKey;
	
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKey);

	for(int i=0; i<arStldKey.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetStld(arStldKey.GetAt(i), StldData);
		m_StageCB.AddString(StldData.LoadCaseName);
		m_Load1CB.AddString(StldData.LoadCaseName);
	  m_Load2CB.AddString(StldData.LoadCaseName);
	  m_Load3CB.AddString(StldData.LoadCaseName);
	}
}

void CCMLoadSTDGWizardSub::OnOK() 
{
	CArray<T_STOR_K, T_STOR_K> aStorKeyList;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorKeyList);
	if(aStorKeyList.GetSize() <= 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_ESEQ_ERROR6)); 
		return; 
	}

	if (!SetDlg2Data()) return;

	CDialog::OnOK();

	// ParentDlg ÃÊ±âÈ­
	m_pParentDlg->InitAutoGenerationData();
}

void CCMLoadSTDGWizardSub::SetData2Dlg()
{
	m_dStoryIncr1.SetValue(0.0);
	m_dStoryIncr2.SetValue(0.0);
	m_dStoryIncr3.SetValue(0.0);
	m_dDayIncr1.SetValue(0.0);
	m_dDayIncr2.SetValue(0.0);
	m_dDayIncr3.SetValue(0.0);
	m_dStartDay1.SetValue(0.0);
	m_dStartDay2.SetValue(0.0);
	m_dStartDay3.SetValue(0.0);
	m_dMembAge.SetValue(0.0);
	m_dStageDur.SetValue(0.0);
	m_dStoryIncr.SetValue(0.0);

	m_dStoryIncr1.SetRange(0, 1000);
	m_dStoryIncr2.SetRange(0, 1000);
	m_dStoryIncr3.SetRange(0, 1000);
	m_dDayIncr1.SetRange(0, 10000);
	m_dDayIncr2.SetRange(0, 10000);
	m_dDayIncr3.SetRange(0, 10000);
	m_dStartDay1.SetRange(0, 10000);
	m_dStartDay2.SetRange(0, 10000);
	m_dStartDay3.SetRange(0, 10000);
	m_dMembAge.SetRange(0, 10000);
	m_dStageDur.SetRange(0, 10000);
	m_dStoryIncr.SetRange(0, 10000);

	T_STLD_D StldData;
	m_nStage = m_pATGNData->bCheck;
	if(m_nStage)
	{
	  m_pDoc->m_pAttrCtrl->GetStld(m_pATGNData->StldKey, StldData);
		m_StageCB.SelectString(-1, StldData.LoadCaseName);  

		m_dStoryIncr.SetValue(m_pATGNData->dStoryIncr);
		m_dStageDur.SetValue(m_pATGNData->dStageDurt);
		m_dMembAge.SetValue(m_pATGNData->dMemberAge);
	}

	switch(m_pATGNData->arAddDeadLoad.GetSize())
	{
	case 1:
		m_nLoad1 = TRUE;
	  m_pDoc->m_pAttrCtrl->GetStld(m_pATGNData->arAddDeadLoad[0].StldKey, StldData);
		m_Load1CB.SelectString(-1, StldData.LoadCaseName);  
		m_dStoryIncr1.SetValue(m_pATGNData->arAddDeadLoad[0].dSotryIncr);
		m_dStartDay1.SetValue(m_pATGNData->arAddDeadLoad[0].dStartDay);
		m_dDayIncr1.SetValue(m_pATGNData->arAddDeadLoad[0].dDayIncr);

		m_nLoad2 = FALSE;
		m_nLoad3 = FALSE;
		break;
	case 2:
		m_nLoad1 = TRUE;
	  m_pDoc->m_pAttrCtrl->GetStld(m_pATGNData->arAddDeadLoad[0].StldKey, StldData);
		m_Load1CB.SelectString(-1, StldData.LoadCaseName);  
		m_dStoryIncr1.SetValue(m_pATGNData->arAddDeadLoad[0].dSotryIncr);
		m_dStartDay1.SetValue(m_pATGNData->arAddDeadLoad[0].dStartDay);
		m_dDayIncr1.SetValue(m_pATGNData->arAddDeadLoad[0].dDayIncr);

		m_nLoad2 = TRUE;
	  m_pDoc->m_pAttrCtrl->GetStld(m_pATGNData->arAddDeadLoad[1].StldKey, StldData);
		m_Load2CB.SelectString(-1, StldData.LoadCaseName);  
		m_dStoryIncr2.SetValue(m_pATGNData->arAddDeadLoad[1].dSotryIncr);
		m_dStartDay2.SetValue(m_pATGNData->arAddDeadLoad[1].dStartDay);
		m_dDayIncr2.SetValue(m_pATGNData->arAddDeadLoad[1].dDayIncr);

		m_nLoad3 = FALSE;
		break;
	case 3:
		m_nLoad1 = TRUE;
	  m_pDoc->m_pAttrCtrl->GetStld(m_pATGNData->arAddDeadLoad[0].StldKey, StldData);
		m_Load1CB.SelectString(-1, StldData.LoadCaseName);  
		m_dStoryIncr1.SetValue(m_pATGNData->arAddDeadLoad[0].dSotryIncr);
		m_dStartDay1.SetValue(m_pATGNData->arAddDeadLoad[0].dStartDay);
		m_dDayIncr1.SetValue(m_pATGNData->arAddDeadLoad[0].dDayIncr);

		m_nLoad2 = TRUE;
	  m_pDoc->m_pAttrCtrl->GetStld(m_pATGNData->arAddDeadLoad[1].StldKey, StldData);
		m_Load2CB.SelectString(-1, StldData.LoadCaseName);  
		m_dStoryIncr2.SetValue(m_pATGNData->arAddDeadLoad[1].dSotryIncr);
		m_dStartDay2.SetValue(m_pATGNData->arAddDeadLoad[1].dStartDay);
		m_dDayIncr2.SetValue(m_pATGNData->arAddDeadLoad[1].dDayIncr);

		m_nLoad3 = TRUE;
	  m_pDoc->m_pAttrCtrl->GetStld(m_pATGNData->arAddDeadLoad[2].StldKey, StldData);
		m_Load3CB.SelectString(-1, StldData.LoadCaseName);  
		m_dStoryIncr3.SetValue(m_pATGNData->arAddDeadLoad[2].dSotryIncr);
		m_dStartDay3.SetValue(m_pATGNData->arAddDeadLoad[2].dStartDay);
		m_dDayIncr3.SetValue(m_pATGNData->arAddDeadLoad[2].dDayIncr);
		break;
	default:
		m_nLoad1 = FALSE;
		m_nLoad2 = FALSE;
		m_nLoad3 = FALSE;
		break;
	}

	UpdateData(FALSE);
}

BOOL CCMLoadSTDGWizardSub::SetDlg2Data()
{
 	CString strTempName;
 
	m_pATGNData->Initialize();
	UpdateData(TRUE);
 
	m_pATGNData->bCheck = m_nStage;
	if(m_pATGNData->bCheck)
	{
	  m_StageCB.GetWindowText(strTempName);
		if(strTempName == _T(""))
		{ 
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___LoadCase_Name_was_not_sel)); 
			return FALSE; 
		}

		m_pATGNData->StldKey    = m_pDoc->m_pAttrCtrl->GetStldKey(strTempName); 	
		m_pATGNData->dStoryIncr = m_dStoryIncr.GetEditValue();
		m_pATGNData->dStageDurt = m_dStageDur.GetEditValue();
		m_pATGNData->dMemberAge = m_dMembAge.GetEditValue();

		// Error Handling
		if(m_pATGNData->dStoryIncr <= 0.){ GSaveHistoryFormatNF(_LS(IDS_CMD_ESEQ_ERROR1)); return FALSE; }
		if(m_pATGNData->dStageDurt <  0.){ GSaveHistoryFormatNF(_LS(IDS_CMD_ESEQ_ERROR2)); return FALSE; }
		if(m_pATGNData->dMemberAge <  0.){ GSaveHistoryFormatNF(_LS(IDS_CMD_ESEQ_ERROR3)); return FALSE; }
	}

	if(m_nLoad1)
	{
		m_Load1CB.GetWindowText(strTempName);
		if(strTempName == _T(""))
		{ 
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___LoadCase_Name_was_not_sel)); 
			return FALSE; 
		}

		T_ADDL_D tmpADDLData; 
		tmpADDLData.Initialize();
		tmpADDLData.StldKey    = m_pDoc->m_pAttrCtrl->GetStldKey(strTempName); 
		tmpADDLData.dSotryIncr = m_dStoryIncr1.GetEditValue();
		tmpADDLData.dStartDay  = m_dStartDay1.GetEditValue();
		tmpADDLData.dDayIncr   = m_dDayIncr1.GetEditValue();

		if(!CheckValidation(tmpADDLData)) return FALSE;
		m_pATGNData->arAddDeadLoad.Add(tmpADDLData);
	 }

	if(m_nLoad2)
	{
		m_Load2CB.GetWindowText(strTempName);
		if(strTempName == _T(""))
		{ 
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___LoadCase_Name_was_not_sel)); 
			return FALSE; 
		}

		T_ADDL_D tmpADDLData; 
		tmpADDLData.Initialize();
		tmpADDLData.StldKey    = m_pDoc->m_pAttrCtrl->GetStldKey(strTempName); 
		tmpADDLData.dSotryIncr = m_dStoryIncr2.GetEditValue();
		tmpADDLData.dStartDay  = m_dStartDay2.GetEditValue();
		tmpADDLData.dDayIncr   = m_dDayIncr2.GetEditValue();

		if(!CheckValidation(tmpADDLData)) return FALSE;
		m_pATGNData->arAddDeadLoad.Add(tmpADDLData);
	}

	if(m_nLoad3)
	{
		m_Load3CB.GetWindowText(strTempName);
		if(strTempName == _T(""))
		{ 
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___LoadCase_Name_was_not_sel)); 
			return FALSE; 
		}

		T_ADDL_D tmpADDLData; 
		tmpADDLData.Initialize();
		tmpADDLData.StldKey    = m_pDoc->m_pAttrCtrl->GetStldKey(strTempName); 
		tmpADDLData.dSotryIncr = m_dStoryIncr3.GetEditValue();
		tmpADDLData.dStartDay  = m_dStartDay3.GetEditValue();
		tmpADDLData.dDayIncr   = m_dDayIncr3.GetEditValue();

		if(!CheckValidation(tmpADDLData)) return FALSE;
		m_pATGNData->arAddDeadLoad.Add(tmpADDLData);
	}  

	return TRUE;
}

BOOL CCMLoadSTDGWizardSub::CheckValidation(T_ADDL_D &data)
{
	// Error Handling
	if(data.dSotryIncr <= 0.){ GSaveHistoryFormatNF(_LS(IDS_CMD_ESEQ_ERROR1));  return FALSE; }  
	if(data.dStartDay < 0.)  { GSaveHistoryFormatNF(_LS(IDS_CMD_ESEQ_ERROR4));  return FALSE; }  
	if(data.dDayIncr <= 0.)  { GSaveHistoryFormatNF(_LS(IDS_CMD_ESEQ_ERROR5));  return FALSE; }  

	return TRUE;
}

void CCMLoadSTDGWizardSub::OnCancel() 
{
	CDialog::OnCancel();
}

void CCMLoadSTDGWizardSub::OnAdditionalDeadloadChk1() 
{
	UpdateData(TRUE);

	CWnd* pWnd[7];
	pWnd[0] = GetDlgItem(IDC_DAED1_LOADCASE_CB);
	pWnd[1] = GetDlgItem(IDC_DAED1_STORY_INCR);
	pWnd[2] = GetDlgItem(IDC_DEAD1_STARTING_DAY);
	pWnd[3] = GetDlgItem(IDC_DEAD1_DAY_INCREASE);

	for(int i=0; i<4; i++) pWnd[i]->EnableWindow(m_nLoad1); 
}

void CCMLoadSTDGWizardSub::OnAdditionalDeadloadChk2() 
{
	UpdateData(TRUE);

	CWnd* pWnd[7];
	pWnd[0] = GetDlgItem(IDC_DAED2_LOADCASE_CB);
	pWnd[1] = GetDlgItem(IDC_DAED2_STORY_INCR);
	pWnd[2] = GetDlgItem(IDC_DEAD2_STARTING_DAY);
	pWnd[3] = GetDlgItem(IDC_DEAD2_DAY_INCREASE);

	for(int i=0; i<4; i++) pWnd[i]->EnableWindow(m_nLoad2); 
}

void CCMLoadSTDGWizardSub::OnAdditionalDeadloadChk3() 
{
	UpdateData(TRUE);

	CWnd* pWnd[7];
	pWnd[0] = GetDlgItem(IDC_DAED3_LOADCASE_CB);
	pWnd[1] = GetDlgItem(IDC_DAED3_STORY_INCR);
	pWnd[2] = GetDlgItem(IDC_DEAD3_STARTING_DAY);
	pWnd[3] = GetDlgItem(IDC_DEAD3_DAY_INCREASE);

	for(int i=0; i<4; i++) pWnd[i]->EnableWindow(m_nLoad3); 
}

void CCMLoadSTDGWizardSub::OnConstructionStageChk() 
{
	UpdateData(TRUE);

	CWnd* pWnd[4];
	pWnd[0] = GetDlgItem(IDC_STAGE_LOADCASE_CB);
	pWnd[1] = GetDlgItem(IDC_STAGE_STORY_INCR);
	pWnd[2] = GetDlgItem(IDC_STAGE_STAGE_DURATION);
	pWnd[3] = GetDlgItem(IDC_STAGE_MEMBER_AGE);

	for(int i=0; i<4; i++) pWnd[i]->EnableWindow(m_nStage); 
}
