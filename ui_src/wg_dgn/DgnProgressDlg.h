#if !defined(__DGNPROGRESSDLG_H__)
#define __DGNPROGRESSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnProgressDlg.h : header file
//

#include "..\wg_base\wg_base_ModelessMoveDlg.h"
#include "wg_dgnRes2.h"

#include "..\dgnengine\idesign\DGN_lib\DgnProgObserver.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnProgressDlg dialog

#include "HeaderPre.h"

// wg_rcs/RCSCStruct.h, wg_rcs/RCSStruct.h에 선언된 emum과 번호 맞출 것.
enum PROGRESS_TYPE
{
	PROGRESS_TYPE_NONE                         =    0, // None
	PROGRESS_TYPE_STEEL                        =    1, // Steel
	PROGRESS_TYPE_SRC                          =    2, // SRC
	PROGRESS_TYPE_CFS                          =    3, // Cold Formed Steel
	PROGRESS_TYPE_STEEL_FIRE_PREVENTION			=   4, // Steel Fire Prevention Design
	PROGRESS_TYPE_TOWER                        =    5, // Tower
	PROGRESS_TYPE_RC_BEAM_DGN                  =   11, // RC Beam Design
	PROGRESS_TYPE_RC_BEAM_CHK                  =   12, // RC Beam Checking
	PROGRESS_TYPE_RC_BEAM_DUCTILE_DGN          =   13, // RC Beam Ductile Design
	PROGRESS_TYPE_RC_COLUMN_DGN                =   21, // RC Column Design
	PROGRESS_TYPE_RC_COLUMN_CHK                =   22, // RC Column Checking
	PROGRESS_TYPE_RC_COLUMN_DUCTILE_DGN        =   23, // RC Column Ductile Design
	PROGRESS_TYPE_RC_BRACE_DGN                 =   31, // RC Brace Design
	PROGRESS_TYPE_RC_BRACE_CHK                 =   32, // RC Brace Checking
	PROGRESS_TYPE_RC_WALL_DGN                  =   41, // RC Wall Design
	PROGRESS_TYPE_RC_WALL_CHK                  =   42, // RC Wall Checking
    PROGRESS_TYPE_RC_PSBEAM_DGN                =   51,
    PROGRESS_TYPE_RC_PSBEAM_CHK                =   52,
	PROGRESS_TYPE_RC_SRC_BEAM_CHK              =   62, // RC base SRC-Beam Checking
	PROGRESS_TYPE_RC_HAUNCH_BEAM_DGN           =   71, // RC Haunched Beam Design
	PROGRESS_TYPE_RC_SLAB_DGN                  =   81, // RC Slab Design
	PROGRESS_TYPE_RC_SLAB_CHK                  =   82, // RC Slab Checking
	PROGRESS_TYPE_RC_SLAB_SERV_CHK             =   85, // RC Slab Serviceability Checking
	PROGRESS_TYPE_RC_SLAB_PUNCHING_FORCE       =   86, // RC Slab Punching shear Checking by Force 
	PROGRESS_TYPE_RC_SLAB_PUNCHING_STRESS      =   87, // RC Slab Punching shear Checking by Stress 
	PROGRESS_TYPE_RC_SLAB_PUNCHING_MAKE_FORCE  =   88, // RC Slab Making Punching shear Force
	PROGRESS_TYPE_RC_SLAB_PUNCHING_MAKE_STRESS =   89, // RC Slab Making Punching shear Stress
	PROGRESS_TYPE_RC_MESHED_WALL_DGN           =   91, // RC Meshed wall Design
	PROGRESS_TYPE_RC_MESHED_WALL_CHK           =   92, // RC Meshed wall Checking
	PROGRESS_TYPE_STEEL_PFDN_OMEGA			   =   93, // Steel Performance Design
    PROGRESS_TYPE_PSC_DGN_XL_RPT               =   94, // Civil PSC Design for Excel Report
    PROGRESS_TYPE_PSC_RATING                   =   95, // Civil PSC Rating
	PROGRESS_TYPE_STEEL_COMPOSITE              =   96, // Civil STEEL Composite
	PROGRESS_TYPE_CFS_PFDN_OMEGA			   =   97, // CFSteel Performance Design

	PROGRESS_TYPE_SRC_BEAM_DGN                 =  101, // SRC-Beam Design
	PROGRESS_TYPE_SRC_BEAM_CHK                 =  102, // SRC-Beam Checking
	PROGRESS_TYPE_SRC_COLUMN_DGN               =  111, // SRC-Column Design
	PROGRESS_TYPE_SRC_COLUMN_CHK               =  112, // SRC-Column Checking

	PROGRESS_TYPE_RC_INC_SLAB_DGN              =  301,// Inclined Slab Design
	PROGRESS_TYPE_RC_INC_SLAB_CHK              =  302,// Inclined Slab Checking
	PROGRESS_TYPE_JFD_DGN                      =  401,// iGen Japan Foundation Design
	PROGRESS_TYPE_JFD_CHK                      =  402,// iGen Japan Foundation Checking
	PROGRESS_TYPE_STRIP_DGN                    =  501,// Strip Design
	PROGRESS_TYPE_STRIP_CHK                    =  502,// Strip Checking
	PROGRESS_TYPE_STRIP_CALC_FORCE             =  503,// Strip Force Calculation
	PROGRESS_TYPE_RC_PLATE_BEAM_DGN            =  601, // RC Plate Beam Design
	PROGRESS_TYPE_RC_PLATE_BEAM_CHK            =  602, // RC Plate Beam Checking
	PROGRESS_TYPE_RC_PLATE_COLUMN_DGN          =  611, // RC Plate Column Design
	PROGRESS_TYPE_RC_PLATE_COLUMN_CHK          =  612, // RC Plate Column Checking
	PROGRESS_TYPE_RC_SHELL_FLEX_DGN            =  701, // RC Shell Flexural Design
	PROGRESS_TYPE_RC_SHELL_FLEX_CHK            =  702, // RC Shell Flexural Checking
	PROGRESS_TYPE_RC_SHELL_SHEAR_CHK           =  704, // RC Shell Shear Checking
	PROGRESS_TYPE_RLC_ANALYSIS_CHK             = 1001,// Lateral Load Carrying Capacity Checking
	PROGRESS_TYPE_RLC_ANALYSIS_RPT             = 1002,// Lateral Load Carrying Capacity Report

	PROGRESS_TYPE_STEEL_XL_RPT                 =  2101, // Civil Steel Design for Excel Report
	PROGRESS_TYPE_RC_BEAM_XL_RPT               =  2112, // Civil RC Beam Checking for Excel Report
	PROGRESS_TYPE_RC_COLUMN_XL_RPT             =  2122, // Civil RC Column Checking for Excel Report
	PROGRESS_TYPE_RC_WALL_XL_RPT               =  2142, // Civil RC Wall Checking for Excel Report.
	PROGRESS_TYPE_RC_PLATE_XL_RPT              =  2182, // Civil RC Plate Checking for Excel Report
	PROGRESS_TYPE_PSC_RATING_XL_RPT            =  2202, // Civil PSC Rating Design for Excel Report
	PROGRESS_TYPE_RC_RATING_XL_RPT             =  2203, // Civil RC Rating Design for Excel Report

	PROGRESS_TYPE_SEIS_EVAL_PRE_CONCOLM        =  3001, // Pre Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_PRE_CONWALL        =  3002, // Pre Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_PRE_STLCOLM        =  3003, // Pre Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_PRE_STLBRAC        =  3004, // Pre Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_LIN_CONBEAM        =  3011, // Lin Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_LIN_CONCOLM        =  3012, // Lin Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_LIN_CONWALL        =  3013, // Lin Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_LIN_STLBEAM        =  3014, // Lin Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_LIN_STLCOLM        =  3015, // Lin Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_LIN_STLBRAC        =  3016, // Lin Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_LIN_INFWALL        =  3017, // Lin Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_1ST_CONBEAM        =  3021, // 1st Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_1ST_CONCOLM        =  3022, // 1st Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_1ST_CONWALL        =  3023, // 1st Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_1ST_STLBEAM        =  3024, // 1st Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_1ST_STLCOLM        =  3025, // 1st Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_1ST_STLPANZ        =  3026, // 1st Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_1ST_STLBRAC        =  3027, // 1st Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_1ST_INFWALL        =  3028, // 1st Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_2ND_CONBEAM        =  3031, // 2nd Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_2ND_CONCOLM        =  3032, // 2nd Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_2ND_CONWALL        =  3033, // 2nd Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_2ND_STLBEAM        =  3034, // 2nd Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_2ND_STLCOLM        =  3035, // 2nd Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_2ND_STLBRAC        =  3036, // 2nd Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_2ND_INFWALL        =  3037, // 2nd Seismic Evaluation
	PROGRESS_TYPE_SEIS_EVAL_XL_RPT						 =  3038, // Seismic Evaluation for Excel Report

	PROGRESS_TYPE_BRDG_SEIS_EVAL               =  4001, // Bridge Seismic Evaluation
    PROGRESS_TYPE_BRDG_SEIS_EVAL_BEARING       =  4002, // Bridge Seismic Evaluation Bearing
	PROGRESS_TYPE_BRDG_SEIS_EVAL_XL_RPT        =  4010, // Bridge Seismic Evaluation Excel Report

	PROGRESS_TYPE_BRDG_SEIS_EVAL_JP			   =  4020, // Bridge Seismic Evaluation JP
	PROGRESS_TYPE_BRDG_SEIS_EVAL_XL_RPT_JP	   =  4030, // Bridge Seismic Evaluation Excel Report JP

    PROGRESS_TYPE_DGNPERFORM_THREAD            =  5001, // run design/checking using design engine perform module.
};

#define WM_USER_UPDATE_PROGRESS WM_USER+1

class __MY_EXT_CLASS__ CDgnProgressDlg : public CModelessMoveDlg
{
// Construction
public:
	CDgnProgressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnProgressDlg)
	enum { IDD = IDD_DGN_PROGRESS_DLG };
	CProgressCtrl	m_ProgressBar;
	CString	m_strName;
	CString	m_strPercent;
	UINT m_nTotalSteps;
	UINT m_nCurrentStep;
	//}}AFX_DATA

protected:
	CDgnProgressDlg* m_pDlg;
	int m_iType;
	int m_nPos;
	void Set_PosInc(int iCur, int iTot);
	void Set_PosInc2(int iCur, int iTot);
	void PumpMessages();
public:
	BOOL m_bStopExecute;
	BOOL m_bActOnUpdate; // Update_ProgressDlg시 ProgressDlg를 Active시킬지 여부 (동작중 ProgressDlg가 Active상태를 유지못할때 사용)
	void Create_ProgressDlg(int iType, CWnd* pParentWnd=NULL);
	void Create_ProgressDlgCVL(int iType, CWnd* pParentWnd=NULL);
	BOOL NextStep();
	BOOL Update_ProgressDlg(int iCur, int iTot);
	BOOL Update_ProgressDlg2(int iCur, int iTot);
    BOOL Update_ProgressDlg(CString strMessage, int iCur, int iTot);
	void Update_ProgressDlg_Subject(CString strMessage);
	void Delete_ProgressDlg();

private:
    void DelayTreate(const int& iPercent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnProgressDlg)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnProgressDlg)
	afx_msg void OnDgnStopBtn();
    afx_msg LRESULT OnUserFunc(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// CDgnEngineProgressDlg에서 사용하기 위한 함수 ////////////////////////////////////////////////////
protected:
	void Set_PosInc(CString strMessage, int iPercent);
	void Set_DlgData(CString strTitle, CString strMessage, int iPercent);
public:
	void Create_ProgressDlg(CString strWindow, CString strMessage, BOOL bUseStopBtn=TRUE);
	BOOL Update_ProgressDlg(CString strWindow, CString strMessage, int iPercent); // iPercent : (0∼100)
	BOOL Update_ProgressDlg(CString strMessage, int iPercent); // iPercent : (0∼100)

#pragma region // for DgnPerfom thread
public:
    static DWORD ThreadProgress(LPVOID IpParam);
#pragma endregion
};

#pragma region // DgnPerfom thread Observer
class CDgnEngineThreadObserver : public dgn::lib::IDgnProgObserver
{
public:
    CDgnEngineThreadObserver()
    {
        m_bUpdate = false;
        auto* pSubject = dgn::lib::CDgnProgObserver::Instance();
        if (pSubject != nullptr)
        {
            pSubject->InitData();
            pSubject->AttachObservers(this);
        }
    }
    ~CDgnEngineThreadObserver()
    {
        auto* pSubject = dgn::lib::CDgnProgObserver::Instance();
        if (pSubject != nullptr)
        {
            pSubject->DetachObservers(this);
        }
    }

    virtual void Update()
    {
        m_bUpdate = true;
    }

    BOOL IsUpdate()
    {
        return m_bUpdate;
    }

    void SetUpdateFalse() { m_bUpdate = FALSE; }

private:
    bool m_bUpdate;
};
#pragma endregion

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNPROGRESSDLG_H__)

