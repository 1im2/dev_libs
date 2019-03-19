#ifndef __LIME_UTIL_BUFFER_H__
#define __LIME_UTIL_BUFFER_H__


#include <string>
#include <stdlib.h>


namespace lime {
	class buffer {
		public:
			typedef void *(func_alloc)(size_t);
			typedef void (func_free)(void*);

		public:
			buffer();
			buffer(size_t defaultSize_, func_alloc malloc_ = malloc, func_free free_ = free);
			~buffer();

			void     dataReset(void);
			
			size_t   getDataSize(void) { return this->_dataSize; }
			size_t   getBuffSize(void) { return this->_buffSize; }
			void   * getBuffer(void)   { return this->_buff;     }
			size_t   setSize(size_t wantSize_, bool doDataCopy_ = false);
			size_t   extend(size_t extendSize_ = 0);

			bool     copyData(void * data_, size_t dataSize_);
			bool     setData(void * data_, size_t dataSize_);

			std::string  getLastError(void) { return this->_lastError; }

			buffer & operator=(buffer &rhs);
			buffer & operator=(std::string &rhs);
			buffer & operator=(char * rhs);
			bool     operator==(buffer &rhs);
			bool     operator==(std::string &rhs);
			bool     operator==(char * rhs);

		private:
			void       * _buff;
			size_t       _buffSize;
			size_t       _dataSize;

			func_alloc * _malloc;
			func_free  * _free;

			std::string  _lastError;
	};
}

#endif
