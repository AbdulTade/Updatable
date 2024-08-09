# pragma once

# include <iostream>
# include <fstream>
# include <filesystem>
# include <string_view>
# include "Expected.hpp"

enum class FileStatus {
	AlreadyExists, FileNotFound,
	UnknownFileError, NotADirectory,
	Success
};

struct Filesystem {

	static Expected<FileStatus, std::string> readFile(const std::filesystem::path& filepath) {
		if (!std::filesystem::exists(filepath))
			return { FileStatus::FileNotFound };
		std::string output;
		std::fstream file(filepath, std::ios::binary | std::ios::in);

		if (file.fail())
			return { FileStatus::UnknownFileError };

		file >> output;
		file.close();
		return { output };
	}

	static FileStatus writeFile(const std::filesystem::path& filepath, const char* data, size_t size) {
		std::fstream file(filepath, std::ios::binary | std::ios::out);

		if (file.fail())
			return FileStatus::UnknownFileError;

		file << std::string_view(data, size);
		file.close();
		return FileStatus::Success;
	}

	static FileStatus makeDirectory(const std::filesystem::path& filepath) {
		if (std::filesystem::exists(filepath))
			return FileStatus::AlreadyExists;
		else if (!std::filesystem::is_directory(filepath))
			return FileStatus::NotADirectory;
		else if (!std::filesystem::create_directory(filepath))
			return FileStatus::UnknownFileError;
		return FileStatus::Success;
	}

	using DirList = std::vector<std::filesystem::directory_entry>;

	static Expected<FileStatus, DirList> listDirectory(const std::filesystem::path& filepath) {
		if (!std::filesystem::exists(filepath))
			return { FileStatus::FileNotFound };
		else if (!std::filesystem::is_directory(filepath))
			return { FileStatus::NotADirectory };
		std::filesystem::directory_iterator dirIt(filepath);
		std::vector<std::filesystem::directory_entry> list;
		for (auto&& entry : dirIt) {
			list.emplace_back(entry);
		}
		return { list };
	}
};
