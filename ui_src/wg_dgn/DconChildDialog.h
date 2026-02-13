#if !defined(__DGN_CON_CODE_CHILD_DLG_H__)
#define __DGN_CON_CODE_CHILD_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DconChildDialog.h : header file
//
#include "..\wg_base\wg_base_ChildDialog.h"

/////////////////////////////////////////////////////////////////////////////
// DconChildDialog dialog

#include "HeaderPre.h"

class CDgnConCodeNewDlg;
class CDgnDataCtrl;
class __MY_EXT_CLASS__ CDconChildDialog : public CChildDialog
{
// Construction
public:
	CDconChildDialog(UINT resID,CDgnConCodeNewDlg* pParent /*=NULL*/, T_DCON_D* pData, BOOL bExistData/*=FALSE*/); // standard constructor
	virtual ~CDconChildDialog(); // standard destructor
	// MNET:2138-GSJI-20060526
	// RS Design Code의 데이터를 저장하기 위한 함수
	virtual BOOL Dlg2Data();
	virtual void Data2Dlg() {}
	virtual void Initialize() {}
//   virtual void SaveDlgData(){}
//   virtual void ShowDlgData(){}
// 	virtual void EndNotClose(){} // for Pushover Curve EC8

	void SetCodeType(int nCodeType); // For IBC2009/2012
	void SetDesignCode(const CString& strDgnCode);
	virtual void UpdateChildData();
	virtual void ResizeChildDlg();
	virtual void ChangeDesignCode() {};
// Dialog Data
	//{{AFX_DATA(CDconChildDialog)

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
protected:
	int GetDataStatus(const CString& strCode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDconChildDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int		  m_iColmMethod;
	int		  m_iSafeLevel;
	int		  m_iEqGrade;
	int		  m_iPmCurve;
	int     m_iEqCategory;
	int     m_iSlabType;
	int     m_iFrameType;
	int		  m_iDuctility;

	CString m_strDesinCode;
	BOOL	m_bSpecialEQ;
	BOOL  m_bExistData;

	T_DCON_D* m_pData;

protected:
	CDgnConCodeNewDlg* m_pDconParent;
    CDgnDataCtrl* m_pDgnDataCtrl;
	// Generated message map functions
	//{{AFX_MSG(CDconChildDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGN_CON_CODE_CHILD_DLG_H__)
