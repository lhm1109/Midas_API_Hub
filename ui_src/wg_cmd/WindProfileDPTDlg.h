#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_base\wg_base_MySRGraph.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CWindProfileDPTDlg : public CDialogMove
{
public:
    CWindProfileDPTDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_CMD_WINDPF_DPT };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    afx_msg void OnChangeDir();
    afx_msg void OnChangeCompont();
    afx_msg void OnChangeProfileType();
    afx_msg void OnWINDPFViewCalcSheet();
    afx_msg void OnWindPFBrowse();
    afx_msg void OnBtnDyGen();
    DECLARE_MESSAGE_MAP()

public:
    void InitWindProfile(T_WIND_K nKey, T_WIND_D& rDataWind, T_KEY KeyWAorWT, T_WNAT_D* pWnatD = nullptr);

private:
    void InitChart();
    void InitGrid(int nCode);
    void InitFileName();

    void SetValue(int nIndex, int nGroup, double x, double y);
    void UpdateChart();
    void UpdateGrid();

private:
    CString ConvertValToFmtStr(double Val, int nLimit);
    UINT GetKeyWA(const T_WIND_D* pWindD);
    int  GetComponent();
    void SetAxisLabelText(int nComponent, LPCTSTR lbl);

    BOOL IsEnableAcross(const T_WIND_D* pWindD);
    BOOL IsEnableTorsional(const T_WIND_D* pWindD);
    BOOL IsHideColPresssure(int nDir, const T_WIND_D* pWindD);
    BOOL IsHideColMaxDispAccl(int nDir, const T_WIND_D* pWindD);

private:
    void SetComponentText(UINT nDir);

private:
    CLateralLoad m_LateralLoad;
    CGXGridWnd m_WindPfGrid;
    SREGraphView m_GraphView;
    CMySRGraph m_Graph;
    CArray<UINT, UINT> m_aCtrlProfileSub;

private:
    int	m_nDir;
    int	m_nComponent;
    int	m_nProfileType;
    CString	m_strFileName;
    MButton	m_btnDyGen;

private:
    T_WIND_D* m_pDataWind;
    T_WNAT_D* m_pWnatD;

    T_WIND_K m_Key;
    T_WIND_K m_KeyWAorWT;
    BOOL m_LogFlagX;
    BOOL m_LogFlagY;
};
