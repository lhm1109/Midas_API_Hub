template <typename TEnumDataType, typename TDataKey>
TDgnAnalRsltKey<TEnumDataType, TDataKey>::TDgnAnalRsltKey() :
    Type(static_cast<TEnumDataType>(enNone)),
    Key(static_cast<TDataKey>(enNone))
{
}

template <typename TEnumDataType, typename TDataKey>
TDgnAnalRsltKey<TEnumDataType, TDataKey>::TDgnAnalRsltKey(const TEnumDataType Type, TDataKey Key) :
    Type(Type),
    Key(Key)
{
}

template <typename TEnumDataType, typename TDataKey>
TDgnAnalRsltKey<TEnumDataType, TDataKey>::TDgnAnalRsltKey(const TDgnAnalRsltKey& src)
{
    this->Type = src.Type;
    this->Key = src.Key;
}

template <typename TEnumDataType, typename TDataKey>
bool TDgnAnalRsltKey<TEnumDataType, TDataKey>::operator < (const TDgnAnalRsltKey& rhs) const
{
    // ±ÍÂú´Ù
    return std::make_pair(this->Type, this->Key) < std::make_pair(rhs.Type, rhs.Key);
}

template <typename TEnumDataType, typename TDataKey>
bool TDgnAnalRsltKey<TEnumDataType, TDataKey>::IsValid() const
{
    if ( this->Type <= enNone )
    {
        ASSERT(0); return false;
    }

    if ( this->Key <= enNone )
    {
        ASSERT(0); return false;
    }

    return true;
}

template <typename TEnumDataType, typename TDataKey>
const TEnumDataType& TDgnAnalRsltKey<TEnumDataType, TDataKey>::GetType() const
{
    return Type;
}

template <typename TEnumDataType, typename TDataKey>
const TDataKey& TDgnAnalRsltKey<TEnumDataType, TDataKey>::GetKey() const
{
    return Key;
}