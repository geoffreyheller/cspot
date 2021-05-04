#ifndef CHUNKEDAUDIOSTREAM_H
#define CHUNKEDAUDIOSTREAM_H

#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include <unistd.h>
#include "ivorbisfile.h"
#include "MercuryManager.h"
#include "AudioSink.h"
#include "AudioChunk.h"
#include "platform/WrappedMutex.h"

#define SPOTIFY_HEADER_SIZE 167
#define BUFFER_SIZE 0x20000 * 1.5
typedef std::function<void(std::vector<uint8_t>&)> pcmDataCallback;

enum class Whence
{
    START,
    CURRENT,
    END
};

class ChunkedAudioStream
{
private:
    // Vorbis related
    OggVorbis_File vorbisFile;
    ov_callbacks vorbisCallbacks;
    int currentSection;

    // Audio chunking
    std::vector<uint8_t> audioKey;
    std::vector<std::shared_ptr<AudioChunk>> chunks;

    // Audio data
    uint32_t duration;

    bool loadingChunks = false;
    uint16_t lastSequenceId = 0;

    std::shared_ptr<MercuryManager> manager;
    std::vector<uint8_t> fileId;
    uint32_t startPositionMs;

    std::shared_ptr<AudioChunk> requestChunk(size_t chunkIndex);
    void fetchTraillingPacket();
    std::shared_ptr<AudioChunk> findChunkForPosition(size_t position);

public:
    ChunkedAudioStream(std::vector<uint8_t> fileId, std::vector<uint8_t> audioKey, uint32_t duration, std::shared_ptr<MercuryManager> manager, uint32_t startPositionMs);
    ~ChunkedAudioStream();
    int requestedChunkIndex = 0;
    std::function<void()> streamFinishedCallback;
    size_t pos = SPOTIFY_HEADER_SIZE; // size of some spotify header
    uint32_t fileSize;
    uint32_t readBeforeSeek = 0;
    bool loadingMeta = true;
    bool isPaused = false;
    bool isRunning = false;
    bool finished = false;
    pcmDataCallback pcmCallback;
    std::shared_ptr<AudioSink> audioSink;
    WrappedMutex seekMutex;

    std::vector<uint8_t> read(size_t bytes);
    void seekMs(uint32_t positionMs);
    void seek(size_t pos, Whence whence);
    void startPlaybackLoop();
};

#endif
