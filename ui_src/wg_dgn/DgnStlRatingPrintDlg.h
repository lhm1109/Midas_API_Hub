#if !defined(AFX_DGNSTLRATINGPRINTDLG_H__)
#define AFX_DGNSTLRATINGPRINTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlRationPrintDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\MIT_frx\MComboBox.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRationPrintDlg dialog
class CDgnStlRationPrintDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CDgnStlRationPrintDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	
	enum { IDD = IDD_DGN_STL_RATING_PRINT_OPTION_DLG };

	int m_nOption;
	int m_nElemType;
	int m_nPart;
	int m_nFlexure;
	int m_nShear;
	int m_nService;
	mit::frx::MComboBox m_cmbFlexure;
	mit::frx::MComboBox m_cmbShear;
	mit::frx::MComboBox m_cmbService;
	BOOL m_bFat_Min ;
	BOOL m_bFat_EVL1;
	BOOL m_bFat_EVL2;
	BOOL m_bFat_Mean;

private:
	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	CDBDoc* m_pDoc;

	void InitCtrl();
	void AlignControl();
	void EnableDisableControls();
	void ControlsShowHide();
	void Initial_SelectItem();

	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnArDel();
	afx_msg void OnDgnFlexureRdo();
	afx_msg void OnDgnShearRdo();
	afx_msg void OnDgnServiceRdo();
	afx_msg void OnDgnCPGElemTypeRdo();
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_DGNSTLRATINGPRINTDLG_H__)
