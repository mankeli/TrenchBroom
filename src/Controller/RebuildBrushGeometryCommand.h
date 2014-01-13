/*
 Copyright (C) 2010-2014 Kristian Duske
 
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

#ifndef __TrenchBroom__RebuildBrushGeometryCommand__
#define __TrenchBroom__RebuildBrushGeometryCommand__

#include "StringUtils.h"
#include "SharedPointer.h"
#include "Controller/Command.h"
#include "Controller/BrushVertexHandleCommand.h"
#include "Model/ModelTypes.h"
#include "Model/Snapshot.h"
#include "View/ViewTypes.h"

namespace TrenchBroom {
    namespace Controller {
        class RebuildBrushGeometryCommand : public BrushVertexHandleCommand {
        public:
            static const CommandType Type;
            typedef std::tr1::shared_ptr<RebuildBrushGeometryCommand> Ptr;
        private:
            typedef std::map<Model::Brush*, Vec3::List> BrushVerticesMap;
            
            View::MapDocumentWPtr m_document;
            Model::BrushList m_brushes;
        public:
            static Ptr rebuildBrushGeometry(View::MapDocumentWPtr document, const Model::BrushList& brushes);
        private:
            RebuildBrushGeometryCommand(View::MapDocumentWPtr document, const Model::BrushList& brushes);
            
            bool doPerformDo();
            
            void doRemoveBrushes(View::VertexHandleManager& manager) const;
            void doAddBrushes(View::VertexHandleManager& manager) const;
            void doSelectNewHandlePositions(View::VertexHandleManager& manager) const;
            void doSelectOldHandlePositions(View::VertexHandleManager& manager) const;
        };
    }
}

#endif /* defined(__TrenchBroom__RebuildBrushGeometryCommand__) */
