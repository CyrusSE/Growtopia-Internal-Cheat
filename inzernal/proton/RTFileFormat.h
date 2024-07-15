#ifndef RTFileFormat_h__
#define RTFileFormat_h__
#include <cstdint>

//all RT file formats start with the same main header...

//Don't change!
#define C_RTFILE_PACKAGE_LATEST_VERSION 0
#define C_RTFILE_PACKAGE_HEADER "RTPACK"
#define C_RTFILE_PACKAGE_HEADER_BYTE_SIZE 6

struct RTFileHeader {
	char fileTypeID[C_RTFILE_PACKAGE_HEADER_BYTE_SIZE];
	uint8_t version;
	uint8_t reserved[1];
};


enum eCompressionType {
	C_COMPRESSION_NONE = 0,
	C_COMPRESSION_ZLIB = 1
};



struct rtpack_header {
	RTFileHeader rtFileHeader;
	unsigned int compressedSize;
	unsigned int decompressedSize;
	uint8_t compressionType; //one of eCompressionType
	uint8_t reserved[15];
};

#define RT_FORMAT_EMBEDDED_FILE 20000000
#define C_RTFILE_TEXTURE_HEADER "RTTXTR"
struct rttex_header {
	RTFileHeader rtFileHeader;

	//our custom header
	int height;
	int width;
	int format; // probably GL_UNSIGNED_BYTE , or GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4, or RT_FORMAT_EMBEDDED_FILE for jpgs, etc
	int originalHeight; //before we padded to be a power of 2, if applicable.
	int originalWidth; //before we padded to be a power of 2, if applicable.
	unsigned char bUsesAlpha;
	unsigned char bAlreadyCompressed; //if 1, it means we don't require additional compression
	unsigned char reservedFlags[2]; //keep this struct packed right
	int mipmapCount; //how many tex infos are followed...

	int reserved[16];

	//texture info to follow, 1 for each mip map
};


struct rttex_mip_header {
	int height;
	int width;
	int dataSize;
	int mipLevel;
	int reserved[2];
};


//generic shit to store everything rt struct related
class rtfile_t {
public:
	rttex_header* hdr;
	rttex_mip_header* mips;
	uint8_t* data;
	rtfile_t() {}
	rtfile_t(uint8_t* data) { load(data); }
	void load(uint8_t* data) {
		uint8_t* pos = data;
		this->hdr = (rttex_header*)pos; pos += sizeof(rttex_header);
		this->mips = (rttex_mip_header*)pos; pos += sizeof(rttex_mip_header);
		this->data = (uint8_t*)pos;
	}
};

class rtpackedfile_t {
public:
	rtpack_header* hdr;
	void* data;
	rtpackedfile_t(rtpack_header* hdr) {
		this->hdr = hdr;
		this->data = nullptr;
	}
	~rtpackedfile_t() {}
};

#endif // RTFileFormat_h__
