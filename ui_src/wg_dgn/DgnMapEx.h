//=========================================================
//	TITLE:		Map extention
//				for WinNT, MSVC 6.0, MFC 6.00
//				Copyright (C) Matrix Baltic Software
//				Vilnius, Lithuania
//	MODULE:		MapEx.h
//	PURPOSE:	Interface of the CMapEx class.
//
//	AUTHOR:		Audrius Vasiliauskas
// 
//	NOTES:		Added special hashkey for string maps
//
//=========================================================

#ifndef __DGN_MAP_EX_H__
#define __DGN_MAP_EX_H__

#ifndef __AFXTEMPL_H__
	#include <afxtempl.h>
#endif



////////////////////////////////////////////////////////////////////////////
//
// Special Hashkey for String Maps
//

template<> 
inline UINT AFXAPI HashKey<CString> (CString strKey)
{
	LPCTSTR key = strKey;
	UINT nHash = 0;
	while (*key)
	{
		nHash = (nHash<<5) + nHash + *key++;
	}
	return nHash;
}

template<> 
inline UINT AFXAPI HashKey<CString&> (CString& strKey)
{
	LPCTSTR key = strKey;
	UINT nHash = 0;
	while (*key)
	{
		nHash = (nHash<<5) + nHash + *key++;
	}
	return nHash;
}

////////////////////////////////////////////////////////////////////////////


template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class CDgnMapEx : public CMapEx<KEY, ARG_KEY, VALUE, ARG_VALUE>
{

public:
// Construction
	CDgnMapEx(int nBlockSize = 10);
	CDgnMapEx(const CDgnMapEx &x);

// assigment
	CDgnMapEx &operator = (const CDgnMapEx& x);

	// Accessing elements
	VALUE GetAt(int nIndex) const;

	// overwrite CArray methods
	int GetSize( ) const;
};


template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline CDgnMapEx<KEY, ARG_KEY, VALUE, ARG_VALUE>::CDgnMapEx(int nBlockSize)
	:CMapEx<KEY, ARG_KEY, VALUE, ARG_VALUE>(nBlockSize)
{
}


template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline CDgnMapEx<KEY, ARG_KEY, VALUE, ARG_VALUE>::CDgnMapEx(const CDgnMapEx &x)
{
	*this = x;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline CDgnMapEx<KEY, ARG_KEY, VALUE, ARG_VALUE> &
	   CDgnMapEx<KEY, ARG_KEY, VALUE, ARG_VALUE>::operator = (const CDgnMapEx& x)
{
	KEY rKey;
	VALUE rValue;
	POSITION pos = x.GetStartPosition();

	RemoveAll();
	while(pos != NULL)
	{
		x.GetNextAssoc(pos,rKey,rValue);
		SetAt(rKey,rValue);
	}
	return *this;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline VALUE CDgnMapEx<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetAt(int nIndex) const
{
	ASSERT(nIndex >= 0 && nIndex < GetCount());
	
	POSITION pos = GetStartPosition();
	KEY rKey;
	VALUE rValue;

	do	
	{
		GetNextAssoc(pos,rKey,rValue);
	}while(nIndex --);

	return rValue;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline int CDgnMapEx<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetSize( ) const
{
	return GetCount();
}


#endif