//
//////////////////////////////////////////////////////////////////////
#ifndef __CMTLGTDATADEF_H__
#define __CMTLGTDATADEF_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\UserDefType.h"

enum TRAIN_CODE
{
	TRAINCODE_UNKNOWN = 0,
	TRAINCODE_KOREA,
	TRAINCODE_UK,
    TRAINCODE_EUROCODE,
	TRAINCODE_USER,
	TRAINCODE_JAPANRAIL
};


enum TRAIN_TYPE
{
	TRAINTYPE_UNKNOWN = 0,
	TRAINTYPE_KOREA_MKH_D2,
	TRAINTYPE_KOREA_PMC_8,
	TRAINTYPE_KOREA_KTX_20,
	TRAINTYPE_KOREA_EL18_6,
	TRAINTYPE_KOREA_EL18_8,
	TRAINTYPE_KOREA_EL18_10,
	TRAINTYPE_UK_ATC_SINGLE_AXLE,
	TRAINTYPE_UK_ATC_DOUBLE_DECK_11,
	TRAINTYPE_UK_ATC_DOUBLE_DECK_10,
	TRAINTYPE_UK_ATC_DISTRIBUTED,
	TRAINTYPE_UK_CVN_CURRENT,
	TRAINTYPE_UK_CVN_OMNI,
	TRAINTYPE_UK_CVN_FUTURE,
	TRAINTYPE_UK_CVN_OMNI_28M,
	TRAINTYPE_UK_CVN_SEGMENTED_28M,
    TRAINTYPE_EUROCODE_A1,
    TRAINTYPE_EUROCODE_A2,
    TRAINTYPE_EUROCODE_A3,
    TRAINTYPE_EUROCODE_A4,
    TRAINTYPE_EUROCODE_A5,
    TRAINTYPE_EUROCODE_A6,
    TRAINTYPE_EUROCODE_A7,
    TRAINTYPE_EUROCODE_A8,
    TRAINTYPE_EUROCODE_A9,
    TRAINTYPE_EUROCODE_A10,
    TRAINTYPE_EUROCODE_B,
	TRAINTYPE_USER,
	TRAINCODE_JAPANRAIL_N16,
	TRAINCODE_JAPANRAIL_P16,
	TRAINCODE_JAPANRAIL_P17,
	TRAINCODE_JAPANRAIL_M18
};

enum DIR_TYPE
{
	DIR_X = 0,
	DIR_Y,
	DIR_Z,
	DIR_N_X,
	DIR_N_Y,
	DIR_N_Z
};

struct TLGT_NODE_ITEM
{
	T_NODE_K KeyNode;
	double dDistance; // 이전 노드와의 거리
	void Initialize()
	{
		KeyNode = 0;
		dDistance = 0.0;
	}
};


struct TLGT_ITEM
{
	double dLength;
	double dForce;
	
	void Initialize()
	{
		dLength = 0;
		dForce = 0;
	}
	
	TLGT_ITEM() { Initialize(); }
	TLGT_ITEM(const TLGT_ITEM& src) { *this = src; }
	
	TLGT_ITEM& operator=(const TLGT_ITEM& src)
	{
		dLength = src.dLength;
		dForce = src.dForce;

		return *this;
	}
};

typedef CArray<TLGT_ITEM, TLGT_ITEM&> ARR_TLGT_ITEM;

// Train Load Generator
struct TLGT_INFO
{
	CArray<T_NODE_K, T_NODE_K> arKeyNode;	// Node Array
	T_THIS_K		keyThis;				// Time Load Case
	CString			strName;				// Name
	int				nVehicleCode;			// Vehicle Code
	int				nVehicleType;			// Vehicle Type
	int				nNumWheel;				// Number of Wheels
	double			dVelocity;				// Train Velocity
	int				nScaling;				// 0:Scale Factor, 1:Maximum Value
	double			dSacleFactor;			// 스케일팩터
	double			dMaximumValue;			// 최대값
	double			dTime;					// 시간
	int				nDir;					// 0: X,	1: Y,	2: Z
	int				nNumVehicle;			// Number of Vehicle
	ARR_TLGT_ITEM	arTlgtItem;				// Item Array
	
	TLGT_INFO() { Initialize(); }
	TLGT_INFO(const TLGT_INFO& src) { *this = src; }
	
	TLGT_INFO& operator=(const TLGT_INFO& src)
	{
		arKeyNode.Copy(src.arKeyNode);
		keyThis			= src.keyThis		;
		strName			= src.strName		;	
		nVehicleCode	= src.nVehicleCode	;
		nVehicleType	= src.nVehicleType	;
		nNumWheel		= src.nNumWheel		;
		dVelocity		= src.dVelocity		;
		nScaling		= src.nScaling		;
		dSacleFactor	= src.dSacleFactor	;
		dMaximumValue	= src.dMaximumValue	;
		dTime			= src.dTime			;
		nDir			= src.nDir			;
		nNumVehicle		= src.nNumVehicle   ;
		arTlgtItem.Copy(src.arTlgtItem);
		return *this;
	}

	void Initialize()
	{
		arKeyNode.RemoveAll();
		keyThis = 0;
		strName = "";
		nVehicleCode = TRAINCODE_KOREA;
		nVehicleType = TRAINTYPE_KOREA_MKH_D2;
		nNumWheel = 0;
		dVelocity = 0.0;
		nScaling = 0;
		dSacleFactor = 1.0;
		dMaximumValue = 0.0;
		dTime = 0.0;
		nDir = DIR_N_Z;
		nNumVehicle = 1;
		arTlgtItem.RemoveAll();
	}
	void ConvertUnitOut();
	void ConvertUnitIn();
};

struct TLGT_INFO_900
{
	CArray<T_NODE_K, T_NODE_K> arKeyNode;	// Node Array
	T_THIS_K		keyThis;				// Time Load Case
	CString			strName;				// Name
	int				nVehicleCode;			// Vehicle Code
	int				nVehicleType;			// Vehicle Type
	int				nNumWheel;				// Number of Wheels
	double			dVelocity;				// Train Velocity
	int				nScaling;				// 0:Scale Factor, 1:Maximum Value
	double			dSacleFactor;			// 스케일팩터
	double			dMaximumValue;			// 최대값
	double			dTime;					// 시간
	int				nDir;					// 0: X,	1: Y,	2: Z
	ARR_TLGT_ITEM	arTlgtItem;				// Item Array

	TLGT_INFO_900() { Initialize(); }
	TLGT_INFO_900(const TLGT_INFO_900& src) { *this = src; }

	TLGT_INFO_900& operator=(const TLGT_INFO_900& src)
	{
		arKeyNode.Copy(src.arKeyNode);
		keyThis = src.keyThis;
		strName = src.strName;
		nVehicleCode = src.nVehicleCode;
		nVehicleType = src.nVehicleType;
		nNumWheel = src.nNumWheel;
		dVelocity = src.dVelocity;
		nScaling = src.nScaling;
		dSacleFactor = src.dSacleFactor;
		dMaximumValue = src.dMaximumValue;
		dTime = src.dTime;
		nDir = src.nDir;
		arTlgtItem.Copy(src.arTlgtItem);
		return *this;
	}

	void Initialize()
	{
		arKeyNode.RemoveAll();
		keyThis = 0;
		strName = "";
		nVehicleCode = TRAINCODE_KOREA;
		nVehicleType = TRAINTYPE_KOREA_MKH_D2;
		nNumWheel = 0;
		dVelocity = 0.0;
		nScaling = 0;
		dSacleFactor = 1.0;
		dMaximumValue = 0.0;
		dTime = 0.0;
		nDir = DIR_N_Z;
		arTlgtItem.RemoveAll();
	}

	BOOL ConvertToLatest(TLGT_INFO& rData)
	{
		rData.arKeyNode.Copy(arKeyNode);
		rData.keyThis = keyThis;
		rData.strName = strName;
		rData.nVehicleCode = nVehicleCode;
		rData.nVehicleType = nVehicleType;
		rData.nNumWheel = nNumWheel;
		rData.dVelocity = dVelocity;
		rData.nScaling = nScaling;
		rData.dSacleFactor = dSacleFactor;
		rData.dMaximumValue = dMaximumValue;
		rData.dTime = dTime;
		rData.nDir = nDir;
		rData.nNumVehicle = 1;
		rData.arTlgtItem.Copy(arTlgtItem);

		return TRUE;
	}
};

#endif