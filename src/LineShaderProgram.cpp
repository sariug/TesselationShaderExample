#include "LineShaderProgram.h"
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

        LineShaderProgram::LineShaderProgram()
        {
            MAGNUM_ASSERT_GL_VERSION_SUPPORTED(GL::Version::GL400);

            /* Load and compile shaders from compiled-in resource */
            Utility::Resource rs("Shaders-glsl");

            GL::Shader vs{GL::Version::GL400, GL::Shader::Type::Vertex};
            GL::Shader tcs{GL::Version::GL400, GL::Shader::Type::TessellationControl};
            GL::Shader tes{GL::Version::GL400, GL::Shader::Type::TessellationEvaluation};
            GL::Shader fs{GL::Version::GL400, GL::Shader::Type::Fragment};

            vs.addSource(rs.get("LineShader.vs"));
            tcs.addSource(rs.get("LineShader.tcs"));
            tes.addSource(rs.get("LineShader.tes"));
            fs.addSource(rs.get("LineShader.fs"));

            CORRADE_INTERNAL_ASSERT_OUTPUT(GL::Shader::compile({vs, tcs, tes, fs}));

            attachShaders({vs, tcs, tes, fs});

            CORRADE_INTERNAL_ASSERT_OUTPUT(link());

            // Retrieve uniforms
            m_lineColor = uniformLocation("lineColor");
            m_numSegments = uniformLocation("numSegments");
            m_numStrips = uniformLocation("numStrips");
            m_transformationProjectionMatrix = uniformLocation("MVP");
        } // namespace Examples
        LineShaderProgram &LineShaderProgram::setNumberOfSegments(const int number)
        {
            setUniform(m_numSegments, number);
            return *this;
        }
        LineShaderProgram &LineShaderProgram::setNumberOfStrips(const int number)
        {
            setUniform(m_numStrips, number);
            return *this;
        }
        LineShaderProgram &LineShaderProgram::setColor(const Color3& color)
        {
            setUniform(m_lineColor, color);
            return *this;
        }
        LineShaderProgram &LineShaderProgram::setTransformationProjectionMatrix(const Matrix4& matrix)
        {
            setUniform(m_transformationProjectionMatrix, matrix);
            return *this;
        }

    } // namespace Examples
} // namespace Magnum