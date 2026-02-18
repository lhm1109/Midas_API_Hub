#if !defined(AFX_DgnSeisAnchorEvalGuide2Dlg_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_)
#define AFX_DgnSeisAnchorEvalGuide2Dlg_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorEvalGuide2Dlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "DgnSeisAnchorEvalEditBtn.h"
#include "..\mit_frx\MillustViewer.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalGuide2Dlg dialog

class CDgnSeisAnchorEvalGuide2Dlg : public CDialogMove
{
// Construction
public:
	CDgnSeisAnchorEvalGuide2Dlg(CWnd* pParent = NULL, int nType=0);   // standard constructor
	~CDgnSeisAnchorEvalGuide2Dlg();

public:
	void SetDataSource(T_ANEV_D* pDataSrc);
	void SetDataToView();
	void ResetDlgPos(); // 듀얼모니터에서는 안따라옴...
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorEvalGuide2Dlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_EVAL_GUIDE2_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorEvalGuide2Dlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorEvalGuide2Dlg)
	virtual BOOL OnInitDialog();	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSeisAnchorEvalGuideRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//	afx_msg void OnCmdSectCentroidBtn();

	void ChangeBitmap();

	//CDgnSeisAnchorEvalEditBtn m_View;
	T_ANEV_D* m_pData;
	BOOL m_bInitDone;
	CWnd* m_pParent;

	mit::frx::MillustViewer m_wndPicture;

	int       m_nViewType;
	int       m_nGuideType; //0:Anchor Evaluation Guide   1:Coping Guide
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnSeisAnchorEvalGuide2Dlg_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_)
