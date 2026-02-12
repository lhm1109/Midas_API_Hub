#include "stdafx.h"
#include "wg_cmd.h"
#include "CMNXNlctLoadStepDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMNXNlctLoadStepDlg::CMNXNlctLoadStepDlg(CWnd* pParent /*= NULL*/)
	:CDialogMove(CMNXNlctLoadStepDlg::IDD, pParent)
{

}

void CMNXNlctLoadStepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNXNlctLoadStepDlg)
	DDX_Control(pDX, IDC_CMD_LOADSTEP_NUM_EDT, m_edtNum);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMNXNlctLoadStepDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_LOADSTEP_GENERATE_BTN, OnCmdGenerateLoadFactorBtn)
	ON_EN_CHANGE(IDC_CMD_LOADSTEP_NUM_EDT, OnChangeCmdGenerateLoadFactorEdt)
END_MESSAGE_MAP()

void CMNXNlctLoadStepDlg::SetData(T_NLCT_DATA& data, int nMethod)
{
	m_Data.Initialize();
	m_Data = data;
// 	m_Data.nIterationMethod = nMethod;
}

void CMNXNlctLoadStepDlg::GetData(T_NLCT_DATA& data)
{
	data = m_Data;
}

void CMNXNlctLoadStepDlg::OnDataChanged()
{
	
}

BOOL CMNXNlctLoadStepDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	m_wndGrid.SubclassDlgItem(IDC_CMD_PLLD_GRID, this);
	m_wndGrid.Initialize(&m_Data);
	m_wndGrid.SetScrollBarMode(SB_VERT | SB_HORZ, gxnEnabled | gxnEnhanced);
	m_wndGrid.MakeItemEx();

	m_edtNum.SetRange(1, 100);
	m_edtNum.SetInteger(TRUE);

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

void CMNXNlctLoadStepDlg::OnOK()
{
	if (!CheckGridData())return;
	CDialogMove::OnOK();
}

void CMNXNlctLoadStepDlg::OnCmdGenerateLoadFactorBtn()
{
	int nStep = m_edtNum.GetEditValueInt();
	double dValue = 1.0 / nStep;

	m_wndGrid.GenerateValue(dValue);
}

void CMNXNlctLoadStepDlg::OnChangeCmdGenerateLoadFactorEdt()
{
	int nNum = m_edtNum.GetEditValueInt();
	if (nNum > 0) 
	{
		m_wndGrid.SetRowCountUser(nNum);
		m_Data.nNumberOfIncrement = nNum;  // Step 수 저장
	}
}

BOOL CMNXNlctLoadStepDlg::CheckGridData()
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
	if(nNum <= 0) 	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD_Number_of_load_steps_must_be_greater_than_zero));
		return FALSE;
	}

	int nStep = (int)m_Data.aLoadSteps.GetCount();
	int nMin = min(nNum, nStep);
	if (nNum != nStep)
	{
		ASSERT(0);
		m_Data.aLoadSteps.SetSize(nNum);
	}

	CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> aLoadSteps;
	aLoadSteps.RemoveAll();
	aLoadSteps.SetSize(nNum);

	int nKey;
	ROWCOL nRow;
	for (int i = 0; i < nNum; i++)
	{
		T_NLCT_LOADSTEP temp; temp.Initialize();
		nRow = (ROWCOL)i+1;
		m_wndGrid.GetValue(nRow,nKey,temp);
		aLoadSteps.SetAt(i,temp);
	}

	for (int i = 0; i < m_Data.aLoadSteps.GetCount(); i++)
	{
		if (m_Data.aLoadSteps[i].nStep != aLoadSteps[i].nStep)
		{
			ASSERT(0);
			continue;
		}

		if (m_Data.aLoadSteps[i].bSave != aLoadSteps[i].bSave)
		{
			m_Data.aLoadSteps[i].bSave = aLoadSteps[i].bSave;
		}

		double dDiff = fabs(m_Data.aLoadSteps[i].dLoadIncFactor - aLoadSteps[i].dLoadIncFactor);
		if (dDiff > 1.e-10)
		{
			m_Data.aLoadSteps[i].dLoadIncFactor = aLoadSteps[i].dLoadIncFactor;
		}
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

BOOL CMNXNlctLoadStepDlg::PreTranslateMessage(MSG* pMsg)
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