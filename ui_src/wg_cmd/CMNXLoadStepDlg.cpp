#include "stdafx.h"
#include "wg_cmd.h"
#include "CMNXLoadStepDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMNXLoadStepDlg::CMNXLoadStepDlg(CWnd* pParent /*= NULL*/)
	:CDialogMove(CMNXLoadStepDlg::IDD, pParent)
{

}

void CMNXLoadStepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNXLoadStepDlg)
	DDX_Control(pDX, IDC_CMD_LOADSTEP_NUM_EDT, m_edtNum);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMNXLoadStepDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_LOADSTEP_GENERATE_BTN, OnCmdGenerateLoadFactorBtn)
	ON_EN_CHANGE(IDC_CMD_LOADSTEP_NUM_EDT, OnChangeCmdGenerateLoadFactorEdt)
END_MESSAGE_MAP()

void CMNXLoadStepDlg::SetData(T_ACTL_D& data, int nMethod)
{
	m_Data.Initialize();
	m_Data = data;
// 	m_Data.nIterationMethod = nMethod;
}

void CMNXLoadStepDlg::GetData(T_ACTL_D& data)
{
	data = m_Data;
}

void CMNXLoadStepDlg::OnDataChanged()
{
	
}

BOOL CMNXLoadStepDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	m_wndGrid.SubclassDlgItem(IDC_CMD_PLLD_GRID, this);
	m_wndGrid.Initialize(&m_Data);
	m_wndGrid.SetScrollBarMode(SB_VERT | SB_HORZ, gxnEnabled | gxnEnhanced);
	m_wndGrid.MakeItemEx();

	m_edtNum.SetRange(1, 100);
	m_edtNum.SetInteger(TRUE);

	// 기존 데이터가 있으면 로드
	int nstep = (int)m_Data.aLoadSteps.GetCount();
	if (nstep > 0)
	{
		m_edtNum.SetValue(nstep);
	}
	else
	{
		m_edtNum.SetValue(1);
	}

	UpdateData(FALSE);
	return TRUE;
}

void CMNXLoadStepDlg::OnOK()
{
	if (!CheckGridData())return;
	CDialogMove::OnOK();
}

void CMNXLoadStepDlg::OnCmdGenerateLoadFactorBtn()
{
	int nStep = m_edtNum.GetEditValueInt();
	double dValue = 1.0 / nStep;

	m_wndGrid.GenerateValue(dValue);
}

void CMNXLoadStepDlg::OnChangeCmdGenerateLoadFactorEdt()
{
	int nNum = m_edtNum.GetEditValueInt();
	if (nNum > 0) 
	{
		m_wndGrid.SetRowCountUser(nNum);
		m_Data.nNumberOfIncrement = nNum;  // Step 수 저장
	}
}

BOOL CMNXLoadStepDlg::CheckGridData()
{
// 	if (m_Data.nIterationMethod == 2) //Displacement Control
// 	{
// 		double dSum = 0;
// 		for (int i = 0; i < m_Data.aLoadSteps.GetCount(); i++)
// 		{
// 			if (m_Data.aLoadSteps[i].dLoadIncFactor == 0.)
// 			{
// 				GSaveHistoryFormatNF(_LS(IDS_WG_CMD2_ADDD_Value_is_not_zero_number));
// 				return FALSE;
// 			}
// 			dSum += m_Data.aLoadSteps[i].dLoadIncFactor;
// 		}
// 		if (fabs(dSum - m_Data.dMaxDisplacementDispct) > 1e-6)
// 		{
// 			GSaveHistoryFormatNF(_LS(IDS_CMD2_Load_Incre_Factor_Sum_must_be_MAX_DISP));
// 			return FALSE;
// 		}
// 	}
// 	else
// 	{
// 		double dSum = 0;
// 		for (int i = 0; i < m_Data.aLoadSteps.GetCount(); i++)
// 		{
// 			dSum += m_Data.aLoadSteps[i].dLoadIncFactor;
// 		}
// 		if (fabs(dSum - 1.) > 1e-6)
// 		{
// 			GSaveHistoryFormatNF(_LS(IDS_CMD2_Load_Incre_Factor_Sum_must_be_1));
// 			return FALSE;
// 		}
// 	}

	// row 을 바꾸지 않고 바로 OK 를 누를 경우 데이터를 가져오지 못하는 경우 처리
	int nNum = m_edtNum.GetEditValueInt();
	if (nNum <= 0) {
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD_Number_of_load_steps_must_be_greater_than_zero));
		return FALSE;
	}

	int nStep = (int)m_Data.aLoadSteps.GetCount();
	if (nNum != nStep)
	{
		ASSERT(0);
		m_Data.aLoadSteps.SetSize(nNum);
	}

	CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> aLoadSteps;
	aLoadSteps.RemoveAll();
	aLoadSteps.SetSize(nNum);

	double dSum = 0.;
	int nKey;
	ROWCOL nRow;
	for (int i = 0; i < nNum; i++)
	{
		T_NLCT_LOADSTEP temp; temp.Initialize();
		nRow = (ROWCOL)i+1;
		m_wndGrid.GetValue(nRow,nKey,temp);

		if (temp.dLoadIncFactor <= 0.)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD2_ADDD_Value_is_negative_number));
			return FALSE;
		}

		aLoadSteps.SetAt(i,temp);
		dSum += temp.dLoadIncFactor;
	}

	if (fabs(dSum - 1.0) > 1e-6)
	{
	 	GSaveHistoryFormatNF(_LS(IDS_CMD2_Load_Incre_Factor_Sum_must_be_1));
	 	return FALSE;
	}

	bool bSameStep = true;
	dSum = 0.;
	for (int i = 0; i < m_Data.aLoadSteps.GetCount(); i++)
	{
		if (m_Data.aLoadSteps[i].nStep != aLoadSteps[i].nStep)
		{
			ASSERT(0);
			bSameStep = false;
			continue;
		}

		if (bSameStep)
		{
			if (m_Data.aLoadSteps[i].bSave != aLoadSteps[i].bSave)
			{
				m_Data.aLoadSteps[i].bSave = aLoadSteps[i].bSave;
			}

			double dDiff = fabs(m_Data.aLoadSteps[i].dLoadIncFactor - aLoadSteps[i].dLoadIncFactor);
			if (dDiff > 1.e-10 || aLoadSteps[i].dLoadIncFactor > 0.)
			{
				m_Data.aLoadSteps[i].dLoadIncFactor = aLoadSteps[i].dLoadIncFactor;
			}
		}

		dSum += m_Data.aLoadSteps[i].dLoadIncFactor;
	}

	if (fabs(dSum - 1.0) > 1e-6)
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD2_Load_Incre_Factor_Sum_must_be_1));
		return FALSE;
	}
	//

	//Save Step이 하나도 정의되어 있지 않다면 마지막 스텝에 체크
	BOOL bExistSave = FALSE;
	for (int i = 0; i < m_Data.aLoadSteps.GetCount(); i++)
	{
		if (m_Data.aLoadSteps[i].bSave)
		{
			bExistSave = TRUE;
			break;
		}
	}
	if (!bExistSave && m_Data.aLoadSteps.GetCount())
	{
		m_Data.aLoadSteps[m_Data.aLoadSteps.GetCount() - 1].bSave = TRUE;
	}
	return TRUE;
}

BOOL CMNXLoadStepDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			if (pMsg->wParam == VK_RETURN)
				if (pMsg->wParam == VK_ESCAPE)

					::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
			// DO NOT process further
		}
	}
	return CDialogMove::PreTranslateMessage(pMsg);
}