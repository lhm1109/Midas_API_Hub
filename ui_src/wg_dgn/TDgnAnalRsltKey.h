#pragma once

template <typename TEnumDataType, typename TDataKey>
class TDgnAnalRsltKey
{
public:
    TDgnAnalRsltKey();
    TDgnAnalRsltKey(const TEnumDataType Type, TDataKey Key);
    TDgnAnalRsltKey(const TDgnAnalRsltKey& src);

    bool operator < (const TDgnAnalRsltKey& rhs) const;

    bool IsValid() const;

    const TEnumDataType& GetType() const;
    const TDataKey& GetKey() const;

public:
    TEnumDataType Type;
    TDataKey Key;

private:
    enum
    {
        enNone = 0,
    };
};

#include "TDgnAnalRsltKey.inl"
