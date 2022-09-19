#include "geometria.h"
#include "Editor/Editor.h"
#include "Application/Application.h"
#include "Physics/PhysicsManager.h"
#include "Physics/Rigidbody/Rigidbody.h"
#include "Physics/Colliders/BoxCollider.h"
#include "Game/Scripts/HubManager.h"
#include "Game/Scripts/DynamicLinker.h"
#include "Game/Scripts/EngineUpdater.h"
#include "Game/Scripts/ToolkitUpdater.h"
#include "Game/Scripts/FirstTimeInstaller.h"
#include "Game/Scripts/Debugger/Debugger.h"

DONT_UPDATE_FILE()

//Original name: Chingatumadre Engine

static std::vector<Model*> models;
bool firstFrame = true, secondFrame = true;
bool HubManager::creating = false, HubManager::isGitThreadSpawned = false, HubManager::isGitFinished = false, HubManager::isProjectCreationFinished = false, HubManager::isFromCommandLine = false;
std::string HubManager::currentUrl, HubManager::currentProjectName;

bool HubManager::defaultValuesAdded = false;
ImGUIElement* HubManager::win;
ImGUIElement* HubManager::status;

std::string FirstTimeInstaller::input, FirstTimeInstaller::pkgMgr, FirstTimeInstaller::linuxDistro;
bool FirstTimeInstaller::makeFirstTimeFile;
bool FirstTimeInstaller::downloadGitInstaller;

void Run_Debugger()
{
    Debugger::RunLLDB();
}

void Main_Compile(std::string variable)
{
    bool unknownValue = false;

    if(variable == "debug")
        std::cout << "Compiling [Debug]..." << std::endl;
    else if(variable == "" || variable == "release")
        std::cout << "Compiling [Release]..." << std::endl;
    else
    {
        std::cout << "ERROR: Unknown second parameter." << std::endl;
        unknownValue = true;
    }

    if(!unknownValue)
    {
        if(variable == "debug")
            Debugger::ReadCPPandHFiles(false);
    
        if (Application::IsPlatform(Application::Windows))
        {
            //TODO: Add dynamic linker.
    
            //DynamicLinker::Link();
    
            std::string getMSBuild = Files::GetPathFromCommand(Files::ConvertToWindowsCmdPath("C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe") + " -latest -prerelease -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -find MSBuild/**/Bin/MSBuild.exe");
            std::string visualStudioVersion = Files::GetValueFromCommand(Files::ConvertToWindowsCmdPath("C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe") + " -latest -prerelease -products * -property productLineVersion");
            std::string premakeCmd = "premake5-windows.exe vs" + visualStudioVersion;
    
            if (getMSBuild != "")
            {
                system(premakeCmd.c_str());
                system(Files::ConvertToWindowsCmdPath(getMSBuild).c_str());
                std::cout << "Project Compiled!" << std::endl;
            }
        }
        else if(Application::IsPlatform(Application::Linux))
        {
            system("chmod -R 777 premake5-linux");
            system("./premake5-linux gmake2");
            system("make");
            std::cout << "Project Compiled!" << std::endl;
        }
    
        if(variable == "debug")
            Debugger::Cleanup();
    }
}

int main(int argc, char** argv)
{
    if(argc != 0)
    {
        if(std::experimental::filesystem::exists("geo.exe.old"))
            Files::Remove("geo.exe.old");
    }

    for (int i = 0; i < argc; i++)
    {
        std::string commandLine = argv[i];
        if (commandLine == "--bypass-intel")
            Graphics::BypassIntel(true);
        else if (commandLine == "--compile")
        {
            std::string second;
            if(i + 1 < argc)
                second = argv[i + 1];

            Main_Compile(second);
            exit(0);
        }
        else if(commandLine == "--path-compiler")
        {
            if (Application::IsPlatform(Application::Windows))
                std::cout << Files::GetPathFromCommand(Files::ConvertToWindowsCmdPath("C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe") + " -latest -prerelease -products * -requires Microsoft.Component.MSBuild -find MSBuild/**/Bin/MSBuild.exe") << std::endl;

            exit(0);
        }
        else if(commandLine == "--run")
        {
            std::cout << "Running..." << std::endl;
            Run_Debugger();
            exit(0);
        }
        else if(commandLine == "--update-engine")
        {
            std::string message;
            std::cout << "WARNING: Updating to the latest version can cause engine modifications you made be overwritten\n";
            std::cout << "(make sure you have a backup if you're going to do this).\n";
            std::cout << "If you're sure you want to do this. Type \"Yes, i perfectly know what i'm doing\"\n";
            std::cout << "Otherwise, type anything but that command to cancel.\n";
            std::getline(std::cin, message);

            if(message == "Yes, i perfectly know what i'm doing")
            {
                EngineUpdater::Update();
                HubManager::ChangeCurrentURL(std::experimental::filesystem::current_path().u8string());
                HubManager::ChangeCurrentProjectName(std::experimental::filesystem::current_path().filename().u8string());
                HubManager::SetUpProject();
            }
            else
                std::cout << "Canceling Geometria Engine Update...\n";

            exit(0);
        }
        else if(commandLine == "--update-toolkit")
        {
            ToolkitUpdater::DownloadLatestToolkit();
            exit(0);
        }
        else if(commandLine == "--create")
        {
            HubManager::isFromCommandLine = true;

            if(i + 1 < argc)
            {
                std::string projectName, url;
                projectName = argv[i + 1];

                if(i + 2 < argc)
                    url = argv[i + 2];
                else
                {
                    url = std::experimental::filesystem::current_path().u8string();
                    std::cout << "No path added, making project in current directory..." << std::endl;
                }

                GDT_WATCHPOINT_INIT();

                GDT_WATCHPOINT_START();
                HubManager::CreateProject(projectName, url);
                GDT_WATCHPOINT_END();
                std::cout << "Project Folder Created! Downloading from GitHub repository..." << std::endl;
                HubManager::GitClone();
                std::cout << "Finished Downloading!" << std::endl;
                std::cout << "Setting Up Project..." << std::endl;
                HubManager::SetUpProject();
            }
            else
            {
                std::cout << "Not enough arguments...\n";
            }

            exit(0);
        }
        else if(commandLine == "--get-exec-path")
        {
            std::cout << Files::GetExecutablePath() << std::endl;
            exit(0);
        }
        else if(commandLine == "--version")
        {
            std::cout << "0.2.5" << std::endl;
        }
        else if(commandLine == "--macro-test")
        {
            //ToolkitUpdater::InstallNewToolkit("hello :D");
            exit(0);
        }
    }

    std::string gamefolder = std::experimental::filesystem::current_path().u8string() + "/Game";
    if(!Files::DirectoryExists(gamefolder.c_str()))
        exit(0);

    if(!std::experimental::filesystem::exists("firsttime"))
    {
        FirstTimeInstaller::Start();
        exit(0);
    }

    if (Graphics::IsIntelGPUBypassed())
    {
        std::cout << "Intel GPU Bypassed!" << std::endl;
    }

    Graphics::Init();
    Graphics::CreateWindow(640, 480, "Yooooooooooooooo");

    Graphics::Start();

    Application::Start();

   /* Graphics::MainCamera() = new Camera(Vector3(0, 0, 2), 70.0f, (float)Graphics::GetMainWindow().width / (float)Graphics::GetMainWindow().height, 0.1f, 1000.0f);
    Graphics::MainCamera()->objectClassName = "Main Camera";

    RendererCore::SetUp();*/

    //STARTUP COMPONENTS INSTANTIATE HERE!!!
    //======================================================================================

    //You can instantiate more draw calls with this
    /*for (int i = 0; i < 9; i++)
    {
        DrawCall& d = SceneManager::MainScene().CreateDrawCall();
        d.sort = SceneManager::MainScene().MainDrawCall().sort;
        d.type = SceneManager::MainScene().MainDrawCall().type;
    }*/

    //Put Textures
    /*Texture t("Resources/button.png", Texture::Type::Default);
    Texture t2("Resources/pixeltest2.png", Texture::Type::Default);
    Texture t3("Resources/pixeltest1.png", Texture::Type::Default);
    Texture t4("Resources/pixeltest4.png", Texture::Type::Default);
    Texture t5("Resources/pixeltest3.png", Texture::Type::Default);
    Texture t6("Resources/p5.png", Texture::Type::Default);
    Texture t7("Resources/p6.png", Texture::Type::Default);
    Texture t9("Resources/Icons.png", Texture::Type::Default);
    Texture t10("Resources/ayo.png", Texture::Type::Default);
    Texture t11("Resources/TOSHIanimations.0006.png", Texture::Type::Default);
    Texture t12("Resources/TOSHIanimations.0007.png", Texture::Type::Default);
    Texture t13("Resources/TOSHIanimations.0011.png", Texture::Type::Default);
    Texture t14("Resources/TOSHIanimations.0012.png", Texture::Type::Default);
    Texture t15("Resources/TOSHIanimations.0014.png", Texture::Type::Default);
    Texture t16("Resources/TOSHIanimations.0018.png", Texture::Type::Default);*/

    //Pack them and upload them to the RAM and GPU. Don't use this in Update yet!!
    //TextureManager::UploadToGPU();

    // MODEL SCENE EXAMPLES
    //===========================================================================

    //Spawn 100k quads in 1 draw call
    /*for (int i = 0; i < 10000; i++)
    {
        Model* model = new Model(Model::Square(), Vector3(i, 0, -i), Vector3(0), Vector3(1));
        models.push_back(model);
        RendererCore::AddModel(*model);
    }*/

    //Spawn 10k quads in "i" draw calls, with random colors
    /*for (int m = 0; m < 1000; m++)
    {
        int x = rand() % 200, y = rand() % 100, z = rand() % 100;
        float r = (rand() % 100) / 100.0f, g = (rand() % 100) / 100.0f, b = (rand() % 100) / 100.0f;
        Model* model = new Model(Model::Square(), Vector3(x - 50, y - 50, z - 50), Vector3(0), Vector3(1));
        model->color = Vector4(r, g, b, 0.2f);
        model->AddScript<Rigidbody>();
        model->AddScript<BoxCollider>();
        RendererCore::AddModel(*model);
    }

    Model* model = new Model(Model::Square(), Vector3(0, -100, 0), Vector3(0, 0, 0), Vector3(1000, 0.5f, 1000));
    model->GetTransform().rotation = Vector3(90, 0, 0);
    model->color = Color::white();
    model->AddScript<BoxCollider>();
    RendererCore::AddModel(*model);*/

    //SceneTest::Init();
    //SampleScene::Init();

    //Spawn 2 quads with textures
    /*Model* model = new Model(Model::Square(), Vector3(0, 0, 0), Vector3(0), Vector3(1, 1, 1));*/

    ////model->uitransform = UITransform(UITransform::UIPosition::BOTTOM_CENTER, Vector2(800, 600));
    //model->uitransform.position = Vector2(-100, 0);
    //model->uitransform.size = Vector2(300, 100);
    //model->uitransform.anchors.min = Vector2(1, 0.5);
    //model->uitransform.anchors.max = Vector2(1, 0.5);
    ///*model->uitransform.top = 0;
    //model->uitransform.left = 0;
    //model->uitransform.bottom = 0;
    //model->uitransform.right = 0;*/

    ////model->uitransform.position = Vector2(0);
    ////model->uitransform.size = Vector2(800, 600);
    /*model->texture = &t;
    models.push_back(model);
    RendererCore::AddModel(*model);

    Model* model2 = new Model(Model::Square(), Vector3(1, 0, 0), Vector3(0), Vector3(1));
    model2->texture = &t10;
    models.push_back(model2);
    RendererCore::AddModel(*model2);

    ImGUIElement* window = new ImGUIElement(ImGUIElement::GUIType::Window, "Editor");
    ImGUIElement* text = new ImGUIElement(ImGUIElement::GUIType::Text, *window, "Hey the Editor UI is working now");
    RendererCore::AddImGUIElement(*window, SceneManager::MainScene()._drawCalls[1]);*/

    //d.AddMyselfToHierarchy();

    //START AND UPDATE
    //===========================================================

    //RendererCore::Start();

    //Application::SetEditor();

    //SceneSaveAndLoad::StartSceneSave(&SceneManager::MainScene());

    //SceneTest::Init();

    //Editor::Begin();

    int speed = 4;

    //SceneSaveAndLoad::StartSceneSave(&SceneManager::MainScene());

    bool stopCamera = false;
    bool firstPerson = false;
    int sensitivity = 3;

    //This loop is the "Update" loop, which means the lines inside here will be called each frame.

    while (!Graphics::CanClose())
    {
        Input::UpdateKeyState();

        if (!ImGUIElement::IsMouseOnAnyWindow())
        {
            //Camera controls example
            if (Input::GetKey(GLFW_KEY_C) && speed == 4)
                speed = 30;
            else if (!Input::GetKey(GLFW_KEY_C) && speed == 30)
                speed = 4;

            if (!stopCamera)
            {
                if (Input::GetKey(GLFW_KEY_S))
                    Graphics::MainCamera()->editorPosition -= Graphics::MainCamera()->forward * speed * Graphics::DeltaTime();
                if (Input::GetKey(GLFW_KEY_W))
                    Graphics::MainCamera()->editorPosition += Graphics::MainCamera()->forward * speed * Graphics::DeltaTime();
                if (Input::GetKey(GLFW_KEY_A))
                    Graphics::MainCamera()->editorPosition -= Graphics::MainCamera()->right * speed * Graphics::DeltaTime();
                if (Input::GetKey(GLFW_KEY_D))
                    Graphics::MainCamera()->editorPosition += Graphics::MainCamera()->right * speed * Graphics::DeltaTime();
                if (Input::GetKey(GLFW_KEY_E))
                    Graphics::MainCamera()->editorPosition -= Graphics::MainCamera()->up * speed * Graphics::DeltaTime();
                if (Input::GetKey(GLFW_KEY_Q))
                    Graphics::MainCamera()->editorPosition += Graphics::MainCamera()->up * speed * Graphics::DeltaTime();
            }
        }

        if (firstPerson)
        {
            Graphics::MainCamera()->Mouse();
        }

        //Models movement example
        if (!firstFrame)
        {
            if (Input::GetKey(GLFW_KEY_LEFT_CONTROL))
            {
                if (Input::GetKey(GLFW_KEY_LEFT_ALT))
                {
                    if (Input::GetKeyDown(GLFW_KEY_TAB))
                        stopCamera = !stopCamera;

                    if (Input::GetKeyDown(GLFW_KEY_X))
                    {
                        firstPerson = !firstPerson;

                        if (firstPerson)
                            glfwSetInputMode(Graphics::GetMainWindow().openGLWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                        else
                            glfwSetInputMode(Graphics::GetMainWindow().openGLWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    }

                    if (Input::GetKeyDown(GLFW_KEY_P))
                    {
                        switch (Application::_engineState)
                        {
                        case Application::State::Game:
                            Application::SetEditor();
                            std::cout << "State: Editing..." << std::endl;
                            break;

                        case Application::State::Editor:
                            Application::SetGame();
                            std::cout << "State: Gaming..." << std::endl;
                            break;
                        }
                    }

                    if (Input::GetKeyDown(GLFW_KEY_Y))
                    {
                        if (!Graphics::MainCamera()->editorModeCamera)
                        {
                            Editor::SwitchEditorView();
                        }
                        else
                        {
                            Editor::SwitchGameView();
                        }
                    }

                    //if (Input::GetKeyDown(GLFW_KEY_C))
                    //    CompilationCore::Compile();

                    if (Input::GetKeyDown(GLFW_KEY_S))
                    {
                        DrawCall* main = SceneManager::MainScene().MainDrawCall();
                        main->isMain = true;
                        main->sort = SceneManager::MainScene().MainDrawCall()->sort;
                        main->type = SceneManager::MainScene().MainDrawCall()->type;
                        Hierarchy::allScripts[SceneManager::MainScene().MainDrawCall()->id] = main;
                        std::cout << "Saved!" << std::endl;
                        SceneSaveAndLoad::StartSceneSave(&SceneManager::MainScene());
                    }

                    if (Input::GetKeyDown(GLFW_KEY_R))
                    {
                        std::cout << "Restarting Scene" << std::endl;
                        Application::Start();
                        std::cout << "Scene Restarted!" << std::endl;
                    }
                }
            }

            //models[0]->OnChange();
            if (Input::GetKey(GLFW_KEY_R))
            {
                //models[0]->rotation = Vector3(0);
                //models[0]->color -= Vector4(0, 0, 2.f * Graphics::DeltaTime(), 1.f * Graphics::DeltaTime());
                //models[0]->rotation += Vector3(0, 45.0f * Graphics::DeltaTime(), 45.0f * Graphics::DeltaTime());

                //Hierarchy::allScripts[1]->transform.position -= Vector3(0, 0, 10 * Graphics::DeltaTime());
                ////Hierarchy::allScripts[1]->color -= Vector4(0, 1.f * Graphics::DeltaTime(), 0, 0.09f * Graphics::DeltaTime());
                //Hierarchy::allScripts[1]->transform.rotation += Vector3(45.0f * Graphics::DeltaTime(), 0, 45.0f * Graphics::DeltaTime());

                /*models[2]->position -= Vector3(0, 0, 20 * Graphics::DeltaTime());
                models[2]->color -= Vector4(1.f * Graphics::DeltaTime(), 0, 0, 0.09f * Graphics::DeltaTime());
                models[2]->rotation += Vector3(0, 0, 45.0f * Graphics::DeltaTime());

                models[7]->position -= Vector3(0, 0, 5 * Graphics::DeltaTime());
                models[7]->color -= Vector4(0, 0, 1.f * Graphics::DeltaTime(), 0.09f * Graphics::DeltaTime());
                models[7]->rotation += Vector3(0, 45.0f * Graphics::DeltaTime(), 45.0f * Graphics::DeltaTime());

                models[8]->position -= Vector3(0, 0, 10 * Graphics::DeltaTime());
                models[8]->color -= Vector4(0, 1.f * Graphics::DeltaTime(), 0, 0.09f * Graphics::DeltaTime());
                models[8]->rotation += Vector3(45.0f * Graphics::DeltaTime(), 0, 45.0f * Graphics::DeltaTime());

                models[9]->position -= Vector3(0, 0, 20 * Graphics::DeltaTime());
                models[9]->color -= Vector4(1.f * Graphics::DeltaTime(), 0, 0, 0.09f * Graphics::DeltaTime());
                models[9]->rotation += Vector3(0, 0, 45.0f * Graphics::DeltaTime());
                */
            }
        }

        Hierarchy::UpdateScripts();

        RendererCore::Render();

        Graphics::Update();

        Broadcast::Update();

        firstFrame = false;
    }

    Graphics::Exit();
    exit(0);
    return 0;
}
