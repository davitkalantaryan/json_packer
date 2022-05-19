//
// file:            jsonpacker_packer.cpp
// path:			src/core/jsonpacker_packer.cpp
// created on:		2022 May 11
// created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
//

#include <jsonpacker/packer.hpp>
#include <string>
#include <map>
#include <fstream>
#include <cstdlib>
#include <stdint.h>
#include <jsonpacker/disable_utils_warnings.h>
#include <QJsonDocument>
#include <QByteArray>
#include <QJsonObject>
#include "tlv_box.h"

#define JSON_PACKER_MAX_SIZE    4096  // one page
#define JSON_PACKER_OUT_BOX_TYPE    1

namespace jsonpacker{ namespace packer{

typedef std::pair<int,QJsonValue::Type> TypeDictSecond; // this is done compilacated for future use (if we check type consistency per key)
typedef ::std::map<QString,TypeDictSecond> MapType;    // this is done compilacated for future use (if we check type consistency per key)
// if type consistency per key is not important, then instead upper 2 maps, we can have below simple map
// typedef ::std::map<QString,int> MapType; //

template <typename stringT>
JSONPACKER_DLL_PRIVATE stringT StringToUString(const char* a_strIn, size_t a_strLen);
template <>
JSONPACKER_DLL_PRIVATE std::string StringToUString<std::string>(const char* a_strIn, size_t a_strLen);
template <>
JSONPACKER_DLL_PRIVATE std::wstring StringToUString<std::wstring>(const char* a_strIn, size_t a_strLen);
template <>
JSONPACKER_DLL_PRIVATE std::u16string StringToUString<std::u16string>(const char* a_strIn, size_t a_strLen);
template <>
JSONPACKER_DLL_PRIVATE std::u32string StringToUString<std::u32string>(const char* a_strIn, size_t a_strLen);

template <typename CharOut>
static bool SingleDataToFile(const ::std::string& a_inpStr, ::std::basic_ostream<CharOut>& a_out, MapType* a_pMap);

static ::std::string UStringToString(const char* a_strIn, size_t a_count);
static ::std::string UStringToString(const wchar_t*  a_str, size_t a_count);
static ::std::string UStringToString(const char16_t* a_str, size_t a_count);
static ::std::string UStringToString(const char32_t* a_str, size_t a_count);



/*//////////////////////////////////////////////////////////////////////////*/


JSONPACKER_EXPORT bool Pack(const char* a_inpFileName, const char* a_outFileName)
{
    ::std::fstream  fileIn, fileOut;
    fileIn.open(a_inpFileName,std::ios_base::in|std::ios_base::binary);
    if(!fileIn.is_open()){return false;}
    fileOut.open(a_outFileName,std::ios_base::out|std::ios_base::binary);
    if(!fileOut.is_open()){return false;}
    return Pack(fileIn,fileOut);
}


template <typename CharIn>
JSONPACKER_EXPORT bool Pack(::std::basic_istream<CharIn>& a_inp, const char* a_outFileName)
{
    ::std::fstream  fileOut;
    fileOut.open(a_outFileName,std::ios_base::out|std::ios_base::binary);
    if(!fileOut.is_open()){return false;}
    return Pack(a_inp,fileOut);
}


template <typename CharOut>
JSONPACKER_EXPORT bool Pack(const char* a_inpFileName, ::std::basic_ostream<CharOut>& a_out)
{
    ::std::fstream  fileIn;
    fileIn.open(a_inpFileName,std::ios_base::in|std::ios_base::binary);
    if(!fileIn.is_open()){return false;}
    return Pack(fileIn,a_out);
}


template <typename CharIn, typename CharOut>
JSONPACKER_EXPORT bool Pack(::std::basic_istream<CharIn>& a_inp, ::std::basic_ostream<CharOut>& a_out)
{
    try{
        MapType aMap;
        ::std::basic_string<CharIn> inString(JSON_PACKER_MAX_SIZE,0);
        CharIn* pcBuffer = const_cast<CharIn*>(inString.data());
        int nLinesParsed(0);

        const typename std::basic_ostream<CharOut>::pos_type initPos =  a_out.tellp();

        while(!a_inp.eof()){

            a_inp.getline(pcBuffer,JSON_PACKER_MAX_SIZE);
            if(a_inp.fail()){
                // we have line with more than 4095 characters
                if(a_inp.gcount()>(JSON_PACKER_MAX_SIZE-2)){
                    // todo: report that we have larger than 4096 byte line
                    return false;
                }
                break;
            }
            const size_t cunCount = a_inp.gcount();
            const ::std::string inpStr = UStringToString(pcBuffer,cunCount);
            if(SingleDataToFile(inpStr,a_out,&aMap)){
                ++nLinesParsed;
            }
            else{
                //return false;
                // just report that this line is not json
            }

        } // while(!a_inp.eof()){

        if(nLinesParsed==0){
            return false; // no any JSON line found
        }

        // let's put dictonary itself
        tlv::TlvBox aTlvBoxInner, aTlvBoxOut;
        MapType::const_iterator iter = aMap.begin();
        const MapType::const_iterator iterEnd = aMap.end();

        for(;iter!=iterEnd;++iter){
            aTlvBoxInner.PutStringValue(iter->second.first,iter->first.toStdString());
        }

        if (!aTlvBoxInner.Serialize()) { // I'm not sure if this is needed to put this to bigger box (no time to check, and performance is not imp.)
            return false;
        }

        aTlvBoxOut.PutObjectValue(JSON_PACKER_OUT_BOX_TYPE, aTlvBoxInner);

        if (!aTlvBoxOut.Serialize()) {
            return false;
        }

        const size_t outBufferSize (size_t(aTlvBoxOut.GetSerializedBytes()));
        if(outBufferSize<1){return false;}

        const char* pcOutput = reinterpret_cast<char*>(aTlvBoxOut.GetSerializedBuffer());
        if(!pcOutput){
            return false;
        }
        a_out.seekp(initPos); // this can throw std::ios_base::failure
        ::std::basic_string<CharOut> outStr = StringToUString<std::basic_string<CharOut> >(pcOutput,outBufferSize);
        a_out.write(outStr.c_str(),outStr.size());
    }
    catch(...){  // generally speaking we will have  std::ios_base::failure, so we can write `catch(const  std::ios_base::failure&)`
        return false;
    }

    return a_out.fail()?false:true;
}

template JSONPACKER_EXPORT bool Pack<char,char> (::std::basic_istream<char>& a_inp, ::std::basic_ostream<char>& a_out);
template JSONPACKER_EXPORT bool Pack<char,wchar_t> (::std::basic_istream<char>& a_inp, ::std::basic_ostream<wchar_t>& a_out);
template JSONPACKER_EXPORT bool Pack<char,char16_t> (::std::basic_istream<char>& a_inp, ::std::basic_ostream<char16_t>& a_out);
template JSONPACKER_EXPORT bool Pack<char,char32_t> (::std::basic_istream<char>& a_inp, ::std::basic_ostream<char32_t>& a_out);
template JSONPACKER_EXPORT bool Pack<wchar_t,char> (::std::basic_istream<wchar_t>& a_inp, ::std::basic_ostream<char>& a_out);
template JSONPACKER_EXPORT bool Pack<wchar_t,wchar_t> (::std::basic_istream<wchar_t>& a_inp, ::std::basic_ostream<wchar_t>& a_out);
template JSONPACKER_EXPORT bool Pack<wchar_t,char16_t> (::std::basic_istream<wchar_t>& a_inp, ::std::basic_ostream<char16_t>& a_out);
template JSONPACKER_EXPORT bool Pack<wchar_t,char32_t> (::std::basic_istream<wchar_t>& a_inp, ::std::basic_ostream<char32_t>& a_out);
template JSONPACKER_EXPORT bool Pack<char16_t,char> (::std::basic_istream<char16_t>& a_inp, ::std::basic_ostream<char>& a_out);
template JSONPACKER_EXPORT bool Pack<char16_t,wchar_t> (::std::basic_istream<char16_t>& a_inp, ::std::basic_ostream<wchar_t>& a_out);
template JSONPACKER_EXPORT bool Pack<char16_t,char16_t> (::std::basic_istream<char16_t>& a_inp, ::std::basic_ostream<char16_t>& a_out);
template JSONPACKER_EXPORT bool Pack<char16_t,char32_t> (::std::basic_istream<char16_t>& a_inp, ::std::basic_ostream<char32_t>& a_out);
template JSONPACKER_EXPORT bool Pack<char32_t,char> (::std::basic_istream<char32_t>& a_inp, ::std::basic_ostream<char>& a_out);
template JSONPACKER_EXPORT bool Pack<char32_t,wchar_t> (::std::basic_istream<char32_t>& a_inp, ::std::basic_ostream<wchar_t>& a_out);
template JSONPACKER_EXPORT bool Pack<char32_t,char16_t> (::std::basic_istream<char32_t>& a_inp, ::std::basic_ostream<char16_t>& a_out);
template JSONPACKER_EXPORT bool Pack<char32_t,char32_t> (::std::basic_istream<char32_t>& a_inp, ::std::basic_ostream<char32_t>& a_out);
//
template JSONPACKER_EXPORT bool Pack<char>(::std::basic_istream<char>& a_inp, const char* a_outFileName);
template JSONPACKER_EXPORT bool Pack<wchar_t>(::std::basic_istream<wchar_t>& a_inp, const char* a_outFileName);
template JSONPACKER_EXPORT bool Pack<char16_t>(::std::basic_istream<char16_t>& a_inp, const char* a_outFileName);
template JSONPACKER_EXPORT bool Pack<char32_t>(::std::basic_istream<char32_t>& a_inp, const char* a_outFileName);
//
template JSONPACKER_EXPORT bool Pack<char>(const char* a_inpFileName, ::std::basic_ostream<char>& a_out);
template JSONPACKER_EXPORT bool Pack<wchar_t>(const char* a_inpFileName, ::std::basic_ostream<wchar_t>& a_out);
template JSONPACKER_EXPORT bool Pack<char16_t>(const char* a_inpFileName, ::std::basic_ostream<char16_t>& a_out);
template JSONPACKER_EXPORT bool Pack<char32_t>(const char* a_inpFileName, ::std::basic_ostream<char32_t>& a_out);


/*/////////////////////////////////////////////////////////////////////////////*/


template <typename CharOut>
static bool SingleDataToFile(const ::std::string& a_inpStr, ::std::basic_ostream<CharOut>& a_out, MapType* a_pMap)
{
    //const QByteArray jsonByteArray (a_inpStr.c_str(),int(a_inpStr.size()));
    const QByteArray jsonByteArray (a_inpStr.c_str());
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonByteArray);
    if(!jsonDoc.isObject()){return false;}
    int nNewKeyIntValue = int(a_pMap->size()), nKeyIntValue;
    tlv::TlvBox aTlvBox;
    const QJsonObject jsonObject = jsonDoc.object();
    QJsonObject::const_iterator iter = jsonObject.begin();
    const QJsonObject::const_iterator iterEnd = jsonObject.end();
    for(;iter!=iterEnd;++iter){
        const QJsonValue jsonValue = iter.value();
        const QJsonValue::Type jsonValueType = jsonValue.type();
        const QString keyStr = iter.key();
        const MapType::const_iterator iterDict = a_pMap->find(keyStr);
        if(iterDict==a_pMap->end()){
            nKeyIntValue = ++nNewKeyIntValue;
            a_pMap->insert(::std::pair<QString,TypeDictSecond>(keyStr,TypeDictSecond(nKeyIntValue,jsonValueType)));
        }
        else{
            nKeyIntValue = iterDict->second.first;
            //if(iterDict->second.second!=jsonValueType){return false;} // or?
        }

        // we will make following for the key:
        // key is int => 32 bit, we will split it to 2 16 bit parts,
        // one part will show data type (Null,Bool,Double,String)
        // second part will be responsible for key name (equal to nKeyIntValue)
        const uint16_t typePart = uint16_t(jsonValueType);
        const uint16_t keyNamePart = uint16_t(nKeyIntValue);
        const int cnCombinedKey((int(typePart)<<16)|int(keyNamePart));

        switch(jsonValueType){
        case QJsonValue::Null:
            aTlvBox.PutNoValue(cnCombinedKey);
            break;
        case QJsonValue::Bool:
            aTlvBox.PutBoolValue(cnCombinedKey,jsonValue.toBool());
            break;
        case QJsonValue::Double:
            aTlvBox.PutDoubleValue(cnCombinedKey,jsonValue.toDouble());
            break;
        case QJsonValue::String:
            aTlvBox.PutStringValue(cnCombinedKey,jsonValue.toString().toStdString());
            break;
        default:
            return false;
        } // switch(jsonValueType){

    }  // for(;iter!=iterEnd;++iter){

    if (!aTlvBox.Serialize()) {
        return false;
    }

    const size_t outBufferSize (size_t(aTlvBox.GetSerializedBytes()));
    if(outBufferSize<1){return false;}

    const char* pcOutput = reinterpret_cast<char*>(aTlvBox.GetSerializedBuffer());
    if(!pcOutput){
        return false;
    }
    ::std::basic_string<CharOut> outStr = StringToUString<std::basic_string<CharOut> >(pcOutput,outBufferSize);
    a_out.write(outStr.c_str(),outStr.size());

    return a_out.fail()?false:true;
}


/*/////////////////////////////////////////////////////////////////////////////*/


template <>
JSONPACKER_DLL_PRIVATE std::string StringToUString<std::string>(const char* a_strIn, size_t a_strLen)
{
	return ::std::string(a_strIn,a_strLen);
}


template <>
JSONPACKER_DLL_PRIVATE std::wstring StringToUString<std::wstring>(const char* a_strIn, size_t a_strLen)
{
    const size_t cunStrLen = a_strLen+1;
	size_t unRetSize;
	std::wstring retStr;
	wchar_t* pcBuf;

	retStr.resize(cunStrLen);
	pcBuf = const_cast<wchar_t*>(retStr.data());
	unRetSize=std::mbstowcs(pcBuf,a_strIn,cunStrLen);
	if(unRetSize!=static_cast<size_t> (-1)){
		retStr.resize(unRetSize);
	}

	return retStr;
}


template <>
JSONPACKER_DLL_PRIVATE std::u16string StringToUString<std::u16string>(const char* a_strIn, size_t a_strLen)
{
    const ::std::wstring strOut( StringToUString< std::wstring>(a_strIn,a_strLen));
    return ::std::u16string(strOut.begin(),strOut.end());
}


template <>
JSONPACKER_DLL_PRIVATE std::u32string StringToUString<std::u32string>(const char* a_strIn, size_t a_strLen)
{
    const ::std::wstring strOut( StringToUString< std::wstring>(a_strIn,a_strLen));
    return ::std::u32string(strOut.begin(),strOut.end());
}

static ::std::string UStringToString(const char* a_strIn, size_t a_count)
{
    return ::std::string(a_strIn,a_count);
}

static ::std::string UStringToString(const wchar_t* a_strIn, size_t a_count)
{
    const size_t cunMaxBufSize = MB_CUR_MAX*a_count+1;
	size_t unRetSize;
    ::std::string retStr;
	char* bufForData;

	retStr.resize(cunMaxBufSize);
	bufForData = const_cast<char*>(retStr.data());
	unRetSize=std::wcstombs(bufForData, a_strIn, cunMaxBufSize);
	if(unRetSize!=static_cast<size_t> (-1)){
		retStr.resize(unRetSize);
	}

	return retStr;
}


template <typename CharT>
static inline std::wstring toStdWStringT(const CharT* a_str, size_t a_count)
{
	std::wstring retStr(a_count,0);
	for(size_t i(0);i<a_count;++i){
		retStr.push_back(static_cast<wchar_t>(a_str[i]));
	}
	return retStr;
}



static ::std::string UStringToString(const char16_t* a_str, size_t a_count)
{
    const ::std::wstring strIn( toStdWStringT(a_str,a_count) );
    return UStringToString(strIn.c_str(),a_count);
}


static ::std::string UStringToString(const char32_t* a_str, size_t a_count)
{
    const ::std::wstring strIn( toStdWStringT(a_str,a_count) );
    return UStringToString(strIn.c_str(),a_count);
}




}}  // namespace jsonpacker{ namespace packer{
