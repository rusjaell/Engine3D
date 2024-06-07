#ifndef EDITOR_H
#define EDITOR_H

#include "Scene.h"
#include "FrameBuffer.h"
#include "Application.h"

#include <iostream>
#include <map>
#include <string>
#include <filesystem>

struct FileEntry 
{
    std::string name;
    std::filesystem::path path;
};

struct DirectoryCacheEntry 
{
    std::string name;
    std::filesystem::path path;
    std::vector<Shared<DirectoryCacheEntry>> entries;
    std::vector<Shared<FileEntry>> files;

    void LoadDirectoryContents(const std::filesystem::path& dir_path) 
    {
        if (!std::filesystem::exists(dir_path) || !std::filesystem::is_directory(dir_path)) {
            std::cerr << "Invalid directory path: " << dir_path << std::endl;
            return;
        }

        for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
            if (entry.is_directory()) {
                Shared<DirectoryCacheEntry> cacheEntry = MakeShared<DirectoryCacheEntry>();
                cacheEntry->name = entry.path().filename().string();
                cacheEntry->path = entry.path();
                cacheEntry->LoadDirectoryContents(cacheEntry->path);
                entries.push_back(cacheEntry);
            }
            else {
                Shared<FileEntry> file = MakeShared<FileEntry>();
                file->name = entry.path().filename().string();
                file->path = entry.path();
                files.push_back(file);
            }
        }
    }

    void PrintEntries(int indentLevel = 0) const 
    {
        std::string indent(indentLevel * 2, ' '); // Indentation for better readability
        for (const Shared<DirectoryCacheEntry>& entry : entries) {
            std::cout << indent << "Directory: " << entry->name << " Path: " << entry->path.string() << std::endl;
            entry->PrintEntries(indentLevel + 1); // Recursively print subdirectories
        }
    }
};

struct DirectoryCache 
{
    Shared<DirectoryCacheEntry> root;

    void InitializeRoot(const std::filesystem::path& rootPath) 
    {
        root = MakeShared<DirectoryCacheEntry>();
        root->name = rootPath.filename().string();
        root->path = rootPath;
        root->LoadDirectoryContents(rootPath);
    }

    void Print() const 
    {
        root->PrintEntries();
    }
};

class Editor : public Application
{
public:
	Editor(const char* title, unsigned int width, unsigned int height);
	~Editor();

	virtual void OnEvent(Event& event) override;
	virtual void OnImGuiRender(double time, double dt) override;
	virtual void OnUpdate(double time, double dt) override;
	virtual void OnRender(double time, double dt) override;

private:
	void RenderScene(double time, double dt);

	// ImGUI
	void RenderViewport();
	void RenderHierarchy();
	void RenderConsole();
	void RenderInspector();
	void RenderDebug();
	
	// Content
	void RenderContent();
	void RenderAssetList();
	void RenderAssetListContent();
	void RenderAssetListTreeView(const Shared <DirectoryCacheEntry>& entry);

	// Content
private:
	DirectoryCache directoryCache_;

private:
	Scene* scene_;

	glm::vec2 viewportSize_;
	Shared<FrameBuffer> viewportFrameBuffer_;

	glm::vec3 backgroundColor_ = { 0.2, 0.2, 0.2 };
};

#endif