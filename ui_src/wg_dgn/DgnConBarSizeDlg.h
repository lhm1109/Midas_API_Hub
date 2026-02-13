#if !defined(AFX_DGNCONBARSIZEDLG_H__61D65D41_D7B4_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNCONBARSIZEDLG_H__61D65D41_D7B4_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBarSizeDlg.h : header file
//
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_base\wg_base_DialogMove.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarSizeDlg dialog

class CDgnConBarSizeDlg : public CDialogMove
{
// Construction
public:
	CDgnConBarSizeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConBarSizeDlg)
	enum { IDD = IDD_DGN_CON_BAR_SIZE_DLG };
	//}}AFX_DATA

public:
	BOOL	m_bRbar11[CONST_BAR_KS  ];	// 11=KS01/KS/KS01-Civil/KS-Civil.
	BOOL	m_bRbar12[CONST_BAR_CNS ];	// 12=CNS.
	BOOL	m_bRbar13[CONST_BAR_JIS ];	// 13=JIS/JIS-Civil.
	BOOL	m_bRbar14[CONST_BAR_PNS49];	// 14=PNS49.
	BOOL	m_bRbar21[CONST_BAR_ASTM];	// 21=ASTM.
	BOOL	m_bRbar31[CONST_BAR_BSEN];	// 31=BS/EN.
	BOOL	m_bRbar32[CONST_BAR_IS  ];	// 32=IS.
	BOOL	m_bRbar33[CONST_BAR_UNI ];	// 33=UNI.
	BOOL	m_bRbar34[CONST_BAR_SS  ];	// 34=SS560.
	BOOL	m_bRbar41[CONST_BAR_GB  ];	// 41=GB/GB-Civil.
	BOOL	m_bRbar51[CONST_BAR_CSA ];	// 51=CSA.
	BOOL	m_bRbar61[CONST_BAR_GOST_SP ];	// 61=GOST_SP.
	BOOL	m_bRbar71[CONST_BAR_AS];		// 71=AS.

	int m_CheckCount;
	CString m_TotalReBarSize;

public:
	CString Get_SelectReBarSize();
	BOOL Cutting_RebarSizeString(CString str, CArray<CString,CString>& BarSize);
	BOOL Show_RebarKind();
	BOOL Check_SizeData(BOOL& bSize);

private:
	void SetDisableCtrl(int nSize, const CArray<UINT,UINT>& aRbarID, BOOL abRbar[]);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBarSizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnConBarSizeDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

	afx_msg void OnDgnCheck1101();
	afx_msg void OnDgnCheck1102();
	afx_msg void OnDgnCheck1103();
	afx_msg void OnDgnCheck1104();
	afx_msg void OnDgnCheck1105();
	afx_msg void OnDgnCheck1106();
	afx_msg void OnDgnCheck1107();
	afx_msg void OnDgnCheck1108();
	afx_msg void OnDgnCheck1109();
	afx_msg void OnDgnCheck1110();
	afx_msg void OnDgnCheck1111();
	afx_msg void OnDgnCheck1112();
	afx_msg void OnDgnCheck1113();
	afx_msg void OnDgnCheck1114();
	afx_msg void OnDgnCheck1115();

	afx_msg void OnDgnCheck1201();
	afx_msg void OnDgnCheck1202();
	afx_msg void OnDgnCheck1203();
	afx_msg void OnDgnCheck1204();
	afx_msg void OnDgnCheck1205();
	afx_msg void OnDgnCheck1206();
	afx_msg void OnDgnCheck1207();
	afx_msg void OnDgnCheck1208();
	afx_msg void OnDgnCheck1209();
	afx_msg void OnDgnCheck1210();
	afx_msg void OnDgnCheck1211();
	afx_msg void OnDgnCheck1212();
	afx_msg void OnDgnCheck1213();

	afx_msg void OnDgnCheck1301();
	afx_msg void OnDgnCheck1302();
	afx_msg void OnDgnCheck1303();
	afx_msg void OnDgnCheck1304();
	afx_msg void OnDgnCheck1305();
	afx_msg void OnDgnCheck1306();
	afx_msg void OnDgnCheck1307();
	afx_msg void OnDgnCheck1308();
	afx_msg void OnDgnCheck1309();
	afx_msg void OnDgnCheck1310();
	afx_msg void OnDgnCheck1311();
	afx_msg void OnDgnCheck1312();
	afx_msg void OnDgnCheck1313();

	afx_msg void OnDgnCheck1401();
	afx_msg void OnDgnCheck1402();
	afx_msg void OnDgnCheck1403();
	afx_msg void OnDgnCheck1404();
	afx_msg void OnDgnCheck1405();
	afx_msg void OnDgnCheck1406();
	afx_msg void OnDgnCheck1407();
	afx_msg void OnDgnCheck1408();
	afx_msg void OnDgnCheck1409();
	afx_msg void OnDgnCheck1410();

	afx_msg void OnDgnCheck2101();
	afx_msg void OnDgnCheck2102();
	afx_msg void OnDgnCheck2103();
	afx_msg void OnDgnCheck2104();
	afx_msg void OnDgnCheck2105();
	afx_msg void OnDgnCheck2106();
	afx_msg void OnDgnCheck2107();
	afx_msg void OnDgnCheck2108();
	afx_msg void OnDgnCheck2109();
	afx_msg void OnDgnCheck2110();
	afx_msg void OnDgnCheck2111();

	afx_msg void OnDgnCheck3101();
	afx_msg void OnDgnCheck3102();
	afx_msg void OnDgnCheck3103();
	afx_msg void OnDgnCheck3104();
	afx_msg void OnDgnCheck3105();
	afx_msg void OnDgnCheck3106();
	afx_msg void OnDgnCheck3107();
	afx_msg void OnDgnCheck3108();
	afx_msg void OnDgnCheck3109();
	afx_msg void OnDgnCheck3110();
	afx_msg void OnDgnCheck3111();
	afx_msg void OnDgnCheck3112();
	afx_msg void OnDgnCheck3113();
	afx_msg void OnDgnCheck3114();

	afx_msg void OnDgnCheck3201();
	afx_msg void OnDgnCheck3202();
	afx_msg void OnDgnCheck3203();
	afx_msg void OnDgnCheck3204();
	afx_msg void OnDgnCheck3205();
	afx_msg void OnDgnCheck3206();
	afx_msg void OnDgnCheck3207();
	afx_msg void OnDgnCheck3208();
	afx_msg void OnDgnCheck3209();
	afx_msg void OnDgnCheck3210();
	afx_msg void OnDgnCheck3211();
	afx_msg void OnDgnCheck3212();
	afx_msg void OnDgnCheck3213();

	afx_msg void OnDgnCheck3301();
	afx_msg void OnDgnCheck3302();
	afx_msg void OnDgnCheck3303();
	afx_msg void OnDgnCheck3304();
	afx_msg void OnDgnCheck3305();
	afx_msg void OnDgnCheck3306();
	afx_msg void OnDgnCheck3307();
	afx_msg void OnDgnCheck3308();
	afx_msg void OnDgnCheck3309();
	afx_msg void OnDgnCheck3310();
	afx_msg void OnDgnCheck3311();
	afx_msg void OnDgnCheck3312();
	afx_msg void OnDgnCheck3313();
	afx_msg void OnDgnCheck3314();
	afx_msg void OnDgnCheck3315();
	afx_msg void OnDgnCheck3316();
	afx_msg void OnDgnCheck3317();

	afx_msg void OnDgnCheck3401();
	afx_msg void OnDgnCheck3402();
	afx_msg void OnDgnCheck3403();
	afx_msg void OnDgnCheck3404();
	afx_msg void OnDgnCheck3405();
	afx_msg void OnDgnCheck3406();
	afx_msg void OnDgnCheck3407();
	afx_msg void OnDgnCheck3408();
	afx_msg void OnDgnCheck3409();
	afx_msg void OnDgnCheck3410();
	afx_msg void OnDgnCheck3411();
	afx_msg void OnDgnCheck3412();
	afx_msg void OnDgnCheck3413();
	afx_msg void OnDgnCheck3414();

	afx_msg void OnDgnCheck4101();
	afx_msg void OnDgnCheck4102();
	afx_msg void OnDgnCheck4103();
	afx_msg void OnDgnCheck4104();
	afx_msg void OnDgnCheck4105();
	afx_msg void OnDgnCheck4106();
	afx_msg void OnDgnCheck4107();
	afx_msg void OnDgnCheck4108();
	afx_msg void OnDgnCheck4109();
	afx_msg void OnDgnCheck4110();
	afx_msg void OnDgnCheck4111();
	afx_msg void OnDgnCheck4112();
	afx_msg void OnDgnCheck4113();
	afx_msg void OnDgnCheck4114();
	afx_msg void OnDgnCheck4115();
	afx_msg void OnDgnCheck4116();

	afx_msg void OnDgnCheck5101();
	afx_msg void OnDgnCheck5102();
	afx_msg void OnDgnCheck5103();
	afx_msg void OnDgnCheck5104();
	afx_msg void OnDgnCheck5105();
	afx_msg void OnDgnCheck5106();
	afx_msg void OnDgnCheck5107();
	afx_msg void OnDgnCheck5108();

	afx_msg void OnDgnCheck6101();
	afx_msg void OnDgnCheck6102();
	afx_msg void OnDgnCheck6103();
	afx_msg void OnDgnCheck6104();
	afx_msg void OnDgnCheck6105();
	afx_msg void OnDgnCheck6106();
	afx_msg void OnDgnCheck6107();
	afx_msg void OnDgnCheck6108();
	afx_msg void OnDgnCheck6109();
	afx_msg void OnDgnCheck6110();
	afx_msg void OnDgnCheck6111();
	afx_msg void OnDgnCheck6112();
	afx_msg void OnDgnCheck6113();
	afx_msg void OnDgnCheck6114();

	afx_msg void OnDgnCheck7101();
	afx_msg void OnDgnCheck7102();
	afx_msg void OnDgnCheck7103();
	afx_msg void OnDgnCheck7104();
	afx_msg void OnDgnCheck7105();
	afx_msg void OnDgnCheck7106();
	afx_msg void OnDgnCheck7107();
	afx_msg void OnDgnCheck7108();
	afx_msg void OnDgnCheck7109();
	afx_msg void OnDgnCheck7110();
	afx_msg void OnDgnCheck7111();
	afx_msg void OnDgnCheck7112();
	afx_msg void OnDgnCheck7113();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONBARSIZEDLG_H__61D65D41_D7B4_11D3_888F_0000C0F30D4D__INCLUDED_)
