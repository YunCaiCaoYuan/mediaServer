#ifndef RTPPAYLOAD_H
#define RTPPAYLOAD_H
#include "config.h"
#include <memory>
#include <array>

class RTPPayload
{
public:
	using shared = std::shared_ptr<RTPPayload>;
public:
	RTPPayload();
	
	RTPPayload::shared Clone();
	bool SetPayload(const BYTE *data,DWORD size);
	bool SkipPayload(DWORD skip);
	bool PrefixPayload(BYTE *data,DWORD size);
	
	BYTE* GetMediaData()			{ return payload;		}
	const BYTE* GetMediaData()	const	{ return payload;		}
	DWORD GetMediaLength()		const	{ return payloadLen;		}
	DWORD GetMaxMediaLength()	const	{ return SIZE;			}
	
	BYTE* GetPayloadData()			{ return payload;		}
	void SetMediaLength(DWORD len)		{ this->payloadLen = len;	}
private:
	static const DWORD SIZE = 1700;
	static const DWORD PREFIX = 200;	
private:
	std::array<BYTE,SIZE+PREFIX> buffer;
	BYTE*   payload;
	DWORD	payloadLen;

};

#endif /* RTPPAYLOAD_H */

