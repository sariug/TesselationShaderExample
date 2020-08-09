#include "Quad2DShaderProgram.h"
#include <Corrade/Containers/Reference.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/GL/Version.h>
#include <Magnum/GL/Context.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
namespace Magnum
{
    namespace Examples
    {

        Quad2DShaderProgram::Quad2DShaderProgram()
        {
            MAGNUM_ASSERT_GL_VERSION_SUPPORTED(GL::Version::GL400);

            /* Load and compile shaders from compiled-in resource */
            Utility::Resource rs("Shaders-glsl");

            GL::Shader vs{GL::Version::GL400, GL::Shader::Type::Vertex};
            GL::Shader tcs{GL::Version::GL400, GL::Shader::Type::TessellationControl};
            GL::Shader tes{GL::Version::GL400, GL::Shader::Type::TessellationEvaluation};
            GL::Shader gs{GL::Version::GL400, GL::Shader::Type::Geometry};
            GL::Shader fs{GL::Version::GL400, GL::Shader::Type::Fragment};

            vs.addSource(rs.get("glsl/ForAll.vs"));
            tcs.addSource(rs.get("glsl/Quad2D.tcs"));
            tes.addSource(rs.get("glsl/Quad2D.tes"));
            gs.addSource(rs.get("glsl/Quad2D.gs"));
            fs.addSource(rs.get("glsl/Quad2D.fs"));

            CORRADE_INTERNAL_ASSERT_OUTPUT(GL::Shader::compile({vs, tcs, tes, gs, fs}));

            attachShaders({vs, tcs, tes, gs, fs});

            CORRADE_INTERNAL_ASSERT_OUTPUT(link());

            // Retrieve uniforms
            m_lineColor = uniformLocation("lineColor");
            m_lineWidth = uniformLocation("lineWidth");
            m_quadColor = uniformLocation("quadColor");
            m_viewportMatrix = uniformLocation("ViewportMatrix");
            m_outer = uniformLocation("Outer");
            m_inner = uniformLocation("Inner");

            m_transformationProjectionMatrix = uniformLocation("MVP");
        } // namespace Examples
        Quad2DShaderProgram &Quad2DShaderProgram::setLineWidth(const float number)
        {
            setUniform(m_lineWidth, number);
            return *this;
        }
        Quad2DShaderProgram &Quad2DShaderProgram::setNumberOfOuter(const int number)
        {
            setUniform(m_outer, number);
            return *this;
        }
        Quad2DShaderProgram &Quad2DShaderProgram::setNumberOfInner(const int number)
        {
            setUniform(m_inner, number);
            return *this;
        }
        Quad2DShaderProgram &Quad2DShaderProgram::setLineColor(const Color3& color)
        {
            setUniform(m_lineColor, color);
            return *this;
        }
            Quad2DShaderProgram &Quad2DShaderProgram::setQuadColor(const Color3& color)
        {
            setUniform(m_quadColor, color);
            return *this;
        }
        Quad2DShaderProgram &Quad2DShaderProgram::setViewportMatrix(const Matrix4& matrix)
        {
            setUniform(m_viewportMatrix, matrix);
            return *this;
        }
        Quad2DShaderProgram &Quad2DShaderProgram::setTransformationProjectionMatrix(const Matrix4& matrix)
        {
            setUniform(m_transformationProjectionMatrix, matrix);
            return *this;
        }

    } // namespace Examples
} // namespace Magnum