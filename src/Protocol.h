#pragma once
#include "types.h"
#include <vector>

// forward declaration
class D2Matrix8U;
#define PROTOCOL_MSG_SIZE 128
#pragma pack(push,1)
enum EProtocolIdentity : uint16_t
{
	EPI_NONE = 0,
	EPI_STREAM_CLIENT = 1,
	EPI_STREAM_SERVER = 2,
	EPI_BROADCAST_CHNNNEL	 = 3,//servers brodcast channel
	EPI_LOCAL_MASTER = 4,
	EPI_BROADCAST_STREAM_CLIENT = 5, //clients brodcast channel
	EPI_LAST = EPI_BROADCAST_STREAM_CLIENT
};
static const char* EPIStrings[EPI_LAST + 1] = { "NONE", "STRMCLNT", "STRMSRVR", "BROADCAST", "LOCALHOST", "BROADCAST_STREAM_CLIENT" };




enum  EProtocolCommandClass
{
	EPCC_CONTROL = 1,
	EPCC_CYCLE = 2,
	EPCC_LIVE = 3
};

enum  EProtocolCommand : uint16_t
{
	EPC_NONE = 0,
	EPC_GRAB_POST_TRIGGER=1 ,
	EPC_GRAB_PRE_TRIGGER=2,
	EPC_GRAB_SECS_TRIGGER=3,
	EPC_GRAB_START_CLOCK=4 ,
	EPC_GRAB_STOP_CLOCK=5 ,
	EPC_GRAB_START_CYCLE=6 ,
	EPC_GRAB_END_CYCLE=7 ,
	EPC_GRAB_CYCLE_FRAME=8 ,
	EPC_GRAB_CYCLE_FRAME_LOST=9 ,
	EPC_GRAB_LIVE_FRAME=10 ,
	EPC_GRAB_LIVE_FRAME_LOST=11 ,
	EPC_GRAB_CANCEL=12,
	EPC_GRAB_STOP =13,
	EPC_GRAB_FETCH =14,
	EPC_REGISTER=15,
	EPC_UNREGISTER=16,
	EPC_ACKNOLEDGE=17,
	EPC_SYNC_LIVE_FRAME = 18,
	EPC_LAST = EPC_SYNC_LIVE_FRAME
};
inline EProtocolCommandClass ProtocolCommandClass(EProtocolCommand cmd)
{
	if(cmd==EPC_GRAB_LIVE_FRAME || cmd==EPC_GRAB_LIVE_FRAME_LOST)	return EPCC_LIVE;
	else if(cmd==EPC_GRAB_CYCLE_FRAME || cmd==EPC_GRAB_CYCLE_FRAME_LOST)	return EPCC_CYCLE;
	else return EPCC_CONTROL;
};



// message data size
#define PROTOCOL_DATA_SIZE PROTOCOL_MSG_SIZE - sizeof(MessageHeader) // for allignment
// message header
static const char* EPCStrings[EPC_LAST+1] = {"NONE","GRAB_POST_TRIGGER","GRAB_PRE_TRIGGER","GRAB_SECS_TRIGGER","GRAB_START_CLOCK","GRAB_STOP_CLOCK","GRAB_START_CYCLE",
											"GRAB_END_CYCLE","GRAB_CYCLE_FRAME","GRAB_CYCLE_FRAME_LOST","GRAB_LIVE_FRAME","GRAB_LIVE_FRAME_LOST",
											"GRAB_CANCEL","GRAB_STOP","GRAB_FETCH","REGISTER","UNREGISTER","ACKNOLEDGE","SYNC_LIVE_FRAME"};
struct MessageHeader
{
	union
	{
		uint16_t m_dstFGCID;	// unique destination FGC identifier
		EProtocolIdentity m_target;	// destination Type
	};
	uint16_t m_sequence;  // sequence for control protocol
	uint16_t m_cmdFlag;   // commande hunique identifier
	uint16_t m_dataSize;  // data size folowing the header
	const char* GetCommandStr()const { return  EPCStrings[m_cmdFlag>EPC_LAST ? EPC_LAST : m_cmdFlag]; };
	EProtocolCommand GetCommand()const {return  (EProtocolCommand)m_cmdFlag;};
	const char* GetTargetStr()const { return  EPIStrings[m_target>EPI_LAST ? EPI_LAST : m_target]; };
	EProtocolIdentity GetTarget()const { return  m_target; };
};

struct ProtocolEvent
{
	ProtocolEvent(){ memset(&m_header, 0, sizeof(m_header)); memset(m_data, 0, sizeof(m_data)); };
	ProtocolEvent(const ProtocolEvent& other){ *this = other; };
	ProtocolEvent& operator=(const ProtocolEvent& other){ m_header = other.m_header; memcpy(m_data, other.m_data, sizeof(m_data)); return *this; };
	MessageHeader	m_header;
	char			m_data[PROTOCOL_DATA_SIZE];
	EProtocolCommand GetCommand() const{ return  m_header.GetCommand(); };
	const char* GetCommandStr() const { return  m_header.GetCommandStr(); };
	const char* GetTargetStr()const { return  m_header.GetTargetStr(); };
	EProtocolIdentity GetTarget()const { return  m_header.GetTarget(); };
};


struct BufferHeader0
{
	uint64_t		m_bufferMicroStamp;
	uint32_t		m_sequenceIDX;
	uint16_t		m_cycleIDX;
	uint16_t		m_width;
	uint16_t		m_height;
	uint16_t		m_stepline;
	EGenericType	m_egt;
	EMemoryHeap		m_emh;
};
enum BufferFlags : uint16_t
{
	EBHF_NONE = 0x000,
	EBHF_INVALID = 0x001
};

struct BufferHeader
{
	uint64_t		m_bufferMicroStamp;
	uint32_t		m_sequenceIDX;
	uint16_t		m_cycleIDX;
	uint16_t		m_width;
	uint16_t		m_height;
	uint16_t		m_stepline;
	EGenericType	m_egt;
	EMemoryHeap		m_emh;
	uint32_t		m_SDITime;
	uint16_t		m_bufferFlags; 
	uint32_t		m_reserved[9];
};// 64 byte aligned

struct ClientHeader
{
	char m_listenAddress[16];
	char m_clientModule[16]; 
	uint32_t m_listenPort; 
	uint64_t m_proccesID;
};

struct TriggerHeader
{
	uint16_t m_caps; // 
	uint16_t m_bursts;
	uint16_t m_interleave;
	uint16_t m_wait;
	inline uint16_t TotalGrab()
	{return m_bursts*(m_caps*(m_interleave+1) +m_wait);};
	inline uint16_t TotalOutput()
	{return m_bursts*m_caps;};
	char m_dir[PROTOCOL_DATA_SIZE-8];
};

struct CycleHeader
{
	char m_dir[80];
	uint16_t		m_caps; // 
	uint32_t		m_sequenceIDX;
	uint16_t		m_cycleIDX;
	uint64_t		m_bufferMicroStamp;
	uint32_t		m_sdiTime; // adi 03012016
};

struct ClockHeader
{
	uint8_t  m_systemSynch; // boolean
	uint32_t m_systemFrameIndex; // next frame,not last frame,global index
	uint64_t m_systemMicroTime; // next frame,not last frame,system global MicroSeconds (From System UP) 
	uint32_t m_externalOutputs;
	uint32_t m_CCSource;
	float	FPS;
};

struct SeekHeader
{
	uint64_t m_requestedFrameIndex; 
    uint64_t m_firstFrameLost;
    uint64_t m_lastFrameLost;
	bool m_lastFrame;
	bool m_seekSuccess;
	uint64_t m_state;
	uint64_t reserved;
};

#define  RUN_MSG (0xF0F0F0F0)
#define  STRT_MSG (0xF1F2F3F4)

// basic generic event

typedef ProtocolEvent MessageAck;
typedef ProtocolEvent MessageFrameEvent;
typedef ProtocolEvent MessageTrigger;
/*struct MessageFrameEvent
{
	inline operator ProtocolEvent*(){return (ProtocolEvent*)this;};
	inline operator ProtocolEvent&(){return *(ProtocolEvent*)this;};
	ProtocolEvent m_event;// 
	inline BufferHeader*  Data() {return  (BufferHeader*)m_event.Data();};
	inline const BufferHeader*  Data() const {return  (BufferHeader*)m_event.Data();};
};
struct MessageTrigger
{
	inline operator ProtocolEvent*(){return (ProtocolEvent*)this;};
	inline operator ProtocolEvent&(){return *(ProtocolEvent*)this;};
	ProtocolEvent m_event;// 
	inline TriggerHeader*  Data() {return  (TriggerHeader*)m_event.Data();};
	inline const TriggerHeader*  Data() const {return  (TriggerHeader*)m_event.Data();};
};
*/
/*
struct MessageClientHeader
{
	inline operator ProtocolEvent*(){return (ProtocolEvent*)this;};
	inline operator ProtocolEvent&(){return *(ProtocolEvent*)this;};
	ProtocolEvent m_event;// 
	inline ClientHeader*  Data() {return  (ClientHeader*)m_event.m_data;};
	inline const ClientHeader* Data() const {return  (ClientHeader*)m_event.m_data;};
};
*/
//typedef MessageClientHeader MessageRegister;
//typedef MessageClientHeader MessageUnRegister;
typedef MessageTrigger MessageGrabPreTrigger;
typedef MessageTrigger MessageGrabPostTrigger;
typedef MessageTrigger MessageGrabClockStart;
typedef MessageTrigger MessageGrabClockStop;
typedef MessageTrigger MessageGrabCycleStart;
typedef MessageTrigger MessageGrabCycleEnd;
typedef MessageTrigger MessageGrabTriggerBySeconds;
typedef MessageTrigger MessageGrabCancel;
typedef MessageTrigger MessageGrabStop;

struct MessageGrabFetch
{
	inline operator ProtocolEvent*(){return (ProtocolEvent*)this;};
	inline operator ProtocolEvent&(){return *(ProtocolEvent*)this;};
	MessageHeader m_header;// 
	uint32_t m_startAt; // fetch frames
	uint32_t m_endAt; // fetch frames
	uint32_t m_startPreVidAt; // video frames - medium resolution JPG images, storage is reversed! starts at end frame and stops at start frame
	uint32_t m_endPreVidAt; // video frames - medium resolution JPG images, storage is reversed! starts at end frame and stops at start frame
	uint32_t m_startPostVidAt; // video frames - medium resolution JPG images
	uint32_t m_endPostVidAt; // video frames - medium resolution JPG images
	char m_reserved[PROTOCOL_DATA_SIZE-24];
};








enum EncoderStreamType{ ESTFull = 0, ESTHd = 1, ESTMatting1 = 2, ESTMatting2 = 3, ESTSpare = 4, ESTUnknown = 5 };
static const char* EncoderStreamSuffix[ESTSpare + 2] = { "Full.h264", "Hd.h264", "Bin.rle", "Mog.rle", "Spare", "Unknown" };
#define  GET_LAST_FRAME (0xFFFFF3F4F5F6F7F8)
/*
struct VeryOldEncodeSequence
{
	uint8_t					encoderStreamType;
	uint8_t		            version;// version compatibility 
	uint32_t                systemFrameIdx;// global Frame no.
	uint32_t                streamFrameIdx;//CurrPicIdx      Frame no. for which the bitstream is being retrieved. 
	uint32_t                bitstreamSizeInBytes;//nBitstreamDataLen - Actual number of bytes generated and copied to the memory pointed by bitstreamBufferPtr. 
	uint64_t				bitstreamOffsetInBytes;
	uint8_t	                numSlices;// Number of slices in the encoded picture. Will be reported only if NV_ENC_INITIALIZE_PARAMS::reportSliceOffsets set to 1. 
	uint32_t                sliceOffsets[16]; // pSliceDataOffsets - Array which receives the slice offsets. This is not supported if NV_ENC_CONFIG_H264::sliceMode is 1 on Kepler GPUs. Array size must be equal to size of frame in MBs. 
	uint64_t                streamOutputTimeStamp;//Presentation timestamp associated with the encoded output. 
	uint64_t                systemOutputTimeStamp;//Presentation timestamp associated with the encoded output. 
	uint64_t                outputDuration;       //Presentation duration associates with the encoded output. 
	uint8_t					pictureType;//NV_ENC_PIC_TYPE\field_pic_flag - Picture type of the encoded picture. 
	uint8_t			        pictureStruct;//NV_ENC_PIC_STRUCT\bottom_field_flag /*Structure of the generated output picture. 
	uint32_t                frameAvgQP;//Average QP of the frame. 
	uint32_t                frameSatd; //Total SATD cost for whole frame. 
	bool					ltrFrame;//intra_pic_flag - This picture is entirely intra coded Flag indicating this frame is marked as LTR frame- This picture is a reference picture
	uint32_t                ltrFrameIdx;//ref_pic_flag - /*Frame index associated with this LTR frame. 
};

struct VeryOldEncoderStreamConfiguration
{
	uint8_t						encoderStreamType;
	uint8_t						version;                                    // struct version
	uint8_t						codec;                                    // 0=H264 , 1 = HEVC
	uint8_t						profile;                                    // 0=auto(=lowest possible profile), 66=Baseline, 77=Main, 100=High, 128 = (High Profile Stereo), 244 = (High Profile 444), 257 = (Constrained High Profile)
	uint8_t						preset;										// 0=NV_ENC_PRESET_DEFAULT, 1=NV_ENC_PRESET_LOW_LATENCY_DEFAULT, 2=NV_ENC_PRESET_HP, 3=NV_ENC_PRESET_HQ, 4=NV_ENC_PRESET_BD, 5=NV_ENC_PRESET_LOW_LATENCY_HQ, 6=NV_ENC_PRESET_LOW_LATENCY_HP,8=NV_ENC_PRESET_LOSSLESS_DEFAULT ,8=NV_ENC_PRESET_LOSSLESS_HP
	uint8_t						level;										// 10,11,12,13,20,21,22,30,31,32,40,41,42,50,51=level_idc, 0=auto(=lowest possible level)
	uint8_t						gopLength;									// [in]: Specifies the number of pictures in one GOP. Low latency application client can set goplength to NVENC_INFINITE_GOPLENGTH so that keyframes are not inserted automatically. 
	uint8_t						numBFrames;									 // number of B frames between I and P frames
	uint8_t						monoChromeEncoding;                          // [in]: Set this to 1 to enable monochrome encoding for this session. 
	uint8_t						frameFieldMode;                              // [in]: Specifies the frame/field mode. Check support for field encoding using ::NV_ENC_CAPS_SUPPORT_FIELD_ENCODING caps. 
	uint8_t                     mvPrecision;                                 // [in]: Specifies the desired motion vector prediction precision. 
	uint8_t						rateControlMode;                             // [in]: Specifies the rate control mode. Check support for various rate control modes using ::NV_ENC_CAPS_SUPPORTED_RATECONTROL_MODES caps. 
	uint8_t						constQP;                                     // [in]: Specifies the initial QP to be used for encoding, these values would be used for all frames if in Constant QP mode. 
	uint8_t                     enableMinQP;                                 // [in]: Set this to 1 if minimum QP used for rate control. 
	uint8_t						minQP;                                       // [in]: Specifies the minimum QP used for rate control. Client must set NV_ENC_CONFIG::enableMinQP to 1. 
	uint8_t                     enableMaxQP;                                 // [in]: Set this to 1 if maximum QP used for rate control. 
	uint8_t						maxQP;                                       // [in]: Specifies the maximum QP used for rate control. Client must set NV_ENC_CONFIG::enableMaxQP to 1. 
	uint8_t                     enableInitialRCQP;                           // [in]: Set this to 1 if user suppplied initial QP is used for rate control. 
	uint8_t                     enableAQ;                                    // [in]: Set this to 1 to enable adaptive quantization. 
	uint8_t						enableIntraRefresh;                          // [in]: Set to 1 to enable gradual decoder refresh or intra refresh. If the GOP structure uses B frames this will be ignored 
	uint8_t		                isYuv444;

	uint32_t					frameWidth;
	uint32_t					frameHeight;
	uint32_t					frameRateNumerator;
	uint32_t					frameRateDenominator;
	uint32_t                    averageBitRate;                              // [in]: Specifies the average bitrate(in bits/sec) used for encoding. 
	uint32_t                    maxBitRate;                                  // [in]: Specifies the maximum bitrate for the encoded output. This is used for VBR and ignored for CBR mode. 
	uint32_t                    vbvBufferSize;                               // [in]: Specifies the VBV(HRD) buffer size. in bits. Set 0 to use the default VBV  buffer size. 
	uint32_t                    vbvInitialDelay;                             // [in]: Specifies the VBV(HRD) initial delay in bits. Set 0 to use the default VBV  initial delay .
	uint32_t                    intraRefreshPeriod;							// [in]: Specifies the interval between successive intra refresh if enableIntrarefresh is set. Requires enableIntraRefresh to be set.
	uint32_t                    intraRefreshDuration;							// [in]: Specifies the length of intra refresh in number of frames for periodic intra refresh. This value should be smaller than intraRefreshPeriod 
};

struct VeryOldEncoderStream
{
	uint8_t		               streamCount;
	VeryOldEncoderStreamConfiguration streamsHeaders[4];
};
*/
#define EncoderProtocolVersion 2

struct MiniFrameHeader
{
	uint8_t					encoderStreamType;
	uint8_t		            version;// version compatibility 
	uint32_t                systemFrameIdx;// global Frame no.
	uint32_t                streamFrameIdx;//CurrPicIdx      Frame no. for which the bitstream is being retrieved. 
};

struct FrameHeader
{
	uint8_t		encoderStreamType;
	uint8_t		version;
	uint32_t    systemFrameIdx;// global Frame no.*/
	uint32_t    streamFrameIdx;// local Frame no.*/
	uint64_t    streamOutputTimeStamp;
	uint64_t    outputDuration;       //Presentation duration associates with the encoded output. 
	union
	{
		uint64_t    frameUtcTime; // systemOutputTimeStamp
		uint64_t    systemOutputTimeStamp;
	};


	union
	{
		uint32_t	frameSize;
		uint32_t	 bitstreamSizeInBytes;
	};
	union 
	{
		uint64_t    frameFileOffset;// bitstreamOffsetInBytes;
		uint64_t    bitstreamOffsetInBytes;
		uint8_t*	frameDataStart;
	};
	union
	{
		uint8_t		frameType;
		uint8_t		pictureType;
	};

	uint32_t    lastValidframe; // valid only if frameSkipped. holds the last valid existing frame.
	uint32_t    nextValidframe; // valid only if frameSkipped. holds the next valid existing frame.
	uint32_t    isLastFrame; // indicates that this is the last frame in the sequence.
    uint32_t    fwdStreamHeader; //indicates that the stream header exists right after the frameheader for this packet
	uint32_t    reserved[3];
};


struct StreamHeader
{
	uint8_t						encoderStreamType;
	uint8_t						version;      // struct version
	uint32_t					codec;      // cudaVideoCodec_enum  cudaVideoCodec_H264,
	uint8_t						gopLength;  // [in]: Specifies the number of pictures in one GOP. Low latency application client can set goplength to NVENC_INFINITE_GOPLENGTH so that keyframes are not inserted automatically. */
	uint32_t					frameWidth;
	uint32_t					frameHeight;
	uint32_t					frameRateNumerator;
	uint32_t					frameRateDenominator;
	uint32_t					chromaFormat; //cudaVideoChromaFormat_420;cudaVideoChromaFormat_444;//
	uint32_t					averageBitRate;   // video bitrate (bps, 0=unknown)
	uint32_t					firstValidFrameIndex;
	uint32_t					lastValidFrameIndex;
	uint32_t					reserved[3];
};

struct SystemStreamsHeader
{
	uint8_t		              streamCount;
	StreamHeader              streamsHeaders[ESTSpare];
};

/*
#ifndef EncoderProtocolVersion 
#define EncoderProtocolVersion 0x00
#endif
#if EncoderProtocolVersion == 0 
typedef VeryOldEncoderStreamConfiguration EncoderStreamConfiguration;
typedef VeryOldEncoderStream EncoderStream;
typedef VeryOldEncodeSequence EncodeSequence;
#endif //EncoderProtocolVersion == 0 
#if EncoderProtocolVersion == 1 
typedef StreamHeader EncoderStreamConfiguration;
typedef SystemStreamsHeader EncoderStream;
typedef FrameHeader EncodeSequence;
#endif //EncoderProtocolVersion == 1 
//typedef std::vector<EncodeSequence> SequenceList;
*/
enum ETaskType : uint8_t
{
	ESTT_NONE =0,
	ESTT_CONTROL = ESTT_NONE,
	ESTT_LORES,
	ESTT_HIRES,
	ESTT_FETCH,
	ESTT_DISPLAY,
	ESTT_STORAGE,
	ESTT_PRE_VIDEO,
	ESTT_POST_VIDEO,
	ESTT_CONVERT,
	ESTT_BACKGROUND,
	ESTT_LAST
};
static const char* ESTTStrings[ESTT_LAST] = {"NONE","LoRes","HiRes","Fetch","Display","Storage","PreVideo","PostVideo","Convert","Background"};

struct TaskHeader
{
	uint32_t		m_channelIDX;
	void*			m_data;
	uint8_t			m_status;
	bool			m_pushFront;
	ETaskType		m_taskType;
	uint8_t			m_reserved;
};

struct FrameEvent
{
	FrameEvent()
	{
		memset(&m_bufferHeader, 0, sizeof(m_bufferHeader));
		memset(&m_task,0,sizeof(m_task));
	}
	FrameEvent(const BufferHeader* bufferHeader)
	{
		memcpy(&m_bufferHeader, bufferHeader, sizeof(m_bufferHeader));
		memset(&m_task,0,sizeof(m_task));
	};
	bool LoadMMF(bool copy = true);
	void* BufferFromMMF();
	void* MapFromMMF();

	BufferHeader	m_bufferHeader;
	TaskHeader		m_task;
	void*			m_handle;
};

class IMProtocolDispatcher
{
public:
	IMProtocolDispatcher(){};
	virtual ~IMProtocolDispatcher(){};
	virtual bool HandleProtocolEvent(const ProtocolEvent* mcevent, int length,const char* fromIP, int fromPort) = 0;
};

#pragma pack(pop)   