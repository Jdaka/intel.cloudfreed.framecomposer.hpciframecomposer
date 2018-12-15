#include <iostream>
#include "IFrameComposer.h"
#include "HpciFrameComposer.h"

using namespace std;
using namespace Intel::CloudFreed::FrameComposer::Interfaces;
using namespace Intel::CloudFreed::FrameComposer::HpciFrameComposer;

int main()
{
	cout << "Example App" << std::endl;

	IFrameComposer* composer = new HpciFrameComposer();

	size_t rawFrameSize = 1024;
	void* rawFrame = operator new(rawFrameSize);

	size_t bufferSize = 2024;
	void* composedFrame = operator new(bufferSize);
	size_t composedFrameSize = 0;

	composer->SetMD(rawFrame, rawFrameSize, 111, 222, composedFrame, composedFrameSize);

	cout << "Composed frame size is:" << composedFrameSize << std::endl;

	long extractedFrameId = 0;
	long extractedTimeStamp = 0;
	long extractedBodyOffset = 0;
	long extractedBodySize = 0;
	

	composer->GetMD(composedFrame, extractedFrameId, extractedTimeStamp, extractedBodyOffset, extractedBodySize);

	cout << "Extracted data are:" << std::endl;
	cout << "FrameId: " << extractedFrameId << std::endl;
	cout << "TimeStamp: " << extractedTimeStamp << std::endl;
	cout << "Body Offset: " << extractedBodyOffset << std::endl;
	cout << "Body Size: " << extractedBodySize << std::endl;

    return 0;
}