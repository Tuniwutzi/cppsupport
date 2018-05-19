#pragma once

#include <string>

namespace cppsupport
{
	namespace FileSystem
	{
		class Directory;
		class FileSystemEntry
		{
		public:
			enum Type
			{
				FileEntry,
				DirectoryEntry
			};

		private:
			Type type;
			std::string path;

		protected:
			explicit FileSystemEntry(Type type, std::string const& path = "");

		public:
			virtual ~FileSystemEntry() {}

        public:
            bool operator==(FileSystemEntry const& orig) const;
            bool operator!=(FileSystemEntry const& orig) const;

        protected:
            void assertValid() const;

		public:
            Type getType() const;

            bool exists() const;

            std::string getFullPath() const;
			std::string const& getPath() const; //Gibt den Pfad zurueck, mit dem das Objekt erstellt wurde
			Directory getParent() const;

            virtual bool isNull() const = 0;
            virtual bool isValid() const = 0;

            virtual void create(bool createIntermediates = false) = 0;
			virtual void erase() = 0;
            virtual void copy(std::string const& to, bool overwrite = false) = 0;
			virtual void move(std::string const& to, bool overwrite = false) = 0;

        public:
            static bool Exists(std::string const& path);
		};
	}
}
