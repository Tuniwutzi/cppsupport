#include <FileSystem/File.hpp>


#include <cstdio>
#include <fstream>

#include <sys/stat.h>

#include <FileSystem/Directory.hpp>


using namespace std;

namespace cppsupport
{
	namespace FileSystem
	{
#ifdef _WINDOWS
#else
		bool getStats(char const*, struct stat*);
#endif

		File::File()
			:FileSystemEntry(FileSystemEntry::FileEntry, "")
		{
		}
		File::File(std::string const& path)
			:FileSystemEntry(FileSystemEntry::FileEntry, path)
		{
		}

		bool File::isNull() const
		{
			return this->getPath().size() <= 0;
		}
		bool File::isValid() const
		{
			//TODO: Cachen oder so? Wird haeufig abgefragt
#ifdef _WINDOWS
			if (!this->isNull())
			{
				DWORD attr = GetFileAttributesA(this->getPath().data());
				return (attr == INVALID_FILE_ATTRIBUTES) || !(attr & FILE_ATTRIBUTE_DIRECTORY);
			}
			else
				return false;
#else
			if (!this->isNull())
			{
				struct stat fi;

				if (getStats(this->getPath().data(), &fi))
					return S_ISREG(fi.st_mode);
				else
					return true;
			}
			else
				return false;
#endif
		}

		void File::create(bool createIntermediates)
		{
			this->assertValid();

			if (createIntermediates)
			{
				Directory parent = this->getParent();
				if (!parent.exists())
					parent.create(true);
			}

#ifdef _WINDOWS
			HANDLE hfile = CreateFileA(this->getPath().data(), 0, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hfile == INVALID_HANDLE_VALUE)
				throw OSApiException("Could not create file", GetLastError());
			else
				CloseHandle(hfile);
#else
			fstream fs(this->getPath().data());
			fs.flush();
			fs.close();
#endif
		}
		void File::erase()
		{
			this->assertValid();

			if (remove(this->getPath().data()))
				throw std::runtime_error("Error removing file");
		}
		void File::move(std::string const& to, bool overwrite)
		{
			this->assertValid();

#ifdef _WINDOWS
			if (!MoveFileExA(this->getPath().data(), to.data(), MOVEFILE_COPY_ALLOWED | (overwrite ? MOVEFILE_REPLACE_EXISTING : 0)))
				throw OSApiException("Error moving file", GetLastError());
#else
			bool targetExists = File::Exists(to);

			if (targetExists)
			{
				if (overwrite)
					File(to).erase();
				else
					throw std::runtime_error("Target file exists");
			}

			int rv = std::rename(this->getPath().data(), to.data());
			if (rv != 0)
				throw std::runtime_error("Error moving file");
#endif
		}
		void File::copy(std::string const& to, bool overwrite)
		{
			this->assertValid();

			/*
			#ifdef _WINDOWS
			if (!CopyFileA(this->getPath().data(), to.data(), (overwrite ? FALSE : TRUE)))
			throw OSApiException("Could not copy file", GetLastError());
			#else
			*/
			File target(to);
			if (target.exists())
			{
				if (overwrite)
					target.erase();
				else
					throw std::runtime_error("Target file exists");
			}

			std::ifstream  src(this->getPath().data(), std::ios::binary);
			std::ofstream  dst(to.data(), std::ios::binary);

			dst << src.rdbuf();
			//#endif
		}

        ifstream File::openRead(bool binary) const
        {
            this->assertValid();

            if (this->exists())
                return ifstream(this->getPath().data(), istream::in | (binary ? istream::binary : (ios::openmode)0));
            else
                throw std::runtime_error("File does not exist");
        }

		//TODO: Unit test
		File::FileInfo File::getFileInfo() const
		{
			this->assertValid();

			struct stat stats;
			if (stat(this->getPath().data(), &stats))
				throw std::runtime_error("Could not get file info");

			FileInfo rv;
			rv.Size = stats.st_size;
			return rv;
		}

		File File::Create(std::string const& path, bool createIntermediates)
		{
			File f(path);
			f.create(createIntermediates);
			return f;
		}
		//File File::GetExecutable()
		//{
		//	char buf[MAX_PATH + 1];
		//	if (GetModuleFileNameA(NULL, buf, sizeof(buf)) == 0)
		//		throw OSApiException("GetModuleFileName failed", GetLastError());

		//	return File(buf);
		//}
	}
}
