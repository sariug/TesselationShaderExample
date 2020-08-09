#include "SurfaceShaderProgram.h"
#include <Corrade/Containers/Reference.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/GL/Version.h>
#include <Magnum/GL/Context.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Math/Matrix4.h>
namespace Magnum
{
    namespace Examples
    {

        SurfaceShaderProgram::SurfaceShaderProgram()
        {
            MAGNUM_ASSERT_GL_VERSION_SUPPORTED(GL::Version::GL400);

            /* Load and compile shaders from compiled-in resource */
            Utility::Resource rs("Shaders-glsl");

            GL::Shader vs{GL::Version::GL400, GL::Shader::Type::Vertex};
            GL::Shader tcs{GL::Version::GL400, GL::Shader::Type::TessellationControl};
            GL::Shader tes{GL::Version::GL400, GL::Shader::Type::TessellationEvaluation};
            GL::Shader gs{GL::Version::GL400, GL::Shader::Type::Geometry};
            GL::Shader fs{GL::Version::GL400, GL::Shader::Type::Fragment};

            vs.addSource(rs.get("ForAll.vs"));
            tcs.addSource(rs.get("SurfaceShader.tcs"));
            tes.addSource(rs.get("SurfaceShader.tes"));
            gs.addSource(rs.get("SurfaceShader.gs"));
            fs.addSource(rs.get("SurfaceShader.fs"));

            CORRADE_INTERNAL_ASSERT_OUTPUT(GL::Shader::compile({vs, tcs, tes, gs, fs}));

            attachShaders({vs, tcs, tes, gs, fs});

            CORRADE_INTERNAL_ASSERT_OUTPUT(link());

            // Retrieve uniforms
            m_lineColor = uniformLocation("lineColor");
            m_lineWidth = uniformLocation("lineWidth");
            m_viewportMatrix = uniformLocation("ViewportMatrix");
            m_tessLevel = uniformLocation("TessLevel");

            m_transformationProjectionMatrix = uniformLocation("MVP");
        } // namespace Examples
        SurfaceShaderProgram &SurfaceShaderProgram::setLineWidth(const float number)
        {
            setUniform(m_lineWidth, number);
            return *this;
        }
        SurfaceShaderProgram &SurfaceShaderProgram::setTessLevel(const int number)
        {
            setUniform(m_tessLevel, number);
            return *this;
        }
        SurfaceShaderProgram &SurfaceShaderProgram::setLineColor(const Color3 &color)
        {
            setUniform(m_lineColor, color);
            return *this;
        }
        SurfaceShaderProgram &SurfaceShaderProgram::setViewportMatrix(const Matrix4 &matrix)
        {
            setUniform(m_viewportMatrix, matrix);
            return *this;
        }
        SurfaceShaderProgram &SurfaceShaderProgram::setTransformationProjectionMatrix(const Matrix4 &matrix)
        {
            setUniform(m_transformationProjectionMatrix, matrix);
            return *this;
        }
    } // namespace Examples
} // namespace Magnum