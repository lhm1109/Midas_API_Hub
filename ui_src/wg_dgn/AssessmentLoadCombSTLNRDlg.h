#if !defined(__CMAssessmentLoadCombSTLNRDlg_H__)
#define __CMAssessmentLoadCombSTLNRDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\NotifyCtrl.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "DgnDlgBase.h"

class CAssessmentLoadCombMainGridSTLNR;
class CAssessmentLoadCombSubGridSTLNR;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CAssessmentLoadCombSTLNRDlg : public CDgnDlgBase, public CDBUpdateConnector
{
	// Construction
public:
	CAssessmentLoadCombSTLNRDlg(CWnd* pParent = NULL);   // standard constructor
	~CAssessmentLoadCombSTLNRDlg();

	// Dialog Data
	//{{AFX_DATA(CAssessmentLoadCombSTLNRDlg)
	enum { IDD = IDD_DGN_ASSESSMENT_LOADCOMB_STL_NR_DLG };	
	//}}AFX_DATA

	MButton	m_wndAutoGenBtn;
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

	void OnStageChanged();
	void UpdateStld();

	BOOL GetData(T_ALCS_D &AlcsD);	
	void SetData(T_ALCS_K AlcsK);

	void EnableDisableControls();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAssessmentLoadCombSTLNRDlg)

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL

	// Implementation
	
	// Generated message map functions
	//{{AFX_MSG(CAssessmentLoadCombSTLNRDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgClose();
	afx_msg void OnPaint(); 
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnClose();
	afx_msg void PostNcDestroy();	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnCopyToLCom();		
	afx_msg void OnStaticVhlChk();
	afx_msg void OnStaticVhlCmb();
	afx_msg void OnStaticVhlEdt();
	afx_msg void OnDynamicVhlChk();
	afx_msg void OnDynamicVhlCmb();
	afx_msg void OnDynamicVhlEdt();
	afx_msg void OnCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:  
	void SetControl();	
	void Data2Dlg(const T_ALCS_D &AlcsD);
	BOOL Dlg2Data(T_ALCS_D &AlcsD);
	void SetStaticVhlCmb(MComboBox& cobxMv);
	void SetDynamicVhlCmb(MComboBox& cobxMv);
	void SetMainGridModeEdit();
	CAssessmentLoadCombMainGridSTLNR* m_pLoadCombList;
	CAssessmentLoadCombSubGridSTLNR*  m_pContentList;

	CDBDoc* m_pDoc;

	int       m_nLiveType;
	BOOL      m_bStatic;
	MComboBox m_cmbStaticVhl;
	MEdit     m_edtStaticVhl;
	BOOL      m_bDynamic;
	MComboBox m_cmbDynamicVhl;
	MEdit     m_edtDynamicVhl;

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
#endif // !defined(__CMAssessmentLoadCombSTLNRDlg_H__)