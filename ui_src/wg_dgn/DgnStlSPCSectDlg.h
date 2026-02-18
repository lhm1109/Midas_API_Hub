#if !defined(AFX_DGNSTLSPCSECTDLG_H__)
#define AFX_DGNSTLSPCSECTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlSPCSectDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_I_Factory_DlgViewPanel.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;


#include "DgnStlSPCSectITabDlg.h"
#include "DgnStlSPCSectJTabDlg.h"

#include "HeaderPre.h"

#define I_TAB		0
#define J_TAB		1

#define POINT_MODE_NONE		0
#define POINT_MODE_1		  1
#define POINT_MODE_2		  2
#define POINT_MODE_3		  3
#define POINT_MODE_4		  4
/////////////////////////////////////////////////////////////////////////////
// CDgnStlSPCSectDlg dialog

class CPointLabel : public I_DlgView_Label
{
public:
	CPointLabel();
	~CPointLabel();
	virtual void On_Paint(I_RenderEngine* pIRE);

	void SetDlg(CDgnStlSPCSectDlg *pDlg) { m_pDlg = pDlg; }
public:
	CFont			m_Font;
	LOGFONT		m_LogFont;
	CDgnStlSPCSectDlg *m_pDlg;
protected:
	void DrawPointLabel(I_RenderEngine* pIRE);        

protected:
	UINT m_RotAlign;

	LOGFONT m_lf;
};


class I_DlgViewPanel;
class CStlSPCSectEventHandler;
class CDgnStlSPCSectITabDlg;
class CDgnStlSPCSectJTabDlg;

class __MY_EXT_CLASS__ CDgnStlSPCSectDlg : public CDialogMove
{
// Construction
public:
	CDgnStlSPCSectDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnStlSPCSectDlg();

	void SelectLineRedraw(ROWCOL nRow);
	void Redraw(BOOL bZoomAll = TRUE);
	void SetViewLButtonDown(double dX, double dY);

	void GetPointData(CArray<DV_3dp, DV_3dp&>& arPoint);

	//void GetNearPoint(double dx, double dy, double& dPointX, double& dPointY);
	//void GetSubGroupList(double dx, double dy, CArray<int, int&>& arGroup, CArray<int, int&>& arSubGroup);

	void SetSectIData();
	void SetSectJData();

	int m_PointMode;

	void SetPointMode(int Mode);

	
// Dialog Data
	//{{AFX_DATA(CDgnStlSPCSectDlg)
	enum { IDD = IDD_DGN_STL_SPC_SECT_DLG };	
	CDlgTabCtrl m_ctrlTab;
	MComboBox	m_ComboSectName;
	MButton		m_btnApply;
	CBCGPStatic	  m_strTextOut;
	//}}AFX_DATA




	protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlSPCSectDlg)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support		
	//}}AFX_VIRTUAL

	I_DlgViewPanel* m_pIVP;
	CPointLabel m_PointLabel;
	CStlSPCSectEventHandler* m_pEventHandler; 

	

// Implementation
protected:
	CDBDoc*						m_pDoc;

	CDgnStlSPCSectITabDlg*      m_pIDlg;
	CDgnStlSPCSectJTabDlg*			m_pJDlg;
			 
	BOOL InitTabCtrl();
	void InitSectNameCombo();
	void InitLineData();
	CArray<T_SECT_K, T_SECT_K&> m_aSectList;

	T_SECT_D m_sectData;
	T_SECT_SECTBASE_D* m_pDrawData;
	CArray<T_GSEC_LDGN, T_GSEC_LDGN&> m_aLineData;

	CArray<DV_3dp, DV_3dp&> m_aPointData;

	BOOL Data2Dlg();
	BOOL Dlg2Data();

	void InitPointListData();
	void SetLineDraw();
	void SetPointDraw();
	void SetSelectLineDraw(ROWCOL nRow);
	  
	// Generated message map functions
	//{{AFX_MSG(CDgnStlSPCSectDlg)
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeSectCombo();
	afx_msg void OnClickApplyBtn();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSTLSPCSECTDLG_H__)
