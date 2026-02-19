#if !defined(__DGN_CON_CODE_NEW_CHILD_8_DLG_H__)
#define __DGN_CON_CODE_NEW_CHILD_8_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCodeNewChild_8_Dlg.h.h : header file
//
#include "..\mit_frx\MButton.h"

#include "DconChildDialog.h"
#include "HeaderPre.h"

class CDBDoc;
class CDgnDataCtrl;
/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild8Dlg dialog
class __MY_EXT_CLASS__ CDgnConCodeNewChild8Dlg : public CDconChildDialog
{
// Construction
public:
	CDgnConCodeNewChild8Dlg(CDgnConCodeNewDlg* pParent = NULL, T_DCON_D* pData=NULL, BOOL bExistData=FALSE);   // standard constructor
    ~CDgnConCodeNewChild8Dlg();

// Dialog Data
	//{{AFX_DATA(CDgnConCodeNewChild8Dlg)
	enum { IDD = IDD_DGN_CON_CODE_CHILD8_DLG };
	
	double  m_dMrft;
	double  m_dTrft;
	BOOL m_bTorsionDesign;
	BOOL m_bShowTorsion;

	mit::frx::MButton	  m_btnTorsion;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCodeNewChild8Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	virtual void ChangeDesignCode();
	virtual void UpdateChildData();
	virtual void ResizeChildDlg();
	virtual BOOL Dlg2Data();
protected:
	CArray<UINT, UINT> m_aSafetyLevel;
	CArray<UINT, UINT> m_aEarthquake;
	CArray<UINT, UINT> m_aSlabType;
	CArray<UINT, UINT> m_aTorsionDesign;
	CArray<UINT, UINT> m_aMomentRedistr;

	int m_nTorsionGap;

    CDgnDataCtrl* m_pDgnDataCtrl;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConCodeNewChild8Dlg)
	void InitCtrl();
	void SetShowTorsion(const CString& strCode);
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnRcctrlSpecial();
	afx_msg void OnChangeTorsion();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGN_CON_CODE_NEW_CHILD_8_DLG_H__)
