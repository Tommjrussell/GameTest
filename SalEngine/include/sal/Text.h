#pragma once

#include "sokol_gfx.h"
#include "stb_rect_pack.h"
#include "stb_truetype.h"
#include "glm/glm.hpp"

#include "Gfx.h"

#include <vector>
#include <memory>

template <int ID>
struct SalException : public std::exception
{
	SalException(char const* const message) : std::exception(message) {}
};

namespace sal
{
	typedef SalException<1> FontException;

	struct TypefaceHandle
	{
		int id;
	};

	struct FontHandle
	{
		int id;
	};

	class FontCollection
	{
		friend class FontCollectionBuilder;

		struct FontVertex
		{
			glm::vec3 pos;
			glm::i16vec2 texcoord0;
			glm::u8vec4 colour;
		};

		struct Typeface
		{
			stbtt_fontinfo m_info = {};
			std::vector<uint8_t> data;
		};

		struct Font
		{
			TypefaceHandle typeface;
			uint32_t size;
			uint32_t firstCharIndex;
			float scale;
			std::vector<stbtt_packedchar> m_chars;
		};

		FontCollection() = default;
	public:
		void DrawString(FontHandle font, float x, float y, glm::vec4 colour, bool useKerning, const char* str);
		BBox GetStringBB(FontHandle font, float x, float y, bool useKerning, const char* str) const;
		void StartFrame();
		void Render(int width, int height);

	private:
		std::vector<Typeface> m_typefaces;
		std::vector<Font> m_fonts;

		std::vector<FontVertex> m_vertices;
		std::vector<uint16_t> m_indices;

		TextureHandle m_texture;
		PipelineHandle m_pip;
		sg_bindings m_bind;
	};

	class FontCollectionBuilder
	{
	public:
		explicit FontCollectionBuilder(uint16_t texWidth, uint16_t texHeight);

		TypefaceHandle AddTypeface(const char* filename);
		FontHandle AddFont(TypefaceHandle typeface, uint32_t pixelSize, uint32_t firstUnicodeCodepoint, uint32_t numUnicodeChars);

		std::unique_ptr<FontCollection> Generate();

	private:
		std::vector<FontCollection::Typeface> m_typefaces;
		std::vector<FontCollection::Font> m_fonts;
		std::vector<uint8_t> m_atlas;
		uint16_t m_texWidth;
		uint16_t m_texHeight;
		stbtt_pack_context m_pc;
		bool m_contextInitialised = false;
	};

}