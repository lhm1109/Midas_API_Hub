#if !defined(AFX_STLDPRESDLG2_H__8D16092B_250B_47D3_AEC0_BF6233C3BD12__INCLUDED_)
#define AFX_STLDPRESDLG2_H__8D16092B_250B_47D3_AEC0_BF6233C3BD12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldPresDlg2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStldPresDlg2 dialog
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"

class CStldPresDlg2 : public CMenuBarChildDlg
{
// Construction
	CBitmap* m_pBitmap;
public:
	void InitCtrls();
	CStldPresDlg2(CWnd* pParent = NULL);   // standard constructor
	~CStldPresDlg2();
	virtual void Execute() { OnTmExecute(); }
	void CheckMode(BOOL bCheck);
	void SetPresUnit();
	void SetEdgeFaceCobx();
	void SetDirCobx();
	void SelectionCtrlMan();
	void ProjectionCtrlMan();
	void LoadCtrlMan();
	void OPCtrlMan();
	void ChangeBitmap();
	int  GetElemTypeExternal();
	void GetWCSVectorVal(double& VX, double& VY, double& VZ);
	void GetElemByType(CArray<UINT, UINT> &aSelKey,CArray<int,int>&arFaceOrEdge);

// Dialog Data
	//{{AFX_DATA(CStldPresDlg2)
	enum { IDD = IDD_TM_STLD_PRES2 };
	mit::frx::MComboBox	m_CobxEType;
	CEditUnit		m_wndPres1;
	CEditUnit		m_wndPres2;
	CEditUnit		m_wndPres3;
	CEditUnit		m_wndPres4;
	CTextUnit		m_wndPresUnit4;
	CTextUnit		m_wndPresUnit3;
	CTextUnit		m_wndPresUnit2;
	CTextUnit		m_wndPresUnit1;
	CMouseEdit	m_EditVector;
	mit::frx::MComboBox		m_CobxFaceEdge;
	mit::frx::MComboBox		m_CobxDir;
	CCobxLdgr		m_CobxLdgr;
	CSelectLC		m_CobxLC;
	mit::frx::MComboBox		m_CobxPslt;
	int   m_nInputType;  // 0:Load case, 1:Load Type
	int		m_nOption;
	int		m_nProjection;
	int		m_nSelection;
	int		m_nLoadType;

	mit::frx::MillustViewer m_wndElementTypePicture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldPresDlg2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStldPresDlg2)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmSubLoadType();
	afx_msg void OnTmOption();
	afx_msg void OnSelchangeTmDirCombo();
	afx_msg void OnSelchangeTmElemType();
	afx_msg void OnSelchangeTmPreedgeCombo();
	afx_msg void OnSelchangeTmLType();
	afx_msg void OnTmSelectType();
	afx_msg void OnTmBtnLc();
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnTmClose();
	afx_msg void OnTmExecute();
	afx_msg void OnInputTypeRdo();
	afx_msg void OnTmBtnLType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CArray<UINT,UINT> m_aCtrlLCaseInputEnable;
	CArray<UINT,UINT> m_aCtrlLTypeInputEnable;
	void ResetPsltCmb();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDPRESDLG2_H__8D16092B_250B_47D3_AEC0_BF6233C3BD12__INCLUDED_)
