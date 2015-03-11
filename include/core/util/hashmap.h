#ifndef CAT_CORE_UTIL_HASHMAP_H
#define CAT_CORE_UTIL_HASHMAP_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * hashmap.h: Contains the singleton Manager class for the ConfigManager.
 *
 * Author: Catlin Zilinski
 * Date: September 17, 2013
 */

#include <
namespace Cat {

	template<Class Key, Class T>
	class HashMap {
		public:
			HashMap();

			void set(Key &key, T value);
			Key& find(const Key &key);



		private: 
			std::unordered_map<Key, T>	backing_;
	};

} // namespace Cat

#endif // CAT_CORE_UTIL_HASHMAP_H

