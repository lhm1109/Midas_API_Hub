#pragma once

#include <map>
#include "..\wg_db\DB_ST_DN_ENUM.h"
#include "..\wg_db\DB_ST_DT_SEIS_ENUM.h"

#define DISP_COMP_DX   0
#define DISP_COMP_DY   1
#define DISP_COMP_DZ   2
#define DISP_COMP_RX   3
#define DISP_COMP_RY   4
#define DISP_COMP_RZ   5

#define STEK_POSI_I		0
#define STEK_POSI_J		1
#define STEK_AXIS		0
#define STEK_SHEAR_Y	1
#define STEK_SHEAR_Z	2
#define STEK_TORSION	3
#define STEK_MOMENT_Y	4
#define STEK_MOMENT_Z	5

#define STCK_POSI_I		0
#define STCK_POSI_J		1
#define STCK_COM_AXIS		0
#define STCK_COM_SHEAR_Y	1
#define STCK_COM_SHEAR_Z	2
#define STCK_COM_TORSION	3
#define STCK_COM_MOMENT_Y	4
#define STCK_COM_MOMENT_Z	5
#define STCK_VAL_AXIS		0
#define STCK_VAL_SHEAR_Y	1
#define STCK_VAL_SHEAR_Z	2
#define STCK_VAL_TORSION	3
#define STCK_VAL_MOMENT_Y	4
#define STCK_VAL_MOMENT_Z	5

#define STRB_POSI_I		0
#define STRB_POSI_J		3
#define STRB_AXIS_I		0
#define STRB_SHEAR_Y_I	1
#define STRB_SHEAR_Z_I	2
#define STRB_TORSION_I	3
#define STRB_MOMENT_Y_I	4
#define STRB_MOMENT_Z_I	5
#define STRB_AXIS_J		6
#define STRB_SHEAR_Y_J	7
#define STRB_SHEAR_Z_J	8
#define STRB_TORSION_J	9
#define STRB_MOMENT_Y_J	10
#define STRB_MOMENT_Z_J	11

#define SBCF_POSI_I			0
#define SBCF_POSI_J			3
#define SBCF_POSI_SUB_I		0
#define SBCF_POSI_SUB_J		1
#define SBCF_COM_AXIS		0
#define SBCF_COM_SHEAR_Y	1
#define SBCF_COM_SHEAR_Z	2
#define SBCF_COM_TORSION	3
#define SBCF_COM_MOMENT_Y	4
#define SBCF_COM_MOMENT_Z	5
#define SBCF_VAL_AXIS		0
#define SBCF_VAL_SHEAR_Y	1
#define SBCF_VAL_SHEAR_Z	2
#define SBCF_VAL_TORSION	3
#define SBCF_VAL_MOMENT_Y	4
#define SBCF_VAL_MOMENT_Z	5

#define INMF_POSI_I		0
#define INMF_POSI_J		1
#define INMF_AXIS		0
#define INMF_SHEAR_Y	1
#define INMF_SHEAR_Z	2
#define INMF_TORSION	3
#define INMF_MOMENT_Y	4
#define INMF_MOMENT_Z	5

#define LOCAL_AXIS_Y 0
#define LOCAL_AXIS_Z 1

class CDBDoc;
class CDgnProgressDlg;

//휨 조사 인터페이스
class iSeismicDesignSibd
{
public:
	virtual void AddProgressStep(CDgnProgressDlg* pProgress) = 0;
	virtual BOOL Calculate(CDgnProgressDlg* pProgress) = 0;
};
//변위 소성율 조사 인터페이스
class iSeismicDesignSidp
{
public:
	virtual void AddProgressStep(CDgnProgressDlg* pProgress) = 0;
	virtual BOOL Calculate(CDgnProgressDlg* pProgress) = 0;
};
//전단 조사 인터페이스
class iSeismicDesignSisr
{
public:
	virtual void AddProgressStep(CDgnProgressDlg* pProgress) = 0;
	virtual BOOL Calculate(CDgnProgressDlg* pProgress) = 0;
};
//그 외 조사 인터페이스
class iSeismicDesignSiet
{
public:
	virtual void AddProgressStep(CDgnProgressDlg* pProgress) = 0;
	virtual BOOL Calculate(CDgnProgressDlg* pProgress) = 0;
};

//요약결과
class iSeismicDesignSumm
{
public:
	virtual void AddProgressStep(CDgnProgressDlg* pProgress) = 0;
	virtual BOOL Calculate(CDgnProgressDlg* pProgress) = 0;
};

class CSeismicDesignBase
{
protected:
	struct P_Member
	{
		DB_MEMB_TYPE nType;
		seis_jp::eDirShear nDirShear;
		int nMaterialType;

		void Init()
		{
			nType = D_MBTP_ETC;
			nDirShear = seis_jp::kDirShear_0;
		}
	};

	CDBDoc* m_pDoc;
	CDgnProgressDlg* m_pProgress;
	std::map<T_ELEM_K, P_Member> m_mMemberMap;

	CSeismicDesignBase();
	virtual ~CSeismicDesignBase();

	void GetStrKeyList(const CArray<unsigned int, unsigned int>& keyList, OUT CString& rStr);

	BOOL GetTimeIndex(const T_THIS_K& kThis, const double& nStep, OUT int& nTimeIndex);

	int GetCurrentDgnCode();

	/**
	* 휨, 전단 조사의 대한 공통 함수
	* T_SIGR_K 에 대한 조사 대상 요소 별로 P_Member 구조체를 만들고 m_mMemberMap 에 넣는다.
	*/
	void MakeMemberMap(const T_SIGR_K& kSigr, OUT T_ELEM_K_LIST& rElemList);

public:
	void SetProgressDlg(CDgnProgressDlg* pProgress) { m_pProgress = pProgress; };

	enum eVector2d
	{
		kDx,
		kDy,
		kMax2d,
	};

	enum eVector3d
	{
		k3Dx,
		k3Dy,
		k3Dz,
		kMax3d,
	};

	enum ePos
	{
		k_I,
		k_J,
		kPosSize,
	};
};