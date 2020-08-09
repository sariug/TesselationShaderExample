#include <Corrade/Containers/ArrayView.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/Platform/GlfwApplication.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/Math/Color.h>
#include <Corrade/Containers/ArrayViewStl.h>

#include "Line1DShaderProgram.h"
#include "Quad2DShaderProgram.h"
#include "Surface3DShaderProgram.h"

#include <memory>
#include <random>

namespace Magnum
{
    using namespace Math::Literals;
    namespace Examples
    {
        typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
        typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;

        class TesselationShaderExample : public Platform::GlfwApplication
        {
        public:
            explicit TesselationShaderExample(const Arguments &arguments);
        private:
            void drawEvent() override;
            void set1DLineTesselationControlPoints();
            void set2DQuadTesselationControlPoints();
            void set3DSurfaceTesselationControlPoints();
            void renderLine();
            void renderQuad();
            void renderSurface();

            void setupMeshandBuffers();
            // Camera attributes
            Object3D* m_cameraObject;
            SceneGraph::Camera3D* m_camera;

            Scene3D m_scene;
            std::unique_ptr<Line1DShaderProgram> m_LineShader;
            std::unique_ptr<Quad2DShaderProgram> m_QuadShader;
            std::unique_ptr<Surface3DShaderProgram> m_SurfaceShader;

            GL::Buffer m_controlPointsBuffer{ NoCreate };
            GL::Mesh m_geometryMesh{ NoCreate }, m_controlPointsMesh;
            std::vector<Vector3> m_controlVertices;
            Shaders::VertexColor3D m_controlPointsShaderProgram{ NoCreate };
        };

        TesselationShaderExample::TesselationShaderExample(const Arguments &arguments) : Platform::Application{ arguments,
            Configuration{}
            .setTitle("TesselationShaderExample")
            .setWindowFlags(Configuration::WindowFlag::Resizable)
            .setSize({ 800, 600 }) }
        {
            m_cameraObject = new Object3D(&m_scene);
            m_cameraObject->translate(Vector3::zAxis(1.5f));

            m_camera = new SceneGraph::Camera3D(*m_cameraObject);
            m_camera->setProjectionMatrix(Matrix4::orthographicProjection({ 2.8, 2.1 }, 0.1, 100))
                .setViewport(GL::defaultFramebuffer.viewport().size());


            // initial vertices
            set2DQuadTesselationControlPoints();
            Debug{}<<m_controlVertices;
            m_controlPointsShaderProgram = Shaders::VertexColor3D{};
            m_controlPointsShaderProgram.setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());
            GL::Renderer::setClearColor(Color4{ 0.0f, 0.1f, 0.4f, 1.0f });
            GL::Renderer::setPointSize(10);
            GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
        }
        void TesselationShaderExample::drawEvent()
        {

            GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);
            renderQuad();
            m_controlPointsShaderProgram.draw(m_controlPointsMesh);
            swapBuffers();
            redraw();
        }
        void TesselationShaderExample::setupMeshandBuffers() {
            m_controlPointsBuffer = GL::Buffer(GL::Buffer::TargetHint::Array);
            m_controlPointsBuffer.setData(m_controlVertices);
            m_geometryMesh = GL::Mesh(GL::MeshPrimitive::Patches);
            m_geometryMesh.setCount(m_controlVertices.size())
                .addVertexBuffer(m_controlPointsBuffer, 0, Surface3DShaderProgram::VertexPosition{});

            m_controlPointsMesh = GL::Mesh(GL::MeshPrimitive::Points);
            m_controlPointsMesh.setCount(m_controlVertices.size())
                .addVertexBuffer(m_controlPointsBuffer, 0, Shaders::VertexColor3D::Position{});

            GL::Renderer::setPatchVertexCount(m_controlVertices.size());
        }
        void TesselationShaderExample::set1DLineTesselationControlPoints() {
            m_LineShader = std::make_unique<Line1DShaderProgram>();
            m_controlVertices.clear();
            m_controlVertices.push_back({ -1.0f, -1.0f, 0.0f });
            m_controlVertices.push_back({ -1.0f, 1.0f, 0.0f });
            m_controlVertices.push_back({ 1.0f, 1.0f, 0.0f });
            setupMeshandBuffers();
        }
        void TesselationShaderExample::set2DQuadTesselationControlPoints() {

            m_QuadShader = std::make_unique<Quad2DShaderProgram>();

            // initial vertices
            m_controlVertices.clear();
            m_controlVertices.push_back({ -1.0f, -1.0f, 0.0f });
            m_controlVertices.push_back({ 1.0f, -1.0f, 0.0f });
            m_controlVertices.push_back({ 1.0f, 1.0f, 0.0f });
            m_controlVertices.push_back({ -1.0f, 1.0f, 0.0f });
            setupMeshandBuffers();
        }
        void TesselationShaderExample::set3DSurfaceTesselationControlPoints() {
            m_SurfaceShader = std::make_unique<Surface3DShaderProgram>();
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> dist(0.0, 1.0);

            auto randPos = []() -> float { return static_cast<float>(rand() % 100) / 100.0f; };
            // Bezier Surface control point generation
            m_controlVertices.clear();
            const int nx=4, ny = 4;
            const float interval = 1.0/(nx-1);
            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    m_controlVertices.push_back({ i * interval, j * interval, dist(mt) });
                }
            }
            m_controlVertices.shrink_to_fit();
            setupMeshandBuffers();

        }
        void TesselationShaderExample::renderLine() {
            m_LineShader->setNumberOfSegments(50)
                .setNumberOfStrips(3)
                .setColor(Color3{ 1.0f, 0.0f, 0.0f })
                .setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());

            m_LineShader->draw(m_geometryMesh);
        }
        void TesselationShaderExample::renderQuad() {
            m_QuadShader->setNumberOfInner(3)
                .setNumberOfOuter(3)
                .setLineWidth(1.5)
                .setLineColor(Color3{ 1.0f, 0.0f, 0.0f })
                .setQuadColor(Color3{ 1.0f, 1.0f, 1.0f })
                .setViewportMatrix(200 * m_camera->projectionMatrix().inverted())
                .setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());
            m_QuadShader->draw(m_geometryMesh);
        }
        void TesselationShaderExample::renderSurface() {
            m_cameraObject->rotateX(1.0_degf);
            m_SurfaceShader->setTessLevel(15)
                .setLineWidth(1.0f)
                .setLineColor(Color3{ .0f, 0.0f, 0.0f })
                .setViewportMatrix(m_camera->projectionMatrix().inverted() * 200)
                .setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());
            m_SurfaceShader->draw(m_geometryMesh);
            m_controlPointsShaderProgram.setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());
        }
    } // namespace Examples
} // namespace Magnum
MAGNUM_APPLICATION_MAIN(Magnum::Examples::TesselationShaderExample)