#pragma once

#include <FileSystem/FileSystemEntry.hpp>

namespace cppsupport
{
	namespace FileSystem
	{
		class File : public FileSystemEntry
		{
		public:
			struct FileInfo
			{
				uint64_t Size;
			};

		public:
            explicit File();
			explicit File(std::string const& path);

		public:
            bool isNull() const override;
            bool isValid() const override;

            void create(bool createIntermediates = false) override;
			void erase() override;
            void copy(std::string const& to, bool overwrite = false) override;
			void move(std::string const& to, bool overwrite = false) override;

			std::ofstream openWrite(bool binary = false);
			std::ifstream openRead(bool binary = false) const;
			std::fstream open(bool binary = false);

			FileInfo getFileInfo() const;

        public:
            static File Create(std::string const& path, bool createIntermediates = false);
			//static File GetExecutable(); //Scheint nicht sinnvoll auf mehreren Plattformen machbar zu sein
		};
	}
}
