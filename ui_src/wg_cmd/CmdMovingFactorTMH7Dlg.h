#if !defined(__MOVINGFACTORTMH7DLG_H__)
#define __MOVINGFACTORTMH7DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingLiveFactorDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"


/////////////////////////////////////////////////////////////////////////////
// RatingLiveFactorDlg dialog

#include "HeaderPre.h"


class __MY_EXT_CLASS__ CMovingFactorTMH7Dlg : public CDialogMove
{
    // Construction
public:
    CMovingFactorTMH7Dlg(CWnd* pParent = NULL);   // standard constructor
    ~CMovingFactorTMH7Dlg();
    // Dialog Data
    //{{AFX_DATA(RatingLiveFactorDlg)
    enum { IDD = IDD_CMD_LCOMBDEFAULT_TMH7_MOVING_DLG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
public:

    // Generated message map functions
    //{{AFX_MSG(RatingLiveFactorDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__RATINGLIVEFACTORDLG_H__)
