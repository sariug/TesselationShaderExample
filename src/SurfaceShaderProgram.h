#include <Magnum/GL/Shader.h>
#include <Magnum/GL/AbstractShaderProgram.h>

namespace Magnum
{
    namespace Examples
    {
        class SurfaceShaderProgram : public GL::AbstractShaderProgram
        {
        public:
            typedef GL::Attribute<0, Vector3> VertexPosition;
            SurfaceShaderProgram();
            SurfaceShaderProgram &setTessLevel(const int number);
            SurfaceShaderProgram &setLineColor(const Color3 &color);
            SurfaceShaderProgram &setQuadColor(const Color3 &color);
            SurfaceShaderProgram &setViewportMatrix(const Matrix4 &matrix);
            SurfaceShaderProgram &setTransformationProjectionMatrix(const Matrix4 &matrix);
            SurfaceShaderProgram &setLineWidth(const float number);

        private:
            Int m_lineColor, m_lineWidth, m_quadColor, m_transformationProjectionMatrix, m_viewportMatrix, m_outer, m_tessLevel;
        };
    } // namespace Examples
} // namespace Magnum