// HeelLoadChildDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HeelLoadChildDialog.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_INITCTRL.h"
#include "..\wg_db\wg_db_unitctrl.h"

#include "..\wg_base\wg_base_StrParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeelLoadChildDialog dialog


CHeelLoadChildDialog::CHeelLoadChildDialog(UINT resID,CWnd* pParent /*=NULL*/)
	: CChildDialog(resID, pParent)
{
	CChildDialog::resID = resID;
	m_bIsCreated = FALSE;
	m_pParent = pParent;
	m_pAssocWindow = pParent;
		
	//{{AFX_DATA_INIT(CHeelLoadChildDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CHeelLoadChildDialog::~CHeelLoadChildDialog()
{
}


void CHeelLoadChildDialog::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHeelLoadChildDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHeelLoadChildDialog, CChildDialog)
	//{{AFX_MSG_MAP(CHeelLoadChildDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeelLoadChildDialog message handlers

void CHeelLoadChildDialog::SetDefault()// virtual
{

}




void CHeelLoadChildDialog::SetData() // virtual 
{

}

void CHeelLoadChildDialog::GetData()  // virtual
{

}

BOOL CHeelLoadChildDialog::ISVALID() // virtual
{

 return TRUE;
}

BOOL CHeelLoadChildDialog::IsValid_n(CEdit* pEdt,int nMin,int nMax,int nDefValue)
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


BOOL CHeelLoadChildDialog::IsValid_d(CEdit* pEdt,double dMin,double dMax,double dDefValue)
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

void CHeelLoadChildDialog::MakeSpectrumData()
{

}

BOOL CHeelLoadChildDialog::IsValidCmb_n(CComboBox* pEdt,int nMin,int nMax,int nDefValue)
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


BOOL CHeelLoadChildDialog::IsValidCmb_d(CComboBox* pEdt,double dMin,double dMax,double dDefValue)
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

void CHeelLoadChildDialog::GetConvertFactor_ForceUnitToCurrent(int Index,double& dMultFact,double& dDivFact)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
	T_UNIT_INDEX UnitIndexOld, UnitIndexNew;

	pUnitCtrl->GetUnitIndexCurrent(UnitIndexOld);
	UnitIndexNew=UnitIndexOld;
	UnitIndexNew.nBase_Force= Index;
	pUnitCtrl->SaveUnitChangeFactor();
	pUnitCtrl->SetUnitIndexCurrent(UnitIndexNew);
	pUnitCtrl->SetUnitIndexCurrent(UnitIndexOld);

	dMultFact = pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_FORCE);
	dDivFact = pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_FORCE);

	pUnitCtrl->RestoreUnitChangeFactor();
	
}

void CHeelLoadChildDialog::GetConvertFactor_AreaUnitToCurrent(int Index, double& dMultFact, double& dDivFact)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
	T_UNIT_INDEX UnitIndexOld, UnitIndexNew;

	pUnitCtrl->GetUnitIndexCurrent(UnitIndexOld);
	UnitIndexNew = UnitIndexOld;
	UnitIndexNew.nBase_Length = Index;
	pUnitCtrl->SaveUnitChangeFactor();
	pUnitCtrl->SetUnitIndexCurrent(UnitIndexNew);
	pUnitCtrl->SetUnitIndexCurrent(UnitIndexOld);

	dMultFact = pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_AREA);
	dDivFact = pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_AREA);

	pUnitCtrl->RestoreUnitChangeFactor();

}
void CHeelLoadChildDialog::GetConvertFactor_StressUnitToCurrent(int IndexF, int IndexL, double& dMultFact, double& dDivFact)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
	T_UNIT_INDEX UnitIndexOld, UnitIndexNew;

	pUnitCtrl->GetUnitIndexCurrent(UnitIndexOld);
	UnitIndexNew = UnitIndexOld;
	UnitIndexNew.nBase_Force = IndexF;
	UnitIndexNew.nBase_Length = IndexL;
	pUnitCtrl->SaveUnitChangeFactor();
	pUnitCtrl->SetUnitIndexCurrent(UnitIndexNew);
	pUnitCtrl->SetUnitIndexCurrent(UnitIndexOld);

	dMultFact = pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_STRESS);
	dDivFact = pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS);

	pUnitCtrl->RestoreUnitChangeFactor();

}

