#include "HpciFrameComposer.h"
#include "Protocol.h" // mpc project
#include "External.h"

#include <string.h>
#include <exception>

using namespace Intel::CloudFreed::FrameComposer::Interfaces;

namespace Intel {
	namespace CloudFreed {
		namespace FrameComposer {
			namespace HpciFrameComposer
			{		
				void HpciFrameComposer::GetMD(void* bufferIn, long &frameId, long &timeStamp, long &bodyOffset, long &bodySize)
				{
					
					FrameHeader *data = (FrameHeader*)bufferIn;
					frameId = data->systemFrameIdx;
					timeStamp = data->streamOutputTimeStamp;
					bodySize = data->bitstreamSizeInBytes;

					if (data->fwdStreamHeader)
						bodyOffset = sizeof(FrameHeader) + sizeof(StreamHeader);
					else
						bodyOffset = sizeof(FrameHeader);
					

				}

				void HpciFrameComposer::SetMD(void* bufferIn, size_t sizeIn, long frameId, long timeStamp, void* bufferOut, size_t &sizeOut)
				{
					int offset = 0;
					FrameHeader *data = (FrameHeader*)bufferOut;
					data->systemFrameIdx = frameId;
					data->streamOutputTimeStamp = timeStamp;
					data->bitstreamSizeInBytes = sizeIn;


					if (((uint8_t*)bufferIn)[4] == 0x67) // I frame
					{
						data->fwdStreamHeader = 1;
						data->frameType = NV_ENC_PIC_TYPE_IDR;
						sizeOut = sizeIn + sizeof(FrameHeader) + sizeof(StreamHeader);
						offset = sizeof(FrameHeader) + sizeof(StreamHeader);
					}
					else
					{
						data->fwdStreamHeader = 0;
						data->frameType = NV_ENC_PIC_TYPE_P;
						sizeOut = sizeIn + sizeof(FrameHeader);
						offset = sizeof(FrameHeader);
					}

					memcpy(bufferOut + offset, bufferIn, sizeOut);
				}
			}
		}
	}
}

