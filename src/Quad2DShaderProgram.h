#include <Magnum/GL/Shader.h>
#include <Magnum/GL/AbstractShaderProgram.h>

namespace Magnum
{
    namespace Examples
    {
        class Quad2DShaderProgram : public GL::AbstractShaderProgram
        {
        public:
            typedef GL::Attribute<0, Vector3> VertexPosition;
            Quad2DShaderProgram();
            Quad2DShaderProgram &setNumberOfOuter(const int number);
            Quad2DShaderProgram &setNumberOfInner(const int number);
            Quad2DShaderProgram &setLineColor(const Color3 &color);
            Quad2DShaderProgram &setQuadColor(const Color3 &color);
            Quad2DShaderProgram &setViewportMatrix(const Matrix4 &matrix);
            Quad2DShaderProgram &setTransformationProjectionMatrix(const Matrix4 &matrix);
            Quad2DShaderProgram &setLineWidth(const float number);

        private:
            Int m_lineColor, m_lineWidth, m_quadColor, m_transformationProjectionMatrix, m_viewportMatrix, m_outer, m_inner;
        };
    } // namespace Examples
} // namespace Magnum