/*
Jasper ter Weeme
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <iomanip>

#ifdef __GNUC__
#include <endian.h>
#endif

struct Header
{
    uint32_t chunkID;
    uint32_t chunkSize;
    uint16_t formatType;
    uint16_t tracks;
    uint16_t timeDivision;
};

class Utility
{
public:
    static inline uint32_t be_32_toh(uint32_t x);
    static inline uint16_t be_16_toh(uint16_t x);
    static void hex(uint8_t *data, size_t len);
};

class CHeader
{
private:
    Header header;
public:
    CHeader() { }
    CHeader(Header header) { this->header = header; }
    CHeader(std::istream iStream) { this->read(iStream); }
    void read(std::istream &inFile);
    int getTrackCount() { return ::Utility::be_16_toh(header.tracks); }
    std::string toString();
};

class CTrack
{
private:
    uint32_t chunkIDBE;
    uint32_t chunkSizeBE;
    uint8_t *data;

    static const uint8_t META_TAG = 0xff;
    static const uint8_t TIME_SIGNATURE = 0x58;
    
public:
    void read(std::istream &iStream);
    std::string toString();
    size_t getChunkSize() { return ::Utility::be_32_toh(chunkSizeBE); }
    uint8_t *getRawData() { return data; }
    void parse();
};

class KarFile
{
private:
    CHeader header;
    std::vector<CTrack> tracks;
 public:
    void read(std::istream &iStream);
    void dump();
    CTrack getTrack(int n) { return tracks[n]; }
};

class KarParser1
{
public:
    int run();
};

void CTrack::parse()
{
    uint8_t timeSignature;
    uint8_t chunkSize;

    for (int i = 0; i < getChunkSize(); i++)
    {
        if (data[i] == META_TAG)
        {
            timeSignature = data[++i];
            //std::cout << "Time Signature: " << (int)timeSignature << std::endl;
            chunkSize = data[++i];
            //std::cout << "Chunk Size: " << (int)chunkSize << std::endl;
        }

    }
}

void CHeader::read(std::istream &inFile)
{
    inFile.read((char *)&header, 14);   // moet 14 zijn!
}

void CTrack::read(std::istream &iStream)
{
    iStream.read((char *)&chunkIDBE, sizeof(chunkIDBE));
    iStream.read((char *)&chunkSizeBE, sizeof(chunkSizeBE));
    size_t chunkSize = ::Utility::be_32_toh(chunkSizeBE);
    data = new uint8_t[chunkSize];
    iStream.read((char *)data, chunkSize);
}

uint16_t Utility::be_16_toh(uint16_t x)
{
#ifdef __GNUC__
    return be16toh(x);
#endif
	return (x & 0x00ff) << 8 | (x & 0xff00) >> 8;
}

uint32_t Utility::be_32_toh(uint32_t x)
{
#ifdef __GNUC__
    return be32toh(x);
#endif
	x = (x & 0x0000FFFF) << 16 | (x & 0xFFFF0000) >> 16;
	return (x & 0x00FF00FF) << 8 | (x & 0xFF00FF00) >> 8;
}

std::string CTrack::toString()
{
    std::stringstream ss;
    ss << "[Track]" << std::endl;
    ss << "Signature: 0x" << std::hex << Utility::be_32_toh(chunkIDBE) << std::endl;
    ss << "Chunk Size: " << std::dec << Utility::be_32_toh(chunkSizeBE);
    return ss.str();
}

std::string CHeader::toString()
{
    std::stringstream ss;
    ss << "[Header]" << std::endl;
    ss << "Signature: " << std::hex << header.chunkID << std::endl;
    ss << "Chunk Size: " << std::dec << ::Utility::be_32_toh(header.chunkSize) << std::endl;
	ss << "Format Type: " << ::Utility::be_16_toh(header.formatType) << std::endl;
	ss << "Tracks: " << ::Utility::be_16_toh(header.tracks) << std::endl;
	ss << "Time Division: " << ::Utility::be_16_toh(header.timeDivision);
    return ss.str();
}

void KarFile::read(std::istream &iStream)
{
    header.read(iStream);

    // read all tracks
    for (int i = 0; i < header.getTrackCount(); i++)
    {
        CTrack currentTrack;
        currentTrack.read(iStream);
        tracks.push_back(currentTrack);
    }
}

void KarFile::dump()
{
    std::cout << header.toString() << std::endl << std::endl;

    for (std::vector<CTrack>::iterator it = tracks.begin(); it != tracks.end(); ++it)
        std::cout << it->toString() << std::endl << std::endl;
}

void Utility::hex(uint8_t *data, size_t len)
{
    std::cout << std::hex;

/*
    for (int i = 0, column = 0; i < len; i++)
    {
        std::cout << std::setw(2) << std::setfill('0') << (int)data[i];

        if (++column >= 16)
        {
            std::cout << std::endl;
            column = 0;
        }
    }
*/

    for (int i = 0; i < len; i++)
    {
        std::cout << std::hex;

        for (int j = i; (j < (i + 16)) && (j < len); ++j)
            std::cout << std::setw(2) << std::setfill('0') << (int)data[j] << " ";

        for (int j = i; (j < i + 16) && j < len; ++j)
        {
            if (data[j] > 0x20)
                std::cout << data[j];
            else
                std::cout << ".";
        }

        std::cout << std::endl;
        i += 16;
    }
}

int KarParser1::run()
{
    KarFile karFile;
    karFile.read(std::cin);
    karFile.dump();

    CTrack track4 = karFile.getTrack(2);
    track4.parse();
    std::cout << track4.toString() << std::endl;
    uint8_t *track4data = track4.getRawData();
    Utility::hex(track4data, track4.getChunkSize());
    std::cout << std::endl;

    return 0;
}

int main()
{
    KarParser1 kp1;
    return kp1.run();
}

