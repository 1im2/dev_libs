#ifndef  __LIME_UTIL_TIMECHECKER_HPP__
#define  __LIME_UTIL_TIMECHECKER_HPP__

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

namespace lime {
	class timeChecker {
		public:
			// TODO: 사용자를 위한 callback 등록 가능하게 하여 시작, 종료시간을 줄수 있도록..
			timeChecker() {
				init("");
			}

			timeChecker(std::string a_name) {
				init(a_name);
			}

			void init(std::string a_name)
			{
				name       = a_name;
				resetTime();
			}

			void resetTime(void)
			{
				bResume    = false;
				crnEnd     = {};
				crnResume  = {};
				crnElapsed = std::chrono::duration<double>(0.0);
				crnBegin   = std::chrono::system_clock::now();
			}

			~timeChecker() {
				PAUSE();
				crnEnd  = std::chrono::system_clock::now();
				std::time_t                    endTime         = std::chrono::system_clock::to_time_t(crnEnd);
				std::chrono::duration<double>  elapsedSeconds  = crnEnd - crnBegin;

				if (0 == crnElapsed.count())
				{
					crnElapsed  = elapsedSeconds;
				}

				std::cout.setf(std::ios::fixed);
				std::cout.precision(9);
				std::cout << "(elapsed time: " << crnElapsed.count() << " sec) " << name << " finished computation at " << std::ctime(&endTime); // std::ctime have std::endl
			}

			void PAUSE(void) {
				if (bResume)
				{
					std::chrono::time_point<std::chrono::system_clock>  now = std::chrono::system_clock::now();
					crnElapsed  += (now - crnResume);
					bResume      = false;
				}
			}

			void RESUME(void) {
				if (false == bResume)
				{
					crnResume = std::chrono::system_clock::now();
					bResume   = true;
				}
			}

		private:
			std::chrono::time_point<std::chrono::system_clock>  crnBegin, crnEnd;
			std::chrono::time_point<std::chrono::system_clock>  crnResume;
			std::chrono::duration<double>                       crnElapsed;
			std::string                                         name;
			bool                                                bResume;
	};
}

#endif
