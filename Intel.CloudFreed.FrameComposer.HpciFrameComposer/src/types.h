#pragma once
#include <cstdint>
#include <cstring>

//#ifndef _MTYPES_H_
//#define _MTYPES_H_
/*
#ifdef CONSOLE_EXPORTS
#define CONSOLE_API  __declspec(dllexport)
#else 
#define CONSOLE_API  __declspec(dllimport)
#endif
#ifdef CONSOLE_STATIC
#undef CONSOLE_API  
#define CONSOLE_API  
#endif
*/
//#define CONSOLE_API  __cdecl


#ifdef _WIN32
#include <windows.h>
#pragma warning(disable:4786)
#pragma warning(disable:4996)
typedef void* OSSHandle;
#endif// _WIN32
#define STRING_LENGTH 2048
#ifndef TRUE
#define TRUE 1
#endif //TRUE
#ifndef FALSE
#define FALSE 0
#endif // FALSE

#define GlobalFrameName "ReplayGrabberGlobalFrame"
#define GlobalFrameNameH264 "ReplayGrabberGlobalFrameH264"



typedef unsigned char byte_t;
typedef byte_t bool_t;
//typedef signed __int8           int8_t;
//typedef signed __int16          int16_t;
//typedef signed __int32          int32_t;
//typedef signed __int64          int64_t;
//typedef unsigned __int8         uint8_t;
//typedef unsigned __int16        uint16_t;
//typedef unsigned __int32        uint32_t;
//typedef unsigned __int64        uint64_t;




struct uint128_t
{
	uint128_t& operator=(const uint128_t& other)
	{
		lsb=other.lsb;msb=other.msb;return *this;
	};
	bool operator>=(const uint128_t& other) const
	{
		if(msb>other.msb) return true;
		else if (msb<other.msb) return false;
		else if(lsb>other.lsb) return true;
		else if(lsb<other.lsb) return false;
		else return true;
	};
	bool operator<(const uint128_t& other) const
	{
		if(msb<other.msb) return true;
		else if (msb>other.msb) return false;
		else if(lsb<other.lsb) return true;
		else return false;
	};
	uint64_t lsb;
	uint64_t msb;
};

typedef float					float32_t;
typedef double					float64_t;
typedef float F32C3[3]; 
typedef unsigned char U8C3[3]; 
typedef unsigned char U8C4[4]; 


typedef enum EGenericType : unsigned char
{
	EGT_NONE ,// unsigned int 8 bit
	EGT_8U ,// unsigned int 8 bit
	EGT_8S ,// signed int 8 bit
	EGT_16U ,// unsigned int 16 bit
	EGT_16UC3 ,// unsigned int 16 bit[3]
	EGT_16S ,// signed int 16 bit
	EGT_16SC3 ,// signed int 16 bit 16[3] 
	EGT_8UC3 ,// unsigned int 24 bit 8[3] 
	EGT_8UC4 ,// unsigned int 32 bit 8[4] 
	EGT_32U ,// unsigned int 32 bit
	EGT_32S ,// signed int 32 bit
	EGT_32SC3 ,// unsigned int 32 bit with 3 channels uint[3]
	EGT_64U ,// unsigned int 64 bit
	EGT_64S ,// signed int 64 bit
	EGT_32F ,// float 32 bit
	EGT_32FC2,// float 32 bit with 2 channels float[2]
	EGT_32FC3,// float 32 bit with 3 channels float[3]
	EGT_32FC4 ,// float 32 bit with 4 channels float[4]
	EGT_64F ,// float 64 bit
	EGT_64FC3 ,// float 64 bit with 3 channels double[3]
	EGT_TemplateType,
	EGT_FIRST = EGT_8U,// unsigned int 8 bit
	EGT_LAST
}EGenericType;

enum EMemoryHeap : unsigned char // lsb 4 bits
{
	EMH_HEAP	= 0,// normal virtual memory
	EMH_IPP     , // normal virtual memory over ipp - alligned
	EMH_EXTERNAL, // external heap buffer virtual memory
	EMH_MMFCLNT,//	globaly shared memory
	EMH_MMFCLNT_H264,//	globaly shared memory specific to H264
	EMH_MMFSRVR,//	globaly shared memory
	EMH_MMFSRVR_H264,//	globaly shared memory specific to H264
	EMH_LAST_VIRT = EMH_MMFSRVR_H264,// last normal virtual memory
	EMH_CUDA_HOST,// Cuda () GPU driver shared phisical host memory 
	EMH_OGL_HOST, // OpenGL() GPU driver shared phisical host memory 
	EMH_LAST_HOST = EMH_OGL_HOST,// last host memory
	EMH_CUDA_ARR,// Cuda () GPU array 
	EMH_CUDA_BUF,// Cuda(GPU) buffer  
	EMH_OGL_TEX,//  OpenGL(GPU) texture object
	EMH_OGL_BUF,//  OpenGL(GPU) buffer object
	EMH_LAST = EMH_OGL_BUF,
	EMH_ALL		 = 0XFF
};

enum EFrameCompressionType
{
	RAW_COMPRESSION_TYPE = 0,
	H264_COMPRESSION_TYPE = 1,
	LOSSLESS_COMPRESSION_TYPE = 2,
	LAST_COMPRESSION_TYPE
};


inline bool IsHostMemory(EMemoryHeap emh){ return (emh <= EMH_LAST_HOST); };
inline bool IsDeviceMemory(EMemoryHeap emh){ return (emh > EMH_LAST_HOST); };
inline bool IsVirtHostMemory(EMemoryHeap emh) { return (emh <= EMH_LAST_VIRT); };
inline int SizeOfGenericType(EGenericType egt)
{
	switch (egt)
	{
	case EGT_8U    : return sizeof(uint8_t);// unsigned int 8 bit
	case EGT_8UC3   : return 3 * sizeof(uint8_t);// unsigned int 8 bit
	case EGT_8UC4  : return 4 * sizeof(uint8_t);
	case EGT_8S    : return sizeof(int8_t);// signed int 8 bit
	case EGT_16U   : return sizeof(uint16_t);// unsigned int 16 bit
	case EGT_16UC3 : return 3 * sizeof(uint16_t);// unsigned int 16 bit
	case EGT_16S   : return sizeof(int16_t);// signed int 16 bit
	case EGT_16SC3 : return 3 *sizeof(int16_t);// signed int 16 bit
	case EGT_32U   : return sizeof(uint32_t);// unsigned int 32 bit
	case EGT_32S   : return sizeof(int32_t);// signed int 32 bit
	case EGT_32SC3 : return 3* sizeof(uint32_t);// unsigned int 32 bit
	case EGT_64U   : return sizeof(uint64_t);// unsigned int 64 bit
	case EGT_64S   : return sizeof(int64_t);// signed int 64 bit
	case EGT_32F   : return sizeof(float32_t);// float 32 bit
	case EGT_32FC3 : return 3* sizeof(float32_t);// float 32 bit
	case EGT_32FC4 : return 4* sizeof(float32_t);// float 32 bit
	case EGT_64F   : return sizeof(float64_t);// float 64 bit
	case EGT_64FC3 : return 3* sizeof(float64_t);// float 64 bit
	default		 :return -1;
	}
};

struct  uint24_t
{
public:
	inline uint24_t()
	{memset(m_rgb,0,3);};
	uint24_t(const uint24_t& other)
	{*this = other;};
	uint8_t& operator[](const int index)
	{return m_rgb[index];};
	uint24_t& operator=(const uint24_t& other)
	{memcpy(m_rgb,other.m_rgb,3);
	return *this;};
	operator uint32_t()
	{uint32_t res(0);
	memcpy(((uint8_t*)(&res))+1,m_rgb,3);
	return res;	};
	bool operator>(const uint24_t& other)
	{memcpy(m_rgb,other.m_rgb,3);
	return (m_rgb[0] ==other.m_rgb[0] &&m_rgb[1] ==other.m_rgb[1] &&m_rgb[2] == other.m_rgb[2]);};
	bool operator==(const uint24_t& other)
	{return (0 == memcmp(m_rgb,other.m_rgb,3));};
	uint24_t& operator+=(const uint24_t& other)
	{	m_rgb[0] += other.m_rgb[0];
	m_rgb[1] += other.m_rgb[1];
	m_rgb[2] += other.m_rgb[2];
	return *this;};
	uint24_t operator+(const uint24_t& other)
	{	uint24_t* res = new uint24_t(*this); 
	return ((*res) += other);};
	uint24_t& operator-=(const uint24_t& other)
	{	m_rgb[0] -= other.m_rgb[0];
	m_rgb[1] -= other.m_rgb[1];
	m_rgb[2] -= other.m_rgb[2];
	return *this;};
	uint24_t operator-(const uint24_t& other)
	{uint24_t* res = new uint24_t(*this); 
	return ((*res) -= other);};
	uint8_t m_rgb[3];
};

typedef uint24_t RGBColor;



//#endif // _TYPES_H_
