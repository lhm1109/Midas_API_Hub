#if !defined(__ETCACTLDLG_MEC_H__)
#define __ETCACTLDLG_MEC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EtcActlDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CEtcActlDlg_MEC dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CEtcActlDlg_MEC : public CDialogMove
{
// Construction
public:
	CEtcActlDlg_MEC(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEtcActlDlg_MEC)
	enum { IDD = IDD_ETC_ACTL_MEC };

	BOOL	m_bAutoCons1;
	BOOL	m_bAutoCons2;
	BOOL	m_bConsiderSecf;
	BOOL  m_bConsiderRbar;
	BOOL  m_bConsiderSectOffset;
	BOOL	m_bConsiderSCS;
	BOOL	m_bConsiderTDF;
	BOOL  m_bTransferReac;
	BOOL  m_bChangeLcAxOfTapSect;
	BOOL  m_bAutoWallConstraint;
	BOOL  m_bCalculateBeamStresses;
	BOOL	m_bDKFormulaShellElem;
	BOOL    m_bClassicalFormulaSolid;
	int     m_nBeamSectionChange;
	BOOL m_bConsiderCable;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	void InitDialogData();
	void AlignControls();

	T_ACTL_D m_Data;
protected:
	CDBDoc* m_pDoc;

	CArray<UINT,UINT> m_arIncludeType;
	CArray<UINT,UINT> m_arExcludeType;

	// Generated message map functions
	//{{AFX_MSG(CEtcActlDlg_MEC)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdTrussElemBtn();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetCtrlText() const;
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ETCACTLDLG_MEC_H__)
