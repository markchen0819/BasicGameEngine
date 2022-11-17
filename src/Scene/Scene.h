#pragma once
class Scene
{

public:
	Scene();
	Scene(std::string name);
	virtual ~Scene();
	virtual void Init(GameWindow* gw);

	// Call these in order for correct scene loading in your Init()
	void LoadResources(std::string filename);
	void LoadGameObjects(std::string filename);
	void InitializeGameObjects();
	void CreateAllDebugCollisionAreas();
	void BuildHiearchy(std::string filename);
	void SetupCamara(GameWindow* window);

	void Update();
	void Draw();
	void LateUpdate();
	void DeferredDeleteGameObject();

	void Destroy();

	void Pause();
	void Resume();

	std::string Name = "";

private:
	Root root;
	ResourceManager* resourceManager; // aggregation
	ObjectFactory* objectFactory; // aggregation
	PhysicsManager* physicsManager; // aggregation

	bool isRunning = true;
	void recursiveBuildHierachy(rapidjson::Value::ConstMemberIterator nodeIterator, Node* parent);
};

