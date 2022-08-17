#pragma once

namespace vre::Renderer
{

	class RendererObject
	{
	public:
		virtual void Create() {};
		virtual void Destroy() = 0;
	};

}