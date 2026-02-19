// DgnStlRatingEffectLengUframeDlg.h: interface for the CDgnStlRatingEffectLengUframeDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DgnStlRatingEffectLengUframeDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_DgnStlRatingEffectLengUframeDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MillustViewer.h"

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class __MY_EXT_CLASS__ CDgnStlRatingEffectLengUframeDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CDgnStlRatingEffectLengUframeDlg(CWnd* pParent = NULL);

	// Dialog Data
		//{{AFX_DATA(CDgnStlRatingEffectLengUframeDlg)
	enum { IDD = IDD_DGN_STL_RATING_EFFECT_LENG_U_FRAME_DLG };

	int	   	  m_nOption;
	int       m_nConnectType;
	CEditUnit m_edtd1;
	CTextUnit m_untd1;
	CEditUnit m_edtd2;
	CTextUnit m_untd2;
	int       m_nBeam;
	CEditUnit m_edtB;
	CTextUnit m_untB;
	int       m_nJoint;
	//}}AFX_DATA

private:
	mit::frx::MillustViewer m_wndPicture;

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDgnStlRatingEffectLengUframeDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Implementation
protected:
	void InitUnit();
	void Initial_SelectItem();
	void EnableDisableControls();

	BOOL Dlg2Data();

	CDBDoc* m_pDoc;

	T_ELUF_D m_Data;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlRatingEffectLengUframeDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnOption();
	afx_msg void OnDgnConnectType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnStlRatingEffectLengUframeDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
