// Copyright(c) 2019 - 2020, #Momo
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
// 
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and /or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "FileManager.h"
#include "Utilities/Profiler/Profiler.h"
#include "Utilities/Memory/Memory.h"
#include "Utilities/Format/Format.h"
#include "Core/Config/GlobalConfig.h"
#include <portable-file-dialogs.h>
#undef CreateDirectory
#undef ERROR

namespace MxEngine
{
    MxString FileManager::OpenFileDialog(const MxString& types, const MxString& description)
    {
        auto selection = pfd::open_file("Select file", FileManager::GetWorkingDirectory().string(),
            { description.c_str(), types.c_str(), "All Files", "*" }, pfd::opt::multiselect).result();
        return selection.empty() ? "" : ToMxString(selection.front());

    }

    MxString FileManager::SaveFileDialog(const MxString& types, const MxString& description)
    {
        auto selection = pfd::save_file("Save file", FileManager::GetWorkingDirectory().string(),
            { description.c_str(), types.c_str(), "All Files", "*" }).result();
        return ToMxString(selection);
    }

    void FileManager::InitializeRootDirectory(const FilePath& directory)
    {
        if (!File::Exists(directory))
        {
            File::CreateDirectory(directory);
            MXLOG_DEBUG("MxEngine::FileManager", "creating directory: " + ToMxString(directory));
        }

        MxVector<FilePath> ignoredEntries;
        auto& ignoredFolders = GlobalConfig::GetIgnoredFolders();
        for(const auto& folder : ignoredFolders)
        {
             ignoredEntries.push_back(directory / ToFilePath(folder));
        }

        auto InIgnoredDirectories = [&ignoredEntries](const FilePath& path)
        {
            for (const auto& directory : ignoredEntries)
                if (FileManager::IsInDirectory(path, directory))
                    return true;
            return false;
        };

        auto it = std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied);
        for (const auto& entry : it)
        {
            if (entry.is_regular_file())
            {
                auto proximate = FileManager::GetProximatePath(entry, directory);
                
                if(!InIgnoredDirectories(entry.path()))
                    FileManager::AddFile(proximate);
            }
        }
    }

    const FilePath& FileManager::GetFilePath(StringId filename)
    {
        if (!FileManager::FileExists(filename))
        {
            static FilePath empty;
            return empty;
        }
        return manager->filetable[filename];
    }

    FilePath FileManager::GetEngineRootDirectory()
    {
        return FileManager::GetWorkingDirectory() / "Engine";
    }

    FilePath FileManager::GetEngineShaderDirectory()
    {
        return FileManager::GetEngineRootDirectory() / "Shaders";
    }

    FilePath FileManager::GetEngineTextureDirectory()
    {
        return FileManager::GetEngineRootDirectory() / "Textures";
    }

    FilePath FileManager::GetEngineModelDirectory()
    {
        return FileManager::GetEngineRootDirectory() / "Models";
    }

    bool FileManager::FileExists(StringId filename)
    {
        return manager->filetable.find(filename) != manager->filetable.end();
    }

    FilePath FileManager::SearchForExtensionsInDirectory(const FilePath& directory, const MxString& extension)
    {
        namespace fs = std::filesystem;
        FilePath ext = ToFilePath(extension);
        auto it = fs::recursive_directory_iterator(directory);
        for (const auto& entry : it)
        {
            if (entry.path().extension() == ext)
            {
                return entry.path();
            }
        }
        return FilePath();
    }

    FilePath FileManager::SearchFileInDirectory(const FilePath& directory, const MxString& filename)
    {
        return SearchFileInDirectory(directory, ToFilePath(filename));
    }

    FilePath FileManager::SearchFileInDirectory(const FilePath& directory, const FilePath& filename)
    {
        if (!File::Exists(directory)) return FilePath();

        namespace fs = std::filesystem;
        auto it = fs::recursive_directory_iterator(directory);
        for (const auto& entry : it)
        {
            if (entry.is_regular_file() && entry.path().filename() == filename)
            {
                return entry.path();
            }
        }
        return FilePath();
    }

    FilePath FileManager::SearchSubDirectoryInDirectory(const FilePath& directory, const FilePath& filename)
    {
        if (!File::Exists(directory)) return FilePath();

        namespace fs = std::filesystem;
        auto it = fs::recursive_directory_iterator(directory);
        for (const auto& entry : it)
        {
            if (entry.is_directory() && entry.path().filename() == filename)
            {
                return entry.path();
            }
        }
        return FilePath();
    }

    FilePath FileManager::SearchSubDirectoryInDirectory(const FilePath& directory, const MxString& filename)
    {
        return FileManager::SearchSubDirectoryInDirectory(directory, ToFilePath(filename));
    }

    FilePath FileManager::GetRelativePath(const FilePath& path, const FilePath& directory)
    {
        return std::filesystem::relative(path, directory);
    }

    FilePath FileManager::GetProximatePath(const FilePath& path, const FilePath& directory)
    {
        return std::filesystem::proximate(path, directory);
    }

    StringId FileManager::RegisterExternalResource(const FilePath& path)
    {
        auto workingDirectory = FileManager::GetWorkingDirectory();
        auto absolutePath = std::filesystem::absolute(path);
        FilePath localPath;

        if (!FileManager::IsInDirectory(absolutePath, workingDirectory))
        {
            auto parent = absolutePath.parent_path();
            auto destination = FileManager::GetProximatePath(absolutePath, parent);
            localPath = workingDirectory / destination;

            MXLOG_INFO("MxEngine::FileManager", "path " + ToMxString(absolutePath) + " is not in project directory, copying to working directory");

            if(File::IsDirectory(localPath))
                File::CreateDirectory(localPath);

            FileManager::Copy(absolutePath, localPath);
            localPath = FileManager::GetProximatePath(localPath, workingDirectory);
        }
        else
        {
            localPath = FileManager::GetProximatePath(absolutePath, workingDirectory);
        }

        if (!File::Exists(localPath))
        {
            File _(localPath, File::WRITE);
        }

        if (File::IsFile(localPath))
        {
            return FileManager::AddFile(localPath);
        }
        else
        {
            auto it = std::filesystem::recursive_directory_iterator(localPath);
            for (const auto& entry : it)
            {
                if (entry.is_regular_file())
                {
                    FileManager::AddFile(entry.path());
                }
            }
            return 0;
        }
    }

    bool FileManager::IsInDirectory(const FilePath& path, const FilePath& directory)
    {
        auto current = path;
        auto root = current.root_path();
        while (!current.empty() && current != root)
        {
            if (current == directory) return true;
            current = current.parent_path();
        }
        return false;
    }

    void FileManager::Copy(const FilePath& from, const FilePath& to)
    {
        std::error_code ec;
        std::filesystem::copy(from, to, std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive, ec);
        if (ec)
        {
            MXLOG_ERROR("MxEngine::FileManager", MxFormat("cannot copy {0} to {1}", ToMxString(from), ToMxString(to)));
        }
    }

    StringId FileManager::AddFile(const FilePath& file)
    {
        auto filenameString = file.lexically_normal().string();
        std::replace(filenameString.begin(), filenameString.end(), '\\', '/');
        FilePath filename = filenameString;

        auto filehash = MakeStringId(filenameString);
        auto collision = manager->filetable.find(filehash);
        if (collision != manager->filetable.end() && collision->second != filename)
        {
            MXLOG_WARNING("MxEngine::FileManager", MxFormat("hash of file \"{0}\" conflicts with other one in the project: {1}", filenameString, manager->filetable[filehash].string()));
        }
        else if (collision == manager->filetable.end())
        {
            MXLOG_DEBUG("MxEngine::FileManager", MxFormat("file added to the project: {0}", filenameString));
            manager->filetable.emplace(filehash, filename);
        }
        return filehash;
    }

    void FileManager::Init()
    {
        manager = Alloc<FileManagerImpl>();
        auto workingDirectory = FileManager::GetWorkingDirectory();
        MXLOG_INFO("MxEngine::FileManager", "project working directory is set to: " + ToMxString(workingDirectory));

        if (!File::Exists(FileManager::GetEngineRootDirectory()))
        {
            File::CreateDirectory(FileManager::GetEngineRootDirectory());
            MXLOG_ERROR("MxEngine::FileManager", "cannot find root directory. Executable is probably launched from wrong folder");
        }

        if (!File::Exists(FileManager::GetEngineTextureDirectory()))
            File::CreateDirectory(FileManager::GetEngineTextureDirectory());

        if (!File::Exists(FileManager::GetEngineShaderDirectory()))
            File::CreateDirectory(FileManager::GetEngineShaderDirectory());

        if (!File::Exists(FileManager::GetEngineModelDirectory()))
            File::CreateDirectory(FileManager::GetEngineModelDirectory());
    }

    void FileManager::Clone(FileManagerImpl* other)
    {
        manager = other;
    }

    FileManagerImpl* FileManager::GetImpl()
    {
        return manager;
    }

    FilePath FileManager::GetWorkingDirectory()
    {
        return std::filesystem::current_path();
    }
}