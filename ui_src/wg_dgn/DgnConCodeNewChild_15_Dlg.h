#pragma once
#include "DconChildDialog.h"

class CDBDoc;
class CDgnDataCtrl;
class CDgnConCodeNewChild15Dlg : public CDconChildDialog
{
// Construction
public:
	CDgnConCodeNewChild15Dlg(CDgnConCodeNewDlg* pParent = NULL, T_DCON_D* pData=NULL, BOOL bExistData=FALSE);   // standard constructor
    ~CDgnConCodeNewChild15Dlg();

	enum { IDD = IDD_DGN_CON_CODE_CHILD15_DLG };
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCodeNewChild15Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	virtual void ChangeDesignCode();
	virtual void UpdateChildData();
	virtual void ResizeChildDlg();
	virtual BOOL Dlg2Data();
protected:
	CArray<UINT, UINT> m_aSeismicID;

	BOOL m_bConsiderPhi;
	BOOL m_bStaticallyDeterminable;
	BOOL m_bCreep;
	BOOL m_bCosiderSP14_1330_2018;
	BOOL m_bIncludeSeismics;
	BOOL m_bNonlinearCalculation;
	int m_nIntensityDegree;
	int m_nPMCurveMethod;

    CDgnDataCtrl* m_pDgnDataCtrl;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConCodeNewChild15Dlg)
	void InitCtrl();
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSpSeismicChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

