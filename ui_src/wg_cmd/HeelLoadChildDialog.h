#if !defined(AFX_HeelLoadChildDialog_H__E5FC9381_9B84_11D3_AA8A_0080AD78AAC8__INCLUDED_)
#define AFX_HeelLoadChildDialog_H__E5FC9381_9B84_11D3_AA8A_0080AD78AAC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeelLoadChildDialog.h : header file
//

//struct T_PREFERENCE;

#define PRD_ST 0.0
#define PRD_ED 6.
#define PRD_NUM 100

#include "..\wg_base\wg_base_ChildDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CHeelLoadChildDialog dialog
//#include "..\wg_base\wg_base_childdialog.h"
class CHeelLoadChildDialog : public CChildDialog
{
// Construction
public:
	CString m_strFuncName;
	CString m_sDescript;
	virtual void MakeSpectrumData();
	CArray <double,double>* m_parPeriod,*m_parAccel;
	CWnd* m_pWndError;
	virtual BOOL ISVALID();
	virtual void SetData();
	virtual void GetData();
	virtual void SetDefault();
	CHeelLoadChildDialog(UINT resID,CWnd* pParent /*=NULL*/); // standard constructor
	virtual ~CHeelLoadChildDialog(); // standard destructor
	BOOL IsValid_d(CEdit* pEdt,double dMin,double dMax,double dDefValue);
	BOOL IsValid_n(CEdit* pEdt,int nMin,int nMax,int nDefValue);
	BOOL IsValidCmb_d(CComboBox* pEdt,double dMin,double dMax,double dDefValue);
	BOOL IsValidCmb_n(CComboBox* pEdt,int nMin,int nMax,int nDefValue);
	void GetConvertFactor_ForceUnitToCurrent(int Index,double& dMultFact,double& dDivFact);
	void GetConvertFactor_AreaUnitToCurrent(int Index, double& dMultFact, double& dDivFact);
	void GetConvertFactor_StressUnitToCurrent(int IndexF, int IndexL, double& dMultFact, double& dDivFact);
// Dialog Data
	//{{AFX_DATA(CHeelLoadChildDialog)

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeelLoadChildDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHeelLoadChildDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HeelLoadChildDialog_H__E5FC9381_9B84_11D3_AA8A_0080AD78AAC8__INCLUDED_)
