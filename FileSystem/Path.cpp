#include <FileSystem/Path.hpp>

#ifdef _WINDOWS
#include <Shlwapi.h>
#else
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#endif


using namespace std;


namespace cppsupport
{
	namespace FileSystem
	{
		std::string Path::Combine(std::string const& pathA, std::string const& pathB)
		{
			std::string rv(pathA);

#ifdef _WINDOWS
            if (pathA.find_last_of('/') != pathA.size() -1 && pathA.find_last_of('\\') != pathA.size() - 1)
			{
				if (pathB.find('/') != 0 && pathB.find('\\') != 0)
				{
                    if (pathA.find('/') != std::string::npos)
						rv += "/";
					else
						rv += "\\";
				}
			}

			rv += pathB;
#else
			if (pathA.find_last_of('/') != pathA.size() - 1)
			{
				if (pathB.find('/') != 0)
					rv += '/';
			}
			else if (pathB.find('/') == 0)
				rv = rv.substr(0, rv.size() - 1);

			rv += pathB;
#endif

			return rv;
		}


        bool Path::IsAbsolute(std::string const& path)
        {
#ifdef _WINDOWS
            //if (path.size() >= 2)
            //{
            //    char second = path.at(1);
            //    return second == ':';
            //}
            //return false;
            return PathIsRelativeA(path.data()) == FALSE;
#else
            return path.find('/') == 0;
#endif
        }
        std::string Path::ToAbsolute(std::string const& path)
        {
#ifdef _WINDOWS
            char buffer[512];

            if (path.size() == 0)
            {
                DWORD chars = GetCurrentDirectoryA(sizeof(buffer), buffer);
                if (!chars)
                    throw std::runtime_error("Could not determine full path, could not get working directory");
            }
            else
            {
                if (!GetFullPathNameA(path.data(), sizeof(buffer), buffer, NULL)) //TODO: Wenn der Buffer zu klein ist bekomme ich die mindestgr��e des Buffers zur�ck, die gebraucht wird
                    throw std::runtime_error("Could not determine full path");
            }

            return std::string(buffer);

#else
            char buffer[PATH_MAX + 1];
            if (path.size() == 0)
            {
            	if (!getcwd(buffer, sizeof(buffer)))
            		throw std::runtime_error("Could not determine current working directory");
            	else
            		return std::string(buffer);
            }
            else if (!Path::IsAbsolute(path))
            {
				if (!realpath(path.data(), buffer))
				{
					//TODO: File/Dir does not exist -> must build smallest possible absolute path manually
					//Can not just concatentate working directory and path, because FullPath is used for checking equality of File-instances.
					throw std::runtime_error("Error determining full path");
				}
				else
					return std::string(buffer);
            }
            else
            	return path;
            //TODO: realpath (Aber: REQUIRES THE PATH TO EXIST; nicht wirklich dolle)
#endif
//            if (Path::IsAbsolute(this->getPath()))
//                return this->getPath();
//            else
//            {
//                std::string workingDirectory;
//#ifdef _WINDOWS
//                char buffer[MAX_PATH];
//                DWORD chars = GetCurrentDirectoryA(sizeof(buffer), buffer);
//                if (!chars)
//                    throw OSApiException("Could not get working directory", GetLastError());
//                else
//                    workingDirectory = buffer;
//#else
//                implement
//#endif
//
//                if (this->getPath().size() == 0) //Passiert nur bei Directories
//                    return workingDirectory;
//#ifdef _WINDOWS
//                else if (this->getPath().at(0) == '/' || this->getPath().at(0) == '\\')
//                {
//                    if (chars < 2)
//                        throw Exception("Unexpected exception in FileSystemEntry::getFullPath");
//                    return Path::Combine(workingDirectory.substr(0, 2), this->getPath());
//                }
//#else
//                    implement
//#endif
//                else
//                    return Path::Combine(workingDirectory, this->getPath());
//            }
        }
        std::string Path::GetRoot(std::string const& path)
        {
#ifdef _WINDOWS
            std::string abs = Path::ToAbsolute(path);
            vector<std::string> parts = Path::GetParts(path);
            return parts.at(0);
#else
            return "/";
#endif
        }

        vector<std::string> Path::GetParts(std::string const& path)
        {
            vector<std::string> rv;
            std::string::size_type len = 0;

            for (std::string::size_type i = 0; i < path.size(); i++)
            {
                char c = path.at(i);
#ifdef _WINDOWS
                if (c == '/' || c == '\\')
#else
                if (c == '/')
#endif
                {
                    rv.push_back(path.substr(len, i - len));
                    len = i + 1;
                }
            }

            if (len < path.size())
                rv.push_back(path.substr(len));

            return rv;
        }
        std::string Path::GetLastSegment(std::string const& path)
		{
#ifdef _WINDOWS
            std::string::size_type li = path.find_last_of('\\');
			if (li == std::string::npos)
			{
                li = path.find_last_of('/');
				if (li == std::string::npos)
					return path;
			}

			return path.substr(li + 1);
#else
			std::string::size_type li = path.find_last_of('/');
			if (li == std::string::npos)
				return path;
			else
				return path.substr(li + 1);
#endif
		}
        std::string Path::GetFileName(std::string const& path, bool includeExtension)
        {
            std::string fn = Path::GetLastSegment(path);
            if (!includeExtension)
            {
                std::string::size_type li = fn.find_last_of('.');
                if (li != std::string::npos)
                    fn = fn.substr(0, li);
            }
            return fn;
        }
		std::string Path::GetFileExtension(std::string const& path)
		{
			std::string fn = Path::GetLastSegment(path);
			std::string::size_type li = fn.find_last_of('.');
			if (li != std::string::npos)
				return fn.substr(li);
			else
				throw std::runtime_error("File has no extension");
		}
        
        std::string Path::ToRelative(std::string const& path, std::string const& baseDirectory) {
            auto fullPath = Path::ToAbsolute(path);
            auto fullBase = Path::ToAbsolute(baseDirectory);
            
            if (fullPath.find(fullBase) != 0) {
                throw std::runtime_error("The base directory (" + baseDirectory + ") is not a parent of the entry pointed to by path (" + path + ")");
            }
            
            auto rv = fullPath.substr(fullBase.size());
#ifndef _WINDOWS
            if (rv.at(0) == '/') {
                rv = rv.substr(1);
                if (rv.empty()) {
                    rv = ".";
                }
            }
#endif
            return rv;
        }
	}
}
