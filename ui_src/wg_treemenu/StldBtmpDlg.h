#if !defined(AFX_STLDBTMPDLG_H__9AEE086A_F347_4DD9_8347_E76D9733C27B__INCLUDED_)
#define AFX_STLDBTMPDLG_H__9AEE086A_F347_4DD9_8347_E76D9733C27B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldBtmpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStldBtmpDlg dialog
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditTemp.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

#include "StldBtmpDefCodeProvUtil.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"

struct T_BTMP_D;  
struct T_BTMP_STMP;
class CStldBtmpDlg : public CMenuBarChildDlg , public CDBUpdateConnector
{
// Construction
	T_BTMP_D * m_pData;
public:
	CStldBtmpDlg(CWnd* pParent = NULL);   // standard constructor
	~CStldBtmpDlg();
	virtual void Execute() { OnTmExecute(); }
	void MakeListHeader();
	void MakeListData();
	void MaterialCtrlMan();
	void OptionCtrlMan();
	
	int  FindEndSectTemp();
	BOOL GetParameter(T_BTMP_D * pBtmpD);
	BOOL GetSectTemp(T_BTMP_STMP * pSectTempD);
	BOOL SetSectTemp(T_BTMP_STMP * pSectTempD);
	BOOL ModifySecTemp();
	BOOL DeleteSecTemp();
	BOOL AddSecTemp();
	void DoUnitChange();

// Dialog Data
	
	//{{AFX_DATA(CStldBtmpDlg)
	enum { IDD = IDD_TM_STLD_BTMP };
	CTextUnit 	m_wndT0Unit;
	CEditTemp	m_wndT0Edit;
	CTextUnit	m_wndBUnit;
	CEditUnit	m_wndBEdit;
	CSelectLC	m_SelectLC;
	CCobxLdgr	m_CobxLdgr;
	CTextUnit	m_wndH2Unit;
	CEditUnit	m_wndH2Edit;
	CTextUnit	m_wndH1Unit;
	CEditUnit	m_wndH1Edit;
	CEditUnit	m_wndT2Edit;
	CTextUnit	m_wndT2Unit;
	CTextUnit	m_wndT1Unit;
	CEditUnit	m_wndT1Edit;
	CTextUnit	m_wndTermUnit;
	CTextUnit	m_wndElstUnit;
	CEditUnit	m_wndElstEdit;
	CEditUnit	m_wndTermEdit;
	CListCtrl	m_wndSTempList;
	CListCtrl	m_wndSTempBlankList;  
	int		m_nDirection;
	int		m_nMaterial;
	int		m_nOption;
	int   m_nRefPosition;
	int   m_nType; // 0:General, 1:Comp/PSC
	int		m_nRef;
	int		m_nBType;
	int   m_nH1Type;
	int   m_nH2Type;
	mit::frx::MComboBox m_cmbH1Z;
	mit::frx::MComboBox m_cmbH2Z;
	mit::frx::MButton m_chkApplyByCodeProv;

	mit::frx::MillustViewer m_wndDirPosPicture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldBtmpDlg)

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Implementation
protected:
	CDBDoc* m_pDoc;
	CBitmap*  m_pBitmap;
	CArray<UINT, UINT> m_aRcEnabeByApplyCodeProv;
	CArray<UINT, UINT> m_aMoveCtrl4Rus;  
	CArray<UINT, UINT> m_aMoveCtrl4Slope;  

	BTMP_DEF_CODE_PROV_D m_DefCodeProvD;

	BOOL m_bOpenSlope;

	void ChangeBitmap(int nDir, int nPos);

	int GetH1Type();
	int GetH2Type();
	void InitRcAr();
	void SetEnable();
	void SetShowHide();
	void MoveCtrls();
	BOOL GetChkApplyByCodeProv();


	// Generated message map functions
	//{{AFX_MSG(CStldBtmpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnTmBtnLc();
	afx_msg void OnMaterialOption();
	afx_msg void OnTmOptChange();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnTmAddButton();
	afx_msg void OnTmDeleteButton();
	afx_msg void OnTmModifyButton();
	afx_msg void OnItemchangedTmStempList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmT0TempBtn();
	afx_msg void OnTmDirRadio();
	afx_msg void OnTmRefPostRadio();
	afx_msg void OnTmTypeOption();
	afx_msg void OnTmBType();
	afx_msg void OnTmH1Type();
	afx_msg void OnTmH2Type();
	afx_msg void OnApplyByCode();
	afx_msg void OnChkApplyByCode();
	afx_msg void OnSlopeOfPSCSect();  
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
 




};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDBTMPDLG_H__9AEE086A_F347_4DD9_8347_E76D9733C27B__INCLUDED_)
