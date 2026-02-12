#pragma once

#include "..\MIT_frx\MComboBox.h"
#include "..\wg_db\EearthquakeWaveMgr.h"

struct WAVE_DATA_CH
{
public:
	WAVE_DATA_CH(double dEPA=0, double dEPV=0, double dTg=0) { Initial(dEPA, dEPV, dTg); }
	double m_dEPA;
	double m_dEPV;
	double m_dTg;
public:
	void Initial(double dEPA, double dEPV, double dTg)
	{
		m_dEPA = dEPA;
		m_dEPV = dEPV;
		m_dTg  = dTg;
	}
};

#include "HeaderPre.h"
class __MY_EXT_CLASS__ EQ_REC_DLG : public CDialogMove
{
public:
	EQ_REC_DLG(CWnd* pParent = NULL);
	enum { IDD = IDD_ETC_EARTHQUAKE_REC };

public:
	virtual BOOL Create(CWnd* pParentWnd);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

protected:
	afx_msg void OnSelchangecmbEARTHQUAKE();
	afx_msg void OnSelchangecmbTg();
	afx_msg void OnButton1();
	afx_msg void OnEqGuandongCheck_CH();
	afx_msg void OnSelGDCate_CH();
	afx_msg void OnSelGDTg_CH();
	DECLARE_MESSAGE_MAP()

private:
	void GetFile();
	CString GetFilePath(LPCTSTR lpFileName, BOOL bGuangDong_CH = FALSE);
	CString GetFileName(LPCTSTR lpName);
	void AddExtraWaveToComb(int nTgType);
	void InitialWaveCombx();
	void InitialWaveCombx_GD_CH();
	CString GetCurDescription();
	IEearthquakeWaveMgr::WAVE_TG GetWaveTg(int nTgType);
	BOOL IsExtraWave();
	BOOL IsNeedShowWave(LPCTSTR lpWaveName, int nTgType = 0);
	BOOL GetWaveData(LPCTSTR lpWaveName, WAVE_DATA_CH &waveData);
	void InitialWaveData(CString aWaveName[], int nSize);

	BOOL IsUseExtraWave();
	BOOL IsUseExtraWave_GEN();
	void InitComboString_CH();
	void InitGuangdongWaveFolder();
	void GetComboStrByIndex(int nIndex, CStringArray& str);//nindex 0 site;1 Tg;2 Struct Base Tg
	CString GetFileStrBy_Index(int nIndex, const CString& strArr);
	void GetGuangdongWaveFileName(CStringArray& str);
	void AddExtraWaveToComb_GuangDong_CH();
	int EQ_REC_DLG::lex(FILE *fp, char *yytext);
	BOOL EQ_REC_DLG::ReadGuangDongInfo(const CString& strFileName);
	void AdjustUILayoutForChinese();
public:
	CString m_sFileTitle;
	FILE* m_fp;
	CString m_sDescript;

private:
	CStringArray m_strEarthquakeDetail;
	CString FilePath;

	BOOL bGetFile;
	mit::frx::MComboBox m_ctlCmbEarthquake;

public:
	float	m_fScaleFactor;
	float	m_fTimeScale;
	CString	m_strSelectedEQ;
	CString	m_strEarthquake;

private:
	CMap<CString, LPCTSTR, WAVE_DATA_CH, const WAVE_DATA_CH&> m_mapWaveCH;
	BOOL m_bGuanDong_CH;
	CStringArray m_strGuangdongWave;
	CMap<CString, LPCTSTR, CString, const CString&> m_mapCate2TgCH;
	CMap<CString, LPCTSTR, CString, const CString&> m_mapTg2StTgCH;
	CMap<CString, LPCTSTR, int, int> m_mapCheckGuangDongWave;
	CMap<CString, LPCTSTR, CString, const CString&> m_mapGuangDongDetail;
};
#include "HeaderPost.h"
