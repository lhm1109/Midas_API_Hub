#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdIsolatorGBCoefCalcDlg.h"
#include "CmdIsolatorGBStressLmtTableDlg.h"
#include "ExportFunc.h"
#include "CMLoadCombDlg.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\DampDgnMgr_CH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCmdIsolatorGBCoefCalcDlg::CCmdIsolatorGBCoefCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdIsolatorGBCoefCalcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdIsolatorGBCoefCalcDlg)
	m_nLmtType = 0;
	m_nSeisStor = 0;
	m_nUpStor = 0;
	m_nLcom = 0;
	m_nTempStrutCategory[0] = 5;
	m_nTempStrutCategory[1] = 5;
	//}}AFX_DATA_INIT
}

void CCmdIsolatorGBCoefCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdIsolatorGBCoefCalcDlg)
	DDX_Control(pDX, IDC_CMD_LOAD_ISOLATOR_GB_COEF_CODE_CBO, m_CodeCombox);
	DDX_Control(pDX, IDC_CMD_LOAD_ISOLATOR_GB_COEF_BLD_CBO, m_BldCombox);
	DDX_Control(pDX, IDC_CMD_LOAD_ISOLATOR_GB_COEF_FORTIFICATION_CBO, m_FortificationCombox);
	DDX_Control(pDX, IDC_CMD_LOAD_ISOLATOR_GB_COEF_STRUCTURE_CBO, m_StruCombox);
	DDX_Control(pDX, IDC_CMD_LOAD_ISOLATOR_GB_COEF_LAYER_CBO, m_SeisLayerCombox);
	DDX_Control(pDX, IDC_CMD_LOAD_ISOLATOR_GB_COEF_UPLAYER_CBO, m_UpLayerCombox);
	DDX_Control(pDX, IDC_CMD_LOAD_ISOLATOR_GB_COEF_LCOM_CBO, m_LcomCombox);
	DDX_Control(pDX, IDC_CMD_LOAD_ISOLATOR_GB_COEF_CASE1_LIST, m_CaseList1);
	DDX_Control(pDX, IDC_CMD_LOAD_ISOLATOR_GB_COEF_CASE2_LIST, m_CaseList2);
	DDX_Control(pDX, IDC_CMD_LOAD_ISOLATOR_GB_COEF_CASE3_LIST, m_CaseList3);
	DDX_Radio(pDX, IDC_CMD_LOAD_ISOLATOR_GB_COEF_CODE_RADIO, m_nLmtType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCmdIsolatorGBCoefCalcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdIsolatorGBCoefCalcDlg)
	ON_BN_CLICKED(IDC_CMD_LOAD_ISOLATOR_GB_COEF_R1_BUTTON, OnList1ToList2Btn)
	ON_BN_CLICKED(IDC_CMD_LOAD_ISOLATOR_GB_COEF_L1_BUTTON, OnList2ToList1Btn)
	ON_BN_CLICKED(IDC_CMD_LOAD_ISOLATOR_GB_COEF_R2_BUTTON, OnList1ToList3Btn)
	ON_BN_CLICKED(IDC_CMD_LOAD_ISOLATOR_GB_COEF_L2_BUTTON, OnList3ToList1Btn)
	ON_BN_CLICKED(IDC_CMD_LOAD_ISOLATOR_GB_COEF_LCOM_BUTTON, OnGeneralLcomBtn)
	ON_BN_CLICKED(IDC_CMD_LOAD_ISOLATOR_GB_COEF_CODE_RADIO, OnCmdLmtTypeRdo)
	ON_BN_CLICKED(IDC_CMD_LOAD_ISOLATOR_GB_COEF_USER_RADIO, OnCmdLmtTypeRdo)
	ON_BN_CLICKED(IDC_CMD_LOAD_ISOLATOR_GB_COEF_LMT_BUTTON, OnUserDefineStressLmtBtn)
	ON_BN_CLICKED(IDC_CMD_LOAD_ISOLATOR_GB_COEF_OK_BTN, OnOK)
	ON_BN_CLICKED(IDC_CMD_LOAD_ISOLATOR_GB_COEF_CANCEL_BTN, OnCancel)
	ON_CBN_SELCHANGE(IDC_CMD_LOAD_ISOLATOR_GB_COEF_LCOM_CBO, OnSelChangeLcomList)
	ON_CBN_SELCHANGE(IDC_CMD_LOAD_ISOLATOR_GB_COEF_CODE_CBO, OnSelChangeCodeList)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CCmdIsolatorGBCoefCalcDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();
	InitCacheList();

	SetCodeCmb();
	SetBuildingCategoryCmb();
	SetFortificationCategoryCmb();
	SetStructureCategoryCmb();
	SetIsolationSeisLayerCmb();
	SetIsolationUpLayerCmb();
	SetLcomGCmb();
	Data2Dlg();
	return TRUE;
}

void CCmdIsolatorGBCoefCalcDlg::Data2Dlg()
{
	if (m_Data.nLmtType == 0)
	{
		((CButton *)GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_CODE_RADIO))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_USER_RADIO))->SetCheck(FALSE);
		GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_LMT_BUTTON)->EnableWindow(FALSE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_CODE_RADIO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_USER_RADIO))->SetCheck(TRUE);
		GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_LMT_BUTTON)->EnableWindow(TRUE);
	}
	m_CodeCombox.SetCurSel(m_Data.nCode);
	UpdateCaseNewList();

	
	m_nPreCode = m_Data.nCode;
	m_FortificationCombox.SetCurSel(m_Data.nFortificationCategory);
	m_BldCombox.SetCurSel(m_Data.nBldCategory);
	m_StruCombox.SetCurSel(m_Data.nStrutCategory);
	if(m_Data.nCode == 0)
	{
		m_nTempStrutCategory[0] = m_Data.nStrutCategory;
	}
	else
	{
		m_nTempStrutCategory[1] = m_Data.nStrutCategory;
	}
	m_SeisLayerCombox.SetCurSel(m_nSeisStor);
	m_UpLayerCombox.SetCurSel(m_nUpStor);
		
	m_LcomCombox.SetCurSel(m_nLcom);
	OnSelChangeCodeList();
}

BOOL CCmdIsolatorGBCoefCalcDlg::Dlg2Data()
{
	UpdateData(TRUE);
	//m_Data.Initialize();
	m_Data.nCode = m_CodeCombox.GetCurSel();
	m_Data.nFortificationCategory = m_FortificationCombox.GetCurSel();
	m_Data.nBldCategory = m_BldCombox.GetCurSel();
	m_Data.nStrutCategory = m_StruCombox.GetCurSel();
	if (m_SeisLayerCombox.GetCount() < 1 || m_UpLayerCombox.GetCount() < 1)
	{
		AfxMessageBox(_LS(IDS_CMD_LOAD_ISOLATION_LAYER_ERROR), MB_OK);
		return FALSE;
	}
	CString strSeisLayer, strLayer, strLcom;
	m_SeisLayerCombox.GetWindowText(strSeisLayer);
	m_Data.nSeisLayer = m_pDoc->m_pAttrCtrl->GetStorKey(strSeisLayer);
	m_UpLayerCombox.GetWindowText(strLayer);
	m_Data.nBeginLayer = m_pDoc->m_pAttrCtrl->GetStorKey(strLayer);
	if (m_LcomCombox.GetCount() < 1)
	{
		AfxMessageBox(_LS(IDS_CMD_LOAD_ISOLATION_LCOM_ERROR), MB_OK);
		return FALSE;
	}
	m_LcomCombox.GetWindowText(strLcom);
	m_Data.nLcomG = m_pDoc->m_pAttrCtrl->GetLcomKey(D_LCOMTYPE_GENERAL, strLcom);

	CStringArray Contents;
	DWORD nKey;
	m_Data.aResistSplcCase.RemoveAll();
	m_Data.aResistThisCase.RemoveAll();
	m_Data.aRareEThisCase.RemoveAll();
	m_Data.aRareESplcCase.RemoveAll();
	m_Data.aExtreRareEThisCase.RemoveAll();
	m_Data.aExtreRareESplcCase.RemoveAll();
	int nList1 = m_CaseList1.GetItemCount();
	for (int i=0; i<nList1; i++)
	{
		Contents.RemoveAll();
		CDlgUtil::GetListItem(&m_CaseList1, i, Contents, nKey);
		ASSERT(m_TempListData[m_Data.nCode][0][i].nKey == nKey);
		if(m_TempListData[m_Data.nCode][0][i].nType == 1)
		{
			m_Data.aResistSplcCase.Add((T_SPLC_K)nKey);
		}	
		else
		{
			m_Data.aResistThisCase.Add((T_THIS_K)nKey);
		}
	}

	int nList2 = m_CaseList2.GetItemCount();
	for (int i = 0; i < nList2; i++)
	{
		Contents.RemoveAll();
		CDlgUtil::GetListItem(&m_CaseList2, i, Contents, nKey);

		ASSERT(m_TempListData[m_Data.nCode][1][i].nKey == nKey);
		if (m_TempListData[m_Data.nCode][1][i].nType == 1)
		{
			m_Data.aRareESplcCase.Add((T_SPLC_K)nKey);
		}
		else
		{
			m_Data.aRareEThisCase.Add((T_THIS_K)nKey);
		}
	}

	int nList3 = m_CaseList3.GetItemCount();
	for (int i = 0; i < nList3; i++)
	{
		Contents.RemoveAll();
		CDlgUtil::GetListItem(&m_CaseList3, i, Contents, nKey);

		ASSERT(m_TempListData[m_Data.nCode][2][i].nKey == nKey);
		if (m_TempListData[m_Data.nCode][2][i].nType == 1)
		{
			m_Data.aExtreRareESplcCase.Add((T_SPLC_K)nKey);
		}
		else
		{
			m_Data.aExtreRareEThisCase.Add((T_THIS_K)nKey);
		}
	}
	m_Data.nLmtType = GetCheckedRadioButton(IDC_CMD_LOAD_ISOLATOR_GB_COEF_CODE_RADIO, IDC_CMD_LOAD_ISOLATOR_GB_COEF_USER_RADIO) == IDC_CMD_LOAD_ISOLATOR_GB_COEF_CODE_RADIO ? 0 : 1;
	return TRUE;

}

void CCmdIsolatorGBCoefCalcDlg::OnList1ToList2Btn()
{
	OnMoveList(0);
}

void CCmdIsolatorGBCoefCalcDlg::OnList2ToList1Btn()
{
	OnMoveList(1);
}

void CCmdIsolatorGBCoefCalcDlg::OnList1ToList3Btn()
{
	OnMoveList(2);
}

void CCmdIsolatorGBCoefCalcDlg::OnList3ToList1Btn()
{
	OnMoveList(3);
}

void CCmdIsolatorGBCoefCalcDlg::OnGeneralLcomBtn()
{
	CreateOrActivateDlg((CDBDoc*)CDBDoc::GetDocPoint(), CCMLoadCombDlg::IDD, NULL);
}

void CCmdIsolatorGBCoefCalcDlg::OnUserDefineStressLmtBtn()
{
	CCmdIsolatorGBStressLmtTableDlg dlg;
	dlg.SetParamData(&m_Data);
	dlg.DoModal();
	if (!dlg.GetParamData(&m_Data))
		return;
	m_Data.nLmtType = 1;
}

void CCmdIsolatorGBCoefCalcDlg::OnCmdLmtTypeRdo()
{
	UpdateData(TRUE);
	if (GetCheckedRadioButton(IDC_CMD_LOAD_ISOLATOR_GB_COEF_CODE_RADIO, IDC_CMD_LOAD_ISOLATOR_GB_COEF_USER_RADIO) == IDC_CMD_LOAD_ISOLATOR_GB_COEF_CODE_RADIO)
	{
		((CButton *)GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_CODE_RADIO))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_USER_RADIO))->SetCheck(FALSE);
		GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_LMT_BUTTON)->EnableWindow(FALSE);
		m_nLmtType = 0;
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_CODE_RADIO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_USER_RADIO))->SetCheck(TRUE);
		GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_LMT_BUTTON)->EnableWindow(TRUE);
		m_nLmtType = 1;
	}
	UpdateData(FALSE);
	RedrawWindow();
}

void CCmdIsolatorGBCoefCalcDlg::OnOK()
{
	if (!Dlg2Data())	return;
	CArray<T_NLNK_K, T_NLNK_K> aNlnK;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetKeyListStorUserNlnk(m_Data.nSeisLayer, 1, &aNlnK);
	if (aNlnK.GetCount() < 1)
	{
		AfxMessageBox(_LS(IDS_CMD_ISGB_SUPPORT_NOT_EXSIT), MB_OK);
		return;
	}
	//m_pDoc->m_pUnitCtrl->ConvertUnitIsolationIn(m_Data);
	if(!CDBDoc::GetDocPoint()->m_pDataCtrl->AddIsolation(m_Data)) return;
	CDialogMove::OnOK();
}

void CCmdIsolatorGBCoefCalcDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CCmdIsolatorGBCoefCalcDlg::SetCodeCmb()
{
	CString strCode[] = {_LS(IDS_MAIN_RIBBON_DAMP_CODE_GB_51408_2021),_LS(IDS_MAIN_RIBBON_DAMP_CODE_GB_11_2075_2022)};
	m_CodeCombox.ResetContent();
	for(int i = 0; i < 2; i++)
	{
		m_CodeCombox.AddString(strCode[i]);
	}
	m_CodeCombox.SetCurSel(0);
}

void CCmdIsolatorGBCoefCalcDlg::SetFortificationCategoryCmb()
{
	CString strBld[3] = { _LS(IDS_CMD_LOAD_SPECIAL_FORTIFICATION) , _LS(IDS_CMD_LOAD_KEY_FORTIFICATION) , _LS(IDS_CMD_LOAD_STANDARD_FORTIFICATION) };
	m_FortificationCombox.ResetContent();
	m_FortificationCombox.AddString(strBld[0]);
	m_FortificationCombox.AddString(strBld[1]);
	m_FortificationCombox.AddString(strBld[2]);
	m_FortificationCombox.SetCurSel(1);
}

void CCmdIsolatorGBCoefCalcDlg::SetBuildingCategoryCmb()
{
	CString strBld[3] = {_LS(IDS_CMD_DAGB_Building_Type_1) , _LS(IDS_CMD_DAGB_Building_Type_2),_LS(IDS_CMD_DAGB_Building_Type_3)};
	m_BldCombox.ResetContent();
	m_BldCombox.AddString(strBld[0]);
	m_BldCombox.AddString(strBld[1]);
	//int nCodeType = CDampDesignMgr_CH::Instance()->GetDampIsoCodeType();
	//if(nCodeType == 1)
	{
		m_BldCombox.AddString(strBld[2]);
	}
	m_BldCombox.SetCurSel(0);
}

void CCmdIsolatorGBCoefCalcDlg::SetStructureCategoryCmb()
{
	m_StruCombox.ResetContent();
	if(m_CodeCombox.GetCurSel() == 0)
	{
		CString _tcsstructure[6] = {_LS(IDS_CMD_LOAD_REINFORCED_C_FRAME_STRUCTURE),_LS(IDS_CMD_LOAD_REINFORCED_C_FRAME_SEIS_WALL_STRUCTURE),_LS(IDS_CMD_LOAD_FRAME_CORE_CUBE_STRUCTURE),
			_LS(IDS_CMD_LOAD_PLATE_PRISMATIC_SEIS_WALL_STRUCTURE),_LS(IDS_CMD_LOAD_REINFORCED_C_SEIS_WALL_STRUCTURE),_LS(IDS_CMD_LOAD_STEEL_STRUCTURE)};

		for(int i = 0; i < 6; i++)
		{
			m_StruCombox.AddString(_tcsstructure[i]);
		}
	}
	else
	{
		CString _tcsstructure[8] = {_LS(IDS_CMD_LOAD_REINFORCED_C_FRAME_STRUCTURE),_LS(IDS_CMD_LOAD_REINFORCED_C_FRAME_SEIS_WALL_STRUCTURE),
			_LS(IDS_CMD_LOAD_FRAME_CORE_CUBE_STRUCTURE),_LS(IDS_CMD_LOAD_REINFORCED_C_SEIS_WALL_STRUCTURE),
			_LS(IDS_CMD_LOAD_PLATE_PRISMATIC_SEIS_WALL_STRUCTURE),_LS(IDS_CMD_LOAD_TUBE_IN_TUBE),_LS(IDS_CMD_LOAD_REINFORCED_C_FRAME_SUPPORT_STRUCTURE),
			_LS(IDS_CMD_LOAD_Multi_STOREY_STEEL_STRUCTURE)
		};
		for(int i = 0; i < 8; i++)
		{
			m_StruCombox.AddString(_tcsstructure[i]);
		}
	}
	//m_StruCombox.SetCurSel(5);
}

void CCmdIsolatorGBCoefCalcDlg::SetIsolationSeisLayerCmb()
{
	m_SeisLayerCombox.ResetContent();
	CArray<T_STOR_K, T_STOR_K> aKey, aUgStorK;
	m_pDoc->m_pAttrCtrl->GetStorKeyByGL(aKey, aUgStorK);
	if (aKey.GetCount() < 1)	return;
	
	T_STOR_D StorD, StorD_Seis;	StorD_Seis.Initialize();
	m_pDoc->m_pAttrCtrl->GetStor(m_Data.nSeisLayer, StorD_Seis);

	for (int i = 0; i < aKey.GetCount(); i++)
	{
		StorD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetStor(aKey[i], StorD))
		{
			ASSERT(0);
			continue;
		}
		if (StorD_Seis.StoryName.Compare(StorD.StoryName) == 0)
			m_nSeisStor = i;
		m_SeisLayerCombox.AddString(StorD.StoryName);
	}
	m_SeisLayerCombox.SetCurSel(0);
}

void CCmdIsolatorGBCoefCalcDlg::SetIsolationUpLayerCmb()
{
	m_UpLayerCombox.ResetContent();
	CArray<T_STOR_K, T_STOR_K> aKey, aUgStorK;
	m_pDoc->m_pAttrCtrl->GetStorKeyByGL(aKey, aUgStorK);
	if (aKey.GetCount() < 1)	return;

	T_STOR_D StorD_Up;	StorD_Up.Initialize();
	m_pDoc->m_pAttrCtrl->GetStor(m_Data.nBeginLayer, StorD_Up);
	
	T_STOR_D StorD;
	for (int i = 0; i < aKey.GetCount(); i++)
	{
		StorD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetStor(aKey[i], StorD))
		{
			ASSERT(0);
			continue;
		}
		if (StorD_Up.StoryName.Compare(StorD.StoryName) == 0)
			m_nUpStor = i;
		m_UpLayerCombox.AddString(StorD.StoryName);
	}
	if (m_UpLayerCombox.GetCount()>1)
		m_UpLayerCombox.SetCurSel(1);
	else
		m_UpLayerCombox.SetCurSel(0);
}

void CCmdIsolatorGBCoefCalcDlg::SetLcomGCmb()
{
	m_LcomCombox.ResetContent();
	CArray<T_LCOM_K, T_LCOM_K> aLcomKey;
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_GENERAL, aLcomKey);
	if (aLcomKey.GetCount() < 1) 	return;
	
	T_LCOM_D LcomD;
	for (int i = 0; i < aLcomKey.GetCount(); i++)
	{
		LcomD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_GENERAL, aLcomKey[i], LcomD))
		{
			ASSERT(0);
			continue;
		}
		if (m_Data.nLcomG == aLcomKey[i])
			m_nLcom = i;
		m_LcomCombox.AddString(LcomD.LoadCombName);
	}

}

void CCmdIsolatorGBCoefCalcDlg::OnSelChangeLcomList()
{
	
}

void CCmdIsolatorGBCoefCalcDlg::OnSelChangeCodeList()
{
	BOOL bDB11_2075_2022 = m_CodeCombox.GetCurSel() == 1;
	GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_BLD_STATIC)->EnableWindow(bDB11_2075_2022);
	GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_BLD_CBO)->EnableWindow(bDB11_2075_2022);
	//Save Independent data
	int nPreStruCategory = m_StruCombox.GetCurSel();
	if(m_nPreCode == 0)
	{
		m_nTempStrutCategory[0] = nPreStruCategory;
	}
	else
	{
		m_nTempStrutCategory[1] = nPreStruCategory;
	}
	m_nPreCode = m_CodeCombox.GetCurSel();
	SetStructureCategoryCmb();
	if(bDB11_2075_2022)
	{
		m_StruCombox.SetCurSel(m_nTempStrutCategory[1]);
		GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_FREQ_STATIC)->SetWindowText(_LS(IDS_CMD0417__Frequent_E_Q_));
		GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_SCARCE_STATIC)->SetWindowText(_LS(IDS_CMD0417__Middle_E_Q_));
		GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_RARE_STATIC)->SetWindowText(_LS(IDS_CMD0417__Scarce_E_Q_));
	}
	else
	{
		m_StruCombox.SetCurSel(m_nTempStrutCategory[0]);
		GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_FREQ_STATIC)->SetWindowText(_LS(IDS_CMD0417__Middle_E_Q_));
		GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_SCARCE_STATIC)->SetWindowText(_LS(IDS_CMD0417__Scarce_E_Q_));
		GetDlgItem(IDC_CMD_LOAD_ISOLATOR_GB_COEF_RARE_STATIC)->SetWindowText(_LS(IDS_CMD0417__Very_Scarce_E_Q_));
	}
	UpdateCaseNewList();
}

void CCmdIsolatorGBCoefCalcDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	switch (lHint)
	{
	case D_UPDATE_DGN_FORCE_CHANGED:
		SetLcomGCmb();
		m_LcomCombox.SetCurSel(0);
	default:
		break;
	}
}


/************************************************************************/
/*					Update DB11/2075-2022 UI Data						*/
/************************************************************************/

void CCmdIsolatorGBCoefCalcDlg::InitCacheList()
{
	T_SPLC_D SplcD;
	T_THIS_D ThisD;
	CArray<T_SPLC_K, T_SPLC_K> SplcKeys;
	CArray<T_THIS_K, T_THIS_K> aThisKey;
	//1.Init DB Code;if DB NOExist AutomaticGetSpfcListType
	if(!m_pDoc->m_pAttrCtrl->GetIsolation(m_Data))
	{
		m_Data.Initialize();
		m_pDoc->m_pUnitCtrl->ConvertUnitIsolationOut(m_Data);
		m_Data.nCode = CDampDesignMgr_CH::Instance()->GetDampIsoCodeType();
		int nCodeType = m_Data.nCode;
		m_pDoc->m_pAttrCtrl->GetSplcKeyList(SplcKeys);
		int nSplc = SplcKeys.GetCount();
		for(int i = 0; i < nSplc; i++)
		{
			SplcD.Initialize();
			if(m_pDoc->m_pAttrCtrl->GetSplc(SplcKeys[i], SplcD))
			{
				int nListType = AutomaticGetSpfcListType(SplcD.aSpfcKey[0], nCodeType);
				if(nListType < 0)
					continue;
				m_aCacheList[nCodeType][0][nListType].Add(SplcKeys[i]);
			}
		}
		m_pDoc->m_pAttrCtrl->GetThisKeyList(aThisKey, FALSE);
		for(int i = 0; i < aThisKey.GetCount(); i++)
		{
			m_aCacheList[nCodeType][1][0].Add(aThisKey[i]);
		}
	}
	else
	{
		ASSERT(m_Data.nCode == CDampDesignMgr_CH::Instance()->GetDampIsoCodeType());
		int nCodeType = m_Data.nCode;
		m_aCacheList[nCodeType][0][0].Copy(m_Data.aResistSplcCase);
		m_aCacheList[nCodeType][0][1].Copy(m_Data.aRareESplcCase);
		m_aCacheList[nCodeType][0][2].Copy(m_Data.aExtreRareESplcCase);

		m_aCacheList[nCodeType][1][0].Copy(m_Data.aResistThisCase);
		m_aCacheList[nCodeType][1][1].Copy(m_Data.aRareEThisCase);
		m_aCacheList[nCodeType][1][2].Copy(m_Data.aExtreRareEThisCase);
	}
	//2.Init Other DB Code
	int nCodeType = m_Data.nCode;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(SplcKeys);
	int nSplc = SplcKeys.GetCount();
	for(int i = 0; i < nSplc; i++)
	{
		SplcD.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetSplc(SplcKeys[i], SplcD))
		{
			int nListType = AutomaticGetSpfcListType(SplcD.aSpfcKey[0], 1 - nCodeType);
			if(nListType < 0)
				continue;
			m_aCacheList[1-nCodeType][0][nListType].Add(SplcKeys[i]);
		}
	}
	m_pDoc->m_pAttrCtrl->GetThisKeyList(aThisKey, FALSE);
	for(int i = 0; i < aThisKey.GetCount(); i++)
	{
		m_aCacheList[1 - nCodeType][1][0].Add(aThisKey[i]);
	}
}

int CCmdIsolatorGBCoefCalcDlg::AutomaticGetSpfcListType(UINT Key, int nCodeType)
{
	T_SPFC_D SpfcD;
	if(!m_pDoc->m_pAttrCtrl->GetSpfc(Key, SpfcD))
		return 0;
	int nListType =0;
	if(nCodeType == 0)
	{
		if(SpfcD.nSpecCodeType == D_SPFC_CODE_CH2019)
		{
			if(SpfcD.CodeParam.CH2019.nEarthResponse == 1)
				nListType = 0;
			else if(SpfcD.CodeParam.CH2019.nEarthResponse == 2)
				nListType = 1;
			else
			{
				nListType = -1;
			}
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_CH2010)
		{
			if(SpfcD.CodeParam.CH2010.nEarthResponse == 1)
				nListType = 0;
			else if(SpfcD.CodeParam.CH2010.nEarthResponse == 2)
				nListType = 1;
			else
			{
				nListType = -1;
			}
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_CH_GB_T_51408_2021)
		{
			if(SpfcD.CodeParam.GB_T_51408_2021.nEarthResponse == 0)
				nListType = 0;
			else if(SpfcD.CodeParam.GB_T_51408_2021.nEarthResponse == 1)
				nListType = 1;
			else if(SpfcD.CodeParam.GB_T_51408_2021.nEarthResponse == 2)
				nListType = 2;
			else
			{
				ASSERT(FALSE);
				nListType = -1;
			}
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_CH2002)
		{
			if(SpfcD.CodeParam.CH2002.nEarthResponse == 1)
				nListType = 1;
			else
			{
				nListType = -1;
			}
		}
		else
			ASSERT(0);
	}
	else
	{
		if(SpfcD.nSpecCodeType == D_SPFC_CODE_CH2019)
		{
			if(SpfcD.CodeParam.CH2019.nEarthResponse == 1)
				nListType = 1;
			else if(SpfcD.CodeParam.CH2019.nEarthResponse == 2)
				nListType = 2;
			else
			{
				nListType = 0;
			}
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_CH2010)
		{
			if(SpfcD.CodeParam.CH2010.nEarthResponse == 1)
				nListType = 1;
			else if(SpfcD.CodeParam.CH2010.nEarthResponse == 2)
				nListType = 2;
			else
			{
				nListType = 0;
			}
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_CH_GB_T_51408_2021)
		{
			if(SpfcD.CodeParam.GB_T_51408_2021.nEarthResponse == 0)
				nListType = 1;
			else if(SpfcD.CodeParam.GB_T_51408_2021.nEarthResponse == 1)
				nListType = 2;
			else
				nListType = -1;
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_CH2002)
		{
			if(SpfcD.CodeParam.CH2002.nEarthResponse == 0)
				nListType = 0;
			else if(SpfcD.CodeParam.CH2002.nEarthResponse == 1)
				nListType = 2;
			else
			{
				ASSERT(0);
				nListType = -1;
			}
		}
		else
			ASSERT(0);
	}
	return nListType;
}

void CCmdIsolatorGBCoefCalcDlg::AutomaticAddList(UINT Key, int nRsType, int nCodeType /* = 0 */)
{
	int nListIndex = 0;
	bool exist = false;
	for(; nListIndex < 3; nListIndex++)
	{
		const CArray<UINT, UINT>& aTempArray = m_aCacheList[nCodeType][nRsType][nListIndex];
		for(int j = 0; j < aTempArray.GetSize(); j++)
		{
			if(aTempArray[j] == Key)
			{
				exist = true;
				break;
			}
		}
		if(exist)
			break;
	}
	if(!exist)
	{
		return;
	}
	if(nListIndex > 2)
	{
		ASSERT(FALSE);
		nListIndex = 0;
	}
	//Add List
	CStringArray Contents;	
	Contents.RemoveAll();
	T_CASE_TYPE CaseD;
	CaseD.Initialize();
	CListCtrl* pListCtrl = 0;
	if(nListIndex == 0)
		pListCtrl = &m_CaseList1;
	else if(nListIndex == 1)
		pListCtrl = &m_CaseList2;
	else if(nListIndex == 2)
		pListCtrl = &m_CaseList3;
	else
	{
		ASSERT(FALSE);
		pListCtrl = &m_CaseList1;
	}
	if(nRsType == 0)
	{
		T_SPLC_D SplcD;
		m_pDoc->m_pAttrCtrl->GetSplc(Key, SplcD);
		Contents.Add(SplcD.LoadCaseName);
		CaseD.nType = 1;
		CaseD.strName = SplcD.LoadCaseName;
	}
	else
	{
		T_THIS_D ThisD;
		m_pDoc->m_pAttrCtrl->GetThis(Key, ThisD);
		Contents.Add(ThisD.LoadCaseName);
		CaseD.strName = ThisD.LoadCaseName;
		CaseD.nType = 0;
	}
	CaseD.nKey = Key;
	CDlgUtil::SetListItem(pListCtrl, m_TempListData[nCodeType][nListIndex].GetCount(), Contents, Key);
	m_TempListData[nCodeType][nListIndex].Add(CaseD);
}

void CCmdIsolatorGBCoefCalcDlg::UpdateCaseNewList()
{
	m_CaseList1.DeleteAllItems();
	m_CaseList2.DeleteAllItems();
	m_CaseList3.DeleteAllItems();
	int nCodeType = m_CodeCombox.GetCurSel();
	for(int i = 0; i < 3; i++)
	{
		m_TempListData[nCodeType][i].RemoveAll();
	}
	T_SPLC_D SplcD;
	T_THIS_D ThisD;
	CArray<T_SPLC_K, T_SPLC_K> SplcKeys;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(SplcKeys);
	int nSplc = SplcKeys.GetCount();
	
	for(int i = 0; i < nSplc; i++)
	{
		SplcD.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetSplc(SplcKeys[i], SplcD))
		{
			AutomaticAddList(SplcKeys[i], 0, nCodeType);
		}
	}

	CArray<T_THIS_K, T_THIS_K> aThisKey;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(aThisKey, FALSE);
	for(int i = 0; i < aThisKey.GetCount(); i++)
	{
		AutomaticAddList(aThisKey[i], 1, nCodeType);
	}
}

void CCmdIsolatorGBCoefCalcDlg::OnMoveList(int nType)
{
	/*nType : 0 list1->list2 ; 1 list2->list1 ; 2 list1->list3 ; 3 list3->list1 */
	//CListCtrl
	CListCtrl* pListCtrlSt = 0;
	CListCtrl* pListCtrlEnd = 0;
	//CListCtrl所对应的UI数据
	CArray<T_CASE_TYPE, T_CASE_TYPE>* pStCacheData = 0;
	CArray<T_CASE_TYPE, T_CASE_TYPE>* pEndCacheData = 0;
	//缓冲数据库数据;nIndex为spfc和this 
	CArray<UINT, UINT>* pStCacheDB[2] = {0};
	CArray<UINT, UINT>* pEndCacheDB[2] = {0};

	int nCodeType = m_CodeCombox.GetCurSel();
	if(nType == 0 || nType == 2)
	{
		pListCtrlSt = &m_CaseList1;
		pStCacheData = &m_TempListData[nCodeType][0];
		if(nType == 0)
		{
			pListCtrlEnd = &m_CaseList2;
			pEndCacheData = &m_TempListData[nCodeType][1];
		}
		else
		{
			pListCtrlEnd = &m_CaseList3;
			pEndCacheData = &m_TempListData[nCodeType][2];
		}
	}
	else
	{
		pListCtrlEnd = &m_CaseList1;
		pEndCacheData = &m_TempListData[nCodeType][0];
		if(nType == 1)
		{
			pListCtrlSt = &m_CaseList2;
			pStCacheData = &m_TempListData[nCodeType][1];
		}
		else
		{
			pListCtrlSt = &m_CaseList3;
			pStCacheData = &m_TempListData[nCodeType][2];
		}
			
	}

	//1.Get Select Index
	CArray<int, int> arIndex;
	CArray<CString, CString> arCaseName;
	CArray<DWORD, DWORD> SItem;
	CDlgUtil::GetSelectedListItemIndex(pListCtrlSt, arIndex);
	CStringArray Contents;
	//2.Delete StartList
	CArray<T_CASE_TYPE, T_CASE_TYPE> aCaseData;
	int nItem = pListCtrlSt->GetItemCount();
	DWORD nKey;
	for(int i = nItem - 1; i >= 0; i--)
	{
		bool bdel = false;
		for(int k = 0; k < arIndex.GetSize(); k++)
		{
			if(arIndex[k] == i)
			{
				bdel = true;
				break;
			}
		}
		if(bdel)
		{
			Contents.RemoveAll();
			CDlgUtil::GetListItem(pListCtrlSt, i, Contents, nKey);
			if(nKey != pStCacheData->GetAt(i).nKey)
			{
				ASSERT(FALSE);
			}
			pListCtrlSt->DeleteItem(i);
			aCaseData.Add(pStCacheData->GetAt(i));
			arCaseName.Add(Contents[0]);
			pStCacheData->RemoveAt(i);
		}
	}
	//3.Add EndList
	int nItems = pListCtrlEnd->GetItemCount();
	for(int i = 0; i < aCaseData.GetCount(); i++)
	{
		Contents.RemoveAll();
		Contents.Add(arCaseName[i]);
		CDlgUtil::SetListItem(pListCtrlEnd, nItems + i, Contents, aCaseData[i].nKey);
		pEndCacheData->Add(aCaseData[i]);
	}
}