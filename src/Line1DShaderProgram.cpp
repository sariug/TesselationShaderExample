#include "Line1DShaderProgram.h"
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

        Line1DShaderProgram::Line1DShaderProgram()
        {
            MAGNUM_ASSERT_GL_VERSION_SUPPORTED(GL::Version::GL400);

            /* Load and compile shaders from compiled-in resource */
            Utility::Resource rs("Shaders-glsl");

            GL::Shader vs{GL::Version::GL400, GL::Shader::Type::Vertex};
            GL::Shader tcs{GL::Version::GL400, GL::Shader::Type::TessellationControl};
            GL::Shader tes{GL::Version::GL400, GL::Shader::Type::TessellationEvaluation};
            GL::Shader fs{GL::Version::GL400, GL::Shader::Type::Fragment};

            vs.addSource(rs.get("glsl/ForAll.vs"));
            tcs.addSource(rs.get("glsl/Line1D.tcs"));
            tes.addSource(rs.get("glsl/Line1D.tes"));
            fs.addSource(rs.get("glsl/Line1D.fs"));

            CORRADE_INTERNAL_ASSERT_OUTPUT(GL::Shader::compile({vs, tcs, tes, fs}));

            attachShaders({vs, tcs, tes, fs});

            CORRADE_INTERNAL_ASSERT_OUTPUT(link());

            // Retrieve uniforms
            m_lineColor = uniformLocation("lineColor");
            m_numSegments = uniformLocation("numSegments");
            m_numStrips = uniformLocation("numStrips");
            m_transformationProjectionMatrix = uniformLocation("MVP");
        } // namespace Examples
        Line1DShaderProgram &Line1DShaderProgram::setNumberOfSegments(const int number)
        {
            setUniform(m_numSegments, number);
            return *this;
        }
        Line1DShaderProgram &Line1DShaderProgram::setNumberOfStrips(const int number)
        {
            setUniform(m_numStrips, number);
            return *this;
        }
        Line1DShaderProgram &Line1DShaderProgram::setColor(const Color3& color)
        {
            setUniform(m_lineColor, color);
            return *this;
        }
        Line1DShaderProgram &Line1DShaderProgram::setTransformationProjectionMatrix(const Matrix4& matrix)
        {
            setUniform(m_transformationProjectionMatrix, matrix);
            return *this;
        }

    } // namespace Examples
} // namespace Magnum