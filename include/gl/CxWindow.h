#ifndef CX_GL_CXWINDOW_H
#define CX_GL_CXWINDOW_H

/**
 * @copyright Copyright Catlin Zilinski, 2016.  All rights reserved.
 *
 * @file CxWindow.h
 * The class to construct and interact with an application window in
 * a cross-platform way.
 *
 * @author Catlin Zilinski
 * @date Jan 1, 2016
 */

#include "gl/CxGL.h"
#include "gl/core/CxWindowInputState.h"

namespace cat {

	/**
	 * @class CxWindow CxWindow.h "gl/CxWindow.h"
	 * 
	 * The CxWindow class allows the creation and management of application
	 * windows in a cross-platform manor.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Jan 1, 2016
	 */
	class CxWindow {
	  public:
		/** @brief Enum of different boolean window properties/States */
		enum StateEnum {
			CxResizeable	= 1 << 0,
			CxDecorated		= 1 << 1,
			CxAutoIconify	= 1 << 2,

			CxFloating		= 1 << 3,
			CxClosed			= 1 << 4,
			CxIconfied     = 1 << 5,

			CxCursorTracked = 1 << 6,
		};
		CX_FLAGS(StateEnum, StateFlags);
		
		/** @brief Create null window. */
		CxWindow();

		/** @brief Create a new window with the specified configuration. */
		CxWindow(const CxWindowConfig &in_config);

		/** @brief Destroy the window and release all the crap it needs. */
		~CxWindow();

		/** @brief Get the position of the cursor in the window. */
		
	  private:
		CxWindowInputState 
		CxDisplayDevice *mp_display;
		CxCursor *mp_cursor;
		void *mp_user;
		
		StateFlags m_state;
		
	};
} // namespace cat

#endif // CX_GL_CXWINDOW_H
