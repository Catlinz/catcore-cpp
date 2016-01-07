#ifndef CX_ENGINE_CATSCRATCH_H
#define CX_ENGINE_CATSCRATCH_H

/**
 * @copyright Copyright Catlin Zilinski, 2016.  All rights reserved.
 *
 * @file CatScratch.h
 * The main CatScratch game engine code.
 *
 * @author Catlin Zilinski
 * @date Jan 7, 2016
 */

#include "core/Cx.h"
#include "engine/events/CxInputQueue.h"
#include "engine/events/CxInputHandlers.h"

namespace cat {

	/**
	 * @class CatScratchEngine CatScratch.h "engine/CatScratch.h"
	 * 
	 * The main core of the Cat Scratch engine.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Jan 7, 2016
	 */
	class CatScratchEngine {
	  public:
		/** @brief Methods to remove the input handler from the top of the stack and return it. */
		CxInputHandler * popKeyPressInputHandler();
		CxInputHandler * popKeyCharInputHandler();

		CxInputHandler * popMouseClickInputHandler();
		CxInputHandler * popScrollInputHandler();
		CxInputHandler * popMouseMoveInputHandler();
		CxInputHandler * popMouseEnterInputHandler();
		CxInputHandler * popMouseLeaveInputHandler();
		
		CxInputHandler * popResizeInputHandler();

		/** @brief Process all the available input events. */
		void processInput();

		/** @brief Methods to add input handlers to the stacks*/
		void registerKeyPressInputHandler(CxInputHandler *in_h);
		void registerKeyCharInputHandler(CxInputHandler *in_h);

		void registerMouseClickInputHandler(CxInputHandler *in_h);
		void registerScrollInputHandler(CxInputHandler *in_h);
		void registerMouseMoveInputHandler(CxInputHandler *in_h);
		void registerMouseEnterInputHandler(CxInputHandler *in_h);
		void registerMouseLeaveInputHandler(CxInputHandler *in_h);
		
		void registerResizeInputHandler(CxInputHandler *in_h);



		/** @brief Create a new instance of the engine based on current config. */
		static CatScratchEngine * createInstance();

		/** 
		 * @brief Initialise all of the systems the engine needs.
		 * This method initializes all of the required things and 
		 * then creates a new instance of the engine based on the current
		 * loaded configuration values.
		 */
		static CxBool initializeEngine();

	  protected:
		/** @brief The keyboard that we are using for input. */
		CxKeyboard *mp_keyboard;
		
		CxInputQueue m_inputQ;
		
		/* The event handler stacks */
		CxInputHandler * mp_keyPress;
		CxInputHandler * mp_keyChar;
		
		CxInputHandler * mp_mouseClick;
		CxInputHandler * mp_scroll;
		CxInputHandler * mp_mouseMove;
		CxInputHandler * mp_mouseEnter;
		CxInputHandler * mp_mouseLeave;

		CxInputHandler * mp_resize;

		/** @brief Create a new instance. */
	   CatScratchEngine();

		/** @brief Initialise the input */
		void initializeInput();
  
	};

	extern CatScratchEngine *CatScratch;
	
} // namespace cat


#endif // CX_ENGINE_CATSCRATCH_H
