#include "pch.h"

Node::Node():transform(new Transform()), mesh(new Mesh()) {}
Node::Node(Transform t, Mesh m, Material* mat)
{
	transform = new Transform(t);
	mesh = new Mesh(m);
	material = mat;
}
Node::~Node()
{
	if (transform)
	{
		delete transform;
	}
	if (mesh)
	{
		delete mesh;
	}

	childNodes.clear();
}

void Node::Init()
{
	for (Node* n : childNodes)
	{
		n->Init();
	}
}
void Node::Update()
{
	// Render ability now belongs to RenderComponent
	// Renderer::GetInstance()->UpdateModelMatrix(parent, transform);
	for (Node* n : childNodes)
	{
		n->Update();
	}
}
void Node::Destroy()
{
	std::string s = "Destroy Node: " + Name;
	TraceMessage(s.c_str());
	for (Node* n : childNodes)
	{
		n->Destroy();
		delete n;
		n = nullptr;
	}
	childNodes.clear();
}
//void Node::HandleEvent(void* eventData){ }
void Node::Draw()
{
	// Render ability now belongs to RenderComponent
	// Renderer::GetInstance()->Draw(Name, transform, mesh, material);
	for (Node* n : childNodes)
	{
		n->Draw();
	}
}

Node* Node::GetParent()
{
	return parent;
}
void Node::AddChild(Node* node)
{
	node->parent = this;
	childNodes.push_back(node);
}

void Node::RemoveChild(Node* node)
{
	node->parent = nullptr;
	for (int i = 0; i < childNodes.size(); ++i)
	{
		if (childNodes[i] == node)
		{
			TraceMessage(("Remove child { " + node->GetName() + " } from parent { " + this->GetName()+" }").c_str());
			childNodes.erase(childNodes.begin()+i);
			break;
		}
	}
}

void Node::SetName(std::string name)
{
	Name = name;
}

std::string Node::GetName()
{
	return Name;
}
