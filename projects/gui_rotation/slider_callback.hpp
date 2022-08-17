#pragma once
#include <project_relevant_headers.hpp>
#include <QObject>

class SliderCallback : public QObject
{
	Q_OBJECT

public:

public slots:
	void RotateX(int x)
	{
		m_x = x;
		rotate();
	}

	void RotateY(int y)
	{
		m_y = y;
		rotate();
	}

	void RotateZ(int z)
	{
		m_z = z;
		rotate();
	}

	void RotateModelX(int x)
	{
		m_modelX = x;
		rotateModel();
	}

	void RotateModelY(int y)
	{
		m_modelY = y;
		rotateModel();
	}

	void RotateModelZ(int z)
	{
		m_modelZ = z;
		rotateModel();
	}

	void SetNode(std::shared_ptr<vre::Node> node) { m_node = node; }
	void SetNode2(std::shared_ptr<vre::Node> node) { m_node2 = node; }
	void SetModelTransformation(std::shared_ptr<vre::ModelTransformation> modelTransformation) { m_modelTransformation = modelTransformation; }
private:
	int rotationValue = 0;
	std::shared_ptr<vre::Node> m_node{};
	std::shared_ptr<vre::Node> m_node2{};
	std::shared_ptr<vre::ModelTransformation> m_modelTransformation{};
	int m_x{};
	int m_y{};
	int m_z{};

	int m_modelX{};
	int m_modelY{};
	int m_modelZ{};

	void rotate()
	{
		auto transform = m_node->GetTransformation();
		transform->setToIdentity();
		transform->rotate(m_x, 1.0f, 0.0f);
		transform->rotate(m_y, 0.0f, 1.0f);
		transform->rotate(m_z, 0.0f, 0.0f, 1.0);
		m_node->UpdateTransformation();
	}

	void rotateModel()
	{
		auto transform = new QMatrix4x4();
		// offset of -90 degrees so models are pointing upwards
		transform->rotate(m_modelX -90.0, 1.0f, 0.0f);
		transform->rotate(m_modelY, 0.0f, 1.0f);
		transform->rotate(m_modelZ, 0.0f, 0.0f, 1.0);
		auto transform2 = std::shared_ptr<QMatrix4x4>(transform);
		m_modelTransformation->Set(transform2);
		
		// after updating the mode rotation the final transformation of the drawable has to be
		// updated manually
		m_node->UpdateTransformation();
		m_node2->UpdateTransformation();
	}
};