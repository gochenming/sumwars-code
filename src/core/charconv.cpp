#include "charconv.h"
#include "debug.h"
#include "world.h"
#include <algorithm>

CharConv::CharConv()
#ifndef NO_RAKNET
	: m_bitstream()
#endif
{
#ifndef NO_RAKNET
	m_timestamp = RakNet::GetTime();
	m_bitstream.Write((char) ID_TIMESTAMP);
	m_bitstream.Write(m_timestamp);
#endif
	m_stream =0;
	m_version =World::getWorld()->getVersion();
}

CharConv::CharConv(int dummy)
#ifndef NO_RAKNET	
	: m_bitstream()
#endif	
{
	m_stream =0;
	m_version =World::getWorld()->getVersion();
}

CharConv::CharConv(unsigned char* data, unsigned int size)
#ifndef NO_RAKNET
	: m_bitstream((const char*) data, size, false)
#endif
{
#ifndef NO_RAKNET
	m_timestamp = RakNet::GetTime();
	m_stream=0;
	m_version =World::getWorld()->getVersion();
#endif
}


CharConv::CharConv(Packet* packet)
#ifndef NO_RAKNET
	: m_bitstream((const char*) packet->data, packet->length, false)
#endif
{
#ifndef NO_RAKNET
	if (packet->data[0] == ID_TIMESTAMP)
	{
		char tmp;
		m_bitstream.Read(tmp);
		m_bitstream.Read(m_timestamp);
	}
	else
	{
		m_timestamp = RakNet::GetTime();
	}
	m_stream=0;
	m_version =World::getWorld()->getVersion();
#else
	ERRORMSG("Called RakNet lib in NO_RAKNET build");
#endif
}


CharConv::CharConv(std::iostream* stream)
{
	m_stream = stream;
	m_version =World::getWorld()->getVersion();
}


void CharConv::toBuffer(const char* data, unsigned int size)
{
	if (m_stream ==0)
	{
#ifndef NO_RAKNET
		m_bitstream.Write(data,size);
#else
		ERRORMSG("Called RakNet lib in NO_RAKNET build");
#endif
	}
	else
	{
		m_stream->write(data,size);
	}
}

void CharConv::fromBuffer(char* data, unsigned int size)
{
	if (m_stream ==0)
	{
#ifndef NO_RAKNET
		m_bitstream.Read(data,size);
#else
		ERRORMSG("Called RakNet lib in NO_RAKNET build");
#endif
	}
	else
	{
		m_stream->read(data,size);
	}
}

void CharConv::toBuffer(std::string s, unsigned int size)
{
	if (m_stream ==0)
	{
		char * data = new char[size];
		memset(data,0,size);
		memcpy(data,s.data(), std::min(size,(unsigned int) s.size()));
		toBuffer(data, size);
		delete data;
	}
	else
	{
		(*m_stream) << s << " ";
	}
}

void CharConv::fromBuffer(std::string& s, unsigned int size)
{
	if (m_stream == 0)
	{
		char* data = new char[size+1];
		fromBuffer(data,size);
		data[size] = '\0';
		s = data;
		delete data;
	}
	else
	{
		(*m_stream) >> s;
	}
}

void CharConv::toBuffer(std::string s)
{
	if (m_stream ==0)
	{
		toBuffer<int>(s.size());
		if (s.size() !=0 )
		{
			toBuffer(s.data(), s.size());
		}
	}
	else
	{
		if (s=="")
		{
			(*m_stream) << "#empty# ";
		}
		else
		{
			int pos = s.find(' ');
			while (pos != std::string::npos)
			{
				s.replace(pos,1,"#~#");
				pos = s.find(' ',pos+1);
			}
			(*m_stream) << s << " ";
		}
	}
}

void CharConv::fromBuffer(std::string& s)
{
	if (m_stream ==0)
	{
		int len;
		fromBuffer<int>(len);
		if (len != 0)
		{
			char * data = new char[len];
			fromBuffer(data,len);
			s.assign(data,len);
			delete data;
		}
		else
		{
			s = "";
		}
	}
	else
	{
		(*m_stream) >> s ;
		if (s == "#empty#")
		{
			s="";
		}
		
		int pos = s.find("#~#");
		while (pos != std::string::npos)
		{
			s.replace(pos,3," ");
			pos = s.find("#~#",pos+1);
		}
	}
}

template<>
void CharConv::toBuffer<char>(char c)
{
	if (m_stream ==0)
	{
#ifndef NO_RAKNET
		m_bitstream.Write(c);
#else
		ERRORMSG("Called RakNet lib in NO_RAKNET build");
#endif
	}
	else
	{
		(*m_stream) << (int) c << " ";
	}
}

template<>
void CharConv::fromBuffer<char>(char &c)
{
	if (m_stream ==0)
	{
#ifndef NO_RAKNET
		m_bitstream.Read(c);
#else
		ERRORMSG("Called RakNet lib in NO_RAKNET build");
#endif
	}
	else
	{
		int i;
		(*m_stream) >> i ;
		c = (char) i;
	}
}

