#if !defined(MMvhlStdEuroBSNewTempForRefactor_H)
#define MMvhlStdEuroBSNewTempForRefactor_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../wg_db/DB_ST_DT.h"
#include "../wg_db/DB_ST_DT_ELEM.h"
#include "../wg_db/DB_ST_DT_MOVE.h"


class CMMvhlStdEuroBSNewTempForRefactor // Euro코드 Refactoring하기 전에 Euro와 BS에 중복으로 들어가는 함수를 이 클래스에 잠시 넣어 둠... 나중에 이 클레스의 기능을 VhelDB에 넣어야 함..
{
public:
	CMMvhlStdEuroBSNewTempForRefactor(){};

	static int GetGridListSize(T_MVHL_D &m_Data, int nIndex, BOOL bPatchType = FALSE);
	static int GetGridListSizeSvSov( T_MVHL_D &m_Data, int nListSize );
	static int GetGridListSizeEuroModel3( T_MVHL_D &m_Data, int nListSize );
	static int GetGridListSizeCS454AllModel1( T_MVHL_D &m_Data, int nListSize );
	static int GetGridListSizeNetworkRailStandards(T_MVHL_D& m_Data, int nListSize);
	static int GetGridListSizeNetworkRailStandards2(T_MVHL_D& m_Data, int nListSize);
	static CString GetDescListTxt(T_MVHL_D &m_Data, int nIndex);
	static CString GetDescListTxtSvSov( T_MVHL_D &m_Data, CString strDescTxt );
	static CString GetDescListTxtCS454AllModel1( T_MVHL_D &m_Data, CString strDescTxt );
	static CString GetDescListTxtNetworkRailStandards(T_MVHL_D& m_Data, CString strDescTxt);
	static CString GetDescListTxtNetworkRailStandards2(T_MVHL_D& m_Data, CString strDescTxt);

	static CString GetDescListChk(T_MVHL_D &m_Data, int nIndex);

	static BOOL IsPatchTypeFatigueVehicle(T_MVHL_D& m_Data, int nIndex);
	static CString VehiclePatchTypeString(int nType);

};

#endif // !defined(MMvhlStdEuroBSNewTempForRefactor_H)
