#ifndef __DB_ST_DT_MTCS_H__
#define __DB_ST_DT_MTCS_H__

#define MIDAS_SDK

#pragma pack(push, 1)

/// mtcs ///
// Material Coordinates System
#define T_MTCS_K unsigned int
struct T_MTCS_D
{
	CString strName;
	double dOrigin[3];      // L
	double dPtOnXAxis[3];   // L
	double dPtOnXY[3];      // L
	T_MTCS_D()
	{
	}
	T_MTCS_D(const T_MTCS_D& src)
	{
		*this = src;
	}
	T_MTCS_D& operator=(const T_MTCS_D& src)
	{
		strName = src.strName;
		for(int i=0; i<3; i++) dOrigin[i]    = src.dOrigin[i];
		for(int j=0; j<3; j++) dPtOnXAxis[j] = src.dPtOnXAxis[j];
		for(int k=0; k<3; k++) dPtOnXY[k]    = src.dPtOnXY[k];
		return *this;
	}
	void Initialize()
	{
		strName.Empty();

		for(int i=0; i<3; i++) dOrigin[i]    = 0.0;
		for(int j=0; j<3; j++) dPtOnXAxis[j] = 0.0;
		for(int k=0; k<3; k++) dPtOnXY[k]    = 0.0;
	}
};
struct T_MTCS_D_CH
{
	char strName[80];
	double dOrigin[3];      // L
	double dPtOnXAxis[3];   // L
	double dPtOnXY[3];      // L

	void ConvertToString(T_MTCS_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		memcpy(rData.dOrigin, dOrigin, sizeof(dOrigin));
		memcpy(rData.dPtOnXAxis, dPtOnXAxis, sizeof(dPtOnXAxis));
		memcpy(rData.dPtOnXY, dPtOnXY, sizeof(dPtOnXY));
	}
	void ConvertToChar(T_MTCS_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		memcpy(dOrigin, rData.dOrigin, sizeof(dOrigin));
		memcpy(dPtOnXAxis, rData.dPtOnXAxis, sizeof(dPtOnXAxis));
		memcpy(dPtOnXY, rData.dPtOnXY, sizeof(dPtOnXY));
	}
};
struct T_MTCS_UDRD_D
{
	T_MTCS_K key;
	T_MTCS_D data;
};
struct T_MTCS_UDRD_D_CH
{
	T_MTCS_K key;
	T_MTCS_D_CH data;
};
struct T_MTCS_UNIT
{
	int strName;
	int dOrigin[3];      // L
	int dPtOnXAxis[3];   // L
	int dPtOnXY[3];      // L

	T_MTCS_UNIT()
	{
		strName = D_UNITSYS_NONE;
		dOrigin[0] = D_UNITSYS_BASE_LENGTH;
		dOrigin[1] = D_UNITSYS_BASE_LENGTH;
		dOrigin[2] = D_UNITSYS_BASE_LENGTH;
		dPtOnXAxis[0] = D_UNITSYS_BASE_LENGTH;
		dPtOnXAxis[1] = D_UNITSYS_BASE_LENGTH;
		dPtOnXAxis[2] = D_UNITSYS_BASE_LENGTH;
		dPtOnXY[0] = D_UNITSYS_BASE_LENGTH;
		dPtOnXY[1] = D_UNITSYS_BASE_LENGTH;
		dPtOnXY[2] = D_UNITSYS_BASE_LENGTH;
	}
};

//#ifdef _MDEMO
//  #define HASHSIZEMTCS 101
//#else
	#define HASHSIZEMTCS 101
//#endif
#define D_MTCS_MINNUM       1   // 입력가능한 최소 번호
#define D_MTCS_MAXNUM   99999   // 입력가능한 최대 번호

#pragma pack(pop)

#endif  // __DB_ST_DT_MTCS_H__
