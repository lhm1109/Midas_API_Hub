#if !defined(__DGNSRCCODEDLG_H__)
#define __DGNSRCCODEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSrcCodeDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MComboBox.h"

#include "HeaderPre.h"

const int iDgn_SrcID2 = 4;
const int iDgn_SrcID3 = 5;
const int iDgn_SrcID4 = 13;

using namespace mit::frx;

class CDgnDataCtrl;
class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnSrcCodeDlg dialog
class __MY_EXT_CLASS__ CDgnSrcCodeDlg : public CDialogMove
{
// Construction
public:
	CDgnSrcCodeDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDgnSrcCodeDlg();

// Dialog Data
	//{{AFX_DATA(CDgnSrcCodeDlg)
	enum { IDD = IDD_DGN_SRC_CODE_DLG };
	MComboBox	m_DesignCode;
	BOOL	m_bSpecialEQ;
	int		m_iSafeLevel;
	int		m_iEqGrade;
		int     m_iAlphaType1;
		double  m_dAlpha1;
		double  m_dAlpha2;
	//}}AFX_DATA
	int m_iID2[iDgn_SrcID2];
	int m_iID3[iDgn_SrcID3];
	int m_iID4[iDgn_SrcID4];

protected:
	CDgnDataCtrl* m_pDgnDataCtrl;

public:
	int m_nCodeType;
	CArray<BOOL, BOOL> m_aCodeEnable;
	int m_nOldCodeIndex;

public:
	void Initial_ExistData();
	void Initial_Data();
	CRect MoveCtrl(int iKind, int* pID);
	void ChangeDlgItemInfo();
	int GetDataStatus(CString strCode);
	void SelchangeDgnCtrldataDesigncode(BOOL bUpdateDef);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSrcCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnSrcCodeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDgnSrcctrlSpecial();
	afx_msg void OnSelchangeDgnCtrldataDesigncode();
	afx_msg void OnChangeAlphaType();
	afx_msg void OnUpdateByCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif