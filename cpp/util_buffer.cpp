#include <string.h>

#include <string>

#include "util_buffer.h"



#ifdef __UNIT_TEST__
#include <iostream>

int main(int argc, char * argv[])
{
	lime::buffer  nb;
	std::string   strData = "lakjsfkdjaskljf2084htj02i";
	char          pData[] = "1234567890";


	nb  = strData;

	if (nb == strData)
	{
		std::cout << "buffer class == std::string SUCCESS!!" << std::endl;
	}
	else
	{
		std::cout << "buffer class == std::string FAIL!!" << std::endl;
	}

	nb  = pData;

	if (nb == pData)
	{
		std::cout << "buffer class == char[] SUCCESS!!" << std::endl;
	}
	else
	{
		std::cout << "buffer class == char[] FAIL!!" << std::endl;
	}

	return 0;
}
#endif



using namespace lime;

buffer::buffer(void)
{
	this->_buff      = NULL;
	this->_buffSize  = 0;
	this->_dataSize  = 0;
	this->_malloc    = malloc;
	this->_free      = free;

	if (0 < setSize(4096)) { memset(this->_buff, 0x00, 4096); }
}

buffer::buffer(size_t defaultSize_, func_alloc malloc_, func_free free_)
{
	this->_buff      = NULL;
	this->_buffSize  = 0;
	this->_dataSize  = 0;
	this->_malloc    = malloc_;
	this->_free      = free_;

	if (0 < setSize(defaultSize_)) { memset(this->_buff, 0x00, defaultSize_); }
}

buffer::~buffer()
{
	if (this->_buff) { free(this->_buff); this->_buff  = NULL; }
}

size_t  buffer::setSize(size_t wantSize_, bool doDataCopy_)
{
	void    * b         = NULL; // buffer
	size_t    alignSize = 0;


	if (0 >= wantSize_ || 0x7FFFFFFF <= wantSize_)
	{
		this->_lastError  = "Too big(small or wrong) size memory allocation request.";
		return 0;
	}

	alignSize  = (wantSize_ + 0x07) & ~((size_t)0x07);
	if (NULL == (b = this->_malloc(alignSize)))
	{
		this->_lastError  = "Memory allocation fail";
		return 0;
	}

	if (this->_buff)
	{
		if (doDataCopy_)
		{
			memcpy(b, this->_buff, this->_dataSize);
		}
		free(this->_buff);
	}
	this->_buff      = b;
	this->_buffSize  = alignSize;

	return wantSize_;
}

size_t  buffer::extend(size_t extendSize_)
{
	if (0 >= extendSize_)
	{
		if (this->_buffSize > 512 * 1024)
		{
			extendSize_  = 128 * 1024;
		}
		else
		{
			extendSize_  = ((this->_buffSize >> 1) << 1);
		}
	}

	return setSize(this->_buffSize + extendSize_, true);
}

bool    buffer::copyData(void * data_, size_t dataSize_)
{
	if (NULL == data_)
	{
		this->_lastError  = "set data isn't exist";
		return false;
	}

	if (this->_buffSize < dataSize_)
	{
		if (0 >= setSize(dataSize_)) { return false; }
	}

	memcpy(this->_buff, data_, dataSize_);
	this->_dataSize  = dataSize_;

	return true;
}

// Warning! 
bool    buffer::setData(void * data_, size_t dataSize_)
{
	if (NULL == data_)
	{
		this->_lastError  = "set data isn't exist";
		return false;
	}

	if (this->_buff) { free(this->_buff); }
	this->_buff      = data_;
	this->_dataSize  = dataSize_;
	this->_buffSize  = dataSize_;

	return true;
}

buffer & buffer::operator=(buffer &rhs)
{
	copyData(rhs._buff, rhs._dataSize);
	return *this;
}

buffer & buffer::operator=(std::string &rhs)
{
	copyData((void*)rhs.c_str(), rhs.size());

	return *this;
}

buffer & buffer::operator=(char * rhs)
{
	if (rhs) { copyData((void*)rhs, strlen(rhs)); }

	return *this;
}

bool     buffer::operator==(buffer &rhs)
{
	if ( this->_dataSize == rhs._dataSize &&
			0 == memcmp(this->_buff, rhs._buff, this->_dataSize)
		)
	{
		return true;
	}

	return false;
}

bool     buffer::operator==(std::string &rhs)
{
	if ( this->_dataSize == rhs.size() &&
			0 == memcmp(this->_buff, rhs.c_str(), this->_dataSize)
		)
	{
		return true;
	}

	return false;
}

bool     buffer::operator==(char * rhs)
{
	if ( rhs && this->_dataSize == strlen(rhs) &&
			0 == memcmp(this->_buff, rhs, this->_dataSize)
		)
	{
		return true;
	}

	return false;
}

void  buffer::dataReset(void)
{
	if (this->_buff)
	{
		memset(this->_buff, 0x00, this->_dataSize);
		this->_dataSize  = 0;
	}
}


// EOF: util_buffer.cpp
