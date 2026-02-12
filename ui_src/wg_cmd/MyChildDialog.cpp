// MyChildDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MyChildDialog.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\wg_base_StrParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\wg_db\DB_ST_DT.h"

/////////////////////////////////////////////////////////////////////////////
// CMyChildDialog dialog


CMyChildDialog::CMyChildDialog(UINT resID,CWnd* pParent /*=NULL*/, std::shared_ptr<CSpfcBaseUtil> pSpfcUtil)
	: CChildDialog(resID, pParent)
{
	CChildDialog::resID = resID;
	m_bIsCreated = FALSE;
	m_pParent = pParent;
	m_pAssocWindow = pParent;

	m_pSpfcBaseUtil = pSpfcUtil;

	m_nCodeType = 0;
	m_nStepCoeff = ChildDlg::STEP_COEFF;
	m_nPeriodCoeff = ChildDlg::PERIOD_COEFF;

	//{{AFX_DATA_INIT(CMyChildDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CMyChildDialog::~CMyChildDialog()
{
}


void CMyChildDialog::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyChildDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyChildDialog, CChildDialog)
	//{{AFX_MSG_MAP(CMyChildDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyChildDialog message handlers

void CMyChildDialog::SetDefault()// virtual
{

}




void CMyChildDialog::SetData() // virtual 
{

}

void CMyChildDialog::GetData()  // virtual
{

}
UINT CMyChildDialog::GetSpfcK()
{
	return 0;
}

BOOL CMyChildDialog::ISVALID() // virtual
{

 return TRUE;
}

BOOL CMyChildDialog::IsValid_n(CEdit* pEdt,int nMin,int nMax,int nDefValue)
{
	MyParser parser;  
	CString sValue,msg;
	int nValue;
	
	if(IsWindow(pEdt->m_hWnd))
	{
		pEdt->GetWindowText(sValue);
		sValue.TrimLeft();  
		if(parser.ParsingINumberArg(sValue) && sValue != _T(""))
		{
			parser.GetINumberData(nValue);
			if(nValue < nMin )
			{
				sValue.Format(_T("%d"),nMin);
				pEdt->SetWindowText(sValue);
				pEdt->SetFocus();
				pEdt->SetSel(0,-1);
				m_pWndError = pEdt;
				msg.Format(_LS(IDS_WG_CMD__ADDD__the_Value_must_be_greater_than__d),nMin);
				MessageBox(msg);

				return FALSE;
			}
			else if( nValue > nMax)
			{
				sValue.Format(_T("%d"),nMax);
				pEdt->SetWindowText(sValue);
				pEdt->SetFocus();
				pEdt->SetSel(0,-1);
				m_pWndError = pEdt;
				msg.Format(_LS(IDS_WG_CMD__ADDD__the_Value_must_be_less_than__d),nMax);
				MessageBox(msg);

				return FALSE;
			}
			else
				return TRUE;
		}
		else
		{ 
					
			sValue.Format(_T("%d"),nDefValue);
			pEdt->SetWindowText(sValue);
			pEdt->SetFocus();
			pEdt->SetSel(0,-1);
			m_pWndError = pEdt;
			MessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_Input));
			return FALSE;
		}
	}
	
	return TRUE;
}


BOOL CMyChildDialog::IsValid_d(CEdit* pEdt,double dMin,double dMax,double dDefValue)
{
	MyParser parser;  
	CString sValue,msg;
	double dValue;
	
	if(IsWindow(pEdt->m_hWnd))
	{
		pEdt->GetWindowText(sValue);
		sValue.TrimLeft();
		if(parser.ParsingNumberArg(sValue) && sValue != _T(""))
		{
			parser.GetNumberData(dValue);
			if(dValue < dMin )
			{
				sValue.Format(_T("%4.1f"),dMin);
				pEdt->SetWindowText(sValue);
				pEdt->SetFocus();
				pEdt->SetSel(0,-1);
				m_pWndError = pEdt;
				msg.Format(_LS(IDS_WG_CMD__ADDD__the_Value_must_be_greater_than__4),dMin);
				MessageBox(msg);
				return FALSE;
			}
			else if( dValue > dMax)
			{
				sValue.Format(_T("%4.1f"),dMax);
				pEdt->SetWindowText(sValue);
				pEdt->SetFocus();
				pEdt->SetSel(0,-1);
				m_pWndError = pEdt;
				msg.Format(_LS(IDS_WG_CMD__ADDD__the_Value_must_be_less_than__4_1f),dMax);
				MessageBox(msg);

				return FALSE;
			}
			else
				return TRUE;
		}
		else
		{ 
	
			sValue.Format(_T("%4.1f"),dDefValue);
			pEdt->SetWindowText(sValue);
			pEdt->SetFocus();
			pEdt->SetSel(0,-1);
			m_pWndError = pEdt;
			MessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_Input));
/*    ERROR  // 포커스 순서 문제.
			MessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_Input));
			sValue.Format(_T("%f"),dDefValue);
			pEdt->SetWindowText(sValue);
			pEdt->SetFocus();
*/
			return FALSE;
		}
	}
	return TRUE;
}

void CMyChildDialog::MakeSpectrumData(BOOL bOnlyCalc/*=FALSE*/)
{
	if (m_pSpfcBaseUtil == nullptr)
	{
		ASSERT(false);
		return;
	}

	m_pSpfcBaseUtil->MakeSpectrumData(static_cast<bool>(bOnlyCalc));
	SetSpfcResult();
}

void CMyChildDialog::MakePOSpectrumData(double dDamping,double SRa,double SRv,CArray <double,double>& aPeriod,CArray <double,double>& aAccel)
{
	if (m_pSpfcBaseUtil == nullptr)
	{
		ASSERT(false);
		return;
	}

	m_pSpfcBaseUtil->MakePOSpectrumData(dDamping, SRa, SRv, aPeriod, aAccel);
}

void CMyChildDialog::SetPushOverData(BOOL bInit/*=TRUE*/)
{

}

BOOL CMyChildDialog::IsValidCmb_n(CComboBox* pEdt,int nMin,int nMax,int nDefValue)
{
	MyParser parser;  
	CString sValue,msg;
	int nValue;
	
	if(IsWindow(pEdt->m_hWnd))
	{
		pEdt->GetWindowText(sValue);
		sValue.TrimLeft();
		if(parser.ParsingINumberArg(sValue) && sValue != _T(""))
		{
			parser.GetINumberData(nValue);
			if(nValue < nMin )
			{
				sValue.Format(_T("%d"),nMin);
				pEdt->SetWindowText(sValue);
				pEdt->SetFocus();
				pEdt->SetEditSel(0,-1);
				m_pWndError = pEdt;
				msg.Format(_LS(IDS_WG_CMD__ADDD__the_Value_must_be_greater_than__d),nMin);
				MessageBox(msg);

				return FALSE;
			}
			else if( nValue > nMax)
			{
				sValue.Format(_T("%d"),nMax);
				pEdt->SetWindowText(sValue);
				pEdt->SetFocus();
				pEdt->SetEditSel(0,-1);
				m_pWndError = pEdt;
				msg.Format(_LS(IDS_WG_CMD__ADDD__the_Value_must_be_less_than__d),nMax);
				MessageBox(msg);

				return FALSE;
			}
			else
				return TRUE;
		}
		else
		{ 
					
			sValue.Format(_T("%4d"),nDefValue);
			pEdt->SetWindowText(sValue);
			pEdt->SetFocus();
			pEdt->SetEditSel(0,-1);
			m_pWndError = pEdt;
			MessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_Input));
			return FALSE;
		}
	}
	
	return TRUE;
}


BOOL CMyChildDialog::IsValidCmb_d(CComboBox* pEdt,double dMin,double dMax,double dDefValue)
{
	MyParser parser;  
	CString sValue,msg;
	double dValue;
	
	if(IsWindow(pEdt->m_hWnd))
	{
		pEdt->GetWindowText(sValue);
		sValue.TrimLeft(); 
		if(parser.ParsingNumberArg(sValue) && sValue != _T(""))
		{
			parser.GetNumberData(dValue);
			if(dValue < dMin )
			{
				sValue.Format(_T("%4.1f"),dMin);
				pEdt->SetWindowText(sValue);
				pEdt->SetFocus();
				pEdt->SetEditSel(0,-1);
				m_pWndError = pEdt;
				msg.Format(_LS(IDS_WG_CMD__ADDD__the_Value_must_be_greater_than__4),dMin);
				MessageBox(msg);
				return FALSE;
			}
			else if( dValue > dMax)
			{
				sValue.Format(_T("%4.1f"),dMax);
				pEdt->SetWindowText(sValue);
				pEdt->SetFocus();
				pEdt->SetEditSel(0,-1);
				m_pWndError = pEdt;
				msg.Format(_LS(IDS_WG_CMD__ADDD__the_Value_must_be_less_than__4_1f),dMax);
				MessageBox(msg);

				return FALSE;
			}
			else
				return TRUE;
		}
		else
		{   
			sValue.Format(_T("%4.1f"),dDefValue);
			pEdt->SetWindowText(sValue);
			pEdt->SetFocus();
			pEdt->SetEditSel(0,-1);
			m_pWndError = pEdt;
			MessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_Input));
/*    ERROR  // 포커스 순서 문제.
			MessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_Input));
			sValue.Format(_T("%f"),dDefValue);
			pEdt->SetWindowText(sValue);
			pEdt->SetFocus();
*/
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CMyChildDialog::GetGravity_UserDefinedSpec(double& dGravity)
{
	return TRUE;
}

void CMyChildDialog::SetCodeType(int nCodeType)
{
	m_nCodeType = nCodeType;
}

void CMyChildDialog::SetSpfcResult()
{
	if (m_pSpfcBaseUtil == nullptr)
	{
		ASSERT(false);
		return;
	}

	const auto& vecPeriod = m_pSpfcBaseUtil->GetPeriod();
	const auto& vecAccel = m_pSpfcBaseUtil->GetAccel();

	for (const auto& period : vecPeriod)
		m_parPeriod->Add(period);

	for (const auto& accel : vecAccel)
		m_parAccel->Add(accel);

	m_strFuncName = m_pSpfcBaseUtil->GetFuncName().data();
	m_sDescript = m_pSpfcBaseUtil->GetDescript().data();
}