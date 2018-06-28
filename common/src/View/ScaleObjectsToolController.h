#/*
Copyright (C) 2010-2017 Kristian Duske

This file is part of TrenchBroom.

TrenchBroom is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

TrenchBroom is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TrenchBroom_ScaleObjectsToolController
#define TrenchBroom_ScaleObjectsToolController

#include "Model/Hit.h"
#include "Renderer/EdgeRenderer.h"
#include "View/ToolController.h"
#include "View/ViewTypes.h"

namespace TrenchBroom {
    namespace Model {
        class PickResult;
    }
    
    namespace Renderer {
        class RenderBatch;
        class RenderContext;
    }
    
    namespace View {
        class InputState;
        class ScaleObjectsTool;
        
        class ScaleObjectsToolController : public ToolControllerBase<PickingPolicy, KeyPolicy, MousePolicy, RestrictedDragPolicy, RenderPolicy, NoDropPolicy> {
        protected:
            ScaleObjectsTool* m_tool;
        private:
            Vec3 m_debugInitialPoint;
            BBox3 m_bboxAtDragStart;
            Model::Hit m_dragStartHit; // contains the drag type (face/edge/corner)
            MapDocumentWPtr m_document;

            Vec3 m_lastDragDebug;
            Vec3 m_currentDragDebug;
            Line3 m_handleLineDebug;

            Vec3 m_dragCumulativeDelta;


        public:
            explicit ScaleObjectsToolController(ScaleObjectsTool* tool, MapDocumentWPtr document);
            ~ScaleObjectsToolController() override;
        private:
            Tool* doGetTool() override;

            void doPick(const InputState& inputState, Model::PickResult& pickResult) override;
            virtual void doPick(const Ray3 &pickRay, const Renderer::Camera &camera, Model::PickResult &pickResult) = 0;
            
            void doModifierKeyChange(const InputState& inputState) override;
            
            void doMouseMove(const InputState& inputState) override;
            
//            bool doStartMouseDrag(const InputState& inputState) override;
//            bool doMouseDrag(const InputState& inputState) override;
//            void doEndMouseDrag(const InputState& inputState) override;
//            void doCancelMouseDrag() override;

            // RestrictedDragPolicy
            DragInfo doStartDrag(const InputState& inputState) override;
            DragResult doDrag(const InputState& inputState, const Vec3& lastHandlePosition, const Vec3& nextHandlePosition) override;
            void doEndDrag(const InputState& inputState) override;
            void doCancelDrag() override;

            void doSetRenderOptions(const InputState& inputState, Renderer::RenderContext& renderContext) const override;

            void doRender(const InputState& inputState, Renderer::RenderContext& renderContext, Renderer::RenderBatch& renderBatch) override;
            void renderShear(const InputState& inputState, Renderer::RenderContext& renderContext, Renderer::RenderBatch& renderBatch);
            void renderScale(const InputState& inputState, Renderer::RenderContext& renderContext, Renderer::RenderBatch& renderBatch);

            bool doCancel() override;

        protected:
            bool handleInput(const InputState& inputState) const;

        private:
            bool updateResize(const InputState& inputState);
        };
        
        class ScaleObjectsToolController2D : public ScaleObjectsToolController {
        public:
            explicit ScaleObjectsToolController2D(ScaleObjectsTool* tool, MapDocumentWPtr document);
        private:
            void doPick(const Ray3 &pickRay, const Renderer::Camera &camera, Model::PickResult &pickResult) override;
        };
        
        class ScaleObjectsToolController3D : public ScaleObjectsToolController {
        public:
            explicit ScaleObjectsToolController3D(ScaleObjectsTool* tool, MapDocumentWPtr document);
        private:
            void doPick(const Ray3 &pickRay, const Renderer::Camera &camera, Model::PickResult &pickResult) override;
        };
    }
}

#endif /* defined(TrenchBroom_ScaleObjectsToolController) */

