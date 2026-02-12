#if !defined(__WG_CMD_CMTHRDHNGE_H__)
#define __WG_CMD_CMTHRDHNGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdHnge.h : header file
//

#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "CMChildBarBase.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

struct T_INELASTIC_HINGE_INFO
{
	int nHingeType;
	int nHingeLocation;
	int nNumHinge;
	void Initialize()
	{
		nHingeType = 0;
		nHingeLocation = 0;
		nNumHinge = 0;
	}
	T_INELASTIC_HINGE_INFO() {}
	T_INELASTIC_HINGE_INFO(T_INELASTIC_HINGE_INFO& src) {*this = src;}
	T_INELASTIC_HINGE_INFO& operator=(T_INELASTIC_HINGE_INFO& src)
	{
		nHingeType = src.nHingeType;
		nHingeLocation = src.nHingeLocation;
		nNumHinge = src.nNumHinge;
		return *this;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CCMThrdHnge dialog
class CCMThrdHnge : public CCMChildBarBase
{
public:
	BOOL m_bModify;
	T_THRD_D m_pDlgData;
	//T_INELASTICDYN_HINGE_INFO_BASE m_InelstHngeInfoBase;
	T_INELASTIC_HINGE_INFO m_InelstHngeInfoBase;

// Construction
public:
	virtual void	Execute(){ OnCmdApply(); }
	virtual BOOL ExternalInit(UINT key);
	void  ChangeComponent(int nType, int nElemType);
	CCMThrdHnge(CWnd* pParent = NULL);   // standard constructor
	
	BOOL  SetInelstHngeInfo(UINT ElemK);
	void  UpdateLocation(int nPos);
	int   GetPosition();

	LRESULT				OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
// Dialog Data
	//{{AFX_DATA(CCMThrdHnge)
	enum { IDD = IDD_DBAR_THRD_HNGE };
	MComboBox	m_cbxComp;
	CSelectLC	m_cbxLComb;
	MComboBox	m_cbxLocation;
	MComboBox m_cbxStory;

	CMouseEdit	m_edElemK;
	CString	m_sName;
	int		m_nType;
	CString	m_sElemK;
	int   m_nElemType;
	MComboBox m_cbxGL;
	CString m_sStoryData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThrdHnge)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	CArray<UINT, UINT> m_aElement;
	CArray<UINT, UINT> m_aLink;
	CString strOldName;
	BOOL OnExecute();
	void DataInit();
	BOOL Dlg2Data();
	void Data2Dlg();
	void AlignCtrl();
	void MakeNllkKeyList();

	int m_nComboType;

	// Generated message map functions
	//{{AFX_MSG(CCMThrdHnge)
	virtual BOOL OnInitDialog();
	afx_msg void OnGpsBarThtypeDeform();
	afx_msg void OnElemEnbutton();
	afx_msg void OnSelchangeGpsBarThcomp();
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdElemTypeRdo();
	afx_msg void OnChangeStory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WG_CMD_CMTHRDHNGE_H__)
