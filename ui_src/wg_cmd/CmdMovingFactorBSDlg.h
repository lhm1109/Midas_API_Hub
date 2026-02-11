#if !defined(__MOVINGFACTORBSDLG_H__)
#define __MOVINGFACTORBSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingLiveFactorDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"


/////////////////////////////////////////////////////////////////////////////
// RatingLiveFactorDlg dialog

#include "HeaderPre.h"


class __MY_EXT_CLASS__ CMovingFactorBSDlg : public CDialogMove
{
    // Construction
public:
    CMovingFactorBSDlg(CWnd* pParent = NULL);   // standard constructor
    ~CMovingFactorBSDlg();
    // Dialog Data
    //{{AFX_DATA(RatingLiveFactorDlg)
    enum { IDD = IDD_CMD_LCOMBDEFAULT_BS_MOVING_DLG };

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
