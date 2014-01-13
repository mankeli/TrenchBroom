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

#include "CameraTool.h"

#include "Preferences.h"
#include "PreferenceManager.h"
#include "Model/Brush.h"
#include "Model/Entity.h"
#include "Model/HitFilters.h"
#include "View/InputState.h"
#include "View/MapDocument.h"
#include "Renderer/Camera.h"

namespace TrenchBroom {
    namespace View {
        CameraTool::CameraTool(BaseTool* next, MapDocumentWPtr document, ControllerWPtr controller, Renderer::Camera& camera) :
        Tool(next, document, controller),
        m_camera(camera),
        m_orbit(false) {}
        
        void CameraTool::doScroll(const InputState& inputState) {
            if (m_orbit) {
                const Plane3f orbitPlane(m_orbitCenter, m_camera.direction());
                const float maxDistance = std::max(orbitPlane.intersectWithRay(m_camera.viewRay()) - 32.0f, 0.0f);
                const float distance = std::min(inputState.scrollY() * moveSpeed(false), maxDistance);
                m_camera.moveBy(distance * m_camera.direction());
            } else if (move(inputState)) {
                PreferenceManager& prefs = PreferenceManager::instance();
                const Vec3f moveDirection = prefs.get(Preferences::CameraMoveInCursorDir) ? Vec3f(inputState.pickRay().direction) : m_camera.direction();
                const float distance = inputState.scrollY() * moveSpeed(false);
                m_camera.moveBy(distance * moveDirection);
            }
        }

        bool CameraTool::doStartMouseDrag(const InputState& inputState) {
            if (orbit(inputState)) {
                const Model::PickResult::FirstHit firstHit = Model::firstHit(inputState.pickResult(), Model::Brush::BrushHit | Model::Entity::EntityHit, document()->filter(), true);
                if (firstHit.matches) {
                    m_orbit = true;
                    m_orbitCenter = firstHit.hit.hitPoint();
                }
                return true;
            } else if (look(inputState)) {
                return true;
            } else if (pan(inputState)) {
                return true;
            }
            return false;
        }
        
        bool CameraTool::doMouseDrag(const InputState& inputState) {
            if (m_orbit) {
                const float hAngle = inputState.mouseDX() * lookSpeedH();
                const float vAngle = inputState.mouseDY() * lookSpeedV();
                m_camera.orbit(m_orbitCenter, hAngle, vAngle);
                return true;
            } else if (look(inputState)) {
                const float hAngle = inputState.mouseDX() * lookSpeedH();
                const float vAngle = inputState.mouseDY() * lookSpeedV();
                m_camera.rotate(hAngle, vAngle);
                return true;
            } else if (pan(inputState)) {
                PreferenceManager& prefs = PreferenceManager::instance();
                const bool altMove = prefs.get(Preferences::CameraEnableAltMove);
                Vec3f delta;
                if (altMove && inputState.modifierKeysPressed(ModifierKeys::MKAlt)) {
                    delta += inputState.mouseDX() * panSpeedH() * m_camera.right();
                    delta += inputState.mouseDY() * -moveSpeed(altMove) * m_camera.direction();
                } else {
                    delta += inputState.mouseDX() * panSpeedH() * m_camera.right();
                    delta += inputState.mouseDY() * panSpeedV() * m_camera.up();
                }
                m_camera.moveBy(delta);
                return true;
            }
            return false;
        }
        
        void CameraTool::doEndMouseDrag(const InputState& inputState) {
            m_orbit = false;
        }
        
        void CameraTool::doCancelMouseDrag(const InputState& inputState) {
            m_orbit = false;
        }

        bool CameraTool::move(const InputState& inputState) const {
            return (inputState.mouseButtonsPressed(MouseButtons::MBNone) &&
                    inputState.modifierKeysPressed(ModifierKeys::MKNone));
        }

        bool CameraTool::look(const InputState& inputState) const {
            return (inputState.mouseButtonsPressed(MouseButtons::MBRight) &&
                    inputState.modifierKeysPressed(ModifierKeys::MKNone));
        }
        
        bool CameraTool::pan(const InputState& inputState) const {
            return (inputState.mouseButtonsPressed(MouseButtons::MBMiddle) &&
                    (inputState.modifierKeysPressed(ModifierKeys::MKNone) ||
                     inputState.modifierKeysPressed(ModifierKeys::MKAlt)));
        }
        
        bool CameraTool::orbit(const InputState& inputState) const {
            return (inputState.mouseButtonsPressed(MouseButtons::MBRight) &&
                    inputState.modifierKeysPressed(ModifierKeys::MKAlt));
        }

        float CameraTool::lookSpeedH() const {
            PreferenceManager& prefs = PreferenceManager::instance();
            float speed = prefs.get(Preferences::CameraLookSpeed) / -50.0f;
            if (prefs.get(Preferences::CameraLookInvertH))
                speed *= -1.0f;
            return speed;
        }
        
        float CameraTool::lookSpeedV() const {
            PreferenceManager& prefs = PreferenceManager::instance();
            float speed = prefs.get(Preferences::CameraLookSpeed) / -50.0f;
            if (prefs.get(Preferences::CameraLookInvertV))
                speed *= -1.0f;
            return speed;
        }
        
        float CameraTool::panSpeedH() const {
            PreferenceManager& prefs = PreferenceManager::instance();
            float speed = prefs.get(Preferences::CameraPanSpeed);
            if (prefs.get(Preferences::CameraPanInvertH))
                speed *= -1.0f;
            return speed;
        }
        
        float CameraTool::panSpeedV() const {
            PreferenceManager& prefs = PreferenceManager::instance();
            float speed = prefs.get(Preferences::CameraPanSpeed);
            if (prefs.get(Preferences::CameraPanInvertV))
                speed *= -1.0f;
            return speed;
        }
        
        float CameraTool::moveSpeed(const bool altMode) const {
            PreferenceManager& prefs = PreferenceManager::instance();
            float speed = prefs.get(Preferences::CameraMoveSpeed) * 20.0f;
            if (altMode && prefs.get(Preferences::CameraAltMoveInvert))
                speed *= -1.0f;
            return speed;
        }
    }
}
