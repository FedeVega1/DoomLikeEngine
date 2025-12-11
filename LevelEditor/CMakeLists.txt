cmake_minimum_required(VERSION 3.20)

project(LevelEditor LANGUAGES CSharp)

set(CMAKE_DOTNET_TARGET_FRAMEWORK "net8.0-windows10.0.26100.0")

set(SOURCES
    AboutBox1.cs
    AboutBox1.Designer.cs
    AboutBox1.resx
    BSPAnalyzer.cs
    BSPAnalyzer.Designer.cs
    BSPAnalyzer.resx
    BSPBuilder.cs
    FileManager.cs
    LevelEditor.cs
    MainWindow.cs
    MainWindow.Designer.cs
    MainWindow.resx
    MapGrid.cs
    MapGridEditor.cs
    PointExtension.cs
    SectorDrawer.cs
    SelectionManager.cs
    ToolStripNumberControl.cs
    Properties/Resources.Designer.cs
    Properties/Resources.resx
    Dependencies/COLoggerImport.cs
)

add_executable(LevelEditor ${SOURCES})

set_property(TARGET LevelEditor PROPERTY VS_DOTNET_TARGET_FRAMEWORK "net8.0-windows10.0.26100.0")

set_target_properties(LevelEditor PROPERTIES
    DOTNET_SDK "Microsoft.NET.Sdk"
    VS_DOTNET_TARGET_FRAMEWORK "net8.0-windows10.0.26100.0"
    VS_GLOBAL_TargetFramework "net8.0-windows10.0.26100.0"
    VS_GLOBAL_UseWindowsForms "true"
    VS_GLOBAL_ImplicitUsings "enable"
    VS_GLOBAL_Nullable "disable"
    VS_GLOBAL_AllowUnsafeBlocks "true"
    VS_GLOBAL_SupportedOSPlatformVersion "10.0.17763.0"
    WIN32_EXECUTABLE TRUE
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/LevelEditor"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/LevelEditor"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

set_source_files_properties(AboutBox1.resx PROPERTIES
    VS_TOOL_OVERRIDE "EmbeddedResource"
    VS_RESOURCE_GENERATOR "ResXFileCodeGenerator"
    VS_RESOURCE_GENERATOR_NAMESPACE "LevelEditor"
    VS_RESOURCE_GENERATOR_FILE "AboutBox1.Designer.cs"
)

set_source_files_properties(BSPAnalyzer.resx PROPERTIES
    VS_TOOL_OVERRIDE "EmbeddedResource"
    VS_RESOURCE_GENERATOR "ResXFileCodeGenerator"
    VS_RESOURCE_GENERATOR_NAMESPACE "LevelEditor"
    VS_RESOURCE_GENERATOR_FILE "BSPAnalyzer.Designer.cs"
)

set_source_files_properties(MainWindow.resx PROPERTIES
    VS_TOOL_OVERRIDE "EmbeddedResource"
    VS_RESOURCE_GENERATOR "ResXFileCodeGenerator"
    VS_RESOURCE_GENERATOR_NAMESPACE "LevelEditor"
    VS_RESOURCE_GENERATOR_FILE "MainWindow.Designer.cs"
)

set_source_files_properties(Properties/Resources.resx PROPERTIES
    VS_TOOL_OVERRIDE "EmbeddedResource"
    VS_RESOURCE_GENERATOR "ResXFileCodeGenerator"
    VS_RESOURCE_GENERATOR_NAMESPACE "LevelEditor.Properties"
    VS_RESOURCE_GENERATOR_FILE "Properties/Resources.Designer.cs"
)

add_dependencies(LevelEditor COLogger_shared)
