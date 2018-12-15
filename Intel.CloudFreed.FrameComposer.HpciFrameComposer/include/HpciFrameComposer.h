#pragma once
#include "IFrameComposer.h"

using namespace Intel::CloudFreed::FrameComposer::Interfaces;

namespace Intel {
	namespace CloudFreed {
		namespace FrameComposer {
			namespace HpciFrameComposer
			{
				class HpciFrameComposer : public IFrameComposer
				{
				public:				
					void GetMD(void* bufferIn, long &frameId, long &timeStamp, long &bodyOffset, long &bodySize);
					void SetMD(void* bufferIn, size_t sizeIn, long frameId, long timeStamp, void* bufferOut, size_t &sizeOut);
				};
			}
		}
	}
}


