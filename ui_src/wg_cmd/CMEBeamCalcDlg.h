#if !defined(AFX_CMEBeamCalcDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMEBeamCalcDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_rcs\RcsFlatSlabEffWidthStruct.h"

using namespace mit::frx;

class CCMEBeamCalcDlg : public CDialogMove
{
public:
	CCMEBeamCalcDlg(CWnd* pParent = NULL);

public:
	CDBDoc* m_pDoc;
	BOOL		m_bModify;

public:
	inline double GetCalculatedWidth() { return m_dCaculatedWidth; }

	//{{AFX_DATA(CCMEBeamCalcDlg)
	enum { IDD = IDD_CMD_EBEAM_CALC_DLG };
	MComboBox	m_cobxMethod;
	MComboBox	m_cobxGrLocEB  ;
	MComboBox	m_cobxGrSupport;
	MComboBox	m_cobxGrLateral;
	MComboBox	m_cobxCSLocEB  ;
	MComboBox	m_cobxCSJoint  ;
	CEditUnit m_edtL1 ;
	CEditUnit m_edtL2 ;
	CEditUnit m_edtL3 ;
	CEditUnit m_edtC1 ;
	CEditUnit m_edtC2 ;
	CEditUnit m_edtGrD  ;
	CEditUnit m_edtGrH  ;
	CEditUnit m_edtGrKFP;
	CEditUnit m_edtGrKD ;
	CEditUnit m_edtGrA  ;

	CTextUnit m_untGrL1 ;
	CTextUnit m_untGrL2 ;
	CTextUnit m_untGrL3 ;
	CTextUnit m_untGrC1 ;
	CTextUnit m_untGrC2 ;
	CTextUnit m_untGrD  ;
	CTextUnit m_untGrH  ;

	CEditUnit m_edtCalcResult;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMEBeamCalcDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CArray<UINT,UINT> m_aCtrlGrossman;
	CArray<UINT,UINT> m_aCtrlChoiSong;
	CArray<UINT,UINT> m_aCtrlEtc;   // Calculate 이하 컨트롤. 높이 조정을 위해..
	RCSFS_EFF_WIDTH m_RcsFS;        // Calculate 했을 때 입력값
	RCSFS_EFF_WIDTH m_RcsFS_Init;   // 대화상자 열었을 때 입력값. OK 클릭 시 m_RcsFS로 갱신
	double m_dCaculatedWidth;       // 결과값(유효보폭)

	void InitUnit();
	void InitCombobox();
	void AlignCtrl();
	void ShowHideCtrl();
	void Data2Dlg();

private:
	double GetValue(int nResType);
	BOOL GetInputData(RCSFS_EFF_WIDTH &rRcsFS);
	BOOL CheckInputData(const RCSFS_EFF_WIDTH &RcsFS);
	BOOL CheckInputDataGrossman(const RCSFS_EFF_WIDTH &RcsFS);
	BOOL CheckInputDataChoiSong(const RCSFS_EFF_WIDTH &RcsFS);
	BOOL CheckDataDesignStrip(UINT nDgnStr);
	BOOL IsZero(double dValue);
	UINT GetDriftCriteria();
	UINT GetColumnPosition();
	UINT GetBarType();
	UINT GetStripType(int nMethod);
	void GetDefaultData(RCSFS_EFF_WIDTH &rRcsFS);

	// Generated message map functions
	//{{AFX_MSG(CCMEBeamCalcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancel();
	afx_msg void OnSelchangeMethodCmb();
	
	afx_msg void OnChangeGrLocEBCmb();
	afx_msg void OnChangeCsLocEBCmb();
	afx_msg void OnChangeCsJointCmb();

	afx_msg void OnChangeSupport();
	afx_msg void OnChangeLateral();
	afx_msg void OnApply();
	afx_msg void OnCalculate();
	afx_msg void EnableDisableCtrls();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMEBeamCalcDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
