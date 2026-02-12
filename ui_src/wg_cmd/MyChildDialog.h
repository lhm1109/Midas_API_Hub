#if !defined(AFX_MYCHILDDIALOG_H__E5FC9381_9B84_11D3_AA8A_0080AD78AAC8__INCLUDED_)
#define AFX_MYCHILDDIALOG_H__E5FC9381_9B84_11D3_AA8A_0080AD78AAC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyChildDialog.h : header file
//

//struct T_PREFERENCE;
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_base\wg_base_ChildDialog.h"

#include "SpfcBaseUtil.h"
/////////////////////////////////////////////////////////////////////////////
// CMyChildDialog dialog

#include "HeaderPre.h"
namespace ChildDlg
{
	constexpr int STEP_COEFF = 4;
	constexpr int PERIOD_COEFF = 2;
};

class __MY_EXT_CLASS__ CMyChildDialog : public CChildDialog
{
// Construction
public:  
	CString m_strFuncName;
	CString m_sDescript;
	virtual void UpdateDampingWnd(CWnd* pDampingWnd) { };
	virtual void MakeSpectrumData(BOOL bOnlyCalc=FALSE);
	virtual void MakePOSpectrumData(double dDamping,double SRa,double SRv,CArray <double,double>& aPeriod,CArray <double,double>& aAccel);
	virtual void SetPushOverData(BOOL bInit=TRUE);  //Pushover Curve의 RS 생성시 초기값 세팅하는 함수
	CArray <double,double>* m_parPeriod,*m_parAccel;
	CWnd* m_pWndError;
	virtual BOOL ISVALID();
	virtual void SetData();
	virtual void GetData();
	virtual void SetDefault();
	virtual UINT GetSpfcK();

	CMyChildDialog(UINT resID,CWnd* pParent /*=NULL*/, std::shared_ptr<CSpfcBaseUtil> pSpfcUtil = nullptr); // standard constructor
	virtual ~CMyChildDialog(); // standard destructor
	BOOL IsValid_d(CEdit* pEdt,double dMin,double dMax,double dDefValue);
	BOOL IsValid_n(CEdit* pEdt,int nMin,int nMax,int nDefValue);
	BOOL IsValidCmb_d(CComboBox* pEdt,double dMin,double dMax,double dDefValue);
	BOOL IsValidCmb_n(CComboBox* pEdt,int nMin,int nMax,int nDefValue);
	
	// MNET:2138-GSJI-20060526
	// RS Design Code의 데이터를 저장하기 위한 함수
	virtual BOOL Dlg2Data() { return TRUE; }
	virtual void Data2Dlg() {}
	virtual void Initialize() {}
	virtual void Dlg2UtilData() {};
	virtual void SaveDlgData(){}
	virtual void ShowDlgData(){}
	virtual void EndNotClose(){} // for Pushover Curve EC8

	virtual BOOL GetGravity_UserDefinedSpec(double& dGravity);

	void SetCodeType(int nCodeType); // For IBC2009/2012
	void SetSpfcResult();
	
// Dialog Data
	//{{AFX_DATA(CMyChildDialog)

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyChildDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	std::shared_ptr<CSpfcBaseUtil> m_pSpfcBaseUtil;
	int m_nCodeType;
	int m_nStepCoeff;		// 삭제예정
	int m_nPeriodCoeff;		// 삭제예정
	// Generated message map functions
	//{{AFX_MSG(CMyChildDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCHILDDIALOG_H__E5FC9381_9B84_11D3_AA8A_0080AD78AAC8__INCLUDED_)
