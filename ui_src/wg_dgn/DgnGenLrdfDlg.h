#if !defined(__DGNGENLRDFDLG_H__)
#define __DGNGENLRDFDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenLrdfDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_baseMouseEdit2.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "DgnStruct.h"
#include "DgnDlgBase.h"

#include "..\mit_frx\MComboBox.h"

#include "HeaderPre.h"


class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnGenLrdfDlg dialog

struct _DGN_STOR
{
	int KeyStory;
	double dMinX,dMaxX;
	double dMinY,dMaxY;
	double dMinR,dMaxR;

	void Initialize()
	{
		KeyStory = 0;

		dMinX    = 0.;
		dMaxX    = 0.;
		dMinY    = 0.;
		dMaxY    = 0.;
		dMinR    = 0.;
		dMaxR    = 0.;
	}
};

class __MY_EXT_CLASS__ CDgnGenLrdfDlg : public CDgnDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenLrdfDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnGenLrdfDlg)
	enum { IDD = IDD_DGN_GEN_LRDF_DLG };
	CMouseEdit  m_MousePos;
	mit::frx::MComboBox	m_StoryCombo;
	mit::frx::MComboBox	m_Rmin;
	mit::frx::MComboBox	m_Rmax;
	CListCtrl	m_ListCtrl;
	double	m_Rmax2;
	double	m_Rmin2;
	CString	m_strLoadCaseNames;
	CString	m_strXmax;
	CString	m_strXmin;
	CString	m_strYmax;
	CString	m_strYmin;
	int		m_nGenCode;
	BOOL	m_bReduAll;
	BOOL	m_bReduAxl;
	BOOL	m_bReduMom;
	BOOL	m_bReduShr;
	//}}AFX_DATA

public:
	int m_StoryKey;
	double m_Xmax,m_Xmin,m_Ymax,m_Ymin;
	CArray<_DGN_STOR,_DGN_STOR&> m_StoryData;
	CArray<_DGN_REDU,_DGN_REDU&> m_LiveData;

public:
	BOOL ErrorCheck();

	void Select_ListCtrlItem(int nItem);
	void Del_LiveData(T_STLD_D rData);
	void Modify_LiveData(T_STLD_D rData);
	void ConvertUnitListCtrlData();
	void Search_NodeMaxMinCoord(int nNo1,int nNo2);
	void Initial_MouseEdit();
	void Add_LiveData(T_STLD_D rData);
	void Initial_Data();
	void DeleteStoryData(int Index);
	void ModifyStoryData(int Index);
	void AddStoryData();
	void Write_ListCtrlBox(int nCH, int Index,CString strStory,CString strXmin,
												 CString strXmax,CString strYmin,CString strYmax,
												 CString strRmax,CString strRmin);

	void Initial_ListCtrlBox();
	void Initial_SelLiveLoad();
	void Initial_StoryCombo();
	void Initial_ListBox();

	void Modify_SelLiveLoad(T_STLD_K key,T_STLD_D data);
	void Del_SelLiveLoad(T_STLD_K key,T_STLD_D data);
	void Cutting_SelLiveLoad(CString strSelLoadCases,CArray<int,int>& List);
	void Cutting_SelLiveLoad(CString strSelLoadCases,CArray<CString,CString>& List);
	BOOL UpdateReduKindCtrl(BOOL bReduAll);

	virtual void OnUpdate(CView* pView,LPARAM lHint,CObject* pHint);
	virtual void UpdateBuffer();

	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEscape(WPARAM wParam, LPARAM lParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenLrdfDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> arRange;
	void DlgCtrl(int nCtrl);

	// Generated message map functions
	//{{AFX_MSG(CDgnGenLrdfDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnCtrldataAdd();
	afx_msg void OnDgnCtrldataDelete();
	afx_msg void OnDgnCtrldataModify();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeDgnCtrldataRmin();
	afx_msg void OnDgnCtrldataLoadcomb();
	afx_msg void OnItemchangedDgnCtrldataList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeDgnCtrldataRmax();
	afx_msg void OnDgnCtrldataChn();
	afx_msg void OnDgnCtrldataGen();
	afx_msg void OnDgnCtrlReduKind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENLRDFDLG_H__F6014781_E201_11D3_888F_0000C0F30D4D__INCLUDED_)
