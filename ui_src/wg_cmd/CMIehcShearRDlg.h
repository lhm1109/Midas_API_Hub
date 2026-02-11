#pragma once

/////////////////////////////////////////////////////////////////////////////
// CCMIehcShearRDlg 
#include "..\wg_db\wg_db_EditUnit.h"

#include "HeaderPre.h"
enum { D_IEHC_R_GLOBAL, D_IEHC_R_USER };
class __MY_EXT_CLASS__ CCMIehcShearRDlg : public CDialogMove
{
public:
	CCMIehcShearRDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMIehcShearRDlg();

public:
	enum { IDD = IDD_CMD_IEHC_SHEAR_PROP_DLG };

	void   SetGlobal(BOOL bGlobal) { m_bGlobalMode = bGlobal; }
	void   SetData(int nUseOption, double dR) { m_nUseOption=nUseOption; m_dR=dR; }
	int    GetUseOption() { return m_nUseOption; }
	double GetShearRValue() { return m_dR; }

protected:
	CEditUnit m_edtR;

	BOOL   m_bGlobalMode; // TRUE:Inelastic Properties Control Data에서 사용, FALSE:Inelastic Hinge Properties에서 사용
	int    m_nUseOption ; // D_IEHC_R_GLOBAL:Use option of IEHC, D_IEHC_R_GLOBAL:User Defined
	double m_dR;
	CBitmap* m_pBitmap;

	CArray<UINT,UINT> m_aCtrlIDNotGlobal;
	CArray<UINT,UINT> m_aCtrlIDMoveUp;
	CArray<UINT,UINT> m_aCtrlIDUseOptRdo;

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	void InitCtrl();
	void Data2Dlg();
	void Dlg2Data();
	void CtrlManager();
		// Generated message map functions
	//{{AFX_MSG(CCMIehcShearRDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

