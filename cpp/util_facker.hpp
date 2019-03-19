#ifndef  __LIME_UTIL_FAKER_HPP__
#define  __LIME_UTIL_FAKER_HPP__

#include <iostream>
#include <random>
#include <map>
#include <vector>

namespace lime {
	class someGenerate {
		public:
			static const std::string basestr;
			static void generateString(std::string & strGen_, long wantLength_)
			{
				std::random_device rd;
				std::uniform_int_distribution<long> dist(0, someGenerate::basestr.size()-1);


				strGen_.clear();

				for (long i = 0 ; i < wantLength_; i++ )
				{
					strGen_  += basestr.at(dist(rd));
				}
			}

			static std::string generateString(long wantLength_)
			{
				std::string  strGen;

				generateString(strGen, wantLength_);

				return strGen;
			}

			static bool generateRandomMap(
					std::map<std::string, std::string> &rltMap_,
					long genCount_,
					long genStrLength_,
					long keyLength = 0,
					long keyBeginIndex = 0,
					std::vector<std::string> * vecKeyPrefix = NULL
					)
			{
				std::string                          key;
				std::string                          str_index;
				long                                 key_id = 0;
				long                                 len = 0;
				static unsigned long                 inc = 0;
				std::random_device                   rd;
				std::uniform_int_distribution<long>  dist(0, (vecKeyPrefix == NULL ? 1 : vecKeyPrefix->size()));


				if (keyLength)
				{
					for (long l = 0; l < genCount_; l++)
					{
						if (vecKeyPrefix && 0 < vecKeyPrefix->size())
						{
							key_id   = (inc++);
							key_id  += dist(rd);
							key_id  %= (*vecKeyPrefix).size();
							key      = vecKeyPrefix->at(key_id);
						}
						else
						{
							key.clear();
						}

						str_index   = std::to_string(l+keyBeginIndex);
						len         = keyLength - str_index.size();
						if (len > (long)key.size())
						{
							for (long i = len - key.size(); i > 0; i--)
							{
								key  += '0';
							}
						}
						key  += str_index;

						rltMap_[key]  = someGenerate::generateString(genStrLength_);
						// std::cout << key << "] " << std::endl;
					}
				}
				else
				{
					for (long l = 0; l < genCount_; l++)
					{
						if (vecKeyPrefix)
						{
							key_id   = (inc++);
							key_id  += dist(rd);
							key_id  %= (*vecKeyPrefix).size();
							key      = vecKeyPrefix->at(key_id);
						}
						else
						{
							key.clear();
						}

						key  += std::to_string(l+keyBeginIndex);

						rltMap_[key]  = someGenerate::generateString(genStrLength_);
						// std::cout << key << "] " << std::endl;
					}
				}

				return true;
			}
	};
}

#endif
