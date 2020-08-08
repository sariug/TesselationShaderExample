#include <Magnum/GL/Shader.h>
#include <Magnum/GL/AbstractShaderProgram.h>

namespace Magnum
{
    namespace Examples
    {
        class LineShaderProgram : public GL::AbstractShaderProgram
        {
        public:
            typedef GL::Attribute<0, Vector3> VertexPosition;
            LineShaderProgram();
            LineShaderProgram &setNumberOfSegments(const int number);
            LineShaderProgram &setNumberOfStrips(const int number);
            LineShaderProgram &setColor(const Color3 &color);
            LineShaderProgram &setTransformationProjectionMatrix(const Matrix4 &matrix);

        private:
            Int m_lineColor, m_numSegments, m_numStrips, m_transformationProjectionMatrix;
        };
    } // namespace Examples
} // namespace Magnum