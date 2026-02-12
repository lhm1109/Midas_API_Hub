// CMWindPressureVelocityDefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMWindPressureVelocityAddMod.h"
#include "CMWindPressureVelocityCodeMgr.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\LateralLoad.h"

#include "..\wg_base\wg_base_MsgDll.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelocityAddMod dialog
#define CDialog CChildDialogMove


CCMWindPressureVelocityAddMod::CCMWindPressureVelocityAddMod(CWnd* pParent /*=NULL*/)
	: CDialog(CCMWindPressureVelocityAddMod::IDD, pParent)
{
	m_Key = 0;

	m_pMgr = NULL;
	m_pMgr = new CCMWindPressureVelocityCodeMgr(this);

	m_Data.Initialize();
	CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitWvepOut(m_Data);

	m_aWlCodeEnable.RemoveAll();
	m_nOldSel = 0;

	m_dVortexDM = 0.;
	m_dVortexDB = 0.;
	m_dTotalMass = 0.0;
}

CCMWindPressureVelocityAddMod::~CCMWindPressureVelocityAddMod()
{
	if (m_pMgr != NULL)
	{
		delete m_pMgr;
		m_pMgr = NULL;
	}
}



void CCMWindPressureVelocityAddMod::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_PLACE_HOLDER, m_wndHolder);
	DDX_Control(pDX, IDC_CMD_CODE_NAME, m_cobxCodeName);
	DDX_Control(pDX, IDC_CMD_DESCRIPTION, m_edtName);

}


BEGIN_MESSAGE_MAP(CCMWindPressureVelocityAddMod, CDialog)
	ON_CBN_SELCHANGE(IDC_CMD_CODE_NAME, OnSelchangeCmdCodeName)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelocityAddMod message handlers
BOOL CCMWindPressureVelocityAddMod::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndHolder.ShowWindow(SW_HIDE);
	if (m_Key == 0)
	{
#ifdef _US
		m_Data.nCodeType = CCMWindPressureBaseMgr::GetDBCodeIdx(EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022);	
#elif defined(_CH)
		m_Data.nCodeType = CCMWindPressureBaseMgr::GetDBCodeIdx(EN_WIND_PRESSURE_CODE_TYPE_CHINA2019);
#else
		m_Data.nCodeType = CCMWindPressureBaseMgr::GetDBCodeIdx(EN_WIND_PRESSURE_CODE_TYPE_KDS2022);
#endif
		m_Data.CodeParam.Initialize(m_Data.nCodeType);
	}
	InitCodeNameCobx();
	InitRoofHeight();
	InitDmDB4KDS2021();
	InitMassData();
	// 	InitCodeNameCobx();
	if (m_Key == 0) GetInitDefaultValue(m_Data.nCodeType);
	Data2Dlg();

	m_edtName.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindPressureVelocityAddMod::OnSelchangeCmdCodeName()
{
	// TODO: Add your control notification handler code here
	int nSel = m_cobxCodeName.GetCurSel();
	int nCodeIndex = CDlgUtil::CobxGetCurSelItemData(m_cobxCodeName, nSel);
#if defined(_CH) && defined(_MGEN)
	if (m_aWlCodeEnable[nSel] == FALSE)
	{
		CString strCode = _T("");
		m_cobxCodeName.GetWindowText(strCode);
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), strCode);
		GSaveHistoryFormatNF(strErrMsg);
		m_cobxCodeName.SetCurSel(m_nOldSel);
		nCodeIndex = CDlgUtil::CobxGetCurSelItemData(m_cobxCodeName, m_nOldSel);
		return;
	}
	m_nOldSel = nSel;
#endif
	if (nCodeIndex >= 0 && nCodeIndex != m_pMgr->GetCurCodeIndex())
	{
		EN_WIND_PRESSURE_CODE_TYPE enType = (EN_WIND_PRESSURE_CODE_TYPE)m_cobxCodeName.GetItemData(m_cobxCodeName.GetCurSel());
		m_Data.nCodeType = CCMWindPressureBaseMgr::GetDBCodeIdx(enType);
		GetInitDefaultValue(m_Data.nCodeType);
		m_pMgr->ChangeCodeDlg(nCodeIndex);
	}
}

void CCMWindPressureVelocityAddMod::InitCodeNameCobx()
{
	m_cobxCodeName.ResetContent();

	CStringArray aCodeName;
	CArray<int, int> aCodeIndex;
	int nNum = m_pMgr->GetCodeNameList(aCodeName, aCodeIndex, &m_aWlCodeEnable);

	for (int i = 0; i < nNum; i++)
	{
		CDlgUtil::CobxAddItem(m_cobxCodeName, aCodeName[i], aCodeIndex[i]);
	}

	if (nNum > 0)
	{
#ifdef _MGEN_CH
		int nCodeIndex = m_pMgr->GetCodeIdxbyDBCode(m_Data.nCodeType);
		//This should be universal by xuezc;
		//If ch2012 is used under the ch version, the interface will be incorrect during initialization.
		bool bMismatch = true;
		int nCoboxIndex = 0;
		for (int i = 0; i < aCodeIndex.GetSize(); i++)
		{
			if (aCodeIndex[i] == nCodeIndex)
			{
				bMismatch = false;
				nCoboxIndex = i;
				break;
			}
		}
		if (bMismatch)
		{
			nCodeIndex = aCodeIndex[0];
			nCoboxIndex = 0;
			m_Data.nCodeType = m_pMgr->GetDBCodebyCodeIdx(EN_WIND_PRESSURE_CODE_TYPE(nCodeIndex));
		}
		m_cobxCodeName.SetCurSel(nCoboxIndex);
		m_pMgr->ChangeCodeDlg(nCodeIndex);
		m_nOldSel = 0;
#else
		int nCodeIndex = m_pMgr->GetCodeIdxbyDBCode(m_Data.nCodeType);
		CDlgUtil::CobxSetCurSelItemData(m_cobxCodeName, nCodeIndex);
		//m_cobxCodeName.SetCurSel(nCodeIndex);
		m_pMgr->ChangeCodeDlg(nCodeIndex);
		m_nOldSel = 0;
#endif
	}
}

BOOL CCMWindPressureVelocityAddMod::GetHolderRect(CRect* pRect)
{
	if (m_wndHolder.GetSafeHwnd() && ::IsWindow(m_wndHolder.GetSafeHwnd()))
	{
		m_wndHolder.GetWindowRect(pRect);
		return TRUE;
	}
	return FALSE;
}

void CCMWindPressureVelocityAddMod::GetMoveCtrlHolderID(CArray<UINT, UINT>& caCtrls, UINT& uiHolderID)
{
	caCtrls.RemoveAll();
	caCtrls.Add(IDOK);
	caCtrls.Add(IDCANCEL);
	uiHolderID = IDC_CMD_PLACE_HOLDER;
}

BOOL CCMWindPressureVelocityAddMod::Data2Dlg()
{
	m_edtName.SetWindowText(m_Data.StrName);
	if (m_Key != 0)
	{
		EN_WIND_PRESSURE_CODE_TYPE enType = CCMWindPressureBaseMgr::GetCodeIdxbyDBCode(m_Data.nCodeType);
		CDlgUtil::CobxSetCurSelItemData(m_cobxCodeName, enType);
	}
	// 	OnSelchangeCmdCodeName();
	m_pMgr->Data2CodeDlg(m_Data, EN_WIND_PRESSURE_CODE_TYPE_ALL);
	return TRUE;
}

BOOL CCMWindPressureVelocityAddMod::Dlg2Data()
{
	CString strTemp;
	m_edtName.GetWindowText(strTemp);
	strTemp.TrimLeft();
	strTemp.TrimRight();
	if (strTemp.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_CMD_WINDP_VELOCITY_NONAME));
		return FALSE;
	}
	if (m_Key == 0)
	{
		if (CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetWvepKeyByName(strTemp) > 0) //새로 등록하는 경우에 뭔가 이름 같은 녀석이 있다면.
		{
			AfxMessageBox(_LS(IDS_CMD_WINDP_VELOCITY_SAMENAME));
			return FALSE;
		}
	}
	m_Data.StrName = strTemp;

	EN_WIND_PRESSURE_CODE_TYPE enType = (EN_WIND_PRESSURE_CODE_TYPE)m_cobxCodeName.GetItemData(m_cobxCodeName.GetCurSel());
	m_Data.nCodeType = CCMWindPressureBaseMgr::GetDBCodeIdx(enType);

	return m_pMgr->CodeDlg2Data(m_Data, EN_WIND_PRESSURE_CODE_TYPE_ALL);
}

void CCMWindPressureVelocityAddMod::OnOK()
{
	if (Dlg2Data())
	{
		BOOL bRetVal;
		if (m_Key == 0)
			bRetVal = CDBDoc::GetDocPoint()->m_pDataCtrl->AddWvep(m_Data);
		else
			bRetVal = CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyWvep(m_Key, m_Key, m_Data);

		if (bRetVal)
			CChildDialogMove::OnOK();
	}
}

void CCMWindPressureVelocityAddMod::SetWvepKey(T_WVEP_K Key)
{
	BOOL bRetVal;
	m_Key = Key;
	if (m_Key > 0)
	{
		bRetVal = CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetWvep(Key, m_Data);
	}
}

void CCMWindPressureVelocityAddMod::InitMassData()
{
	double dMassX = 0.0;
	double dMassY = 0.0;

	CArray<T_STOR_K, T_STOR_K> aStorK;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	int nNum = aStorK.GetSize();
	if (nNum == 0) return;

	T_BLDC_D BldcD;
	if (!pDoc->m_pAttrCtrl->GetBldc(BldcD)) BldcD.Initialize();

	double(*dDIAPMS)[3] = new double[nNum][3];
	double(*dDIAPXY)[2] = new double[nNum][2];
	double(*dRLSMSS)[2] = new double[nNum][2];

	CLateralLoad StorMass;
	BOOL bUseBedrockLevel = FALSE;
	StorMass.Make_DiaphramMassTableData(dDIAPMS, dDIAPXY, dRLSMSS, bUseBedrockLevel);

	T_STOR_D StorD;
	for (int i = 0; i < aStorK.GetSize(); i++)
	{
		if (!pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD)) { ASSERT(0); continue; }
		if (StorD.dStoryLevel < BldcD.dBaseLevel - (1.e-10))
			continue;

		dMassX += dDIAPMS[i][0] + dRLSMSS[i][0];
		dMassY += dDIAPMS[i][1] + dRLSMSS[i][1];
	}
	m_dTotalMass = max(dMassX, dMassY);
	delete[]dDIAPMS;
	delete[]dDIAPXY;
	delete[]dRLSMSS;
}
void CCMWindPressureVelocityAddMod::InitDmDB4KDS2021()
{

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	// 층개수와 Base Level 구하기 
	double dBaseLevel = pDoc->m_pAttrCtrl->GetGroundLevel();
	T_STOR_D StorD, StorD2;
	CArray<T_STOR_K, T_STOR_K> aStorK;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	int nStorNum = aStorK.GetSize();
	if (nStorNum > 0)	pDoc->m_pAttrCtrl->GetStor(aStorK[nStorNum - 1], StorD);
	else StorD.Initialize();

	double dDmLevel = 2 * StorD.dStoryLevel / 3.;
	double dMaxUp4Dm = 0.0;	//Dm => Diameter of 2H/3 Level
	double dMaxDn4Dm = 0.0;

	double dMaxUp4DB = 0.0;	//DB=> Diameter of Bottom
	double dMaxDn4DB = 0.0;
	if (nStorNum > 0)
	{
		BOOL bFirst = TRUE, bFirstDmLevel = TRUE;
		for (int i = 0; i < nStorNum; i++)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD2);

			// MQC 15282 : Min.Breadth 값을 가져올 때 Story Data에서 Ground Level 아래쪽 Level의 값을 제외하고 
			// 최소값을 가지고 올 수 있도록 수정부탁드립니다. 
			if (StorD2.dStoryLevel < dBaseLevel - (1.e-10))
			{
				dMaxDn4DB = max(dMaxDn4DB, max(StorD2.WindWidthX, StorD2.WindWidthY));
				continue;
			}

			if (StorD2.dStoryLevel < dDmLevel - (1.e-10))
			{
				dMaxDn4Dm = max(dMaxDn4Dm, max(StorD2.WindWidthX, StorD2.WindWidthY));
			}
			else
			{
				if (bFirstDmLevel)
				{
					dMaxUp4Dm = max(StorD2.WindWidthX, StorD2.WindWidthY);
					bFirstDmLevel = FALSE;
				}
			}
			if (bFirst)
			{
				bFirst = FALSE;

				dMaxUp4DB = max(StorD2.WindWidthX, StorD2.WindWidthY);
				continue;
			}
		}
		for (int i = 0; i < nStorNum; i++)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD2);
			if (StorD2.dStoryLevel > dBaseLevel) break;
		}

	}
	m_dVortexDM = max(dMaxDn4Dm, dMaxUp4Dm);
	m_dVortexDB = max(dMaxUp4DB, dMaxDn4DB);

}
void CCMWindPressureVelocityAddMod::InitRoofHeight()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_NODE_K NodeK;
	T_NODE_D NodeD;
	POSITION posNode = pDoc->m_pAttrCtrl->GetStartNode();
	double dMaxZ = 0.0;
	double dMinZ = 0.0;
	BOOL bInit = TRUE;
	while (posNode)
	{
		pDoc->m_pAttrCtrl->GetNextNode(posNode, NodeK, NodeD);
		if (bInit) { dMaxZ = NodeD.z; dMinZ = NodeD.z; bInit = FALSE; }
		else
		{
			if (dMaxZ < NodeD.z) dMaxZ = NodeD.z;
			if (dMinZ > NodeD.z) dMinZ = NodeD.z;
		}
	}

	T_BLDC_D BldcD;
	if (pDoc->m_pAttrCtrl->GetBldc(BldcD))
	{
		if (BldcD.bUseBaseLevel) dMinZ = BldcD.dBaseLevel;
	}

	m_dRoofHeight = dMaxZ - dMinZ;
}

void CCMWindPressureVelocityAddMod::GetInitDefaultValue(int nCodeType)
{
	if (nCodeType == KDS_W_2022)
	{
		m_Data.CodeParam.KDS2021.Initialize();
		m_Data.CodeParam.KDS2021.dRoofHeight = m_dRoofHeight;
		m_Data.CodeParam.KDS2021.dSimpleRoofHeight = m_dRoofHeight;
		m_Data.CodeParam.KDS2021.dVortexAverageRoof = m_dRoofHeight;
		m_Data.CodeParam.KDS2021.dVortexDM = m_dVortexDB;
		m_Data.CodeParam.KDS2021.dVortexDB = m_dVortexDB;
		m_Data.CodeParam.KDS2021.dVortexMass = m_dTotalMass;
	}
	else if (nCodeType == KDS_W_2019)
	{
		m_Data.CodeParam.KDS2019.Initialize();
		m_Data.CodeParam.KDS2019.dRoofHeight = m_dRoofHeight;
		m_Data.CodeParam.KDS2019.dSimpleRoofHeight = m_dRoofHeight;
	}
	else if (nCodeType == KBC_W_2016)
	{
		m_Data.CodeParam.KBC2016.Initialize();
		m_Data.CodeParam.KBC2016.dRoofHeight = m_dRoofHeight;
		m_Data.CodeParam.KBC2016.dSimpleRoofHeight = m_dRoofHeight;
	}
	else if (nCodeType == KBC_W_2009)
	{
		m_Data.CodeParam.KBC2009.Initialize();
		m_Data.CodeParam.KBC2009.dRoofHeight = m_dRoofHeight;
	}
	else if (nCodeType == CH_W_2012) m_Data.CodeParam.CH2012.Initialize();
	else if (nCodeType == CH_W_2002) m_Data.CodeParam.CH2001.Initialize();
	else if (nCodeType == CH_W_2021) m_Data.CodeParam.CH2019.Initialize();
	else if (nCodeType == ASCE7_W_2016)
	{
		m_Data.CodeParam.ASCE7_16.Initialize();
		m_Data.CodeParam.ASCE7_16.dMeanRoofHeight = m_dRoofHeight;
	}
	else if (nCodeType == ASCE7_W_2022)
	{
		m_Data.CodeParam.ASCE7_22.Initialize();
		m_Data.CodeParam.ASCE7_22.dMeanRoofHeight = m_dRoofHeight;
	}
	else if (nCodeType == NSCP_W_2024)
	{
		m_Data.CodeParam.NSCP2024.Initialize();
		m_Data.CodeParam.NSCP2024.dMeanRoofHeight = m_dRoofHeight;
	}

	else if (nCodeType == IS_W_875_2015)
	{
		m_Data.CodeParam.IS875_2015.Initialize();
	}

	else ASSERT(0);
}