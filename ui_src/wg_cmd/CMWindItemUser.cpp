#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemUser.h"
#include "CMWindItemDlg.h"
#include "../wg_base/DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMWindItemUser::CCMWindItemUser(CWnd* pParent /*=NULL*/)
: CChildDialog(CCMWindItemUser::IDD, pParent), m_LateralLoad(0, TRUE)
{
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;	
	m_bModify = FALSE;
	m_bInit = TRUE;
	m_Data.Initialize();

	m_aCtrlExRadio.Add(IDC_CMD_XDIR_POS);
	m_aCtrlExRadio.Add(IDC_CMD_XDIR_NEG);
	m_aCtrlExRadio.Add(IDC_CMD_XDIR_NONE);

	m_aCtrlEyRadio.Add(IDC_CMD_YDIR_POS);
	m_aCtrlEyRadio.Add(IDC_CMD_YDIR_NEG);
	m_aCtrlEyRadio.Add(IDC_CMD_YDIR_NONE);
}

void CCMWindItemUser::SetData2Dlg(T_WIND_D& data, BOOL bModify)
{
	m_bModify = bModify;
	m_Data.nEccen_X = data.nEccen_X;
	m_Data.nEccen_Y = data.nEccen_Y;

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

BOOL CCMWindItemUser::SetDlg2Data(T_WIND_D& data)
{
	BOOL bOK = Dlg2Data();
	data.arUser.RemoveAll();
	data.arUser.Copy(m_Data.arUser);
	data.nEccen_X = m_Data.nEccen_X;
	data.nEccen_Y = m_Data.nEccen_Y;
	return bOK;
}

void CCMWindItemUser::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCMWindItemUser, CChildDialog)
END_MESSAGE_MAP()

BOOL CCMWindItemUser::OnInitDialog()
{
	CChildDialog::OnInitDialog();
	SetControl();
	Data2Dlg();
	m_bInit = FALSE;
	m_bModify = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindItemUser::SetControl()
{
	m_Grid.SubclassDlgItem(IDC_CMD_GRID, this);
	CRect rect;
	GetDlgItem(IDC_CMD_GRID)->GetWindowRect(&rect);
	m_Grid.Initialize(rect.Width());
}

BOOL CCMWindItemUser::DestroyWindow()
{
	Dlg2Data();
	return CChildDialog::DestroyWindow();
}

void CCMWindItemUser::Data2Dlg()
{
	if(!m_bModify && m_Data.arUser.GetSize() == 0)
	{
		m_Data.Initialize();
		m_Data.nCodeType = USER_W_TYPE;
		SetData2Dlg(m_Data, FALSE);
	}
	m_Grid.SetData(m_Data.arUser);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlExRadio, m_Data.nEccen_X);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlEyRadio, m_Data.nEccen_Y);
}

BOOL CCMWindItemUser::Dlg2Data()
{	
	m_Grid.GetData(m_Data.arUser);

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlExRadio, m_Data.nEccen_X);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlEyRadio, m_Data.nEccen_Y);
	return TRUE;
}