/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
 * \file Layer.h
 * \brief Map layers.
 * \author Lars Ake Vinberg
 */

#ifndef LAYER_H_
#define LAYER_H_

#include <MAUtil/String.h>

#include "MemoryMgr.h"
#include "Broadcaster.h"
#include "MapViewport.h"

namespace MAP
{

	using namespace MAPUtil;
	using namespace MAUtil;
	
	//
	// Forward class declarations
	//
	class Layer;
	class LayerRenderer;

	/**
	 * \brief Inteface for notifications about changes in a map layer.
	 */
	class ILayerListener
	{
	public:
		virtual void contentChanged( Layer* sender ) = 0;
	};

	/**
	 * \brief Representation of a single layer in a map.
	 */
	class Layer : 
		public Broadcaster<ILayerListener>
	{
	public:
		Layer( );

		virtual ~Layer( ) { }

		//
		// Render
		//
		virtual void draw( MapViewport* viewport, const Rect& bounds, MagnificationType magnification, bool isLayerSelected ) = 0;
		//
		// Title property
		//
		const char* getTitle( ) const { return mTitle.c_str( ); }
		void setTitle( const char* title ) { mTitle = title; }
		//
		// Layer item selection and activation
		//
		virtual void selectItem( int index ) = 0;
		virtual void selectNextItem( ) = 0;
		virtual void selectPreviousItem( ) = 0;
		virtual void activateSelectedItem( ) = 0;
		//
		// Bounding box for items
		//
		virtual void getBoundingBox( double& left, double& top, double& right, double& bottom ) = 0;
		virtual LonLat getSelectedItemLocation( ) = 0;
		//
		// Layer enabled flag
		//
		void setEnabled( bool enabled );
		bool getEnabled( ) const { return mEnabled; }
	protected:
		virtual void onContentChanged( );

	private:
		String mTitle;
		bool mEnabled;
	};
}
#endif // LAYER_H_

