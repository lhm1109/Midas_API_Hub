// CMWindPressureVelocityDefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMWindPressureFunctionHelper.h"
#include "CMWindPressureFunctionAddModDlg.h"
#include "..\wg_base\PL_Parser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureFunctionHelper dialog

CCMWindPressureFunctionHelper::CCMWindPressureFunctionHelper(CWnd* pParent)
	: CCMDlgBase(CCMWindPressureFunctionHelper::IDD, pParent)
{
	m_pParentDlg = NULL;
	m_pParentDlg = (CCMWindPressureFunctionAddModDlg*)pParent;
	
	BufClear(FALSE);
}

CCMWindPressureFunctionHelper::~CCMWindPressureFunctionHelper()
{
	m_pParentDlg->DestoryHelperDlg();
}

void CCMWindPressureFunctionHelper::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WINDP_FUNC_HELP_EQUATION, m_edtEquation);
}


BEGIN_MESSAGE_MAP(CCMWindPressureFunctionHelper, CCMDlgBase)
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_UNDO,      OnCmdUndo    )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_REDO,      OnCmdRedo    )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_X,         OnCmdNUM_X   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_Y,         OnCmdNUM_Y   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_Z,         OnCmdNUM_Z   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_PI,        OnCmdNUM_pi  )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_E,         OnCmdNUM_e   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_NUM1,      OnCmdNUM_1   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_NUM2,      OnCmdNUM_2   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_NUM3,      OnCmdNUM_3   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_NUM4,      OnCmdNUM_4   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_NUM5,      OnCmdNUM_5   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_NUM6,      OnCmdNUM_6   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_NUM7,      OnCmdNUM_7   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_NUM8,      OnCmdNUM_8   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_NUM9,      OnCmdNUM_9   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_NUM0,      OnCmdNUM_0   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_DOT,       OnCmdNUM_dot )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_ADD,       OnCmdAdd     )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_APPLY,     OnCmdApply   )
	
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_TOOL_BACKSP, OnCmdToolBackSpace )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_TOOL_CLEAR,  OnCmdToolClear     )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_TOOL_PM,     OnCmdToolPM        )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_TOOL_SQRT,   OnCmdToolSqrt      )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_TOOL_DIV,    OnCmdToolDivide    )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_TOOL_INV,    OnCmdToolInv       )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_TOOL_MULT,   OnCmdToolMult      )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_TOOL_MINUS,  OnCmdToolMinus     )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_TOOL_PLUS,   OnCmdToolPlus      )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_TOOL_EQUAL,  OnCmdToolEqual     )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_TOOL_GSTART, OnCmdToolGStart    )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_TOOL_GEND,   OnCmdToolGEnd      )

	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_SIN,    OnCmdFunc_Sin    )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_COS,    OnCmdFunc_Cos    )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_TAN,    OnCmdFunc_Tan    )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_ASIN,   OnCmdFunc_aSin   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_ACOS,   OnCmdFunc_aCos   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_ATAN,   OnCmdFunc_aTan   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_SINH,   OnCmdFunc_Sinh   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_COSH,   OnCmdFunc_Cosh   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_TANH,   OnCmdFunc_Tanh   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_LOG,    OnCmdFunc_Log    )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_LN,     OnCmdFunc_Ln     )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_EXP,    OnCmdFunc_Exp    )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_DEG,    OnCmdFunc_Deg    )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_RAD,    OnCmdFunc_Rad    )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_SQRT,   OnCmdFunc_Sqrt   )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_SQ,     OnCmdFunc_Square )
	ON_BN_CLICKED(IDC_WINDP_FUNC_HELP_CUBIC,  OnCmdFunc_Cubic  )

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureFunctionHelper message handlers
BOOL CCMWindPressureFunctionHelper::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();

	SetTextSpecial();
	ResetXYZ();
	OnCmdToolClear();
	BufClear();  
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindPressureFunctionHelper::OnCmdApply() 
{
	CString strTemp;
	m_edtEquation.GetWindowText(strTemp);
	m_pParentDlg->ApplyEquation(strTemp);
	SetEquation(_T(""), FALSE);
}

void CCMWindPressureFunctionHelper::OnCmdAdd() 
{
	CString strTemp;
	m_edtEquation.GetWindowText(strTemp);
	m_pParentDlg->AppendEquation(strTemp);
	SetEquation(_T(""), FALSE);
}

void CCMWindPressureFunctionHelper::OnCancel()
{
	DestroyWindow();  
}

void CCMWindPressureFunctionHelper::AddStringEqua_Func( const CString& str )
{
	int nStart, nEnd;
	m_edtEquation.GetSel(nStart, nEnd);
	if(nEnd - nStart > 0)
	{
		CString strTemp;
		CString strEq= GetEquation();
		strTemp.Format(_T("%s%s(%s)%s"), strEq.Left(nStart), str, strEq.Mid(nStart, nEnd - nStart), strEq.Right(strEq.GetLength() - nEnd));
		SetEquation(strTemp);
	m_edtEquation.SetSel(nEnd + str.GetLength() + 1, nEnd + str.GetLength() + 1);
	}
	else
	{
		int nPos = m_edtEquation.CharFromPos(m_edtEquation.GetCaretPos());
		CString strTemp;
		CString strEq= GetEquation();
		strTemp.Format(_T("%s%s()%s"), strEq.Left(nPos), str, strEq.Right(strEq.GetLength() - nPos));
		SetEquation(strTemp);
	m_edtEquation.SetSel(nPos + str.GetLength() + 1, nPos + str.GetLength() + 1);
	}
}

void CCMWindPressureFunctionHelper::AddStringEqua( const CString& str, BOOL bCursor )
{
	if(bCursor)
	{
		int nPos = m_edtEquation.CharFromPos(m_edtEquation.GetCaretPos());
		CString strTemp;
		CString strEq= GetEquation();
		strTemp.Format(_T("%s%s%s"), strEq.Left(nPos), str, strEq.Right(strEq.GetLength() - nPos));
		SetEquation(strTemp);
		m_edtEquation.SetSel(nPos+str.GetLength(), nPos+str.GetLength());
	}
	else
	{
		CString strTemp;
		CString strOrg;
		m_edtEquation.GetWindowText(strOrg);
		strTemp.Format(_T("%s%s"), strOrg, str);
		SetEquation(strTemp);
		m_edtEquation.SetSel(strTemp.GetLength(), strTemp.GetLength());
	}
}

void CCMWindPressureFunctionHelper::ResetXYZ()
{
	CWnd* pWndX = GetDlgItem(IDC_WINDP_FUNC_HELP_X);
	CWnd* pWndY = GetDlgItem(IDC_WINDP_FUNC_HELP_Y);
	CWnd* pWndZ = GetDlgItem(IDC_WINDP_FUNC_HELP_Z);

	if(pWndX == NULL || pWndY == NULL || pWndZ == NULL) return;
	if(::IsWindow(pWndX->GetSafeHwnd()) == FALSE)       return;
	if(::IsWindow(pWndY->GetSafeHwnd()) == FALSE)       return;
	if(::IsWindow(pWndZ->GetSafeHwnd()) == FALSE)       return;

	if(m_pParentDlg->IsRectCoordSys())
	{
		pWndX->SetWindowText(_T("X"));
		pWndY->SetWindowText(_T("Y"));
		pWndZ->SetWindowText(_T("Z"));
	}
	else
	{
		pWndX->SetWindowText(_T("R"));
		pWndY->SetWindowText(_T("TH"));
		pWndZ->SetWindowText(_T("Z"));
	}
}

void CCMWindPressureFunctionHelper::ReplaceStringEqua( const CString& str )
{
	SetEquation(str);
}

CString CCMWindPressureFunctionHelper::GetEquation()
{
	CString strTemp;
	m_edtEquation.GetWindowText(strTemp);
	m_edtEquation.SetFocus();
	return strTemp;
}

void CCMWindPressureFunctionHelper::SetEquation( const CString& strInput, BOOL bPush )
{
	if(bPush)
		BufPushStr(GetEquation());

	m_edtEquation.SetWindowText(strInput);
	m_edtEquation.SetFocus();
}

void CCMWindPressureFunctionHelper::BufClear(BOOL bEnableDisable)
{
	for(int i = 0 ; i < 10; i++)
	{
		m_strBufRU[i].Empty();
	}
	
	m_nBufStartIdx = 0;
	m_nBufCntUndo  = 0;
	m_nBufSize     = 0;
	m_nBufCurIdx   = 0;  

	if(bEnableDisable)
		EnableDisableRU();
}

void CCMWindPressureFunctionHelper::BufPushStr(const CString& strPush)
{
	m_strBufRU[m_nBufCurIdx++] = strPush;
	if(m_nBufCurIdx >= 10)    m_nBufCurIdx = 0;
	if(m_nBufSize < 10)       m_nBufSize++;
	else
	{
		m_nBufSize = 10;
		if(++m_nBufStartIdx == 10) m_nBufStartIdx = 0;
	}
	m_nBufCntUndo = 0;
	EnableDisableRU();
}

BOOL CCMWindPressureFunctionHelper::BufPopStr(CString& strPop)
{
	if(m_nBufSize > 0)
	{
		m_nBufSize--;
		if(--m_nBufCurIdx < 0) m_nBufCurIdx = 9;
		strPop = m_strBufRU[m_nBufCurIdx];
		m_nBufCntUndo++;
	}
	else
	{
		BufClear();
		return FALSE;
	}
	
	EnableDisableRU();
	return TRUE;  
}

BOOL CCMWindPressureFunctionHelper::IsEnableUndo()
{
	if(m_nBufSize > 0)
		return TRUE;
	return FALSE;  
}

BOOL CCMWindPressureFunctionHelper::IsEnableRedo()
{
	if(m_nBufCntUndo > 0)
		return TRUE;
	return FALSE;  
}

BOOL CCMWindPressureFunctionHelper::DoRedo()
{
	if(IsEnableRedo())
	{
		if(++m_nBufCurIdx >= 10) m_nBufCurIdx = 0;
		CString strTemp = m_strBufRU[m_nBufCurIdx];
		if(++m_nBufSize > 10)
		{
			ASSERT(0);
			return FALSE;
		}

		m_nBufCntUndo--;
		SetEquation(strTemp, FALSE);
		return TRUE;
	}
	return FALSE;
}

BOOL CCMWindPressureFunctionHelper::DoUndo()
{
	CString strTemp;

	if(IsEnableUndo())
	{
		if(!BufPopStr(strTemp))
		{
			return FALSE;
		}
		
		SetEquation(strTemp, FALSE);
		return TRUE;
	}

	return FALSE;
}

void CCMWindPressureFunctionHelper::EnableDisableRU()
{
	CWnd* pUndoBtn = GetDlgItem(IDC_WINDP_FUNC_HELP_UNDO);
	CWnd* pRedoBtn = GetDlgItem(IDC_WINDP_FUNC_HELP_REDO);
	
	if(pUndoBtn == NULL || pRedoBtn == NULL)         return;
	if(::IsWindow(pUndoBtn->GetSafeHwnd()) == FALSE) return;
	if(::IsWindow(pRedoBtn->GetSafeHwnd()) == FALSE) return;
	
	if(IsEnableUndo()) pUndoBtn->EnableWindow(TRUE);
	else               pUndoBtn->EnableWindow(FALSE);
	
	if(IsEnableRedo()) pRedoBtn->EnableWindow(TRUE);
	else               pRedoBtn->EnableWindow(FALSE);
}

BOOL CCMWindPressureFunctionHelper::GetCurValue(double& dValue)
{
	dValue = 0.0;
	CString strTemp = GetEquation();
	MyParser myParser;
	
	try
	{
		if(!myParser.ParsingMathFuncByNumberArg(strTemp))
			throw 0;
		
		if(myParser.GetNumberData(dValue))
		{
			return TRUE;
		}
	}
	catch(...)
	{
		return FALSE;
	}
	return FALSE;  
}

void CCMWindPressureFunctionHelper::ResetEquation()
{
	CString strTemp;
	double dVal;
	if(GetCurValue(dVal))
	{
		strTemp.Format(_T("%g"),dVal);
		SetEquation(strTemp);
		m_edtEquation.SetSel(strTemp.GetLength(), strTemp.GetLength());
	}
}

void CCMWindPressureFunctionHelper::OnCmdToolSqrt()
{
	CString strTemp;
	CString strOrg = GetEquation();
	strTemp.Format(_T("%s%s%s"), _T("sqrt("), strOrg, _T(")"));
	SetEquation(strTemp);
	ResetEquation(); 
}

void CCMWindPressureFunctionHelper::OnCmdToolPM()
{
	CString strTemp;
	CString strOrg = GetEquation();
	strTemp.Format(_T("%s%s%s"), _T("-1.0*("), strOrg, _T(")"));
	SetEquation(strTemp);
	ResetEquation();  
}

void CCMWindPressureFunctionHelper::OnCmdToolInv()
{
	CString strTemp;
	CString strOrg = GetEquation();
	strTemp.Format(_T("%s%s%s"), _T("1/("), strOrg, _T(")"));
	SetEquation(strTemp);
	ResetEquation();
}

void CCMWindPressureFunctionHelper::OnCmdToolBackSpace()
{
	CString strOrg;
	m_edtEquation.GetWindowText(strOrg);
	int dLength = strOrg.GetLength();
	if(dLength > 0)
	{
		ReplaceStringEqua(strOrg.Left(dLength - 1));    
		m_edtEquation.SetSel(dLength - 1, dLength - 1);
	}
}

void CCMWindPressureFunctionHelper::OnCmdNUM_X()
{
	if(m_pParentDlg->IsRectCoordSys()) AddStringEqua(_T("X"));
	else                               AddStringEqua(_T("R"));
}

void CCMWindPressureFunctionHelper::OnCmdNUM_Y()
{
	if(m_pParentDlg->IsRectCoordSys()) AddStringEqua(_T("Y"));
	else                               AddStringEqua(_T("TH"));
}

void CCMWindPressureFunctionHelper::SetTextSpecial()
{
	GetDlgItem(IDC_WINDP_FUNC_HELP_TOOL_BACKSP)	->SetWindowText(_LS(IDS_CMD_FUNCTION_SPCHAR_BACKSP));
	GetDlgItem(IDC_WINDP_FUNC_HELP_TOOL_PM)		->SetWindowText(_LS(IDS_CMD_FUNCTION_SPCHAR_PM));
	GetDlgItem(IDC_WINDP_FUNC_HELP_TOOL_SQRT)	->SetWindowText(_LS(IDS_CMD_FUNCTION_SPCHAR_SQRT));
	GetDlgItem(IDC_WINDP_FUNC_HELP_SQ)			->SetWindowText(_LS(IDS_CMD_FUNCTION_SPCHAR_SQUARE));
	GetDlgItem(IDC_WINDP_FUNC_HELP_CUBIC)		->SetWindowText(_LS(IDS_CMD_FUNCTION_SPCHAR_CUBIC));
}
