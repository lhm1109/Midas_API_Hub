#if !defined(__DGNSEISMPHIIDEALGUIDEDLG_H__)
#define __DGNSEISMPHIIDEALGUIDEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisMPhiIdealGuideDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\mit_frx\MillustViewer.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiIdealGuideDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSeisMPhiIdealGuideDlg : public CDialogMove
{
// Construction
public:
	CDgnSeisMPhiIdealGuideDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnSeisMPhiIdealGuideDlg();
	
	//----------------------------------------------------------------------
	// Interface
public:
	void SetParamData(int nIdealOpt) 
	{ m_nIdealOpt = nIdealOpt; }

public:
// Dialog Data
	//{{AFX_DATA(CDgnSeisMPhiIdealGuideDlg)
	enum { IDD = IDD_DGN_SEIS_MPHI_IDEAL_GUIDE_DLG };
	
	int m_nIdealOpt;
	//}}AFX_DATA
	mit::frx::MillustViewer m_wndPicture;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisMPhiIdealGuideDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ChangeBitmap();
	
	/*CBitmap* m_pBitmap;*/

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisMPhiIdealGuideDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSeisMPhiIdealOptRdo();
	afx_msg void OnDgnSeisMPhiIdealCloseBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISMPHIIDEALGUIDEDLG_H__)
