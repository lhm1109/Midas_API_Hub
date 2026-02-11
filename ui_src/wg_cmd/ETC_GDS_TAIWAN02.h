#pragma once

#include "MyChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"
class CSpfcTW2006Util;
using namespace mit::frx;

//typedef CArray<double, double> SeisDblArray;
//
//enum EN_TAIWAN_SPECTRUM_TYPE : unsigned int
//{
//	enSpectTypeHOR = 0, // Horizontal
//	enSpectTypeVER, // Vertical
//};
//
//enum EN_TAIWAN_SEIS_ZONE : unsigned int
//{
//	enSeisZoneGEN = 0, // General Zone
//	enSeisZoneNFZ, // Near Fault Zone
//	enSeisZoneTPB, // Taipei Basin
//};
//
//enum EN_TAIWAN_SEIS_SUBZONE : unsigned int
//{
//	enSubZoneTPB1 = 0, // Taipei Basin 1
//	enSubZoneTPB2, // Taipei Basin 2
//	enSubZoneTPB3, // Taipei Basin 3
//	enSubZoneTPB4, // Taipei Basin 4
//	enSubZoneUSER, // User Input
//};
//
//enum EN_TAIWAN_SPECTRUM_USED : unsigned int
//{
//	enSpectUsedDGN = 0, // Design
//	enSpectUsedMED, // Small Medium
//	enSpectUsedMAX, // Maximum
//};
//
//enum EN_TAIWAN_SOIL_TYPE : unsigned int
//{
//	enSoilTypeTYP1 = 0,
//	enSoilTypeTYP2,
//	enSoilTypeTYP3,
//	enSoilTypeUSER,
//};

class CETC_GDS_TAIWAN02 : public CMyChildDialog
{
public:
	CETC_GDS_TAIWAN02(T_SPFC_D* pParamData, CWnd* pParent = NULL);
	enum { IDD = IDD_ETC_GDS_TAIWAN02 };

private:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	virtual void CtrlManager();
	virtual void InitSpecAccNearFaultZone();

private:
	virtual BOOL Dlg2Data();
	virtual void SetPushOverData(BOOL bInit=TRUE);
	//virtual void SetInitValueforPOSpectrum();
	//virtual bool IsFundamentalPeriodREQ();
	//virtual bool IsValidNearFaultFactor(CString& rcsMsg);
	
	virtual void ChangeCaption();
	virtual void ChangeCaptionSmallMedium();
	//virtual UINT GetSeismicCode() { return D_SPFC_CODE_TAIWAN06; };
	//virtual CString GetFunctionName() { return _T("Taiwan(2006)"); };
	//virtual CString GetFunctionDescription();
	//virtual double CalcSaEveryZone(stETC_GDS_TAIWAN02_CALC& sCalc, double dTn, double dT1, double dSRa, double dSRv, BOOL bPOcurve);
	virtual double GetInitFundamentalPeriod();

	//virtual bool CalcGeneralZone(stETC_GDS_TAIWAN02_CALC& sCalc);
	//virtual bool CalcNearFaultZone(stETC_GDS_TAIWAN02_CALC& sCalc);
	//virtual bool CalcTaipeiBasin(stETC_GDS_TAIWAN02_CALC& sCalc);
	//virtual void CalcSiteMagnifyFactor();
	virtual void GetSiteMagnifyFactor();

private:
	afx_msg void OnSelChangeCmbSeisZone();
	afx_msg void OnSelChangeCmbSpecType();
	afx_msg void OnSelChangeCmbSpecCategory();
	afx_msg void OnEditChangeCmbSpecAcc();
	afx_msg void OnSelChangeCmbSpecAcc();
	afx_msg void OnSelChangeCmbSubZone();
	afx_msg void OnSelChangeCmbSoilType();
	afx_msg void OnEditChangeEdtNSFactor();
	afx_msg void OnEditChangeEdtSMFactor();
	afx_msg void OnEditChangeEdtTaipei();
	afx_msg void OnEditChangeEdtFundPeriod();
	afx_msg void OnBtnPeriodCalc();
	DECLARE_MESSAGE_MAP()

protected:
	void InitCtrlInfo();
	void GetFinalValue();
	void EnableCtrlFundamentPeriod();
	void HideCtrlForPushOver();
	void SetComboData(MComboBox& cobx, const double& dValue);
	void ChangeCtrlText(UINT nCtrlID, const CString& csCaption);

	//double GetSaFactor(stETC_GDS_TAIWAN02_CALC& sCalc, const double dT, int nMIN);
	//double GetFuFactor(double& dT, const double dT0, int nMIN);
	//double GetModifiedSaFu(double dSa, double dFu);

	//CString GetSeismicZoneName(int nCode);
	//CString GetSoilTypeName(int nCode);
	//CString GetSubZoneName(int nCode);
	//CString GetSpecCategoryName(int nCode);

private:
	//void MakeSpectrumData(BOOL bOnlyCalc=FALSE);
	//void MakePOSpectrumData(double dDamping, double SRa, double SRv, SeisDblArray& aPeriod, SeisDblArray& aAccel);
	//void MakePOSpectrumData_sub(double dDamping, double SRa, double SRv, SeisDblArray& radPeriod, SeisDblArray& radAccel, BOOL bMakeSpectrum=FALSE, BOOL bOnlyCalc=FALSE);
	BOOL ISVALID();
	void End();
	void EndNotClose() {};

private:
	void InitSpecAcc();
	void InitSiteMagnifyFactor();
	void InitTaipei();
	void InitIFactorCombo();
	void InitAyFactorCombo();
	void InitRCombo();
	void AlignCtrl();
	void InitComboBox(MComboBox& rCombo, std::vector<CString> vcsItem);

private:
	void SetSpectralData();
	void SetSiteMagnifyFactor();
	void SetChangeTaipei();

	//bool GetBFactorbyDamping(stETC_GDS_TAIWAN02_CALC& rstCalc, const double& dDamping);
	//void TransPointEveryZone(stETC_GDS_TAIWAN02_CALC sCalc, double dSRa, double dSRv, double dTn, double dT1);
	//void TransPointPOSpectrum(stETC_GDS_TAIWAN02_CALC sCalc, double dSRa, double dSRv, const double& dt, const double& dTol,
	//	double& rdSa, double dTx, const double& dTn, const double& dT1,
	//	SeisDblArray& radPeriod, SeisDblArray& radAccel, BOOL bPOcurve);

private:
	//double LinearInt(double dx1, double dx2, double dy1, double dy2, double dxx);
	double GetBuildingHeight();

protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcTW2006Util> m_pSpfcUtil;

	//int m_nSpecCategory; // 0:Design ,1: Small-Medium, 2:Maximum
	//int m_nSeisZone;     // 0:General, 1:Near fault, 2:Taipei Basin
	//int m_nSpecType;     // Horizontal, Vertical
	//// - General / Near Fault Zone
	//double m_dSds;        double m_dSd1;    // d -> design ,       m -> maximum
	//double m_dSms;        double m_dSm1;    // s -> short period,  1 -> 1 sec
	//int    m_nSoilType;                     // Site Magnify Factor
	//double m_dFda;        double m_dFdv;
	//double m_dFma;        double m_dFmv;
	//// Near Source Factor(Near Fault Zone only)
	//double m_dNda;        double m_dNdv;
	//double m_dNma;        double m_dNmv;
	//double m_dFda_1;      double m_dFdv_1;    //Fa, Fv 산정시 (Na=1.0)*Sds, (Nv=1.0)*Sd1 에 의해 구함
	//// Taipei Basin only
	//int    m_nSubZone;
	//double m_dSds_t;      double m_dSms_t;
	//double m_dTd0_t;      double m_dTm0_t;

	//double m_dI;
	//double m_dAy;
	//double m_dR;
	//double m_dDamping;
	//double m_dMaxPeriod;
	//double m_dFundPeriod;

protected:
	CArray<UINT, UINT> m_aCtrlMethod0;
	CArray<UINT, UINT> m_aCtrlMethod1;
	CArray<UINT, UINT> m_aCtrlMethod2;
	CArray<UINT, UINT> m_aCtrl1, m_aCtrl2;

protected:
	MComboBox m_cmbSpecCategory;
	MComboBox m_cmbSpecType;
	MComboBox m_cmbSeisZone;
	MComboBox m_cmbSoilType;
	MComboBox m_cmbSubZone;
	MComboBox m_cmbSds;
	MComboBox m_cmbSd1;
	MComboBox m_cmbSms;
	MComboBox m_cmbSm1;
	CEditUnit m_edtFmv;
	CEditUnit m_edtFma;
	CEditUnit m_edtFdv;
	CEditUnit m_edtFda;
	CEditUnit m_edtNmv;
	CEditUnit m_edtNma;
	CEditUnit m_edtNdv;
	CEditUnit m_edtNda;
	CEditUnit m_edtSds_t;
	CEditUnit m_edtSms_t;
	CEditUnit m_edtTm0;
	CEditUnit m_edtTd0;

	CEditUnit m_edtT1;
	MComboBox m_cmbI;
	MComboBox m_cmbAlpha;
	MComboBox m_cmbR;
	CEditUnit m_edtDamping;
	CEditUnit m_edtMaxPeriod;
};
