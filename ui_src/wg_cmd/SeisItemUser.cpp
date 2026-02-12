#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemUser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSeisItemUser::CSeisItemUser(CWnd* pParent /*=NULL*/) 
: CChildDialog(CSeisItemUser::IDD, pParent), m_LateralLoad(0, TRUE)
{
	m_aData.RemoveAll();
	m_bInit = TRUE;
}

void CSeisItemUser::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSeisItemUser, CChildDialog)
END_MESSAGE_MAP()

BOOL CSeisItemUser::OnInitDialog()
{
	CChildDialog::OnInitDialog();
	SetControl();	
	Data2Dlg();
	m_bInit = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemUser::SetControl()
{
	m_Grid.SubclassDlgItem(IDC_CMD_GRID, this);
	CRect rect;
	GetDlgItem(IDC_CMD_GRID)->GetWindowRect(&rect);
	m_Grid.Initialize(rect.Width());
}

BOOL CSeisItemUser::DestroyWindow()
{
	return CChildDialog::DestroyWindow();
}

void CSeisItemUser::SetData2Dlg(T_SEIS_D& data, BOOL bModify)
{
	m_bModify = bModify;
	if (bModify && data.arUser.GetCount() > 0)
	{
		m_aData.Copy(data.arUser);
	}
	else
	{
		m_LateralLoad.MakeSeismicLoad(data, FALSE, _T(""), TRUE);
		int nSize = m_LateralLoad.m_aSeisLoad_X.GetSize();
		if (nSize == 0) return;
		if (m_LateralLoad.m_aSeisLoad_X[nSize - 1].KeyStory == 0)
			--nSize;

		m_aData.SetSize(nSize);
		for (int i = 0; i < nSize; i++)
		{
			stSeisLoad* pData = &m_LateralLoad.m_aSeisLoad_X[i];
			m_aData[i].Initialize();
			m_aData[i].KeyStor = pData->KeyStory;
			m_aData[i].dWeight = pData->dWeight;
			m_aData[i].dElev = pData->dLevel;
			m_aData[i].dForceX = 0.0;
			m_aData[i].dForceY = 0.0;
		}
	}
	if(!m_bInit) Data2Dlg();
}

BOOL CSeisItemUser::SetDlg2Data(T_SEIS_D& data)
{
	BOOL bOK = Dlg2Data();
	data.arUser.RemoveAll();
	data.arUser.Copy(m_aData);
	return bOK;
}

void CSeisItemUser::Data2Dlg()
{
	m_Grid.SetData(m_aData);	
}

BOOL CSeisItemUser::Dlg2Data()
{
	m_Grid.GetData(m_aData);
	return TRUE;
}