//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2016, Image Engine Design Inc. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//      * Redistributions of source code must retain the above
//        copyright notice, this list of conditions and the following
//        disclaimer.
//
//      * Redistributions in binary form must reproduce the above
//        copyright notice, this list of conditions and the following
//        disclaimer in the documentation and/or other materials provided with
//        the distribution.
//
//      * Neither the name of John Haddon nor the names of
//        any other contributors to this software may be used to endorse or
//        promote products derived from this software without specific prior
//        written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////

#ifndef GAFFERSCENEUI_SHADERVIEW_H
#define GAFFERSCENEUI_SHADERVIEW_H

#include "GafferSceneUI/Export.h"
#include "GafferSceneUI/TypeIds.h"

#include "GafferScene/InteractiveRender.h"

#include "GafferImageUI/ImageView.h"

#include "GafferImage/Display.h"

#include <functional>

namespace GafferSceneUI
{

class GAFFERSCENEUI_API ShaderView : public GafferImageUI::ImageView
{

	public :

		ShaderView( const std::string &name = defaultName<ShaderView>() );
		~ShaderView() override;

		GAFFER_NODE_DECLARE_TYPE( GafferSceneUI::ShaderView, ShaderViewTypeId, GafferImageUI::ImageView );

		Gaffer::StringPlug *scenePlug();
		const Gaffer::StringPlug *scenePlug() const;

		// The prefix for the shader currently being viewed.
		std::string shaderPrefix() const;

		// The scene currently being used.
		Gaffer::Node *scene();
		const Gaffer::Node *scene() const;

		typedef boost::signal<void ( ShaderView * )> SceneChangedSignal;
		SceneChangedSignal &sceneChangedSignal();

		void setContext( Gaffer::ContextPtr context ) override;

		typedef std::function<GafferScene::InteractiveRenderPtr ()> RendererCreator;
		static void registerRenderer( const std::string &shaderPrefix, RendererCreator rendererCreator );

		typedef std::function<Gaffer::NodePtr ()> SceneCreator;
		static void registerScene( const std::string &shaderPrefix, const std::string &name, SceneCreator sceneCreator );
		static void registerScene( const std::string &shaderPrefix, const std::string &name, const std::string &referenceFileName );
		static void registeredScenes( const std::string &shaderPrefix, std::vector<std::string> &names );

	private :

		typedef std::pair<std::string, std::string> PrefixAndName;
		typedef std::map<PrefixAndName, Gaffer::NodePtr> Scenes;

		GafferImage::Display *display();
		const GafferImage::Display *display() const;

		void viewportVisibilityChanged();

		void plugSet( Gaffer::Plug *plug );
		void plugDirtied( Gaffer::Plug *plug );
		void sceneRegistrationChanged( const PrefixAndName &prefixAndName );

		void idleUpdate();
		void updateRenderer();
		void updateRendererContext();
		void updateRendererState();
		void updateScene();
		void preRender();

		void driverCreated( IECoreImage::DisplayDriver *driver, const IECore::CompoundData *parameters );

		bool m_framed;
		Gaffer::NodePtr m_imageConverter;

		boost::signals::scoped_connection m_idleConnection;

		GafferScene::InteractiveRenderPtr m_renderer;
		std::string m_rendererShaderPrefix;

		Scenes m_scenes;
		Gaffer::NodePtr m_scene;
		PrefixAndName m_scenePrefixAndName;
		SceneChangedSignal m_sceneChangedSignal;

		static ViewDescription<ShaderView> g_viewDescription;

};

IE_CORE_DECLAREPTR( ShaderView );

} // namespace GafferSceneUI

#endif // GAFFERSCENEUI_SHADERVIEW_H
