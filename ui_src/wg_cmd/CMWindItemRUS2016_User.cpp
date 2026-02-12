#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemRUS2016_User.h"
#include "CMWindItemDlg.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#include "CMWindPulsationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMWindItemRUS2016_User::CCMWindItemRUS2016_User(CWnd* pParent /*=NULL*/)
: CChildDialog(CCMWindItemRUS2016_User::IDD, pParent), m_LateralLoad(0, TRUE)
{
	m_Key = 0;
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;	
	m_bModify = FALSE;
	m_bInit = TRUE;
	m_bSaveWnps = FALSE;
	m_Data.Initialize();
	m_Data.nCodeType = RUS_W_2016;
	m_Data.CodeParam.RUS2016.Initialize();
	m_WnpsD.Initialize();

	m_aCtrlExRadio.Add(IDC_CMD_XDIR_POS);
	m_aCtrlExRadio.Add(IDC_CMD_XDIR_NEG);
	m_aCtrlExRadio.Add(IDC_CMD_XDIR_NONE);

	m_aCtrlEyRadio.Add(IDC_CMD_YDIR_POS);
	m_aCtrlEyRadio.Add(IDC_CMD_YDIR_NEG);
	m_aCtrlEyRadio.Add(IDC_CMD_YDIR_NONE);
}

void CCMWindItemRUS2016_User::SetData2Dlg(T_WIND_D& data, BOOL bModify, T_KEY KeyWind)
{
	m_Key = KeyWind;
	m_bModify = bModify;
	m_Data.nEccen_X = data.nEccen_X;
	m_Data.nEccen_Y = data.nEccen_Y;

	m_Data.CodeParam.RUS2016 = data.CodeParam.RUS2016;
	
	if (bModify && data.arUser.GetCount() > 0)
	{
		m_Data.arUser.Copy(data.arUser);
	}
	else
	{
		m_LateralLoad.MakeWindLoad(data, FALSE, _T(""), TRUE);
		int nSize = m_LateralLoad.m_aWindLoad_X.GetSize();
		if (nSize == 0) return;
		if (m_LateralLoad.m_aWindLoad_X[nSize - 1].KeyStory == 0)
			--nSize;

		m_Data.arUser.SetSize(nSize);
		for (int i = 0; i < nSize; i++)
		{
			m_Data.arUser[i].Initialize();
			m_Data.arUser[i].KeyStor = m_LateralLoad.m_aWindLoad_X[i].KeyStory;
			m_Data.arUser[i].dElev = m_LateralLoad.m_aWindLoad_X[i].dLevel;
			m_Data.arUser[i].dPressX = 0.0;
			m_Data.arUser[i].dPressY = 0.0;
			m_Data.arUser[i].dLoadH = m_LateralLoad.m_aWindLoad_X[i].dHeight;
			m_Data.arUser[i].dLoadBX = m_LateralLoad.m_aWindLoad_X[i].dBreadth;
			m_Data.arUser[i].dLoadBY = m_LateralLoad.m_aWindLoad_Y[i].dBreadth;
		}
	}
	if (!m_bInit) Data2Dlg();
}

BOOL CCMWindItemRUS2016_User::GetWnpsData(T_WNPS_D& WnpsD)
{
	if (m_bSaveWnps)
	{
		WnpsD = m_WnpsD;
		return TRUE;
	}
	return FALSE;
}

BOOL CCMWindItemRUS2016_User::SetDlg2Data(T_WIND_D& data)
{
	BOOL bOK = Dlg2Data();
	data.arUser.RemoveAll();
	data.arUser.Copy(m_Data.arUser);
	data.nEccen_X = m_Data.nEccen_X;
	data.nEccen_Y = m_Data.nEccen_Y;
	data.CodeParam.RUS2016 = m_Data.CodeParam.RUS2016;
	return bOK;
}

void CCMWindItemRUS2016_User::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCMWindItemRUS2016_User, CChildDialog)
	ON_BN_CLICKED(IDC_CMD_WIND_PULSATION_BTN, OnCmdCreateWindPulsation)
	ON_BN_CLICKED(IDC_CMD_WIND_PULSATION_CHK, OnCmdChkWindPulsation)
END_MESSAGE_MAP()

BOOL CCMWindItemRUS2016_User::OnInitDialog()
{
	CChildDialog::OnInitDialog();
	SetControl();
	Data2Dlg();
	m_bInit = FALSE;
	m_bModify = FALSE;

	OnCmdChkWindPulsation();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindItemRUS2016_User::SetControl()
{
	m_Grid.SubclassDlgItem(IDC_CMD_GRID, this);
	CRect rect;
	GetDlgItem(IDC_CMD_GRID)->GetWindowRect(&rect);
	m_Grid.Initialize(rect.Width());
}

BOOL CCMWindItemRUS2016_User::DestroyWindow()
{
	Dlg2Data();
	return CChildDialog::DestroyWindow();
}

void CCMWindItemRUS2016_User::Data2Dlg()
{
	if(!m_bModify && m_Data.arUser.GetSize() == 0)
	{
		m_Data.Initialize();
		m_Data.nCodeType = RUS_W_2016;
		SetData2Dlg(m_Data, FALSE, m_Key);
	}
	m_Grid.SetData(m_Data.arUser);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlExRadio, m_Data.nEccen_X);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlEyRadio, m_Data.nEccen_Y);
}

BOOL CCMWindItemRUS2016_User::Dlg2Data()
{	
	m_Grid.GetData(m_Data.arUser);

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlExRadio, m_Data.nEccen_X);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlEyRadio, m_Data.nEccen_Y);

	return TRUE;
}
void CCMWindItemRUS2016_User::OnCmdChkWindPulsation()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_PULSATION_BTN, pDoc->m_pPostCtrl->IsPostEnable());
}
void CCMWindItemRUS2016_User::OnCmdCreateWindPulsation()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CCMWindPulsationDlg dlg{ pDoc, m_Key };
	dlg.SetInitPos(D_INIT_POS_RT);

	int nModeNum = 0;
	if (pDoc->m_pPostCtrl->IsPostEnable())
	{
		nModeNum = pDoc->m_pPostCtrl->GetNumEigvPost();
	}
	else
	{
		nModeNum = pDoc->m_pAttrCtrl->GetNumEigv();
	}
	
	if ((!pDoc->m_pAttrCtrl2->GetWnps(m_Key, m_WnpsD) || !m_bModify) && !m_bSaveWnps)
	{
		m_WnpsD.Initialize();

		m_WnpsD.arModeShapesX.SetSize(nModeNum);
		m_WnpsD.arModeShapesY.SetSize(nModeNum);
		for (int i = 0; i < nModeNum; i++)
		{
			m_WnpsD.arModeShapesX[i] = true;
			m_WnpsD.arModeShapesY[i] = true;
		}
	}

	if(m_WnpsD.arModeShapesX.GetSize() != nModeNum)
	{
		//무조건 같아야 하는데 다르면 큰일남..
		ASSERT(0);
		m_WnpsD.arModeShapesX.SetSize(nModeNum);
		m_WnpsD.arModeShapesY.SetSize(nModeNum);
		for (int i = 0; i < nModeNum; i++)
		{
			m_WnpsD.arModeShapesX[i] = true;
			m_WnpsD.arModeShapesY[i] = true;
		}
	}

	CArray<double, double> arModePercentX; arModePercentX.SetSize(nModeNum);
	CArray<double, double> arModePercentY; arModePercentY.SetSize(nModeNum);
	T_MPAM_D MpamD; MpamD.Initialize();
	T_EMMR_D EmmrD; EmmrD.Initialize();
	for (int i = 0; i < nModeNum; i++)
	{
		if (pDoc->m_pPostCtrl->IsPostEnable())
		{
			pDoc->m_pPostCtrl->GetEiMpam(i + 1, MpamD);
			pDoc->m_pPostCtrl->GetEiEmmr(i + 1, EmmrD);
		}
		arModePercentX[i] = MpamD.dblMpam[0];
		arModePercentY[i] = MpamD.dblMpam[1];
	}

	dlg.SetDataSource(&m_WnpsD, arModePercentX, arModePercentY);

 	if (dlg.DoModal() == IDOK)
 	{
		m_bSaveWnps = TRUE;
 	}
}