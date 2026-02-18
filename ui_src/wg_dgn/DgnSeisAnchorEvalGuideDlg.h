#if !defined(AFX_DgnSeisAnchorEvalGuideDlg_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_)
#define AFX_DgnSeisAnchorEvalGuideDlg_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorEvalGuideDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "DgnSeisAnchorEvalEditBtn.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalGuideDlg dialog

class CDgnSeisAnchorEvalGuideDlg : public CDialogMove
{
// Construction
public:
	CDgnSeisAnchorEvalGuideDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnSeisAnchorEvalGuideDlg();

public:
	void SetDataSource(T_ANEV_D* pDataSrc);
	void SetDataToView();
	void ResetDlgPos(); // 듀얼모니터에서는 안따라옴...
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorEvalGuideDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_EVAL_GUIDE_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorEvalGuideDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorEvalGuideDlg)
	virtual BOOL OnInitDialog();	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//	afx_msg void OnCmdSectCentroidBtn();
	
	CDgnSeisAnchorEvalEditBtn m_View;
	T_ANEV_D* m_pData;
	BOOL m_bInitDone;
	CWnd* m_pParent;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnSeisAnchorEvalGuideDlg_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_)
