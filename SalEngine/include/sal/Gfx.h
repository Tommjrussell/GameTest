#pragma once

#include "sokol_gfx.h"

namespace sal
{
	template <typename T, void (*D)(T)>
	class Handle
	{
	public:
		Handle() : m_resource{} {}
		Handle(T resource) : m_resource(resource) {}

		Handle(Handle&& a) : m_resource(a.m_resource)
		{
			a.m_resource.id = SG_INVALID_ID;
		}

		explicit operator bool() const
		{
			return m_resource.id != SG_INVALID_ID;
		}

		Handle& operator=(Handle&& a)
		{
			Free();
			m_resource = a.m_resource;
			a.m_resource.id = SG_INVALID_ID;
			return *this;
		}
		~Handle()
		{
			Free();
		}

		operator T() { return m_resource; }

	private:
		void Free()
		{
			if (m_resource.id != SG_INVALID_ID)
			{
				D(m_resource);
			}
		}
	private:
		T m_resource;
	};

	typedef Handle<sg_image, sg_destroy_image> TextureHandle;
	typedef Handle<sg_pipeline, sg_destroy_pipeline> PipelineHandle;
	typedef Handle<sg_buffer, sg_destroy_buffer> BufferHandle;
	typedef Handle<sg_shader, sg_destroy_shader> ShaderHandle;

	struct BBox
	{
		glm::vec2 min;
		glm::vec2 max;

		bool Inside(glm::vec2 p)
		{
			if (p.x < min.x || p.x > max.x)
				return false;
			if (p.y < min.y || p.y > max.y)
				return false;
			return true;
		}
	};

} // namespace sal