#if !defined(AFX_DGNGENLCOMSERVTYPEDLG_H__8CF160FD_DE4C_4842_8924_84D305BC267B__INCLUDED_)
#define AFX_DGNGENLCOMSERVTYPEDLG_H__8CF160FD_DE4C_4842_8924_84D305BC267B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Coded by Seungjun MNet:No.2393 ('20060919)
// DgnGenLcomServTypeDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenLcomServTypeDlg dialog
struct _DGN_LCOM_D
{
	CString strLoadCombName;
	UINT Lcomkey;
	int bElasticStage;
	int iSeqNum;
	BOOL bConc;
	int nServLcomType;
	void Initialize()
	{
		strLoadCombName = _T("");
		Lcomkey = 0;
		bElasticStage=0;
		iSeqNum = 0;
		bConc = TRUE;
		nServLcomType = 0;
	}
};

class __MY_EXT_CLASS__ CDgnGenLcomServTypeDlg : public CDialogMove
{
// Construction
public:
	CDgnGenLcomServTypeDlg(int nType, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnGenLcomServTypeDlg)
	enum { IDD = IDD_DGN_GEN_SER_TYPE_DLG };
	CListCtrl	m_aRareList;
	CListCtrl	m_aQusaiList;
	CListCtrl	m_aServLcomList;
	CListCtrl	m_aFrequentList;
	//}}AFX_DATA

	void SetServTypeCategory(int nCategory);
	void SetLoadCombinationType(int nType);
protected:
	CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_arServLcomMAP;
	CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_arQuasiLcomMAP;
	CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_arFrequentLcomMAP;
	CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_arRareLcomMAP;

	void InitCtrl();
	void AlignControls();
	void InitLcomData();
	void Write_ListCtrl(CListCtrl& LoadList, CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&>& LoadMap);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenLcomServTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nServTypeCategory;    //ServType 대화상자 기능별 구분, 1:PSC, 2:MODS
	// Generated message map functions
	//{{AFX_MSG(CDgnGenLcomServTypeDlg)
	afx_msg void OnDgnAddQuasiBtn();
	afx_msg void OnDgnDelQuasiBtn();
	afx_msg void OnDgnAddFrequentBtn();
	afx_msg void OnDgnDelFrequentBtn();
	afx_msg void OnDgnAddRareBtn();
	afx_msg void OnDgnDelRareBtn();
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    int m_nDgnType;
	/* russian :  0 servType; 1 Strength*/
	int m_nLoadType;
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENLCOMSERVTYPEDLG_H__8CF160FD_DE4C_4842_8924_84D305BC267B__INCLUDED_)
