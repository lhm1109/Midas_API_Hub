#if !defined(AFX_STLDPRESAREADLG_H__8D16092B_250B_47D3_AEC0_BF6233C3BD12__INCLUDED_)
#define AFX_STLDPRESAREADLG_H__8D16092B_250B_47D3_AEC0_BF6233C3BD12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldPresAreaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStldPresAreaDlg dialog
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "..\wg_db\wg_db_CobxLagr.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;
class _EType
{ 
public:
	int nFaceOrEdge;
	int nLoadType;
	int nWhoAmI;
};

class CStldPresAreaDlg : public CMenuBarChildDlg
{
	// Construction
public:
	void InitCtrls();
	CStldPresAreaDlg(CWnd* pParent = NULL);   // standard constructor
	~CStldPresAreaDlg();
	virtual void Execute() { OnTmExecute(); }
	void ProjectionCtrlMan();
	void OPCtrlMan();
	void GetWCSVectorVal(double& VX, double& VY, double& VZ);
	void GetElemByType(CArray<UINT, UINT> &aSelKey,CArray<int,int>&arFaceOrEdge);
	void SetDirCobx();

	// Dialog Data
	//{{AFX_DATA(CStldPresAreaDlg)
	enum { IDD = IDD_TM_STLD_PRES_AREA };
	CMouseEdit	m_EditVector;
	MComboBox	m_CobxDir;
	CCobxLdgr	m_CobxLdgr;
	CSelectLC	m_CobxLC;
	MComboBox	m_CobxPslt;
	CSelectEdit m_edElemList;
	CCobxLagr   m_cobxLagr;
	int		m_nInputType;  // 0:Load case, 1:Load Type
	int		m_nOption;
	int		m_nProjection;
	int		m_nLoadType;
	int		m_nElementType; // 0:Frame, 1:Planar
	int		m_nSelection; // 0:Group, 1:Element
	T_LAGR_K  m_LagrK;
	T_ARPR_D  m_Data;
	mit::frx::MillustViewer m_wndPicture;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldPresAreaDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	BOOL Dlg2Data();
	BOOL Data2Dlg();    
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStldPresAreaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmSubLoadType();
	afx_msg void OnTmOption();
	afx_msg void OnSelchangeTmPreedgeCombo();
	afx_msg void OnSelchangeTmLType();
	afx_msg void OnTmBtnLc();
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnTmClose();
	afx_msg void OnTmExecute();
	afx_msg void OnInputTypeRdo();
	afx_msg void OnTmBtnLType();
	afx_msg void OnCmdTargetChanged();
	afx_msg void OnSelChangeLagr();
	afx_msg void OnCmdBtnLoadAreaGroup();
	afx_msg void OnSelChangeDir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL GetPartionedData(T_ARPR_D& ArprD_org, CArray<T_ARPR_D,T_ARPR_D&>& aArprD);
	void ResetPsltCmb();

	CArray<UINT,UINT> m_aCtrlLCaseInputEnable;
	CArray<UINT,UINT> m_aCtrlLTypeInputEnable;
	CMouseEdit	m_edtInnerPt;
	CTextUnit	m_untInnerPt;
	CEditUnit   m_edtP1;
	CTextUnit	m_untP1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDPRESAREADLG_H__8D16092B_250B_47D3_AEC0_BF6233C3BD12__INCLUDED_)
