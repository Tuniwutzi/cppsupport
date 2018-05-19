#pragma once


#include <vector>
#include <string>


namespace cppsupport
{
	namespace FileSystem
	{
		class Path
		{
		private:
			Path();

		public:
			static std::string Combine(std::string const& pathA, std::string const& pathB);

			static bool IsAbsolute(std::string const& path);
			static std::string ToAbsolute(std::string const& path);
			static std::string GetRoot(std::string const& path); //Returns Partition on Windows (C:, D:, ...) and / on linux
			static std::vector<std::string> GetParts(std::string const& path);

			static std::string GetLastSegment(std::string const& path);
			static std::string GetFileName(std::string const& path, bool includeExtension = true);
			static std::string GetFileExtension(std::string const& path);
		};
	}
}
