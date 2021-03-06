#ifndef ABSTRACT_RENDERER_H
#define ABSTRACT_RENDERER_H

class FontBuilder;
class IFontShaderManager;

#include <vector>
#include <list>
#include <unordered_set>
#include <functional>
#include <shared_mutex>
#include <algorithm>

#include "FontStructures.h"
#include "Externalncludes.h"


class AbstractRenderer
{
public:
	enum class TextAlign {ALIGN_LEFT, ALIGN_CENTER};
	enum class TextAnchor { LEFT_TOP, CENTER, LEFT_DOWN };
	enum class TextType { TEXT, CAPTION };
	enum class AxisYOrigin { TOP, DOWN };

    struct Vertex
    {
        float x, y;
        float u, v;
    };

	struct Color
    {
        float r, g, b, a;
        bool IsSame(const Color & c) const noexcept 
		{
            return (c.r == r) && (c.g == g) &&
				   (c.b == b) && (c.a == a); 
		}
    };
	
	struct RenderParams
	{
		Color color;
		float scale = 1.0f;
	};

	static const Color DEFAULT_COLOR;
	static const RenderParams DEFAULT_PARAMS;

	static std::vector<std::string> GetFontsInDirectory(const std::string & fontDir);

	AbstractRenderer(const FontBuilderSettings& fs, const RenderSettings& r, int glVersion);
    AbstractRenderer(const FontBuilderSettings& fs, const RenderSettings& r, int glVersion,
                     const char * vSource, const char * pSource, std::shared_ptr<IFontShaderManager> sm);
   
	AbstractRenderer(std::shared_ptr<FontBuilder> fb, const RenderSettings& r, int glVersion);
	AbstractRenderer(std::shared_ptr<FontBuilder> fb, const RenderSettings& r, int glVersion,
		const char* vSource, const char* pSource, std::shared_ptr<IFontShaderManager> sm);


	virtual ~AbstractRenderer();

	std::shared_ptr<FontBuilder> GetFontBuilder();
	void SetCanvasSize(int w, int h);
	void SetFontTextureLinearFiler(bool val);
	void SetAxisYOrigin(AxisYOrigin axisY);
	void SetCaption(const UnicodeString & mark, int offsetInPixels);
	void SetCaptionOffset(int offsetInPixels);

	int GetCanvasWidth() const;
	int GetCanvasHeight() const;

	std::shared_ptr<IFontShaderManager> GetShaderManager() const;

	void SwapCanvasWidthHeight();

	void Clear();

	void SetEnabled(bool val);
	bool IsEnabled() const;

	void Render();
    void Render(std::function<void(GLuint)> preDrawCallback, std::function<void()> postDrawCallback);
	

	
protected:
	        
	struct CaptionInfo
	{
		UnicodeString mark;
		int offset;

	};

	struct AABB
	{
		float minX;
		float maxX;

		float minY;
		float maxY;
		

		AABB() noexcept : 
			minX(static_cast<float>(std::numeric_limits<int>::max())),
			minY(static_cast<float>(std::numeric_limits<int>::max())),
			maxX(static_cast<float>(std::numeric_limits<int>::min())),
			maxY(static_cast<float>(std::numeric_limits<int>::min()))
		{}

		bool IsEmpty() noexcept
		{
			return (minX == static_cast<float>(std::numeric_limits<int>::max()));
		}

		float GetWidth() const noexcept
		{
			return maxX - minX;
		}

		float GetHeight() const noexcept
		{
			return maxY - minY;
		}

		void GetCenter(float & x, float & y) const noexcept
		{
			x = minX + this->GetWidth() * 0.5f;
			y = minY + this->GetHeight() * 0.5f;
		}

		void Update(float x, float y, float w, float h) noexcept
		{
			if (x < minX) minX = x;
			if (y < minY) minY = y;
			if (x + w > maxX) maxX = x + w;
			if (y + h > maxY) maxY = y + h;
		}

		void UnionWithOffset(const AABB & b, float xOffset) noexcept
		{
			minX = std::min(minX, b.minX + xOffset);
			minY = std::min(minY, b.minY);

			maxX = std::max(maxX, b.maxX + xOffset);
			maxY = std::max(maxY, b.maxY);
		}
		

	};
	
	struct Shader
	{
        GLuint program;

		const char * vSource;
		const char * pSource;
        bool isDefault;        
	};



    std::shared_ptr<IFontShaderManager> sm;
    RenderSettings rs;
    
	std::shared_ptr<FontBuilder> fb;
	
	CaptionInfo ci;

	AxisYOrigin axisYOrigin;

    int quadsCount;
	std::vector<float> geom;
	
	float psW; //pixel size in width
	float psH; //pixel size in height

	float tW; //pixel size in width
	float tH; //pixel size in height


	bool strChanged;
#ifdef THREAD_SAFETY
	std::shared_timed_mutex m;
#endif

	GLuint vbo;
	GLuint vao;
	GLuint fontTex;
	Shader shader;
	int glVersion;
	
	bool renderEnabled;

	virtual bool GenerateGeometry() = 0;
	 
	void InitGL();
	
	void CreateVAO();
	GLuint CompileGLSLShader(GLenum target, const char* shader);
	GLuint LinkGLSLProgram(GLuint vertexShader, GLuint fragmentShader);

    void AddQuad(const GlyphInfo & gi, float x, float y, const RenderParams & rp);
	void FillTexture();
	void FillVB();
};

#endif
