#include "Text.h"

#include "driver.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"

#include <fstream>
#include <vector>
#include <algorithm>

namespace
{
	constexpr int kMaxTextVertices = 1024;
	constexpr int kMaxTextIndices = kMaxTextVertices + kMaxTextVertices/2;
}

sal::FontCollectionBuilder::FontCollectionBuilder(uint16_t texWidth, uint16_t texHeight)
	: m_texWidth(texWidth)
	, m_texHeight(texHeight)
{
}

sal::TypefaceHandle sal::FontCollectionBuilder::AddTypeface(const char* filename)
{
	FontCollection::Typeface typeface;
	{
		std::ifstream ifs(filename, std::ios_base::binary|std::ios::ate);
		if (!ifs.is_open())
			throw FontException("Failed to open font file.");

		const auto pos = ifs.tellg();
		typeface.data.resize(pos);
		ifs.seekg(0, std::ios::beg);
		ifs.read(reinterpret_cast<char*>(typeface.data.data()), pos);
	}

	if (!stbtt_InitFont(&typeface.m_info, typeface.data.data(), 0))
	{
		throw FontException("stbtt_InitFont failed.");
	}

	m_typefaces.emplace_back(std::move(typeface));
	return { int(m_typefaces.size()) };
}

sal::FontHandle sal::FontCollectionBuilder::AddFont(TypefaceHandle typeface, uint32_t pixelSize,
	uint32_t firstUnicodeCodepoint, uint32_t numUnicodeChars)
{
	if (typeface.id <= 0 || typeface.id > m_typefaces.size())
		throw FontException("TypefaceHandle is invalid.");

	auto& tf = m_typefaces[typeface.id-1];

	if (!m_contextInitialised)
	{
		m_atlas.resize(m_texWidth*m_texHeight, 0);

		if (!stbtt_PackBegin(&m_pc, m_atlas.data(),
			m_texWidth, m_texHeight, m_texWidth, 1, nullptr))
		{
			throw FontException("stbtt_PackBegin failed.");
		}
		m_contextInitialised = true;
	}

	FontCollection::Font font;
	font.typeface = typeface;
	font.size = pixelSize;
	font.firstCharIndex = firstUnicodeCodepoint;
	font.scale = stbtt_ScaleForPixelHeight(&tf.m_info, float(pixelSize));
	font.m_chars.resize(numUnicodeChars);

	if (!stbtt_PackFontRange(&m_pc, tf.data.data(), 0, float(pixelSize),
		int(firstUnicodeCodepoint), int(numUnicodeChars), font.m_chars.data()))
	{
		throw FontException("stbtt_PackFontRange failed.");
	}

	m_fonts.push_back(std::move(font));

	return { int(m_fonts.size()) };
}

std::unique_ptr<sal::FontCollection> sal::FontCollectionBuilder::Generate()
{
	FontCollection fontCollection;

	{
		sg_image_desc desc = {};
		desc.width = m_texWidth;
		desc.height = m_texHeight;
		desc.pixel_format = SG_PIXELFORMAT_L8;
		desc.content.subimage[0][0].ptr = m_atlas.data();
		desc.content.subimage[0][0].size = int(m_atlas.size());
		desc.label = "font atlas texture";
		fontCollection.m_texture = sg_make_image(&desc);
	}

	if (!bool(fontCollection.m_texture))
	{
		throw FontException("Couldn't create font texture");
	}

	fontCollection.m_typefaces = std::move(m_typefaces);
	fontCollection.m_fonts = std::move(m_fonts);

	// a shader for a non-textured cube, rendered in the offscreen pass
	sg_shader font_shd;
	{
		sg_shader_desc desc = {};
		desc.vs.uniform_blocks[0].size = sizeof(glm::mat4x4);
		desc.vs.uniform_blocks[0].uniforms[0].name = "mvp";
		desc.vs.uniform_blocks[0].uniforms[0].type = SG_UNIFORMTYPE_MAT4;
		desc.vs.source =
			R"#~#(
			#version 330
			uniform mat4 mvp;
			in vec3 position;
			in vec2 texcoord0;
			in vec4 colour;
			out vec2 uv;
			out vec4 vcolour;

			void main()
			{
				gl_Position = mvp * vec4(position, 1.0);
				uv = texcoord0;
				vcolour = colour;
			}
)#~#";
		desc.fs.source =
			R"#~#(
			#version 330
			uniform sampler2D tex;
			in vec2 uv;
			in vec4 vcolour;
			out vec4 frag_color;

			void main()
			{
				frag_color = vcolour * vec4(1.0, 1.0, 1.0, texture(tex, uv).r);
			}

)#~#";
		desc.fs.images[0].name="tex";
		desc.fs.images[0].type=SG_IMAGETYPE_2D;
		desc.label = "offscreen-shader";
		font_shd = sg_make_shader(&desc);
	}

	{
		sg_pipeline_desc desc = {};
		desc.layout.attrs[0].name = "position";
		desc.layout.attrs[0].sem_name = "POSITION";
		desc.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
		desc.layout.attrs[1].name = "texcoord0";
		desc.layout.attrs[1].sem_name = "TEXCOORD";
		desc.layout.attrs[1].format = SG_VERTEXFORMAT_SHORT2N;
		desc.layout.attrs[2].name = "colour";
		desc.layout.attrs[2].sem_name = "COLOUR";
		desc.layout.attrs[2].format = SG_VERTEXFORMAT_UBYTE4N;

		desc.shader = font_shd;
		desc.index_type = SG_INDEXTYPE_UINT16;
		desc.blend.enabled = true;
		desc.blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
		desc.blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
		desc.blend.color_write_mask = SG_COLORMASK_RGB;
		desc.label = "text-pipeline";
		fontCollection.m_pip = sg_make_pipeline(&desc);
	}

	fontCollection.m_bind = {};

	fontCollection.m_bind.vertex_buffers[0] = sal::NewBuffer()
		.Usage(SG_USAGE_STREAM)
		.Size(kMaxTextVertices * sizeof(FontCollection::FontVertex))
		.Make();

	fontCollection.m_bind.index_buffer = sal::NewBuffer()
		.Type(SG_BUFFERTYPE_INDEXBUFFER)
		.Usage(SG_USAGE_STREAM)
		.Size(kMaxTextIndices * sizeof(uint16_t))
		.Make();

	fontCollection.m_bind.fs_images[0] = fontCollection.m_texture;

	return std::make_unique<FontCollection>(std::move(fontCollection));
}

void sal::FontCollection::DrawString(FontHandle font, float x, float y, glm::vec4 colour, bool useKerning, const char* str)
{
	if (font.id <= 0 || font.id > m_fonts.size())
		throw FontException("Font is invalid.");

	auto& fontEntry = m_fonts[font.id-1];

	stbtt_fontinfo* fontInfo = nullptr;

	if (useKerning)
	{
		auto& typefaceEntry = m_typefaces[fontEntry.typeface.id-1];
		fontInfo = &typefaceEntry.m_info;
	}

	const auto len = strlen(str);

	glm::u8vec4 iColour
	{
		uint8_t(std::round(255.0f * glm::clamp(colour.r, 0.0f, 1.0f))),
		uint8_t(std::round(255.0f * glm::clamp(colour.g, 0.0f, 1.0f))),
		uint8_t(std::round(255.0f * glm::clamp(colour.b, 0.0f, 1.0f))),
		uint8_t(std::round(255.0f * glm::clamp(colour.a, 0.0f, 1.0f))),
	};

	for (int i = 0; i < len; i++)
	{
		_ASSERTE(uint32_t(str[i]) >= fontEntry.firstCharIndex && (str[i]- fontEntry.firstCharIndex) < fontEntry.m_chars.size());

		const auto& cd = fontEntry.m_chars[str[i] - fontEntry.firstCharIndex];
		stbtt_aligned_quad q;
		{
			const auto xp = floor((x + cd.xoff) + 0.5f);
			const auto yp = floor((y + cd.yoff) + 0.5f);
			q.x0 = xp;
			q.y0 = yp;
			q.x1 = xp + cd.xoff2 - cd.xoff;
			q.y1 = yp + cd.yoff2 - cd.yoff;

			x += cd.xadvance;

			if (useKerning)
			{
				if (str[i + 1])
				{
					x += fontEntry.scale * stbtt_GetCodepointKernAdvance(fontInfo, str[i], str[i + 1]);
				}
			}
		}

		const auto idx = uint16_t(m_vertices.size());

		m_vertices.emplace_back(FontVertex{{q.x0, q.y1, 0.0f}, { cd.x0*32, cd.y1*32 }, iColour });
		m_vertices.emplace_back(FontVertex{{q.x1, q.y1, 0.0f}, { cd.x1*32, cd.y1*32 }, iColour });
		m_vertices.emplace_back(FontVertex{{q.x1, q.y0, 0.0f}, { cd.x1*32, cd.y0*32 }, iColour });
		m_vertices.emplace_back(FontVertex{{q.x0, q.y0, 0.0f}, { cd.x0*32, cd.y0*32 }, iColour });

		m_indices.emplace_back(idx);
		m_indices.emplace_back(idx+1);
		m_indices.emplace_back(idx+2);
		m_indices.emplace_back(idx);
		m_indices.emplace_back(idx+2);
		m_indices.emplace_back(idx+3);
	}
}

sal::BBox sal::FontCollection::GetStringBB(FontHandle font, float x, float y, bool useKerning, const char* str) const
{
	sal::BBox bbox{ {x, y}, {x, y} };

	if (font.id <= 0 || font.id > m_fonts.size())
		throw FontException("Font is invalid.");

	auto & fontEntry = m_fonts[font.id - 1];

	const stbtt_fontinfo* fontInfo = nullptr;

	if (useKerning)
	{
		auto& typefaceEntry = m_typefaces[fontEntry.typeface.id - 1];
		fontInfo = &typefaceEntry.m_info;
	}

	const auto len = strlen(str);

	for (int i = 0; i < len; i++)
	{
		_ASSERTE(uint32_t(str[i]) >= fontEntry.firstCharIndex && (str[i] - fontEntry.firstCharIndex) < fontEntry.m_chars.size());

		const auto & cd = fontEntry.m_chars[str[i] - fontEntry.firstCharIndex];
		const auto xp = floor((x + cd.xoff) + 0.5f);
		const auto yp = floor((y + cd.yoff) + 0.5f);

		bbox.min.x = std::min(bbox.min.x, xp);
		bbox.min.y = std::min(bbox.min.y, yp);

		bbox.max.x = std::max(bbox.max.x, xp + cd.xoff2 - cd.xoff);
		bbox.max.y = std::max(bbox.max.y, yp + cd.yoff2 - cd.yoff);

		x += cd.xadvance;

		if (useKerning)
		{
			if (str[i + 1])
			{
				x += fontEntry.scale * stbtt_GetCodepointKernAdvance(fontInfo, str[i], str[i + 1]);
			}
		}
	}

	return bbox;
}

void sal::FontCollection::StartFrame()
{
	m_vertices.clear();
	m_indices.clear();
}

void sal::FontCollection::Render(int width, int height)
{
	const auto ortho = glm::ortho(0.0f, float(width), float(height), 0.0f, -1000.0f, 1000.0f);

	m_bind.vertex_buffer_offsets[0] = sg_append_buffer(
		m_bind.vertex_buffers[0], m_vertices.data(), int(m_vertices.size() * sizeof(FontVertex)));
	m_bind.index_buffer_offset = sg_append_buffer(
		m_bind.index_buffer, m_indices.data(), int(m_indices.size() * sizeof(uint16_t)));
	sg_apply_pipeline(m_pip);
	sg_apply_bindings(&m_bind);
	sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, &ortho, sizeof(ortho));
	sg_draw(0, int(m_indices.size()), 1);
}
