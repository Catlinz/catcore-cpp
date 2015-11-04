#ifndef CAT_CORE_THREADING_TASKQUEUENODE_H
#define CAT_CORE_THREADING_TASKQUEUENODE_H

/**
 * @copyright Copyright Catlin Zilinski, 2013.  All rights reserved.
 *
 * @file taskqueuenode.h
 * @brief Contains the node to hold a Queued Task.
 *
 * @author Catlin Zilinski
 * @date Mar 14, 2014
 */
#include <cstring>
#include "core/corelib.h"
#include "core/threading/task.h"

namespace Cat {

	/**
	 * @class TaskQueueNode taskqueuenode.h "core/threading/taskqueuenode.h"
	 * @brief A simple Node to accomadate a list of Tasks.
	 *
	 * @version 1
	 * @since Mar 14, 2014
	 * @author Catlin Zilinski
	 */
	class TaskQueueNode {
	  public:
		TaskQueueNode* prev;
		TaskQueueNode* next;
		TaskPtr task;
		
		TaskQueueNode() : prev(NIL), next(NIL) {}
		
		void alloc(TaskQueueNode* root, const TaskPtr& pTask) {
			detach();
			attach(root);			
			task = pTask;
		}

		void dealloc(TaskQueueNode* root) {
			detach();
			attach(root);
			task.setNull();
		}

		void initAsRoot() {
			prev = next = this;
		}		

		void realloc(TaskQueueNode* root) {
			detach();
			attach(root);
		}

		inline void detach() {
			next->prev = prev;
		   prev->next = next;
		}
		
		inline void attach(TaskQueueNode* root) {
			root->prev->next = this;			
			prev = root->prev;
			next = root;
			root->prev = this;
		}

		inline void init(TaskQueueNode* root) {
			root->prev->next = this;
			prev = root->prev;
			next = root;
			root->prev = this;
		}		
	};

} // namespace Cat

#endif // CAT_CORE_THREADING_TASKQUEUENODE_H
