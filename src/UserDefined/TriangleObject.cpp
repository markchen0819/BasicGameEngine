#include "TriangleObject.h"

TriangleObject::TriangleObject()
{
	SetupMesh();
	SetupTransform();
}

TriangleObject::~TriangleObject()
{
}

void TriangleObject::Draw()
{
	Node::Draw();
}

void TriangleObject::SetupTransform()
{
	transform = new Transform(glm::vec3(-50, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
}

void TriangleObject::SetupMesh()
{
	//Create Mesh
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices = { 0, 1, 2 };
	Vertex v0(glm::vec3(0.0f, 36.60f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	Vertex v1(glm::vec3(50.0f, -50.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f));
	Vertex v2(glm::vec3(-50.0f, -50.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f));
	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	mesh = new Mesh(vertices, indices);
}

void TriangleObject::AttachMaterial(Material* m)
{
	material = m;
}