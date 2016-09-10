#include <iostream>
#include "config.h"
using namespace std;


#ifdef HAVE_ZLIB
	#include <zlib.h>
	void hello()
	{
		//Output a string, compressed in gzip format.
		unsigned char in[] = "Hello, I am a program\n";
		unsigned char out[1024] = {};

		z_stream strm = {};
		deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 8+16, 9, Z_DEFAULT_STRATEGY);
		strm.next_in = in;
		strm.avail_in = sizeof(in);
		strm.next_out = out;
		strm.avail_out = sizeof(out);
		deflate(&strm, Z_FINISH);
		deflateEnd(&strm);

		cout.write((const char*)out, strm.total_out);
	}
#else
	void hello()
	{
		char data[]={0x1f,0x8b,0x08,0x00,0x00,0x00,0x00,0x00,0x00,
					 0x03,0xf3,0x48,0xcd,0xc9,0xc9,0xd7,0x51,0xf0,
					 0x54,0x48,0xcc,0x55,0x48,0x54,0x28,0x28,0xca,
					 0x4f,0x2f,0x4a,0xcc,0xe5,0x62,0x00,0x00,0x45,
					 0x29,0xde,0xb8,0x17,0x00,0x00,0x00};

		cout.write(data, sizeof(data));
	}
#endif

int main()
{
	hello();
}
