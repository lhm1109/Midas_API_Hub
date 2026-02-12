#if !defined(AFX_CMTHKITEMVALUEDLG_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMTHKITEMVALUEDLG_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
	// CMThkItemValueDlg.h : header file
	//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_DlgChild.h"
#include "..\mit_frx\MillustViewer.h"

struct Value
{
	int ID;
	CString strName;
	int nThickType;
	double ThickIn;
	double ThickOut;
	BOOL   bOffset;
	int    nOffsetType;
	double dRatio;
	double dValue;
	void Initialize()
	{
		nThickType = 0;
		strName.Empty();
		ThickIn = 0.0;
		ThickOut = 0.0;
		bOffset = FALSE;
		nOffsetType = 0;
		dRatio = 0.0;
		dValue = 0.0;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CCMThkItemValueDlg dialog

class CCMThkItemValueDlg : public CDlgChild
{
  // Construction
public:
  CCMThkItemValueDlg(CDBDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

public:
  CDBDoc* m_pDoc;
  BOOL		m_bModify;
  Value		m_sDlgData;

  T_THIK_K m_Key;
  T_THIK_D m_Data;

public:
  void	SetModifyData(T_THIK_K Key, T_THIK_D &Data);
  BOOL	OnApplyPublic();

  // Dialog Data
  //{{AFX_DATA(CCMThkItemValueDlg)
  enum { IDD = IDD_CMD_TP_ITEM_VALUE };
  CTextUnit	m_UnitBoth;
  CTextUnit	m_UnitIn;
  CTextUnit	m_UnitOut;
  UINT	m_nID;
  CString m_strName;
  double m_dbThkBoth;
  double m_dbThkIn;
  double m_dbThkOut;
  int		m_nPlaneType;
  BOOL      m_bOffset;
  int       m_nOffset;
  CEditUnit m_edtRatio;
  CEditUnit m_edtValue;
  CTextUnit m_untValue;

  mit::frx::MillustViewer m_wndPicture;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCMThkItemValueDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:
  CArray<UINT, UINT> m_arPlaneBoth;
  CArray<UINT, UINT> m_arPlaneEach;

  void MakeUnitSystem();
  BOOL SaveData();
  void GetData(T_THIK_K &Key, T_THIK_D &Data);
  void SetData();
  void InitDlgItems(BOOL bOnInitDialog);

  // Generated message map functions
  //{{AFX_MSG(CCMThkItemValueDlg)
  afx_msg void OnCmdTpThickControl();
  virtual BOOL OnInitDialog();
  afx_msg void OnCmdTpOffset();
  afx_msg void OnCmdTpOffsetType();
  afx_msg void OnCmdAutoChangeName();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHKITEMVALUEDLG_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)

