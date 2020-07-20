#include <Magnum/GL/Shader.h>
#include <Magnum/GL/AbstractShaderProgram.h>

namespace Magnum
{
    namespace Examples
    {
        class CubicBezierShader : public GL::AbstractShaderProgram
        {
        public:
            typedef GL::Attribute<0, Vector2> VertexPosition;
            CubicBezierShader();
            CubicBezierShader &setNumberOfSegments(const int number);
            CubicBezierShader &setNumberOfStrips(const int number);
            CubicBezierShader &setColor(const Color3 &color);
            CubicBezierShader &setTransformationProjectionMatrix(const Matrix4 &matrix);

        private:
            Int m_lineColor, m_numSegments, m_numStrips, m_transformationProjectionMatrix;
        };
    } // namespace Examples
} // namespace Magnum