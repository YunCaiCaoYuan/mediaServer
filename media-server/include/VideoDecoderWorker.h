#ifndef VIDEODECODERWORKER_H
#define VIDEODECODERWORKER_H

#include "codecs.h"
#include "video.h"
#include "waitqueue.h"
#include "rtp.h"

class VideoDecoderWorker 
	: public RTPIncomingMediaStream::Listener
{
public:
	VideoDecoderWorker() = default;
	virtual ~VideoDecoderWorker();

	int Start();
	virtual void onRTP(RTPIncomingMediaStream* stream,const RTPPacket::shared& packet);
	virtual void onEnded(RTPIncomingMediaStream* stream);
	virtual void onBye(RTPIncomingMediaStream* stream);
	int Stop();
	
	void AddVideoOutput(VideoOutput* ouput);
	void RemoveVideoOutput(VideoOutput* ouput);

protected:
	int Decode();

private:
	static void *startDecoding(void *par);

private:
	std::set<VideoOutput*> outputs;
	WaitQueue<RTPPacket::shared> packets;
	pthread_t thread;
	Mutex mutex;
	bool decoding	= false;
	bool muted	= false;
	std::unique_ptr<VideoDecoder>	videoDecoder;
};

#endif /* VIDEODECODERWORKER_H */

