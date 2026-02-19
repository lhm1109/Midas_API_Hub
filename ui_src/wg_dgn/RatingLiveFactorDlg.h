#if !defined(__RATINGLIVEFACTORDLG_H__)
#define __RATINGLIVEFACTORDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingLiveFactorDlg.h : header file
//

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// RatingLiveFactorDlg dialog

#include "HeaderPre.h"


class RatingLiveFactorDlg : public CDialogMove
{
// Construction
public:
	RatingLiveFactorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RatingLiveFactorDlg)
	enum { IDD = IDD_DGN_PSC_RATING_LIVE_FACTOR_DLG };
	int		m_iFactor;
	MEdit m_ctrlPrimary_P5;
	MEdit m_ctrlPrimary_P7;
	MEdit m_ctrlPrimary_P9;
	MEdit m_ctrlPrimary_P11;
	MEdit m_ctrlPrimary_P13;
	MEdit m_ctrlPrimary_User;
	MEdit m_ctrlPrimary_P15;
	MEdit m_ctrlPrimary_HL93I;
	MEdit m_ctrlPrimary_HL93O;
	MEdit m_ctrlAdjacent_HL93I;
	MEdit m_ctrlAdjacent_HL93O;
	//}}AFX_DATA

	double m_dPrimary;
	double m_dAdjacent;
	int	 m_iLimitState; //0=Service Limit State, 1=Strength Limit State
	int  m_nRatingType; //0=Steel, 1:PSC


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RatingLiveFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void InitCtrl();
	void SetEditCtrl();

	// Generated message map functions
	//{{AFX_MSG(RatingLiveFactorDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__RATINGLIVEFACTORDLG_H__)
