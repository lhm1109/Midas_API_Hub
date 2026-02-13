// DgnToolMPhi_SeismicDesign.h: interface for the CDgnToolMPhi_SeismicDesign class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNTOOLMPHI_SEISMICDESIGN_H__E317FC61_6DDA_4EC0_9F41_0C7F92F63ECB__INCLUDED_)
#define AFX_DGNTOOLMPHI_SEISMICDESIGN_H__E317FC61_6DDA_4EC0_9F41_0C7F92F63ECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DB_ST_DT.h"

const int RLCC_ERR_TSG_SHAPE_SCALE = 101;
const int RLCC_ERR_TSG_RBAR_EQ	   = 102;
const int RLCC_ERR_SHEAR_RBAR	     = 103;

#include "..\wg_db\DbToolMphi.h"
#include "DgnSeismicDesignStruct.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnToolMPhi_SeismicDesign : public CDbToolMphi  
{
public:
	CDgnToolMPhi_SeismicDesign();
	virtual ~CDgnToolMPhi_SeismicDesign();

	// Member Variables.
protected:
	//int      m_iRchkPosiCvl_Gen_Main;
	//int      m_iRchkPosiCvl_Gen_Sub;
	//T_REBT_D m_tRchkColmCvl_Gen;

	//double  m_dWeightDensity;
	//double  m_da_tie;           //띠철근의 간격a
	//double  m_dAngle_tie[2];    //띠철근과 연직축과의 각도
	//double  m_dAw_tie[2];       //A와 φ로 배근되는 띠철근의 단면적Aw (0:교축방향, 1:교축직각방향)

	//int      m_iTypeEq_Rlco;// 1=Type1, 2=Type2

	// Member Functions.
protected:
	virtual void Init_InputData();

	virtual BOOL Get_MatlData(int iMatl, int iSect, MPHI_MATL_DATA& InData) override;
	virtual BOOL Get_SectData(int iElem, int iSect, int iPosi, int nMembType, int nMatlType, const MPHI_MATL_DATA& MatlD, const MPHI_USER_DATA& UserD, MPHI_SECT_DATA& InData) override;
	virtual BOOL Get_RbarData(int iElem, int iSect, int iPosi, int nMembType, const MPHI_SECT_DATA&SectD, MPHI_RBAR_DATA& InData) override;
	virtual BOOL Get_AreaUnit(int iElem, int iPosi, int nMembType, int nMatlType, int nSliceNum,
														BOOL bSlicing, const MPHI_RBAR_DATA& RbarD, MPHI_SECT_DATA& InData) override;
	virtual BOOL Get_RbarUnit(int nMembType, int nMatlType, double* dd, const MPHI_SECT_DATA&SectD, MPHI_MATL_DATA& MatlD, MPHI_RBAR_DATA& InData) override;
	virtual BOOL Get_UserData(int iElem, int iPosi, MPHI_USER_DATA& UserD) override;

protected:
	BOOL Calc_RCSinglePier(T_RLCO_D& RlcoD, T_RLCC_D& RlccD, _DGN_RESERVE_LATERAL_CAPA_MEMB_RES& MembResD, MPHI_DATA_ELEM& InData);

	double Calc_SelfWeight(CArray<T_ELEM_K, T_ELEM_K>& arElemK);

public:
	// M-Phi정보로 해당 부재의 곡률 및 변위를 계산함
	//   bMajorAxis : 검토방향이 yy방향인지 여부
	//   arElemK, arMoment, arMPhiStrn : 요소별 Key, Moment, M-Phi정보
	// < dDis : 계산된 변위를 넘겨받을 변수 
	//   nBasicPos : 기준점의 위치 1:I, 2:J, 3:Bottom, 4:Top
	//   dBasicTheta : 기준점의 Local축으로부터의 회전각(radian)
	BOOL CalcPhiDis(BOOL bMajorAxis, CArray<T_ELEM_K, T_ELEM_K>& arElemK, CArray<_DGN_MPHI_MOMENT_D, _DGN_MPHI_MOMENT_D>& arMoment, CArray<_DGN_MPHI_STRN_D, _DGN_MPHI_STRN_D&>& arMPhiStrn, double& dDis, int nBasicPos=3, double dBasicRx=0.0, double dBasicRy=0.0, double dBasicRz=0.0, BOOL bAbsDis=TRUE);
	double Calc_Phi(BOOL bMajorAxis, int iPosi, double dMoment, _DGN_MPHI_STRN_D& MPhiStrnD);

protected:
	double Get_dDeff(BOOL bAxisY, double dHeight, double dBw, MPHI_DATA_ELEM& InData);
	double Calc_cc(int nTypeEq);// 1=Type1, 2=Type2
	double Calc_ce(double dDeff);
	double Calc_cpt(double dPt);
	double Calc_ta(int nStressStrainType, double dfck);
	double Calc_cz(int nStressStrainType, int nRegionalKind, int nTypeEq);
	double Calc_Khc0(int nStressStrainType, int nTypeEq, int nGroundKind, double dT);

	CString Get_SectName(int nShape);
	CString Get_MembTypeName(int nType);
	CString Get_FailureName(int nType);

	void PrintErrorMessage(int nErrID, UINT ElemK);
	BOOL GetSectData(int iElem, int iSect, T_SECT_D& SectD);
	BOOL Calc_dDeff_Rebar(IN const BOOL bAxisY, MPHI_DATA_ELEM& InData, OUT double& dHeight);

public:
	BOOL Calc_RLCC(CArray<T_RLCC_K, T_RLCC_K>& arRlccK, _DGN_RESERVE_LATERAL_CAPA_RES& ResData);


//***// PMS.5568 Civil 일본 내진설계 대응
// 여기서 부터는 일본 교량 내진용 함수 추가
private:
	double Get_dBv(BOOL bAxisY, MPHI_SECT_DATA& InData);
	double Get_dBv_Gen(BOOL bAxisY, MPHI_SECT_DATA& InData);

	double Calc_Cdc(const double& bMajor, const double& da, const double& dDeff);// 전단 스팬에 의해 콘크리트가 부담하는 전단내력의 할증계수
	double Calc_Cds(const double& bMajor, const double& da, const double& dDeff);// 전단 스팬에 의해 띠철근이 부담하는 전단내력의 저감계수
};

#include "HeaderPost.h"

#endif // !defined(AFX_DGNTOOLMPHI_SEISMICDESIGN_H__E317FC61_6DDA_4EC0_9F41_0C7F92F63ECB__INCLUDED_)
