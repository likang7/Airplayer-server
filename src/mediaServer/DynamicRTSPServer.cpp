/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**********/
// Copyright (c) 1996-2013, Live Networks, Inc.  All rights reserved
// A subclass of "RTSPServer" that creates "ServerMediaSession"s on demand,
// based on whether or not the specified stream name exists as a file
// Implementation

#include "DynamicRTSPServer.h"
#include <BasicUsageEnvironment.hh>
#include <liveMedia.hh>
#include <cstring>
#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <sys/stat.h>
using std::string;
using std::vector;
using std::cout;
using std::endl;
/*
**forward
*/
int readTrackinfo(const char *cmd, std::vector<MediaStream>& res); 
int convertMediaWithoutReEncoding(const std::string srcFile, const std::string destpath,
      const std::string destFormat, const std::vector<MediaStream>& res, UsageEnvironment* env);
int createMPEG2TransportStreamIndexer(const std::string inputFileName,UsageEnvironment* env);

DynamicRTSPServer*
DynamicRTSPServer::createNew(UsageEnvironment& env, Port ourPort,
			     UserAuthenticationDatabase* authDatabase,
			     unsigned reclamationTestSeconds) {
  int ourSocket = setUpOurSocket(env, ourPort);
  if (ourSocket == -1) return NULL;

  return new DynamicRTSPServer(env, ourSocket, ourPort, authDatabase, reclamationTestSeconds);
}

DynamicRTSPServer::DynamicRTSPServer(UsageEnvironment& env, int ourSocket,
				     Port ourPort,
				     UserAuthenticationDatabase* authDatabase, unsigned reclamationTestSeconds)
  : RTSPServerSupportingHTTPStreaming(env, ourSocket, ourPort, authDatabase, reclamationTestSeconds) {
}

DynamicRTSPServer::~DynamicRTSPServer() {
}

static ServerMediaSession* createNewSMS(UsageEnvironment& env,
					char const* fileName, FILE* fid); // forward

ServerMediaSession*
DynamicRTSPServer::lookupServerMediaSession(char const* streamName) {
  // First, check whether the specified "streamName" exists as a local file:
  FILE* fid = fopen(streamName, "rb");
  Boolean fileExists = fid != NULL;

  // Next, check whether we already have a "ServerMediaSession" for this file:
  ServerMediaSession* sms = RTSPServer::lookupServerMediaSession(streamName);
  Boolean smsExists = sms != NULL;

  // Handle the four possibilities for "fileExists" and "smsExists":
  if (!fileExists) {
    if (smsExists) {
      // "sms" was created for a file that no longer exists. Remove it:
      removeServerMediaSession(sms);
    }
    return NULL;
  } else {
    if (!smsExists) {
      // Create a new "ServerMediaSession" object for streaming from the named file.
      sms = createNewSMS(envir(), streamName, fid);
      addServerMediaSession(sms);
    }
    fclose(fid);
    return sms;
  }
}

// Special code for handling Matroska files:
static char newMatroskaDemuxWatchVariable;
static MatroskaFileServerDemux* demux;
static void onMatroskaDemuxCreation(MatroskaFileServerDemux* newDemux, void* /*clientData*/) {
  demux = newDemux;
  newMatroskaDemuxWatchVariable = 1;
}
// END Special code for handling Matroska files:

#define NEW_SMS(description) do {\
char const* descStr = description\
    ", streamed by the LIVE555 Media Server";\
sms = ServerMediaSession::createNew(env, fileName, fileName, descStr);\
} while(0)

static ServerMediaSession* createNewSMS(UsageEnvironment& env,
					char const* fileName, FILE* /*fid*/) {
  // Use the file name extension to determine the type of "ServerMediaSession":
  char const* extension = strrchr(fileName, '.');
  if (extension == NULL) return NULL;

  ServerMediaSession* sms = NULL;
  Boolean const reuseSource = False;
  if (strcmp(extension, ".aac") == 0) {
    // Assumed to be an AAC Audio (ADTS format) file:
    NEW_SMS("AAC Audio");
    sms->addSubsession(ADTSAudioFileServerMediaSubsession::createNew(env, fileName, reuseSource));
  } else if (strcmp(extension, ".amr") == 0) {
    // Assumed to be an AMR Audio file:
    NEW_SMS("AMR Audio");
    sms->addSubsession(AMRAudioFileServerMediaSubsession::createNew(env, fileName, reuseSource));
  } else if (strcmp(extension, ".ac3") == 0) {
    // Assumed to be an AC-3 Audio file:
    NEW_SMS("AC-3 Audio");
    sms->addSubsession(AC3AudioFileServerMediaSubsession::createNew(env, fileName, reuseSource));
  } else if (strcmp(extension, ".m4e") == 0) {
    // Assumed to be a MPEG-4 Video Elementary Stream file:
    NEW_SMS("MPEG-4 Video");
    sms->addSubsession(MPEG4VideoFileServerMediaSubsession::createNew(env, fileName, reuseSource));
  } else if (strcmp(extension, ".264") == 0) {
    // Assumed to be a H.264 Video Elementary Stream file:
    NEW_SMS("H.264 Video");
    OutPacketBuffer::maxSize = 100000; // allow for some possibly large H.264 frames
    sms->addSubsession(H264VideoFileServerMediaSubsession::createNew(env, fileName, reuseSource));
  } else if (strcmp(extension, ".mp3") == 0) {
    // Assumed to be a MPEG-1 or 2 Audio file:
    NEW_SMS("MPEG-1 or 2 Audio");
    // To stream using 'ADUs' rather than raw MP3 frames, uncomment the following:
//#define STREAM_USING_ADUS 1
    // To also reorder ADUs before streaming, uncomment the following:
//#define INTERLEAVE_ADUS 1
    // (For more information about ADUs and interleaving,
    //  see <http://www.live555.com/rtp-mp3/>)
    Boolean useADUs = False;
    Interleaving* interleaving = NULL;
#ifdef STREAM_USING_ADUS
    useADUs = True;
#ifdef INTERLEAVE_ADUS
    unsigned char interleaveCycle[] = {0,2,1,3}; // or choose your own...
    unsigned const interleaveCycleSize
      = (sizeof interleaveCycle)/(sizeof (unsigned char));
    interleaving = new Interleaving(interleaveCycleSize, interleaveCycle);
#endif
#endif
    sms->addSubsession(MP3AudioFileServerMediaSubsession::createNew(env, fileName, reuseSource, useADUs, interleaving));
  } else if (strcmp(extension, ".mpg") == 0) {
    // Assumed to be a MPEG-1 or 2 Program Stream (audio+video) file:
    NEW_SMS("MPEG-1 or 2 Program Stream");
    MPEG1or2FileServerDemux* demux
      = MPEG1or2FileServerDemux::createNew(env, fileName, reuseSource);
    sms->addSubsession(demux->newVideoServerMediaSubsession());
    sms->addSubsession(demux->newAudioServerMediaSubsession());
  } else if (strcmp(extension, ".vob") == 0) {
    // Assumed to be a VOB (MPEG-2 Program Stream, with AC-3 audio) file:
    NEW_SMS("VOB (MPEG-2 video with AC-3 audio)");
    MPEG1or2FileServerDemux* demux
      = MPEG1or2FileServerDemux::createNew(env, fileName, reuseSource);
    sms->addSubsession(demux->newVideoServerMediaSubsession());
    sms->addSubsession(demux->newAC3AudioServerMediaSubsession());
  } else if (strcmp(extension, ".ts") == 0) {
    // Assumed to be a MPEG Transport Stream file:
    // Use an index file name that's the same as the TS file name, except with ".tsx":
    unsigned indexFileNameLen = strlen(fileName) + 2; // allow for trailing "x\0"
    char* indexFileName = new char[indexFileNameLen];
    sprintf(indexFileName, "%sx", fileName);
    createMPEG2TransportStreamIndexer(fileName, &env);
    NEW_SMS("MPEG Transport Stream");
    sms->addSubsession(MPEG2TransportFileServerMediaSubsession::createNew(env, fileName, indexFileName, reuseSource));
    delete[] indexFileName;
  } else if (strcmp(extension, ".wav") == 0) {
    // Assumed to be a WAV Audio file:
    NEW_SMS("WAV Audio Stream");
    // To convert 16-bit PCM data to 8-bit u-law, prior to streaming,
    // change the following to True:
    Boolean convertToULaw = False;
    sms->addSubsession(WAVAudioFileServerMediaSubsession::createNew(env, fileName, reuseSource, convertToULaw));
  } else if (strcmp(extension, ".dv") == 0) {
    // Assumed to be a DV Video file
    // First, make sure that the RTPSinks' buffers will be large enough to handle the huge size of DV frames (as big as 288000).
    OutPacketBuffer::maxSize = 300000;

    NEW_SMS("DV Video");
    sms->addSubsession(DVVideoFileServerMediaSubsession::createNew(env, fileName, reuseSource));
  } else if (strcmp(extension, ".mkv") == 0 || strcmp(extension, ".webm") == 0) {
    // As mkv format is not support good, I don't want to stream it.
    goto CONVERT;
    // Assumed to be a Matroska file (note that WebM ('.webm') files are also Matroska files)
    NEW_SMS("Matroska video+audio+(optional)subtitles");
    //not execute now
    // Create a Matroska file server demultiplexor for the specified file.  (We enter the event loop to wait for this to complete.)
    newMatroskaDemuxWatchVariable = 0;
    MatroskaFileServerDemux::createNew(env, fileName, onMatroskaDemuxCreation, NULL);
    env.taskScheduler().doEventLoop(&newMatroskaDemuxWatchVariable);

    ServerMediaSubsession* smss;
    while ((smss = demux->newServerMediaSubsession()) != NULL) {
      sms->addSubsession(smss);
    }
  }
  //mp4 code start.
  //can noly process video encoder H264
  else if((strcmp(extension, ".mp4") == 0) || (strcmp(extension, ".m4v") == 0)){   
    //read mp4 info
CONVERT:
    std::string cmd = std::string("ffmpeg -i ") +  fileName + " 2>&1";
    std::vector<MediaStream> res;
    int r = readTrackinfo(cmd.c_str(), res);
    if(r < 0){
      return sms;
    }

    for(std::size_t i = 0; i < res.size(); i++){
      std::cout << res[i].streamType << ": " << res[i].content << endl;
    }

    const string destFormat = "ts";
    const string destpath = string(fileName) + ".ts";
    r = convertMediaWithoutReEncoding(fileName, destpath, destFormat, res, &env);
    if(r < 0){
      return sms;
    }
    if(destFormat == "ts"){
      NEW_SMS("MPEG Transport Stream");
      sms->addSubsession(MPEG2TransportFileServerMediaSubsession::createNew(env, destpath.c_str(), 
            (destpath + 'x').c_str(), reuseSource)); 
    }
    else{

    }
  }
  //mp4 code end.
  return sms;
}

//read the encoding information about the file with ffprobe
int readTrackinfo(const char *cmd, vector<MediaStream>& res)   
{   
    char buf[1024];   
    char ps[1024]={0};   
    FILE *ptr;   
    strcpy(ps, cmd);   
    //execute the cmd and read it from the pipe with popen func
    if((ptr=popen(ps, "r"))!=NULL)   
    {   
        while(fgets(buf, 1024, ptr)!=NULL)   
        {   
          char* sub = strstr(buf, "Stream #");
          if(sub == NULL)
            continue;
          MediaStream ms;
          sub = strstr(buf, "Video: ");
          if(sub != NULL)
          {
            ms.streamType = Video;
            sub = sub + strlen("Video: ");
            char* from = sub;
            while(*sub != ' ' && *sub != ',' && *sub != '\0' && *sub != '\n'){
              sub++;
            }
            ms.content.append(from, sub - from);
            res.push_back(ms);
          }
          else if((sub = strstr(buf, "Audio: ")) != NULL)
          {
            ms.streamType = Audio;
            sub = sub + strlen("Audio: ");
            char* from = sub;
            while(*sub != ' ' && *sub != ',' && *sub != '\0' && *sub != '\n'){
              sub++;
            }
            ms.content.append(from, sub - from);
            res.push_back(ms);
          }
          else if((sub = strstr(buf, "Subtitle: ")) != NULL)
          {
            ms.streamType = Subtitle;
            fgets(buf, 1024, ptr);
            fgets(buf, 1024, ptr);
            sub = strstr(buf, ": ") + strlen(": ");
            char* from = sub;
            while(*sub != ' ' && *sub != ',' && *sub != '\0' && *sub != '\n'){
              sub++;
            }
            ms.content.append(from, sub - from);
            res.push_back(ms);
          }
          else
          {
            continue;
          }
        }   
        pclose(ptr);   
        ptr = NULL;   
        if(res.size() == 0){
          return -2;
        }
        return 0;
    }   
    else  
    {   
        return -1;
    }   
}  

//extract the media file and compose them with ffmpeg without re-encoding
int convertMediaWithoutReEncoding(const string srcFile, const string destpath, 
    const string destFormat, const vector<MediaStream>& res, UsageEnvironment* env)
{
  for(std::size_t i = 0; i < res.size(); i++){
    if(res[i].streamType == Video)
    {
      if(res[i].content == "h264")
      {
        //firstly, convert the file without re-encoding.
        const string cmd = string("echo N | ffmpeg") + " -i " + srcFile + \
            " -acodec copy -vcodec copy -scodec copy -vbsf h264_mp4toannexb " + destpath;
        pid_t status = system(cmd.c_str());
        if(WIFEXITED(status) < 0 || WEXITSTATUS(status) < 0){
          return -1;
        }
        //secondly, create a .tsx file
        if(destFormat == "ts")
        {
          createMPEG2TransportStreamIndexer(destpath, env);
        }
      }
      return 0;
    }
    /*else if(res[i].streamType == "Audio"){
      if(res[i].content == "aac"){
        string vname = string(fileName) + ".aac";
        const string cmd = string("echo N | ffmpeg") + " -i " + fileName + \
            " -acodec copy " + vname;
        system(cmd.c_str());
        //NEW_SMS("H.264 Video");  
        sms->addSubsession(ADTSAudioFileServerMediaSubsession::createNew(env, vname.c_str(), reuseSource)); 
      }
      else if(res[i].content == "ac3"){
        string vname = string(fileName) + ".ac3";
        const string cmd = string("echo N | ffmpeg") + " -i " + fileName + \
            " -acodec copy " + vname;
        system(cmd.c_str());
        // Assumed to be an AC-3 Audio file:
        //NEW_SMS("AC-3 Audio");
        sms->addSubsession(AC3AudioFileServerMediaSubsession::createNew(env, vname.c_str(), reuseSource));
      }
    }*/
    else
    {
      continue;
    }
  }
  return -1;
}

void afterPlaying(void* clientData) {
  puts("...done");
  char* watchVariable = (char*)clientData;
  *watchVariable = 1;
}

int createMPEG2TransportStreamIndexer(const std::string inputFileName,UsageEnvironment* env){
    // The output file name is the same as the input file name, except with suffix ".tsx":
  const std::string outputFileName = inputFileName + 'x';

  struct stat _stat;
  if(lstat(outputFileName.c_str(), &_stat) >=0  && !S_ISDIR(_stat.st_mode)) 
  {
    printf("%s already exist.\n", outputFileName.c_str());
    return 0;
  }

  // Begin by setting up our usage environment:
  //TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  //UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);

  //Open the input file as a "byte stream file source"
  FramedSource* input
    = ByteStreamFileSource::createNew(*env, inputFileName.c_str(), TRANSPORT_PACKET_SIZE);
  if (input == NULL) {
    *env << "Failed to open input file \"" << inputFileName.c_str() << "\" (does it exist?)\n";
    return -1;
  }
  // Create a filter that indexes the input Transport Stream data:
  FramedSource* indexer
    = MPEG2IFrameIndexFromTransportStream::createNew(*env, input);

  // Open the output file (for writing), as a 'file sink':
  MediaSink* output = FileSink::createNew(*env, outputFileName.c_str());
  if (output == NULL) {
    *env << "Failed to open output file \"" << outputFileName.c_str() << "\"\n";
    return -1;
  }
  // Start playing, to generate the output index file:
  *env << "Writing index file \"" << outputFileName.c_str() << "\"...";

  char watchVariable = 0;
  output->startPlaying(*indexer, afterPlaying, (void*)&watchVariable);

  //char fWatchValue = 0;
  env->taskScheduler().doEventLoop(&watchVariable);

  //delete scheduler;
  return 0;
}