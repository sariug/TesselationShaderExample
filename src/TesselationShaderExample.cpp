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
#include <map>
namespace Magnum
{
    using namespace Math::Literals;
    namespace Examples
    {
        typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
        typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;
        class TesselationShaderExample;
        typedef void (TesselationShaderExample::*set_points_method)(void);


        class TesselationShaderExample : public Platform::GlfwApplication
        {
            enum SCHEMA {
                LINE, QUAD, SURFACE
            };

        public:
            explicit TesselationShaderExample(const Arguments &arguments);
        private:
            void drawEvent() override;
            void keyPressEvent(KeyEvent &event) override;

            void set1DLineTesselationControlPoints();
            void set2DQuadTesselationControlPoints();
            void set3DSurfaceTesselationControlPoints();
            void setupMeshandBuffers();
            void renderLine();
            void renderQuad();
            void renderSurface();
            void lineKeys(KeyEvent &event);
            void quadKeys(KeyEvent &event);
            void surfaceKeys(KeyEvent &event);
            Scene3D m_scene;

            // Camera attributes
            Object3D* m_cameraObject;
            SceneGraph::Camera3D* m_camera;

            std::unique_ptr<Line1DShaderProgram> m_LineShader;
            std::unique_ptr<Quad2DShaderProgram> m_QuadShader;
            std::unique_ptr<Surface3DShaderProgram> m_SurfaceShader;

            GL::Buffer m_controlPointsBuffer{ NoCreate };
            GL::Mesh m_geometryMesh{ NoCreate }, m_controlPointsMesh{ NoCreate };
            std::vector<Vector3> m_controlVertices;
            Shaders::VertexColor3D m_controlPointsShaderProgram{ NoCreate };

            SCHEMA m_schema{ LINE };
            std::map<SCHEMA, void(TesselationShaderExample::*)()> m_pointsMap;
            std::map<SCHEMA, void(TesselationShaderExample::*)()>  m_renderMap;
            std::map<SCHEMA, void(TesselationShaderExample::*)(KeyEvent &event)>  m_keyFuncMap;

            void (TesselationShaderExample::*m_initPoints)(void);

            // Tesselation parameters
            const unsigned int m_maxTess = 15;
            const unsigned int m_minTess = 2;
            unsigned int m_tessLine = m_minTess;
            unsigned int m_tessQuadInner= m_minTess;
            unsigned int m_tessQuadOuter= m_minTess;
            unsigned int m_tessSurface = m_minTess;
        };

        TesselationShaderExample::TesselationShaderExample(const Arguments &arguments) : Platform::Application{ arguments,
            Configuration{}
            .setTitle("TesselationShaderExample")
            .setSize({ 800, 600 }) }
        {
            // Map functions 
            m_pointsMap[SCHEMA::LINE] =  &TesselationShaderExample::set1DLineTesselationControlPoints;
            m_pointsMap[SCHEMA::QUAD] =  &TesselationShaderExample::set2DQuadTesselationControlPoints;
            m_pointsMap[SCHEMA::SURFACE] =  &TesselationShaderExample::set3DSurfaceTesselationControlPoints;
            m_renderMap[SCHEMA::LINE] =  &TesselationShaderExample::renderLine;
            m_renderMap[SCHEMA::QUAD] =  &TesselationShaderExample::renderQuad;
            m_renderMap[SCHEMA::SURFACE] =  &TesselationShaderExample::renderSurface;
            m_keyFuncMap[SCHEMA::LINE] = &TesselationShaderExample::lineKeys;
            m_keyFuncMap[SCHEMA::QUAD] = &TesselationShaderExample::quadKeys;
            m_keyFuncMap[SCHEMA::SURFACE] = &TesselationShaderExample::surfaceKeys;

            m_cameraObject = new Object3D(&m_scene);
            m_cameraObject->translate(Vector3::zAxis(1.5f));
            m_camera = new SceneGraph::Camera3D(*m_cameraObject);
            m_camera->setProjectionMatrix(Matrix4::orthographicProjection({ 2.8, 2.1 }, 0.1, 100))
                .setViewport(GL::defaultFramebuffer.viewport().size());

            // initial vertices
            (this->*(m_pointsMap[m_schema]))();
            m_controlPointsShaderProgram = Shaders::VertexColor3D{};
            m_controlPointsShaderProgram.setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());

            GL::Renderer::setClearColor(Color4{ 0.0f, 0.4f, 0.7f, 1.0f });
            GL::Renderer::setPointSize(10);
            GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
        }
        void TesselationShaderExample::drawEvent()
        {

            GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);
            (this->*(m_renderMap[m_schema]))();
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

            // Bezier Surface control point generation
            m_controlVertices.clear();
            const int nx=4, ny = 4;
            const float interval = 1.0/(nx-1);
            for (int i = 0; i < nx; i++)
            {
                for (int j = 0; j < ny; j++)
                {
                    m_controlVertices.push_back({ i * interval, j * interval,  static_cast<float>(dist(mt)) });
                }
            }
            m_controlVertices.shrink_to_fit();
            setupMeshandBuffers();

        }
        void TesselationShaderExample::renderLine() {
            m_LineShader->setNumberOfSegments(m_tessLine*2)
                .setNumberOfStrips(3)
                .setColor(Color3{ 1.0f, 0.0f, 0.0f })
                .setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());

            m_LineShader->draw(m_geometryMesh);
        }
        void TesselationShaderExample::renderQuad() {
            m_QuadShader->setNumberOfInner(m_tessQuadInner)
                .setNumberOfOuter(m_tessQuadOuter)
                .setLineWidth(1.5)
                .setLineColor(Color3{ 1.0f, 0.0f, 0.0f })
                .setQuadColor(Color3{ 1.0f, 1.0f, 1.0f })
                .setViewportMatrix(200 * m_camera->projectionMatrix().inverted())
                .setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());
            m_QuadShader->draw(m_geometryMesh);
        }
        void TesselationShaderExample::renderSurface() {
            m_cameraObject->rotateX(1.0_degf);
            m_SurfaceShader->setTessLevel(m_tessSurface)
                .setLineWidth(1.0f)
                .setLineColor(Color3{ .0f, 0.0f, 0.0f })
                .setViewportMatrix(m_camera->projectionMatrix().inverted() * 200)
                .setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());
            m_SurfaceShader->draw(m_geometryMesh);
            m_controlPointsShaderProgram.setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());
        }
        void TesselationShaderExample::keyPressEvent(KeyEvent &event)
        {
            if (event.key() == KeyEvent::Key::Esc)
                exit(0);

            if (event.key() == KeyEvent::Key::One || event.key() == KeyEvent::Key::Two || event.key() == KeyEvent::Key::Three) {

                if (event.key() == KeyEvent::Key::One)
                    m_schema = LINE;

                if (event.key() == KeyEvent::Key::Two)
                    m_schema = QUAD;

                if (event.key() == KeyEvent::Key::Three)
                    m_schema = SURFACE;

                m_cameraObject->resetTransformation().translate(Vector3::zAxis(1.5f));
                m_controlPointsShaderProgram.setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());

                (this->*(m_pointsMap[m_schema]))();
            }
            (this->*(m_keyFuncMap[m_schema]))(event);
        }
        void TesselationShaderExample::lineKeys(KeyEvent &event) {
            if (event.key() == KeyEvent::Key::Up && m_tessLine<m_maxTess)
                m_tessLine++;
            else if (event.key() == KeyEvent::Key::Down && m_tessLine>m_minTess)
                m_tessLine--;
        }
        void TesselationShaderExample::quadKeys(KeyEvent &event) {
            if (event.key() == KeyEvent::Key::Up && m_tessQuadOuter<m_maxTess)
                m_tessQuadOuter++;
            else if (event.key() == KeyEvent::Key::Down && m_tessQuadOuter>m_minTess)
                m_tessQuadOuter--;
            else if (event.key() == KeyEvent::Key::Right && m_tessQuadInner<m_maxTess)
                m_tessQuadInner++;
            else if (event.key() == KeyEvent::Key::Left && m_tessQuadInner>m_minTess)
                m_tessQuadInner--;
        }
        void TesselationShaderExample::surfaceKeys(KeyEvent &event) {
            if (event.key() == KeyEvent::Key::Up && m_tessSurface<m_maxTess)
                m_tessSurface++;
            else if (event.key() == KeyEvent::Key::Down && m_tessSurface>m_minTess)
                m_tessSurface--;
        }
    } // namespace Examples
} // namespace Magnum
MAGNUM_APPLICATION_MAIN(Magnum::Examples::TesselationShaderExample)