#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdDampGBCoefCalcDlg.h"
//#include "CmdIsolatorGBStressLmtTableDlg.h"
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


CCmdDampingGBCoefCalcDlg::CCmdDampingGBCoefCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdDampingGBCoefCalcDlg::IDD, pParent)
{
}

void CCmdDampingGBCoefCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdDampingGBCoefCalcDlg)
	DDX_Control(pDX, IDC_CMD_LOAD_DAMPING_GB_COEF_CODE_CBO, m_CodeCombox);
	DDX_Control(pDX, IDC_CMD_LOAD_DAMPING_GB_COEF_BLD_CBO, m_BldCombox);
	DDX_Control(pDX, IDC_CMD_LOAD_DAMPING_GB_COEF_STRUCTURE_CBO, m_StruCombox);
	DDX_Control(pDX, IDC_CMD_LOAD_DAMPING_GB_COEF_CASE1_LIST, m_CaseList[0]);
	DDX_Control(pDX, IDC_CMD_LOAD_DAMPING_GB_COEF_CASE2_LIST, m_CaseList[1]);
	DDX_Control(pDX, IDC_CMD_LOAD_DAMPING_GB_COEF_CASE3_LIST, m_CaseList[2]);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCmdDampingGBCoefCalcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdDampingGBCoefCalcDlg)
	ON_BN_CLICKED(IDC_CMD_LOAD_DAMPING_GB_COEF_R1_BUTTON, OnList1ToList2Btn)
	ON_BN_CLICKED(IDC_CMD_LOAD_DAMPING_GB_COEF_L1_BUTTON, OnList2ToList1Btn)
	ON_BN_CLICKED(IDC_CMD_LOAD_DAMPING_GB_COEF_R2_BUTTON, OnList1ToList3Btn)
	ON_BN_CLICKED(IDC_CMD_LOAD_DAMPING_GB_COEF_L2_BUTTON, OnList3ToList1Btn)
	ON_BN_CLICKED(IDC_CMD_LOAD_DAMPING_GB_COEF_OK_BTN, OnOK)
	ON_BN_CLICKED(IDC_CMD_LOAD_DAMPING_GB_COEF_CANCEL_BTN, OnCancel)
	ON_CBN_SELCHANGE(IDC_CMD_LOAD_DAMPING_GB_COEF_CODE_CBO, OnSelChangeCodeList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CCmdDampingGBCoefCalcDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();
	InitCacheList();

	SetCodeCmb();
	SetBuildingCategoryCmb();
	SetStructureCategoryCmb();
	Data2Dlg();

	return TRUE;
}

void CCmdDampingGBCoefCalcDlg::Data2Dlg()
{
	m_CodeCombox.SetCurSel(m_Data.nCode);
	UpdateCaseNewList();

	m_CodeCombox.SetCurSel(m_Data.nCode);
	m_BldCombox.SetCurSel(m_Data.nBldCategory);
	m_StruCombox.SetCurSel(m_Data.nStrutCategory);
}

BOOL CCmdDampingGBCoefCalcDlg::Dlg2Data()
{
	UpdateData(TRUE);
	//m_Data.Initialize();
	m_Data.nCode = m_CodeCombox.GetCurSel();
	m_Data.nBldCategory = m_BldCombox.GetCurSel();
	m_Data.nStrutCategory = m_StruCombox.GetCurSel();

	CStringArray Contents;
	DWORD nKey;
	m_Data.aNonUseThisCase.RemoveAll();
	m_Data.aNonUseSplcCase.RemoveAll();
	m_Data.aResistSplcCase.RemoveAll();
	m_Data.aResistThisCase.RemoveAll();
	m_Data.aRareEThisCase.RemoveAll();
	m_Data.aRareESplcCase.RemoveAll();

	CArray<T_STLD_K, T_STLD_K>* pArrayCase[2][3];
	pArrayCase[1][0] = &m_Data.aNonUseSplcCase;
	pArrayCase[1][1] = &m_Data.aResistSplcCase;
	pArrayCase[1][2] = &m_Data.aRareESplcCase;

	pArrayCase[0][0] = &m_Data.aNonUseThisCase;
	pArrayCase[0][1] = &m_Data.aResistThisCase;
	pArrayCase[0][2] = &m_Data.aRareEThisCase;

	for(int j = 0; j < 3; j++)
	{
		int nListCount = m_CaseList[j].GetItemCount();
		for(int i = 0; i < nListCount; i++)
		{
			Contents.RemoveAll();
			CDlgUtil::GetListItem(&m_CaseList[j], i, Contents, nKey);

			ASSERT(m_ListData[m_Data.nCode][j][i].nKey == nKey);
			if(m_ListData[m_Data.nCode][j][i].nType == 1)
			{
				pArrayCase[1][j]->Add((T_SPLC_K)nKey);
			}
			else
			{
				pArrayCase[0][j]->Add((T_THIS_K)nKey);
			}
		}
	}
	return TRUE;
}

void CCmdDampingGBCoefCalcDlg::OnList1ToList2Btn()
{
	OnMoveList(0);
}

void CCmdDampingGBCoefCalcDlg::OnList2ToList1Btn()
{
	OnMoveList(1);
}

void CCmdDampingGBCoefCalcDlg::OnList1ToList3Btn()
{
	OnMoveList(2);
}

void CCmdDampingGBCoefCalcDlg::OnList3ToList1Btn()
{
	OnMoveList(3);
}

void CCmdDampingGBCoefCalcDlg::OnOK()
{
	if (!Dlg2Data())	return;
	if(!CDBDoc::GetDocPoint()->m_pDataCtrl->AddDamping(m_Data)) return;
	CDialogMove::OnOK();
}

void CCmdDampingGBCoefCalcDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CCmdDampingGBCoefCalcDlg::SetCodeCmb()
{
	CString strCode[] = { _LS(IDS_MAIN_RIBBON_DAMP_CODE_Seismic_Technology_Approval),_LS(IDS_MAIN_RIBBON_DAMP_CODE_GB_11_2075_2022) };
	m_CodeCombox.ResetContent();
	for (int i = 0; i < 2; i++)
	{
		m_CodeCombox.AddString(strCode[i]);
	}
}

void CCmdDampingGBCoefCalcDlg::SetBuildingCategoryCmb()
{
	CString strBld[3] = { _LS(IDS_CMD_DAGB_Building_Type_1) , _LS(IDS_CMD_DAGB_Building_Type_2),_LS(IDS_CMD_DAGB_Building_Type_3) };
	m_BldCombox.ResetContent();
	m_BldCombox.AddString(strBld[0]);
	m_BldCombox.AddString(strBld[1]);
	int nCodeType = CDampDesignMgr_CH::Instance()->GetDampIsoCodeType();
	if (nCodeType == 1)
	{
		m_BldCombox.AddString(strBld[2]);
	}
}

void CCmdDampingGBCoefCalcDlg::SetStructureCategoryCmb()
{
	CString _tcsstructure[8] = { _LS(IDS_CMD_LOAD_REINFORCED_C_FRAME_STRUCTURE),_LS(IDS_CMD_LOAD_REINFORCED_C_FRAME_SEIS_WALL_STRUCTURE),
		_LS(IDS_CMD_LOAD_FRAME_CORE_CUBE_STRUCTURE),_LS(IDS_CMD_LOAD_PLATE_PRISMATIC_SEIS_WALL_STRUCTURE),_LS(IDS_CMD_LOAD_REINFORCED_C_SEIS_WALL_STRUCTURE),
		_LS(IDS_CMD_LOAD_TUBE_IN_TUBE),_LS(IDS_CMD_LOAD_REINFORCED_C_FRAME_SUPPORT_STRUCTURE),
		_LS(IDS_CMD_LOAD_Multi_STOREY_STEEL_STRUCTURE)
	};
	m_StruCombox.ResetContent();
	for(int i = 0; i < 8; i++)
	{
		m_StruCombox.AddString(_tcsstructure[i]);
	}
}

void CCmdDampingGBCoefCalcDlg::OnSelChangeCodeList()
{
	CString strBld[3] = { _LS(IDS_CMD_DAGB_Building_Type_1) , _LS(IDS_CMD_DAGB_Building_Type_2),_LS(IDS_CMD_DAGB_Building_Type_3) };
	m_BldCombox.ResetContent();
	m_BldCombox.AddString(strBld[0]);
	m_BldCombox.AddString(strBld[1]);
	int nCodeType = m_CodeCombox.GetCurSel();
	if (nCodeType == 1)
	{
		m_BldCombox.AddString(strBld[2]);
		GetDlgItem(IDC_CMD_LOAD_DAMPING_GB_COEF_FREQ_STATIC)->SetWindowText(_LS(IDS_CMD0417__Frequent_E_Q_));
	}
	else
	{
		GetDlgItem(IDC_CMD_LOAD_DAMPING_GB_COEF_FREQ_STATIC)->SetWindowText(_LS(IDS_CMD0417__E_Q_));
	}
	m_BldCombox.SetCurSel(1);
	UpdateCaseNewList();
}

/************************************************************************/
/*					Update DB11/2075-2022 UI Data						*/
/************************************************************************/

void CCmdDampingGBCoefCalcDlg::InitCacheList()
{
	T_SPLC_D SplcD;
	T_THIS_D ThisD;
	CArray<T_SPLC_K, T_SPLC_K> SplcKeys;
	CArray<T_THIS_K, T_THIS_K> aThisKey;
	//1.Init DB Code;if DB NOExist AutomaticGetSpfcListType
	if(!m_pDoc->m_pAttrCtrl->GetDamping(m_Data) || m_Data.nBldCategory == -1)
	{
		m_Data.Initialize();
		m_pDoc->m_pUnitCtrl->ConvertUnitDampingOut(m_Data);
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
		m_aCacheList[nCodeType][0][0].Copy(m_Data.aNonUseSplcCase);
		m_aCacheList[nCodeType][0][1].Copy(m_Data.aResistSplcCase);
		m_aCacheList[nCodeType][0][2].Copy(m_Data.aRareESplcCase);

		m_aCacheList[nCodeType][1][0].Copy(m_Data.aNonUseThisCase);
		m_aCacheList[nCodeType][1][1].Copy(m_Data.aResistThisCase);
		m_aCacheList[nCodeType][1][2].Copy(m_Data.aRareEThisCase);
	}
	if(m_Data.nBldCategory == -1)
	{
		m_Data.nBldCategory = 1;
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
			m_aCacheList[1 - nCodeType][0][nListType].Add(SplcKeys[i]);
		}
	}
	m_pDoc->m_pAttrCtrl->GetThisKeyList(aThisKey, FALSE);
	for(int i = 0; i < aThisKey.GetCount(); i++)
	{
		m_aCacheList[1 - nCodeType][1][0].Add(aThisKey[i]);
	}
}

int CCmdDampingGBCoefCalcDlg::AutomaticGetSpfcListType(UINT Key, int nCodeType)
{
	T_SPFC_D SpfcD;
	if(!m_pDoc->m_pAttrCtrl->GetSpfc(Key, SpfcD))
		return 0;
	int nListType = 0;
	if(nCodeType == 0)
	{
		if(SpfcD.nSpecCodeType == D_SPFC_CODE_CH2019)
		{
			if(SpfcD.CodeParam.CH2019.nEarthResponse == 0)
				nListType = 0;
			else if(SpfcD.CodeParam.CH2019.nEarthResponse == 1)
				nListType = 1;
			else if(SpfcD.CodeParam.CH2019.nEarthResponse == 2)
				nListType = 2;
			else
			{
				nListType = 0;
				//ASSERT(FALSE);
				//nListType = -1;
			}
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_CH2010)
		{
			if(SpfcD.CodeParam.CH2010.nEarthResponse == 0)
				nListType = 0;
			else if(SpfcD.CodeParam.CH2010.nEarthResponse == 1)
				nListType = 1;
			else if(SpfcD.CodeParam.CH2010.nEarthResponse == 2)
				nListType = 2;
			else
			{
				nListType = 0;
				//ASSERT(FALSE);
				//nListType = -1;
			}
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_CH_GB_T_51408_2021)
		{
			if(SpfcD.CodeParam.GB_T_51408_2021.nEarthResponse == 0)
				nListType = 1;
			else if(SpfcD.CodeParam.GB_T_51408_2021.nEarthResponse == 1)
				nListType = 2;
			else if(SpfcD.CodeParam.GB_T_51408_2021.nEarthResponse == 2)
				nListType = -1;
			else
			{
				nListType = 0;
				//ASSERT(FALSE);
				//nListType = -1;
			}
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_CH2002)
		{
			if(SpfcD.CodeParam.CH2002.nEarthResponse == 0)
				nListType = 0;
			else if(SpfcD.CodeParam.CH2002.nEarthResponse == 1)
				nListType = 2;
			else
			{
				nListType = 0;
				//ASSERT(0);
				//nListType = -1;
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

void CCmdDampingGBCoefCalcDlg::AutomaticAddList(UINT Key, int nRsType, int nCodeType /* = 0 */)
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
	if(nListIndex < 0 || nListIndex > 2)
	{
		nListIndex = 0;
		ASSERT(FALSE);
	}
	pListCtrl = &m_CaseList[nListIndex];
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
	CDlgUtil::SetListItem(pListCtrl, m_ListData[nCodeType][nListIndex].GetCount(), Contents, Key);
	m_ListData[nCodeType][nListIndex].Add(CaseD);
}

void CCmdDampingGBCoefCalcDlg::UpdateCaseNewList()
{
	int nCodeType = m_CodeCombox.GetCurSel();
	for(int i = 0; i < 3; i++)
	{
		m_CaseList[i].DeleteAllItems();
		m_ListData[nCodeType][i].RemoveAll();
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

void CCmdDampingGBCoefCalcDlg::OnMoveList(int nType)
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
		pListCtrlSt = &m_CaseList[0];
		pStCacheData = &m_ListData[nCodeType][0];
		if(nType == 0)
		{
			pListCtrlEnd = &m_CaseList[1];
			pEndCacheData = &m_ListData[nCodeType][1];
		}
		else
		{
			pListCtrlEnd = &m_CaseList[2];
			pEndCacheData = &m_ListData[nCodeType][2];
		}
	}
	else
	{
		pListCtrlEnd = &m_CaseList[0];
		pEndCacheData = &m_ListData[nCodeType][0];
		if(nType == 1)
		{
			pListCtrlSt = &m_CaseList[1];
			pStCacheData = &m_ListData[nCodeType][1];
		}
		else
		{
			pListCtrlSt = &m_CaseList[2];
			pStCacheData = &m_ListData[nCodeType][2];
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
