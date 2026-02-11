#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisTaiwan86PeriodDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\wg_base\DlgUtil.h"

CSeisTaiwan86PeriodDlg::CSeisTaiwan86PeriodDlg(int nMode, bool bHideDirY, CWnd* pParent)
	: CDialogMove(CSeisTaiwan86PeriodDlg::IDD, pParent)
{
	m_nMethodX = 0;
	m_nMethodY = 0;
	m_dAcX = 0.0;
	m_dAcY = 0.0;
	m_dHnX = 0.0;
	m_dHnY = 0.0;
	m_nMode = nMode; // EN_DLG_MODE_TAIWAN_XXXX
	m_bHideDirY = bHideDirY;
}

void CSeisTaiwan86PeriodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_SEIS_YAc, m_wndAcY);
	DDX_Control(pDX, IDC_CMD_SEIS_XAc, m_wndAcX);
	DDX_Control(pDX, IDC_ETC_SEIS_YHn, m_wndHnY);
	DDX_Control(pDX, IDC_ETC_SEIS_XHn, m_wndHnX);
	DDX_Radio(pDX, IDC_CMD_XMETHOD1, m_nMethodX);
	DDX_Radio(pDX, IDC_CMD_YMETHOD1, m_nMethodY);
}

BEGIN_MESSAGE_MAP(CSeisTaiwan86PeriodDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_XMETHOD1, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_CMD_XMETHOD2, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_CMD_XMETHOD3, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_CMD_XMETHOD4, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_CMD_YMETHOD1, OnRdoYmethodChange)
	ON_BN_CLICKED(IDC_CMD_YMETHOD2, OnRdoYmethodChange)
	ON_BN_CLICKED(IDC_CMD_YMETHOD3, OnRdoYmethodChange)
	ON_BN_CLICKED(IDC_CMD_YMETHOD4, OnRdoYmethodChange)
END_MESSAGE_MAP()

void CSeisTaiwan86PeriodDlg::OnRdoXmethodChange()
{
	UpdateData(TRUE);
	m_wndAcX.EnableWindow(m_nMethodX == 3);
}

void CSeisTaiwan86PeriodDlg::OnRdoYmethodChange()
{
	UpdateData(TRUE);
	m_wndAcY.EnableWindow(m_nMethodY == 3);
}

void CSeisTaiwan86PeriodDlg::OnOK()
{
	UpdateData(TRUE);

	m_dHnX = m_wndHnX.GetEditValue();
	m_dHnY = m_wndHnY.GetEditValue();
	m_dAcX = m_wndAcX.GetEditValue();
	m_dAcY = m_wndAcY.GetEditValue();
	if ( !CalcPeriod(m_dHnX, m_dAcX, m_dPeriodX, m_nMethodX) ) return;
	if ( !CalcPeriod(m_dHnY, m_dAcY, m_dPeriodY, m_nMethodY) ) return;

	CDialogMove::OnOK();
}

BOOL CSeisTaiwan86PeriodDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	switch ( m_nMode )
	{
	case EN_DLG_MODE_TAIWAN_2006:
	case EN_DLG_MODE_TAIWAN_2011:
	case EN_DLG_MODE_TAIWAN_2022:
		{
			AlignDialog();
			m_wndHnX.SetEditUnit(m_dHnX);
			m_wndHnY.SetEditUnit(m_dHnY);
		}
		break;
	case EN_DLG_MODE_TAIWAN_1999:
		{
			m_wndHnX.SetEditUnit(m_dHnX);
			m_wndHnY.SetEditUnit(m_dHnY);
			m_wndAcX.SetEditUnit(m_dAcX);
			m_wndAcY.SetEditUnit(m_dAcY);
		}
		break;
	default: { ASSERT(0); return FALSE; }
	}
	HideCtrlDirY();
	ChangeCaption();

	OnRdoXmethodChange();
	OnRdoYmethodChange();

	UpdateData(FALSE);
	return TRUE;
}

BOOL CSeisTaiwan86PeriodDlg::CalcPeriod(double dHn, double dAc, double &dPeriod, int nMethod)
{
	switch ( nMethod )
	{
	case 0:
		{
			dPeriod = 0.085 * pow(dHn, ( 3.0/4.0 ));
		}
		break;
	case 1:
		{
			dPeriod = 0.070 * pow(dHn, ( 3.0/4.0 ));
		}
		break;
	case 2:
		{
			dPeriod = 0.050 * pow(dHn, ( 3.0/4.0 ));
		}
		break;
	case 3:
		{
			if ( dAc == 0.0 ) return FALSE;
			dPeriod = 0.075 * pow(dHn, ( 3.0/4.0 )) / sqrt(dAc);
		}
		break;
	}
	return TRUE;
}

void CSeisTaiwan86PeriodDlg::AlignDialog()
{
	// 불필요한 옵션은 없애고...
	CArray<UINT, UINT> aCtrlID;
	aCtrlID.Add(IDC_CMD_XMETHOD4);
	aCtrlID.Add(IDC_CMD_ACX_TITLE);
	aCtrlID.Add(IDC_CMD_SEIS_XAc);
	aCtrlID.Add(IDC_CMD_ACX_UNIT);
	aCtrlID.Add(IDC_CMD_YMETHOD4);
	aCtrlID.Add(IDC_CMD_ACY_TITLE);
	aCtrlID.Add(IDC_CMD_SEIS_YAc);
	aCtrlID.Add(IDC_CMD_ACY_UNIT);
	CDlgUtil::CtrlShowHide(this, aCtrlID, FALSE);

	// 남은 옵션들을 보기좋게 정렬한다.
	aCtrlID.RemoveAll();
	aCtrlID.Add(IDC_ETC_SEIS_XHn);
	aCtrlID.Add(IDC_CMD_HNX_STC);
	aCtrlID.Add(IDC_CMD_HNX_UNIT);
	aCtrlID.Add(IDC_ETC_SEIS_YHn);
	aCtrlID.Add(IDC_CMD_HNY_STC);
	aCtrlID.Add(IDC_CMD_HNY_UNIT);
	CRect rMove;
	for ( int i = 0; i < aCtrlID.GetSize(); ++i )
	{
		CWnd* pWnd = GetDlgItem(aCtrlID[i]);
		pWnd->GetWindowRect(rMove);
		rMove.bottom = rMove.bottom - 26;
		rMove.top = rMove.top - 26;
		ScreenToClient(rMove);
		GetDlgItem(aCtrlID[i])->MoveWindow(rMove);
	}

	aCtrlID.RemoveAll();
	aCtrlID.Add(IDOK);
	aCtrlID.Add(IDCANCEL);
	for ( int i = 0; i < aCtrlID.GetSize(); ++i )
	{
		CWnd* pWnd = GetDlgItem(aCtrlID[i]);
		pWnd->GetWindowRect(rMove);
		rMove.bottom = rMove.bottom - 45;
		rMove.top = rMove.top - 45;
		ScreenToClient(rMove);
		GetDlgItem(aCtrlID[i])->MoveWindow(rMove);
	}

	aCtrlID.RemoveAll();
	aCtrlID.Add(IDC_ETC_SEIS_CALCPERIOD_TAIWAN_Y_STC);
	aCtrlID.Add(IDC_ETC_SEIS_CALCPERIOD_TAIWAN_X_STC);
	for ( int i = 0; i < aCtrlID.GetSize(); ++i )
	{
		CWnd* pWnd = GetDlgItem(aCtrlID[i]);
		pWnd->GetWindowRect(rMove);
		rMove.bottom = rMove.bottom - 45;
		ScreenToClient(rMove);
		GetDlgItem(aCtrlID[i])->MoveWindow(rMove);
	}

	this->GetWindowRect(rMove);
	rMove.bottom = rMove.bottom - 45;
	this->MoveWindow(rMove);
}

void CSeisTaiwan86PeriodDlg::HideCtrlDirY()
{
	if ( !m_bHideDirY ) return;

	//2024.01.18 chlim ini 파일의 문자열로 컨트롤 하도록 함. (IDS_IDD_ETC_SEIS_CALCPERIOD_TAIWAN86_IDC_ETC_SEIS_CALCPERIOD_TAIWAN_X_STC)
	//GetDlgItem(IDC_ETC_SEIS_CALCPERIOD_TAIWAN_X_STC)->SetWindowText(_T("Period"));

	CArray<UINT, UINT> aCtrlHide;
	aCtrlHide.Add(IDC_ETC_SEIS_CALCPERIOD_TAIWAN_Y_STC);
	aCtrlHide.Add(IDC_CMD_YMETHOD1);
	aCtrlHide.Add(IDC_CMD_YMETHOD2);
	aCtrlHide.Add(IDC_CMD_YMETHOD3);
	aCtrlHide.Add(IDC_CMD_YMETHOD4);
	aCtrlHide.Add(IDC_CMD_HNY_STC);
	aCtrlHide.Add(IDC_ETC_SEIS_YHn);
	aCtrlHide.Add(IDC_CMD_HNY_UNIT);
	aCtrlHide.Add(IDC_CMD_ACY_TITLE);
	aCtrlHide.Add(IDC_CMD_SEIS_YAc);
	aCtrlHide.Add(IDC_CMD_ACY_UNIT);
	CDlgUtil::CtrlShowHide(this, aCtrlHide, FALSE);

	CRect rcFrameY, rcFrameX;
	GetDlgItem(IDC_ETC_SEIS_CALCPERIOD_TAIWAN_Y_STC)->GetWindowRect(rcFrameY);
	GetDlgItem(IDC_ETC_SEIS_CALCPERIOD_TAIWAN_X_STC)->GetWindowRect(rcFrameX);
	int nDistX = rcFrameX.right - rcFrameY.right;

	CArray<UINT, UINT> aCtrlMove;
	aCtrlMove.Add(IDOK);
	aCtrlMove.Add(IDCANCEL);
	CDlgUtil::CtrlMoveDistX(this, aCtrlMove, nDistX);

	CRect rcThis;
	this->GetWindowRect(rcThis);
	rcThis.right += nDistX;
	this->MoveWindow(rcThis);
}

void CSeisTaiwan86PeriodDlg::ChangeCaption()
{
	CString csCaption;
	switch ( m_nMode )
	{
	case EN_DLG_MODE_TAIWAN_1999: csCaption = _LSNew("IDS_IDD_ETC_SEIS_CALCPERIOD_TAIWAN86_EN_DLG_MODE_TAIWAN_1999"); break;
	case EN_DLG_MODE_TAIWAN_2006: csCaption = _LSNew("IDS_IDD_ETC_SEIS_CALCPERIOD_TAIWAN86_EN_DLG_MODE_TAIWAN_2006"); break;
	case EN_DLG_MODE_TAIWAN_2011: csCaption = _LSNew("IDS_IDD_ETC_SEIS_CALCPERIOD_TAIWAN86_EN_DLG_MODE_TAIWAN_2011"); break;
	case EN_DLG_MODE_TAIWAN_2022: csCaption = _LSNew("IDS_IDD_ETC_SEIS_CALCPERIOD_TAIWAN86_EN_DLG_MODE_TAIWAN_2022"); break;
	default: ASSERT(0); return;
	}
	this->SetWindowText(csCaption);
}