#include "FormatFilter.h"

FormatFilter::FormatFilter(){

}

set<string> FormatFilter::supportedFormats;

void FormatFilter::init()
{
	supportedFormats.clear();
	supportedFormats.insert(".3g2"); //3GP2 format
	supportedFormats.insert(".3gp"); //3GP format
	supportedFormats.insert(".4xm"); //4X Technologies format
	supportedFormats.insert(".a64"); //a64 video for Commodore 64
	supportedFormats.insert(".aac"); //raw ADTS AAC
	supportedFormats.insert(".ac3"); //raw AC-3
	supportedFormats.insert(".act"); //ACT Voice file format
	supportedFormats.insert(".adf"); //Artworx Data Format
	supportedFormats.insert(".adts"); //ADTS AAC
	supportedFormats.insert(".adx"); //CRI ADX
	supportedFormats.insert(".aea"); //MD STUDIO audio
	supportedFormats.insert(".aiff"); //Audio IFF
	supportedFormats.insert(".alaw"); //PCM A-law format
	supportedFormats.insert(".amr"); //3GPP AMR file format
	supportedFormats.insert(".anm"); //Deluxe Paint Animation
	supportedFormats.insert(".apc"); //CRYO APC format
	supportedFormats.insert(".ape"); //Monkey's Audio
	supportedFormats.insert(".asf"); //ASF format
	supportedFormats.insert(".asf_stream"); //ASF format
	supportedFormats.insert(".au"); //SUN AU format
	supportedFormats.insert(".avi"); //AVI format
	supportedFormats.insert(".avm2"); //Flash 9 (AVM2) format
	supportedFormats.insert(".avs"); //AVISynth
	supportedFormats.insert(".bethsoftvid"); //Bethesda Softworks VID format
	supportedFormats.insert(".bfi"); //Brute Force & Ignorance
	supportedFormats.insert(".bin"); //Binary text
	supportedFormats.insert(".bink"); //Bink
	supportedFormats.insert(".bit"); //G.729 BIT file format
	supportedFormats.insert(".bmv"); //Discworld II BMV
	supportedFormats.insert(".c93"); //Interplay C93
	supportedFormats.insert(".caf"); //Apple Core Audio Format
	supportedFormats.insert(".cavsvideo"); //raw Chinese AVS video
	supportedFormats.insert(".cdg"); //CD Graphics Format
	supportedFormats.insert(".cdxl"); //Commodore CDXL video format
	supportedFormats.insert(".crc"); //CRC testing format
	supportedFormats.insert(".daud"); //D-Cinema audio format
	supportedFormats.insert(".dfa"); //Chronomaster DFA
	supportedFormats.insert(".dirac"); //raw Dirac
	supportedFormats.insert(".dnxhd"); //raw DNxHD (SMPTE VC-3)
	supportedFormats.insert(".dshow"); //DirectShow capture
	supportedFormats.insert(".dsicin"); //Delphine Software International CIN format
	supportedFormats.insert(".dts"); //raw DTS
	supportedFormats.insert(".dv"); //DV video format
	supportedFormats.insert(".dvd"); //MPEG-2 PS format (DVD VOB)
	supportedFormats.insert(".dxa"); //DXA
	supportedFormats.insert(".ea"); //Electronic Arts Multimedia Format
	supportedFormats.insert(".ea_cdata"); //Electronic Arts cdata
	supportedFormats.insert(".eac3"); //raw E-AC-3
	supportedFormats.insert(".f32be"); //PCM 32 bit floating-point big-endian format
	supportedFormats.insert(".f32le"); //PCM 32 bit floating-point little-endian format
	supportedFormats.insert(".f64be"); //PCM 64 bit floating-point big-endian format
	supportedFormats.insert(".f64le"); //PCM 64 bit floating-point little-endian format
	supportedFormats.insert(".ffm"); //FFM (FFserver live feed) format
	supportedFormats.insert(".ffmetadata"); //FFmpeg metadata in text format
	supportedFormats.insert(".film_cpk"); //Sega FILM/CPK format
	supportedFormats.insert(".filmstrip"); //Adobe Filmstrip
	supportedFormats.insert(".flac"); //raw FLAC
	supportedFormats.insert(".flic"); //FLI/FLC/FLX animation format
	supportedFormats.insert(".flv"); //FLV format
	supportedFormats.insert(".framecrc"); //framecrc testing format
	supportedFormats.insert(".framemd5"); //Per-frame MD5 testing format
	supportedFormats.insert(".g722"); //raw G.722
	supportedFormats.insert(".g723_1"); //raw G.723.1
	supportedFormats.insert(".g729"); //G.729 raw format demuxer
	supportedFormats.insert(".gif"); //GIF Animation
	supportedFormats.insert(".gsm"); //raw GSM
	supportedFormats.insert(".gxf"); //GXF format
	supportedFormats.insert(".h261"); //raw H.261
	supportedFormats.insert(".h263"); //raw H.263
	supportedFormats.insert(".h264"); //raw H.264 video format
	supportedFormats.insert(".hls"); //Apple HTTP Live Streaming format
	supportedFormats.insert(".applehttp"); //Apple HTTP Live Streaming format
	supportedFormats.insert(".ico"); //Microsoft Windows ICO
	supportedFormats.insert(".idcin"); //id Cinematic format
	supportedFormats.insert(".idf"); //iCE Draw File
	supportedFormats.insert(".iff"); //Interchange File Format
	supportedFormats.insert(".ilbc"); //iLBC storage file format
	supportedFormats.insert(".image2"); //image2 sequence
	supportedFormats.insert(".image2pipe"); //piped image2 sequence
	supportedFormats.insert(".ingenient"); //raw Ingenient MJPEG
	supportedFormats.insert(".ipmovie"); //Interplay MVE format
	supportedFormats.insert(".ipod"); //iPod H.264 MP4 format
	supportedFormats.insert(".ismv"); //ISMV/ISMA (Smooth Streaming) format
	supportedFormats.insert(".iss"); //Funcom ISS format
	supportedFormats.insert(".iv8"); //A format generated by IndigoVision 8000 video server
	supportedFormats.insert(".ivf"); //On2 IVF
	supportedFormats.insert(".jv"); //Bitmap Brothers JV
	supportedFormats.insert(".latm"); //LOAS/LATM
	supportedFormats.insert(".lavfi"); //Libavfilter virtual input device
	supportedFormats.insert(".libnut"); //nut format
	supportedFormats.insert(".lmlm4"); //lmlm4 raw format
	supportedFormats.insert(".loas"); //LOAS AudioSyncStream
	supportedFormats.insert(".lxf"); //VR native stream format (LXF)
	supportedFormats.insert(".m4v"); //raw MPEG-4 video format
	supportedFormats.insert(".matroska"); //Matroska file format
	supportedFormats.insert(".webm"); //WebM file format
	supportedFormats.insert(".md5"); //MD5 testing format
	supportedFormats.insert(".mgsts"); //Metal Gear Solid: The Twin Snakes
	supportedFormats.insert(".mjpeg"); //raw MJPEG video
	supportedFormats.insert(".mkvtimestamp_v2"); //extract pts as timecode v2 format, as defined by mkvtoolnix
	supportedFormats.insert(".mlp"); //raw MLP
	supportedFormats.insert(".mm"); //American Laser Games MM format
	supportedFormats.insert(".mmf"); //Yamaha SMAF
	supportedFormats.insert(".mov"); //MOV format
	supportedFormats.insert(".m4a"); //QuickTime/MPEG-4/Motion JPEG 2000 format
	supportedFormats.insert(".mj2"); //QuickTime/MPEG-4/Motion JPEG 2000 format
	supportedFormats.insert(".mp2"); //MPEG audio layer 2
	supportedFormats.insert(".mp3"); //MPEG audio layer 3
	supportedFormats.insert(".mp4"); //MP4 format
	supportedFormats.insert(".mpc"); //Musepack
	supportedFormats.insert(".mpc8"); //Musepack SV8
	supportedFormats.insert(".mpeg"); //MPEG-1 System format
	supportedFormats.insert(".mpeg1video"); //raw MPEG-1 video
	supportedFormats.insert(".mpeg2video"); //raw MPEG-2 video
	supportedFormats.insert(".mpegts"); //MPEG-2 transport stream format
	supportedFormats.insert(".mpegtsraw"); //MPEG-2 raw transport stream format
	supportedFormats.insert(".mpegvideo"); //raw MPEG video
	supportedFormats.insert(".mpjpeg"); //MIME multipart JPEG format
	supportedFormats.insert(".msnwctcp"); //MSN TCP Webcam stream
	supportedFormats.insert(".mtv"); //MTV format
	supportedFormats.insert(".mulaw"); //PCM mu-law format
	supportedFormats.insert(".mvi"); //Motion Pixels MVI format
	supportedFormats.insert(".mxf"); //Material eXchange Format
	supportedFormats.insert(".mxf_d10"); //Material eXchange Format, D-10 Mapping
	supportedFormats.insert(".mxg"); //MxPEG clip file format
	supportedFormats.insert(".nc"); //NC camera feed format
	supportedFormats.insert(".nsv"); //Nullsoft Streaming Video
	supportedFormats.insert(".null"); //raw null video format
	supportedFormats.insert(".nut"); //NUT format
	supportedFormats.insert(".nuv"); //NuppelVideo format
	supportedFormats.insert(".ogg"); //Ogg
	supportedFormats.insert(".oma"); //Sony OpenMG audio
	supportedFormats.insert(".pmp"); //Playstation Portable PMP format
	supportedFormats.insert(".psp"); //PSP MP4 format
	supportedFormats.insert(".psxstr"); //Sony Playstation STR format
	supportedFormats.insert(".pva"); //TechnoTrend PVA file and stream format
	supportedFormats.insert(".qcp"); //QCP format
	supportedFormats.insert(".r3d"); //REDCODE R3D format
	supportedFormats.insert(".rawvideo"); //raw video format
	supportedFormats.insert(".rcv"); //VC-1 test bitstream
	supportedFormats.insert(".rl2"); //RL2 format
	supportedFormats.insert(".rm"); //RealMedia format
	supportedFormats.insert(".roq"); //raw id RoQ format
	supportedFormats.insert(".rpl"); //RPL/ARMovie format
	supportedFormats.insert(".rso"); //Lego Mindstorms RSO format
	supportedFormats.insert(".rtp"); //RTP output format
	supportedFormats.insert(".rtsp"); //RTSP output format
	supportedFormats.insert(".s16be"); //PCM signed 16 bit big-endian format
	supportedFormats.insert(".s16le"); //PCM signed 16 bit little-endian format
	supportedFormats.insert(".s24be"); //PCM signed 24 bit big-endian format
	supportedFormats.insert(".s24le"); //PCM signed 24 bit little-endian format
	supportedFormats.insert(".s32be"); //PCM signed 32 bit big-endian format
	supportedFormats.insert(".s32le"); //PCM signed 32 bit little-endian format
	supportedFormats.insert(".s8"); //PCM signed 8 bit format
	supportedFormats.insert(".sap"); //SAP output format
	supportedFormats.insert(".sbg"); //SBaGen binaural beats script
	supportedFormats.insert(".sdl"); //SDL output device
	supportedFormats.insert(".sdp"); //SDP
	supportedFormats.insert(".segment"); //segment muxer
	supportedFormats.insert(".shn"); //raw Shorten
	supportedFormats.insert(".siff"); //Beam Software SIFF
	supportedFormats.insert(".smjpeg"); //Loki SDL MJPEG
	supportedFormats.insert(".smk"); //Smacker video
	supportedFormats.insert(".smush"); //LucasArts Smush
	supportedFormats.insert(".sol"); //Sierra SOL format
	supportedFormats.insert(".sox"); //SoX native format
	supportedFormats.insert(".spdif"); //IEC 61937 (used on S/PDIF - IEC958)
	supportedFormats.insert(".stream_segment"); //streaming segment muxer
	supportedFormats.insert(".ssegment"); //streaming segment muxer
	supportedFormats.insert(".svcd"); //MPEG-2 PS format (VOB)
	supportedFormats.insert(".swf"); //Flash format
	supportedFormats.insert(".thp"); //THP
	supportedFormats.insert(".tiertexseq"); //Tiertex Limited SEQ format
	supportedFormats.insert(".tmv"); //8088flex TMV
	supportedFormats.insert(".truehd"); //raw TrueHD
	supportedFormats.insert(".tta"); //True Audio
	supportedFormats.insert(".txd"); //Renderware TeXture Dictionary
	supportedFormats.insert(".u16be"); //PCM unsigned 16 bit big-endian format
	supportedFormats.insert(".u16le"); //PCM unsigned 16 bit little-endian format
	supportedFormats.insert(".u24be"); //PCM unsigned 24 bit big-endian format
	supportedFormats.insert(".u24le"); //PCM unsigned 24 bit little-endian format
	supportedFormats.insert(".u32be"); //PCM unsigned 32 bit big-endian format
	supportedFormats.insert(".u32le"); //PCM unsigned 32 bit little-endian format
	supportedFormats.insert(".u8"); //PCM unsigned 8 bit format
	supportedFormats.insert(".vc1"); //raw VC-1
	supportedFormats.insert(".vc1test"); //VC-1 test bitstream format
	supportedFormats.insert(".vcd"); //MPEG-1 System format (VCD)
	supportedFormats.insert(".vfwcap"); //VfW video capture
	supportedFormats.insert(".vmd"); //Sierra VMD format
	supportedFormats.insert(".vob"); //MPEG-2 PS format (VOB)
	supportedFormats.insert(".voc"); //Creative Voice file format
	supportedFormats.insert(".vqf"); //Nippon Telegraph and Telephone Corporation (NTT) TwinVQ
	supportedFormats.insert(".w64"); //Sony Wave64 format
	supportedFormats.insert(".wav"); //WAV format
	supportedFormats.insert(".wc3movie"); //Wing Commander III movie format
	supportedFormats.insert(".webm"); //WebM file format
	supportedFormats.insert(".wsaud"); //Westwood Studios audio format
	supportedFormats.insert(".wsvqa"); //Westwood Studios VQA format
	supportedFormats.insert(".wtv"); //Windows Television (WTV)
	supportedFormats.insert(".wv"); //WavPack
	supportedFormats.insert(".xa"); //Maxis XA File Format
	supportedFormats.insert(".xbin"); //eXtended BINary text (XBIN)
	supportedFormats.insert(".xmv"); //Microsoft XMV
	supportedFormats.insert(".xwma"); //Microsoft xWMA
	supportedFormats.insert(".yop"); //Psygnosis YOP Format
	supportedFormats.insert(".yuv4mpegpipe"); //YUV4MPEG pipe format	
	supportedFormats.insert(".mkv"); //Matroska file format	
	supportedFormats.insert(".ts"); //
}

bool FormatFilter::isSupportFormat(const std::string& extension)
{
	if(supportedFormats.empty())
		init();
	if(supportedFormats.find(extension) != supportedFormats.end())	
		return true;
	return false;
}
