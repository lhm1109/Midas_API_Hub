// SoilPropAddModDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "SoilPropAddModDlg.h"
#include "SoilParamAddLoadGrid.h"
#include "SoilPropCalcKhDlg.h"
#include "ImportFFA.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoilPropAddModDlg dialog

CSoilPropAddModDlg::CSoilPropAddModDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSoilPropAddModDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pGridWnd = new CSoilParamAddLoadGrid();
	m_Key = m_OldKey = 0;
	m_bModify = FALSE;

	m_aCtrl_ShowOnKDS2018.Add(IDC_CMD_EP_SOIL_SH_STC);
	m_aCtrl_ShowOnKDS2018.Add(IDC_CMD_EP_SOIL_SH_EDIT);
	m_aCtrl_ShowOnKDS2018.Add(IDC_CMD_EP_SOIL_ST_WIDTH_UNIT);
	m_aCtrl_ShowOnKDS2018.Add(IDC_CMD_EP_SOIL_SW_STC);
	m_aCtrl_ShowOnKDS2018.Add(IDC_CMD_EP_SOIL_SW_EDIT);
	m_aCtrl_ShowOnKDS2018.Add(IDC_CMD_EP_SOIL_ST_HEIGHT_UNIT);

	m_aCtrl_HideOnKDS2018.Add(IDC_CMD_EP_SOIL_USE_N_CHK);
//	m_aCtrl_HideOnKDS2018.Add(IDC_CMD_IMPORT_BTN);
//	m_aCtrl_HideOnKDS2018.Add(IDC_CMD_FFA_BTN);

#if defined(_CIVIL)
	m_bCivil = TRUE;
#else
	m_bCivil = FALSE;
#endif
}

CSoilPropAddModDlg::~CSoilPropAddModDlg()
{
}

void CSoilPropAddModDlg::InitGrid()
{
	CRect rect;
	GetDlgItem(IDC_CMD_EP_SOIL_PARAM_GRID)->GetWindowRect(&rect);
	m_pGridWnd->Initialize(this, rect.Width());
}

BOOL CSoilPropAddModDlg::Dlg2Data()
{
	UpdateData();
	CString strName, strDesc;
	m_editName.GetWindowText(strName);
	m_editDesc.GetWindowText(strDesc);
	strName.TrimLeft();
	strName.TrimRight();
	strDesc.TrimLeft();
	strDesc.TrimRight();

	CString strMsg, strCondition;
	if (m_pDoc->m_pAttrCtrl->GetPospKeyByName(strName) && !m_bModify)
	{
		AfxMessageBox(_LS(IDS_CMD_SAME_NAME_EXIST));
		return FALSE;
	}

	if (strName.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_CMD_SOIL_PROP_NAME_EMPTY_ERROR));
		return FALSE;
	}
	if (strName.GetLength() > D_POSP_STR_MAX_LENGTH)
	{
		strMsg.Format(_LS(IDS_CMD_SOIL_PROP_LENGTH_ERROR), _LS(IDS_DB_DT_POSP_NAME));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (strDesc.GetLength() > D_POSP_STR_MAX_LENGTH)
	{
		strMsg.Format(_LS(IDS_CMD_SOIL_PROP_LENGTH_ERROR), _LS(IDS_DB_DT_POSP_DESC));
		AfxMessageBox(strMsg);
		return FALSE;
	}

	double dGroundLevel = m_editGroundLevel.GetEditValue();
	double dBedrockLevel = m_editBedrockLevel.GetEditValue();
	double dFootingLevel = m_editFootingLevel.GetEditValue();
	double dStructureWidth = m_editStWidth.GetEditValue();
	double dStructureHeight = m_editStHeight.GetEditValue();

	if (dGroundLevel <= dBedrockLevel)
	{
		strMsg.Format(_LS(IDS_CMD_SOIL_PROP_GROUND_LEVEL_INVALID), _LS(IDS_CMD_SOIL_PROP_BEDROCK_LEVEL));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (dGroundLevel <= dFootingLevel)
	{
		strMsg.Format(_LS(IDS_CMD_SOIL_PROP_GROUND_LEVEL_INVALID), _LS(IDS_CMD_SOIL_PROP_FOOTING_LEVEL));
		AfxMessageBox(strMsg);
		return FALSE;
	}

	m_pGridWnd->GetData(m_Data.aSoilParam);

	double dSoilBottomLevel = dGroundLevel;
	T_POSP_BASE_DATA Base;
	for (int i = 0; i < m_Data.aSoilParam.GetSize(); i++)
	{
		Base = m_Data.aSoilParam[i];
		dSoilBottomLevel -= Base.dHeight;
		if (Base.dHeight <= 0.0)
		{
			strCondition.Format(_LS(IDS_CMD_ERR_SOIL_PORP_GREATER_THAN_0_CONDTION), _LS(IDS_CMD_LOAD_SOIL_PROP_TB_HEIGHT));
			strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), strCondition);
			AfxMessageBox(strMsg);
			return FALSE;
		}
		
		if (m_bUseN)
		{
			int nVal = static_cast<int>(Base.dAngleOrN); //N은 자연수임
			if (nVal < 1 || nVal > 50)
			{
				strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), _LS(IDS_CMD_ERR_SOIL_PORP_N_CONDTION));
				AfxMessageBox(strMsg);
				return FALSE;
			}
		}
		else
		{
			if (Base.dAngleOrN < 0.0 || Base.dAngleOrN > 60.0)
			{
				strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), _LS(IDS_CMD_ERR_SOIL_PORP_ANGLE_CONDTION));
				AfxMessageBox(strMsg);
				return FALSE;
			}
		}
		
		if (Base.dDensity <= 0.0)
		{
			strCondition.Format(_LS(IDS_CMD_ERR_SOIL_PORP_GREATER_THAN_0_CONDTION), _LS(IDS_CMD_LOAD_SOIL_PROP_TB_DENSITY));
			strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), strCondition);
			AfxMessageBox(strMsg);
			return FALSE;
		}

		if (Base.dPoissonRatio < 0)
		{
		}

		double dVs = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_VELOCITY, Base.dVs);
		if (dVs <= 0.0)
		{
			strCondition.Format(_LS(IDS_CMD_ERR_SOIL_PORP_GREATER_THAN_0_CONDTION), _LS(IDS_CMD_LOAD_SOIL_PROP_TB_VS));
			strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), strCondition);
			AfxMessageBox(strMsg);
			return FALSE;
		}

		if (m_bCivil) // 현재는 KDS_E_2018 만 고려되므로.
		{
			dVs = CMathFunc::mathRoundOff(dVs, 7);
			if (dVs >= 760.0) // 760 m/s
			{
				AfxMessageBox(_LS(IDS_DB_DT_POSP_VS_760));
				return FALSE;
			}
		}

		if (Base.dKh <= 0.0)
		{
			strCondition.Format(_LS(IDS_CMD_ERR_SOIL_PORP_GREATER_THAN_0_CONDTION), _LS(IDS_CMD_LOAD_SOIL_PROP_TB_KH));
			strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), strCondition);
			AfxMessageBox(strMsg);
			return FALSE;
		}

		if (Base.dPoissonRatio < 0 || Base.dPoissonRatio >1)
		{
			strCondition.Format(_LS(IDS_CMD_ERR_SOIL_PORP_GREATER_THAN_0_CONDTION), _LS(IDS_CMD_LOAD_SOIL_PROP_TB_POISSON));
			strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), strCondition);
			AfxMessageBox(strMsg);
			return FALSE;
		}
	}

	if (dSoilBottomLevel > dBedrockLevel)
	{
		AfxMessageBox(_LS(IDS_CMD_SOIL_PROP_ERR_SOIL_PARAM_BOTTOM_LEVEL));
		return FALSE;
	}

	m_Data.strName = strName;
	m_Data.strDescription = strDesc;
	m_Data.bUseN = m_bUseN;
	m_Data.dGroundLevel = dGroundLevel;
	m_Data.dBedrockLevel = dBedrockLevel;
	m_Data.dFootingLevel = dFootingLevel;
	m_Data.dStructureWidth = dStructureWidth;
	m_Data.dStructureHeight = dStructureHeight;


	return TRUE;
}

BOOL CSoilPropAddModDlg::Data2Dlg()
{
	m_editName.SetWindowText(m_Data.strName);
	m_editDesc.SetWindowText(m_Data.strDescription);

	m_bUseN = m_Data.bUseN;
	m_pGridWnd->SetUseN(m_bUseN, FALSE);
	m_pGridWnd->SetGroundLevel(m_Data.dGroundLevel, FALSE);
	m_pGridWnd->SetData(m_Data.aSoilParam, FALSE);

	CString strTemp;
	strTemp.Format(_T("%g"), m_Data.dGroundLevel);
	m_editGroundLevel.SetWindowText(strTemp);
	strTemp.Format(_T("%g"), m_Data.dBedrockLevel);
	m_editBedrockLevel.SetWindowText(strTemp);
	strTemp.Format(_T("%g"), m_Data.dFootingLevel);
	m_editFootingLevel.SetWindowText(strTemp);
	strTemp.Format(_T("%g"), m_Data.dStructureWidth);
	m_editStWidth.SetWindowText(strTemp);
	strTemp.Format(_T("%g"), m_Data.dStructureHeight);
	m_editStHeight.SetWindowText(strTemp);

	UpdateData(FALSE);
	return TRUE;
}

void CSoilPropAddModDlg::MakeNewData()
{
	m_Key = m_pDoc->m_pAttrCtrl->GetStartNumPosp();
	
	m_Data.Initialize();
	m_Data.aSoilParam.Add(T_POSP_BASE_DATA());

	m_pDoc->m_pUnitCtrl->ConvertUnitPospOut(m_Data);

	m_bUseN = FALSE;
	T_BLDC_D Bldc;
	if (m_pDoc->m_pAttrCtrl->GetBldc(Bldc))
	{
		if(Bldc.bUseBaseLevel)
			m_Data.dGroundLevel = Bldc.dBaseLevel;

		if (Bldc.bUseBedrockLevel)
			m_Data.dBedrockLevel = Bldc.dBedrockLevel;
	}
	
	m_Data.dFootingLevel = m_pDoc->m_pAttrCtrl->GetBottomofFootingLevel();

	//default 값이 아니면
	if(m_Data.dGroundLevel != 0 || m_Data.dFootingLevel != 0)
		m_Data.aSoilParam[0].dHeight = m_Data.dGroundLevel - m_Data.dFootingLevel;

	UpdateData(FALSE);
}

void CSoilPropAddModDlg::InitHeightCtrls()
{
	double dHeight = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 1);

	CString strTemp;
	strTemp.Format(_T("%g"), dHeight);
	m_editHeight.SetWindowText(strTemp);
	m_editCopies.SetValue(1);
}

BOOL CSoilPropAddModDlg::GetHeightData(double &dHeight, int &nRowCnt)
{
	dHeight = m_editHeight.GetEditValue();
	CString strMsg, strCondition;
	if (dHeight <= 0.0)
	{
		strCondition = _LS(IDS_CMD_SOIL_PROP_HEIGHT) + _T(" < 0");
		strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), strCondition);
		AfxMessageBox(strMsg);
		return FALSE;
	}
	
	nRowCnt = m_editCopies.GetEditValueInt();
	if (nRowCnt <= 0.0)
	{
		strCondition = _LS(IDS_CMD_SOIL_PROP_COPIES) + _T(" < 0");
		strMsg.Format(_LS(IDS_CMD_ERR_SOIL_PROP_GRID_INVALID), strCondition);
		AfxMessageBox(strMsg);
		return FALSE;
	}

	return TRUE;
}

void CSoilPropAddModDlg::SetParamData(T_POSP_K Key, T_POSP_D &ParamData)
{
	m_Key = m_OldKey = Key;
	m_Data = ParamData;
	m_bModify = TRUE;
}

void CSoilPropAddModDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_NAME_EDIT, m_editName);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_DESC_EDIT, m_editDesc);
	DDX_Check(pDX, IDC_CMD_EP_SOIL_USE_N_CHK, m_bUseN);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_GL_EDIT, m_editGroundLevel);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_GL_UNIT, m_wndGLevelUnit);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_BEDROCK_EDIT, m_editBedrockLevel);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_BEDROCK_UNIT, m_wndBedLevelUnit);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_FOOTING_LEVEL_EDIT, m_editFootingLevel);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_FOOTING_LEVEL_UNIT, m_wndFootingLevelUnit);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_HEIGHT_EDIT, m_editHeight);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_HEIGHT_UNIT, m_wndHeightUnit);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_ST_WIDTH_UNIT, m_wndStWdithUnit);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_ST_HEIGHT_UNIT, m_wndStHeightUnit);
	DDX_Control(pDX, IDC_CMD_COPIES_EDT, m_editCopies);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_SH_EDIT, m_editStWidth);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_SW_EDIT, m_editStHeight);
	DDX_Control(pDX, IDC_CMD_EP_SOIL_PARAM_GRID, *m_pGridWnd);
}


BEGIN_MESSAGE_MAP(CSoilPropAddModDlg, CDialogMove)
	ON_EN_CHANGE(IDC_CMD_EP_SOIL_GL_EDIT, OnChangeGL)
	ON_BN_CLICKED(IDC_CMD_EP_SOIL_USE_N_CHK, OnCmdUseNChk)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DEL, OnCmdDel)
	ON_BN_CLICKED(IDC_CMD_BTN_INSERT, OnCmdInsert)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_COPIES_SPIN, OnDeltaposCmdCopies)
	ON_BN_CLICKED(IDC_CMD_BTN_CAL, OnCmdCalc)
	ON_BN_CLICKED(IDC_CMD_FFA_BTN, OnCmdFFA)
	ON_BN_CLICKED(IDC_CMD_IMPORT_BTN, OnCmdFTFImport)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoilPropAddModDlg message handlers

BOOL CSoilPropAddModDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	m_editGroundLevel.SetUnitType(D_UNITSYS_BASE_LENGTH);	m_wndGLevelUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editBedrockLevel.SetUnitType(D_UNITSYS_BASE_LENGTH);	m_wndBedLevelUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editFootingLevel.SetUnitType(D_UNITSYS_BASE_LENGTH);	m_wndFootingLevelUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);		m_wndHeightUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_editCopies.SetRange(1, 20);

	if (m_bCivil) AlignControls();
	InitGrid();
	GetDlgItem(IDC_CMD_BTN_CAL)->EnableWindow(CDBDoc::IsEnableEarthPressureSeis());
	if (!m_bModify)
		MakeNewData();
	Data2Dlg();
	InitHeightCtrls();

#if !defined(_MGEN_ORG) && !defined(_CIVIL_ORG)    // Gen / Civil 한국버전이 아니면 보이지 않도록함.
	GetDlgItem(IDC_CMD_IMPORT_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_FFA_BTN)->ShowWindow(SW_HIDE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CSoilPropAddModDlg::OnOK()
{
	if (!Dlg2Data()) return;
	BOOL bSuccess;
	if (m_bModify)
		bSuccess = m_pDoc->m_pDataCtrl->ModifyPosp(m_OldKey, m_Key, m_Data);
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddPosp(m_Data);
	if (!bSuccess) return;

	CDialogMove::OnOK();
}

void CSoilPropAddModDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CSoilPropAddModDlg::OnCmdApply()
{
	if (!Dlg2Data()) return;
	BOOL bSuccess;
	if (m_bModify)
		bSuccess = m_pDoc->m_pDataCtrl->ModifyPosp(m_OldKey, m_Key, m_Data);
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddPosp(m_Data);
	if (!bSuccess) return;
}

void CSoilPropAddModDlg::OnChangeGL()
{
	m_pGridWnd->SetGroundLevel(m_editGroundLevel.GetEditValue());
}

void CSoilPropAddModDlg::OnCmdUseNChk()
{
	UpdateData();
	m_pGridWnd->SetUseN(m_bUseN);
}

// void CSoilPropAddModDlg::OnDeltaposCmdCopies(NMHDR* pNMHDR, LRESULT* pResult)
// {
// 	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
// 	CDlgUtil::SetSpinValue(this, IDC_CMD_COPIES_EDT, 1, 20, pNMHDR, pResult);
// 
// 	*pResult = 0;
// }

void CSoilPropAddModDlg::OnCmdAdd()
{
	double dHeight;
	int nRowCnt;
	if (!GetHeightData(dHeight, nRowCnt))
		return;
	
	m_pGridWnd->AddRow(dHeight, nRowCnt);
}

void CSoilPropAddModDlg::OnCmdDel()
{
	m_pGridWnd->DelRow();
}

void CSoilPropAddModDlg::OnCmdInsert()
{
	double dHeight;
	int nRowCnt;
	if (!GetHeightData(dHeight, nRowCnt))
		return;
	m_pGridWnd->AddRow(dHeight, nRowCnt, TRUE);
}

void CSoilPropAddModDlg::OnCmdCalc()
{
	m_pGridWnd->GetData(m_Data.aSoilParam);

	if (!Dlg2Data()) return;

	CSoilPropCalcKhDlg dlg;
	dlg.SetData(m_Data);
	if (dlg.DoModal() == IDOK)
	{
		m_Data = dlg.GetData();
		m_pGridWnd->SetData(m_Data.aSoilParam);
		m_pGridWnd->MakeItemEx();
	}
}

void CSoilPropAddModDlg::OnCmdFFA()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_TOOLS_FREE_FIELD_ANALYSIS, 0));
}
void CSoilPropAddModDlg::OnCmdFTFImport()
{
	CString csFileName;

	CFileDialog fd(TRUE, _T(".STF"), NULL, OFN_FILEMUSTEXIST | OFN_LONGNAMES,
		_T("SRA Analysis Data Files(*.STF)|*.STF|All File(*.*)|*.*||"), NULL);
	if (fd.DoModal() == IDOK) csFileName = fd.GetPathName();
	else return;

	CImportFFA FtfImporter;
	FFAMsg nMSG = FtfImporter.ImportFFA(csFileName);
	if (nMSG != FFAMsg::EN_DATA_VALID)
	{
		auto L_GetMsg = [](const FFAMsg nMsg) -> CString
		{
			switch (nMsg)
			{
			case FFAMsg::EN_READ_FAIL:
				return _LS(IDS_CMD_FFA_MSG_READ_FAIL);
			case FFAMsg::EN_FILE_EMPTY:
				return _LS(IDS_CMD_FFA_MSG_FILE_IS_EMPTY);
			case FFAMsg::EN_UNIT_LENGTH:
				return _LS(IDS_CMD_FFA_MSG_UNIT_IS_INVALID_LENGTH);
			case FFAMsg::EN_UNIT_FORCE:
				return _LS(IDS_CMD_FFA_MSG_UNIT_IS_INVALID_FORCE);
			case FFAMsg::EN_DATA_EMPTY:
				return _LS(IDS_CMD_FFA_MSG_DATA_NOT_EXIST);
			}
			ASSERT(0); return _LSX(Unknown Error);
		};

		CString csMsg = L_GetMsg(nMSG);
		AfxMessageBox(csMsg, MB_OK | MB_ICONERROR);
		return;
	}


	FtfImporter.GetSoilProp(m_Data.aSoilParam);
		
	m_pGridWnd->SetData(m_Data.aSoilParam);
	m_pGridWnd->MakeItemEx();

}


void CSoilPropAddModDlg::AlignControls()
{
	CDlgUtil::CtrlShowHide(this, m_aCtrl_ShowOnKDS2018, m_bCivil);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_HideOnKDS2018, !m_bCivil);

	CRect rRef, rRefY, rToMove, rToMoveY;
	CArray<UINT, UINT> aCtrls_Remark;
	aCtrls_Remark.Add(IDC_CMD_EP_SOIL_SH_STC);
	aCtrls_Remark.Add(IDC_CMD_EP_SOIL_SH_EDIT);
	aCtrls_Remark.Add(IDC_CMD_EP_SOIL_ST_WIDTH_UNIT);
	aCtrls_Remark.Add(IDC_CMD_EP_SOIL_SW_STC);
	aCtrls_Remark.Add(IDC_CMD_EP_SOIL_SW_EDIT);
	aCtrls_Remark.Add(IDC_CMD_EP_SOIL_ST_HEIGHT_UNIT);


	GetDlgItem(IDC_CMD_EP_SOIL_USE_N_CHK)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_IMPORT_BTN       )->GetWindowRect(rRefY);
	GetDlgItem(IDC_CMD_EP_SOIL_SH_STC)->GetWindowRect(rToMove);
	GetDlgItem(IDC_CMD_EP_SOIL_SH_EDIT)->GetWindowRect(rToMoveY);

	int nDistX = rRef.left - rToMove.left;
	int nDistY = rRefY.top - rToMoveY.top;
	m_pDlgUtil->CtrlMoveDistXY(this, aCtrls_Remark, nDistX, nDistY);
	
}