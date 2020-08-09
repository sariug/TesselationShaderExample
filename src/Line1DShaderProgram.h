#include <Magnum/GL/Shader.h>
#include <Magnum/GL/AbstractShaderProgram.h>

namespace Magnum
{
    namespace Examples
    {
        class Line1DShaderProgram : public GL::AbstractShaderProgram
        {
        public:
            typedef GL::Attribute<0, Vector3> VertexPosition;
            Line1DShaderProgram();
            Line1DShaderProgram &setNumberOfSegments(const int number);
            Line1DShaderProgram &setNumberOfStrips(const int number);
            Line1DShaderProgram &setColor(const Color3 &color);
            Line1DShaderProgram &setTransformationProjectionMatrix(const Matrix4 &matrix);

        private:
            Int m_lineColor, m_numSegments, m_numStrips, m_transformationProjectionMatrix;
        };
    } // namespace Examples
} // namespace Magnum