#pragma once

#include <FileSystem/FileSystemEntry.hpp>

#include <vector>

namespace cppsupport
{
	namespace FileSystem
	{
		class File;
		class Directory : public FileSystemEntry
		{
		private:
		    bool null;
	
		public:
		    explicit Directory();
		    explicit Directory(std::string const& path);
	
		public:
		    bool isNull() const override;
		    /*
		    False, wenn isNull() true, oder das Ziel von Pfad eine Datei ist.
		    True, wenn isNull() false und das Ziel von Pfad nicht existiert oder ein Directory ist.
		    => Wenn true, dann duerfen alle anderen Methoden aufgerufen werden.
		    => Wenn false, koennen alle anderen Methoden pauschal exceptions werfen.
		    */
		    bool isValid() const override;
	
		    void create(bool createIntermediates = false) override;
		    void erase() override;
		    void copy(std::string const& to, bool overwrite = false) override;
		    void move(std::string const& to, bool overwrite = false) override;
	
		    void eraseRecursive();
		    void clear();
	
		    std::vector<std::shared_ptr<FileSystemEntry> > enumerateChildren(std::string const& searchPattern = "*", bool recursive = false) const;
		    std::vector<File> enumerateFiles(std::string const& searchPattern = "*", bool recursive = false) const;
		    std::vector<Directory> enumerateDirectories(std::string const& searchPattern = "*", bool recursive = false) const;
	    
		    void getFile(std::string const& relativeFilePath, FileSystem::File& buffer) const;
		    FileSystem::File getFile(std::string const& relativeFilePath) const;
	
		public:
		    static Directory Create(std::string const& path, bool createIntermediates = false);
	
		    static Directory GetWorkingDirectory();
		    static void SetWorkingDirectory(Directory const& dir);
		};
	}
}
