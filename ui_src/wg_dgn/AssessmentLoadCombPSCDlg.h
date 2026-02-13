#if !defined(__CMAssessmentLoadCombPSCDlg_H__)
#define __CMAssessmentLoadCombPSCDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\NotifyCtrl.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "DgnDlgBase.h"

class CAssessmentLoadCombMainGrid;
class CAssessmentLoadCombSubGrid;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CAssessmentLoadCombPSCDlg : public CDgnDlgBase, public CDBUpdateConnector
{
	// Construction
public:
	CAssessmentLoadCombPSCDlg(CWnd* pParent = NULL);   // standard constructor
	~CAssessmentLoadCombPSCDlg();

	// Dialog Data
	//{{AFX_DATA(CAssessmentLoadCombPSCDlg)
	enum { IDD = IDD_DGN_ASSESSMENT_LOADCOMB_PSC_DLG };	
	//}}AFX_DATA

	MButton	m_wndAutoGenBtn;
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

	void OnStageChanged();
	void UpdateStld();

	BOOL GetData(T_ALCS_D &AlcsD);	
	void SetData(T_ALCS_K AlcsK);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAssessmentLoadCombPSCDlg)

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL

	// Implementation
	
	// Generated message map functions
	//{{AFX_MSG(CAssessmentLoadCombPSCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgClose();
	afx_msg void OnPaint(); 
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnClose();
	afx_msg void PostNcDestroy();	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnCopyToLCom();		
	afx_msg void OnMovTypeRdo();
	afx_msg void OnCombinedVhlCmb();
	afx_msg void OnCombinedVhlEdt();
	afx_msg void OnSpecialVhlCmb();
	afx_msg void OnSpecialVhlEdt();
	afx_msg void OnCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:  
	void SetControl();	
	void CtrlEnable();	
	void Data2Dlg(const T_ALCS_D &AlcsD);
	BOOL Dlg2Data(T_ALCS_D &AlcsD);
	void SetCombinedVhlCmb(MComboBox& cobxMv);
	void SetSpecialVhlCmb(MComboBox& cobxMv);
	void SetMainGridModeEdit();
	CAssessmentLoadCombMainGrid* m_pLoadCombList;
	CAssessmentLoadCombSubGrid*  m_pContentList;   

	BOOL m_bTB10002_1_05;
	CDBDoc* m_pDoc;

	int   m_nMovType;
	MComboBox m_cmbCombinedVhl;
	MEdit m_edtCombinedVhl;
	MComboBox m_cmbSpecialVhl;
	MEdit m_edtSpecialVhl;

	BOOL m_bSetGridEditMode;

	// resizing
	CArray<UINT, UINT> m_aIDResize;
	CArray<UINT, UINT> m_aIDVectMoveHolResize;
	CArray<UINT, UINT> m_aIDHolMoveVertResize;
	CRect m_rectDlg;
	CArray<CRect, CRect> m_aRectResize;
	CArray<CRect, CRect> m_aRectVertMoveHorResize;
	CArray<CRect, CRect> m_aRectHorMoveVertResize;
	CRect m_rectClose;
};
#include "HeaderPost.h"    

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(__CMAssessmentLoadCombPSCDlg_H__)