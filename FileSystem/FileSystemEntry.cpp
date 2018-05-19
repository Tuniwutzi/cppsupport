#include <FileSystem/FileSystemEntry.hpp>

#include <FileSystem/Path.hpp>
#include <FileSystem/Directory.hpp>

#ifndef WINDOWS
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#endif


namespace cppsupport
{
	namespace FileSystem
	{
#ifdef _WINDOWS
#else
	bool getStats(char const* path, struct stat* fi)
    {
		static struct stat fiBuf;
		if (!fi)
			fi = &fiBuf;
    	int rv = lstat(path, fi);
    	if (rv == 0)
    		return true;
    	else if (errno == ENOENT)
    		return false;
    	else
    		throw std::runtime_error("Error getting file info");
    }
#endif

		FileSystemEntry::FileSystemEntry(Type type, std::string const& path)
			:type(type), path(path)
		{
		}

        bool FileSystemEntry::operator==(FileSystemEntry const& other) const
        {
            if (this->type != other.type)
                return false;
            else if (this->isNull() && other.isNull())
                return true;

            std::string a(this->getFullPath());
            std::string b(other.getFullPath());
            return this->getFullPath() == other.getFullPath();
        }
        bool FileSystemEntry::operator!=(FileSystemEntry const& other) const
        {
            return !(*this == other);
        }

        void FileSystemEntry::assertValid() const
        {
            if (!this->isValid())
                throw std::runtime_error("FileSystemEntry is invalid");
        }

        FileSystemEntry::Type FileSystemEntry::getType() const
        {
            return this->type;
        }

        std::string FileSystemEntry::getFullPath() const
        {
            return Path::ToAbsolute(this->getPath());
        }
		std::string const& FileSystemEntry::getPath() const
		{
			return this->path;
		}
        Directory FileSystemEntry::getParent() const
        {
            this->assertValid();

            std::string fullPath = this->getFullPath();

#ifdef _WINDOWS
            //TODO: Verbessern; Mehrere Slashes/Backslashes, insbesodere vermischt, am Ende des Pfads machen alles putt
            String::size_type backPos = fullPath.find_last_of('\\');
            String::size_type slashPos = fullPath.find_last_of('/');
            if (backPos == String::npos || slashPos == fullPath.size() - 1)
                backPos = -1;
            if (slashPos == String::npos || slashPos == fullPath.size() - 1)
                slashPos = -1;

            String::size_type pos = max(backPos, slashPos);
            if (pos == -1)
                return Directory();
            else
                return Directory(fullPath.substr(0, pos));
#else
            std::string::size_type slashPos;
            while ((slashPos = fullPath.find_last_of('/')) == fullPath.size() - 1)
            	fullPath = fullPath.substr(0, slashPos);

            if (slashPos == std::string::npos)
            	return Directory();
            else
            	return Directory(fullPath.substr(0, slashPos));
#endif
        }

        bool FileSystemEntry::exists() const
        {
            this->assertValid();

            return FileSystemEntry::Exists(this->getPath());
        }

        bool FileSystemEntry::Exists(std::string const& path)
        {
#ifdef _WINDOWS
            DWORD attribs = GetFileAttributesA(path.data());
            return attribs != INVALID_FILE_ATTRIBUTES;
#else
            return getStats(path.data(), NULL);
#endif
        }
	}
}
