#ifndef  __LIME_UTIL_MISC_HPP__
#define  __LIME_UTIL_MISC_HPP__

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <string>

namespace lime {
	class keyvaluedbutil {
		public:
			typedef struct kvdb_test_args_structure
			{
				bool         enable_insert;
				bool         enable_select;
				long         random_count;
				long         key_size;
				long         value_size;
				std::string  prefix;

				kvdb_test_args_structure() { enable_insert = false; enable_select = false; random_count = 1000; key_size = 128; value_size = 256 * 10 + 128; }
			} KVDB_ARGS;

		public:
			static bool  createChildDirectories(const char *a_parent_directory, int a_parent_directory_len, int a_child_directory_count)
			{
				char  child_dir[2048] = { 0, };
				bool  ret             = true;
				int   i               = 0;


				memset(child_dir, 0x00, 2048);
				strncpy(child_dir, a_parent_directory, a_parent_directory_len);
				for (i = 0; i < a_child_directory_count; i++)
				{
					snprintf(child_dir + a_parent_directory_len, 2047, "/%08d", i);
					if (access(child_dir, F_OK))
					{
						if (mkdir(child_dir, 0777)) // all user can access
						{
							std::cout << "Child directory create fail.(" << strerror(errno) << ", " << child_dir << ")" << std::endl;
							ret  = false;
							break;
						}
					}
				}

				if (false == ret)
				{
					for (i--; i >= 0; i--)
					{
						snprintf(child_dir + a_parent_directory_len, 2047, "/%08d", i);
						if (!access(child_dir, F_OK))
						{
							if (rmdir(child_dir))
							{
								std::cout << "Child directory create fail.(" << strerror(errno) << ", " << child_dir << ")" << std::endl;
							}
						}
					}
				}

				return ret;
			}

			static bool createTmpDirectoy(const char *a_parent_directory, int a_child_directory_count)
			{
				int  len = 0;


				if (NULL == a_parent_directory)
				{
					std::cout <<  "Parent directory is null." << std::endl;
					return false;
				}

				if (0 > a_child_directory_count)
				{
					a_child_directory_count  = 0;
				}

				if (-1 == (len = strlen(a_parent_directory)) || len > 2000)
				{
					std::cout <<"Wrong Parent directory name. (" << a_parent_directory << ")" << std::endl;
					return false;
				}

				if (access(a_parent_directory, F_OK))
				{
					if (mkdir(a_parent_directory, 0777)) // all user can access
					{
						std::cout <<"Parent directory create fail.(" << strerror(errno) << ", " << a_parent_directory << ")" << std::endl;
						return false;
					}
				}


				return createChildDirectories(a_parent_directory, len, a_child_directory_count);
			}

			static void parse_lvdb_param(int argc, char * argv[], KVDB_ARGS & a_lvdb_args)
			{
				int     opt = -1;


				while (-1 != (opt = getopt(argc, argv, "iIsSr:R:k:K:v:V:p:P:")))
				{
					switch (opt)
					{
						case 'i': case 'I': {  a_lvdb_args.enable_insert = true;         } break;
						case 's': case 'S': {  a_lvdb_args.enable_select = true;         } break;
						case 'r': case 'R': {  a_lvdb_args.random_count  = std::stol(optarg); } break;
						case 'k': case 'K': {  a_lvdb_args.key_size      = std::stol(optarg); } break;
						case 'v': case 'V': {  a_lvdb_args.value_size    = std::stol(optarg); } break;
						case 'p': case 'P': {  a_lvdb_args.prefix        = optarg;       } break;
						default: break;
					}
				}

				if (0 >= a_lvdb_args.random_count || 1024*1024*1024 < a_lvdb_args.random_count)
				{
					a_lvdb_args.random_count  = 1000;
				}

				if (0 >= a_lvdb_args.key_size || 1024 * 1024 < a_lvdb_args.key_size)
				{
					a_lvdb_args.key_size  = 128;
				}

				if (0 >= a_lvdb_args.value_size || 1024 * 1024 * 128 < a_lvdb_args.value_size)
				{
					a_lvdb_args.value_size  = 128 + 10 * 256;
				}
			}

			static void usage(const char * prog)
			{
				std::cout << "USAGE: " << prog << " [isr:k:v:p:]"<< std::endl;
				std::cout << "    i        : insert enable" << std::endl;
				std::cout << "    s        : select enable" << std::endl;
				std::cout << "    r COUNT  : data size" << std::endl;
				std::cout << "    k SIZE   : key size(bytes, default 128)" << std::endl;
				std::cout << "    v SIZE   : value size(bytes, default 128 + 10 * 256)" << std::endl;
				std::cout << "    p STRING : key prefix string" << std::endl;
			}
	};
}

#endif
