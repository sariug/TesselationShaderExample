#include <Magnum/GL/Shader.h>
#include <Magnum/GL/AbstractShaderProgram.h>

namespace Magnum
{
    namespace Examples
    {
        class Surface3DShaderProgram : public GL::AbstractShaderProgram
        {
        public:
            typedef GL::Attribute<0, Vector3> VertexPosition;
            Surface3DShaderProgram();
            Surface3DShaderProgram &setTessLevel(const int number);
            Surface3DShaderProgram &setLineColor(const Color3 &color);
            Surface3DShaderProgram &setQuadColor(const Color3 &color);
            Surface3DShaderProgram &setViewportMatrix(const Matrix4 &matrix);
            Surface3DShaderProgram &setTransformationProjectionMatrix(const Matrix4 &matrix);
            Surface3DShaderProgram &setLineWidth(const float number);

        private:
            Int m_lineColor, m_lineWidth, m_quadColor, m_transformationProjectionMatrix, m_viewportMatrix, m_outer, m_tessLevel;
        };
    } // namespace Examples
} // namespace Magnum