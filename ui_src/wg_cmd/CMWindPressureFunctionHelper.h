#if !defined(__CM_WIND_FUNC_HELPER_DLG_H__)
#define __CM_WIND_FUNC_HELPER_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CMDlgBase.h"
#include "..\MIT_frx\MEdit.h"

#include "HeaderPre.h"      /////////
// CMWindPressureVelocityDefDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureFunctionAddModDlg dialog

using namespace mit::frx;
class CCMWindPressureFunctionAddModDlg;
class __MY_EXT_CLASS__ CCMWindPressureFunctionHelper : public CCMDlgBase
{
// Construction
public:
	CCMWindPressureFunctionHelper(CWnd* pParent);   // standard constructor
	virtual ~CCMWindPressureFunctionHelper();

	enum { IDD = IDD_CMD_DEF_WIND_FUNCTION_HELPER };
	
	void ResetXYZ();
protected:
	MEdit m_edtEquation;
	CCMWindPressureFunctionAddModDlg* m_pParentDlg;
	CString m_strBufRU[10];
	int     m_nBufStartIdx;
	int     m_nBufCntUndo;
	int     m_nBufCurIdx;
	int     m_nBufSize;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	void BufClear(BOOL bEnableDisable = TRUE);
	void BufPushStr(const CString& strPush); //바뀌기전에 사용.
	BOOL BufPopStr(CString& strPop);
	
	void AddStringEqua(const CString& str, BOOL bCursor = TRUE);
	void AddStringEqua_Func(const CString& str);
	void ReplaceStringEqua(const CString& str);

	BOOL IsEnableUndo();
	BOOL IsEnableRedo();
	BOOL DoRedo();
	BOOL DoUndo();
	void EnableDisableRU();

	CString GetEquation();
	void SetEquation(const CString& strInput, BOOL bPush = TRUE);
	void ResetEquation();
	BOOL GetCurValue(double& dValue);
	void SetTextSpecial();
	
	afx_msg void OnCmdApply();
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdUndo() { DoUndo(); EnableDisableRU(); }
	afx_msg void OnCmdRedo() { DoRedo(); EnableDisableRU(); }

	afx_msg void OnCmdNUM_X();
	afx_msg void OnCmdNUM_Y();
	afx_msg void OnCmdNUM_Z()   { AddStringEqua("Z");  }
	afx_msg void OnCmdNUM_pi()  { AddStringEqua("pi"); }
	afx_msg void OnCmdNUM_e()   { AddStringEqua("e");  }
	afx_msg void OnCmdNUM_0()   { AddStringEqua("0");  }
	afx_msg void OnCmdNUM_1()   { AddStringEqua("1");  }
	afx_msg void OnCmdNUM_2()   { AddStringEqua("2");  }
	afx_msg void OnCmdNUM_3()   { AddStringEqua("3");  }
	afx_msg void OnCmdNUM_4()   { AddStringEqua("4");  }
	afx_msg void OnCmdNUM_5()   { AddStringEqua("5");  }
	afx_msg void OnCmdNUM_6()   { AddStringEqua("6");  }
	afx_msg void OnCmdNUM_7()   { AddStringEqua("7");  }
	afx_msg void OnCmdNUM_8()   { AddStringEqua("8");  }
	afx_msg void OnCmdNUM_9()   { AddStringEqua("9");  }
	afx_msg void OnCmdNUM_dot() { AddStringEqua(".");  }
	
	afx_msg void OnCmdToolClear() { ReplaceStringEqua(""); }
	afx_msg void OnCmdToolBackSpace();
	afx_msg void OnCmdToolPM();
	afx_msg void OnCmdToolSqrt();
	afx_msg void OnCmdToolInv();
	afx_msg void OnCmdToolDivide() { AddStringEqua("/"); }
	afx_msg void OnCmdToolMult()   { AddStringEqua("*"); }
	afx_msg void OnCmdToolMinus()  { AddStringEqua("-"); }
	afx_msg void OnCmdToolPlus()   { AddStringEqua("+"); }
	afx_msg void OnCmdToolGStart() { AddStringEqua("("); }
	afx_msg void OnCmdToolGEnd()   { AddStringEqua(")"); }
	afx_msg void OnCmdToolEqual()  { ResetEquation();    }

	afx_msg void OnCmdFunc_Sin()   { AddStringEqua_Func("sin");  }
	afx_msg void OnCmdFunc_Cos()   { AddStringEqua_Func("cos");  }
	afx_msg void OnCmdFunc_Tan()   { AddStringEqua_Func("tan");  }
	afx_msg void OnCmdFunc_aSin()  { AddStringEqua_Func("asin"); }
	afx_msg void OnCmdFunc_aCos()  { AddStringEqua_Func("acos"); }
	afx_msg void OnCmdFunc_aTan()  { AddStringEqua_Func("atan"); }
	afx_msg void OnCmdFunc_Sinh()  { AddStringEqua_Func("sinh"); }
	afx_msg void OnCmdFunc_Cosh()  { AddStringEqua_Func("cosh"); }
	afx_msg void OnCmdFunc_Tanh()  { AddStringEqua_Func("tanh"); }
	afx_msg void OnCmdFunc_Log()   { AddStringEqua_Func("log");  }
	afx_msg void OnCmdFunc_Ln()    { AddStringEqua_Func("ln");   }
	afx_msg void OnCmdFunc_Exp()   { AddStringEqua_Func("exp");  }
	afx_msg void OnCmdFunc_Deg()   { AddStringEqua_Func("deg");  }
	afx_msg void OnCmdFunc_Rad()   { AddStringEqua_Func("rad");  }
	afx_msg void OnCmdFunc_Sqrt()  { AddStringEqua_Func("sqrt"); }
	afx_msg void OnCmdFunc_Square(){ AddStringEqua_Func("square");   }
	afx_msg void OnCmdFunc_Cubic() { AddStringEqua_Func("cubic");   }


	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"      /////////

#endif
