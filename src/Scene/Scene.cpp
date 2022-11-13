#include "pch.h"


Scene::Scene()
{
	resourceManager = ResourceManager::GetInstance();
	objectFactory = ObjectFactory::GetInstance();
	physicsManager = PhysicsManager::GetInstance();
}

Scene::~Scene()
{
}

void Scene::LoadResources(std::string filename) 
{
	TraceMessage("Scene::LoadResources");

	FILE* fp;
	rapidjson::Document doc;
	fopen_s(&fp, filename.c_str(), "rb");
	if (!fp)
	{
		std::string str = "File not found: " + filename;
		TraceMessage(str.c_str());
		exit(EXIT_FAILURE);
	}
	char readBuffer[8192];
	rapidjson::FileReadStream inputStream(fp, readBuffer, sizeof(readBuffer));
	doc.ParseStream(inputStream);
	//
	std::string shadersStr = doc.FindMember(JSONConstants::SHADERS)->value.GetString();
	resourceManager->LoadAllShaders(shadersStr);
	std::string texturesStr = doc.FindMember(JSONConstants::TEXTURES)->value.GetString();
	resourceManager->LoadAllTextures(texturesStr);
	std::string materialsStr = doc.FindMember(JSONConstants::MATERIALS)->value.GetString();
	resourceManager->LoadAllMaterials(materialsStr);
	//
	fclose(fp);
}

void Scene::LoadGameObjects(std::string filename) 
{
	TraceMessage("Scene::LoadGameObjects");

	objectFactory->CreateAllGameObjects(filename);

	// Factor this out in the future
	GameObject* player = objectFactory->GetGameObjectByName("Player");
	GameObject* playerChild = objectFactory->GetGameObjectByName("PlayerChild");
	GameObject* targetObj1 = objectFactory->GetGameObjectByName("Target1");
	GameObject* targetObj2 = objectFactory->GetGameObjectByName("Target2");
	player->AddComponent<PlayerControlComponent>();
	targetObj1->AddComponent<SelfRotateComponent>();
	targetObj2->AddComponent<SelfRotateComponent>();

	objectFactory->InitializeGameObjects();
	objectFactory->CreateAllDebugCollisionAreas();
}

void Scene::BuildHiearchy(std::string filename)
{
	FILE* fp;
	rapidjson::Document doc;
	fopen_s(&fp, filename.c_str(), "rb");
	if (!fp)
	{
		std::string str = "File not found: " + filename;
		TraceMessage(str.c_str());
		exit(EXIT_FAILURE);
	}
	char readBuffer[8192];
	rapidjson::FileReadStream inputStream(fp, readBuffer, sizeof(readBuffer));
	doc.ParseStream(inputStream);
	//
	const rapidjson::Value& nodes = doc.FindMember(JSONConstants::ROOT)->value;
	for (rapidjson::Value::ConstMemberIterator itr = nodes.MemberBegin(); itr != nodes.MemberEnd(); ++itr)
	{
		recursiveBuildHierachy(itr, &root);
	}
	//
	fclose(fp);

	objectFactory->CheckAllGameObjectsInHierachy();
}
void Scene::recursiveBuildHierachy(rapidjson::Value::ConstMemberIterator nodeIterator, Node* parent)
{
	std::string nodeName = nodeIterator->name.GetString();
	GameObject* childNode = objectFactory->GetGameObjectByName(nodeName);
	parent->AddChild(childNode);
	objectFactory->SetGameObjectInHierachy(nodeName);
	TraceMessage((nodeName + " added to hierachy").c_str());
	const rapidjson::Value& childsOfChildNode = nodeIterator->value;
	if (childsOfChildNode.IsObject())
	{
		for (rapidjson::Value::ConstMemberIterator it = childsOfChildNode.MemberBegin(); it != childsOfChildNode.MemberEnd(); ++it)
		{
			recursiveBuildHierachy(it, childNode);
		}
	}
}

void Scene::SetupCamara(GameWindow* window)
{
	TraceMessage("Scene::SetupCamara");

	Renderer* renderer = Renderer::GetInstance();
	Shader* cameraShader = resourceManager->GetShaderByName("CameraShader");
	Shader* sampleShader = resourceManager->GetShaderByName("SampleShader");
	(*cameraShader).useProgram();
	Camera camera(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), window->Props.Heigth, window->Props.Width);
	camera.SetShaderVP(*cameraShader);
	(*renderer).UnuseShaderProgram();
	(*sampleShader).useProgram();
	camera.SetShaderVP(*sampleShader);
	(*renderer).UnuseShaderProgram();
}

void Scene::Update()
{
	root.Update();
}

void Scene::LateUpdate()
{
	(*physicsManager).collisionManager.CheckAllCollisions();
	(*physicsManager).Integrate();
	objectFactory->DeferredDeleteGameObjects();
}

void Scene::Draw()
{
	root.Draw();
}

void Scene::Destroy()
{
	root.Destroy();
	resourceManager->UnloadAllResources();
}

GameObject* Scene::GetGameObjectByName(std::string name)
{
	return objectFactory->GetGameObjectByName(name);
}