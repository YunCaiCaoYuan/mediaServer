/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RTPSource.h
 * Author: Sergio
 *
 * Created on 13 de julio de 2018, 11:49
 */

#ifndef RTPSOURCE_H
#define RTPSOURCE_H

#include "config.h"
#include "acumulator.h"
#include "use.h"
#include "rtp/RTCPSenderReport.h"
#include "rtp/RTCPCompoundPacket.h"

struct LayerSource : LayerInfo
{
	DWORD		numPackets = 0;
	DWORD		totalBytes = 0;
	DWORD		bitrate;
	Acumulator	acumulator;
	
	LayerSource() : acumulator(1000)
	{
		
	}
	
	LayerSource(const LayerInfo& layerInfo) : acumulator(1000)
	{
		spatialLayerId  = layerInfo.spatialLayerId;
		temporalLayerId = layerInfo.temporalLayerId; 
	}
	
	void Update(QWORD now, DWORD size) 
	{
		//Increase stats
		numPackets++;
		totalBytes += size;
		
		//Update bitrate acumulator
		acumulator.Update(now,size);
		
		//Update bitrate in bps
		bitrate = acumulator.GetInstant()*8;
	}
};

struct RTPSource : public Mutex
{
	DWORD	ssrc;
	DWORD   extSeqNum;
	DWORD	cycles;
	DWORD	jitter;
	DWORD	numPackets;
	DWORD	numRTCPPackets;
	DWORD	totalBytes;
	DWORD	totalRTCPBytes;
	DWORD   bitrate;
	Acumulator acumulator;
	
	RTPSource();
	virtual ~RTPSource() = default;
	
	WORD SetSeqNum(WORD seqNum);
	void SetExtSeqNum(DWORD extSeqNum );
	virtual void Update(QWORD now, DWORD seqNum,DWORD size) ;
	virtual void Reset();
	
	
	/*
	 * Get seq num cycles from a past sequence numer
	 */
	WORD RecoverSeqNum(WORD osn);
	
};


#endif /* RTPSOURCE_H */

